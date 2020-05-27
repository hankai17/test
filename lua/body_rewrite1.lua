local zlib = require "zlib"
local ngx_match = ngx.re.match
local uri = ngx.var.uri
local headers = ngx.resp.get_headers()
local request_host = ngx.var.ori_host

function get_type()
    local ngx_status = ngx.status
    if ngx_status ~= 200 then
        return nil
    end
    return nil
end

function is_text_type()
    local content_type = headers["Content-Type"] --image/jpeg
    --ngx.log(ngx.ERR, ">> type: ", content_type)
    if not content_type then
       return nil
    end
    if string.match(content_type, "image") then
       return nil
    end
    if string.match(content_type, "application") then
       return "app"
    end
    if string.match(content_type, "css") then
       return "app"
    end
    if string.match(content_type, "javascript") then
       return "app"
    end
    if string.match(content_type, "text") then
       return "text"
    end
    return nil
end
    
function get_body()
    local ngx_status = ngx.status
    local body = ""
    if ngx_status ~= 200 then
        return nil
    else
        local ngx_chunks = ngx.arg
        local i = 1
        while true do
            if ngx_chunks[i] == true then
        	--ngx.log(ngx.ERR, ">> ngx_chunks == true")
                return nil
            end
            if ngx_chunks[i] == false then
                break
            end 
            body = body..ngx_chunks[i]
            i = i + 1
        end
    end
    return body
end

function test(protocol, host, url)
    local port

    --if string.find(host, request_host) then
    ngx.log(ngx.ERR, ">> ", host, " ", request_host)
    if host == request_host then
        return protocol.."://"..host..url
    else
        --ngx.log(ngx.ERR, ">> host != request_host ", host, " ", request_host)
    end

    --[[
    if string.find(host, request_host) then
        return protocol.."://"..host..url
    end
    ]]--

    if protocol == "http" then
        port = "80"
    elseif protocol == "https" then
        port = "443"
    end

    port_pos = string.find(host, ":")
    if port_pos ~= nil then
        port = string.sub(host, port_pos + 1)
        host = string.sub(host, 0, port_pos - 1)
    end

    new_location = string.format("http://%s.%s.%s.%s%s", host, protocol, port, "pr.net-east.com", url)
    --new_location = string.format("http://%s%s%s", "pr.net-east.com/", host, url)
    return new_location
end

function ifeng_gsub(protocol, host, url)
    local port
    -- default use http
    port = "80"
    prefix = protocol
    protocol = "http"
    port_pos = string.find(host, ":")
    if port_pos ~= nil then
        port = string.sub(host, port_pos + 1)
        host = string.sub(host, 0, port_pos - 1)
    end
    new_location = string.format("%s//%s.%s.%s.%s%s", prefix, host, protocol, port, "pr.net-east.com", url)
    return new_location
end

function ifeng_rewrite(body)
    -- img src="//d.ifengimg.com/w50_h58_q70/x0.ifengimg.com/ucms/2020_20/3DB17BDF1D4B7C8BF7BEE0E0642D025812B04446_w50_h58.png
    body, rep_count = string.gsub(body, "(src=\")//([^/\n%s>\"]+)([^\n%s>\"]*[\n%s>\"])", ifeng_gsub)
    body, rep_count = string.gsub(body, "(thumbnail\":\")//([^/\n%s>\"]+)([^\n%s>\"]*[\n%s>\"])", ifeng_gsub)
    return body
end

function map_baidu_rewrite(body)
    body, rep_count = string.gsub(body, "maponline0.bdimg.com", "maponline0.bdimg.com.http.80.pr.net-east.com")
    body, rep_count = string.gsub(body, "maponline1.bdimg.com", "maponline1.bdimg.com.http.80.pr.net-east.com")
    body, rep_count = string.gsub(body, "maponline2.bdimg.com", "maponline2.bdimg.com.http.80.pr.net-east.com")
    body, rep_count = string.gsub(body, "maponline3.bdimg.com", "maponline3.bdimg.com.http.80.pr.net-east.com")
    body, rep_count = string.gsub(body, "(http.?)://([^/\n%s>\"']+)([^\n%s>\"']*[\n%s>\"'])", test)
    return body
end

function topway_rewrite(body)
    -- body, rep_count = string.gsub(body, "(http.?)://([^/\n%s>\"']+)([^\n%s>\"']*[\n%s>\"'])", test)
    -- body, rep_count = string.gsub(body, "s19.cnzz.com", "s19.cnzz.com.http.80.pr.net-east.com")
    if string.match(uri, "leftsidebar") then -- 我们只修改某个特征的url
        body, rep_count = string.gsub(body, "https://speed.topway.cn", "http://speed.topway.cn.https.443.pr.net-east.com")
    end

    return body
end

function szmg_api_rewrite(body)
    body, rep_count = string.gsub(body, "api.szmg.com.cn", "api.szmg.com.cn.http.80.pr.net-east.com")
    body, rep_count = string.gsub(body, "login.szmg.com.cn", "login.szmg.com.cn.http.80.pr.net-east.com")
    return body
end

function ioa_url_reformat(body)
    body, rep_count = string.gsub(body, "http://www.ioa.cas.cn/dj", "http://www.ioa.cas.cn/dj/")
    body, rep_count = string.gsub(body, "(http.?)://([^/\n%s>\"']+)([^\n%s>\"']*[\n%s>\"'])", test)
    return body
end

function topway_txt_reformat(body)
    --do nothing
    return body
end

--服务的域名js/css/application请求 是否要修改内容
app_domain_dic = {
     ["topway.com.cn"] = { _filter_body_func = topway_rewrite },
}

--外链的js/css/application请求 是否要修改这些外链内容
app_other_domain_dic = {
    ["map.baidu.com"]   = { _filter_body_func = map_baidu_rewrite }, --针对net-east.com中的小地图
    ["www.ifeng.com"]   = { _filter_body_func = ifeng_rewrite },
    ["szmg.com.cn"]     = { _filter_body_func = szmg_api_rewrite },
    ["www.hao123.com"]  = { _filter_body_func = nil },
    ["www.douban.com"]  = { _filter_body_func = nil },
}

--text请求 是否修改 暂时不区分外链&服务
txt_domain_reformat = {
    ["www.ioa.cas.cn"] = { _url_func = ioa_url_reformat }, --声学所 url不规范
}

--js/css/application请求处理流程
function app_request_process(host_p, body)
    host, real_proto, real_port = string.match(host_p, "(.*)%.(%a+)%.(%d+)%.pr%.net%-east%.com")
    ngx.log(ngx.ERR, ">> ", host, real_proto, real_port, host_p)
    if host == nil then --服务的域名
        if not app_domain_dic[host_p] then
            r_host = string.match(host_p, "[^%.]+%.[^%.]+%.[^%.]+$")
            if r_host then
                if not app_domain_dic[r_host] then
                    return body
                else
                    return app_domain_dic[r_host]._filter_body_func(body)
                end
            else
                return body
            end
        else
            return app_domain_dic[host_p]._filter_body_func(body)
        end
    end

    if not app_other_domain_dic[host] then --外链请求
	r_host = string.match(host, "[^%.]+%.[^%.]+%.[^%.]+$")
        if r_host then
            if not app_other_domain_dic[r_host] then
                ngx.log(ngx.ERR, ">> 1 r_host not in dic", r_host)
                return body
            else
                ngx.log(ngx.ERR, ">> 2 r_host in dic", r_host)
                return app_other_domain_dic[r_host]._filter_body_func(body)
            end
        end
        return body
    end
    return app_other_domain_dic[host]._filter_body_func(body)
end

ngx.log(ngx.ERR, ">> coming a request")
local body = get_body()
if body then
    local ngx_hdr_host = ngx.var.host

    if is_text_type() == "app" then
        body = app_request_process(ngx_hdr_host, body)
    elseif is_text_type() == "text" then
        --body, rep_count = string.gsub(body, "(http.?)://([^/\n%s>\"']+)([^\n%s>\"']*[\n%s>\"'])", test)

        if txt_domain_reformat[ngx_hdr_host] then
            body = txt_domain_reformat[ngx_hdr_host]._url_func(body)
        else
            body, rep_count = string.gsub(body, "(http.?)://([^/\n%s>\"']+)([^\n%s>\"']*[\n%s>\"'])", test)
        end
    end

    ngx.arg[1] = body
    ngx.arg[2] = false
else
    --ngx.log(ngx.ERR, ">> no body")
end

local filter_version = "0.0.0"
return
