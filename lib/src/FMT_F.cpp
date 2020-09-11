#include "FMT_F.h"
#include "mac.h"
#include "memory.h"
#include <math.h>

////////////////////////////////////////////////////////
//  ����:
//      Get_Real
//  ����:
//      ��ȡʵ����ϵͳĬ�ϸ�λΪʵ����,addr_in,addr_out������ͬһ��ַ;
//  ����:
//      1.RA0:��������ָ�룬��ʽ[Re | Im]
//      2.RA1:�������ָ�룬��ʽ[Re(n+1) | Re(n)](out)
//      3.RD0:���г���,Ӧ����������е�DWORD��
//  ����ֵ:
//      ��
////////////////////////////////////////////////////////
Sub_AutoField Get_Real
{
	int len = RD0.m_data*2;

	for (int i = 0; i < len; i++)
	{
		RD0 = GET_M(RA0 + i * MMU_BASE);	//������
		RD0 >>= 16;							//ȡ��λ
		RD1 = RD0 & 0xFFFF;

		i++;
		RD0 = GET_M(RA0 + i * MMU_BASE);	//ż����
		RD0 &= 0xFFFF0000;

		RD0 += RD1;
		SET_M(RA1 + (i-1)/2 * MMU_BASE, RD0);
	}
	Return_AutoField(0);

}

////////////////////////////////////////////////////////
//  ����:
//      Get_Imag
//  ����:
//      ��ȡ�鲿��ϵͳĬ�ϵ�λΪ�鲿��,addr_in,addr_out������ͬһ��ַ;
//  ����:
//      1.RA0:��������ָ�룬��ʽ[Re | Im]
//      2.RA1:�������ָ�룬��ʽ[Im(n+1) | Im(n)](out)
//      3.RD0:���г���,Ӧ����������е�DWORD��
//  ����ֵ:
//      ��
////////////////////////////////////////////////////////
Sub_AutoField Get_Imag
{
	int len = RD0.m_data*2;

	for (int i = 0; i < len; i++)
	{
		RD0 = GET_M(RA0 + i * MMU_BASE);	//������						
		RD1 = RD0 & 0xFFFF;					//ȡ��λ

		i++;
		RD0 = GET_M(RA0 + i * MMU_BASE);	//ż����
		RD0 <<= 16;

		RD0 += RD1;
		SET_M(RA1 + (i - 1) / 2 * MMU_BASE, RD0);
	}
	Return_AutoField(0);

}


////////////////////////////////////////////////////////
//  ����:
//      Real_To_Complex2
//  ����:
//      ����16bit��ʽת��Ϊ������ʽ,�鲿����;
//  ����:
//      1.RA0:��������ָ�룬��ʽ[Re(n+1) | Re(n)]
//      2.RA1:�������ָ�룬��ʽ[Re | 0](out)
//      3.RD0:���г���
//  ����ֵ:
//      ��
////////////////////////////////////////////////////////
Sub_AutoField Real_To_Complex2
{
	int len = RD0.m_data;
	int	x[1024];

	for (int i = 0; i < len; i++)
	{
		RD0 = GET_M(RA0 + i * MMU_BASE);				
		RD1 = RD0 << 16;					//ȡ��λ
		x[2 * i] = RD1.m_data;

		RD0 &=0xFFFF0000;					//ȡ��λ
		x[2 * i + 1] = RD0.m_data;
	}

	for (int i = 0; i < (2 * len); i++)
	{
		SET_M(RA1 + i * MMU_BASE, x[i]);
	}
	Return_AutoField(0);

}
