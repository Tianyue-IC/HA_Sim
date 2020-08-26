#include "alu.h"
#include "memory.h"
#include <math.h>

////////////////////////////////////////////////////////
//  ����:
//      dma_trans_ad
//  ����:
//      ��ADC��������ȡһ֡������
//  ����:
//      1.RD0:Դָ��<--δʹ��-->
//      2.RD1:Ŀ��ָ��(out)
//  ����ֵ:
//      ��
////////////////////////////////////////////////////////
Sub_AutoField DMA_Trans_AD
{
		Return_AutoField(0);

}

////////////////////////////////////////////////////////
//  ����:
//      Ram_Clr
//  ����:
//      ���ָ����GRAM��
//  ����:
//      1.RA1:ָ����GRAM���ַ(out)
//		2.RA0:���õĵ�ַ������ָ����һ��Group
//      3.RD1:Dword����
//  ����ֵ:
//      ��
////////////////////////////////////////////////////////
Sub_AutoField Ram_Clr
{
	int len = RD1.m_data;
	for (int i = 0; i < len; i++)
	{
		SET_M(RA1 + i * MMU_BASE, 0);

	}
	Return_AutoField(0);
}


////////////////////////////////////////////////////////
//  ����:
//      Ram_Set
//  ����:
//      ��ָ����GRAM�����ù̶�ֵ(by CPU)
//  ����:
//      1.RA0:ָ����GRAM���ַ(in&out)
//      2.RD0:����DWORD����
//      3.RD1:constֵ
//  ����ֵ:
//      ��
////////////////////////////////////////////////////////
Sub_AutoField Ram_Set
{

	for (int i = 0; i < RD0.m_data; i++)
	{
		SET_M(RA0 + i * MMU_BASE, RD1.m_data);

	}
	Return_AutoField(0);

}




////////////////////////////////////////////////////////
//  ����:
//      Dual_Ser_Add32
//  ����:
//      ˫���мӷ����㣬32bit����
//  ����:
//      1.RA0:��������1ָ�룬32bit��ʽ����(out)
//      2.RA1:��������2ָ�룬32bit��ʽ����
//      3.RD0:����Dword����
//  ����ֵ:
//      1.RD0:�������ָ��
//  ע������:
//   
////////////////////////////////////////////////////////
Sub_AutoField Dual_Ser_Add32
{

	int len = RD0.m_data;
	for (int i = 0; i < len; i++)
	{
		RD0 = GET_M(RA0 + i * MMU_BASE);
		RD1 = GET_M(RA1 + i * MMU_BASE);
		RD0 += RD1;
		SET_M(RA0 + i * MMU_BASE, RD0);
	}
	Return_AutoField(0);

}

////////////////////////////////////////////////////////
//  ����:
//      Dual_Ser_Sub32
//  ����:
//      ˫���м������㣬32bit����
//  ����:
//      1.RA0:��������1ָ�룬32bit��ʽ����
//      2.RA1:��������2ָ�룬32bit��ʽ����
//      3.RD1:�������ָ�룬32bit��ʽ����(out)
//      4.RD0:����Dword����
//  ����ֵ:
//      ��
//  ע������:
//      ��ʱ13.0us;
////////////////////////////////////////////////////////
Sub_AutoField Dual_Ser_Sub32
{
	int len = RD0.m_data;
	for (int i = 0; i < len; i++)
	{
		RD0 = GET_M(RA0 + i * MMU_BASE);
		RD1 = GET_M(RA1 + i * MMU_BASE);
		RD0 -= RD1;
		SET_M(RD1 + i * MMU_BASE, RD0);
	}
	Return_AutoField(0);

}

////////////////////////////////////////////////////////
//  ����:
//      Cal_Single_Shift
//  ����:
//      ��������λ���㣬���ÿ�ѡ
//  ����:
//      1.RD0:����λ��+��λ����ָ�� (��:Op32bit+Rf_SftR1)���DMA_ALU.h
//      2.RD1:���г���
//      3.RA0:��������ָ��(out��ͬ)
//  ����ֵ:
//      ��
////////////////////////////////////////////////////////
Sub_AutoField Cal_Single_Shift
{

	int a;
	if (RD0 == 0b10010000)		//���룬���DMA_ALU.h
		a = -8;
	else if (RD0 == 0b10010001)
		a = -4;
	else if (RD0 == 0b10010010)
		a = -2;
	else if (RD0 == 0b10010011)
		a = -1;
	else if (RD0 == 0b10010100)
		a = 8;
	else if (RD0 == 0b10010101)
		a = 4;
	else if (RD0 == 0b10010110)
		a = 2;
	else if (RD0 == 0b10010111)
		a = 1;
	else
		a = 0;

	unsigned int x;
	for (int i = 0; i < RD1.m_data; i++)
	{
		if (a >= 0)
		{
			RD0 = GET_M(RA0 + i * MMU_BASE);
			x = RD0.m_data;
			x = x << a;
			SET_M(RA0 + i * MMU_BASE, x);
		}
		else
		{
			int y = -a;
			RD0 = GET_M(RA0 + i * MMU_BASE);
			x = RD0.m_data;
			x = x >> y;
			SET_M(RA0 + i * MMU_BASE, x);
		}
	}
	Return_AutoField(0);


}

////////////////////////////////////////////////////////
//  ����:
//      Cal_Single_ShiftR1
//  ����:
//      �������߼�����1λ���㣬32bit����
//  ����:
//      1.RD0:��������ָ�룬32bit��ʽ����(out)
//      2.RD1:��������Dword����
//  ����ֵ:
//      ��
////////////////////////////////////////////////////////
Sub_AutoField Cal_Single_ShiftR1
{

	RA0 = RD0;
	for (int i = 0; i < RD1.m_data; i++)
	{
		RD0 = GET_M(RA0 + i * MMU_BASE);
		RF_ShiftR1(RD0);
		SET_M(RA0 + i * MMU_BASE, RD0);
	}
	Return_AutoField(0);

}

////////////////////////////////////////////////////////
//  ����:
//      Add_DMA_Wola
//  ����:
//      ˫���мӷ����㣬32bit���㣬128DWORD
//  ����:
//      1.RA0:��������1ָ�룬32bit��ʽ����(out)
//      2.RA1:��������2ָ�룬32bit��ʽ����
//  ����ֵ:
//      1.RD0:��
////////////////////////////////////////////////////////
Sub_AutoField Add_DMA_Wola
{
	for (int i = 0; i < 128; i++)
	{
		RD0 = GET_M(RA0 + i * MMU_BASE);
		RD1 = GET_M(RA1 + i * MMU_BASE);
		RD0 += RD1;
		SET_M(RA0 + i * MMU_BASE, RD0);
	}
	Return_AutoField(0);

}


////////////////////////////////////////////////////////
//  ����:
//      Add_DMA_Wola2
//  ����:
//      ˫���мӷ����㣬32bit���㣬DWORD���ɲ���ָ��
//  ����:
//      1.RA0:��������1ָ�룬32bit��ʽ����(out)
//      2.RA1:��������2ָ�룬32bit��ʽ����
//      3.RD0:�����DWORD��
//  ����ֵ:
//      1.RD0:��
////////////////////////////////////////////////////////
Sub_AutoField Add_DMA_Wola2
{
	int len = RD0.m_data;

	for (int i = 0; i < len; i++)
	{
		RD0 = GET_M(RA0 + i * MMU_BASE);
		RD1 = GET_M(RA1 + i * MMU_BASE);
		RD0 += RD1;
		SET_M(RA0 + i * MMU_BASE, RD0);
	}
	Return_AutoField(0);

}


////////////////////////////////////////////////////////
//  ����:
//      Add_LMT
//  ����:
//      ˫���мӷ����㣨�޷���16bit����16bit����
//      д������32λ����DATA0��DATA1��DATA0��16λ������a����16λ������b��DATA1��16λ������c����16λ������d
//      ����a + ����c���޷�����Ϊ����ĸ�16λ������b + ����d���޷�����Ϊ����ĵ�16λ
//  ����:
//      1.RA0:��������1ָ�룬32bit��ʽ����
//      2.RA1:��������2ָ�룬32bit��ʽ����
//      3.RD1:�������ָ�룬32bit��ʽ����(out)
//      4.RD0:����Dword����
//  ����ֵ:
//      ��
////////////////////////////////////////////////////////
Sub_AutoField Add_LMT
{
	push(RA2);
	RA2 = RD1;

	int a, b, c, d;
	int len = RD0.m_data;
	for (int i = 0; i < len; i++)
	{
		RD0 = GET_M(RA0 + i * MMU_BASE);
		a = RD0.m_data >> 16;
		b = *(short*)(&RD0.m_data);
		RD1 = GET_M(RA1 + i * MMU_BASE);
		c = RD1.m_data >> 16;
		d = *(short*)(&RD1.m_data);
		a += c;
		b += d;
		if (a > 32767)                  //�޷�
			a = 32767;
		else if (a < -32767)
			a = -32767;
		if (b > 32767)
			b = 32767;
		else if (b < -32767)
			b = -32767;
		RD0 = (a << 16) + (b & 0xffff);  //intΪ�з�������ab���ǰb�����16λ
		SET_M(RA2 + i * MMU_BASE, RD0);
	}
	pop(RA2);
	Return_AutoField(0);

}

////////////////////////////////////////////////////////
//  ����:
//      Sub_LMT
//  ����:
//      ˫���м������㣨�޷���16bit����16bit����
//  ����:
//      1.RA0:��������1ָ�룬32bit��ʽ����(out)
//      2.RA1:��������2ָ�룬32bit��ʽ����
//      3.RD0:����Dword����
//  ����ֵ:
//      1.RA0:�������ָ�룬32bit��ʽ����
////////////////////////////////////////////////////////
Sub_AutoField Sub_LMT
{
	int len = RD0.m_data;
	int a, b, c, d;
	for (int i = 0; i < len; i++)
	{
		RD0 = GET_M(RA0 + i * MMU_BASE);
		a = RD0.m_data >> 16;
		b = *(short*)(&RD0.m_data);
		RD1 = GET_M(RA1 + i * MMU_BASE);
		c = RD1.m_data >> 16;
		d = *(short*)(&RD1.m_data);
		a -= c;
		b -= d;
		if (a > 32767)                  //�޷�
			a = 32767;
		else if (a < -32767)
			a = -32767;
		if (b > 32767)
			b = 32767;
		else if (b < -32767)
			b = -32767;
		RD0 = (a << 16) + (b & 0xffff);  //intΪ�з�������ab���ǰb�����16λ
		SET_M(RA0 + i * MMU_BASE, RD0);
	}
	Return_AutoField(0);

}

////////////////////////////////////////////////////////
//  ����:
//      Cal_Single_Add_Const
//  ����:
//      �����мӳ������ߵ�16bit����Ч
//  ����:
//      1.RD1:Const
//      2.RA0:Դ��ַ
//      3.RA1:Ŀ���ַ(out)
//      4.RD0:���ݳ���
//  ����ֵ:
//      ��
////////////////////////////////////////////////////////
Sub_AutoField Cal_Single_Add_Const
{

	int a, b, c, d;
	c = RD1.m_data >> 16;
	d = *(short*)(&RD1.m_data);
	int len = RD0.m_data;

	for (int i = 0; i < len; i++)
	{
		RD0 = GET_M(RA0 + i * MMU_BASE);
		a = RD0.m_data >> 16;
		b = *(short*)(&RD0.m_data);
		a += c;
		b += d;
		RD0 = (a << 16) + (b & 0xffff);  //intΪ�з�������ab���ǰb�����16λ
		SET_M(RA1 + i * MMU_BASE, RD0);
	}
	Return_AutoField(0);

}

////////////////////////////////////////////////////////
//  ����:
//      LMT32To16
//  ����:
//      32bit����ת16bit����
//  ����:
//      1.RA0:��������ָ�룬32bit��ʽ����
//      2.RA1:�������ָ�룬16bit���ո�ʽ����(out)
//      3.RD0:�������Dword����
//  ����ֵ:
//      ��
////////////////////////////////////////////////////////
Sub_AutoField LMT32To16
{
	int len = RD0.m_data;

	for (int i = 0; i < len; i++)
	{
		RD0 = GET_M(RA0 + 2 * i * MMU_BASE);
		if (RD0 > 32767)                  //�޷�
			RD0 = 32767;
		else if (RD0 < -32767)
			RD0 = -32767;

		RD1 = GET_M(RA0 + (2 * i + 1) * MMU_BASE);
		if (RD1 > 32767)
			RD1 = 32767;
		else if (RD1 < -32767)
			RD1 = -32767;
		RD0 = (RD1 << 16) + (RD0 & 0xffff);  //intΪ�з����������ǰ�����16λ
		SET_M(RA1 + i * MMU_BASE, RD0);
	}
	Return_AutoField(0);

}

////////////////////////////////////////////////////////
//  ����:
//      DMA_Trans
//  ����:
//      DMA��������
//  ����:
//      1.RA0:Դ��ַ
//      2.RA1:Ŀ���ַ(out)
//      3.RD0:����Dword����
//  ����ֵ:
//      ��
////////////////////////////////////////////////////////
Sub_AutoField DMA_Trans
{
	int len = RD0.m_data;

	for (int i = 0; i < len; i++)
	{
		RD0 = GET_M(RA0 + i * MMU_BASE);
		SET_M(RA1 + i * MMU_BASE, RD0);
	}
	Return_AutoField(0);

}


////////////////////////////////////////////////////////
//  ����:
//      DMA_TransDimin_PATH1
//  ����:
//      ͨ��PATH1��DMA�������ݣ�Ŀ���ַΪ�ݼ�ģʽ������
//  ����:
//      1.RA0:Դָ��
//      2.RA1:Ŀ��ָ��(out)
//      3.RD0:Dword����
//  ����ֵ:
//      ��
/////////////////////////////////////////////////////
Sub_AutoField DMA_TransDimin_PATH1
{
	int len = RD0.m_data;

	for (int i = 0; i < len; i++)
	{
		RD0 = GET_M(RA0 + i * MMU_BASE);
		SET_M(RA1 + (0 - i) * MMU_BASE, RD0);

	}
	Return_AutoField(0);

}

