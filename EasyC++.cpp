using namespace std;
#include "ForwardOperationNode.h"
#include "PoolNode.h"
#include "Pool.h"
//处理类A包含类B，而类B包含类A的好方法是在A里用*B代替B，在B里用*A代替A，并且把A里涉及到B的函数和B里涉及到A的函数在类外定义（类内声明）
//使用指针的原因是C++要求类内成员必须是有明确大小的，而指针是有明确大小的
//使用类外定义的原因是如果在A类的函数涉及到B的成员操作时，此时B是为定义的，会出问题，然而把函数实现滞后于A、B定义之后，A、B都是定义好了的。

int main()
{	
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
