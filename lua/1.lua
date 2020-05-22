--local file = io.open("ifeng.html", "r")
--local file = io.open("baidu.txt", "r")
local file = io.open("1.html", "r")
--local file = io.open("ne.txt", "r")
assert(file)
local data = file:read("*a")
file:close()

--print(data)
--local body2 = string.gsub(data, "http://([^/%?]+)(.+)", "http://".."%1".."http.80.ipv6.net-east.com/".."%2")

function test(protocol, host, url)
    local port
    if string.find(host, "www.net%-east.com") then
        return "http://"..host..url
    end

    if string.find(host, "ipv6.net%-east.com") then --if exits * domain or special domain not touch this
        return "http://"..host..url
    end

    --reset port from host
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
    --print("host: ", host, " port: ", port)

    --reset ori host eg: www.ifeng.com --> www.ifeng.ex1
    --TODO

    new_location = string.format("http://%s.%s.%s.%s%s", host, protocol, port, "ipv6.net-east.com", url)
    return new_location
    
end

function ifeng_gsub(protocol, host, url)
    local port
    if string.find(host, "www.net%-east.com") then
        return "http://"..host..url
    end

    if string.find(host, "ipv6.net%-east.com") then --if exits * domain or special domain not touch this
        return "http://"..host..url
    end
    -- default use http
    port = "80"
    prefix = protocol
    protocol = "http"
    port_pos = string.find(host, ":")
    if port_pos ~= nil then
        port = string.sub(host, port_pos + 1)
        host = string.sub(host, 0, port_pos - 1)
    end
    new_location = string.format("%s//%s.%s.%s.%s%s", prefix, host, protocol, port, "ipv6.net-east.com", url)
    print("new_location: ", new_location)
    return new_location
end

function ifeng_rewrite(body)
    body, rep_count = string.gsub(body, "(src=\")//([^/\n%s>\"]+)([^\n%s>\"]*[\n%s>\"])", ifeng_gsub)
    body, rep_count = string.gsub(body, "(thumbnail\":\")//([^/\n%s>\"]+)([^\n%s>\"]*[\n%s>\"])", ifeng_gsub)
    return body
end

other_domain_dic = {
    ["www.ifeng.com"]   = { _filter_body_func = ifeng_rewrite },
    ["www.iqiyi.com"]   = { _filter_body_func = nil },
    ["www.hao123.com"]  = { _filter_body_func = nil },
    ["www.baidu.com"]   = { _filter_body_func = nil },
    ["www.douban.com"]  = { _filter_body_func = nil },
}

--根据host找对应的规则; rewrite body
function other_domain_check(host, body)
    if not other_domain_dic[host] then
        return body
    end
    return other_domain_dic[host]._filter_body_func(body)
end



print("+++++++++++++++++++++++++++++++++++")
data1, rep_count = string.gsub(data, "(http.?)://([^/\n%s>\"']+)([^\n%s>\"']*[\n%s>\"'])", test)

print("new: ", data1)
print(rep_count)
print("+++++++++++++++++++++++++++++++++++")

--[[
old_url = "http://house.ifeng.com.https.443.ipv6.net-east.com/news/2020_05_09-52741787_0.shtml"
old_host = "house.ifeng.com.https.443.ipv6.net-east.com"
real_host, real_proto, real_port = string.match(old_host, "(.*)%.(%a+)%.(%d+)%.ipv6%.net%-east%.com")
print("real_host: ", real_host)
print("real_proto: ", real_proto)
print("real_port: ", real_port)
m1, m2 = string.match("image-type", "image");
print("m1: ", m1, " m2: ", m2)
]]--

print("----------------------------")
--old_host = "house.ifeng.com.https.443.ipv6.net-east.com"
--old_host = "ipv6.net-east.com"
old_host = "net-east.com"
r_host, base_host = string.match(old_host, "[^%.]+%.[^%.]+%.[^%.]+$")
print("real_host: ", r_host, " base_host: ", base_host)


print("----------------------------")
content_type_dic = {
    ["image"] = "1",
    ["text"] = "2",
    ["application"] = nil,
}
print(content_type_dic["image"])
print(content_type_dic["text"])
print(content_type_dic["application"])
print(content_type_dic["appliation"])


print("----------------------------")
--if string.find("www.net-east.com", "www.net-east.com") then
if "www.net-east.com" == "www.net-east.com" then
    print("found www.net-east.com")
else
    print("not found www.net-east.com")
end

print(string.find("www.net-east.com", "www.net-east.com"))
