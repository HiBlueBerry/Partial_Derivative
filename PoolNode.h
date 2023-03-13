#pragma once
#ifndef  OTHER
#define OTHER
#include "SomeInclude.h"
#endif
#ifndef  POOL_NODE
#define  POOL_NODE
class ForwardOperationNode;
class Pool;
class PoolNode
{
public:
public:
	bool variable = true;
	float value=0.0f;
	ForwardOperationNode* to_forward_operation_node = nullptr;
	Pool* to_pool;
	std::vector<float> d;
	std::vector<PoolNode*>codomain;
	std::vector<int>tag;
	std::vector<PoolNode*>helpNode;
	std::vector<float>help;
	std::vector<PoolNode*>domain;
public:
};
#endif // ! A_H
