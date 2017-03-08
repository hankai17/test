local domain_white_list_file = "/etc/pdns-recursor/domain-white-list"
local domain_black_list_file = "/etc/pdns-recursor/domain-black-list"
local ip_black_list_file = "/etc/pdns-recursor/ip-black-list"
local isp_ip_list_file = "/etc/pdns-recursor/isp-ip-list"

function initconf()
    local conftb = {}
    for line in io.lines("/etc/pdns-recursor/icr.conf") do
        if string.len(line) > 0 and string.sub(line, 1, 1) ~= "#" then
            local key, val = string.match(trim(line), "(.+)=(.+)")
            conftb[key] = val
        end
    end
    return conftb

end

function trim(s)
    return (string.gsub(s, "^%s*(.-)%s*$", "%1"))
end

function build_set_from_file(file, suffix)
    local set = {}
    if not suffix then suffix = "" end
    for line in io.lines(file) do
        if string.len(line) > 0 and string.sub(line, 1, 1) ~= "#" then
            set[trim(line)..suffix] = true
        end
    end
    return set
end

local wl = {}
function group(g)
    for _, domain in pairs(g.domains) do
        wl[domain.."."] = g.servers
    end
end

dofile(domain_white_list_file)
local conftab = initconf()
local autocheck = conftab["domain-auto-check"]
if not autocheck then autocheck = "no" end

local ip_blacklist = build_set_from_file(ip_black_list_file)
local domain_blacklist = build_set_from_file(domain_black_list_file, ".")
local isp_ip_list = build_set_from_file(isp_ip_list_file)

function ip_match(ip)
    if isp_ip_list[ip] then
        return true
    else
        return false
    end
end

function domain_match(tdomain, domain)
    if tdomain[domain] then return domain end
    local dparts = {}
    for w in string.gmatch(domain, "[^%.]+") do
        dparts[#dparts + 1] = w
    end
    local domain2
    local i = 0
    while true do
        i = string.find(domain, "%.", i+1)
        if i == nil then break end
        domain2 = "*."..string.sub(domain, i+1)
        if tdomain[domain2] then return domain2 end
    end
    return nil
end

function preresolve(ip, domain, qtype)
  if autocheck == "yes" then return -1, ret end
  pdnslog("[pdnslog] pre: "..ip..", "..domain..", "..qtype)

  local ret={}
  if domain_match(domain_blacklist, domain) then return -1, ret end
  if ip_blacklist[ip] then return -1, ret end
  -- if (qtype ~= pdns.A and qtype ~= pdns.AAAA) then return -1, ret end
  if domain_match(wl, domain) then
    local ips = wl[domain_match(wl, domain)]
    -- if (qtype == pdns.A or qtype == pdns.AAAA) then
    if qtype == pdns.A then
      for k,v in ipairs(ips)
      do
        ret[k] = {qtype=pdns.A, content=v, ttl=120}
      end
      setvariable()
      return 0, ret                 -- return no error, plus records
    else
      pdnslog("[pdnslog] pre(NOA): "..ip..", "..domain..", "..qtype)
      return -1, ret
    end
  else
    return -1, ret
  end
end

function postresolve(remoteip, domain, qtype, records, origrcode)
  pdnslog("[pdnslog] post: "..remoteip..", "..domain..", "..qtype..", "..origrcode)

  local ret={}
  if domain_match(domain_blacklist, domain) then return -1, ret end
  if ip_blacklist[ip] then return -1, ret end
  if domain_match(wl, domain) then
    if qtype == pdns.AAAA then
    -- if 1==1 then
      for k,v in ipairs(records)
      do
        -- pdnslog("[pdnslog] post: No."..k.." qtype="..v['qtype'].." content="..v['content'])
        if v['qtype'] == pdns.AAAA then
          return -1, ret
        end
      end
      return 0, ret
    end
  end
  
  return -1, records

end

function postresolve_autocheck(remoteip, domain, qtype, records, origrcode)
  if autocheck == "no" then return -1, ret end
  pdnslog("[pdnslog] post: "..remoteip..", "..domain..", "..qtype..", "..origrcode)

  local ret={}
  if domain_match(domain_blacklist, domain) then return -1, ret end
  if ip_blacklist[ip] then return -1, ret end
  -- if (qtype ~= pdns.A and qtype ~= pdns.AAAA) then return -1, ret end
  if domain_match(wl, domain) then
    local ips = wl[domain_match(wl, domain)]
    if (qtype == pdns.A or qtype == pdns.AAAA) then
      -- if A recode in ISP's ip range, then do nothing
      for key,val in ipairs(records)
      do
        if val.qtype == pdns.A and ip_match(val.content) then return -1, ret end
      end
      for k,v in ipairs(ips)
      do
        ret[k] = {qtype=pdns.A, content=v, ttl=120}
      end
      setvariable()
      return 0, ret                 -- return no error, plus records
    else
      -- pdnslog("[pdnslog]: "..ip..", "..domain..", "..qtype)
      return -1, ret
    end
  else
    return -1, ret
  end

end
