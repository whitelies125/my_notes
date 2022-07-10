# 控制反转与MVC模式

## 内部类

定义在一个类中的类，称为内部类。
相对的，前者称为外部类。

一个最简单的内部类的例子：

```java
public class A{// A 为外部类
	...
	public class B{// B 为内部类
		...
	}
	...
}
```

显然，B 虽然是类，但也是 A 的成员，所以仍然遵循类的成员的规则。
比如，内部类 B 仍然可以直接访问 外部类 A 中的成员，包括私有。

内部类根据其所出现位置与作用不同，可分为：成员内部类，局部内部类，静态内部类，匿名内部类。

### 成员内部类

定义在一个类中，作为该类的成员。

刚才的简单例子就是成员内部类：内部类 B 是作为外部类 A 的成员存在。

```java
public class A{// A 为外部类
	...
	public class B{// B 为内部类，成员内部类，作为 A 的成员
		...
	}
	...
}
```

成员内部类中可以直接访问外部类的成员，包括 private。

创建成员内部类对象：

外部类 A 要使用内部类 B 里的 B 的成员，则需创建对象，毕竟只有先创建了类的对象，才真有东西；
而内部类 B 是 外部类 A 的成员，所以需要内部类 B 是依赖与外部类 A 的存在的，
即，无法单独的创建内部类 B 对象，得先创建外部类 A 对象，才能创建内部类 B 对象。

```java
package test;
public class Test {
	public class Person{
		private int age = 0;
		public Person(int age) {
			this.age = age;
		}
		public class Child{
			void eat() {
				System.out.println("age = "+age+":child");
			}
		}
	}
	public static void main(String[] args) {
    	Test t = new Test();//先创建 Test 对象
    	Test.Person p = t.new Person( 6 );//有了 Test 对象，再创建 Person 对象
    	Test.Person.Child c = p.new Child(); //有了 Person 对象，再创建 Child 对象
		c.eat();//输出结果：age = 6:child
	}
}
```

如果内部类的成员与外部类的成员重名，则在内部类中使用该重名，访问的是内部类的成员。如果要在内部类中访问外部类的成员，则使用 外部类.this.成员名，来访问。
（就是最近花括号嘛。）

比如：

```java
package test;
public class Test {
	private int age = 99;
	public class Person{
		private int age = 0;
		public void output(){
            System.out.println("age = "+age);
			System.out.println("this.age = "+this.age);
			System.out.println("Test.this.age = "+Test.this.age);
		}
	}
	public static void main(String[] args) {
    	Test t = new Test();
    	Test.Person p = t.new Person();
		p.output();
        //输出结果为：
        //age = 0
        //this.age = 0
		//Test.this.age = 99
	}
}
```

内部类访问外部类成员还是简单的，外部类访问内部类就得需要先创建对象才行哈。

### 局部内部类

定义在一个类的成员函数或一个作用域内的内部类。

特点，对局部内部类的访问局限于该方法内或该作用域内。
（感觉就是花括号的原因，类似于 for 循环里的 int i，超出 for 循环的花括号就没了）

```java
public class A{
	public function func(){
		class B{
			...
		}
	}
}
```

对局部内部类的访问：

在外部类的成员函数内创建局部内部类对象，然后访问。

```java
package test;
public class Test {
	void test() {
		final int age = 99;//局部内部类只能访问 final 修饰的局部变量
		class Person{
			public void output(){
	            System.out.println("age = "+age);
			}
		}
		Person p = new Person();//仍然得先创建对象，再来访问类中的成员
		p.output();
	}
	public static void main(String[] args) {
    	Test t = new Test();
		t.test();
	}
}
```

类的成员函数中的变量是局部变量，所以根本就没有 public、private，...，这些可见度修饰词。
自然，局部内部类，地位等同于成员函数的局部变量，所以写个 class 就行了，不用也不能加其他可见度修饰词。

注意，对于局部变量类，如要访问同函数中的局部变量，则只能访问 final 修饰的局部变量。
因为，对于外部类的成员函数来说，当这个成员函数被调用时，才创建其中的局部变量，为其分配内存；当使用完该成员函数时，这些临时创建的局部变量就会被回收。（当然 final 的不会）

而创建了的在该成员函数中的局部内部类对象，并不会随着该成员函数的结束被回收。
那么，如果该成员函数中的局部内部类，中有访问该成员函数的局部变量的代码，那么在成员函数调用结束后，我使用这个代码就会有问题，因为临时的局部变量已经被回收、不存在了。

那么，调用该成员函数时，创建其中的局部内部类对象的时刻，将局部变量都复制一份给该对象呢？
这种方式的问题在于，无法保证数据一致性，如果创建时复制了一份，但之后对局部变量有修改，那么通过局部内部类对象访问的是创建时局部变量的值，而非修改后的值。

所以，final 修饰的的变量，是不会被修改的常量。
既然如此，就避免了数据不一致，因为不会修改 final 修饰的变量。

所以，局部内部类对象只能访问 final 修饰的局部变量。

### 静态内部类

定义在一个类中，作为该类的成员，有 static 修饰词。

与成员内部类的区别就是，有 static 修饰词。

```java
public class A{// A 为外部类
	...
	static public class B{// B 为静态内部类
		...
	}
	...
}
```

自然，也符合 static 修饰词的规则。

只能访问 static 修饰的成员。
不依赖外部类的存在的存在。

```java
package test;

public class Test {
	int a = 1;
	static int b = 2;
	static public class Ceshi{
		public void show() {
			//System.out.println("a = "+a);//错误，不能访问非 static
			System.out.println("b = "+b);//正确
		}
	public void aaa() {
		System.out.println("aaa");
	}
	}
	public static void main(String[] args) {
		Test.Ceshi c = new Test.Ceshi();//可直接创建 static 的静态内部类
		c.show();//输出结果:b = 2
	}
}
```

### 匿名内部类

没有名字的内部类，是内部类的简化写法。

本质上，是继承该类或接口的子类，只是没有特地的、专门的为这个类定义。自然也没有名字。
通常用于，下文的控制反转，或者使用次数很少、只是临时使用一次的类。

不使用内部类：

```java
package test;

abstract class Person {
	public abstract void eat();
}
class Child extends Person {
	@Override
    public void eat() {
		System.out.println("drink milk");
	}
}
public class Test {
	public static void main(String[] args) {
		Person p = new Child();//创建 Child 对象，向上造型，赋给 p
		p.eat();
        //调用 p.eat()，实际是调用的 Person 子类 Child 的 eat()
		//输出结果为：eat something
    }
}
```

使用匿名内部类的写法：

```java
package test;
abstract class Person {
	public abstract void eat();
}
public class Test {
	public static void main(String[] args) {
		Person p = new Person() {
			            @Override//试了下，发现这里加 @Override 也没错
						public void eat() {
							System.out.println("drink milk");
						}
					};
        //这里创建的是一个 Person() 的子类的对象。
        //这个子类，其中有这里写得成员函数 public void eat() {...}
        //至于子类的名字，无所谓啊，反正都向上造型赋给 p 了，用 p 来控制这个子类就行了。
		p.eat();
        //调用 p.eat()，实际是调用的该子类的 eat()
        //输出结果为：eat meat
	}
}
```

这个情况还能再简写：

```java
package test;
abstract class Person {
	public abstract void eat();
}
public class Test {
	public static void main(String[] args) {
		new Person(){
            public void eat() {
				System.out.println("drink milk");
			}
		}.eat();
        //本质仍是，创建一个 Person() 的不需要知道名字的、具有我们写的这个成员函数的子类，
		//调用 eat()。
	}
}
```

当然，不限于类，对于接口的实现也可以这么做。

```java
package test;
interface Person {
	public void eat();
}
public class Test {
	public static void main(String[] args) {
		Person p = new Person() {
					public void eat() {
						System.out.println("drink milk");
					}
		};
		p.eat();
	}
}
```

## 控制反转 Inversion of Control，IOC

控制反转，直接举例。

比如，Swing 里面的监听器：

```java
package test;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.JButton;
import javax.swing.JFrame;
public class Test {
	public static void main(String[] args) {
		JFrame jf = new JFrame();//创建窗口
		JButton jb = new JButton("test");//创建按钮
		jf.add(jb);//按钮加入窗口
		jb.addActionListener(new ActionListener() {//匿名内部类
            @Override//可以注意到，这里有继承
            public void actionPerformed(ActionEvent e) {//当点击按钮后，会调用的函数
                System.out.println("success");
            }
		});
		jf.setSize(800, 800);//设置窗口大小
		jf.setVisible(true);//显示窗口
	}
}
```

很容易注意到，我只是使用中 Swing 提供的函数，省去很多事。
但显然，每个人对于点击按钮后，要做什么，的需求是不一样的。
Swing 自然也不可能预知我们要干啥。
所以通过匿名内部类的方式，在 Swing 的代码中，仅仅只是写了点击按钮后会调用 actionPerformed() ，而其中的内容由我们自己来定义。
（本质就是 Swing 调用接口，我们来实现接口）

这就叫控制反转。

## MVC模式 Model-View-Controller

Model-View-Controller，模型-视图-控制器模式。

也就是分为三个层次：

V 表现：从模型获得数据，根据数据画出表示；当被通知数据修改了，则重新获取数据，重新画出表示。

C 控制：从用户获得输入，根据输入调整 M 模型的数据；

M 模型：保存和维护数据，提供接口让外部修改数据，数据修改后，通知 V 表现数据被修改，需要刷新。

可见，用户在界面 V 上的操作，并非由 V 的代码负责，而是由 C 的代码接收用户的操作，然后根据其操作，对 M 模型的数据进行修改，修改后，M 对 V 发出数据被修改的信息，V 从 M 重新获取数据，重新绘图。
V 与 C 不存在直接联系。

这里用  Swing 框架中的 JTable 为例说明 MVC 模式。

```java
package kcb;
import javax.swing.JFrame;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.WindowConstants;
public class Kcb {
	public static void main(String[] args) {
		JFrame jf = new JFrame();//创建窗口
		JTable jt = new JTable(new KcbData());//创建表格
		JScrollPane jsp = new JScrollPane(jt);
		//要用这个 JScrollPane 才能显示列名，现在不同，不过不是重点。
		jf.add(jsp);//窗口加入 jsp
		jf.pack();//自动计算窗口内容位置、大小
		jf.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
		//关闭窗口时结束程序。
		jf.setVisible(true);//显示窗口
	}
}
//以上为 Kcb.java（课程表）
package kcb;
import javax.swing.event.TableModelListener;
import javax.swing.table.TableModel;
public class KcbData implements TableModel {
	private String[] title = {
			"周一","周二","周三","周四","周五","周六","周日"
	};
	private String[][] data = new String[8][7];
	@Override
	public int getRowCount() {//行数
		return 8;
	}
	@Override
	public int getColumnCount() {//列数
		return 7;
	}
	@Override
	public String getColumnName(int columnIndex) {//获得列名
		return title[columnIndex];
	}
	@Override
	public Class<?> getColumnClass(int columnIndex) {//获得列的类型
		return String.class;
	}
	@Override
	public boolean isCellEditable(int rowIndex, int columnIndex) {
	//该位置元素是否可修改
		return true;
	}
	@Override
	public Object getValueAt(int rowIndex, int columnIndex) {//获得该位置元素内容。
		return data[rowIndex][columnIndex];
	}
	@Override
	public void setValueAt(Object aValue, int rowIndex, int columnIndex) {
	//修改该位置元素
		data[rowIndex][columnIndex] = (String)aValue;
	}
	@Override
	public void addTableModelListener(TableModelListener l) {
		// TODO Auto-generated method stub
	}
	@Override
	public void removeTableModelListener(TableModelListener l) {
		// TODO Auto-generated method stub
	}
}
//以上为 KcbData.java
```

这个程序是，一个显示出来的，可以修改的课程表。

显然的，可以注意到表格 JTable，只负责 V ，当需要数据时，则调用接口，从 M，TableModel 中获取。

显然的，JTable 就是 View，TableModel 就是 Model。
在这里，Controler 与 View 合并了，也是 JTable，这是 MVC 的常见的一种做法，因为得到用户的输入本身也在图形界面，所以负责 V 的同时顺带也用来 C了。

这就是 MVS 模式。

2021.01.15