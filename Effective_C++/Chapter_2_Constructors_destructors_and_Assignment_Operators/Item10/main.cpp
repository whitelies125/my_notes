#include <iostream>

class Widget {
public:
    int num_;

    Widget() : num_(0) {};
    Widget(int num) : num_(num) {}
	Widget& operator=(const Widget& rhs) {
        num_ = rhs.num_;
		return *this;
	}


	Widget& operator=(int num) {
        num_ = num;
		return *this;
	}
	Widget& operator+=(const Widget& rhs) {
        num_ += rhs.num_;
		return *this;
	}
};

int main() {
    int x, y, z;
    x = y = z = 15;
    std::cout << x << y << z << std::endl;

    x += y += z;
    std::cout << x << y << z << std::endl;

    Widget w1, w2, w3(1);
    std::cout << w1.num_ << w2.num_ << w3.num_ << std::endl;
    w1 = w2 = w3;
    std::cout << w1.num_ << w2.num_ << w3.num_ << std::endl;

    w3 = 2;
    w1 += w2 += w3;
    std::cout << w1.num_ << w2.num_ << w3.num_ << std::endl;
    return 0;
}
