function table_maxn(t)
    local mn = 0
    for k, v in pairs(t) do
        if mn < k then
            mn = k
        end
    end
    return mn
end

table2 = {9, 8, 7, 6}
for i = 1, table_maxn(table2) do
--for i = 1, 2 do
    print("+++")
end

-- 两表相加操作
mytable = setmetatable({ 1, 2, 3 }, {
  __add = function(table1, table2)
    for i = 1, table_maxn(table2) do --遍历表2的index而已
      table.insert(table1, table_maxn(table1) + 1, table2[i])
    end
    return table1
  end
})

secondtable = {4,5,6}

mytable = mytable + secondtable

for k,v in ipairs(mytable) do
    print(k,v)
end

print("--------------")
other = { foo = 3 } 
t = setmetatable({}, { __index = other })
print(t.foo)
print(t.bar)

print("--------------")
other1 = { foo = 3 } 
t1 = setmetatable(other1, {__index})
print(t1.foo)
print(t1.bar)

print("--------------")
t2 = setmetatable({key1 = "value1"}, {
  __index = function(laskdjflmytable, key)  --两个参数第一个参数有点像this指针
    if key == "key2" then
      return "fount key2"
    else
      return nil
    end
  end
})
print(t2.key1, t2.key2)

print("--------------")
t3 = setmetatable({key1 = "value1"}, { __index = { key2 = "metatablevalue" } })
print(t3.key1, t3.key2)

print("--------------------------------")
t4 = setmetatable({key1 = "value1"}, { __newindex})
print(t4.key1)

t4.newkey = "新值2"
print(t4.newkey, t4.newkey)

t4.key1 = "新值1"
print(t4.key1, t4.key1)

print("--------------------------------")
mymetatable = {}
t5 = setmetatable({key1 = "value1"}, { __newindex = mymetatable })

print(t5.key1)

t5.newkey = "新值2"
print(t5.newkey,mymetatable.newkey)

t5.key1 = "新值1"
print(t5.key1,mymetatable.key1)


print("--------------------------------")
t6 = setmetatable({key1 = "value1"}, {
  __newindex = function(tslkdf, key, value)
                rawset(tslkdf, key, "\""..value.."\"")

  end
})

t6.key1 = "new value"
t6.key2 = 4
print(t6.key1, t6.key2)

print("--------------------------------")
function CreateCaseInsensitiveTable1()
  local table1 = {}

  function table1.__index(t, key) --重载table1的__index 返出字符串小写格式
    if(type(key) == "string") then
      key = key:lower()
    end
    return rawget(t, key)
  end

  function table1.__newindex(t, key, value) --重载table1的__newindex 插入小写
    if(type(key) == "string") then
      key = key:lower()
    end
    rawset(t, key, value)
  end

  local ret = {}
  setmetatable(ret, table1)
  return ret
end

t1 = CreateCaseInsensitiveTable1()
t1.A = "A"
t1.B = "A"
t1.a = "a"
print("t1.a: ", t1.a, "t1.A: ", t1.A)
print("t1.b: ", t1.b, "t1.B: ", t1.B)

