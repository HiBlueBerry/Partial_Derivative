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
			//������ֵ
			void operator =(float data);
			//������ֵ
			void operator =(const ForwardOperationNode& temp);
			//�ӷ�,�������+����
			ForwardOperationNode operator +(const ForwardOperationNode& another);
			//�ӷ����������+����
			ForwardOperationNode operator +(float temp)const;
			//�ӷ���������+����
			friend ForwardOperationNode operator +(float temp,const ForwardOperationNode& another);
			//�������������-����
			ForwardOperationNode operator -(const ForwardOperationNode& another);
			//�������������-����
			ForwardOperationNode operator -(float temp)const;
			//������������-����
			friend ForwardOperationNode operator -(float temp, const ForwardOperationNode& another);
			//ȡ��
			ForwardOperationNode operator -();
			//�˷����������*����
			ForwardOperationNode operator *(const ForwardOperationNode& another);
			//�˷����������*����
			ForwardOperationNode operator *(float temp)const;
			//�˷���������*����
			friend ForwardOperationNode operator *(float temp,const ForwardOperationNode& another);
			//�������������/����
			ForwardOperationNode operator /(const ForwardOperationNode& another);
			//�������������/����
			ForwardOperationNode operator /(float temp)const;
			//������������/����
			friend ForwardOperationNode operator /(float temp, const ForwardOperationNode& another);
			//ָ����������(����(�Ա���),����)
			static ForwardOperationNode exp(const ForwardOperationNode& exponent, float base=EULER);
			//�ݴΣ�������(����(�Ա���),����)
			static ForwardOperationNode pow(const ForwardOperationNode& base,float deg);
			//������������(��(�Ա���),����)
			static ForwardOperationNode log(const ForwardOperationNode& data, float base=EULER);
			//���ң�������
			static ForwardOperationNode sin(const ForwardOperationNode& data);
			//���ң�������
			static ForwardOperationNode cos(const ForwardOperationNode& data);
			//���У�������
			static ForwardOperationNode tan(const ForwardOperationNode& data);
			//�����ң�������
			static ForwardOperationNode asin(const ForwardOperationNode& data);
			//�����ң�������
			static ForwardOperationNode acos(const ForwardOperationNode& data);
			//�����У�������
			static ForwardOperationNode atan(const ForwardOperationNode& data);
			//��dy��dx��ƫ����
			static void GradientBack(ForwardOperationNode* dy, ForwardOperationNode* dx, float& d);
			inline void VariableAssignHelp(PoolNode* temp);
	};
#endif // !FORWARD_OPERATION_NODE

