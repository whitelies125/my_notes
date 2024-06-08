[TOC]

## Makefile Cookbook

让我们来看一个非常精彩的 Make 示例，它非常适合中型项目。

这个 makefile 的妙处在于它会自动为您确定依赖项。您所要做的就是将您的 C/C++ 文件放在 src/ 文件夹中。

~~~Makefile
# Thanks to Job Vranish (https://spin.atomicobject.com/2016/08/26/makefile-c-projects/)
# 目标可执行文件名字
TARGET_EXEC := final_program

# build 目录
BUILD_DIR := ./build
# 源文件目录
SRC_DIRS := ./src

# Find all the C and C++ files we want to compile
# Note the single quotes around the * expressions. The shell will incorrectly expand these otherwise, but we want to send the * directly to the find command.
# 通过 shell 的 find 命令在源文件目录中获取到所有源文件的文件名
SRCS := $(shell find $(SRC_DIRS) -name '*.cpp' -or -name '*.c' -or -name '*.s')

# Prepends BUILD_DIR and appends .o to every src file
# As an example, ./your_dir/hello.cpp turns into ./build/./your_dir/hello.cpp.o
# 通过 $(patsubst pattern,replacement,text) 的 $(text:pattern=replacement) 简写形式，转换得到目标文件文件名
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

# String substitution (suffix version without %).
# As an example, ./build/hello.cpp.o turns into ./build/hello.cpp.d
# 通过 $(patsubst pattern,replacement,text) 的 $(text:suffix=replacement) 简写形式，将 .o 转换得到 .d 文件名
DEPS := $(OBJS:.o=.d)

# Every folder in ./src will need to be passed to GCC so that it can find header files
# 获取 ./src 目录中的所有文件夹的名字
INC_DIRS := $(shell find $(SRC_DIRS) -type d)
# Add a prefix to INC_DIRS. So moduleA would become -ImoduleA. GCC understands this -I flag
# 将 INC_DIRS 添加到编译器寻找头文件的路径中
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

# The -MMD and -MP flags together generate Makefiles for us!
# These files will have .d instead of .o as the output.
# 编译器选项，让编译器生成依赖关系文件 .d 文件的
CPPFLAGS := $(INC_FLAGS) -MMD -MP

# The final build step.
# 可执行文件依赖于所有的目标文件
# $@ 表示 target 名
$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

# Build step for C source
# .c 文件的 rule
# $(dir names) 内置函数，作用是提取 names 的目录部分，如 $(dir src/foo.c hacks) 结果为 src/ ./
# $< 第一个 prerequisite 的名字
$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# Build step for C++ source
# .cpp 文件的 rule
$(BUILD_DIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@


.PHONY: clean
clean:
	rm -r $(BUILD_DIR)

# Include the .d makefiles. The - at the front suppresses the errors of missing
# Makefiles. Initially, all the .d files will be missing, and we don't want those
# errors to show up.
# include 生成的依赖关系文件 .d 文件
# 加 - 忽略错误，是因为第一次执行 make 时，还没有生成 .d 文件
# 因为 .d 文件是在编译时作为编译的副产物生成的
-include $(DEPS)
~~~

为什么要生成依赖文件。

注意到，我们编写 makefile 时，只描述了 .o 目标文件依赖于 .cpp 源文件，这意味着 .o 文件是否更新只取决于 .cpp 文件的时间戳是否更新。

而并没有描述 .o 依赖于 .cpp 文件 include 的头文件，因此，如果仅修改 .cpp 依赖 include 的头文件，而没有修改 .cpp 文件，则 .cpp 文件的修改时间戳未更新，.o 文件自然也不会更新。

所以需要传入编译器选项，通过编译器来帮助我们自动生成依赖文件，然后 include 进来。这样修改 .cpp inlucde 的头文件，就能够正常更新编译程序了。

例如：main.cpp include 了 test.h

~~~shell
code
|---Makefile
|---src
|   |---include
|	|	|---test.h
|	|---main.cpp
~~~

通过上例的 Makefile 生成的 .d 文件实际上也是一个符合 Makefile 语法的文本：build/src/main.cpp.d

~~~Makefile
build/./src/main.cpp.o: src/main.cpp src/include/test.h
src/include/test.h:
~~~

可见，.d 文件中说明了 .o 依赖于 .h 文件。

于是当 .h 文件发生更新时，make 就会更新 .o 文件，而更新 .o 文件的 command 则已经通过 $(BUILD_DIR)/%.cpp.o: %.cpp 隐式规则给出。

#### 关于 -MMD

gcc 预处理器选项，参考：https://gcc.gnu.org/onlinedocs/gcc-5.5.0/gcc/Preprocessor-Options.html

-E

> If you use the -E option, nothing is done except preprocessing. Some of these options make sense only together with -E because they cause the preprocessor output to be unsuitable for actual compilation.
>
> 如果使用 -E 选项，则除了预处理外不会执行任何操作。其中一些选项只有与 -E 一起使用才有意义，因为它们会导致预处理器的输出不符合实际的编译。

-M

> Instead of outputting the result of preprocessing, output a rule suitable for `make` describing the dependencies of the main source file. The preprocessor outputs one `make` rule containing the object file name for that source file, a colon, and the names of all the included files, including those coming from -include or -imacros command-line options.
>
> 输出符合 make 描述主源文件依赖的 rule，而不是输出预处理的结果。预处理器输出一个 make rule，其中包含源文件名，冒号，源文件 include 的所有文件名——包括来源于命令行选项 -include 及 -imacros 指出的文件。

-MF file

> When used with -M or -MM, specifies a file to write the dependencies to.
>
> 当与 -M 或 -MM 一起使用时，指定要将依赖项写入的文件。
>
> When used with the driver options -MD or -MMD, -MF overrides the default dependency output file.
>
> 当与 -MD 或 -MMD 一起使用时，-MF 重写默认依赖输出文件。
>
> If file is -, then the dependencies are written to stdout.
>
> 如果 file 未空，则默认写到标准输出

-MD

> is equivalent to -M -MF file, except that -E is not implied. The driver determines file based on whether an -o option is given. If it is, the driver uses its argument but with a suffix of .d, otherwise it takes the name of the input file, removes any directory components and suffix, and applies a .d suffix.
>
> 等价于 -M -MF，但不隐含 -E。驱动程序根据是否给出了 -o 选项决定文件。如果给出了 -o 选择，则驱动程序使用其参数，并添加 .d 后缀；否则使用输入的文件名，并去除其目录内容及后缀，再替换为 .d 后缀。
>
> If -MD is used in conjunction with -E, any -o switch is understood to specify the dependency output file (see [-MF](https://gcc.gnu.org/onlinedocs/gcc-5.5.0/gcc/Preprocessor-Options.html#dashMF)), but if used without -E, each -o is understood to specify a target object file.
>
> Since -E is not implied, -MD can be used to generate a dependency output file as a side-effect of the compilation process.
>
> 由于 -E 是不隐含的，因此 -MD 可以用于生成依赖文件，作为编译过程的副产物。

-MMD

> Like -MD except mention only user header files, not system header files.
>
> 类似 -MD，但仅生成用户头文件，不生成系统头文件



例子：main.cpp include 了 test.h

~~~shell
code
|---Makefile
|---src
|	|---test.h
|	|---main.cpp
~~~

shell 结果：g++ -MMD main.cpp，可以看见输出的依赖文件中，只有用户自己的头文件

~~~shell
whitelies125@DESKTOP-H47PT8Q:~/code/makefile/src$ g++ -MMD main.cpp
whitelies125@DESKTOP-H47PT8Q:~/code/makefile/src$ ls
a-main.d  a.out  include  main.cpp  test.h
whitelies125@DESKTOP-H47PT8Q:~/code/makefile/src$ cat a-main.d
main.o: main.cpp test.h
whitelies125@DESKTOP-H47PT8Q:~/code/makefile/src$
~~~

shell 结果：g++ -MD main.cpp，可以看见输出的依赖文件中，写出了系统文件

~~~shell
whitelies125@DESKTOP-H47PT8Q:~/code/makefile/src$ ls
include  main.cpp  test.h
whitelies125@DESKTOP-H47PT8Q:~/code/makefile/src$ g++ -MD main.cpp
whitelies125@DESKTOP-H47PT8Q:~/code/makefile/src$ ls
a-main.d  a.out  include  main.cpp  test.h
whitelies125@DESKTOP-H47PT8Q:~/code/makefile/src$ cat a-main.d
main.o: main.cpp /usr/include/stdc-predef.h test.h \
 /usr/include/c++/11/iostream \
 /usr/include/x86_64-linux-gnu/c++/11/bits/c++config.h \
 /usr/include/x86_64-linux-gnu/c++/11/bits/os_defines.h \
 /usr/include/features.h /usr/include/features-time64.h \
 /usr/include/x86_64-linux-gnu/bits/wordsize.h \
 /usr/include/x86_64-linux-gnu/bits/timesize.h \
 /usr/include/x86_64-linux-gnu/sys/cdefs.h \
 /usr/include/x86_64-linux-gnu/bits/long-double.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs-64.h \
 /usr/include/x86_64-linux-gnu/c++/11/bits/cpu_defines.h \
 /usr/include/c++/11/pstl/pstl_config.h /usr/include/c++/11/ostream \
 /usr/include/c++/11/ios /usr/include/c++/11/iosfwd \
 /usr/include/c++/11/bits/stringfwd.h \
 /usr/include/c++/11/bits/memoryfwd.h /usr/include/c++/11/bits/postypes.h \
 /usr/include/c++/11/cwchar /usr/include/wchar.h \
 /usr/include/x86_64-linux-gnu/bits/libc-header-start.h \
 /usr/include/x86_64-linux-gnu/bits/floatn.h \
 /usr/include/x86_64-linux-gnu/bits/floatn-common.h \
 /usr/lib/gcc/x86_64-linux-gnu/11/include/stddef.h \
 /usr/lib/gcc/x86_64-linux-gnu/11/include/stdarg.h \
 /usr/include/x86_64-linux-gnu/bits/wchar.h \
 /usr/include/x86_64-linux-gnu/bits/types/wint_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/mbstate_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__mbstate_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/locale_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__locale_t.h \
 /usr/include/c++/11/exception /usr/include/c++/11/bits/exception.h \
 /usr/include/c++/11/bits/exception_ptr.h \
 /usr/include/c++/11/bits/exception_defines.h \
 /usr/include/c++/11/bits/cxxabi_init_exception.h \
 /usr/include/c++/11/typeinfo /usr/include/c++/11/bits/hash_bytes.h \
 /usr/include/c++/11/new /usr/include/c++/11/bits/move.h \
 /usr/include/c++/11/type_traits \
 /usr/include/c++/11/bits/nested_exception.h \
 /usr/include/c++/11/bits/char_traits.h \
 /usr/include/c++/11/bits/stl_algobase.h \
 /usr/include/c++/11/bits/functexcept.h \
 /usr/include/c++/11/bits/cpp_type_traits.h \
 /usr/include/c++/11/ext/type_traits.h \
 /usr/include/c++/11/ext/numeric_traits.h \
 /usr/include/c++/11/bits/stl_pair.h \
 /usr/include/c++/11/bits/stl_iterator_base_types.h \
 /usr/include/c++/11/bits/stl_iterator_base_funcs.h \
 /usr/include/c++/11/bits/concept_check.h \
 /usr/include/c++/11/debug/assertions.h \
 /usr/include/c++/11/bits/stl_iterator.h \
 /usr/include/c++/11/bits/ptr_traits.h /usr/include/c++/11/debug/debug.h \
 /usr/include/c++/11/bits/predefined_ops.h /usr/include/c++/11/cstdint \
 /usr/lib/gcc/x86_64-linux-gnu/11/include/stdint.h /usr/include/stdint.h \
 /usr/include/x86_64-linux-gnu/bits/types.h \
 /usr/include/x86_64-linux-gnu/bits/typesizes.h \
 /usr/include/x86_64-linux-gnu/bits/time64.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-intn.h \
 /usr/include/x86_64-linux-gnu/bits/stdint-uintn.h \
 /usr/include/c++/11/bits/localefwd.h \
 /usr/include/x86_64-linux-gnu/c++/11/bits/c++locale.h \
 /usr/include/c++/11/clocale /usr/include/locale.h \
 /usr/include/x86_64-linux-gnu/bits/locale.h /usr/include/c++/11/cctype \
 /usr/include/ctype.h /usr/include/x86_64-linux-gnu/bits/endian.h \
 /usr/include/x86_64-linux-gnu/bits/endianness.h \
 /usr/include/c++/11/bits/ios_base.h /usr/include/c++/11/ext/atomicity.h \
 /usr/include/x86_64-linux-gnu/c++/11/bits/gthr.h \
 /usr/include/x86_64-linux-gnu/c++/11/bits/gthr-default.h \
 /usr/include/pthread.h /usr/include/sched.h \
 /usr/include/x86_64-linux-gnu/bits/types/time_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timespec.h \
 /usr/include/x86_64-linux-gnu/bits/sched.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_sched_param.h \
 /usr/include/x86_64-linux-gnu/bits/cpu-set.h /usr/include/time.h \
 /usr/include/x86_64-linux-gnu/bits/time.h \
 /usr/include/x86_64-linux-gnu/bits/timex.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_timeval.h \
 /usr/include/x86_64-linux-gnu/bits/types/clock_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_tm.h \
 /usr/include/x86_64-linux-gnu/bits/types/clockid_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/timer_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_itimerspec.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes.h \
 /usr/include/x86_64-linux-gnu/bits/thread-shared-types.h \
 /usr/include/x86_64-linux-gnu/bits/pthreadtypes-arch.h \
 /usr/include/x86_64-linux-gnu/bits/atomic_wide_counter.h \
 /usr/include/x86_64-linux-gnu/bits/struct_mutex.h \
 /usr/include/x86_64-linux-gnu/bits/struct_rwlock.h \
 /usr/include/x86_64-linux-gnu/bits/setjmp.h \
 /usr/include/x86_64-linux-gnu/bits/types/__sigset_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct___jmp_buf_tag.h \
 /usr/include/x86_64-linux-gnu/bits/pthread_stack_min-dynamic.h \
 /usr/include/x86_64-linux-gnu/c++/11/bits/atomic_word.h \
 /usr/include/x86_64-linux-gnu/sys/single_threaded.h \
 /usr/include/c++/11/bits/locale_classes.h /usr/include/c++/11/string \
 /usr/include/c++/11/bits/allocator.h \
 /usr/include/x86_64-linux-gnu/c++/11/bits/c++allocator.h \
 /usr/include/c++/11/ext/new_allocator.h \
 /usr/include/c++/11/bits/ostream_insert.h \
 /usr/include/c++/11/bits/cxxabi_forced.h \
 /usr/include/c++/11/bits/stl_function.h \
 /usr/include/c++/11/backward/binders.h \
 /usr/include/c++/11/bits/range_access.h \
 /usr/include/c++/11/initializer_list \
 /usr/include/c++/11/bits/basic_string.h \
 /usr/include/c++/11/ext/alloc_traits.h \
 /usr/include/c++/11/bits/alloc_traits.h \
 /usr/include/c++/11/bits/stl_construct.h /usr/include/c++/11/string_view \
 /usr/include/c++/11/bits/functional_hash.h \
 /usr/include/c++/11/bits/string_view.tcc \
 /usr/include/c++/11/ext/string_conversions.h /usr/include/c++/11/cstdlib \
 /usr/include/stdlib.h /usr/include/x86_64-linux-gnu/bits/waitflags.h \
 /usr/include/x86_64-linux-gnu/bits/waitstatus.h \
 /usr/include/x86_64-linux-gnu/sys/types.h /usr/include/endian.h \
 /usr/include/x86_64-linux-gnu/bits/byteswap.h \
 /usr/include/x86_64-linux-gnu/bits/uintn-identity.h \
 /usr/include/x86_64-linux-gnu/sys/select.h \
 /usr/include/x86_64-linux-gnu/bits/select.h \
 /usr/include/x86_64-linux-gnu/bits/types/sigset_t.h \
 /usr/include/alloca.h /usr/include/x86_64-linux-gnu/bits/stdlib-float.h \
 /usr/include/c++/11/bits/std_abs.h /usr/include/c++/11/cstdio \
 /usr/include/stdio.h /usr/include/x86_64-linux-gnu/bits/types/__fpos_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos64_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/cookie_io_functions_t.h \
 /usr/include/x86_64-linux-gnu/bits/stdio_lim.h \
 /usr/include/c++/11/cerrno /usr/include/errno.h \
 /usr/include/x86_64-linux-gnu/bits/errno.h /usr/include/linux/errno.h \
 /usr/include/x86_64-linux-gnu/asm/errno.h \
 /usr/include/asm-generic/errno.h /usr/include/asm-generic/errno-base.h \
 /usr/include/x86_64-linux-gnu/bits/types/error_t.h \
 /usr/include/c++/11/bits/charconv.h \
 /usr/include/c++/11/bits/basic_string.tcc \
 /usr/include/c++/11/bits/locale_classes.tcc \
 /usr/include/c++/11/system_error \
 /usr/include/x86_64-linux-gnu/c++/11/bits/error_constants.h \
 /usr/include/c++/11/stdexcept /usr/include/c++/11/streambuf \
 /usr/include/c++/11/bits/streambuf.tcc \
 /usr/include/c++/11/bits/basic_ios.h \
 /usr/include/c++/11/bits/locale_facets.h /usr/include/c++/11/cwctype \
 /usr/include/wctype.h /usr/include/x86_64-linux-gnu/bits/wctype-wchar.h \
 /usr/include/x86_64-linux-gnu/c++/11/bits/ctype_base.h \
 /usr/include/c++/11/bits/streambuf_iterator.h \
 /usr/include/x86_64-linux-gnu/c++/11/bits/ctype_inline.h \
 /usr/include/c++/11/bits/locale_facets.tcc \
 /usr/include/c++/11/bits/basic_ios.tcc \
 /usr/include/c++/11/bits/ostream.tcc /usr/include/c++/11/istream \
 /usr/include/c++/11/bits/istream.tcc
whitelies125@DESKTOP-H47PT8Q:~/code/makefile/src$
~~~

#### 关于 -MP

> This option instructs CPP to add a phony target for each dependency other than the main file, causing each to depend on nothing. These dummy rules work around errors `make` gives if you remove header files without updating the Makefile to match.
>
> 该选项指示 CPP 为每个依赖项添加不依赖其他任何项的 phony 目标。这个 rule 以应对，当你删除了头文件而没有更新 Mafkefile 去匹配新的依赖关系时将导致 make出现错误。

虽说 gcc 官方文档写的是 phont target，但实际上使用 -MP 生成的 target，在删除了相应头文件的情况下，在 make 的官方文档中应该属于 force  target。

参见：https://www.gnu.org/software/make/manual/html_node/Force-Targets.html

> If a rule has no prerequisites or recipe, and the target of the rule is a nonexistent file, then `make` imagines this target to have been updated whenever its rule is run. This implies that all targets depending on this one will always have their recipe run.
>
> 如果一个 rule 没有 prerequisites 和 commands，且该 rule 的 target 是一个不存在的文件，则 make 假定当该 rule 执行时，其 target 为最新。这隐含着，所有依赖于这个 target 的其他 target 都将执行它们的 commands。
>
> ~~~Makefile
> clean: FORCE
>         rm $(objects)
> FORCE:
> ~~~
>
> Here the target ‘FORCE’ satisfies the special conditions, so the target clean that depends on it is forced to run its recipe. There is nothing special about the name ‘FORCE’, but that is one name commonly used this way.
>
> 例如：此处 target FORCE 满足了这个特殊条件，因此依赖于 FORCE 的 target clean 将会执行它的 commands。至于 FORCE 这个 target 名并无特别含义，这里换用任何一个常见的名字都行。
>
> As you can see, using ‘FORCE’ this way has the same results as using ‘.PHONY: clean’.
>
> 如你所见，使用 FORCE 的这种方式，实际与使用 .PHONY: clean 效果一致。
>
> Using ‘.PHONY’ is more explicit and more efficient. However, other versions of `make` do not support ‘.PHONY’; thus ‘FORCE’ appears in many makefiles. See [Phony Targets](https://www.gnu.org/software/make/manual/html_node/Phony-Targets.html).
>
> .PHONY 更明确与高效，不过 make 的有些版本不支持 .PHONY，因此会在许多 makefile 中出现 FORCE。

使用 -MP 选项的用途在于以下场景：

源文件 a.cpp 起初依赖于头文件 a.h，某次修改后，该源文件不再依赖该头文件，且删除了头文件 a.h

随后执行 make，而此时依赖文件 .d 中的 rule 仍然是：a.o 依赖于 a.cpp a.h。但 a.h 已被删除，不存在，make 找不到 a.h 文件，于是 make 会报错，No rule to make target a.h。

而添加了 -MP 选项，则生成规则时就会生成无 prerequisite 和 commands 的 rule,a.h:。当出现上述场景，a.h 被删除时，则该 rule 就变为了 force rule，make 直接认为改 target 为最新，依赖 a.h 的 target 都会被更新。

而本次 make 执行，又会重新生成依赖文件 .d，只有 a.o 的 rule 也就变成了 a.o: a.cpp，不再依赖 a.h 了。

个人感觉，-MP 选项与上例中 include 依赖文件时最前面加上减号 -，其问题的本质，都是源于依赖文件是编译过程的副产物，本次 make 的执行使用的其实是上一次的依赖关系，需要本次编译完成才是最新的依赖关系。

这就导致了在文件 -MP 选项与 -include

例子：使用上面 Makefile Cookbook 的 makefile，其中 main.cpp include 了 test.h

~~~shell
code
|---Makefile
|---src
|   |---include
|	|	|---test.h
|	|---main.cpp
~~~

shell 结果：g++ -MMD -MP main.cpp，可以看见输出的依赖文件中，还生成了无 prerequisites 无 commands 的 src/include/test.h 目标。不加 -MP 则没有。

~~~shell
whitelies125@DESKTOP-H47PT8Q:~/code/makefile$ make
mkdir -p build/./src/
g++ -I./src -I./src/include -MMD -MP  -c src/main.cpp -o build/./src/main.cpp.o
g++ ./build/./src/main.cpp.o -o build/final_program
whitelies125@DESKTOP-H47PT8Q:~/code/makefile$ cat build/src/main.cpp.d
build/./src/main.cpp.o: src/main.cpp src/include/test.h
src/include/test.h:
whitelies125@DESKTOP-H47PT8Q:~/code/makefile$
~~~

此时，修改 main.cpp 不再 include test.h，并删除 test.h：

shell 结果：

~~~shell
whitelies125@DESKTOP-H47PT8Q:~/code/makefile$ nvim
whitelies125@DESKTOP-H47PT8Q:~/code/makefile$ rm src/include/test.h
whitelies125@DESKTOP-H47PT8Q:~/code/makefile$ make
mkdir -p build/./src/
g++ -I./src -I./src/include -MMD -MP  -c src/main.cpp -o build/./src/main.cpp.o
g++ ./build/./src/main.cpp.o -o build/final_program
whitelies125@DESKTOP-H47PT8Q:~/code/makefile$ cat build/src/main.cpp.d
build/./src/main.cpp.o: src/main.cpp
whitelies125@DESKTOP-H47PT8Q:~/code/makefile$
~~~

可见，make 并没有报错。依赖文件也正常更新，去除 build/./src/main.cpp.o 对 src/include/test.h 的依赖。

与前文分析一致，由于使用了 force target，此时 make 认为 build/./src/main.cpp.o 依赖于 src/include/test.h ，根据 make 的执行逻辑，发现 src/include/test.h 不存在，则 make 会根据 rule 生成 src/include/test.h，但因为 src/include/test.h 是 force target 因此并不会因为未找到生成 src/include/test.h 的 rule 而报错，而是 make 直接认为 src/include/test.h 已为最新，所有依赖 src/include/test.h 的 target 都需要更新。

下面对比如果不使用 -MP 选项。

从头开始：

main.cpp include 了 test.h

~~~shell
code
|---Makefile
|---src
|   |---include
|	|	|---test.h
|	|---main.cpp
~~~

shell 结果：

~~~shell
whitelies125@DESKTOP-H47PT8Q:~/code/makefile/src$ ls
include  main.cpp  test.h
whitelies125@DESKTOP-H47PT8Q:~/code/makefile/src$ g++ -MMD main.cpp
whitelies125@DESKTOP-H47PT8Q:~/code/makefile/src$ ls
a-main.d  a.out  include  main.cpp  test.h
whitelies125@DESKTOP-H47PT8Q:~/code/makefile/src$ cat a-main.d
main.o: main.cpp test.h
whitelies125@DESKTOP-H47PT8Q:~/code/makefile/src$
~~~

make 执行正常，生成的依赖文件 .d 中，没有 src/include/test.h 这个 target。

此时，修改 main.cpp 不再 include test.h，并删除 test.h：

shell 结果：

~~~Makefile
whitelies125@DESKTOP-H47PT8Q:~/code/makefile$ nvim
whitelies125@DESKTOP-H47PT8Q:~/code/makefile$ cat src/main.cpp
// #include "test.h"

int main() {
    // print();
    return 0;
}
whitelies125@DESKTOP-H47PT8Q:~/code/makefile$ rm src/include/test.h
whitelies125@DESKTOP-H47PT8Q:~/code/makefile$ make
make: *** No rule to make target 'src/include/test.h', needed by 'build/./src/main.cpp.o'.  Stop.
whitelies125@DESKTOP-H47PT8Q:~/code/makefile$
~~~

make 执行失败，报错没有生成 src/include/test.h 的 rule。

与前文分析一致，由于没有使用 force target，此时 make 认为 build/./src/main.cpp.o 依赖于 src/include/test.h ，根据 make 的执行逻辑，发现 src/include/test.h 不存在，则 make 会根据 rule 生成 src/include/test.h，此时未找到生成 src/include/test.h 的 rule，因此报错。

2024.06.05
