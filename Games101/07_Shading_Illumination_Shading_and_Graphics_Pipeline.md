
光照，着色，图形，管道。

回顾：
1. 光栅化三角形
2. 采样理论
3. 反走样


## Visibility / occlusion 可见性
### Painter's Algorithm

画家算法
从远到近绘制物体，近处物体覆盖远处物体。

但是画家算法对于有些情况是无法解决的。

### Z-Buffer

深度缓冲



Shading
- Illumination & Shading
- Graphics Pipeline
## Shading 着色

在本课程中，着色指的是为物体应用材质。

### Blinn-Phong 着色模型
Blinn-Phong 着色模型主要有三部分组成：

- Diffuse（漫反射）：由物体本身的颜色决定
- Specular（高光）：由观察角度与光线反射角度共同决定，两者夹角越小，高光越明显
- Ambient（环境光）：由环境决定