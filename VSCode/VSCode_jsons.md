# VS Code jsons

## tasks.json

VS Code 的 tasks.json

对于单个 cpp 文件：

~~~json
{
    "tasks": [
        {
            "type": "cppbuild",
            
            //label 是任务列表里显示的名字，
			//可以改成自己喜欢的名字，方便从任务列表里来执行这个 task
            "label": "C/C++: g++.exe 生成活动文件",
            
            //command: 哪个程序来运行，这里是 g++.exe
            "command": "E:/mingw64/bin/g++.exe",

            //args,就是自己在终端调用 g++ 输入的命令，如 g++ -g test.cpp -o test.exe;
            "args": [
                "-fdiagnostics-color=always",
                "-g",
                "${file}",//${file} 表示当前激活的文件，比如我当前激活的是 test.cpp
                "-o",
                "${fileDirname}\\${fileBasenameNoExtension}.exe"//${fileDirname} 当前的工作目录;${fileBasenameNoExtension} 没有扩展名的文件名，也就是 test
            ],
            "options": {
                "cwd": "E:/mingw64/bin"
            },
            "problemMatcher": [
                "$gcc"
            ],
            "group": {
                "kind": "build",
                "isDefault": true
            },
            //detail 是任务列表中，任务名字下方对这个任务进行描述的小字。
            "detail": "调试器生成的任务。"
        }
    ],
    "version": "2.0.0"
}
~~~

对于多个 .cpp 文件，需要修改两个地方：

~~~json
{
    "tasks": [
        {
            "type": "cppbuild",
            "label": "C/C++: g++.exe 生成活动文件",
            "command": "E:/mingw64/bin/g++.exe",
            "args": [
                "-fdiagnostics-color=always",
                "-g",
                "${workspaceFolder}/*.cpp",//使用 ${workspaceFolder}/*.cpp 来编译 VS Code 当前工作目录下所有 .cpp 文件，而不再是 ${file}
                "-o",
                "${workspaceFolder}\\myProgram.exe"//这里也改一下，当前工作目录下输出最终的 .exe 文件。叫 myProgram，而不再是 ${fileDirname}\\${fileBasenameNoExtension}.exe
            ],
            "options": {
                "cwd": "E:/mingw64/bin"
            },
            "problemMatcher": [
                "$gcc"
            ],
            "group": {
                "kind": "build",
                "isDefault": true//标记为默认 tasks.json，当点击运行的箭头按钮时，会在 .vscode 文件夹中运行默认的 tasks.json，即 "isDefault": true 的 tasks.json
            },
            //如果 .vscode 中有多个默认 tasks.json,将其它的 "group": {...}, 改为：
            //"group": "build",
            
            "detail": "调试器生成的任务。"
        }
    ],
    "version": "2.0.0"
}
~~~

## launch.json

~~~json
{
    "version": "0.2.0",
    "configurations": [
      {
        "name": "C/C++: g++.exe 生成活动文件",
        "type": "cppdbg",
        "request": "launch",
         
        //program,指定你想要运行的 .exe 程序
        //这里设置为当前激活 .cpp 文件同名的 .exe 程序
        "program": "${fileDirname}\\${fileBasenameNoExtension}.exe",

		//args,内容为在 runtime 时传递给程序的参数
        "args": [],

        //若将其改为 true，则 debug 时会自动停在 main() 函数入口处
        "stopAtEntry": false,
        "cwd": "${fileDirname}",
        "environment": [],

        //为 flase，则使用 VS Code 中的终端；为 true，则新开一个 cmd 窗口
        "externalConsole": false,
        "MIMode": "gdb",

        //需要修改为自己的 gdb.exe 路径
        "miDebuggerPath": "E:\\mingw64\\bin\\gdb.exe",
        "setupCommands": [
          {
            "description": "Enable pretty-printing for gdb",
            "text": "-enable-pretty-printing",
            "ignoreFailures": true
          }
        ],
        //需要与 tasks.json 中的 label 一致，
        //作用是每次调用 launch 时会先调用 tasks.json，从而省事
        "preLaunchTask": "C/C++: g++.exe 生成活动文件"
      }
    ]
  }
~~~

