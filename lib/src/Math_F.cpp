#include "Math_F.h"
#include "memory.h"
#include <math.h>


////////////////////////////////////////////////////////
//  名称:
//      power_fix
//  功能:
//      定点求Power = 10^(RD0/10)
//  参数:
//      1.RD0: 数据
//  返回值:
//      1.RD0: 结果（32q8）
////////////////////////////////////////////////////////
Sub_AutoField power_fix
{
    push(RD4);
    push(RD5);
    push(RD6);
    push(RD7);

    #define E  RD4
    #define MM RD5
    #define x  RD6
    #define r  RD7

    x = RD0;

    // 1. 计算x = Level*0.33219 = (Level*10885)/Q15
    // x = level * 10885;
    //    RD0 = 10885;
    //    Multi24_X=RD0;
    //    RD0 = x;
    //    Multi24_Y=RD0;
    //    nop;
    //    RD0=Multi24_XY;
    //    RD0_ClrByteH8;
        //x = RD0;

    RD1 = 10885;
    call_AutoField _Rs_Multi;

    //x >>= 15;
    RD1 = 15;
    call_AutoField _Rf_ShiftR_Reg;
    x = RD0;

    // 2. 计算E和r，根据公式 x = (E+1) + (r-1),  E为正整数 ， r为[0,1)的数
    // E = (x >> 8) + 1;               // x是16q8，低8位为小数
    //RD0 = x;
    RD1 = 8;
    call_AutoField _Rf_ShiftR_Reg;
    RD0+=1;
    E = RD0;

    // r = x & 0xFF;
    RD0 = x;
    RF_GetL8(RD0);
    r = RD0;

    //  if ((r & 0x1) == 1)         // 判bit0是否为1
    if (RD0_Bit0 == 0) goto L_power_fix_0;
    //r += 2;
    r += 2;

L_power_fix_0:
    //r = r >> 1;                   // 移剩下的1
    RF_ShiftR1(r);

    // 如果r==N,r-=1
    // if (r == N)
    RD1 = 128;
    RD1 -= r;
    if (RQ_nZero) goto L_power_fix_1;
    // r -= 1;
    r-=1;
L_power_fix_1:

    // 2. 以M为地址读取结果
    //rst = (1 << E) * exp2_table[r];
    RD0 = E;
    RF_Exp(RD0);
    RD2 = RD0;

    RD0 = r;
    RF_ShiftL1(RD0);
    RD1 = RN_Pow2_Table_ADDR;
    RF_GetL16(RD1);
    RF_ShiftL2(RD1);
    RD0 += RD1;
    call ConstROM_Read_Word;
    RD1 = RD2;

    call_AutoField _Rs_Multi;
    //    Multi24_X=RD1;
    //    //RD0 = x;
    //    Multi24_Y=RD0;
    //    nop;
    //    RD0=Multi24_XY;
    //    RD0_ClrByteH8;

    #undef E
    #undef MM
    #undef x
    #undef r

    pop(RD7);
    pop(RD6);
    pop(RD5);
    pop(RD4);

    Return_AutoField(0);

}


////////////////////////////////////////////////////////
//  名称:
//      recip_fix
//  功能:
//      定点求倒数
//  参数:
//      1.RD0: 数据(q0)
//  返回值:
//      1.RD0: 结果（q23）
////////////////////////////////////////////////////////
Sub_AutoField recip_fix
{

    push(RD4);
    push(RD5);
    push(RD6);

    #define E  RD4
    #define MM RD5
    #define x  RD6

    RD2 = 0;
    if (RD0_Bit31 == 0) goto L_recip_fix_2;
    RD2 = 1;
    RF_Not(RD0);
    RD0 += 1;
L_recip_fix_2:
    // 1. 计算E和M，根据公式 x = 2^E * M,  E为正整数 ， M为[0.5~1)的数
    // E = log2(x);                 // 汇编有CPU指令支持
    x = RD0;
    RF_Log(RD0);
    E = RD0;

    // M = x << index;                 // 汇编可优化成移位
    RD0 = x;
    RD1 = 8;
    call_AutoField _Rf_ShiftL_Reg;
    MM = RD0;

    // M = M >> (E);                   // 先移log2(x)-1    目的是四舍五入
    RD0 = MM;
    RD1 = E;
    call_AutoField _Rf_ShiftR_Reg;
    MM = RD0;

    // if ((M & 0x1) == 1)             // 判bit0是否为1
    if (RD0_Bit0 == 0) goto L_recip_fix_0;
    // M += 2;
    MM += 2;

L_recip_fix_0:
    // M = (M >> 1) - 128;                 // 移剩下的1
    RD0 = MM;
    RF_ShiftR1(RD0);
    RD0 -= 128;
    MM = RD0;

    // 2. 以M为地址读取结果
    // 如果M==N,M-=1
    // if (M == N)
    RD1 = 128;
    RD0 = MM;
    RD1 -= RD0;
    if (RQ_nZero) goto L_recip_fix_1;
    // M -= 1;
    MM -= 1;

L_recip_fix_1:
    // rst = recip_table[M] >> (E);
    //RD0 = MM;
    //RD1 = RN_Recip_Table_ADDR;
    //RA0 = RD1;
    //RD0 = M[RA0 + RD0];

    RD0 = MM;
    RD1 = RN_Recip_Table_ADDR;
    RD0 += RD1;
    call ConstROM_Read_DWord;


    RD1 = E;
    call_AutoField _Rf_ShiftR_Reg;

    #undef E
    #undef MM
    #undef x
    RD1 = 1;
    RD1 ^= RD2;
    if (RQ_nZero) goto L_recip_fix_End;
    RF_Not(RD0);
    RD0 += 1;
L_recip_fix_End:

    pop(RD6);
    pop(RD5);
    pop(RD4);

    Return_AutoField(0);

}

////////////////////////////////////////////////////////
//  名称:
//      recip_fix_Q7
//  功能:
//      定点求倒数
//  参数:
//      1.RD0: 数据(q7)
//  返回值:
//      1.RD0: 结果（q7）
////////////////////////////////////////////////////////
Sub_AutoField recip_fix_Q7
{
    call_AutoField recip_fix;
    RF_ShiftR2(RD0);
    RF_ShiftR2(RD0);
    RF_ShiftR2(RD0);
    RF_ShiftR2(RD0);
    RF_ShiftR1(RD0);
    Return_AutoField(0);
}





////////////////////////////////////////////////////////
//  名称:
//      power2_fix 修改过的2指数计算 取值范围有限(x>=0) 需要注意输入输出是否在值域内
//  功能:
//      定点求Power = 2^(RD0)
//  参数:
//      1.RD0: 数据	(16q8)
//  返回值:
//      1.RD0: 结果（32q8）
////////////////////////////////////////////////////////
Sub_AutoField power2_fix
{
	int x = RD0.m_data;

//建表
    int power2[128] = { 0x0080,0x0080,
0x0082,0x0081,
0x0083,0x0082,
0x0084,0x0084,
0x0086,0x0085,
0x0087,0x0087,
0x0089,0x0088,
0x008A,0x008A,
0x008C,0x008B,
0x008D,0x008D,
0x008F,0x008E,
0x0090,0x0090,
0x0092,0x0091,
0x0094,0x0093,
0x0095,0x0094,
0x0097,0x0096,
0x0099,0x0098,
0x009A,0x0099,
0x009C,0x009B,
0x009E,0x009D,
0x009F,0x009E,
0x00A1,0x00A0,
0x00A3,0x00A2,
0x00A5,0x00A4,
0x00A6,0x00A5,
0x00A8,0x00A7,
0x00AA,0x00A9,
0x00AC,0x00AB,
0x00AE,0x00AD,
0x00B0,0x00AF,
0x00B2,0x00B1,
0x00B4,0x00B3,
0x00B6,0x00B5,
0x00B7,0x00B6,
0x00B9,0x00B8,
0x00BC,0x00BA,
0x00BE,0x00BD,
0x00C0,0x00BF,
0x00C2,0x00C1,
0x00C4,0x00C3,
0x00C6,0x00C5,
0x00C8,0x00C7,
0x00CA,0x00C9,
0x00CD,0x00CB,
0x00CF,0x00CE,
0x00D1,0x00D0,
0x00D3,0x00D2,
0x00D6,0x00D4,
0x00D8,0x00D7,
0x00DA,0x00D9,
0x00DD,0x00DB,
0x00DF,0x00DE,
0x00E2,0x00E0,
0x00E4,0x00E3,
0x00E6,0x00E5,
0x00E9,0x00E8,
0x00EC,0x00EA,
0x00EE,0x00ED,
0x00F1,0x00EF,
0x00F3,0x00F2,
0x00F6,0x00F5,
0x00F9,0x00F7,
0x00FB,0x00FA,
0x00FE,0x00FD };

//x=xh+xl，2^x=(x^xh)*(x^xl)
    x = x + 1;
    int xl = (x & 0xff) >> 1;
    int xh = ((x & 0xff00) >> 8) + 1;
    RD1 = 2 ^ xh;		//x^xh
    RD0 = power2[xl];	//x^xl查表
    long long y = RD1.m_data;
    y = RD0.m_data * y;
    RD0 = y >> 8;
    Return_AutoField(0);

}


////////////////////////////////////////////////////////
//  名称:
//      log2_fix
//  功能:
//      定点求log2
//  参数:
//      1.x: 数据 
//  返回值:
//      1.RD0: 结果（q15）
////////////////////////////////////////////////////////
Sub_AutoField log2_fix
{
	int x = RD0.m_data;

//建表
    int Log2[128] = { 0x8170, 0x8000,
	0x8448, 0x82DD,
	0x8714, 0x85AF,
	0x89D6, 0x8876,
	0x8C8D, 0x8B32,
	0x8F3A, 0x8DE5,
	0x91DD, 0x908D,
	0x9477, 0x932B,
	0x9708, 0x95C1,
	0x998F, 0x984D,
	0x9C0E, 0x9AD0,
	0x9E85, 0x9D4A,
	0xA0F3, 0x9FBD,
	0xA359, 0xA227,
	0xA5B7, 0xA489,
	0xA80D, 0xA6E3,
	0xAA5C, 0xA935,
	0xACA4, 0xAB81,
	0xAEE4, 0xADC5,
	0xB11E, 0xB002,
	0xB351, 0xB238,
	0xB57D, 0xB467,
	0xB7A2, 0xB690,
	0xB9C2, 0xB8B3,
	0xBBDB, 0xBACF,
	0xBDEE, 0xBCE5,
	0xBFFB, 0xBEF6,
	0xC203, 0xC100,
	0xC405, 0xC305,
	0xC601, 0xC504,
	0xC7F8, 0xC6FD,
	0xC9EA, 0xC8F2,
	0xCBD6, 0xCAE1,
	0xCDBD, 0xCCCA,
	0xCFA0, 0xCEAF,
	0xD17D, 0xD08F,
	0xD356, 0xD26A,
	0xD52A, 0xD441,
	0xD6FA, 0xD613,
	0xD8C5, 0xD7E0,
	0xDA8B, 0xD9A9,
	0xDC4D, 0xDB6D,
	0xDE0B, 0xDD2D,
	0xDFC5, 0xDEE9,
	0xE17B, 0xE0A1,
	0xE32D, 0xE254,
	0xE4DA, 0xE404,
	0xE684, 0xE5B0,
	0xE82A, 0xE758,
	0xE9CD, 0xE8FC,
	0xEB6C, 0xEA9D,
	0xED07, 0xEC3A,
	0xEE9E, 0xEDD3,
	0xF032, 0xEF69,
	0xF1C3, 0xF0FB,
	0xF350, 0xF28A,
	0xF4DA, 0xF415,
	0xF661, 0xF59E,
	0xF7E4, 0xF723,
	0xF965, 0xF8A5,
	0xFAE2, 0xFA24,
	0xFC5C, 0xFB9F,
	0xFDD3, 0xFD18,
	0xFF47, 0xFE8E
	};

	if (x == 0)
		return;

	//x=2^e*m
	int e = log2(x) + 1;
	int m;

	if (e > 8)
	{
		//M = (x >> (E-index)) - N
		x = x >> (e - 8);
		m = x - 128;
	}
	else if (e < 8)
	{
		//M = (x << (index - E)) - N;
		x = x << (8 - e);
		m = x - 128;
	}
	else
		m = 0;

	//查表
	x = e << 15;
	RD0 = Log2[m];	//查表
	RD0 += x;

}

