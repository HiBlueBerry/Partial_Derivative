#pragma once
#ifndef  OTHER
#define OTHER
#include "SomeInclude.h"
#endif
#ifndef  POOL
#define  POOL
#include "PoolNode.h"
	class Pool
	{
		public:
			std::vector<PoolNode*> pool_node;
			//返回池中节点个数
			int NodeLength();
			//求池节点中一个对另一个节点的偏导数
			float GradientBack(PoolNode* dy, PoolNode* dx, float& d,int depth=0);
	};
#endif