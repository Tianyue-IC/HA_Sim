#include "mac.h"
#include "memory.h"
#include <math.h>
#include "global.h"

//模拟器内部函数，无需用类硬件写法
extern void Multi24(int Multi24_0, int Multi24_1);
extern void Mac_Sim(int addr_0, int addr_1, int Const_Reg, unsigned int Config_Reg, int addr_out, int len);
extern void Multi24_16x24(int Multi24_0, int Multi24_1);
extern void Mac_Sim32(int addr_0, int addr_1, int Const_Reg, unsigned int Config_Reg, int addr_out, int len);

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
//      RD1:输出序列指针，紧凑16bit格式序列(out),此为算法验证中间数，有需要时打开注释
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
	//SET_M(RA2, RD0);

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
		//SET_M(RA2 + i * MMU_BASE, RD0);

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

////////////////////////////////////////////////////////
//  名称:
//      ComplexMulti 
//  功能:
//      双序列复数乘法运算
//  参数:
//      1.RA0：输入序列0地址，紧凑16bit格式
//      2.RA1：输入序列1地址，紧凑16bit格式
//      3.RD1：输出序列地址，紧凑16bit格式
//      4.RD0：数据长度|输出格式指令。高位为数据长度，低2位为输出格式指令；00：低16bit，01：中16bit，10：高16bit
//  返回值值:
//      无
//  注意事项:
//		无;
////////////////////////////////////////////////////////
Sub_AutoField ComplexMulti
{
	push(RA2);
	RA2 = RD1;

	int Q = RD0.m_data;		//config配置参数
	int len = Q >> 16;		//高位为序列长度
	Q = RD0 & 0x3;			//低2bit为输出格式指令 

	int A, B, C, D, AC, BD, AD, BC, H, L;
	long long x, y, z;
	y = 0x7fffffff;			//限幅数据
	z = 0xffffffff80000000;

	for (int i = 0; i < len; i++)
	{
		//根据公式：(A+Bi)*(C+Di)=(AC-BD)+(AD+BC)i
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
		if (x > y)	//限幅
			x = y;
		else if (x < z)
			x = z;
		H = x;

		x = AD;
		x += BC;
		if (x > y)	//限幅
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
//  函数名称:
//      Mac_Sim32
//  函数功能:
//      模拟32位宽Mac硬件数据路径
//  参数:
//      addr_0:data0地址
//      addr_1:data1地址
//      addr_out:out地址
//		Const_Reg：常数值
//		Config_Reg：配置参数
//      len:序列长度
//  注意事项:
//      参数编码(暂定):
//		bit8 是否关掉X*C0+Y*C1形式下的Y*C1乘法,0-不关;1-关掉
//		bit3,4决定功能00XX:X*C0+Y*C1(32bit双序列乘常量);01XX:X*Y=Z(32bit乘法)
//		bit0，1决定取值，00/01/10分别对应从低到高三档结果取值.00-[b31:b0];01-[b38:b7];10-[b46:b15];
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

		////提取配置选择参数
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
				Y = 0;	// 不算第二部分
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
//  函数名称:
//      MultiConst32_Dual_Q4615
//  函数功能:
//      32bit双序列乘常数再相加运算，X*C0+Y*C1
//  输入参数:
//      RA0:输入序列1指针,32bit格式序列
//      RA1:输入序列2指针,32bit格式序列
//      RD0:序列长度
//      RD1:常数，紧凑16bit格式
//  输出参数:
//      RA2:输出序列指针,结果为48位中的高32位[b46:b15]
//  注意事项:
//      
////////////////////////////////////////////////////////
Sub_AutoField MultiConst32_Dual_Q4615
{
	Mac_Sim32(RA0.m_data, RA1.m_data, RD1.m_data, 0x2, RA2.m_data, RD0.m_data);//高32
	//Mac_Sim32(RA0.m_data, RA1.m_data, RD1.m_data, 0x1, RA2.m_data, RD0.m_data);//中32
	//Mac_Sim32(RA0.m_data, RA1.m_data, RD1.m_data, 0x0, RA2.m_data, RD0.m_data);//低32

	Return_AutoField(0);
}

////////////////////////////////////////////////////////
//  函数名称:
//      MultiConst32_Single_Q4615
//  函数功能:
//      32bit单序列乘常数，X*C0
//  输入参数:
//      RA0:输入序列1指针,32bit格式序列
//      RD0:序列长度
//      RD1:常数，低16bit有效
//  输出参数:
//      RA1:输出序列指针,结果为48位中的高32位[b46:b15]
//  注意事项:
//      
////////////////////////////////////////////////////////
Sub_AutoField MultiConst32_Single_Q4615
{
	Mac_Sim32(RA0.m_data, RA0.m_data, RD1.m_data, 0x102, RA1.m_data, RD0.m_data);//高32

	Return_AutoField(0);
}
////////////////////////////////////////////////////////
//  函数名称:
//      MultiConst32_Single_Q3807
//  函数功能:
//      32bit单序列乘常数，X*C0
//  输入参数:
//      RA0:输入序列1指针,32bit格式序列
//      RD0:序列长度
//      RD1:常数，低16bit有效
//  输出参数:
//      RA1:输出序列指针,结果为48位中的中32位[b38:b07]
//  注意事项:
//      
////////////////////////////////////////////////////////
Sub_AutoField MultiConst32_Single_Q3807
{
	Mac_Sim32(RA0.m_data, RA0.m_data, RD1.m_data, 0x101, RA1.m_data, RD0.m_data);//中32

	Return_AutoField(0);
}
////////////////////////////////////////////////////////
//  函数名称:
//      MultiConst32_Single_Q3100
//  函数功能:
//      32bit单序列乘常数，X*C0
//  输入参数:
//      RA0:输入序列1指针,32bit格式序列
//      RD0:序列长度
//      RD1:常数，低16bit有效
//  输出参数:
//      RA1:输出序列指针,结果为48位中的低32位[b31:b00]
//  注意事项:
//      
////////////////////////////////////////////////////////
Sub_AutoField MultiConst32_Single_Q3100
{
	Mac_Sim32(RA0.m_data, RA0.m_data, RD1.m_data, 0x100, RA1.m_data, RD0.m_data);//低32

	Return_AutoField(0);
}


////////////////////////////////////////////////////////
//  函数名称:
//      SeqMulti_32
//  函数功能:
//      32bit双序列乘运算
//  输入参数:
//      RA0:输入序列1指针,32bit格式序列
//      RA1:输入序列2指针,32bit格式序列
//      RD0:序列长度
//  输出参数:
//      RD1:输出序列指针,结果为64位中高32位；
//  注意事项:
//      
////////////////////////////////////////////////////////
Sub_AutoField SeqMulti_32
{
	Mac_Sim32(RA0.m_data, RA1.m_data, 0, 0x6, RD1.m_data, RD0.m_data);//高32bit
	//Mac_Sim32(RA0.m_data, RA1.m_data, 0, 0x5, RD1.m_data, RD0.m_data);//中间32bit
	//Mac_Sim32(RA0.m_data, RA1.m_data, 0, 0x4, RD1.m_data, RD0.m_data);//低32bit

	Return_AutoField(0);
}
