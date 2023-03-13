#include "Pool.h"
//���س��нڵ����
int Pool::NodeLength()
{
	return pool_node.size();
}
//��ؽڵ���һ������һ���ڵ��ƫ����
float Pool::GradientBack(PoolNode* dy, PoolNode* dx,float& d,int depth)
{
	if (depth == 0)
	{
		d = 0.f;
	}
	//����ýڵ�û��ֵ���ˣ��򷵻�
	if (dx->codomain.size() == 0)
	{
		return 0;
	}
	float temp;
	float d1 = 0.0f;
	//����ýڵ���Խ���ֵ������
	for (int k = 0; k < dx->codomain.size(); k++)
	{
		//����ýڵ��ֵ���д���dy,�򷵻�
		if (dx->codomain[k] == dy)
		{
			return dx->d[k];
		}
		//����ýڵ��ֵ����û��dy,��������һ��ڵ���Ѱ��
		else
		{	
			temp=dx->d[k]*GradientBack(dy,dx->codomain[k],d,depth+1);
		}
		//�ۼ����
		d1 += temp;
		
	}
	//�����ʱ������dx,�򷵻ظýڵ��ƫ����
	if (depth != 0)
	{
		return d1;
	}
	//�����ʱ�ڵ���dx,����ҵ��˵�����������ͬʱ����1����ƫ��������ɹ�
	else
	{
		d = d1;
		return 1;
	}
}
