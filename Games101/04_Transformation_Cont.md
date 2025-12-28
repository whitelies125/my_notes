3D transformation

Viewing transformation 观测变换
- View / Camera transformation 视图变换
- Project transformation 投影变换
	- Orthographic projection 正交投影
	- Perspective projection 透视投影

### View / Camera transformation 视图变换
变化来源于相对运动，为了方便，我们总是认为摄像机不会运动，总是其他物体在运动。
即总是从摄像机的视角观察空间。

假设在我们当前的标准正交基张成的空间 $A$ 中，相机位于 $\vec{e}$ ，面朝 $\vec{g}$，头顶方向 $\vec{t}$ 。

那么对空间 $A$ 进行一个变换，使得摄像机由当前位置移动至原点，且调整面朝为 -Z 方向，头顶方向为 +Y 方向。
因为是对整个空间 $A$ 的变换，而非仅摄像机自身，空间 $A$ 内所有坐标也随之变换，因此变换后也就得到了摄像机视角下的空间 $B$ 。

个人看法，这一步其实是在进行一个坐标转换，将我们用于描述空间的基失由 $\vec{x},\vec{y},\vec{z}$ 转换为相机的基失 $\vec{x}\rightarrow\vec{g}\times\vec{t},\vec{y}\rightarrow\vec{t},\vec{z}\rightarrow\vec{g}$ ，也就得到物体在空间 $B$ 基失下的坐标。

假设完成这个 view transformation 的矩阵为 $M_{view}$，则：

$$
\begin{aligned}M_{view}&=R_{view}T_{view} \\&=R_{view}\begin{bmatrix}&1&0&0&-x_e\\&0&1&0&-y_e\\&0&0&1&-z_e\\&0&0&0&1\end{bmatrix}\end{aligned}
$$
其中：
$$
T_{view}=\begin{bmatrix}&1&0&0&-x_e\\&0&1&0&-y_e\\&0&0&1&-z_e\\&0&0&0&1\end{bmatrix}
$$
完成平移变换。
$$
R_{view}=
\begin{bmatrix}
&x_{\vec{g}\times\vec{t}}&y_{\vec{g}\times\vec{t}}&z_{\vec{g}\times\vec{t}}&0
\\&x_{t}&y_{t}&z_{t}&0
\\&x_{-g}&y_{-g}&z_{-g}&0
\\&0&0&0&1
\end{bmatrix}
$$
完成旋转变换。
$R_{view}$ 的简单求法，先写出基失向摄像机基失转换的矩阵，再求逆可得。
## Project transformation 投影变换
投影变换，完成3D空间向2D图像的变换。

上一步 view transformation 使得我们可以从摄像机的视角（以摄像机为原点）去观察到空间中的物体位置。

但并非空间中所有物体都需要进行计算、显示。
因此我们可以选定一个 3D 空间范围，只对这个空间范围内进行处理。

3D 空间的靠近摄像机的面为近平面，远处为远平面。
把所有物体投影到近平面，3D空间变为2D平面，到这一步，这里近平面上的3D空间投影，已经接近于我们最终要显示在屏幕上的画面了。

对3D空间范围形状的选择不同，也就有了下面正交投影、头饰投影之分。
正交投影，选择的3D空间为长方体。
透视投影，选择的3D空间为四棱台。

当然，选中的 3D 空间范围可能是任意位置。
为了方便，我们仍然可以将其移动到原点进行处理，毕竟无相对运动，那就无所谓。
### Orthographic projection 正交投影
正交投影，选择的3D空间为长方体。

1. 移动到原点（平移变换）
2. 标准化为 $[-1.1]^3$ 的标准立方体
   因为选择的 3D空间范围本身就是长方体，因此就是简单的缩放。
### Perspective projection 透视投影
透视投影，选择的3D空间为四棱台。

1. 移动到原点（平移变换）
2. 变为为长方体
   对近平面与远平面之间的每一个面，都缩放为近平面相同尺寸，从而变为长方体。
3. 标准化为 $[-1.1]^3$ 的标准立方体
   经过第二步已经是长方体了，也是简单的缩放。

2025.08.03