# Partial_Derivative
Find partial derivative<br>
一个简单的求偏导数的程序，在EasyC++.cpp中调用相关函数和类。<br>
请使用ForwardOperationNode来创建基本的运算变量。<br>
程序将数值量分为自变量，因变量和常量三种，其中任意初始化的ForwardOperationNode对象都是自变量，自变量可以重复赋值。<br>
一旦对一个自变量进行了表达式赋值就将其标记为因变量，因变量一旦确定就不可以再更改其表达式。<br>
自变量、因变量将会存储在一个计算图中，而常量将不会存储在计算图中。<br>
为了方便管理，所有变量运算应当在一个变量池中，不同变量池的变量之间不可以产生任何关联。<br>
本程序提供基本的加减乘除、取反以及反三角函数、三角函数(弧度制)、对数函数、指数函数、幂函数。<br>
以下是一个例子：<br>
Pool p;<br>
Pool p1;<br>
ForwardOperationNode X(5,&p);//自变量<br>
ForwardOperationNode Y(6,&p);//自变量<br>
ForwardOperationNode Z(7,&p1)//自变量<br>
Y=X+5;//Y变为因变量，而X仍为自变量<br>
X=5;//允许，因为此时X为自变量<br>
Y=5;//不允许，因为此时Y为自变量，此时尝试修改Y的表达式为Y=5<br>
Y=X+Z;//不允许，因为X与Z不在同一个变量池<br>
float d;<br>
ForwardOperationNode::GradientBack(&Y,&X,d);//尝试寻找DY/DX，并将其赋给d
