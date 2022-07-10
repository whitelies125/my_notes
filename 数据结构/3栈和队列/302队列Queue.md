# 队列Queue

## 一. 队列的基本概念

## 1.1 队列的定义

队列（Queue）：只允许在表的一段进行插入，而在表的另一端进行删除的**线性表**。

入队、进队：向队列中插入元素。

出队、离队：删除元素。

### 1.2 队列的常见操作

~~~c
InitQueue(&Q)：初始化队列，构造一个空队列 Q。

QueueEmpty(Q)：判队列空，若队列 Q 为空返回 true，否则返回 false。

EnQueue(&Q,x)：入队，若队列 Q 未满，将 x 加入，使之称为新的队尾。

DeQueue(&Q,&x)：出队，若队列 Q 非空，删除队头元素，并用 x 返回。

GetHead(Q,&x)：读队头元素，若队列 Q 非空，则将队头元素赋值给 x。
~~~

## 二. 队列的存储结构

### 2.1 队列的顺序存储-SqQueue

#### 2.1.1 队列的顺序存储-SqQueue

~~~C
#define MaxSize 50
typedef struct{
    ElemType data[MaxSize];	//存放队列元素
    int front;				//队头指针
    int rear;				//队尾指针
} SqQueue;
~~~

#### 2.1.2 循环队列

注意一下判断循环队列队空、队满的条件。

非循环队列的判断队空条件 Q.rear == Q.front，在循环队列中，其队空、队满时的状态都满足，所以要稍加修改。

三种实现方式：

1. 牺牲一个单元来区分队空和队满。

   ~~~c
   队满：(Q.rear+1) % MaxSize == Q.front（牺牲了一个单元，最后一个单元是无法入队的）
   队空：Q.rear == Q.front（与非循环队列一样）
   队中元素个数：(Q.rear - Q.front + MaxSize) % MaxSize
   ~~~

2. 类型中增设表示元素个数的数据成员。

   ~~~c
   #define MaxSize 50
   typedef struct{
       ElemType data[MaxSize];	//存放队列元素
       int front;				//队头指针
       int rear;				//队尾指针
       int size;				//当前队列长度，每次入队加 1，出队减 1
   } SqQueue;
   
   队满：Q.size == 0
   队空：Q.size == MaxSize
   队中元素个数：Q.size
   ~~~

3. 类型中增设 tag 数据成员。

   ~~~c
   #define MaxSize 50
   typedef struct{
       ElemType data[MaxSize];	//存放队列元素
       int front;				//队头指针
       int rear;				//队尾指针
       int tag;				//记录最近进行的操作是入队还是出队，
       						//每次入队成功修改 tag = 1，出队成功修改 tag = 0
   } SqQueue;
   
   队满：tag == 1 && Q.rear == Q.front
   队空：tag == 0 && Q.rear == Q.front
   队中元素个数：(Q.rear - Q.front + MaxSize) % MaxSize
   ~~~

#### 2.1.3 循环队列的操作

......

### 2.2 队列的链式存储-链队列LiQueue

采用顺序存储的队列称为链队列。

（单链表）

~~~c
typedef struct{				//链式队列结点
    ElemType data;
    struct LinkNode *next;
}LinkNode;

typedef struct{
	LinkNode * front;		//队头指针
    LinkNode * rear;		//队尾指针
}LinkQueue;
~~~

#### 2.2.2 链式队列的基本操作

......

## 三. 双端队列

双端队列：允许两端都可以进行入队和出队操作的队列。

输出受限的双端队列：允许在一端进行插入和删除，但在另一端只允许插入的双端队列。

输入受限的双端队列：允许在一端进行插入和删除，但在另一端只允许删除的双端队列。

2021.07.27