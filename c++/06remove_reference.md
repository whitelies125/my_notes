# std::move()

std::move() 的实现，move.h：

~~~C++
/**
  *  @brief  Convert a value to an rvalue.
  *  @param  __t  A thing of arbitrary type.
  *  @return The parameter cast to an rvalue-reference to allow moving it.
  */
template<typename _Tp>
_GLIBCXX_NODISCARD
constexpr typename std::remove_reference<_Tp>::type&& move(_Tp&& __t) noexcept
{
    return static_cast<typename std::remove_reference<_Tp>::type&&>(__t);
}
~~~

### remove_reference

显然涉及到 remove_reference，所以先讲讲 remove_reference。

remove_reference 的实现，type_traits.h：

~~~C++
/// remove_reference
template<typename _Tp>
struct remove_reference
{ typedef _Tp   type; };

template<typename _Tp>
struct remove_reference<_Tp&>
{ typedef _Tp   type; };

template<typename _Tp>
struct remove_reference<_Tp&&>
{ typedef _Tp   type; };
~~~

不过 std::move() 中使用的是第三个。

~~~C++
template<typename _Tp>
struct remove_reference<_Tp&&> {
    typedef _Tp   type;
};
~~~

以 string 为例分析：

当传递给该模板左值时，因左值传递给右值引用，故 \_Tp 推断为左值的引用类型 (const) string&，于是 type 为 _Tp 的别名，(const) string。

当传递给该模板右值时，因左值传递给右值引用，故 \_Tp 推断为左值的基本类型 string，于是 type 为 _Tp 的别名，string。

1. string
2. string&
3. string&&
4. const string
5. const string&
6. const string&&



