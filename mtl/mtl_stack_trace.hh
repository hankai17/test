#ifndef mtl_stack_trace_hh
#define mtl_stack_trace_hh

#define MTL_STACK_TRACE_MAX_LEVELS		100 // The max number of levels in the stack trace

#ifdef __cplusplus
extern "C"
{
#endif
/* dumps the current back trace to stderr */
  void mtl_stack_trace_dump();
#ifdef __cplusplus
}
#endif

#endif                          /* mtl_stack_trace_h */
