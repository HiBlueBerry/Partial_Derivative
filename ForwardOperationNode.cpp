#include"ForwardOperationNode.h"
//规定一：对于任意运算，其导数值存入在此运算中作为自变量的变量的对应的内存池节点中
//规定二：不允许运算时出现几个对象不属于同一个内存池的情况
//规定三：对应一个函数或过程而言，不应当存在一次以上的赋值机会

//创建自定义变量
ForwardOperationNode::ForwardOperationNode(float data, Pool* pool)
{	//赋值，确定指向哪个内存池
	this->data = data;
	this->to_pool = pool;
	//创建内存池节点，并为其配备内存池
	PoolNode* temp = new PoolNode();
	temp->to_pool = pool;
	//建立内存池节点和前向操作节点的连接
	temp->to_forward_operation_node = this;
	to_pool_node = temp;
	temp->value = data;
	//对应内存池把内存池节点加入
	pool->pool_node.push_back(temp);
}
//删除临时变量和自定义变量时把内存池中对应节点的连接断开
ForwardOperationNode::~ForwardOperationNode()
{	//如果有在内存池中有对应节点
	if (to_pool_node != nullptr)
	{	//析构时把连接断开
		to_pool_node->to_forward_operation_node = nullptr;
	}
}
//自变量赋值更新辅助函数
void ForwardOperationNode::VariableAssignHelp(PoolNode* temp)
{	
	for (int i = 0; i < temp->codomain.size(); i++)
	{
		switch (temp->tag[i])
		{
		case EQUAL:
			temp->codomain[i]->value = temp->helpNode[i]->value;
			break;
		case PLUS:
			if (temp->helpNode[i] != nullptr)
			{
				temp->codomain[i]->value = temp->value + temp->helpNode[i]->value;
			}
			else
			{
				temp->codomain[i]->value = temp->value + temp->help[i];
			}
			break;
		case MINUS_1:
			if (temp->helpNode[i] != nullptr)
			{
				temp->codomain[i]->value = temp->value - temp->helpNode[i]->value;
			}
			else
			{
				temp->codomain[i]->value = temp->value - temp->help[i];
			}
			break;
		case MINUS_2:
			if (temp->helpNode[i] != nullptr)
			{
				temp->codomain[i]->value = temp->helpNode[i]->value - temp->value;
			}
			else
			{
				temp->codomain[i]->value = temp->help[i] - temp->value;
			}break;
		case MULTIPLY:
			if (temp->helpNode[i] != nullptr)
			{
				temp->codomain[i]->value = temp->value * temp->helpNode[i]->value;
				temp->d[i] = temp->helpNode[i]->value;
			}
			else
			{
				temp->codomain[i]->value = temp->value * temp->help[i];
				temp->d[i] = temp->help[i];
			}
			break;
		case DIVIDE:
			if (temp->helpNode[i] != nullptr)
			{
				temp->codomain[i]->value = temp->helpNode[i]->value / temp->value;
				temp->d[i] = -temp->helpNode[i]->value / temp->value / temp->value;
			}
			else
			{
				temp->codomain[i]->value = temp->help[i] / temp->value;
				temp->d[i] = -temp->help[i] / temp->value / temp->value;
			}
			break;
		case DIVIDED:
			if (temp->helpNode[i] != nullptr)
			{
				temp->codomain[i]->value = temp->value / temp->helpNode[i]->value;
				temp->d[i] = 1.f / temp->helpNode[i]->value;
			}
			else
			{
				temp->codomain[i]->value = temp->value / temp->help[i];
				temp->d[i] = 1.f / temp->help[i];
			}
			break;
		case NEGATIVE:
			temp->codomain[i]->value = -temp->value;
			break;
		case EXP:
			temp->codomain[i]->value = std::pow(temp->help[i], temp->value);
			temp->d[i] = std::pow(temp->help[i], temp->value) * std::log(temp->help[i]);
			break;
		case POW:
			temp->codomain[i]->value = std::pow(temp->value, temp->help[i]);
			temp->d[i] = temp->help[i] * std::pow(temp->value, temp->help[i] - 1);
			break;
		case LOG:
			temp->codomain[i]->value = std::log(temp->value) / std::log(temp->help[i]);
			temp->d[i] = 1.f / (temp->value * std::log(temp->help[i]));
			break;
		case SIN:
			temp->codomain[i]->value = std::sin(temp->value);
			temp->d[i] = std::cos(temp->value);
			break;
		case COS:
			temp->codomain[i]->value = std::cos(temp->value);
			temp->d[i] = -std::sin(temp->value);
			break;
		case TAN:
			temp->codomain[i]->value = std::tan(temp->value);
			temp->d[i] = 1.f / std::cos(temp->value) / std::cos(temp->value);
			break;
		case ASIN:
			temp->codomain[i]->value = std::asin(temp->value);
			temp->d[i] = 1.f / (std::sqrt(1.f - temp->value * temp->value));
			break;
		case ACOS:
			temp->codomain[i]->value = std::asin(temp->value);
			temp->d[i] = -1.f / (std::sqrt(1.f - temp->value * temp->value));
			break;
		case ATAN:
			temp->codomain[i]->value = std::atan(temp->value);
			temp->d[i] = 1.f / (1 + temp->value * temp->value);
			break;

		}
		VariableAssignHelp(temp->codomain[i]);
	}
	if (temp->to_forward_operation_node != nullptr)
	{
		temp->to_forward_operation_node->data = temp->value;
	}
	else
	{
		std::cout << "null" << std::endl;
	}

}
//赋值，处理（临时+常量）形式
void ForwardOperationNode::operator =(float data)
{	//在一个过程中不应当尝试再次改变因变量的表达式
	if (to_pool_node != nullptr)
	{	if(to_pool_node->domain.size()!=0)
		{
			std::cerr << "在" << this << "处尝试改变其作为因变量的表达式，此次赋值无效" << std::endl;
			return;
		}
	}
	//左值必然是自定义变量，而自定义变量在声明时必然会在内存池中创建一个节点并建立连接
	this->data = data;
	to_pool_node->value = data;
	//临时常量必然没有在内存池中有节点与之对应,而且不可能被赋值
	//由于限定了变量仅能被赋值一次，对于这种形式不需要为常量开辟节点，开辟节点仅仅增加开销，而且导数为0，对求导数无实际作用
	//应当允许自变量重复赋值的机制
	VariableAssignHelp(to_pool_node);
}
//赋值，处理（临时/自定义+变量）形式
void ForwardOperationNode::operator =(const ForwardOperationNode& temp)
{	//在一个过程中不应当尝试再次改变因变量的表达式
	if (to_pool_node != nullptr)
	{
		if (to_pool_node->domain.size() != 0)
		{

			std::cerr << "在" << this << "处尝试改变其作为因变量的表达式，此次赋值无效" << std::endl;
			return;
		}
	}
	//右值必然是ForwardOperationNode结构的变量，而ForwardOperationNode声明时必然会在内存池中创建一个节点并建立连接
	//不允许两个变量的内存池不同
	if (to_pool != temp.to_pool)
	{
		std::cerr << this << "和" << &temp << "变量的内存池不同"<<std::endl;
		return;
	}
	data = temp.data;
	to_pool_node->value = data;
	//为左值的定义域中传入右值节点
	to_pool_node->domain.push_back(temp.to_pool_node);
	//为右值的陪域中传入左值节点
	temp.to_pool_node->codomain.push_back(to_pool_node);
	//为右值节点的导数集中传入本次导数
	temp.to_pool_node->d.push_back(1.f);
	//为右值节点的标签集中传入本次操作的标签
	temp.to_pool_node->tag.push_back(EQUAL);
	temp.to_pool_node->helpNode.push_back(temp.to_pool_node);
	temp.to_pool_node->help.push_back(0.0f);

}
//加法,处理变量+变量
ForwardOperationNode ForwardOperationNode::operator +(const ForwardOperationNode& another)
{
	//不允许两个变量的内存池不同
	if (to_pool != another.to_pool)
	{
		std::cerr << this << "和" << &another << "变量的内存池不同,将默认构建与左侧加量指向相同内存池的节点" << std::endl;
		return *this + ForwardOperationNode(another.data,to_pool);
	}
	//新建一个节点，作为新产生的和的节点
	ForwardOperationNode temp(data+another.data,to_pool);
	//建立和与两个加量的连接关系
	to_pool_node->codomain.push_back(temp.to_pool_node);
	another.to_pool_node->codomain.push_back(temp.to_pool_node);
	temp.to_pool_node->domain.push_back(to_pool_node);
	temp.to_pool_node->domain.push_back(another.to_pool_node);
	temp.to_pool_node->value += data + another.data;
	//确定和对两个加法量的偏导数,分别存入两个加法量(因为它们在这个运算中作为了自变量)中
	to_pool_node->d.push_back(1.f);
	another.to_pool_node->d.push_back(1.f);
	//为右值节点的标签集中传入本次操作的标签
	//to_pool_node->t.push_back(PoolNode::ce());
	to_pool_node->tag.push_back(PLUS);
	to_pool_node->helpNode.push_back(another.to_pool_node);
	to_pool_node->help.push_back(0.0f);
	another.to_pool_node->tag.push_back(PLUS);
	another.to_pool_node->helpNode.push_back(to_pool_node);
	another.to_pool_node->help.push_back(0.0f);
	return temp;

}
//加法，处理变量+常量
ForwardOperationNode ForwardOperationNode::operator +(float temp) const
{	
	//新建一个节点，作为新产生的和的节点
	ForwardOperationNode tempNode(data + temp, to_pool);
	//建立和与加量的连接关系
	to_pool_node->codomain.push_back(tempNode.to_pool_node);
	tempNode.to_pool_node->domain.push_back(to_pool_node);
	tempNode.to_pool_node->value = data + temp;
	//确定和对加法量的导数,存入加法量(因为它在这个运算中作为了自变量)中
	to_pool_node->d.push_back(1.f);
	//为右值节点的标签集中传入本次操作的标签
	to_pool_node->tag.push_back(PLUS);
	to_pool_node->helpNode.push_back(nullptr);
	to_pool_node->help.push_back(temp);
	
	return tempNode;
}
//加法，处理常量+变量
ForwardOperationNode operator +(float temp, const ForwardOperationNode& another)
{
	return another + temp;
}
//减法，处理变量-变量
ForwardOperationNode ForwardOperationNode::operator -(const ForwardOperationNode& another)
{
	//不允许两个变量的内存池不同
	if (to_pool != another.to_pool)
	{
		std::cerr << this << "和" << &another << "变量的内存池不同,将默认构建与左侧加量指向相同内存池的节点" << std::endl;
		return *this - ForwardOperationNode(another.data, to_pool);
	}
	//新建一个节点，作为新产生的差的节点
	ForwardOperationNode temp(data - another.data, to_pool);
	//建立和与两个减量的连接关系
	to_pool_node->codomain.push_back(temp.to_pool_node);
	another.to_pool_node->codomain.push_back(temp.to_pool_node);
	temp.to_pool_node->domain.push_back(to_pool_node);
	temp.to_pool_node->domain.push_back(another.to_pool_node);
	temp.to_pool_node->value = data - another.data;
	//确定和对两个减法量的偏导数,分别存入两个减法量(因为它们在这个运算中作为了自变量)中
	to_pool_node->d.push_back(1.f);
	another.to_pool_node->d.push_back(-1.f);
	//为右值节点的标签集中传入本次操作的标签
	to_pool_node->tag.push_back(MINUS_1);
	temp.to_pool_node->helpNode.push_back(another.to_pool_node);
	temp.to_pool_node->help.push_back(0.0f);
	another.to_pool_node->tag.push_back(MINUS_2);
	another.to_pool_node->helpNode.push_back(to_pool_node);
	another.to_pool_node->help.push_back(0.0f);
	return temp;

}
//减法，处理变量-常量
ForwardOperationNode ForwardOperationNode::operator -(float temp) const
{
	//新建一个节点，作为新产生的差的节点
	ForwardOperationNode tempNode(data - temp, to_pool);
	//建立差与减量的连接关系
	to_pool_node->codomain.push_back(tempNode.to_pool_node);
	tempNode.to_pool_node->domain.push_back(to_pool_node);
	tempNode.to_pool_node->value = data - temp;
	//确定和对减法量的导数,存入减法量(因为它在这个运算中作为了自变量)中
	to_pool_node->d.push_back(1.f);
	//为右值节点的标签集中传入本次操作的标签
	to_pool_node->tag.push_back(MINUS_1);
	to_pool_node->helpNode.push_back(nullptr);
	to_pool_node->help.push_back(temp);
	return tempNode;
}
//减法，处理常量-变量
ForwardOperationNode operator -(float temp, const ForwardOperationNode& another) 
{
	//新建一个节点，作为新产生的差的节点
	ForwardOperationNode tempNode(temp-another.data, another.to_pool);
	//建立差与减量的连接关系
	another.to_pool_node->codomain.push_back(tempNode.to_pool_node);
	tempNode.to_pool_node->domain.push_back(another.to_pool_node);
	tempNode.to_pool_node->value = temp - another.data;
	//确定和对减法量的导数,存入减法量(因为它在这个运算中作为了自变量)中
	another.to_pool_node->d.push_back(-1.f);
	//为右值节点的标签集中传入本次操作的标签
	another.to_pool_node->tag.push_back(MINUS_2);
	another.to_pool_node->helpNode.push_back(nullptr);
	another.to_pool_node->help.push_back(temp);
	return tempNode;
}
//取反
ForwardOperationNode ForwardOperationNode::operator -()
{
	//新建一个节点，作为新产生的积的节点
	ForwardOperationNode tempNode(data * -1.f, to_pool);
	//建立差与乘量的连接关系
	to_pool_node->codomain.push_back(tempNode.to_pool_node);
	tempNode.to_pool_node->domain.push_back(to_pool_node);
	tempNode.to_pool_node->value = -data;
	//确定和对乘法量的导数,存入乘法量(因为它在这个运算中作为了自变量)中
	to_pool_node->d.push_back(-1.f);
	//为右值节点的标签集中传入本次操作的标签
	to_pool_node->tag.push_back(NEGATIVE);
	to_pool_node->helpNode.push_back(nullptr);
	to_pool_node->help.push_back(-1.0f);
	return tempNode;
}
//乘法，处理变量*变量
ForwardOperationNode ForwardOperationNode::operator *(const ForwardOperationNode& another)
{
	//不允许两个变量的内存池不同
	if (to_pool != another.to_pool)
	{
		std::cerr << this << "和" << &another << "变量的内存池不同,将默认构建与左侧加量指向相同内存池的节点" << std::endl;
		return *this * ForwardOperationNode(another.data, to_pool);
	}
	//新建一个节点，作为新产生的积的节点
	ForwardOperationNode temp(data * another.data, to_pool);
	//建立和与两个乘量的连接关系
	to_pool_node->codomain.push_back(temp.to_pool_node);
	another.to_pool_node->codomain.push_back(temp.to_pool_node);
	temp.to_pool_node->domain.push_back(to_pool_node);
	temp.to_pool_node->domain.push_back(another.to_pool_node);
	temp.to_pool_node->value = data * another.data;
	//确定和对两个乘法量的偏导数,分别存入两个乘法量(因为它们在这个运算中作为了自变量)中
	to_pool_node->d.push_back(another.data);
	another.to_pool_node->d.push_back(data);
	//为右值节点的标签集中传入本次操作的标签
	to_pool_node->tag.push_back(MULTIPLY);
	to_pool_node->helpNode.push_back(another.to_pool_node);
	to_pool_node->help.push_back(0.0f);
	another.to_pool_node->tag.push_back(MULTIPLY);
	another.to_pool_node->helpNode.push_back(to_pool_node);
	another.to_pool_node->help.push_back(0.0f);
	return temp;

}
//乘法，处理变量*常量
ForwardOperationNode ForwardOperationNode::operator *(float temp) const
{
	//新建一个节点，作为新产生的积的节点
	ForwardOperationNode tempNode(data * temp, to_pool);
	//建立差与乘量的连接关系
	to_pool_node->codomain.push_back(tempNode.to_pool_node);
	tempNode.to_pool_node->domain.push_back(to_pool_node);
	tempNode.to_pool_node->value = data * temp;
	//确定和对乘法量的导数,存入乘法量(因为它在这个运算中作为了自变量)中
	to_pool_node->d.push_back(temp);
	//为右值节点的标签集中传入本次操作的标签
	to_pool_node->tag.push_back(MULTIPLY);
	to_pool_node->helpNode.push_back(nullptr);
	to_pool_node->help.push_back(temp);
	return tempNode;
}
//乘法，处理常量*变量
ForwardOperationNode operator *(float temp, const ForwardOperationNode& another)
{
	return another * temp;
}
//除法，处理变量/变量
ForwardOperationNode ForwardOperationNode::operator /(const ForwardOperationNode& another)
{
	//不允许两个变量的内存池不同
	if (to_pool != another.to_pool)
	{
		std::cerr << this << "和" << &another << "变量的内存池不同,将默认构建与左侧加量指向相同内存池的节点" << std::endl;
		return *this / ForwardOperationNode(another.data, to_pool);
	}
	//除数不可以为0
	if (another.data == 0)
	{
		std::cerr << &another << "处的数值为0，然而尝试除以0，这是不允许的。"<<std::endl;
	}
	//新建一个节点，作为新产生的商的节点
	ForwardOperationNode temp(data / another.data, to_pool);
	//建立和与两个除法量的连接关系
	to_pool_node->codomain.push_back(temp.to_pool_node);
	another.to_pool_node->codomain.push_back(temp.to_pool_node);
	temp.to_pool_node->domain.push_back(to_pool_node);
	temp.to_pool_node->domain.push_back(another.to_pool_node);
	temp.to_pool_node->value = data / another.data;
	//确定和对两个除法量的偏导数,分别存入两个除法量(因为它们在这个运算中作为了自变量)中
	float d1;
	float d2;
	d1 = 1.f /another.data ;
	d2 = -1.f * data / another.data / another.data;
	to_pool_node->d.push_back(d1);
	another.to_pool_node->d.push_back(d2);
	//为右值节点的标签集中传入本次操作的标签
	to_pool_node->tag.push_back(DIVIDED);
	to_pool_node->helpNode.push_back(another.to_pool_node);
	to_pool_node->help.push_back(0.0f);
	another.to_pool_node->tag.push_back(DIVIDE);
	another.to_pool_node->helpNode.push_back(to_pool_node);
	another.to_pool_node->help.push_back(0.0f);
	return temp;
}
//除法，处理变量/常量
ForwardOperationNode ForwardOperationNode::operator /(float temp) const
{	//除数不可以为0
	if (temp == 0)
	{
		std::cerr << &temp << "处的数值为0，然而尝试除以0，这是不允许的。" << std::endl;
	}
	//新建一个节点，作为新产生的商的节点
	ForwardOperationNode tempNode(data / temp, to_pool);
	//建立商与被除量的连接关系
	to_pool_node->codomain.push_back(tempNode.to_pool_node);
	tempNode.to_pool_node->domain.push_back(to_pool_node);
	tempNode.to_pool_node->value = data / temp;
	//确定商对除法量的导数,存入被除量(因为它在这个运算中作为了自变量)中
	to_pool_node->d.push_back(1.f/temp);
	//为右值节点的标签集中传入本次操作的标签
	to_pool_node->tag.push_back(DIVIDED);
	to_pool_node->helpNode.push_back(nullptr);
	to_pool_node->help.push_back(temp);
	return tempNode;
}
//除法，处理常量/变量
ForwardOperationNode operator /(float temp, const ForwardOperationNode& another)
{	//除数不可以为0
	if (another.data == 0)
	{
		std::cerr << &another << "处的数值为0，然而尝试除以0，这是不允许的。" << std::endl;
	}
	//新建一个节点，作为新产生的和的节点
	ForwardOperationNode tempNode(temp / another.data, another.to_pool);
	//建立商与除量的连接关系
	another.to_pool_node->codomain.push_back(tempNode.to_pool_node);
	tempNode.to_pool_node->domain.push_back(another.to_pool_node);
	tempNode.to_pool_node->value = temp / another.data;
	//确定商对除法量的导数,存入除法量(因为它在这个运算中作为了自变量)中
	float d;
	d=-temp / another.data / another.data;
	another.to_pool_node->d.push_back(d);
	//为右值节点的标签集中传入本次操作的标签
	another.to_pool_node->tag.push_back(DIVIDED);
	another.to_pool_node->helpNode.push_back(nullptr);
	another.to_pool_node->help.push_back(temp);
	return tempNode;
}
//指数，参数：(次数(自变量),基底)
ForwardOperationNode ForwardOperationNode::exp(const ForwardOperationNode& exponent, float base)
{	//做运算
	float value = std::pow(base, exponent.data);
	ForwardOperationNode tempNode(value, exponent.to_pool);
	//确定连接关系
	exponent.to_pool_node->codomain.push_back(tempNode.to_pool_node);
	tempNode.to_pool_node->domain.push_back(exponent.to_pool_node);
	tempNode.to_pool_node->value = value;
	float d = value * std::log(base);
	//求导数
	exponent.to_pool_node->d.push_back(d);
	//为右值节点的标签集中传入本次操作的标签
	exponent.to_pool_node->tag.push_back(EXP);
	exponent.to_pool_node->helpNode.push_back(nullptr);
	exponent.to_pool_node->help.push_back(base);
	return tempNode;
}
//幂次，参数：(基底(自变量),次数)
ForwardOperationNode ForwardOperationNode::pow(const ForwardOperationNode& base, float deg)
{
	//做运算
	float value = std::pow(base.data, deg);
	ForwardOperationNode tempNode(value, base.to_pool);
	//确定连接关系
	base.to_pool_node->codomain.push_back(tempNode.to_pool_node);
	tempNode.to_pool_node->domain.push_back(base.to_pool_node);
	tempNode.to_pool_node->value = value;
	float d = deg * std::pow(base.data, deg - 1);
	//求导数
	base.to_pool_node->d.push_back(d);
	//为右值节点的标签集中传入本次操作的标签
	base.to_pool_node->tag.push_back(POW);
	base.to_pool_node->helpNode.push_back(nullptr);
	base.to_pool_node->help.push_back(deg);
	return tempNode;
}
//对数，参数：(幂(自变量),基底)
ForwardOperationNode ForwardOperationNode::log(const ForwardOperationNode& data, float base)
{
	if (base <= 0)
	{
		std::cerr << "基底不是大于0的数。" << std::endl;
	}
	if (data.data <= 0)
	{
		std::cerr << "自变量不是大于0的数。" << std::endl;
	}
	//做运算
	float value = std::log(data.data) / std::log(base);
	ForwardOperationNode tempNode(std::log(data.data) / std::log(base), data.to_pool);
	//确定连接关系
	data.to_pool_node->codomain.push_back(tempNode.to_pool_node);
	tempNode.to_pool_node->domain.push_back(data.to_pool_node);
	tempNode.to_pool_node->value = value;
	float d = 1.f / (data.data * std::log(base));
	//求导数
	data.to_pool_node->d.push_back(d);
	//为右值节点的标签集中传入本次操作的标签
	data.to_pool_node->tag.push_back(LOG);
	data.to_pool_node->helpNode.push_back(nullptr);
	data.to_pool_node->help.push_back(base);
	return tempNode;
}
//正弦，弧度制
ForwardOperationNode ForwardOperationNode::sin(const ForwardOperationNode& data)
{
	//做运算
	float value= std::sin(data.data);
	ForwardOperationNode tempNode(value, data.to_pool);
	//确定连接关系
	data.to_pool_node->codomain.push_back(tempNode.to_pool_node);
	tempNode.to_pool_node->domain.push_back(data.to_pool_node);
	tempNode.to_pool_node->value = value;
	float d = std::cos(data.data);
	//求导数
	data.to_pool_node->d.push_back(d);
	//为右值节点的标签集中传入本次操作的标签
	data.to_pool_node->tag.push_back(SIN);
	data.to_pool_node->helpNode.push_back(nullptr);
	data.to_pool_node->help.push_back(1.0f);
	return tempNode;
}
//余弦，弧度制
ForwardOperationNode ForwardOperationNode::cos(const ForwardOperationNode& data)
{
	//做运算
	float value = std::cos(data.data);
	ForwardOperationNode tempNode(std::cos(data.data), data.to_pool);
	//确定连接关系
	data.to_pool_node->codomain.push_back(tempNode.to_pool_node);
	tempNode.to_pool_node->domain.push_back(data.to_pool_node);
	tempNode.to_pool_node->value = value;
	float d = -std::sin(data.data);
	//求导数
	data.to_pool_node->d.push_back(d);
	//为右值节点的标签集中传入本次操作的标签
	data.to_pool_node->tag.push_back(COS);
	data.to_pool_node->helpNode.push_back(nullptr);
	data.to_pool_node->help.push_back(1.0f);
	return tempNode;
}
//正切，弧度制
ForwardOperationNode ForwardOperationNode::tan(const ForwardOperationNode& data)
{
	//做运算
	float value= std::tan(data.data);
	ForwardOperationNode tempNode(std::tan(data.data), data.to_pool);
	//确定连接关系
	data.to_pool_node->codomain.push_back(tempNode.to_pool_node);
	tempNode.to_pool_node->domain.push_back(data.to_pool_node);
	tempNode.to_pool_node->value = value;
	float d = 1.f/ std::cos(data.data) /std::cos(data.data);
	//求导数
	data.to_pool_node->d.push_back(d);
	//为右值节点的标签集中传入本次操作的标签
	data.to_pool_node->tag.push_back(TAN);
	data.to_pool_node->helpNode.push_back(nullptr);
	data.to_pool_node->help.push_back(1.0f);
	return tempNode;
}
//反正弦，弧度制
ForwardOperationNode ForwardOperationNode::asin(const ForwardOperationNode& data)
{
	//做运算
	float value = std::asin(data.data);
	ForwardOperationNode tempNode(std::asin(data.data), data.to_pool);
	//确定连接关系
	data.to_pool_node->codomain.push_back(tempNode.to_pool_node);
	tempNode.to_pool_node->domain.push_back(data.to_pool_node);
	tempNode.to_pool_node->value = value;
	float d = 1.f / (std::sqrt(1.f - data.data * data.data));
	//求导数
	data.to_pool_node->d.push_back(d);
	//为右值节点的标签集中传入本次操作的标签
	data.to_pool_node->tag.push_back(ASIN);
	data.to_pool_node->helpNode.push_back(nullptr);
	data.to_pool_node->help.push_back(1.0f);
	return tempNode;
}
//反余弦，弧度制
ForwardOperationNode ForwardOperationNode::acos(const ForwardOperationNode& data)
{
	//做运算
	float value = std::acos(data.data);
	ForwardOperationNode tempNode(std::acos(data.data), data.to_pool);
	//确定连接关系
	data.to_pool_node->codomain.push_back(tempNode.to_pool_node);
	tempNode.to_pool_node->domain.push_back(data.to_pool_node);
	tempNode.to_pool_node->value = value;
	float d = -1.f / (std::sqrt(1.f - data.data * data.data));
	//求导数
	data.to_pool_node->d.push_back(d);
	//为右值节点的标签集中传入本次操作的标签
	data.to_pool_node->tag.push_back(ACOS);
	data.to_pool_node->helpNode.push_back(nullptr);
	data.to_pool_node->help.push_back(1.0f);
	return tempNode;
}
//反正切，弧度制
ForwardOperationNode ForwardOperationNode::atan(const ForwardOperationNode& data)
{
	//做运算
	float value = std::atan(data.data);
	ForwardOperationNode tempNode(std::atan(data.data), data.to_pool);
	//确定连接关系
	data.to_pool_node->codomain.push_back(tempNode.to_pool_node);
	tempNode.to_pool_node->domain.push_back(data.to_pool_node);
	tempNode.to_pool_node->value = value;
	float d = 1.f / (1 + data.data * data.data);
	//求导数
	data.to_pool_node->d.push_back(d);
	//为右值节点的标签集中传入本次操作的标签
	data.to_pool_node->tag.push_back(ATAN);
	data.to_pool_node->helpNode.push_back(nullptr);
	data.to_pool_node->help.push_back(1.0f);
	return tempNode;
}
void ForwardOperationNode::GradientBack(ForwardOperationNode* dy, ForwardOperationNode* dx, float& d)
{
	Pool* pool = dx->to_pool;
	bool dy_exist = false;
	bool dx_exist = false;
	for (int i = 0; i < pool->pool_node.size(); i++)
	{
		if (pool->pool_node[i] == dy->to_pool_node)
		{
			dy_exist = true;
		}
		if (pool->pool_node[i] == dx->to_pool_node)
		{
			dx_exist = true;
		}
	}
	if (!(dx_exist && dy_exist))
	{
		std::cerr << dx << "或者" << dy << "不存在于池中" << std::endl;
		return;
	}
	pool->GradientBack(dy->to_pool_node, dx->to_pool_node, d);
}
