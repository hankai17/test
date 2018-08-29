package core

import (
	"net/http"
	"net/http/httputil"
	"net/url"
	"app/core/config"
	"strconv"
	"strings"
	"crypto/tls"
	"net"
	"log"
)

type Proxy struct {
	Req *http.Request
}

func (p *Proxy) Serve(w http.ResponseWriter) {
	mproxy := matchProxy(p.Req, config.App.Proxys)
	if mproxy == nil {
		w.Write([]byte("service not found"))
		return
	}
	peer := mproxy.Service.GetPeer()
	schema := "http"
	if peer.Schema != "" {
		schema = peer.Schema
	}

	if peer.Port == 0 {
		peer.Port = 80
	}
	remote, err := url.Parse(schema + "://" + peer.Host + ":" + strconv.Itoa(peer.Port))
	log.Println("remote", remote)
	if err != nil {
		panic(err)
	}
	proxy := httputil.NewSingleHostReverseProxy(remote)
	proxy.Transport = &http.Transport{
		TLSClientConfig: &tls.Config{InsecureSkipVerify: true},
		DialTLS:         dialTLS,
	}
	//
	director := proxy.Director
	proxy.Director = func(req *http.Request) {
		director(req)
		req.Host = req.URL.Host
	}
	proxy.ServeHTTP(w, p.Req)
}

func dialTLS(network, addr string) (net.Conn, error) {
	conn, err := net.Dial(network, addr)
	if err != nil {
		return nil, err
	}

	host, _, err := net.SplitHostPort(addr)
	if err != nil {
		return nil, err
	}
	cfg := &tls.Config{ServerName: host}

	tlsConn := tls.Client(conn, cfg)
	if err := tlsConn.Handshake(); err != nil {
		conn.Close()
		return nil, err
	}

	cs := tlsConn.ConnectionState()
	cert := cs.PeerCertificates[0]

	// Verify here
	cert.VerifyHostname(host)
	return tlsConn, nil
}

func matchProxy(r *http.Request, proxys []*config.Proxy) *config.Proxy {
	for _, proxy := range proxys {
		log.Println("proxy ", proxy)
		// 先走host , r.Host解析出来为 host + port 结构
		host := r.Host[: strings.Index(r.Host, ":")]
		log.Println("match host:", host)
		if proxy.Match.Host != "*" && proxy.Match.Host != host {
			continue
		}

		// 完全匹配
		log.Println("match path:", r.URL.Path)
		if proxy.Match.RuleType == "" {
			if r.URL.Path != proxy.Match.Path {
				continue
			}
			return proxy
		}

		// URL 前缀匹配
		if proxy.Match.RuleType == "prefix" {
			// 匹配失败
			if len(proxy.Match.Path) > len(r.URL.Path) || !strings.HasPrefix(r.URL.Path, proxy.Match.Path) {
				continue
			}
			// 匹配成功
			return proxy
		}
	}
	return nil
}

