#include <cstdio>
#include <cstring>
#include <climits>
#include <cstdlib>
#include <cinttypes>

#include "ts/ts.h"
#include "ts/experimental.h"
//#include "tscore/ink_defs.h"

#define PLUGIN_NAME "crossdomain"

static TSCont global_contp;

struct intercept_state_t {
  TSVConn net_vc;
  TSVIO read_vio;
  TSVIO write_vio;

  TSIOBuffer req_buffer;
  TSIOBuffer resp_buffer;
  TSIOBufferReader resp_reader;

  TSHttpTxn http_txnp;

  int64_t total_bytes;
  int total_items;
  int done;

  bool write_pending;
};

using intercept_state = struct intercept_state_t;

static int
handle_accept(TSCont contp, TSEvent event, TSVConn vc)
{
  TSDebug(PLUGIN_NAME, "handle_accept event: %d", event);
  intercept_state *cstate = (intercept_state *)TSContDataGet(contp);

  if (event == TS_EVENT_NET_ACCEPT) {
    if (cstate) {
      // setup vc, buffers
      cstate->net_vc = vc;

      cstate->req_buffer  = TSIOBufferCreate();
      cstate->resp_buffer = TSIOBufferCreate();
      cstate->resp_reader = TSIOBufferReaderAlloc(cstate->resp_buffer);

      cstate->read_vio = TSVConnRead(cstate->net_vc, contp, cstate->req_buffer, INT64_MAX);
      TSDebug(PLUGIN_NAME, "handle_accept NET_ACCEPT");
    } else {
      TSVConnClose(vc);
      TSContDestroy(contp);
    }
  } else {
    TSDebug(PLUGIN_NAME, "handle_accept NET_ACCEPT_FAIL");
    // net_accept failed
    if (cstate) {
      TSfree(cstate);
    }
    TSContDestroy(contp);
  }

  return 0;
}

static void
cleanup(TSCont contp)
{
  TSDebug(PLUGIN_NAME, "cleanup");
  // shutdown vc and free memory
  intercept_state *cstate = (intercept_state *)TSContDataGet(contp);

  if (cstate) {
    // cancel any pending cache scan actions, since we will be destroying the
    // continuation

    if (cstate->net_vc) {
      TSVConnShutdown(cstate->net_vc, 1, 1);
    }

    if (cstate->req_buffer) {
      TSIOBufferDestroy(cstate->req_buffer);
      cstate->req_buffer = nullptr;
    }

    if (cstate->resp_buffer) {
      TSIOBufferDestroy(cstate->resp_buffer);
      cstate->resp_buffer = nullptr;
    }

    TSVConnClose(cstate->net_vc);
    TSfree(cstate);
  }
  TSContDestroy(contp);
}

static int
handle_io(TSCont contp, TSEvent event, void * /* edata ATS_UNUSED */)
{
  intercept_state *cstate = (intercept_state *)TSContDataGet(contp);

  switch (event) {
  case TS_EVENT_VCONN_READ_READY:
  case TS_EVENT_VCONN_READ_COMPLETE: {
    TSDebug(PLUGIN_NAME, "handle_io read_ready/complete");
    // we don't care about the request, so just shut down the read vc
    TSVConnShutdown(cstate->net_vc, 1, 0);
    // setup the response headers so we are ready to write body
    char hdrs[]         = "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\nContent-length: 6\r\n\r\n123456";
    cstate->total_bytes = TSIOBufferWrite(cstate->resp_buffer, hdrs, sizeof(hdrs) - 1);

    //char head[] = "<h3>Cache Contents:</h3>\n<p><pre>\n";
    //cstate->total_bytes += TSIOBufferWrite(cstate->resp_buffer, head, sizeof(head) - 1);
    cstate->write_vio  = TSVConnWrite(cstate->net_vc, contp, cstate->resp_reader, INT64_MAX);
    //TSVIONBytesSet(cstate->write_vio, cstate->total_bytes);
    //TSVIOReenable(cstate->write_vio);

    return 0;
  } break;
  case TS_EVENT_VCONN_WRITE_READY: {
    TSDebug(PLUGIN_NAME, "handle_io write_ready");
    TSDebug(PLUGIN_NAME, "ndone: %" PRId64 " total_bytes: % " PRId64, TSVIONDoneGet(cstate->write_vio), cstate->total_bytes);
    cstate->write_pending = false;
    // the cache scan handler should call vio reenable when there is
    // available data
    TSVIOReenable(cstate->write_vio);
    return 0;
  } break;
  case TS_EVENT_VCONN_WRITE_COMPLETE: {
    TSDebug(PLUGIN_NAME, "write complete");
    cstate->done = 1;
    cleanup(contp);
  } break;
  case TS_EVENT_VCONN_EOS:
  default: {
    cstate->done = 1;
    cleanup(contp);
  } break;
  }

  return 0;
}

// handler for VConnection and CacheScan events
static int
begin_intercept(TSCont contp, TSEvent event, void *edata)
{
  TSDebug(PLUGIN_NAME, "begin_intercept event: %d", event);

  switch (event) {
  case TS_EVENT_NET_ACCEPT:
  case TS_EVENT_NET_ACCEPT_FAILED:
    return handle_accept(contp, event, (TSVConn)edata);
  case TS_EVENT_VCONN_READ_READY:
  case TS_EVENT_VCONN_READ_COMPLETE:
  case TS_EVENT_VCONN_WRITE_READY:
  case TS_EVENT_VCONN_WRITE_COMPLETE:
  case TS_EVENT_VCONN_EOS:
    return handle_io(contp, event, edata);
  case TS_EVENT_ERROR:
    cleanup(contp);
    return 0;
  default:
    TSError("[%s] Unknown event in begin_intercept: %d", PLUGIN_NAME, event);
    cleanup(contp);
    return 0;
  }
}

static int
deal_request(TSCont contp, TSHttpTxn txnp)
{
  TSMBuffer bufp;
  TSMLoc hdr_loc;
  TSMLoc url_loc;
  TSCont tmp_contp;
  const char *path;
  intercept_state *cstate;
  int path_len;

  TSAssert(contp == global_contp);

  if (TSHttpTxnClientReqGet(txnp, &bufp, &hdr_loc) != TS_SUCCESS) {
    TSError("[%s] Couldn't retrieve client request header", PLUGIN_NAME);
    TSHttpTxnReenable(txnp, TS_EVENT_HTTP_CONTINUE);
    return TS_SUCCESS;
  }

  if (TSHttpHdrUrlGet(bufp, hdr_loc, &url_loc) != TS_SUCCESS) {
    TSError("[%s] Couldn't retrieve request url", PLUGIN_NAME);
    TSHandleMLocRelease(bufp, TS_NULL_MLOC, hdr_loc);
    TSHttpTxnReenable(txnp, TS_EVENT_HTTP_CONTINUE);
    return TS_SUCCESS;
  }

  path = TSUrlPathGet(bufp, url_loc, &path_len);
  if (!path) {
    TSError("[%s] Couldn't retrieve request path", PLUGIN_NAME);
    TSHandleMLocRelease(bufp, hdr_loc, url_loc);
    TSHandleMLocRelease(bufp, TS_NULL_MLOC, hdr_loc);
    TSHttpTxnReenable(txnp, TS_EVENT_HTTP_CONTINUE);
    return TS_SUCCESS;
  }

  //query = TSUrlHttpQueryGet(bufp, url_loc, &query_len);

  if (strstr(path, "crossdomain.xml") != NULL) {
    tmp_contp = TSContCreate(begin_intercept, TSMutexCreate());
    TSHttpTxnIntercept(tmp_contp, txnp);
    cstate = (intercept_state *)TSmalloc(sizeof(intercept_state));
    memset(cstate, 0, sizeof(intercept_state));
    cstate->http_txnp = txnp;

    TSContDataSet(tmp_contp, cstate);
    TSDebug(PLUGIN_NAME, "setup cache intercept");
  } else {
    TSDebug(PLUGIN_NAME, "not a cache iter request");
  }

//Ldone:
  TSHandleMLocRelease(bufp, hdr_loc, url_loc);
  TSHandleMLocRelease(bufp, TS_NULL_MLOC, hdr_loc);
  TSHttpTxnReenable(txnp, TS_EVENT_HTTP_CONTINUE);
  return TS_SUCCESS;
}

static int
intercept_req(TSCont contp, TSEvent event, void *edata)
{
  switch (event) {
  case TS_EVENT_HTTP_READ_REQUEST_HDR:
    return deal_request(contp, (TSHttpTxn)edata);
  default:
    break;
  }
  TSHttpTxnReenable((TSHttpTxn)edata, TS_EVENT_HTTP_CONTINUE);
  return TS_SUCCESS;
}

void
TSPluginInit(int /* argc ATS_UNUSED */, const char * /* argv ATS_UNUSED */ [])
{
  TSPluginRegistrationInfo info;

  info.plugin_name   = PLUGIN_NAME;
  info.vendor_name   = "Apache Software Foundation";
  info.support_email = "dev@trafficserver.apache.org";

  if (TSPluginRegister(&info) == TS_SUCCESS) {
    global_contp = TSContCreate(intercept_req, TSMutexCreate());
    TSHttpHookAdd(TS_HTTP_READ_REQUEST_HDR_HOOK, global_contp);
  } else {
    TSError("[%s] Plugin registration failed", PLUGIN_NAME);
  }
}
