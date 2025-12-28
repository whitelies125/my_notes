transformation 变换

2D transformation
- scale 缩放
- reflection 翻转
- shear 剪切
- rotate 旋转，默认为绕原点逆时针方向

Homogeneous coordinates 齐次坐标
- translation 平移

向量/矩阵多引入一个维度 $(x,y,w)^T$，使得 translation 与其他 2D transformation 可以使用同一表达式进行表达、计算。
$$
\begin{aligned}
2D point &= (x,y,w)^T = (x/w,y/w,1)^T \\
2D vector &= (x,y,0)^T
\end{aligned}
$$
2025.07.09