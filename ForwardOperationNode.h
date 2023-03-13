#pragma once
#ifndef  OTHER
#define OTHER
#include "SomeInclude.h"
#endif
#ifndef FORWARD_OPERATION_NODE
#define FORWARD_OPERATION_NODE
#include "Pool.h"
	class ForwardOperationNode
	{	
		public:
			float data;
			PoolNode* to_pool_node = nullptr;
			Pool* to_pool;
		public:
			ForwardOperationNode(float data, Pool* pool);
			~ForwardOperationNode();
			//常数赋值
			void operator =(float data);
			//变量赋值
			void operator =(const ForwardOperationNode& temp);
			//加法,处理变量+变量
			ForwardOperationNode operator +(const ForwardOperationNode& another);
			//加法，处理变量+常量
			ForwardOperationNode operator +(float temp)const;
			//加法，处理常量+变量
			friend ForwardOperationNode operator +(float temp,const ForwardOperationNode& another);
			//减法，处理变量-变量
			ForwardOperationNode operator -(const ForwardOperationNode& another);
			//减法，处理变量-常量
			ForwardOperationNode operator -(float temp)const;
			//减法，处理常量-变量
			friend ForwardOperationNode operator -(float temp, const ForwardOperationNode& another);
			//取反
			ForwardOperationNode operator -();
			//乘法，处理变量*变量
			ForwardOperationNode operator *(const ForwardOperationNode& another);
			//乘法，处理变量*常量
			ForwardOperationNode operator *(float temp)const;
			//乘法，处理常量*变量
			friend ForwardOperationNode operator *(float temp,const ForwardOperationNode& another);
			//除法，处理变量/变量
			ForwardOperationNode operator /(const ForwardOperationNode& another);
			//除法，处理变量/常量
			ForwardOperationNode operator /(float temp)const;
			//除法，处理常量/变量
			friend ForwardOperationNode operator /(float temp, const ForwardOperationNode& another);
			//指数，参数：(次数(自变量),基底)
			static ForwardOperationNode exp(const ForwardOperationNode& exponent, float base=EULER);
			//幂次，参数：(基底(自变量),次数)
			static ForwardOperationNode pow(const ForwardOperationNode& base,float deg);
			//对数，参数：(幂(自变量),基底)
			static ForwardOperationNode log(const ForwardOperationNode& data, float base=EULER);
			//正弦，弧度制
			static ForwardOperationNode sin(const ForwardOperationNode& data);
			//余弦，弧度制
			static ForwardOperationNode cos(const ForwardOperationNode& data);
			//正切，弧度制
			static ForwardOperationNode tan(const ForwardOperationNode& data);
			//反正弦，弧度制
			static ForwardOperationNode asin(const ForwardOperationNode& data);
			//反余弦，弧度制
			static ForwardOperationNode acos(const ForwardOperationNode& data);
			//反正切，弧度制
			static ForwardOperationNode atan(const ForwardOperationNode& data);
			//求dy对dx的偏导数
			static void GradientBack(ForwardOperationNode* dy, ForwardOperationNode* dx, float& d);
			inline void VariableAssignHelp(PoolNode* temp);
	};
#endif // !FORWARD_OPERATION_NODE

