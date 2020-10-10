#include "main.h"
#include "inc_lib.h"


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
	RD0 = 0xfcf05cf2;
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

	RD0 = RN_GRAM1;
	RA0 = RD0;
	RD0 = 0x0000296e,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x0000296e,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x00003fda,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xffffc569,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x00001302,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xffffd692,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xffffa070,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x00000000,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xffffa070,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xffffd692,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x00001302,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xffffc568,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x00003fda,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x0000296e,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x00000cb3,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x00007530,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x00000cb3,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x0000296e,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x00003fda,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xffffc569,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x00001302,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xffffd692,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xffffa070,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x00000000,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xffffa070,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xffffd692,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x00001302,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xffffc568,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x00003fda,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x0000296e,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x00000cb3,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x00007530,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x00000cb3,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x0000296e,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x00003fda,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xffffc569,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x00001302,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xffffd692,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xffffa070,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x00000000,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xffffa070,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xffffd692,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x00001302,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xffffc568,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x00003fda,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x0000296e,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x00000cb3,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x00007530,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x00000cb3,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x0000296e,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x00003fda,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xffffc568,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x00001302,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xffffd692,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xffffa070,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x00000000,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xffffa070,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xffffd692,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x00001302,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xffffc568,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x00003fda,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x0000296e,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x00000cb3,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x00007530,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x00000cb3,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x0000296e,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x00003fda,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xffffc568,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x00001302,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xffffd692,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xffffa070,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x00000000,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xffffa070,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xffffd692,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x00001302,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xffffc568,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x00003fda,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x0000296e,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x00000cb3,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x00007530,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x00000cb3,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x0000296e,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x00003fda,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xffffc568,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x00001302,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xffffd692,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xffffa070,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x00000000,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xffffa070,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xffffd692,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x00001302,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xffffc568,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x00003fda,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x0000296e,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x00000cb3,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x00007530,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x00000cb3,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x0000296e,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x00003fda,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xffffc568,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x00001302,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xffffd692,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xffffa070,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x00000000,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xffffa070,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xffffd692,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x00001302,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xffffc568,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x00003fda,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x0000296e,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x00000cb3,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x00007530,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x00000cb3,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x0000296e,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x00003fda,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xffffc568,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x00001302,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xffffd692,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xffffa070,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x00000000,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xffffa070,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xffffd692,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x00001302,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0xffffc568,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x00003fda,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x0000296e,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x00000cb3,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	RD0 = 0x00007530,
		SET_M(RA0, RD0);
	RA0 += 1 * MMU_BASE;
	
	
	RA0 = RN_GRAM0;
	RD0 = 128;
	RD1 = 0x43000090;
	call_AutoField Abs_Mean_Float;




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


	RD0 = RN_GRAM1;
	RA1 = RD0;

	RD0 = 0x2FC7D19B;
	M[RA1++] = RD0;
	RD0 = 0x0E52CABD;
	M[RA1++] = RD0;
	RD0 = 0x233CD954;
	M[RA1++] = RD0;
	RD0 = 0x13754194;
	M[RA1++] = RD0;
	RD0 = 0x2015C03A;
	M[RA1++] = RD0;
	RD0 = 0x0BA573BA;
	M[RA1++] = RD0;
	RD0 = 0x3043C284;
	M[RA1++] = RD0;
	RD0 = 0x20BBB5F5;
	M[RA1++] = RD0;
	RD0 = 0x2C7A0CAA;
	M[RA1++] = RD0;
	RD0 = 0x2FB62DA1;
	M[RA1++] = RD0;
	RD0 = 0x0831C5A4;
	M[RA1++] = RD0;
	RD0 = 0x152CA2D1;
	M[RA1++] = RD0;
	RD0 = 0x26FB0E74;
	M[RA1++] = RD0;
	RD0 = 0x082E0C94;
	M[RA1++] = RD0;
	RD0 = 0x27C1879A;
	M[RA1++] = RD0;
	RD0 = 0x1CE8C2C5;
	M[RA1++] = RD0;
	RD0 = 0x0BF8E480;
	M[RA1++] = RD0;
	RD0 = 0x2814CCB1;
	M[RA1++] = RD0;
	RD0 = 0x184BB3BD;
	M[RA1++] = RD0;
	RD0 = 0x2A1DECD6;
	M[RA1++] = RD0;
	RD0 = 0x2574FACF;
	M[RA1++] = RD0;
	RD0 = 0x2BD6379C;
	M[RA1++] = RD0;
	RD0 = 0x04594DBC;
	M[RA1++] = RD0;
	RD0 = 0x30218B5E;
	M[RA1++] = RD0;
	RD0 = 0x17DBF5F6;
	M[RA1++] = RD0;
	RD0 = 0x14D43205;
	M[RA1++] = RD0;
	RD0 = 0x0BA32D4D;
	M[RA1++] = RD0;
	RD0 = 0x173FFD2C;
	M[RA1++] = RD0;
	RD0 = 0x2538C0F0;
	M[RA1++] = RD0;
	RD0 = 0x0DC16963;
	M[RA1++] = RD0;
	RD0 = 0x138634E1;
	M[RA1++] = RD0;
	RD0 = 0x1597BC8B;
	M[RA1++] = RD0;
	RD0 = 0x2DC545B5;
	M[RA1++] = RD0;
	RD0 = 0x0A52B200;
	M[RA1++] = RD0;
	RD0 = 0x022360A4;
	M[RA1++] = RD0;
	RD0 = 0x26DD9424;
	M[RA1++] = RD0;
	RD0 = 0x1D9B9C34;
	M[RA1++] = RD0;
	RD0 = 0x0284E73E;
	M[RA1++] = RD0;
	RD0 = 0x119A4D5A;
	M[RA1++] = RD0;
	RD0 = 0x077BE553;
	M[RA1++] = RD0;
	RD0 = 0x0AADCA7A;
	M[RA1++] = RD0;
	RD0 = 0x2C10DB44;
	M[RA1++] = RD0;
	RD0 = 0x20C13042;
	M[RA1++] = RD0;
	RD0 = 0x0D6A0CEE;
	M[RA1++] = RD0;
	RD0 = 0x0593C62E;
	M[RA1++] = RD0;
	RD0 = 0x28F95510;
	M[RA1++] = RD0;
	RD0 = 0x0F8E83B6;
	M[RA1++] = RD0;
	RD0 = 0x16706B54;
	M[RA1++] = RD0;
	RD0 = 0x28AE88C1;
	M[RA1++] = RD0;
	RD0 = 0x2A1E5C2B;
	M[RA1++] = RD0;
	RD0 = 0x2C1D95A9;
	M[RA1++] = RD0;
	RD0 = 0x0C55774D;
	M[RA1++] = RD0;
	RD0 = 0x23653460;
	M[RA1++] = RD0;
	RD0 = 0x07454E81;
	M[RA1++] = RD0;
	RD0 = 0x1A61AE2D;
	M[RA1++] = RD0;
	RD0 = 0x0A5B34FF;
	M[RA1++] = RD0;
	RD0 = 0x0E31FA7B;
	M[RA1++] = RD0;
	RD0 = 0x020976BA;
	M[RA1++] = RD0;
	RD0 = 0x19AFE4F6;
	M[RA1++] = RD0;
	RD0 = 0x1B9B3097;
	M[RA1++] = RD0;
	RD0 = 0x0A1312F5;
	M[RA1++] = RD0;
	RD0 = 0x25955BCE;
	M[RA1++] = RD0;
	RD0 = 0x0FCFE43A;
	M[RA1++] = RD0;
	RD0 = 0x08E6B5A9;
	M[RA1++] = RD0;
	RD0 = 0x0ABE9080;
	M[RA1++] = RD0;
	RD0 = 0x237E3684;
	M[RA1++] = RD0;
	RD0 = 0x068697B9;
	M[RA1++] = RD0;
	RD0 = 0x0F1667E1;
	M[RA1++] = RD0;
	RD0 = 0x29BBCE6D;
	M[RA1++] = RD0;
	RD0 = 0x063EAEA1;
	M[RA1++] = RD0;
	RD0 = 0x0D09CD33;
	M[RA1++] = RD0;
	RD0 = 0x2028EEC0;
	M[RA1++] = RD0;
	RD0 = 0x20B6912B;
	M[RA1++] = RD0;
	RD0 = 0x2C739198;
	M[RA1++] = RD0;
	RD0 = 0x06ACF3F2;
	M[RA1++] = RD0;
	RD0 = 0x173A3ED7;
	M[RA1++] = RD0;
	RD0 = 0x20B86889;
	M[RA1++] = RD0;
	RD0 = 0x265BB788;
	M[RA1++] = RD0;
	RD0 = 0x2D82B2C5;
	M[RA1++] = RD0;
	RD0 = 0x188BCEEA;
	M[RA1++] = RD0;
	RD0 = 0x09ED1B29;
	M[RA1++] = RD0;
	RD0 = 0x15D24802;
	M[RA1++] = RD0;
	RD0 = 0x03907C2B;
	M[RA1++] = RD0;
	RD0 = 0x1EED0409;
	M[RA1++] = RD0;
	RD0 = 0x176BDAEE;
	M[RA1++] = RD0;
	RD0 = 0x129A2FD5;
	M[RA1++] = RD0;
	RD0 = 0x09017FCA;
	M[RA1++] = RD0;
	RD0 = 0x274155E2;
	M[RA1++] = RD0;
	RD0 = 0x1971EEAD;
	M[RA1++] = RD0;
	RD0 = 0x2F82F776;
	M[RA1++] = RD0;
	RD0 = 0x2095808D;
	M[RA1++] = RD0;
	RD0 = 0x2F7247FB;
	M[RA1++] = RD0;
	RD0 = 0x2B38A503;
	M[RA1++] = RD0;
	RD0 = 0x132C85E4;
	M[RA1++] = RD0;
	RD0 = 0x10591F61;
	M[RA1++] = RD0;
	RD0 = 0x04EDBE1C;
	M[RA1++] = RD0;
	RD0 = 0x32633A5B;
	M[RA1++] = RD0;
	RD0 = 0x22C2E41C;
	M[RA1++] = RD0;
	RD0 = 0x325C7FD6;
	M[RA1++] = RD0;
	RD0 = 0x24DDBD50;
	M[RA1++] = RD0;
	RD0 = 0x0D71AD82;
	M[RA1++] = RD0;
	RD0 = 0x0F1909F6;
	M[RA1++] = RD0;
	RD0 = 0x1482A9EF;
	M[RA1++] = RD0;
	RD0 = 0x18E33FE1;
	M[RA1++] = RD0;
	RD0 = 0x2C9A86D0;
	M[RA1++] = RD0;
	RD0 = 0x2B102C12;
	M[RA1++] = RD0;
	RD0 = 0x2CE58C7B;
	M[RA1++] = RD0;
	RD0 = 0x095CA091;
	M[RA1++] = RD0;
	RD0 = 0x241D2C5F;
	M[RA1++] = RD0;
	RD0 = 0x2BF1424C;
	M[RA1++] = RD0;
	RD0 = 0x0A516D16;
	M[RA1++] = RD0;
	RD0 = 0x25417652;
	M[RA1++] = RD0;
	RD0 = 0x036E4FDF;
	M[RA1++] = RD0;
	RD0 = 0x2242BDF5;
	M[RA1++] = RD0;
	RD0 = 0x309746EF;
	M[RA1++] = RD0;
	RD0 = 0x29F63889;
	M[RA1++] = RD0;
	RD0 = 0x1386EAF2;
	M[RA1++] = RD0;
	RD0 = 0x2FAE456F;
	M[RA1++] = RD0;
	RD0 = 0x0AB7D347;
	M[RA1++] = RD0;
	RD0 = 0x0CCC8967;
	M[RA1++] = RD0;
	RD0 = 0x0A9184E4;
	M[RA1++] = RD0;
	RD0 = 0x16648248;
	M[RA1++] = RD0;
	RD0 = 0x1418C4B4;
	M[RA1++] = RD0;
	RD0 = 0x2F6EAFD1;
	M[RA1++] = RD0;
	RD0 = 0x27B936C5;
	M[RA1++] = RD0;
	RD0 = 0x19F8436F;
	M[RA1++] = RD0;
	RD0 = 0x25C7D1DA;
	M[RA1++] = RD0;
	RD0 = 0x01D4E2E3;
	M[RA1++] = RD0;

	RD0 = RN_GRAM0;
	RA1 = RD0;

	RD0 = 0x82F39AB3;
	M[RA1++] = RD0;
	RD0 = 0xAEF61E5B;
	M[RA1++] = RD0;
	RD0 = 0xAE48C5D7;
	M[RA1++] = RD0;
	RD0 = 0x9A52E12A;
	M[RA1++] = RD0;
	RD0 = 0x9225A1FD;
	M[RA1++] = RD0;
	RD0 = 0xAF10C61A;
	M[RA1++] = RD0;
	RD0 = 0xAC91C1E6;
	M[RA1++] = RD0;
	RD0 = 0x8D66E205;
	M[RA1++] = RD0;
	RD0 = 0xA2F646BC;
	M[RA1++] = RD0;
	RD0 = 0x80B5D236;
	M[RA1++] = RD0;
	RD0 = 0x899EA7A5;
	M[RA1++] = RD0;
	RD0 = 0xAC3F39F0;
	M[RA1++] = RD0;
	RD0 = 0x9469DB84;
	M[RA1++] = RD0;
	RD0 = 0x9E1A3243;
	M[RA1++] = RD0;
	RD0 = 0xA87BEA03;
	M[RA1++] = RD0;
	RD0 = 0x9D8466E7;
	M[RA1++] = RD0;
	RD0 = 0x9EBCC50B;
	M[RA1++] = RD0;
	RD0 = 0xA5F9BCEA;
	M[RA1++] = RD0;
	RD0 = 0x97D619FB;
	M[RA1++] = RD0;
	RD0 = 0x85580627;
	M[RA1++] = RD0;
	RD0 = 0x89E369AB;
	M[RA1++] = RD0;
	RD0 = 0x9DFE1777;
	M[RA1++] = RD0;
	RD0 = 0x9AD8996A;
	M[RA1++] = RD0;
	RD0 = 0x9FF6CC8F;
	M[RA1++] = RD0;
	RD0 = 0x8B5BF02D;
	M[RA1++] = RD0;
	RD0 = 0xA381A11E;
	M[RA1++] = RD0;
	RD0 = 0xAEF3604B;
	M[RA1++] = RD0;
	RD0 = 0x990A725A;
	M[RA1++] = RD0;
	RD0 = 0xA1DECE34;
	M[RA1++] = RD0;
	RD0 = 0xAE62B896;
	M[RA1++] = RD0;
	RD0 = 0x906276DC;
	M[RA1++] = RD0;
	RD0 = 0x8466C8D0;
	M[RA1++] = RD0;
	RD0 = 0x80E1DC36;
	M[RA1++] = RD0;
	RD0 = 0xA99213FB;
	M[RA1++] = RD0;
	RD0 = 0x85B9F7E8;
	M[RA1++] = RD0;
	RD0 = 0xAC953463;
	M[RA1++] = RD0;
	RD0 = 0x80461E8F;
	M[RA1++] = RD0;
	RD0 = 0x9321E117;
	M[RA1++] = RD0;
	RD0 = 0x939D0505;
	M[RA1++] = RD0;
	RD0 = 0xA6919DB2;
	M[RA1++] = RD0;
	RD0 = 0xA74342C7;
	M[RA1++] = RD0;
	RD0 = 0xA3C2E0B2;
	M[RA1++] = RD0;
	RD0 = 0x9BC66CE7;
	M[RA1++] = RD0;
	RD0 = 0xA261BC56;
	M[RA1++] = RD0;
	RD0 = 0xA8309115;
	M[RA1++] = RD0;
	RD0 = 0xA81FE563;
	M[RA1++] = RD0;
	RD0 = 0x8B4DA0C7;
	M[RA1++] = RD0;
	RD0 = 0x89CCF399;
	M[RA1++] = RD0;
	RD0 = 0xAF75086E;
	M[RA1++] = RD0;
	RD0 = 0x8DAA032C;
	M[RA1++] = RD0;
	RD0 = 0x8E6ED2AD;
	M[RA1++] = RD0;
	RD0 = 0x95CE7085;
	M[RA1++] = RD0;
	RD0 = 0xA505A53B;
	M[RA1++] = RD0;
	RD0 = 0xA51D7C33;
	M[RA1++] = RD0;
	RD0 = 0x84321C71;
	M[RA1++] = RD0;
	RD0 = 0x9D273D4B;
	M[RA1++] = RD0;
	RD0 = 0x9F5AB0C3;
	M[RA1++] = RD0;
	RD0 = 0xA21659F9;
	M[RA1++] = RD0;
	RD0 = 0xB2084A3B;
	M[RA1++] = RD0;
	RD0 = 0x9D7D5D1A;
	M[RA1++] = RD0;
	RD0 = 0xB1347D87;
	M[RA1++] = RD0;
	RD0 = 0x83B2B506;
	M[RA1++] = RD0;
	RD0 = 0x8F558DF3;
	M[RA1++] = RD0;
	RD0 = 0x922FC344;
	M[RA1++] = RD0;
	RD0 = 0xA521D6F6;
	M[RA1++] = RD0;
	RD0 = 0xA787847E;
	M[RA1++] = RD0;
	RD0 = 0xAA154FC0;
	M[RA1++] = RD0;
	RD0 = 0xA27E019D;
	M[RA1++] = RD0;
	RD0 = 0xA97EA420;
	M[RA1++] = RD0;
	RD0 = 0x8BCAF633;
	M[RA1++] = RD0;
	RD0 = 0x8692059D;
	M[RA1++] = RD0;
	RD0 = 0x87E851F6;
	M[RA1++] = RD0;
	RD0 = 0xA4B1ADC9;
	M[RA1++] = RD0;
	RD0 = 0x8AA6F602;
	M[RA1++] = RD0;
	RD0 = 0xAACA87C0;
	M[RA1++] = RD0;
	RD0 = 0x8A3A3D4C;
	M[RA1++] = RD0;
	RD0 = 0xA10C0FB5;
	M[RA1++] = RD0;
	RD0 = 0xAD1F4211;
	M[RA1++] = RD0;
	RD0 = 0xAADF74D2;
	M[RA1++] = RD0;
	RD0 = 0x8BF21814;
	M[RA1++] = RD0;
	RD0 = 0x855607E7;
	M[RA1++] = RD0;
	RD0 = 0x90C9F49C;
	M[RA1++] = RD0;
	RD0 = 0x946A461D;
	M[RA1++] = RD0;
	RD0 = 0x8B759F51;
	M[RA1++] = RD0;
	RD0 = 0x857E2ED1;
	M[RA1++] = RD0;
	RD0 = 0xAC42F3BF;
	M[RA1++] = RD0;
	RD0 = 0x8284F2E5;
	M[RA1++] = RD0;
	RD0 = 0xA7A09AE2;
	M[RA1++] = RD0;
	RD0 = 0x8457EAD5;
	M[RA1++] = RD0;
	RD0 = 0x8293637A;
	M[RA1++] = RD0;
	RD0 = 0x910BCAE4;
	M[RA1++] = RD0;
	RD0 = 0x95F0C489;
	M[RA1++] = RD0;
	RD0 = 0xAA04959B;
	M[RA1++] = RD0;
	RD0 = 0x9A500BF0;
	M[RA1++] = RD0;
	RD0 = 0xB1273B2B;
	M[RA1++] = RD0;
	RD0 = 0x8F153863;
	M[RA1++] = RD0;
	RD0 = 0xA27CF144;
	M[RA1++] = RD0;
	RD0 = 0x9E2845E6;
	M[RA1++] = RD0;
	RD0 = 0xA6BB53F6;
	M[RA1++] = RD0;
	RD0 = 0xA7D18769;
	M[RA1++] = RD0;
	RD0 = 0x8162807E;
	M[RA1++] = RD0;
	RD0 = 0x83B811B5;
	M[RA1++] = RD0;
	RD0 = 0xA2586547;
	M[RA1++] = RD0;
	RD0 = 0xAE771388;
	M[RA1++] = RD0;
	RD0 = 0x8FB5FB30;
	M[RA1++] = RD0;
	RD0 = 0x80DAEFEA;
	M[RA1++] = RD0;
	RD0 = 0x840485CD;
	M[RA1++] = RD0;
	RD0 = 0xADE2B494;
	M[RA1++] = RD0;
	RD0 = 0xAD72E8B6;
	M[RA1++] = RD0;
	RD0 = 0x9C19F2BF;
	M[RA1++] = RD0;
	RD0 = 0xAA1FB689;
	M[RA1++] = RD0;
	RD0 = 0x9C7F58BC;
	M[RA1++] = RD0;
	RD0 = 0xA3155E8F;
	M[RA1++] = RD0;
	RD0 = 0xA9E9CF69;
	M[RA1++] = RD0;
	RD0 = 0x8B0994AD;
	M[RA1++] = RD0;
	RD0 = 0xA7DF720F;
	M[RA1++] = RD0;
	RD0 = 0x9BAE9A2C;
	M[RA1++] = RD0;
	RD0 = 0xA0F120F2;
	M[RA1++] = RD0;
	RD0 = 0x8C04113D;
	M[RA1++] = RD0;
	RD0 = 0x89860341;
	M[RA1++] = RD0;
	RD0 = 0xA99859DB;
	M[RA1++] = RD0;
	RD0 = 0x8074727F;
	M[RA1++] = RD0;
	RD0 = 0xB09EBB9F;
	M[RA1++] = RD0;
	RD0 = 0xA3AE5779;
	M[RA1++] = RD0;
	RD0 = 0xA1E897EC;
	M[RA1++] = RD0;
	RD0 = 0x9E533589;
	M[RA1++] = RD0;
	RD0 = 0x950568EC;
	M[RA1++] = RD0;
	RD0 = 0xA106AC0A;
	M[RA1++] = RD0;

	RD0 = RN_GRAM0;
	RA0 = RD0;
	RA1 = RN_GRAM1;
	RD0 = 128;

	call_AutoField CompareMax_Float_Dual;

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

//本函数用作将txt文件转为bin文件，使用前需将原txt文件转为十进制形式
void txt2bin()
{
	CData_io data_io = CData_io("./user/data/steam_out.bin", "./user/data/data_out.bin");     // 数据IO类初始化
	CData_io data_io2 = CData_io("./user/data/steamB_out.bin", "./user/data/data_out.bin");     // 数据IO类初始化

	FILE* fp;
	char buf[100];
	FILE* fp0;
	char buf0[100];
	FILE* fp1;
	char buf1[100];
	fp = fopen("Debug_Out.txt", "r");//打开文件，本文件无意义，使用与fp0同样文件，仅作停止符使用
	fp0 = fopen("Debug_Out0.txt", "r");//打开文件，高位数据
	fp1 = fopen("Debug_Out1.txt", "w");

	while (fgets(buf, sizeof(buf), fp))
	{

		RA0 = RN_GRAM_OUT;

		for (int i = 0; i < 16; i++)
		{
			fgets(buf0, sizeof(buf0), fp0);
			long a = atoi(buf0);
			long b = a;
			RD0 = ((b << 16) & 0xffff0000) + ((a >> 16) & 0xffff);
			//a = a & 0xffff;
			//fprintf(fp1, "%d\r", a);
			//a = (b >> 16) & 0xffff;
			fprintf(fp1, "%d\r", RD0.m_data);
			RD0 = a;
			M[RA0++] = RD0;
		}
		for (int i = 0; i < 16; i++)
		{
			fgets(buf, sizeof(buf), fp);
		}



		// RN_GRAM_OUT输出一帧数据到DAC
		RD0 = RN_GRAM_OUT;
		RA0 = RD0;
		data_io.outData_1Frame(RA0, 32);


	}


	fclose(fp);//关闭文件。
	fclose(fp0);//关闭文件。
	fclose(fp1);//关闭文件。


	return;
}
