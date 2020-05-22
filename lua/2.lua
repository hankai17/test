local file = io.open("1.html", "r")
assert(file)
local data = file:read("*a")
file:close()

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

data1, rep_count = string.gsub(data, "(http.?)://([^/\n%s>\"']+)([^\n%s>\"']*[\n%s>\"'])", test)
print("new: ", data1)
print(rep_count)
