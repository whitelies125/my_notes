光栅化，抗锯齿，Z-buffering


Antialiasing 抗锯齿
- Sampling theory 采样理论
- Antialiasing in practice 抗锯齿练习
Visibility / occlusion 可见性/ 遮挡
- Z-buffering


### Blurring（Pre-Filtering）Before Sample

在采样前，先模糊

之前：平面->采样
改进：平面->模糊->采样

之前：平面->采样，每个像素点的亮度只来源于采样点本身
改进：平面->模糊->采样，每个像素点的亮度来源于周边点的"平均贡献"。

### MSAA

但是上面的 平面->模糊->采样，其实还是挺难的。

那还有另一种方法可以近似这种模糊操作：MSAA（超采样）

之前：在一个像素里面放 1 个采样点
MSAA：指在一个像素里面放多个采样点，来计算图像对这个像素的覆盖率。

MSAA 之后，可根据覆盖率（例如被覆盖的采样点数量）决定像素点亮度。

FXAA
TAA