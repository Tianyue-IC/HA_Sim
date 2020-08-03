#include "main.h"


// 单操作数
void test_simple_oper()
{
	RD0 = -65536;
	RF_RotateR8(RD0);
	RF_RotateR8(RD0);
	RF_RotateR16(RD0);

	RF_Sft32SR1(RD0);
	RF_Sft32SR2(RD0);
	RF_Sft32SR4(RD0);
	RF_Sft32SR8(RD0);

	RD0 = 0x4000C000;
	RF_Sft16SR1(RD0);
	RF_Sft16SR2(RD0);
	RF_Sft16SR4(RD0);
	RF_Sft16SR8(RD0);

	RD0 = 0x8866AA33;
	RF_GetL8(RD0);
	RD0 = 0x8866AA33;
	RF_GetML8(RD0);
	RD0 = 0x8866AA33;
	RF_GetMH8(RD0);
	RD0 = 0x8866AA33;
	RF_GetH8(RD0);
	RD0 = 0x8866AA33;
	RF_GetL16(RD0);
	RD0 = 0x8866AA33;
	RF_GetH16(RD0);

	RD0 = 0x1234567f;
	RF_Reverse(RD0);
	RF_Reverse(RD0);

	RD0 = 0x1234567f;
	RF_LSB2MSB(RD0);
	RF_MSB2LSB(RD0);
	RD0 = 0x1234567f;
	RF_ExchangeL16(RD0);

	RD0 = 0x1234567f;
	RF_GetH4(RD0);
	RD0 = 0x7f123456;
	RF_GetH4(RD0);

	RF_Get0(RD0);
	RF_Get1(RD0);

	RD0 = -3;
	RF_Abs(RD0);
	RD0 = 3;
	RF_Abs(RD0);
	RD0 = -3;
	RF_Not(RD0);
	RD0 = 3;
	RF_Not(RD0);
	RD0 = -3;
	RF_Neg(RD0);
	RD0 = 3;
	RF_Neg(RD0);

	RD0 = 0x2F010207;
	RF_Parity(RD0);

	RD0 = 0xf0a;
	RF_Exp(RD0);

	RD0 = 0xff;
	RF_Log(RD0);

	RD0 = 0x12345678;
	RD0_SetByteL8;
	RD0 = 0x12345678;
	RD0_SetByteML8;
	RD0 = 0x12345678;
	RD0_SetByteH24;
	RD0 = 0x12345678;
	RD0_ClrByteL8;
	RD0 = 0x12345678;
	RD0_ClrByteML8;
	RD0 = 0x12345678;
	RD0_ClrByteH24;

	RD0 = 0x12345678;
	RD0_SignExtL8;
	RD0 = 0x12345678;
	RD0_SignExtL16;
	RD0 = 0x12345678;
	RD0_SignExtL24;

	RD0 = 0x123456F8;
	RD0_SignExtL8;
	RD0 = 0x1234F678;
	RD0_SignExtL16;
	RD0 = 0x12F45678;
	RD0_SignExtL24;

}

// 条件判断
void test_conditional()
{
	RD0 = 0xfffffffe;
	if (RD0_Bit0==0)
	{
		RD0 = 1;
	}
	if (RD0_Bit0==1)
	{
		RD0 = 0;
	}

	RD0 = 0xfffffffe;
	if (RD0_Bit31 == 0)
	{
		RD0 = 1;
	}
	if (RD0_Bit31 == 1)
	{
		RD0 = 0;
	}

	RD0 = 0xfffffffe;
	if (RD0 >= 0)
	{
		RD0 = -1;
	}


	RD0 = 0xfffffffe;
	if (RD0 < 0)
	{
		RD0 = 1;
	}

	RD0 = 0xfffffff0;
	if (RD0_L4 == 0)
	{
		RD1 = 4;
	}
	if (RD0_L8 == 0)
	{
		RD1 = 8;
	}
	if (RD0_L16 == 0)
	{
		RD1 = 16;
	}


	RD0 = 1;
	RD1 = 2;
	RD0 -= RD1;
	if (RQ_Bit0 == 0)
	{
		RD0 = 3;
	}
	if (RQ_Bit31 == 1)
	{
		RD0 = 3;
	}

	RD0 = 1;
	RD1 = 1;
	RD0 -= RD1;
	if (RQ_nZero)
	{
		RD1 = 4;
	}
	if (RQ_L4 == 0)
	{
		RD1 = 4;
	}
	if (RQ_L8 == 0)
	{
		RD1 = 8;
	}
	if (RQ_L16 == 0)
	{
		RD1 = 16;
	}

	RD0 = 1;
	RD1 = -2;
	RD0 -= RD1;
	if (RQ_Borrow)
	{
		if (RQ_nBorrow)
		{
			int here;
		}
	}

	RD0 = 1;
	RD1 = 2;
	RD0 -= RD1;
	if (SRQ>=0)
	{
		if (SRQ<0)
		{
			int here;
		}
	}

	RD0 = INT_MIN;
	RD1 = 2;
	RD0 -= RD1;
	if (RQ_OverFlow)
	{
		if (RQ_OverFlow_H)
		{
			int here;
			here = 1;
		}
	}


}





// 寄存器、内存操作demo
void test_mem_oper()
{

	//// 申请3*MMU_BASE,地址给到RA0
	//RA0 = RSP_MINUS(3 * MMU_BASE);
	//RA0 = RSP;

	//// 申请3*MMU_BASE,地址给到RA1
	//RSP_MINUS(3 * MMU_BASE);
	//RA1 = RSP;

	//// set RA0 memory
	//RD0 = 0x11111111;
	//SET_M(RA0 + 0 * MMU_BASE, RD0);
	//RD0 = 0x22222222;
	//SET_M(RA0 + 1 * MMU_BASE, RD0);
	//RD0 = 0x33333333;
	//SET_M(RA0 + 2 * MMU_BASE, RD0);

	//// COPY RA0 TO RA1
	//memcpy(3 * MMU_BASE, RA0, RA1);

	//// get RA1 memory
	//RD1 = GET_M(RA1 + 0 * MMU_BASE);
	//RD2 = GET_M(RA1 + 1 * MMU_BASE);
	//RD3 = GET_M(RA1 + 2 * MMU_BASE);

	//push(RD1);
	//pop(RD0);

	//// 释放RSP memory
	//RSP_ADD(6 * MMU_BASE);


	// 访问GRAM
	RD0 = RN_GRAM0;
	RA0 = RD0;

	RD0 = RN_GRAM1;
	RA1 = RD0;

	//此处隐藏数据录入代码
	RD0 = 0xD0000000;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xD0000040;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xD0000060;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xD0000080;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xD00000A0;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xD00000C0;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xD00000E0;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xD0000370;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xD0000390;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x00000000;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x00000000;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x00000000;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x00000000;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x00000000;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x00000000;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x00000000;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x78797FFF;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x6BCA71C7;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x61866666;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x590B5D17;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x51EC5555;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x4BDA4EC5;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x469F4925;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x42114444;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x41F84000;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x45BE43E1;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x4952478D;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x4CBB4B0B;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x50004E62;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x53235195;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x562954A9;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x591557A2;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x07C30000;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x15FF0F14;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x22CF1C90;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x2E7428C3;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x392033E6;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x42FA3E25;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x4C1F47A1;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x54A85076;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x03240000;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x09640646;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x0F8D0C7C;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x15901294;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x1B5D187E;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x20E71E2B;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x2620238E;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x2AFB289A;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x3FEC4000;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x3F4F3FB1;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x3E153EC5;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x3C423D3F;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x39DB3B21;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x36E53871;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x33683537;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x2F6C3179;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x64850648;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x64877FF6;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x64887FFE;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x00007FFF;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x9fad2000;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x20009fad;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x5dc10000;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x1de3dba5;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x04218000;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xbfd22000;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x00002000;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x3fab0000;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x80009fcf;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x04218000;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xbee82000;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x3ee80000;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x7a5ea000;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x7152d821;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x05449b6f;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xbff52000;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x00001000;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x3f920000;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x80008fd1;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x05448000;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xfcf02000;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xfcf05cf2;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x7bb02000;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x7624da53;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x05449d32;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xfe3b2000;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xfe3b5e3d;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x7dac2000;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x7c30dd50;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x05449f40;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xfb6c2000;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xfb6c5b74;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x7a452000;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x74d8d8fa;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x05449d39;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xfcbe2000;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xfcbe5cc6;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x7c362000;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x7ac8dbe5;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x05449f45;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xf84a2000;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xf84a5867;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x77d42000;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x7299d6b0;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x05449d45;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xfa0d2000;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xfa0d5a2a;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x79ab2000;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x786ad97b;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x05449f58;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xf4262000;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xf426547f;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x73bf2000;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x6e92d2e4;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x05449d34;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xf5ef2000;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xf5ef5649;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x75902000;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x7452d5a0;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x05449f54;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xeedf2000;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xeedf4fb7;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x6e842000;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x697fce3f;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x05449d29;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xf0a22000;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xf0a2517d;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x70472000;
	SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;

	RA0 = RN_GRAM0;
	RA1 = RN_GRAM1;
	RD0 = 128;
	call_AutoField Real_To_Complex2;
	RA0 = RN_GRAM0;
	RA1 = RN_GRAM0;
	RD0 = 128;
	call_AutoField Real_To_Complex2;

	//RD1 = 128;
	//call_AutoField FindMaxIndex;

	RD0 = 0X401D520D;
	RD1 = 0X435EADBF;
	call_AutoField Power10_Float;


	// 测试全局变量
	RD0 = 0x11223344;
	g_0 = RD0;
	RD1 = g_0;

	RD0 = 0x55667788;
	g_1 = RD0;
	RD1 = g_1;

	// 测试M[RA]
	RD0 = 0x11112222;
	RSP -= 4 * MMU_BASE;
	RA0 = RSP;
	M[RA0] = RD0;

	M[RA0] += 1;
	M[RA0] -= 1;

	RA0 += 1 * MMU_BASE;
	RD0 += 1;
	M[RA0] = RD0;
	RA0 += 1 * MMU_BASE;
	RD0 += 1;
	M[RA0] = RD0;
	RA0 += 1 * MMU_BASE;
	RD0 += 1;
	M[RA0] = RD0;
	RA0 += 1 * MMU_BASE;
	RD0 += 1;

	RD1 = M[RSP];
	RSP += 4 * MMU_BASE;

	// 测试M[RA++]/M[RA--]
	RD0 = 0x44556600;
	RSP -= 4 * MMU_BASE;
	RA0 = RSP;
	M[RA0++] = 1;
	RD0 += 1;
	M[RA0++] = RD0;
	RD0 += 1;
	M[RA0++] = RD0;
	RD0 += 1;
	M[RA0++] = RD0;
	RD0 += 1;
	M[--RA0] = RD0;
	RD0 += 1;
	M[--RA0] = RD0;
	RD0 += 1;
	M[--RA0] = RD0;
	RD0 += 1;
	M[--RA0] = RD0;
	RD0 += 1;



	RSP += 4 * MMU_BASE;


	call_AutoField En_GRAM_To_CPU;
	call_AutoField Dis_GRAM_To_CPU;
	call_AutoField En_AllRAM_To_CPU;
	call_AutoField En_AllGRAM_To_CPU;
	call_AutoField En_AllXRAM_To_CPU;


}

void test_float_model()
{
	// 定转浮
	RD0 = 256;
	RD1 = 0;
	call_AutoField fix2float;
	// 浮转定
	RD1 = 0;
	call_AutoField float2fix;

	// 0*0
	RD0 = 0;
	RD1 = 0;
	call_AutoField fix2float;
	RD1 = RD0;
	call_AutoField Mul_Float;
	RD1 = 0;
	call_AutoField float2fix;

	// 测试加法
	float x = 11782056;
	float* p = &x;
	byte* pb = (byte*)p;
	*(pb + 0) = 0xA8;
	*(pb + 1) = 0xC7;
	*(pb + 2) = 0x33;
	*(pb + 3) = 0x4F;

	RD0 = 0x4B33C7A8;
	RD1 = 0;
	call_AutoField float2fix;
	pop(RD0);
	RD1 = 0;
	call_AutoField float2fix;



}