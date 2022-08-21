# 641. Design Circular Deque

类似题目：[622_Design_Circular_Queue](622_Design_Circular_Queue.md)

~~~C++
class MyCircularDeque {
private:
    int* arr;
    int size;
    int head;
    int tail;
public:
    MyCircularDeque(int k) {
        size = k+1;
        arr = (int*)malloc( size * sizeof(int) );
        memset(arr, 0, size);
        head = 0;
        tail = 0;
    }
    
    bool insertFront(int value) {
        if(isFull()) return false;
        if(head == 0) head = size-1;
        else -- head;
        arr[head] = value;
        return true;
    }
    
    bool insertLast(int value) {
        if(isFull()) return false;
        arr[tail] = value;
        tail = (tail+1)%size;
        return true;
    }
    
    bool deleteFront() {
        if(isEmpty()) return false;
        head = (head+1)%size;
        return true;
    }
    
    bool deleteLast() {
        if(isEmpty()) return false;
        if(tail == 0) tail = size-1;
        else -- tail;
        return true;
    }
    
    int getFront() {
        if(isEmpty()) return -1;
        return arr[head];
    }
    
    int getRear() {
        if(isEmpty()) return -1;
        if(tail == 0) return arr[size-1];
        else return arr[tail-1];
    }
    
    bool isEmpty() {
        return head == tail;
    }
    
    bool isFull() {
        return (tail + 1) % size == head;
    }
};

/**
 * Your MyCircularDeque object will be instantiated and called as such:
 * MyCircularDeque* obj = new MyCircularDeque(k);
 * bool param_1 = obj->insertFront(value);
 * bool param_2 = obj->insertLast(value);
 * bool param_3 = obj->deleteFront();
 * bool param_4 = obj->deleteLast();
 * int param_5 = obj->getFront();
 * int param_6 = obj->getRear();
 * bool param_7 = obj->isEmpty();
 * bool param_8 = obj->isFull();
 */
~~~

2022.08.15