using namespace std;
#include "ForwardOperationNode.h"
#include "PoolNode.h"
#include "Pool.h"
//处理类A包含类B，而类B包含类A的好方法是在A里用*B代替B，在B里用*A代替A，并且把A里涉及到B的函数和B里涉及到A的函数在类外定义（类内声明）
//使用指针的原因是C++要求类内成员必须是有明确大小的，而指针是有明确大小的
//使用类外定义的原因是如果在A类的函数涉及到B的成员操作时，此时B是为定义的，会出问题，然而把函数实现滞后于A、B定义之后，A、B都是定义好了的。

int main()
{	/*
	SqList s;
	int a;
	double dur;
	for (int i = 0; i < 100000; i++)
	{
		SqListItem* t1 = new SqListItem(i);
		s.Add(t1);
	}
	auto start = chrono::system_clock::now();
	auto end = chrono::system_clock::now();
	auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
	for (int i = 0; i < 100000; i++)
	{	start= chrono::system_clock::now();
		a = s[i]->gay;
		end = chrono::system_clock::now();
		duration = chrono::duration_cast<chrono::microseconds>(end - start);
		dur = double(duration.count()) * chrono::microseconds::period::num / chrono::microseconds::period::den;
		std::cout << a<< "--->"<<dur<<std::endl;
	}
	*/
	/*
	Pool p;
	Pool p2;
	ForwardOperationNode a1(2,&p);
	ForwardOperationNode a2(6,&p);
	ForwardOperationNode a3(3, &p);
	ForwardOperationNode a4(2, &p);
	a2 = ForwardOperationNode::exp(a1);
	a3=  (ForwardOperationNode::exp(a1)+6)*5;
	a4 = a2 + a3;
	float d;
	ForwardOperationNode::GradientBack(&a4,&a1,d);
	std::cout << a4.data<<std::endl;
	std::cout << d<<std::endl;
	a1 = 3;
	std::cout << a4.data << std::endl;
	ForwardOperationNode::GradientBack(&a4, &a1, d);
	std::cout << d;
	*/
	
	return 0;
}


// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
