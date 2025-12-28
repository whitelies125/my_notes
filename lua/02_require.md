## require(modname)

```powershell
├─lua
│  │  require_impl.lua
│  │  test_use_require.lua
│  │  test_use_require_impl.lua
│  │
│  └─include
│          module.lua
```

### require 的执行流程

参考《Lua 5.4 Reference Manual》6.3 Module 中 **require (modname)** 及 **package.searchers** 的描述，写出 require 的粗略执行流程：

```lua
-- require_impl.lua

local function require_impl(modname)
    if package.loaded[modname] then
        return package.loaded[modname]
    end
    -- To find a loader, require is guided by the table package.searchers.
    -- Each item in this table is a search function,
    -- that searches for the module in a particular way.
    for n,searcher in pairs(package.searchers) do
        print("count : " .. n)
        local loader, loader_data = searcher(modname)
        -- If it(searcher) cannot find the module,
        -- it returns a string explaining why (or nil if it has nothing to say).
        if loader == nil or loader and type(loader) == "string" then
            print(loader)
        end
        if loader and type(loader) == "function" then
            local ret = loader(modname, loader_data)
            -- If the loader returns any non-nil value,
            -- require assigns the returned value to package.loaded[modname].
            if ret ~= nil then
                package.loaded[modname] = ret
            -- If the loader does not return a non-nil value
            -- and has not assigned any value to package.loaded[modname],
            -- then require assigns true to this entry.
            elseif package.loaded[modname] == nil then
                package.loaded[modname] = true
            end
            -- In any case, require returns the final value of package.loaded[modname].
            return package.loaded[modname], loader_data
        end
    end
end
```

### test

```text
-- module.lua
load_num = load_num or 0
load_num = load_num + 1
print("load success. num : " .. load_num)
```

#### 使用自己的 require_impl：

```text
-- test_use_require_impl.lua

package.path = package.path .. ";./include/?.lua"
print("package.loaded[\"module\"] : ", package.loaded["module"])
print()

ret, loader_data = require_impl("module")
print(ret, loader_data)
print("package.loaded[\"module\"] : ", package.loaded["module"])
print()

ret, loader_data = require_impl("module")
-- (The absence of a second result in this case signals that this call did not have to load the module.)
print(ret, loader_data)
print("package.loaded[\"module\"] : ", package.loaded["module"])
print()

package.loaded["module"] = false
ret, loader_data = require_impl("module")
print(ret, loader_data)
print("package.loaded[\"module\"] : ", package.loaded["module"])
print()

ret, loader_data = require_impl("module")
print(ret, loader_data)
print("package.loaded[\"module\"] : ", package.loaded["module"])
print()
```

输出：

```powershell
PS E:\Code\My\lua> lua require_impl.lua
package.loaded["module"] :      nil

searcher index : 1
no field package.preload['module']
searcher index : 2
load success. num : 1
true    ./include/module.lua
package.loaded["module"] :      true

true    nil
package.loaded["module"] :      true

searcher index : 1
no field package.preload['module']
searcher index : 2
load success. num : 2
false   ./include/module.lua
package.loaded["module"] :      false

searcher index : 1
no field package.preload['module']
searcher index : 2
load success. num : 3
false   ./include/module.lua
package.loaded["module"] :      false

PS E:\Code\My\lua>
```

因为：

> The first searcher simply looks for a loader in the package.preload table.
>
> The second searcher looks for a loader as a Lua library, using the path stored at package.path. 
>
> The third searcher looks for a loader as a C library, using the path given by the variable package.cpath.
>
> The fourth searcher tries an *all-in-one loader*. It searches the C path for a library for the root name of the given module.

所以，在输出中可见，总是在 searcher index = 2 时成功 load。

#### 使用 require：

```text
-- test_use_require.lua

package.path = package.path .. ";./include/?.lua"
print("package.loaded[\"module\"] : ", package.loaded["module"])
print()

ret, loader_data = require "module"
print(ret, loader_data)
print("package.loaded[\"module\"] : ", package.loaded["module"])
print()

ret, loader_data = require "module"
-- (The absence of a second result in this case signals that this call did not have to load the module.)
print(ret, loader_data)
print("package.loaded[\"module\"] : ", package.loaded["module"])
print()

package.loaded["module"] = false
ret, loader_data = require "module"
print(ret, loader_data)
print("package.loaded[\"module\"] : ", package.loaded["module"])
print()

ret, loader_data = require "module"
print(ret, loader_data)
print("package.loaded[\"module\"] : ", package.loaded["module"])
print()
```

输出：

```powershell
PS E:\Code\My\lua> lua require.lua
package.loaded["module"] :      nil

load success. num : 1
true    ./include/module.lua
package.loaded["module"] :      true

true    nil
package.loaded["module"] :      true

load success. num : 2
false   ./include/module.lua
package.loaded["module"] :      false

load success. num : 3
false   ./include/module.lua
package.loaded["module"] :      false

PS E:\Code\My\lua> 
```

与自己按文档写的 require_impl 一致。

2023.03.04