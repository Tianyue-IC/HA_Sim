#include "STA_F.h"
#include "memory.h"
#include <math.h>

////////////////////////////////////////////////////////
//  ����:
//      FindMaxMin
//  ����:
//      �����м�ֵ��STA1��
//  ����:
//      1.RD0:���ݵ�ַ
//      2.RD1:���ݳ���
//  ����ֵ:
//      1.RD0:���ֵ
//      2.RD1:��Сֵ
////////////////////////////////////////////////////////
Sub_AutoField FindMaxMin
{
	RA0 = RD0;
	int len = RD1.m_data;

	int max, min;
	RD0 = GET_M(RA0);
	max = *(short*)(&RD0.m_data);					
	min = *(short*)(&RD0.m_data);

	for (int i = 0; i < len; i++)
	{
		RD0 = GET_M(RA0 + i * MMU_BASE);
		RD1 = *(short*)(&RD0.m_data);
		if (max < RD1.m_data)
			max = RD1.m_data;
		if (min > RD1.m_data)
			min = RD1.m_data;
		RD1 = RD0 >> 16;
		if (max < RD1.m_data)
			max = RD1.m_data;
		if (min > RD1.m_data)
			min = RD1.m_data;

	}
	max &= 0xffff;
	min &= 0xffff;
	RD0 = max;
	RD1 = min;
	Return_AutoField(0);

}

////////////////////////////////////////////////////////
//  ����:
//      AbsSum
//  ����:
//      �����еľ���ֵ�ۼӺͣ�STA1��
//  ����:
//      1.RD0:���ݵ�ַ
//      2.RD1:���ݳ���
//  ����ֵ:
//      1.RD0:����ֵ�ۼӺ�
////////////////////////////////////////////////////////
Sub_AutoField AbsSum
{
	RA0 = RD0;	
	int len = RD1.m_data;

	int sum = 0;

	for (int i = 0; i < len; i++)
	{
		RD0 = GET_M(RA0 + i * MMU_BASE);
		RD1 = RD0 >> 16;
		sum = abs(RD1.m_data) + sum;
		RD1 = *(short*)(&RD0.m_data);
		sum = abs(RD1.m_data) + sum;
	}
	RD0 = sum;

	pop(RA0);
	Return_AutoField(0);

}

////////////////////////////////////////////////////////
//  ����:
//      FindMaxIndex
//  ����:
//      �����о���ֵ�����ֵ��Index��STA2��,index��DWORDΪ��λ,0..L-1;
//  ����:
//      1.RD0:���ݵ�ַ
//      2.RD1:���ݳ���
//  ����ֵ:
//      1.RD0:���ֵ��Index, 0 to L-1;
//	��ע:
//		ȡ���еĸ�16λ
////////////////////////////////////////////////////////
Sub_AutoField FindMaxIndex
{
	int len = RD1.m_data;

	RD0 = GET_M(RA0);
	int max = abs(RD0.m_data);
	int n = 0;
	for (int i = 0; i < len; i++)
	{
		RD0 = GET_M(RA0 + i * MMU_BASE);
		RF_GetH16(RD0);
		RD0_SignExtL16;
		RD0 = abs(RD0.m_data);
		if (max < RD0.m_data)
		{
			max = RD0.m_data;
			n = i;
		}
	}
	RD0 = n;
	Return_AutoField(0);

}