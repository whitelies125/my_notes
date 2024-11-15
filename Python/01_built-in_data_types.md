## Built-in Data Types

In programming, data type is an important concept.

Variables can store data of different types, and different types can do different things.

Python has the following data types built-in by default, in these categories:

| Category        | data type                          |
| --------------- | ---------------------------------- |
| Text Type:      | `str`                              |
| Numeric Types:  | `int`, `float`, `complex`          |
| Sequence Types: | `list`, `tuple`, `range`           |
| Mapping Type:   | `dict`                             |
| Set Types:      | `set`, `frozenset`                 |
| Boolean Type:   | `bool`                             |
| Binary Types:   | `bytes`, `bytearray`, `memoryview` |
| None Type:      | `NoneType`                         |

You can get the data type of any object by using the `type()` function:

~~~python
x = 5
print(type(x))
~~~

### List []

Ordered：有序，新插入元素在 list 尾部

~~~python
thislist = ["apple", "banana", "cherry", "durian"]

print(thislist[0]) # apple
print(thislist[-1]) # durian
print(thislist[1:3]) # ['banana', 'cherry'] # 左闭右开

thislist.append("orange")
print(thislist) # ['apple', 'banana', 'cherry', 'durian', 'orange']

thislist.insert(1, "lemon")
print(thislist) # ['apple', 'lemon', 'banana', 'cherry', 'durian', 'orange']

thislist.remove("lemon")
print(thislist) # ['apple', 'banana', 'cherry', 'durian', 'orange']

thislist.pop(1)
print(thislist) # ['apple', 'cherry', 'durian', 'orange']

thislist.pop()
print(thislist) # ['apple', 'cherry', 'durian']

thislist.clear()
print(thislist) # []

thislist = ["apple", "banana", "cherry"]
for x in thislist:
  print(x)
for i in range(len(thislist)):
  print(thislist[i])
i = 0
while i < len(thislist):
  print(thislist[i])
  i = i + 1
~~~

### Tuple ()

Ordered：有序

Unchangeable：不可修改

~~~python
thistuple = ("apple", "banana", "cherry")
print(thistuple[1])
print(thistuple[-1])

fruits = ("apple", "banana", "cherry")
(green, yellow, red) = fruits

for x in thistuple:
  print(x)
~~~

### Set {}

Unordered：无序

Unchangeable：不可修改

Duplicates Not Allowed：不允许重复元素

~~~python
thisset = {"apple", "banana", "cherry"}
print(thisset)

thisset.add("orange")
print(thisset)

tropical = {"pineapple", "mango", "papaya"}
thisset.update(tropical)
print(thisset)

mylist = ["kiwi", "orange"]
thisset.update(mylist)
print(thisset)

for x in thisset:
  print(x)
~~~

### Dictionary { "key": value }

Ordered or Unordered?
As of Python version 3.7, dictionaries are ordered.
In Python 3.6 and earlier, dictionaries are unordered.

~~~python
thisdict = {
  "brand": "Ford",
  "model": "Mustang",
  "year": 1964
}
x = thisdict["model"]
x = thisdict.get("model")
keys = thisdict.keys()
values = thisdict.values()

thisdict.pop("model")
print(thisdict)
~~~

2024.11.15
