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

2024.06.05
