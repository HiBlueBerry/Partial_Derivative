#include"ForwardOperationNode.h"
//�涨һ�������������㣬�䵼��ֵ�����ڴ���������Ϊ�Ա����ı����Ķ�Ӧ���ڴ�ؽڵ���
//�涨��������������ʱ���ּ�����������ͬһ���ڴ�ص����
//�涨������Ӧһ����������̶��ԣ���Ӧ������һ�����ϵĸ�ֵ����

//�����Զ������
ForwardOperationNode::ForwardOperationNode(float data, Pool* pool)
{	//��ֵ��ȷ��ָ���ĸ��ڴ��
	this->data = data;
	this->to_pool = pool;
	//�����ڴ�ؽڵ㣬��Ϊ���䱸�ڴ��
	PoolNode* temp = new PoolNode();
	temp->to_pool = pool;
	//�����ڴ�ؽڵ��ǰ������ڵ������
	temp->to_forward_operation_node = this;
	to_pool_node = temp;
	temp->value = data;
	//��Ӧ�ڴ�ذ��ڴ�ؽڵ����
	pool->pool_node.push_back(temp);
}
//ɾ����ʱ�������Զ������ʱ���ڴ���ж�Ӧ�ڵ�����ӶϿ�
ForwardOperationNode::~ForwardOperationNode()
{	//��������ڴ�����ж�Ӧ�ڵ�
	if (to_pool_node != nullptr)
	{	//����ʱ�����ӶϿ�
		to_pool_node->to_forward_operation_node = nullptr;
	}
}
//�Ա�����ֵ���¸�������
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
//��ֵ��������ʱ+��������ʽ
void ForwardOperationNode::operator =(float data)
{	//��һ�������в�Ӧ�������ٴθı�������ı��ʽ
	if (to_pool_node != nullptr)
	{	if(to_pool_node->domain.size()!=0)
		{
			std::cerr << "��" << this << "�����Ըı�����Ϊ������ı��ʽ���˴θ�ֵ��Ч" << std::endl;
			return;
		}
	}
	//��ֵ��Ȼ���Զ�����������Զ������������ʱ��Ȼ�����ڴ���д���һ���ڵ㲢��������
	this->data = data;
	to_pool_node->value = data;
	//��ʱ������Ȼû�����ڴ�����нڵ���֮��Ӧ,���Ҳ����ܱ���ֵ
	//�����޶��˱������ܱ���ֵһ�Σ�����������ʽ����ҪΪ�������ٽڵ㣬���ٽڵ�������ӿ��������ҵ���Ϊ0����������ʵ������
	//Ӧ�������Ա����ظ���ֵ�Ļ���
	VariableAssignHelp(to_pool_node);
}
//��ֵ��������ʱ/�Զ���+��������ʽ
void ForwardOperationNode::operator =(const ForwardOperationNode& temp)
{	//��һ�������в�Ӧ�������ٴθı�������ı��ʽ
	if (to_pool_node != nullptr)
	{
		if (to_pool_node->domain.size() != 0)
		{

			std::cerr << "��" << this << "�����Ըı�����Ϊ������ı��ʽ���˴θ�ֵ��Ч" << std::endl;
			return;
		}
	}
	//��ֵ��Ȼ��ForwardOperationNode�ṹ�ı�������ForwardOperationNode����ʱ��Ȼ�����ڴ���д���һ���ڵ㲢��������
	//�����������������ڴ�ز�ͬ
	if (to_pool != temp.to_pool)
	{
		std::cerr << this << "��" << &temp << "�������ڴ�ز�ͬ"<<std::endl;
		return;
	}
	data = temp.data;
	to_pool_node->value = data;
	//Ϊ��ֵ�Ķ������д�����ֵ�ڵ�
	to_pool_node->domain.push_back(temp.to_pool_node);
	//Ϊ��ֵ�������д�����ֵ�ڵ�
	temp.to_pool_node->codomain.push_back(to_pool_node);
	//Ϊ��ֵ�ڵ�ĵ������д��뱾�ε���
	temp.to_pool_node->d.push_back(1.f);
	//Ϊ��ֵ�ڵ�ı�ǩ���д��뱾�β����ı�ǩ
	temp.to_pool_node->tag.push_back(EQUAL);
	temp.to_pool_node->helpNode.push_back(temp.to_pool_node);
	temp.to_pool_node->help.push_back(0.0f);

}
//�ӷ�,�������+����
ForwardOperationNode ForwardOperationNode::operator +(const ForwardOperationNode& another)
{
	//�����������������ڴ�ز�ͬ
	if (to_pool != another.to_pool)
	{
		std::cerr << this << "��" << &another << "�������ڴ�ز�ͬ,��Ĭ�Ϲ�����������ָ����ͬ�ڴ�صĽڵ�" << std::endl;
		return *this + ForwardOperationNode(another.data,to_pool);
	}
	//�½�һ���ڵ㣬��Ϊ�²����ĺ͵Ľڵ�
	ForwardOperationNode temp(data+another.data,to_pool);
	//���������������������ӹ�ϵ
	to_pool_node->codomain.push_back(temp.to_pool_node);
	another.to_pool_node->codomain.push_back(temp.to_pool_node);
	temp.to_pool_node->domain.push_back(to_pool_node);
	temp.to_pool_node->domain.push_back(another.to_pool_node);
	temp.to_pool_node->value += data + another.data;
	//ȷ���Ͷ������ӷ�����ƫ����,�ֱ���������ӷ���(��Ϊ�����������������Ϊ���Ա���)��
	to_pool_node->d.push_back(1.f);
	another.to_pool_node->d.push_back(1.f);
	//Ϊ��ֵ�ڵ�ı�ǩ���д��뱾�β����ı�ǩ
	//to_pool_node->t.push_back(PoolNode::ce());
	to_pool_node->tag.push_back(PLUS);
	to_pool_node->helpNode.push_back(another.to_pool_node);
	to_pool_node->help.push_back(0.0f);
	another.to_pool_node->tag.push_back(PLUS);
	another.to_pool_node->helpNode.push_back(to_pool_node);
	another.to_pool_node->help.push_back(0.0f);
	return temp;

}
//�ӷ����������+����
ForwardOperationNode ForwardOperationNode::operator +(float temp) const
{	
	//�½�һ���ڵ㣬��Ϊ�²����ĺ͵Ľڵ�
	ForwardOperationNode tempNode(data + temp, to_pool);
	//����������������ӹ�ϵ
	to_pool_node->codomain.push_back(tempNode.to_pool_node);
	tempNode.to_pool_node->domain.push_back(to_pool_node);
	tempNode.to_pool_node->value = data + temp;
	//ȷ���ͶԼӷ����ĵ���,����ӷ���(��Ϊ���������������Ϊ���Ա���)��
	to_pool_node->d.push_back(1.f);
	//Ϊ��ֵ�ڵ�ı�ǩ���д��뱾�β����ı�ǩ
	to_pool_node->tag.push_back(PLUS);
	to_pool_node->helpNode.push_back(nullptr);
	to_pool_node->help.push_back(temp);
	
	return tempNode;
}
//�ӷ���������+����
ForwardOperationNode operator +(float temp, const ForwardOperationNode& another)
{
	return another + temp;
}
//�������������-����
ForwardOperationNode ForwardOperationNode::operator -(const ForwardOperationNode& another)
{
	//�����������������ڴ�ز�ͬ
	if (to_pool != another.to_pool)
	{
		std::cerr << this << "��" << &another << "�������ڴ�ز�ͬ,��Ĭ�Ϲ�����������ָ����ͬ�ڴ�صĽڵ�" << std::endl;
		return *this - ForwardOperationNode(another.data, to_pool);
	}
	//�½�һ���ڵ㣬��Ϊ�²����Ĳ�Ľڵ�
	ForwardOperationNode temp(data - another.data, to_pool);
	//���������������������ӹ�ϵ
	to_pool_node->codomain.push_back(temp.to_pool_node);
	another.to_pool_node->codomain.push_back(temp.to_pool_node);
	temp.to_pool_node->domain.push_back(to_pool_node);
	temp.to_pool_node->domain.push_back(another.to_pool_node);
	temp.to_pool_node->value = data - another.data;
	//ȷ���Ͷ�������������ƫ����,�ֱ��������������(��Ϊ�����������������Ϊ���Ա���)��
	to_pool_node->d.push_back(1.f);
	another.to_pool_node->d.push_back(-1.f);
	//Ϊ��ֵ�ڵ�ı�ǩ���д��뱾�β����ı�ǩ
	to_pool_node->tag.push_back(MINUS_1);
	temp.to_pool_node->helpNode.push_back(another.to_pool_node);
	temp.to_pool_node->help.push_back(0.0f);
	another.to_pool_node->tag.push_back(MINUS_2);
	another.to_pool_node->helpNode.push_back(to_pool_node);
	another.to_pool_node->help.push_back(0.0f);
	return temp;

}
//�������������-����
ForwardOperationNode ForwardOperationNode::operator -(float temp) const
{
	//�½�һ���ڵ㣬��Ϊ�²����Ĳ�Ľڵ�
	ForwardOperationNode tempNode(data - temp, to_pool);
	//����������������ӹ�ϵ
	to_pool_node->codomain.push_back(tempNode.to_pool_node);
	tempNode.to_pool_node->domain.push_back(to_pool_node);
	tempNode.to_pool_node->value = data - temp;
	//ȷ���ͶԼ������ĵ���,���������(��Ϊ���������������Ϊ���Ա���)��
	to_pool_node->d.push_back(1.f);
	//Ϊ��ֵ�ڵ�ı�ǩ���д��뱾�β����ı�ǩ
	to_pool_node->tag.push_back(MINUS_1);
	to_pool_node->helpNode.push_back(nullptr);
	to_pool_node->help.push_back(temp);
	return tempNode;
}
//������������-����
ForwardOperationNode operator -(float temp, const ForwardOperationNode& another) 
{
	//�½�һ���ڵ㣬��Ϊ�²����Ĳ�Ľڵ�
	ForwardOperationNode tempNode(temp-another.data, another.to_pool);
	//����������������ӹ�ϵ
	another.to_pool_node->codomain.push_back(tempNode.to_pool_node);
	tempNode.to_pool_node->domain.push_back(another.to_pool_node);
	tempNode.to_pool_node->value = temp - another.data;
	//ȷ���ͶԼ������ĵ���,���������(��Ϊ���������������Ϊ���Ա���)��
	another.to_pool_node->d.push_back(-1.f);
	//Ϊ��ֵ�ڵ�ı�ǩ���д��뱾�β����ı�ǩ
	another.to_pool_node->tag.push_back(MINUS_2);
	another.to_pool_node->helpNode.push_back(nullptr);
	another.to_pool_node->help.push_back(temp);
	return tempNode;
}
//ȡ��
ForwardOperationNode ForwardOperationNode::operator -()
{
	//�½�һ���ڵ㣬��Ϊ�²����Ļ��Ľڵ�
	ForwardOperationNode tempNode(data * -1.f, to_pool);
	//����������������ӹ�ϵ
	to_pool_node->codomain.push_back(tempNode.to_pool_node);
	tempNode.to_pool_node->domain.push_back(to_pool_node);
	tempNode.to_pool_node->value = -data;
	//ȷ���ͶԳ˷����ĵ���,����˷���(��Ϊ���������������Ϊ���Ա���)��
	to_pool_node->d.push_back(-1.f);
	//Ϊ��ֵ�ڵ�ı�ǩ���д��뱾�β����ı�ǩ
	to_pool_node->tag.push_back(NEGATIVE);
	to_pool_node->helpNode.push_back(nullptr);
	to_pool_node->help.push_back(-1.0f);
	return tempNode;
}
//�˷����������*����
ForwardOperationNode ForwardOperationNode::operator *(const ForwardOperationNode& another)
{
	//�����������������ڴ�ز�ͬ
	if (to_pool != another.to_pool)
	{
		std::cerr << this << "��" << &another << "�������ڴ�ز�ͬ,��Ĭ�Ϲ�����������ָ����ͬ�ڴ�صĽڵ�" << std::endl;
		return *this * ForwardOperationNode(another.data, to_pool);
	}
	//�½�һ���ڵ㣬��Ϊ�²����Ļ��Ľڵ�
	ForwardOperationNode temp(data * another.data, to_pool);
	//���������������������ӹ�ϵ
	to_pool_node->codomain.push_back(temp.to_pool_node);
	another.to_pool_node->codomain.push_back(temp.to_pool_node);
	temp.to_pool_node->domain.push_back(to_pool_node);
	temp.to_pool_node->domain.push_back(another.to_pool_node);
	temp.to_pool_node->value = data * another.data;
	//ȷ���Ͷ������˷�����ƫ����,�ֱ���������˷���(��Ϊ�����������������Ϊ���Ա���)��
	to_pool_node->d.push_back(another.data);
	another.to_pool_node->d.push_back(data);
	//Ϊ��ֵ�ڵ�ı�ǩ���д��뱾�β����ı�ǩ
	to_pool_node->tag.push_back(MULTIPLY);
	to_pool_node->helpNode.push_back(another.to_pool_node);
	to_pool_node->help.push_back(0.0f);
	another.to_pool_node->tag.push_back(MULTIPLY);
	another.to_pool_node->helpNode.push_back(to_pool_node);
	another.to_pool_node->help.push_back(0.0f);
	return temp;

}
//�˷����������*����
ForwardOperationNode ForwardOperationNode::operator *(float temp) const
{
	//�½�һ���ڵ㣬��Ϊ�²����Ļ��Ľڵ�
	ForwardOperationNode tempNode(data * temp, to_pool);
	//����������������ӹ�ϵ
	to_pool_node->codomain.push_back(tempNode.to_pool_node);
	tempNode.to_pool_node->domain.push_back(to_pool_node);
	tempNode.to_pool_node->value = data * temp;
	//ȷ���ͶԳ˷����ĵ���,����˷���(��Ϊ���������������Ϊ���Ա���)��
	to_pool_node->d.push_back(temp);
	//Ϊ��ֵ�ڵ�ı�ǩ���д��뱾�β����ı�ǩ
	to_pool_node->tag.push_back(MULTIPLY);
	to_pool_node->helpNode.push_back(nullptr);
	to_pool_node->help.push_back(temp);
	return tempNode;
}
//�˷���������*����
ForwardOperationNode operator *(float temp, const ForwardOperationNode& another)
{
	return another * temp;
}
//�������������/����
ForwardOperationNode ForwardOperationNode::operator /(const ForwardOperationNode& another)
{
	//�����������������ڴ�ز�ͬ
	if (to_pool != another.to_pool)
	{
		std::cerr << this << "��" << &another << "�������ڴ�ز�ͬ,��Ĭ�Ϲ�����������ָ����ͬ�ڴ�صĽڵ�" << std::endl;
		return *this / ForwardOperationNode(another.data, to_pool);
	}
	//����������Ϊ0
	if (another.data == 0)
	{
		std::cerr << &another << "������ֵΪ0��Ȼ�����Գ���0�����ǲ�����ġ�"<<std::endl;
	}
	//�½�һ���ڵ㣬��Ϊ�²������̵Ľڵ�
	ForwardOperationNode temp(data / another.data, to_pool);
	//�����������������������ӹ�ϵ
	to_pool_node->codomain.push_back(temp.to_pool_node);
	another.to_pool_node->codomain.push_back(temp.to_pool_node);
	temp.to_pool_node->domain.push_back(to_pool_node);
	temp.to_pool_node->domain.push_back(another.to_pool_node);
	temp.to_pool_node->value = data / another.data;
	//ȷ���Ͷ�������������ƫ����,�ֱ��������������(��Ϊ�����������������Ϊ���Ա���)��
	float d1;
	float d2;
	d1 = 1.f /another.data ;
	d2 = -1.f * data / another.data / another.data;
	to_pool_node->d.push_back(d1);
	another.to_pool_node->d.push_back(d2);
	//Ϊ��ֵ�ڵ�ı�ǩ���д��뱾�β����ı�ǩ
	to_pool_node->tag.push_back(DIVIDED);
	to_pool_node->helpNode.push_back(another.to_pool_node);
	to_pool_node->help.push_back(0.0f);
	another.to_pool_node->tag.push_back(DIVIDE);
	another.to_pool_node->helpNode.push_back(to_pool_node);
	another.to_pool_node->help.push_back(0.0f);
	return temp;
}
//�������������/����
ForwardOperationNode ForwardOperationNode::operator /(float temp) const
{	//����������Ϊ0
	if (temp == 0)
	{
		std::cerr << &temp << "������ֵΪ0��Ȼ�����Գ���0�����ǲ�����ġ�" << std::endl;
	}
	//�½�һ���ڵ㣬��Ϊ�²������̵Ľڵ�
	ForwardOperationNode tempNode(data / temp, to_pool);
	//�������뱻���������ӹ�ϵ
	to_pool_node->codomain.push_back(tempNode.to_pool_node);
	tempNode.to_pool_node->domain.push_back(to_pool_node);
	tempNode.to_pool_node->value = data / temp;
	//ȷ���̶Գ������ĵ���,���뱻����(��Ϊ���������������Ϊ���Ա���)��
	to_pool_node->d.push_back(1.f/temp);
	//Ϊ��ֵ�ڵ�ı�ǩ���д��뱾�β����ı�ǩ
	to_pool_node->tag.push_back(DIVIDED);
	to_pool_node->helpNode.push_back(nullptr);
	to_pool_node->help.push_back(temp);
	return tempNode;
}
//������������/����
ForwardOperationNode operator /(float temp, const ForwardOperationNode& another)
{	//����������Ϊ0
	if (another.data == 0)
	{
		std::cerr << &another << "������ֵΪ0��Ȼ�����Գ���0�����ǲ�����ġ�" << std::endl;
	}
	//�½�һ���ڵ㣬��Ϊ�²����ĺ͵Ľڵ�
	ForwardOperationNode tempNode(temp / another.data, another.to_pool);
	//����������������ӹ�ϵ
	another.to_pool_node->codomain.push_back(tempNode.to_pool_node);
	tempNode.to_pool_node->domain.push_back(another.to_pool_node);
	tempNode.to_pool_node->value = temp / another.data;
	//ȷ���̶Գ������ĵ���,���������(��Ϊ���������������Ϊ���Ա���)��
	float d;
	d=-temp / another.data / another.data;
	another.to_pool_node->d.push_back(d);
	//Ϊ��ֵ�ڵ�ı�ǩ���д��뱾�β����ı�ǩ
	another.to_pool_node->tag.push_back(DIVIDED);
	another.to_pool_node->helpNode.push_back(nullptr);
	another.to_pool_node->help.push_back(temp);
	return tempNode;
}
//ָ����������(����(�Ա���),����)
ForwardOperationNode ForwardOperationNode::exp(const ForwardOperationNode& exponent, float base)
{	//������
	float value = std::pow(base, exponent.data);
	ForwardOperationNode tempNode(value, exponent.to_pool);
	//ȷ�����ӹ�ϵ
	exponent.to_pool_node->codomain.push_back(tempNode.to_pool_node);
	tempNode.to_pool_node->domain.push_back(exponent.to_pool_node);
	tempNode.to_pool_node->value = value;
	float d = value * std::log(base);
	//����
	exponent.to_pool_node->d.push_back(d);
	//Ϊ��ֵ�ڵ�ı�ǩ���д��뱾�β����ı�ǩ
	exponent.to_pool_node->tag.push_back(EXP);
	exponent.to_pool_node->helpNode.push_back(nullptr);
	exponent.to_pool_node->help.push_back(base);
	return tempNode;
}
//�ݴΣ�������(����(�Ա���),����)
ForwardOperationNode ForwardOperationNode::pow(const ForwardOperationNode& base, float deg)
{
	//������
	float value = std::pow(base.data, deg);
	ForwardOperationNode tempNode(value, base.to_pool);
	//ȷ�����ӹ�ϵ
	base.to_pool_node->codomain.push_back(tempNode.to_pool_node);
	tempNode.to_pool_node->domain.push_back(base.to_pool_node);
	tempNode.to_pool_node->value = value;
	float d = deg * std::pow(base.data, deg - 1);
	//����
	base.to_pool_node->d.push_back(d);
	//Ϊ��ֵ�ڵ�ı�ǩ���д��뱾�β����ı�ǩ
	base.to_pool_node->tag.push_back(POW);
	base.to_pool_node->helpNode.push_back(nullptr);
	base.to_pool_node->help.push_back(deg);
	return tempNode;
}
//������������(��(�Ա���),����)
ForwardOperationNode ForwardOperationNode::log(const ForwardOperationNode& data, float base)
{
	if (base <= 0)
	{
		std::cerr << "���ײ��Ǵ���0������" << std::endl;
	}
	if (data.data <= 0)
	{
		std::cerr << "�Ա������Ǵ���0������" << std::endl;
	}
	//������
	float value = std::log(data.data) / std::log(base);
	ForwardOperationNode tempNode(std::log(data.data) / std::log(base), data.to_pool);
	//ȷ�����ӹ�ϵ
	data.to_pool_node->codomain.push_back(tempNode.to_pool_node);
	tempNode.to_pool_node->domain.push_back(data.to_pool_node);
	tempNode.to_pool_node->value = value;
	float d = 1.f / (data.data * std::log(base));
	//����
	data.to_pool_node->d.push_back(d);
	//Ϊ��ֵ�ڵ�ı�ǩ���д��뱾�β����ı�ǩ
	data.to_pool_node->tag.push_back(LOG);
	data.to_pool_node->helpNode.push_back(nullptr);
	data.to_pool_node->help.push_back(base);
	return tempNode;
}
//���ң�������
ForwardOperationNode ForwardOperationNode::sin(const ForwardOperationNode& data)
{
	//������
	float value= std::sin(data.data);
	ForwardOperationNode tempNode(value, data.to_pool);
	//ȷ�����ӹ�ϵ
	data.to_pool_node->codomain.push_back(tempNode.to_pool_node);
	tempNode.to_pool_node->domain.push_back(data.to_pool_node);
	tempNode.to_pool_node->value = value;
	float d = std::cos(data.data);
	//����
	data.to_pool_node->d.push_back(d);
	//Ϊ��ֵ�ڵ�ı�ǩ���д��뱾�β����ı�ǩ
	data.to_pool_node->tag.push_back(SIN);
	data.to_pool_node->helpNode.push_back(nullptr);
	data.to_pool_node->help.push_back(1.0f);
	return tempNode;
}
//���ң�������
ForwardOperationNode ForwardOperationNode::cos(const ForwardOperationNode& data)
{
	//������
	float value = std::cos(data.data);
	ForwardOperationNode tempNode(std::cos(data.data), data.to_pool);
	//ȷ�����ӹ�ϵ
	data.to_pool_node->codomain.push_back(tempNode.to_pool_node);
	tempNode.to_pool_node->domain.push_back(data.to_pool_node);
	tempNode.to_pool_node->value = value;
	float d = -std::sin(data.data);
	//����
	data.to_pool_node->d.push_back(d);
	//Ϊ��ֵ�ڵ�ı�ǩ���д��뱾�β����ı�ǩ
	data.to_pool_node->tag.push_back(COS);
	data.to_pool_node->helpNode.push_back(nullptr);
	data.to_pool_node->help.push_back(1.0f);
	return tempNode;
}
//���У�������
ForwardOperationNode ForwardOperationNode::tan(const ForwardOperationNode& data)
{
	//������
	float value= std::tan(data.data);
	ForwardOperationNode tempNode(std::tan(data.data), data.to_pool);
	//ȷ�����ӹ�ϵ
	data.to_pool_node->codomain.push_back(tempNode.to_pool_node);
	tempNode.to_pool_node->domain.push_back(data.to_pool_node);
	tempNode.to_pool_node->value = value;
	float d = 1.f/ std::cos(data.data) /std::cos(data.data);
	//����
	data.to_pool_node->d.push_back(d);
	//Ϊ��ֵ�ڵ�ı�ǩ���д��뱾�β����ı�ǩ
	data.to_pool_node->tag.push_back(TAN);
	data.to_pool_node->helpNode.push_back(nullptr);
	data.to_pool_node->help.push_back(1.0f);
	return tempNode;
}
//�����ң�������
ForwardOperationNode ForwardOperationNode::asin(const ForwardOperationNode& data)
{
	//������
	float value = std::asin(data.data);
	ForwardOperationNode tempNode(std::asin(data.data), data.to_pool);
	//ȷ�����ӹ�ϵ
	data.to_pool_node->codomain.push_back(tempNode.to_pool_node);
	tempNode.to_pool_node->domain.push_back(data.to_pool_node);
	tempNode.to_pool_node->value = value;
	float d = 1.f / (std::sqrt(1.f - data.data * data.data));
	//����
	data.to_pool_node->d.push_back(d);
	//Ϊ��ֵ�ڵ�ı�ǩ���д��뱾�β����ı�ǩ
	data.to_pool_node->tag.push_back(ASIN);
	data.to_pool_node->helpNode.push_back(nullptr);
	data.to_pool_node->help.push_back(1.0f);
	return tempNode;
}
//�����ң�������
ForwardOperationNode ForwardOperationNode::acos(const ForwardOperationNode& data)
{
	//������
	float value = std::acos(data.data);
	ForwardOperationNode tempNode(std::acos(data.data), data.to_pool);
	//ȷ�����ӹ�ϵ
	data.to_pool_node->codomain.push_back(tempNode.to_pool_node);
	tempNode.to_pool_node->domain.push_back(data.to_pool_node);
	tempNode.to_pool_node->value = value;
	float d = -1.f / (std::sqrt(1.f - data.data * data.data));
	//����
	data.to_pool_node->d.push_back(d);
	//Ϊ��ֵ�ڵ�ı�ǩ���д��뱾�β����ı�ǩ
	data.to_pool_node->tag.push_back(ACOS);
	data.to_pool_node->helpNode.push_back(nullptr);
	data.to_pool_node->help.push_back(1.0f);
	return tempNode;
}
//�����У�������
ForwardOperationNode ForwardOperationNode::atan(const ForwardOperationNode& data)
{
	//������
	float value = std::atan(data.data);
	ForwardOperationNode tempNode(std::atan(data.data), data.to_pool);
	//ȷ�����ӹ�ϵ
	data.to_pool_node->codomain.push_back(tempNode.to_pool_node);
	tempNode.to_pool_node->domain.push_back(data.to_pool_node);
	tempNode.to_pool_node->value = value;
	float d = 1.f / (1 + data.data * data.data);
	//����
	data.to_pool_node->d.push_back(d);
	//Ϊ��ֵ�ڵ�ı�ǩ���д��뱾�β����ı�ǩ
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
		std::cerr << dx << "����" << dy << "�������ڳ���" << std::endl;
		return;
	}
	pool->GradientBack(dy->to_pool_node, dx->to_pool_node, d);
}
