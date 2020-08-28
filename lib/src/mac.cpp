#include "mac.h"
#include "memory.h"
#include <math.h>

//模拟器内部函数，无需用类硬件写法
extern void Multi24(int Multi24_0, int Multi24_1);
extern void Mac_Sim(int addr_0, int addr_1, int Const_Reg, unsigned int Config_Reg, int addr_out, int len);
extern void Multi24_16x24(int Multi24_0, int Multi24_1);


//本函数库本质是给定不同参数调取Mac硬件，模拟器中实际表现为调取Mac_Sim函数
//本函数库分两批写成，前面约一半为各自完成功能，后一半使用了Mac_Sim模拟算法

////////////////////////////////////////////////////////
//  名称:
//      Multi24
//  功能:
//      乘法器，通过c语言模拟硬件24位乘法器特性。凡涉及到MAC硬件的乘法都需使用本函数替代。
//  参数:
//      1.Multi24_0:乘数，对应MAC硬件内的RA0序列，16bit格式
//      2.Multi24_1:乘数，对应MAC硬件内的RA1序列，16bit格式进来，计算时扩充为24bit计算
//      3.RD0:乘法结果，32bit格式
//  返回值值:
//      无
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
	RD0 = *(int*)(&x);					//低32bit 


}



////////////////////////////////////////////////////////
//  名称:
//      SingleSerSquare
//  功能:
//      单序列平方运算
//  参数:
//      1.RA0:输入序列指针，紧凑16bit格式，高低位都有数
//      2.RA1:输出序列指针，紧凑16bit格式(out)
//      3.RA0:Dword长度
//  返回值值:
//      无
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
		x = *(short*)(&x);					//低16bit 
		Multi24(x, x);
		RD1 = RD0;
		RD1 = RD1 >> 15;

		x = RD2.m_data;
		x = x >> 16;								//高16bit
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
//  名称:
//      MultiSum
//  功能:
//      双序列乘累加运算
//  参数:
//      RA0:输入序列1指针，紧凑16bit格式序列
//      RA1:输入序列2指针，紧凑16bit格式序列
//      RD1:输出序列指针，紧凑16bit格式序列(out)
//      RD0:序列长度
//  返回值:
//      RD0:乘累加结果
////////////////////////////////////////////////////////
Sub_AutoField MultiSum
{
	push(RA2);
	int len = RD0.m_data;

	RD0 = RD1;
	RA2 = RD0;

	RD0 = GET_M(RA0);   //首数
	int a = *(short*)(&RD0.m_data);		//a为序列1的低位  
	int b = RD0 >> 16;				//b为序列1的高位
	RD1 = GET_M(RA1);
	int c = *(short*)(&RD1.m_data);		//c为序列2的低位  
	int d = RD1 >> 16;				//d为序列2的高位
	Multi24(a, c);
	int j = RD0.m_data;					//第一轮低位乘积(保留32位)
	RD0 = RD0 >> 15;
	RD1 = RD0;						//低位乘积（低16bit有效）
	Multi24(b, d);
	int k = RD0.m_data;					//第一轮高位乘积(保留32位)
	RD0 = RD0 >> 15;					//高位乘积（低16bit有效）
	RD0 = (RD0 << 16);
	RD0 += RD1;
	SET_M(RA2, RD0);

	for (int i = 1; i < len; i++)
	{
		RD0 = GET_M(RA0 + i * MMU_BASE);
		int a = *(short*)(&RD0);		//a为序列1的低位  
		int b = RD0 >> 16;				//b为序列1的高位
		RD1 = GET_M(RA1 + i * MMU_BASE);
		int c = *(short*)(&RD1);		//c为序列2的低位  
		int d = RD1 >> 16;				//d为序列2的高位

		Multi24(a, c);
		j += RD0.m_data;						//本轮低位乘累加和（32bit）
		RD0 = j >> 15;
		RD0 = RD0 & 0XFFFF;				//int为32bit有符号数，相加前清除高16位
		RD1 = RD0;						//本轮低位乘累加和（16bit有效）
		Multi24(b, d);					//本轮低位乘累加和（32bit）
		k += RD0.m_data;
		RD0 = k >> 15;					//本轮低位乘累加和（低16bit有效）
		RD0 = (RD0 << 16);
		RD0 += RD1;
		SET_M(RA2 + i * MMU_BASE, RD0);

	}
	//此时RD0的值就是最后乘累加的值

	pop(RA2);
	Return_AutoField(0);

}
////////////////////////////////////////////////////////
//  名称:
//      MAC_MultiConst16
//  功能:
//      为单序列乘常量操作配置DMA_Ctrl参数, Z(n)=X[n]*CONST/32767
//  参数:
//      1.M[RSP+3*MMU_BASE]：X(n) 首地址（字节地址）
//      2.M[RSP+2*MMU_BASE]：Const 注意要求高16位与低16位相同
//      3.M[RSP+1*MMU_BASE]：Z(n) 首地址
//      4.M[RSP+0*MMU_BASE]：数据长度
//  返回值:
//      无
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
//  名称:
//      MultiConstH16L16
//  功能:
//      序列与Const相乘运算,Q15输出;H16L16都有数
//  参数:
//      RA0:输入序列1指针，紧凑16bit格式序列
//      RA1:输出序列指针，紧凑16bit格式序列(out)
//      RD1:Const值
//      RD0:序列长度
//  返回值:
//      none
////////////////////////////////////////////////////////
Sub_AutoField MultiConstH16L16
{

	Mac_Sim(RA0.m_data, RA0.m_data, RD1.m_data, 0x0C, RA1.m_data, RD0.m_data);

	Return_AutoField(0);

}

////////////////////////////////////////////////////////
//  函数名称:
//      MultiVector2
//  函数功能:
//      双矢量乘法，Q7输出
//  入口参数:
//      RA0:*in1
//      RA1:*in2
//      RD1:*out
//      RD0:序列长度
//  出口参数:
//      无
////////////////////////////////////////////////////////
Sub_AutoField MultiVector2
{
	Mac_Sim(RA0.m_data, RA1.m_data, 0, 0x81, RD1.m_data, RD0.m_data);

	Return_AutoField(0);

}


////////////////////////////////////////////////////////
//  函数名称:
//      Mac_Sim
//  函数功能:
//      模拟Mac硬件数据路径
//  参数:
//      addr_0:data0地址
//      addr_1:data1地址
//      addr_out:out地址
//		Const_Reg：常数值
//		Config_Reg：配置参数，详情见MAC数据路径示意图
//      len:序列长度
//  注意事项:
//      本函数用来模拟MAC流程，并不全面。如需使用乘累加器，请使用MultiSum函数。
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
		int a = *(short*)(&RD0);		//a为序列的低位  
		int b = RD0 >> 16;				//b为序列的高位
		int c, d;

		//提取输入选择参数
		RD1 = (Config_Reg & 0x1c) >> 2;
		if (RD1 == 0)
		{
			RD0 = GET_M(RA1 + i * MMU_BASE);
			c = *(short*)(&RD0);		//c为序列的低位  
			d = RD0 >> 16;				//d为序列的高位
			c = c << 8;					//24bit进乘法器
			d = d << 8;
		}
		else if (RD1 == 1)
		{
			c = a;						//c为序列的低位,求平方时使用，DATA0=DATA1
			d = b;						//d为序列的高位
			c = c << 8;					//24bit进乘法器
			d = d << 8;
		}
		else if (RD1 == 2)
		{
			RD0 = GET_M(RA1 + i * MMU_BASE);
			c = RD0 >> 16;				//c为序列的低位，实部调制时使用，低位=高位  
			d = RD0 >> 16;				//d为序列的高位
			c = c << 8;					//24bit进乘法器
			d = d << 8;
		}
		else if (RD1 == 3)
		{
			RD0 = Const_Reg;
			c = *(short*)(&RD0);		//c为序列的低位，乘常数时使用
			d = RD0 >> 16;				//d为序列的高位
			c = c << 8;					//24bit进乘法器
			d = d << 8;
		}
		else if (RD1 == 7)
		{
			RD0 = Const_Reg;
			d = Const_Reg & 0xffffff;	//常数的低24位，乘常数时使用
			c = 0;						//此时M0关闭工作，c给任意数皆可
		}
		else return;

		//模拟乘法器特性
		Multi24_16x24(a, c);
		int M0 = RD0.m_data;
		Multi24_16x24(b, d);
		int M1 = RD0.m_data;

		//提取输出选择参数
		RD1 = Config_Reg & 0x3;
		if (RD1 == 0)
		{
			M0 = M0 >> 15;
			M0 = M0 & 0xffff;		//低位
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
			{	//限幅器打开
				if (M0 > 32767)                  //限幅
					M0 = 32767;
				else if (M0 < -32767)
					M0 = -32767;
				if (M1 > 32767)
					M1 = 32767;
				else if (M1 < -32767)
					M1 = -32767;
			}
			M0 = M0 & 0xffff;		//低位
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
//  名称:
//      Multi24_16x24
//  功能:
//      乘法器，通过c语言模拟硬件24位乘法器特性。凡涉及到MAC硬件的乘法都需使用本函数替代。
//  参数:
//      1.Multi24_0:乘数，对应MAC硬件内的RA0序列，16bit格式
//      2.Multi24_1:乘数，对应MAC硬件内的RA1序列，24bit格式
//      3.RD0:乘法结果，32bit格式
//  返回值值:
//      无
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
	RD0 = *(int*)(&x);					//低32bit 


}


////////////////////////////////////////////////////////
//  函数名称:
//      ModulationToZero
//  函数功能:
//      MAC指令为0x09, Q7输出
//		表示（1）实部调制(M1-IN0,M0-IN1)调制,计算y(n).H16*x(n).H16 以及 y(n).H16*x(n).L16,
//      （2）Q0方式输出,即Q0.H16 = DATA1.H16*DATA0.H16（M1乘法器）, Q0.L16 = DATA1.H16*DATA0.L16（M0乘法器）,
//      参见MAC数据路径示意图（2）文档。
//  输入参数:
//      1.RA0: x(n), [H16|L16]格式的32位操作数序列;
//      2.RA1: y(n), [H16|0]格式的高16位操作数序列;
//      3.RD1: z(n) = x(n).*y(n)的目标地址;
//      4.RD0: 数据长度
//  输出参数:
//      无
//  注意事项:
//      无
////////////////////////////////////////////////////////
Sub_AutoField ModulationToZero
{
	Mac_Sim(RA0.m_data, RA1.m_data, 0, 0x09, RD1.m_data, RD0.m_data);

	Return_AutoField(0);

}
////////////////////////////////////////////////////////
//  名称:
//      DualSequMulti_Q0
//  功能:
//      双序列乘,Q0输出;MAC指令为0x00
//  参数:
//      1.RA0:输入序列1指针,紧凑16bit格式
//      2.RA1:输入序列2指针,紧凑16bit格式
//      3.RD0:Dword长度
//  返回值值:
//      1.RD1:输出序列指针,紧凑16bit格式(out)
//  注意事项:
//      实虚部平方只能采用Q0方式输出,不可以Q1带限幅输出;
////////////////////////////////////////////////////////
Sub_AutoField DualSequMulti_Q0
{
	Mac_Sim(RA0.m_data, RA1.m_data, 0, 0x00, RD1.m_data, RD0.m_data);

	Return_AutoField(0);

}

////////////////////////////////////////////////////////
//  名称:
//      MultiConst24_DivQ7_LMT
//  功能:
//      序列与Const相乘运算，MAC指令为0x9d
//  参数:
//      RA0:输入序列1指针,高16bit格式序列
//      RA1:输出序列指针,结果为: QM1L[22:7],QM0L[22:7]
//      RD1:Const值,低8位为0
//      RD0:序列长度
//  返回值:
//      无
//  注意事项:
//      耗时12.2us
////////////////////////////////////////////////////////
Sub_AutoField MultiConst24_DivQ7_LMT
{
	Mac_Sim(RA0.m_data, RA0.m_data, RD1.m_data, 0x9d, RA1.m_data, RD0.m_data);

	Return_AutoField(0);


}

////////////////////////////////////////////////////////
//  函数名称:
//      SeqMulti_DivQ7
//  函数功能:
//      双序列乘运算，MAC指令为0x81
//  输入参数:
//      RA0:输入序列1指针,高16bit格式序列
//      RA1:输入序列2指针,高16bit格式序列
//      RD0:序列长度
//  输出参数:
//      RD1:输出序列指针,结果为；QM1L[22:7],QM0L[22:7]
//  注意事项:
//      耗时12.7us;
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
//  名称:
//      ModulationToZero2
//  功能:
//      调制
//		MAC指令为0x08, Q7输出
//  参数:
//      1.RA0:表地址
//      2.RA1:操作数地址
//      3.RD1:目标地址
//      4.RD0:数据长度对应的TimerNum值(Dword长度*3+3)
//  返回值:
//      无
////////////////////////////////////////////////////////
Sub_AutoField ModulationToZero2
{
	Mac_Sim(RA0.m_data, RA1.m_data, 0, 0x08, RD1.m_data, RD0.m_data);

	Return_AutoField(0);

}

////////////////////////////////////////////////////////
//  名称:
//      SingleSerSquareL
//  功能:
//      单序列平方运算
//  参数:
//      1.RA0:输入序列指针,紧凑16bit格式
//      2.RA1:输出序列指针,紧凑16bit格式(out)
//      3.RD0:序列长度
//  返回值值:
//      无
//  注意事项:
//      实虚部平方只能采用Q0方式输出,不可以Q1带限幅输出;
////////////////////////////////////////////////////////
Sub_AutoField SingleSerSquareL
{
	Mac_Sim(RA0.m_data, RA0.m_data, 0, 0x00, RA1.m_data, RD0.m_data);

	Return_AutoField(0);

}

////////////////////////////////////////////////////////
//  名称:
//      SingleSerSquare32
//  功能:
//      单序列平方运算，输出结果为输入序列高16位的平方，保留32位结果
//  参数:
//      1.RA0:输入序列指针,紧凑16bit格式
//      2.RA1:输出序列指针,紧凑16bit格式(out)
//      3.RD0:序列长度
//  返回值值:
//      无
//  注意事项:
//      舍弃低16位平方结果
////////////////////////////////////////////////////////
Sub_AutoField SingleSerSquare32
{
	Mac_Sim(RA0.m_data, RA0.m_data, 0, 0x46, RA1.m_data, RD0.m_data);

	Return_AutoField(0);

}


////////////////////////////////////////////////////////
//  名称:
//      MAC_MultiConst16_Q2207
//  功能:
//      为单序列乘常量操作配置DMA_Ctrl参数
//  参数:
//      1.M[RSP+3*MMU_BASE]：X(n) 首地址（字节地址）
//      2.M[RSP+2*MMU_BASE]：Const 注意要求高16位与低16位相同
//      3.M[RSP+1*MMU_BASE]：Z(n) 首地址
//      4.M[RSP+0*MMU_BASE]：数据长度
//  返回值:
//      无
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

