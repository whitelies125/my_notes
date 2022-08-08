# Design Circular Queue

~~~C++
class MyCircularQueue {
private:
    int * vec;
    int head;
    int tail;
    int size;

public:
    MyCircularQueue(int k) {
        vec = new int[k+1];
        head = 0;
        tail = 0;
        size = k+1;
    }
    
    bool enQueue(int value) {
        if(isFull()) return false;
        vec[tail] = value;
        tail = (tail+1) % size;
        return true;
    }
    
    bool deQueue() {
        if(isEmpty()) return false;
        head = (head+1) % size;
        return true;
    }
    
    int Front() {
        if(isEmpty()) return -1;
        return vec[head];
    }
    
    int Rear() {
        if(isEmpty()) return -1;
        if(tail) return vec[tail-1];
        return vec[size-1];
    }
    
    bool isEmpty() {
        return head == tail ? 1 : 0;
    }
    
    bool isFull() {
        return (tail+1) % size == head ? true : false;
    }
};
~~~

