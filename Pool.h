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
			//���س��нڵ����
			int NodeLength();
			//��ؽڵ���һ������һ���ڵ��ƫ����
			float GradientBack(PoolNode* dy, PoolNode* dx, float& d,int depth=0);
	};
#endif