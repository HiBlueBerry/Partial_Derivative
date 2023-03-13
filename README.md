# Partial_Derivative
Find partial derivative
一个简单的求偏导数的程序，在EasyC++.cpp中调用相关函数和类。
请使用ForwardOperationNode来创建基本的运算变量。
程序将数值量分为自变量，因变量和常量三种，其中任意初始化的ForwardOperationNode对象都是自变量，自变量可以重复赋值。
一旦对一个自变量进行了表达式赋值就将其标记为因变量，因变量一旦确定就不可以再更改其表达式。
自变量、因变量将会存储在一个计算图中，而常量将不会存储在计算图中。
为了方便管理，所有变量运算应当在一个变量池中，不同变量池的变量之间不可以产生任何关联。
本程序提供基本的加减乘除、取反以及反三角函数、三角函数(弧度制)、对数函数、指数函数、幂函数。
以下是一个例子：
Pool p;
Pool p1;
ForwardOperationNode X(5,&p);//自变量
ForwardOperationNode Y(6,&p);//自变量
ForwardOperationNode Z(7,&p1)//自变量
Y=X+5;//Y变为因变量，而X仍为自变量
X=5;//允许，因为此时X为自变量
Y=5;//不允许，因为此时Y为自变量，此时尝试修改Y的表达式为Y=5
Y=X+Z;//不允许，因为X与Z不在同一个变量池
float d;
ForwardOperationNode::GradientBack(&Y,&X,d);//尝试寻找DY/DX，并将其赋给d
