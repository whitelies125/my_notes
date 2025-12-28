

~~~lua
-- There are eight basic types in Lua:
-- nil
    -- The type *nil* has one single value, **nil**,
    -- whose main property is to be different from any other value;
    -- it often represents the absence of a useful value.
local n = nil
print(type(n))
-- boolean
local f = false
if not f and not nil then
    print("both nil and false make a condition false")
end
-- number
local num = 1
-- string
local string = "use .. "
print(string .. " to concatenate string")
-- function
local func = function()
    print("call func")
end
func()
-- userdata
-- thread
-- table
local function print_tbl(tbl)
    for k,v in pairs(tbl) do
        print(k, v)
    end
    print()
end
local tbl = {"gamma", "alpha", "beta"}
    -- as array
for i = 1, #tbl, 1 do -- initial value, limit, step
    print(i, tbl[i])
end
table.sort(tbl)
print("sort:")
print_tbl(tbl)
    -- as map
tbl = {
    a = "alpha",
    b = "beta",
    [123] = 321,
}
print_tbl(tbl)
tbl.c = "chi"
tbl.a = "algo"
tbl.b = nil
print_tbl(tbl)
    -- as class
    -- delete table
tbl = nil
    -- is table empty?
tbl = {}
if next(tbl) == nil then
    print("empty")
end
~~~

2023.03.04