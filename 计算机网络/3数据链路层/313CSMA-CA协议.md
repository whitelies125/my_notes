# 313 CSMA/CA 协议

介质访问控制—动态划分信道—随机访问介质控制—CSMA/CA 协议。

载波监听多点接入/碰撞避免，CSMA/CA（Carrier Sense multiple Access with Collision Avoidance）

对比 CSMA/CD 协议，CSMA/CA 协议同样需要在发送数据前以及发送数据中都要监听信道。

使用条件，**CSMA/CD 协议用于以太网（有线局域网），而 CSMA/CA 协议用于无线局域网。**

这是因为无线局域网难以检测碰撞。在有线局域网中，计算机使用同一根总线，因此所有计算机都能监听到所有的信号，便于监听信道。
但无线局域网中，每台计算机的信号范围有限。则可能会出现**隐蔽站**问题，如站 B 在站 A 和站 C 的信号范围内，但 A、C 互相不在对方的信号范围内，则当 A 向 B 发送数据时，只能知道在 A 的信号范围内无其他计算机在向 B 发送数据，导致无论 C 是否正在或同时向 B 发送数据，由于 A 接收不到 C 的信号，判断 B 空闲，则会直接向 B 发送信号，导致 B 收到的信号错误。

So，CSMA/CA 协议就是为了解决这个问题。

## 一. CSMA/CA 协议工作原理

发送方发送数据前，先检测信道是否空闲。

若空闲，则发出 **RTS（request to send）**，RTS 包括发射端的地址、接收端的地址、下一份数据将持续发送的时间。
若信道忙，则等待。

接收方收到 RTS 后，将响应 **CTS（clear to send）**，即将 CTS 发送给发送方。
接收方响应后，接受方只会接收来自该 RTS 的正确的发送方的数据。

发送方收到 CTS 后，开始发送数据帧，
同时**预约信道**，即告知其他站点自己要传多久数据。

接收方接收正确的发送方的数据帧后，响应 **ACK 帧**。

发送方收到 ACK 后，就可以开始下一个数据帧的发送，若没有，则一直重传至规定重传次数为止（二进制指数退避算法）。

## 二. 对比 CSMA/CD 和 CSMA/CA 协议

相同点：
都是源于 CSMA 的思路，但 CSMA/CD 和 CSMA/CA 协议在发送数据前、发送数据中都需要监听信道。

不同点：

传输介质不同：
CSMA/CD 协议用于总线式以太网（有线），
CSMA/CA 协议用于无线局域网（无线）。

载波检测方式不同：
CSMA/CD 协议通过电缆中的电压变化来检测，
CSMA/CA 协议采用能量检测（ED），载波检测（CS）和能力载波混合检测三种检测信道空闲的方式。

CSMA/CD 协议是检测冲突，
CSMA/CA 协议是避免冲突。
当然，冲突后都会进行有上限次数的重传。

补充：
CSMA、CSMA/CD，都是自己能够检测出冲突的。所以接收方不用发回确认（不靠确认来判断冲突）。
CSMA/CA，需要通过由是否收到接收方发回的确认，来判断是否冲突（因为 A 只能知道 A 范围内的情况，只有 B 才知道 B 范围内的情况，也只有 A 收到 B 返回的确认，才能判定此时没有冲突）。

2021.02.04

