#include "mac.h"
#include "memory.h"
#include <math.h>
#include "global.h"

//ģ�����ڲ���������������Ӳ��д��
extern void Multi24(int Multi24_0, int Multi24_1);
extern void Mac_Sim(int addr_0, int addr_1, int Const_Reg, unsigned int Config_Reg, int addr_out, int len);
extern void Multi24_16x24(int Multi24_0, int Multi24_1);
extern void Mac_Sim32(int addr_0, int addr_1, int Const_Reg, unsigned int Config_Reg, int addr_out, int len);

//�������Ȿ���Ǹ�����ͬ������ȡMacӲ����ģ������ʵ�ʱ���Ϊ��ȡMac_Sim����
//�������������д�ɣ�ǰ��Լһ��Ϊ������ɹ��ܣ���һ��ʹ����Mac_Simģ���㷨

////////////////////////////////////////////////////////
//  ����:
//      Multi24
//  ����:
//      �˷�����ͨ��c����ģ��Ӳ��24λ�˷������ԡ����漰��MACӲ���ĳ˷�����ʹ�ñ����������
//  ����:
//      1.Multi24_0:��������ӦMACӲ���ڵ�RA0���У�16bit��ʽ
//      2.Multi24_1:��������ӦMACӲ���ڵ�RA1���У�16bit��ʽ����������ʱ����Ϊ24bit����
//      3.RD0:�˷������32bit��ʽ
//  ����ֵֵ:
//      ��
////////////////////////////////////////////////////////
void Multi24(int Multi24_0, int Multi24_1)
{
	long long x;
	x = Multi24_1;
	x = x * 256;
	if (Multi24_1 & 0x8000000)
	{
		x += 1;
	}
	if (Multi24_0 == 0xffff8000)
	{
		Multi24_0 += 1;
	}
	x = x * Multi24_0;
	x = x >> 8;
	RD0 = *(int*)(&x);					//��32bit 


}



////////////////////////////////////////////////////////
//  ����:
//      SingleSerSquare
//  ����:
//      ������ƽ������
//  ����:
//      1.RA0:��������ָ�룬����16bit��ʽ���ߵ�λ������
//      2.RA1:�������ָ�룬����16bit��ʽ(out)
//      3.RA0:Dword����
//  ����ֵֵ:
//      ��
////////////////////////////////////////////////////////
Sub_AutoField SingleSerSquare
{
	push(RD2);

	int len = RD0.m_data;
	int x;
	for (int i = 0; i < len; i++)
	{
		RD0 = GET_M(RA0.m_data + i * MMU_BASE);
		RD2 = RD0;
		x = RD0.m_data;
		x = *(short*)(&x);					//��16bit 
		Multi24(x, x);
		RD1 = RD0;
		RD1 = RD1 >> 15;

		x = RD2.m_data;
		x = x >> 16;								//��16bit
		Multi24(x, x);
		RD0 = RD0 >> 15;

		RD0 = (RD0 << 16);
		RD0 += RD1;
		SET_M(RA1 + i * MMU_BASE, RD0);
	}

	pop(RD2);
	Return_AutoField(0);
}

////////////////////////////////////////////////////////
//  ����:
//      MultiSum
//  ����:
//      ˫���г��ۼ�����
//  ����:
//      RA0:��������1ָ�룬����16bit��ʽ����
//      RA1:��������2ָ�룬����16bit��ʽ����
//      RD1:�������ָ�룬����16bit��ʽ����(out),��Ϊ�㷨��֤�м���������Ҫʱ��ע��
//      RD0:���г���
//  ����ֵ:
//      RD0:���ۼӽ��
////////////////////////////////////////////////////////
Sub_AutoField MultiSum
{
	push(RA2);
	int len = RD0.m_data;

	RD0 = RD1;
	RA2 = RD0;

	RD0 = GET_M(RA0);   //����
	int a = *(short*)(&RD0.m_data);		//aΪ����1�ĵ�λ  
	int b = RD0 >> 16;				//bΪ����1�ĸ�λ
	RD1 = GET_M(RA1);
	int c = *(short*)(&RD1.m_data);		//cΪ����2�ĵ�λ  
	int d = RD1 >> 16;				//dΪ����2�ĸ�λ
	Multi24(a, c);
	int j = RD0.m_data;					//��һ�ֵ�λ�˻�(����32λ)
	RD0 = RD0 >> 15;
	RD1 = RD0;						//��λ�˻�����16bit��Ч��
	Multi24(b, d);
	int k = RD0.m_data;					//��һ�ָ�λ�˻�(����32λ)
	RD0 = RD0 >> 15;					//��λ�˻�����16bit��Ч��
	RD0 = (RD0 << 16);
	RD0 += RD1;
	//SET_M(RA2, RD0);

	for (int i = 1; i < len; i++)
	{
		RD0 = GET_M(RA0 + i * MMU_BASE);
		int a = *(short*)(&RD0);		//aΪ����1�ĵ�λ  
		int b = RD0 >> 16;				//bΪ����1�ĸ�λ
		RD1 = GET_M(RA1 + i * MMU_BASE);
		int c = *(short*)(&RD1);		//cΪ����2�ĵ�λ  
		int d = RD1 >> 16;				//dΪ����2�ĸ�λ

		Multi24(a, c);
		j += RD0.m_data;						//���ֵ�λ���ۼӺͣ�32bit��
		RD0 = j >> 15;
		RD0 = RD0 & 0XFFFF;				//intΪ32bit�з����������ǰ�����16λ
		RD1 = RD0;						//���ֵ�λ���ۼӺͣ�16bit��Ч��
		Multi24(b, d);					//���ֵ�λ���ۼӺͣ�32bit��
		k += RD0.m_data;
		RD0 = k >> 15;					//���ֵ�λ���ۼӺͣ���16bit��Ч��
		RD0 = (RD0 << 16);
		RD0 += RD1;
		//SET_M(RA2 + i * MMU_BASE, RD0);

	}
	//��ʱRD0��ֵ���������ۼӵ�ֵ

	pop(RA2);
	Return_AutoField(0);

}
////////////////////////////////////////////////////////
//  ����:
//      MAC_MultiConst16
//  ����:
//      Ϊ�����г˳�����������DMA_Ctrl����, Z(n)=X[n]*CONST/32767
//  ����:
//      1.M[RSP+3*MMU_BASE]��X(n) �׵�ַ���ֽڵ�ַ��
//      2.M[RSP+2*MMU_BASE]��Const ע��Ҫ���16λ���16λ��ͬ
//      3.M[RSP+1*MMU_BASE]��Z(n) �׵�ַ
//      4.M[RSP+0*MMU_BASE]�����ݳ���
//  ����ֵ:
//      ��
////////////////////////////////////////////////////////
Sub_AutoField MAC_MultiConst16
{
	RA0 = GET_M(RSP + 3 * MMU_BASE);
	RA1 = GET_M(RSP + 1 * MMU_BASE);
	RD0 = GET_M(RSP + 2 * MMU_BASE);
	RD1 = GET_M(RSP + 0 * MMU_BASE);

	Mac_Sim(RA0.m_data, RA0.m_data, RD0.m_data, 0x0C, RA1.m_data, RD1.m_data);

	Return_AutoField(4 * MMU_BASE);
}

////////////////////////////////////////////////////////
//  ����:
//      MultiConstH16L16
//  ����:
//      ������Const�������,Q15���;H16L16������
//  ����:
//      RA0:��������1ָ�룬����16bit��ʽ����
//      RA1:�������ָ�룬����16bit��ʽ����(out)
//      RD1:Constֵ
//      RD0:���г���
//  ����ֵ:
//      none
////////////////////////////////////////////////////////
Sub_AutoField MultiConstH16L16
{

	Mac_Sim(RA0.m_data, RA0.m_data, RD1.m_data, 0x0C, RA1.m_data, RD0.m_data);

	Return_AutoField(0);

}

////////////////////////////////////////////////////////
//  ��������:
//      MultiVector2
//  ��������:
//      ˫ʸ���˷���Q7���
//  ��ڲ���:
//      RA0:*in1
//      RA1:*in2
//      RD1:*out
//      RD0:���г���
//  ���ڲ���:
//      ��
////////////////////////////////////////////////////////
Sub_AutoField MultiVector2
{
	Mac_Sim(RA0.m_data, RA1.m_data, 0, 0x81, RD1.m_data, RD0.m_data);

	Return_AutoField(0);

}


////////////////////////////////////////////////////////
//  ��������:
//      Mac_Sim
//  ��������:
//      ģ��MacӲ������·��
//  ����:
//      addr_0:data0��ַ
//      addr_1:data1��ַ
//      addr_out:out��ַ
//		Const_Reg������ֵ
//		Config_Reg�����ò����������MAC����·��ʾ��ͼ
//      len:���г���
//  ע������:
//      ����������ģ��MAC���̣�����ȫ�档����ʹ�ó��ۼ�������ʹ��MultiSum������
////////////////////////////////////////////////////////
void Mac_Sim(int addr_0, int addr_1, int Const_Reg, unsigned int Config_Reg, int addr_out, int len)
{
	push(RA0);
	push(RA1);
	push(RA2);

	RD0 = addr_0;
	RA0 = RD0;
	RD0 = addr_1;
	RA1 = RD0;
	RD0 = addr_out;
	RA2 = RD0;

	for (int i = 0; i < len; i++)
	{
		RD0 = GET_M(RA0 + i * MMU_BASE);
		int a = *(short*)(&RD0);		//aΪ���еĵ�λ  
		int b = RD0 >> 16;				//bΪ���еĸ�λ
		int c, d;

		//��ȡ����ѡ�����
		RD1 = (Config_Reg & 0x1c) >> 2;
		if (RD1 == 0)
		{
			RD0 = GET_M(RA1 + i * MMU_BASE);
			c = *(short*)(&RD0);		//cΪ���еĵ�λ  
			d = RD0 >> 16;				//dΪ���еĸ�λ
			c = c << 8;					//24bit���˷���
			d = d << 8;
		}
		else if (RD1 == 1)
		{
			c = a;						//cΪ���еĵ�λ,��ƽ��ʱʹ�ã�DATA0=DATA1
			d = b;						//dΪ���еĸ�λ
			c = c << 8;					//24bit���˷���
			d = d << 8;
		}
		else if (RD1 == 2)
		{
			RD0 = GET_M(RA1 + i * MMU_BASE);
			c = RD0 >> 16;				//cΪ���еĵ�λ��ʵ������ʱʹ�ã���λ=��λ  
			d = RD0 >> 16;				//dΪ���еĸ�λ
			c = c << 8;					//24bit���˷���
			d = d << 8;
		}
		else if (RD1 == 3)
		{
			RD0 = Const_Reg;
			c = *(short*)(&RD0);		//cΪ���еĵ�λ���˳���ʱʹ��
			d = RD0 >> 16;				//dΪ���еĸ�λ
			c = c << 8;					//24bit���˷���
			d = d << 8;
		}
		else if (RD1 == 7)
		{
			RD0 = Const_Reg;
			d = Const_Reg & 0xffffff;	//�����ĵ�24λ���˳���ʱʹ��
			c = 0;						//��ʱM0�رչ�����c���������Կ�
		}
		else return;

		//ģ��˷�������
		Multi24_16x24(a, c);
		int M0 = RD0.m_data;
		Multi24_16x24(b, d);
		int M1 = RD0.m_data;

		//��ȡ���ѡ�����
		RD1 = Config_Reg & 0x3;
		if (RD1 == 0)
		{
			M0 = M0 >> 15;
			M0 = M0 & 0xffff;		//��λ
			M1 = M1 >> 15;
			M1 = M1 << 16;
			RD0 = M1 + M0;
			SET_M(RA2 + i * MMU_BASE, RD0);
		}
		else if (RD1 == 1)
		{
			M0 = M0 >> 7;
			M1 = M1 >> 7;
			if (Config_Reg & 0x80)
			{	//�޷�����
				if (M0 > 32767)                  //�޷�
					M0 = 32767;
				else if (M0 < -32767)
					M0 = -32767;
				if (M1 > 32767)
					M1 = 32767;
				else if (M1 < -32767)
					M1 = -32767;
			}
			M0 = M0 & 0xffff;		//��λ
			M1 = M1 << 16;
			RD0 = M1 + M0;
			SET_M(RA2 + i * MMU_BASE, RD0);
		}
		else if (RD1 == 2)
		{
			RD0 = M1 << 1;
			SET_M(RA2 + i * MMU_BASE, RD0);
		}
		else return;

	}

	pop(RA2);
	pop(RA1);
	pop(RA0);

}

////////////////////////////////////////////////////////
//  ����:
//      Multi24_16x24
//  ����:
//      �˷�����ͨ��c����ģ��Ӳ��24λ�˷������ԡ����漰��MACӲ���ĳ˷�����ʹ�ñ����������
//  ����:
//      1.Multi24_0:��������ӦMACӲ���ڵ�RA0���У�16bit��ʽ
//      2.Multi24_1:��������ӦMACӲ���ڵ�RA1���У�24bit��ʽ
//      3.RD0:�˷������32bit��ʽ
//  ����ֵֵ:
//      ��
////////////////////////////////////////////////////////
void Multi24_16x24(int Multi24_0, int Multi24_1)
{
	long long x;
	x = Multi24_1;
	if (Multi24_1 & 0x8000000)
	{
		x += 1;
	}
	if (Multi24_0 == 0xffff8000)
	{
		Multi24_0 += 1;
	}
	x = x * Multi24_0;
	x = x >> 8;
	RD0 = *(int*)(&x);					//��32bit 


}


////////////////////////////////////////////////////////
//  ��������:
//      ModulationToZero
//  ��������:
//      MACָ��Ϊ0x09, Q7���
//		��ʾ��1��ʵ������(M1-IN0,M0-IN1)����,����y(n).H16*x(n).H16 �Լ� y(n).H16*x(n).L16,
//      ��2��Q0��ʽ���,��Q0.H16 = DATA1.H16*DATA0.H16��M1�˷�����, Q0.L16 = DATA1.H16*DATA0.L16��M0�˷�����,
//      �μ�MAC����·��ʾ��ͼ��2���ĵ���
//  �������:
//      1.RA0: x(n), [H16|L16]��ʽ��32λ����������;
//      2.RA1: y(n), [H16|0]��ʽ�ĸ�16λ����������;
//      3.RD1: z(n) = x(n).*y(n)��Ŀ���ַ;
//      4.RD0: ���ݳ���
//  �������:
//      ��
//  ע������:
//      ��
////////////////////////////////////////////////////////
Sub_AutoField ModulationToZero
{
	Mac_Sim(RA0.m_data, RA1.m_data, 0, 0x09, RD1.m_data, RD0.m_data);

	Return_AutoField(0);

}
////////////////////////////////////////////////////////
//  ����:
//      DualSequMulti_Q0
//  ����:
//      ˫���г�,Q0���;MACָ��Ϊ0x00
//  ����:
//      1.RA0:��������1ָ��,����16bit��ʽ
//      2.RA1:��������2ָ��,����16bit��ʽ
//      3.RD0:Dword����
//  ����ֵֵ:
//      1.RD1:�������ָ��,����16bit��ʽ(out)
//  ע������:
//      ʵ�鲿ƽ��ֻ�ܲ���Q0��ʽ���,������Q1���޷����;
////////////////////////////////////////////////////////
Sub_AutoField DualSequMulti_Q0
{
	Mac_Sim(RA0.m_data, RA1.m_data, 0, 0x00, RD1.m_data, RD0.m_data);

	Return_AutoField(0);

}

////////////////////////////////////////////////////////
//  ����:
//      MultiConst24_DivQ7_LMT
//  ����:
//      ������Const������㣬MACָ��Ϊ0x9d
//  ����:
//      RA0:��������1ָ��,��16bit��ʽ����
//      RA1:�������ָ��,���Ϊ: QM1L[22:7],QM0L[22:7]
//      RD1:Constֵ,��8λΪ0
//      RD0:���г���
//  ����ֵ:
//      ��
//  ע������:
//      ��ʱ12.2us
////////////////////////////////////////////////////////
Sub_AutoField MultiConst24_DivQ7_LMT
{
	Mac_Sim(RA0.m_data, RA0.m_data, RD1.m_data, 0x9d, RA1.m_data, RD0.m_data);

	Return_AutoField(0);


}

////////////////////////////////////////////////////////
//  ��������:
//      SeqMulti_DivQ7
//  ��������:
//      ˫���г����㣬MACָ��Ϊ0x81
//  �������:
//      RA0:��������1ָ��,��16bit��ʽ����
//      RA1:��������2ָ��,��16bit��ʽ����
//      RD0:���г���
//  �������:
//      RD1:�������ָ��,���Ϊ��QM1L[22:7],QM0L[22:7]
//  ע������:
//      ��ʱ12.7us;
////////////////////////////////////////////////////////
Sub_AutoField SeqMulti_DivQ7
{
	int len = RD0.m_data;
	for (int i = 0; i < len; i++)
	{
		RD0 = GET_M(RA0.m_data + i * MMU_BASE);
		RD0 = RD0 & 0xffff0000;
		SET_M(RA0.m_data + i * MMU_BASE, RD0);
		RD0 = GET_M(RA1.m_data + i * MMU_BASE);
		RD0 = RD0 & 0xffff0000;
		SET_M(RA1.m_data + i * MMU_BASE, RD0);
	}

	Mac_Sim(RA0.m_data, RA1.m_data, 0, 0x81, RD1.m_data, len);

	Return_AutoField(0);

}

////////////////////////////////////////////////////////
//  ����:
//      ModulationToZero2
//  ����:
//      ����
//		MACָ��Ϊ0x08, Q7���
//  ����:
//      1.RA0:���ַ
//      2.RA1:��������ַ
//      3.RD1:Ŀ���ַ
//      4.RD0:���ݳ��ȶ�Ӧ��TimerNumֵ(Dword����*3+3)
//  ����ֵ:
//      ��
////////////////////////////////////////////////////////
Sub_AutoField ModulationToZero2
{
	Mac_Sim(RA0.m_data, RA1.m_data, 0, 0x08, RD1.m_data, RD0.m_data);

	Return_AutoField(0);

}

////////////////////////////////////////////////////////
//  ����:
//      SingleSerSquareL
//  ����:
//      ������ƽ������
//  ����:
//      1.RA0:��������ָ��,����16bit��ʽ
//      2.RA1:�������ָ��,����16bit��ʽ(out)
//      3.RD0:���г���
//  ����ֵֵ:
//      ��
//  ע������:
//      ʵ�鲿ƽ��ֻ�ܲ���Q0��ʽ���,������Q1���޷����;
////////////////////////////////////////////////////////
Sub_AutoField SingleSerSquareL
{
	Mac_Sim(RA0.m_data, RA0.m_data, 0, 0x00, RA1.m_data, RD0.m_data);

	Return_AutoField(0);

}

////////////////////////////////////////////////////////
//  ����:
//      SingleSerSquare32
//  ����:
//      ������ƽ�����㣬������Ϊ�������и�16λ��ƽ��������32λ���
//  ����:
//      1.RA0:��������ָ��,����16bit��ʽ
//      2.RA1:�������ָ��,����16bit��ʽ(out)
//      3.RD0:���г���
//  ����ֵֵ:
//      ��
//  ע������:
//      ������16λƽ�����
////////////////////////////////////////////////////////
Sub_AutoField SingleSerSquare32
{
	Mac_Sim(RA0.m_data, RA0.m_data, 0, 0x46, RA1.m_data, RD0.m_data);

	Return_AutoField(0);

}


////////////////////////////////////////////////////////
//  ����:
//      MAC_MultiConst16_Q2207
//  ����:
//      Ϊ�����г˳�����������DMA_Ctrl����
//  ����:
//      1.M[RSP+3*MMU_BASE]��X(n) �׵�ַ���ֽڵ�ַ��
//      2.M[RSP+2*MMU_BASE]��Const ע��Ҫ���16λ���16λ��ͬ
//      3.M[RSP+1*MMU_BASE]��Z(n) �׵�ַ
//      4.M[RSP+0*MMU_BASE]�����ݳ���
//  ����ֵ:
//      ��
////////////////////////////////////////////////////////
Sub_AutoField MAC_MultiConst16_Q2207
{
	RA0 = GET_M(RSP + 3 * MMU_BASE);
	RA1 = GET_M(RSP + 1 * MMU_BASE);
	RD0 = GET_M(RSP + 2 * MMU_BASE);
	RD1 = GET_M(RSP + 0 * MMU_BASE);

	Mac_Sim(RA0.m_data, RA0.m_data, RD0.m_data, 0x8D, RA1.m_data, RD1.m_data);

	Return_AutoField(4 * MMU_BASE);

}

////////////////////////////////////////////////////////
//  ����:
//      ComplexMulti 
//  ����:
//      ˫���и����˷�����
//  ����:
//      1.RA0����������0��ַ������16bit��ʽ
//      2.RA1����������1��ַ������16bit��ʽ
//      3.RD1��������е�ַ������16bit��ʽ
//      4.RD0�����ݳ���|�����ʽָ���λΪ���ݳ��ȣ���2λΪ�����ʽָ�00����16bit��01����16bit��10����16bit
//  ����ֵֵ:
//      ��
//  ע������:
//		��;
////////////////////////////////////////////////////////
Sub_AutoField ComplexMulti
{
	push(RA2);
	RA2 = RD1;

	int Q = RD0.m_data;		//config���ò���
	int len = Q >> 16;		//��λΪ���г���
	Q = RD0 & 0x3;			//��2bitΪ�����ʽָ�� 

	int A, B, C, D, AC, BD, AD, BC, H, L;
	long long x, y, z;
	y = 0x7fffffff;			//�޷�����
	z = 0xffffffff80000000;

	for (int i = 0; i < len; i++)
	{
		//���ݹ�ʽ��(A+Bi)*(C+Di)=(AC-BD)+(AD+BC)i
		RD0 = M[RA0 + i * MMU_BASE];
		A = RD0 >> 16;
		B = *(short*)(&RD0);
		RD1 = M[RA1 + i * MMU_BASE];
		C = RD1 >> 16;
		D = *(short*)(&RD1);
		AC = A * C;
		BD = B * D;
		AD = A * D;
		BC = B * C;

		x = AC;
		x -= BD;
		if (x > y)	//�޷�
			x = y;
		else if (x < z)
			x = z;
		H = x;

		x = AD;
		x += BC;
		if (x > y)	//�޷�
			x = y;
		else if (x < z)
			x = z;
		L = x;

		if (Q == 0)
		{
			H = 0xFFFF & H;
			L = 0xFFFF & L;
			RD0 = (H << 16) + L;
			M[RA2 + i * MMU_BASE] = RD0;
		}
		else if (Q == 1)
		{
			H = 0xFFFF & (H >> 8);
			L = 0xFFFF & (L >> 8);
			RD0 = (H << 16) + L;
			M[RA2 + i * MMU_BASE] = RD0;
		}
		else if (Q == 2)
		{
			H = 0xFFFF0000 & H;
			L = (L >> 16) & 0xFFFF;
			RD0 = H + L;
			M[RA2 + i * MMU_BASE] = RD0;
		}
		else
		{
			pop(RA2);
			return;
		}
	}

	pop(RA2);
	Return_AutoField(0);

}


////////////////////////////////////////////////////////
//  ��������:
//      Mac_Sim32
//  ��������:
//      ģ��32λ��MacӲ������·��
//  ����:
//      addr_0:data0��ַ
//      addr_1:data1��ַ
//      addr_out:out��ַ
//		Const_Reg������ֵ
//		Config_Reg�����ò���
//      len:���г���
//  ע������:
//      ��������(�ݶ�):
//		bit8 �Ƿ�ص�X*C0+Y*C1��ʽ�µ�Y*C1�˷�,0-����;1-�ص�
//		bit3,4��������00XX:X*C0+Y*C1(32bit˫���г˳���);01XX:X*Y=Z(32bit�˷�)
//		bit0��1����ȡֵ��00/01/10�ֱ��Ӧ�ӵ͵����������ȡֵ.00-[b31:b0];01-[b38:b7];10-[b46:b15];
////////////////////////////////////////////////////////
void Mac_Sim32(int addr_0, int addr_1, int Const_Reg, unsigned int Config_Reg, int addr_out, int len)
{
	push(RA0);
	push(RA1);
	push(RA2);

	RD0 = addr_0;
	RA0 = RD0;
	RD0 = addr_1;
	RA1 = RD0;
	RD0 = addr_out;
	RA2 = RD0;

	for (int i = 0; i < len; i++)
	{
		RD0 = M[RA0++];
		long long X = RD0.m_data;
		RD0 = M[RA1++];
		long long Y = RD0.m_data;

		////��ȡ����ѡ�����
		RD0 = (Config_Reg & 0x3);
		RD1 = (Config_Reg & 0xc) >> 2;
		if (RD1 == 0)//X*C0+Y*C1
		{
			int C0 = Const_Reg >> 16;
			int C1 = *(short*)(&Const_Reg);
			X = X * C0;

			int Config_Reg_b8 = (Config_Reg & 0x100) >> 8;
			if (Config_Reg_b8 == 0)
			{
				Y = Y * C1;
			}
			else
			{
				Y = 0;	// ����ڶ�����
			}

			X += Y;
			if (RD0 == 0)
			{
				X &= 0xFFFFFFFF;
			}
			else if (RD0 == 1)
			{
				X = (X >> 7) & 0xFFFFFFFF;
			}
			else if (RD0 == 2)
			{
				X = (X >> 15) & 0xFFFFFFFF;
			}
			else return;
		}
		else if (RD1 == 1)
		{
			X = X * Y;
			if (RD0 == 0)
			{
				X &= 0xFFFFFFFF;
			}
			else if (RD0 == 1)
			{
				X = (X >> 15) & 0xFFFFFFFF;
			}
			else if (RD0 == 2)
			{
				X = (X >> 31) & 0xFFFFFFFF;
			}
			else return;
		}
		else return;

		RD0 = X;
		M[RA2++] = RD0;

	}
	pop(RA2);
	pop(RA1);
	pop(RA0);

}

////////////////////////////////////////////////////////
//  ��������:
//      MultiConst32_Dual_Q4615
//  ��������:
//      32bit˫���г˳�����������㣬X*C0+Y*C1
//  �������:
//      RA0:��������1ָ��,32bit��ʽ����
//      RA1:��������2ָ��,32bit��ʽ����
//      RD0:���г���
//      RD1:����������16bit��ʽ
//  �������:
//      RA2:�������ָ��,���Ϊ48λ�еĸ�32λ[b46:b15]
//  ע������:
//      
////////////////////////////////////////////////////////
Sub_AutoField MultiConst32_Dual_Q4615
{
	Mac_Sim32(RA0.m_data, RA1.m_data, RD1.m_data, 0x2, RA2.m_data, RD0.m_data);//��32
	//Mac_Sim32(RA0.m_data, RA1.m_data, RD1.m_data, 0x1, RA2.m_data, RD0.m_data);//��32
	//Mac_Sim32(RA0.m_data, RA1.m_data, RD1.m_data, 0x0, RA2.m_data, RD0.m_data);//��32

	Return_AutoField(0);
}

////////////////////////////////////////////////////////
//  ��������:
//      MultiConst32_Single_Q4615
//  ��������:
//      32bit�����г˳�����X*C0
//  �������:
//      RA0:��������1ָ��,32bit��ʽ����
//      RD0:���г���
//      RD1:��������16bit��Ч
//  �������:
//      RA1:�������ָ��,���Ϊ48λ�еĸ�32λ[b46:b15]
//  ע������:
//      
////////////////////////////////////////////////////////
Sub_AutoField MultiConst32_Single_Q4615
{
	Mac_Sim32(RA0.m_data, RA0.m_data, RD1.m_data, 0x102, RA1.m_data, RD0.m_data);//��32

	Return_AutoField(0);
}
////////////////////////////////////////////////////////
//  ��������:
//      MultiConst32_Single_Q3807
//  ��������:
//      32bit�����г˳�����X*C0
//  �������:
//      RA0:��������1ָ��,32bit��ʽ����
//      RD0:���г���
//      RD1:��������16bit��Ч
//  �������:
//      RA1:�������ָ��,���Ϊ48λ�е���32λ[b38:b07]
//  ע������:
//      
////////////////////////////////////////////////////////
Sub_AutoField MultiConst32_Single_Q3807
{
	Mac_Sim32(RA0.m_data, RA0.m_data, RD1.m_data, 0x101, RA1.m_data, RD0.m_data);//��32

	Return_AutoField(0);
}
////////////////////////////////////////////////////////
//  ��������:
//      MultiConst32_Single_Q3100
//  ��������:
//      32bit�����г˳�����X*C0
//  �������:
//      RA0:��������1ָ��,32bit��ʽ����
//      RD0:���г���
//      RD1:��������16bit��Ч
//  �������:
//      RA1:�������ָ��,���Ϊ48λ�еĵ�32λ[b31:b00]
//  ע������:
//      
////////////////////////////////////////////////////////
Sub_AutoField MultiConst32_Single_Q3100
{
	Mac_Sim32(RA0.m_data, RA0.m_data, RD1.m_data, 0x100, RA1.m_data, RD0.m_data);//��32

	Return_AutoField(0);
}


////////////////////////////////////////////////////////
//  ��������:
//      SeqMulti_32
//  ��������:
//      32bit˫���г�����
//  �������:
//      RA0:��������1ָ��,32bit��ʽ����
//      RA1:��������2ָ��,32bit��ʽ����
//      RD0:���г���
//  �������:
//      RD1:�������ָ��,���Ϊ64λ�и�32λ��
//  ע������:
//      
////////////////////////////////////////////////////////
Sub_AutoField SeqMulti_32
{
	Mac_Sim32(RA0.m_data, RA1.m_data, 0, 0x6, RD1.m_data, RD0.m_data);//��32bit
	//Mac_Sim32(RA0.m_data, RA1.m_data, 0, 0x5, RD1.m_data, RD0.m_data);//�м�32bit
	//Mac_Sim32(RA0.m_data, RA1.m_data, 0, 0x4, RD1.m_data, RD0.m_data);//��32bit

	Return_AutoField(0);
}
