class Bitmap {};
class Widget {
public:
    Bitmap* pb; // 指向一个 heap 对象
	Widget& operator=(const Widget& rhs) {
        delete pb; // 先释放当前 heap 对象
        pb = new Bitmap(*rhs.pb); // 使用 rhs 的拷贝。error: 若为 assignment 是自身时，rhs.pb 指向的 Bitmap 对象在上一行代码已释放
        return *this; // 见条款10
	}
};

int main() {
    Widget w;
    w.pb = new Bitmap;
    w = w;
    return 0;
}
