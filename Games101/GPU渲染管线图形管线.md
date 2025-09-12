
以 OpenGL 为准：
https://www.khronos.org/opengl/wiki/Rendering_Pipeline_Overview#Pipeline

![[OpenGL_pipeline.png]]
### Vertex Specification
顶点指明

https://www.khronos.org/opengl/wiki/Vertex_Specification

>Vertex Specification is the process of setting up the necessary objects for rendering with a particular shader program, as well as the process of using those objects to render.
>vertex specification 是设置用于渲染的必要的对象、特殊 shader 程序的流程，与其他使用对象去渲染的流程一样

Submitting vertex data for rendering requires creating a stream of vertices, and then telling OpenGL how to interpret that stream.
提交用于渲染的 vertex data 要求创建一个 vertex stream，并且告知 OpenGL 如何解读这个 stream。

https://www.khronos.org/opengl/wiki/Primitive
例如：设置 VAO、VBO、EBO、顶点属性等，为 OpenGL 指明 vertex data 及其格式；设置 Primitive（注意此处与并非图元的含义），为 OpenGL 指明 vertex stream 渲染时代表什么的解释方案（点、线、三角形等）。


Primitive 有两种含义：
1. 确定顶点流在渲染时代表什么的解释方案，例如 GL_POINTS，GL_TRIANGLES
2. 也成为 base primitive 基本图元，指 primitive assembly 图元组装步骤中对 vertex stream 的解读结果。

### Vertex Processing 顶点处理
几乎都是可编程操作

#### Vertex shader 顶点着色器
必选

https://www.khronos.org/opengl/wiki/Vertex_Shader

从 vertex stream 中接收单个 vertex data，并处理生成单个输出顶点数据到 output vertex stream。
输入顶点和输出顶点之间必须是 1:1 的映射的。

顶点着色器通常执行到**投影后空间**的转换，以供顶点后处理阶段使用。
它们也可用于执行逐顶点计算光照，或为后续着色器阶段执行设置工作。

因为 vertex shader 限定了：
1. 一次只处理单个 vertex data
2. 输出与输出 1:1 映射
所以，对于同一个顶点着色器，相同的输入会得到相同的输出。
这就可以优化了，如果检测到本次输入与此前相同（往往是使用 EBO 时，发现顶点索引相同），那么就可以直接使用此前已计算出的结果，避免重复计算。
#### Tessellation 镶嵌
可选

曲面细分
在此阶段，vertex data 被细分为更小的图元  Primitive。

图元 Primitive ，就是几个关联的顶点数据+这些关联顶点表示的图像（点、线、三角形等）
例如：一个包含 12 个顶点的 vertex stream。它可以将顶点解释为三角形、点或线的序列。
它可以将 12 个顶点解释为 4 个独立三角形（将每 3 个顶点视为一个三角形），也可以解释为 10 个从属三角形（将流中每 3 个连续顶点组合成一个三角形）等。（设置为 GL_TRIANGLES 和 GL_TRIANGLE_STRIP 的区别）

#### Geometry shader 几何着色器
可选

GS 的编写目标是接受单个特定的图元类型作为输入，并输出零个或多个特定的图元类型。

几何着色器( GS) 是用GLSL编写的着色器程序，用于控制图元的处理。几何着色器位于顶点着色器（或可选的曲面细分阶段）和固定功能的顶点后处理阶段之间。

>https://learnopengl-cn.github.io/04%20Advanced%20OpenGL/09%20Geometry%20Shader/
>在 vertex shader 和 fragment shader 之间有一个可选的几何着色器(Geometry Shader)。
>几何着色器的输入是一个图元（如点或三角形）的一组顶点。几何着色器可以在顶点发送到下一着色器阶段之前对它们随意变换。
>几何着色器最有趣的地方在于，它能够将（这一组）顶点变换为完全不同的图元，并且还能生成比原来更多的顶点。

比如 geometry shader 沿法线方向移动输入的顶点坐标后输出，从而实现物体爆炸效果。

### Vertex post-processing 顶点后处理

#### Transform Feedback 变换反馈

变换反馈是捕获 Vertex Processing 顶点处理步骤生成的 Primitive 图元，并将这些 Primitive 的数据记录到 buffer object 缓冲区对象中的过程。
这允许我们保存对象变换后的渲染状态，并多次重新提交这些数据。

将 geometry shader 或 primitive assembly 的输出保存到 buffer object 缓冲对象。

大意就是，可以把到这一步的计算结果保存到 buffer object 里。
遇到一些需要迭代计算的，下次就可以使用这个 buffer object 里保存的数据进行计算。
#### Primitive Assembly 图元组装

https://www.khronos.org/opengl/wiki/Primitive_Assembly

图元组装是收集前几个阶段输出的一系列顶点数据，并将其组合成图元序列的过程。
用户渲染的图元类型决定了此过程的运作方式。

该过程的输出是简单图元（线、点或三角形）的有序序列。
例如，如果输入是一个包含 12 个顶点的三角形带图元，则该过程的输出将是 10 个三角形。

图元组装的发生
>The purpose of the primitive assembly step is to convert a vertex stream into a sequence of base primitives. For example, a primitive which is a line list of 12 vertices needs to generate 11 line base primitives.
>The full primitive assembly step (including the processing below) will happen during Vertex Post-Processing.
>However, some Vertex Processing steps require that a primitive be decomposed into a sequence of base primitives.
>For example, a Geometry Shader operates on each input base primitive in the primitive sequence. Therefore, a form of primitive assembly must happen before the GS can execute.
>Such early processing must happen if a Geometry Shader or Tessellation is active.
>图元组装步骤的目的是将 vertex stream 转换为一系列 base primitive。例如，一个包含12 个顶点的线列表图元需要生成 11 个线基本图元。
>完整的图元组装步骤（包括以下处理）将在 Vertex post-processing 期间进行。
>但是，某些 Vertex Processing 中的步骤需要将一个 primitive 分解为一系列 base primitive 。
>例如，Geometry Shader 会对 primitive sequence 中的每个输入基本图元进行操作。因此，在 GS 执行之前，必须先进行某种形式的图元组装。
>如果 GS 或曲面细分处于激活状态，则必须进行提早的 primitive assembly 处理。
#### clipping 裁剪

裁剪到裁剪空间
裁剪空间的范围定位为：$[-w,w]^3$ 

对点：丢弃
对线：完全超出，丢弃；部分超出，截断裁剪
对三角形：完成超出，丢弃；部分超出，以边缘为界裁剪，可能会生成新的三角形。

#### Perspective divide 透视除法
clip space 裁剪空间坐标转换为 normalized device coordinates (NDC)。
除以 w 分量。

$$
\begin{pmatrix}x_{ndc}\\y_{ndc}\\z_{ndc}\end{pmatrix}
=\begin{pmatrix}
\frac {x_{c}}{w_{c}}\\
\frac {y_{c}}{w_{c}}\\
\frac {z_{c}}{w_{c}}
\end{pmatrix}
$$

#### Viewport transform 视口转换
从 NDC 转换到屏幕空间，
其中x​ 、y​ 、width​ 、height​ 、nearVal​ 和farVal​ 是视口参数

$$
{\begin{pmatrix}x_{w}\\y_{w}\\z_{w}\end{pmatrix}}={\begin{pmatrix}{\begin{aligned}{\tfrac {width}{2}}x_{ndc}&+x+{\tfrac {width}{2}}\\{\tfrac {height}{2}}y_{ndc}&+y+{\tfrac {height}{2}}\\{\tfrac {farVal-nearVal}{2}}z_{ndc}&+{\tfrac {farVal+nearVal}{2}}\end{aligned}}\end{pmatrix}} 
$$
#### Face culling 面剔除

设置三角形正面的主要用途是允许剔除正面或背面的三角形。 设想一个立方体；它由 12 个三角形组成，但其中 6 个三角形朝向与另外 6 个相反。
除非立方体是透明的，否则其中 6 个三角形总会被另外 6 个三角形遮挡。
事实上，根据投影的不同，可能会遮挡超过 6 个三角形；想象一下从正面观察一个非常靠近相机的立方体。透视缩短意味着即使是侧面也背对着相机。 面剔除允许在昂贵的光栅化和片段着色器操作之前移除封闭表面的不可见三角形。

### 光栅化

到达此阶段的图元将按照给定的顺序进行光栅化。

光栅化是根据图元的样本覆盖范围， 将每个单独的图元分解为称为 fragment 片段的离散元素的过程。

图元光栅化的结果是一系列 fragment 片元。
fragment 是一组状态，用于计算输出 framebuffer 帧缓冲区中像素（或样本，如果启用了多重采样）的最终数据。
fragment 的状态包括其在屏幕空间中的位置、采样覆盖率（如果启用了多重采样 MSAA）以及从上一个顶点或几何着色器输出的任意数据列表。
最后一组数据是通过对片段顶点的数据值进行插值计算得出的。插值方式由输出这些值的着色器定义。

### fragment Processing 片元着色器

Fragment Shader 片元着色器是将光栅化生成的片段处理为一组颜色和单个深度值的着色器阶段。

### Per-Sample Processing
fragment Processor 输出的片段数据将经过一系列步骤。

#### 剪裁测试
剪裁测试是一种按样本处理操作，它会丢弃位于屏幕某个矩形部分之外的 fragment。

#### Multisample operations 多重采样操作

MSAA

#### Stencil test 模板测试
启用模板测试后，可以根据片段的模板值与存储在当前模板缓冲区中该片段采样位置的模板值之间的按位运算，丢弃该片段。这允许用户在一次渲染过程中设置模板值，然后根据此模式有条件地剔除片段。

#### Depth test 深度测试
启用深度测试后，将根据片段的深度值与片段采样位置处当前深度缓冲区中存储的深度值之间的条件测试来剔除片段。这有利于将几何体隐藏在其他几何体之后。
较近的几何体会使用适当的深度测试条件，设置深度值来掩盖其后方任何片段的渲染。

#### Blending 混合

片段中的每种颜色都可以与片段将要写入的缓冲区中对应的像素颜色进行混合。混合操作的结果就是最终写入的内容。

