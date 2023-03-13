#include "Pool.h"
//返回池中节点个数
int Pool::NodeLength()
{
	return pool_node.size();
}
//求池节点中一个对另一个节点的偏导数
float Pool::GradientBack(PoolNode* dy, PoolNode* dx,float& d,int depth)
{
	if (depth == 0)
	{
		d = 0.f;
	}
	//如果该节点没有值域了，则返回
	if (dx->codomain.size() == 0)
	{
		return 0;
	}
	float temp;
	float d1 = 0.0f;
	//如果该节点可以进行值域搜索
	for (int k = 0; k < dx->codomain.size(); k++)
	{
		//如果该节点的值域中存在dy,则返回
		if (dx->codomain[k] == dy)
		{
			return dx->d[k];
		}
		//如果该节点的值域中没有dy,则深入下一层节点中寻找
		else
		{	
			temp=dx->d[k]*GradientBack(dy,dx->codomain[k],d,depth+1);
		}
		//累加求和
		d1 += temp;
		
	}
	//如果此时并不是dx,则返回该节点的偏导数
	if (depth != 0)
	{
		return d1;
	}
	//如果此时节点是dx,则查找到了导数传播链，同时返回1代表偏导数计算成功
	else
	{
		d = d1;
		return 1;
	}
}
