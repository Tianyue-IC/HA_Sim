#include "float_model.h"
#include "memory.h"
#include <math.h>

extern void float2double(unsigned int x, double& y);
extern void double2float(double x);

////////////////////////////////////////////////////////
//  名称:
//      fix2float
//  功能:
//      定点转浮点数(单数)
//  参数:
//      1.RD0:源数据(out)
//		2.RD1:源数据的Q值
//  备注:
//      本系统浮点数为IEEE754格式的浮点数中的规约形式的32位浮点数，
//		即非规约形式的浮点数和极值不在我系统表示范围内。（详见百度百科）
////////////////////////////////////////////////////////
Sub_AutoField fix2float
{
	int s = 0;
	int x = RD0.m_data;
	if (x < 0)
	{
		s = 1;
		x = -x;
	}
	else if (x == 0)
	{
		RD0 = 0;
		goto fix2float_end;
	}
	int e = log2(x);
	int m;
	if (e > 23)
	{
		m = x >> (e - 23);
	}
	else
	{
		m = x << (23 - e);
	}
	m &= 0x7fffff;
	e = log2(x) + 127 - RD1.m_data;
	RD0 = (s << 31) + (e << 23) + m;
	fix2float_end:
	Return_AutoField(0);
}

////////////////////////////////////////////////////////
//  名称:
//      fix2float_ser
//  功能:
//      定点转浮点数(序列)
//  参数:
//      1.RA0:源数据地址
//		2.RA1:输出序列地址
//		3.RD0:数据长度
//		4.RD1:源数据的Q值
//  备注:
//      本系统浮点数为IEEE754格式的浮点数中的规约形式的32位浮点数，
//		即非规约形式的浮点数和极值不在我系统表示范围内。（详见百度百科）
////////////////////////////////////////////////////////
Sub_AutoField fix2float_ser
{
	int len = RD0.m_data;

	for (int i = 0; i < len; i++)
	{
		RD0 = GET_M(RA0 + i * MMU_BASE);
		call_AutoField fix2float;
		SET_M(RA1 + i * MMU_BASE, RD0);
	}


	Return_AutoField(0);
}

////////////////////////////////////////////////////////
//  名称:
//      float2fix
//  功能:
//      浮点转定点数(单数)
//  参数:
//      1.RD0:源数据(out)
//		2.RD1:出口数据的Q值
//  备注:
//      本系统浮点数为IEEE754格式的浮点数中的规约形式的32位浮点数，
//		即非规约形式的浮点数和极值不在我系统表示范围内。（详见百度百科）
////////////////////////////////////////////////////////
Sub_AutoField float2fix
{
	unsigned int x = RD0.m_data;
	int s = x >> 31;
	int e = (x & 0x7f800000) >> 23;
	int m = (x & 0x7fffff) + 0x800000;

	if (s == 1)
		m = -m;

	e = e - 127 + RD1.m_data;
	if (e > 31)
	{
		if (s == 1)
			RD0 = 0x80000001;
		else
			RD0 = 0x7fffffff;
	}
	else if (e == 31)
	{
		if (s == 1)
			RD0 = 0x80000001;
		else
			RD0 = m << (e - 23);
	}
	else if (e > 23)
	{
		m = m << (e - 23);
		RD0 = m;
	}
	else if (e < 0)
	{
		m = 0;
		RD0 = m;
	}
	else
	{
		m = m >> (23 - e);
		RD0 = m;
	}
	Return_AutoField(0);
}

////////////////////////////////////////////////////////
//  名称:
//      float2fix_ser
//  功能:
//      浮点转定点数(序列)
//  参数:
//      1.RA0:源数据地址
//		2.RA1:输出序列地址
//		3.RD0:数据长度
//		4.RD1:源数据的Q值
//  备注:
//      本系统浮点数为IEEE754格式的浮点数中的规约形式的32位浮点数，
//		即非规约形式的浮点数和极值不在我系统表示范围内。（详见百度百科）
////////////////////////////////////////////////////////
Sub_AutoField float2fix_ser
{
	int len = RD0.m_data;

	for (int i = 0; i < len; i++)
	{
		RD0 = GET_M(RA0 + i * MMU_BASE);
		call_AutoField float2fix;
		SET_M(RA1 + i * MMU_BASE, RD0);
	}


	Return_AutoField(0);
}

////////////////////////////////////////////////////////
//  名称:
//      float2double
//  功能:
//      浮点数转double（内部调用）
//  参数:
//      1.x:源数据
//		2.y:结果（out）
//  备注:
//      本系统浮点数为IEEE754格式的浮点数中的规约形式的32位浮点数，
//		即非规约形式的浮点数和极值不在我系统表示范围内。（详见百度百科）
////////////////////////////////////////////////////////
void float2double(unsigned int x, double& y)
{
	int s = x >> 31;
	int e = ((x & 0x7f800000) >> 23) - 127;
	double m = (x & 0x7fffff);
	m = (m / 8388608) + 1;
	y = pow(-1, s) * pow(2, e) * m;

}

////////////////////////////////////////////////////////
//  名称:
//      double2float
//  功能:
//      double转浮点（内部调用）
//  参数:
//      1.x:源数据
//		2.RD0:结果（out）
//  备注:
//      本系统浮点数为IEEE754格式的浮点数中的规约形式的32位浮点数，
//		即非规约形式的浮点数和极值不在我系统表示范围内。（详见百度百科）
////////////////////////////////////////////////////////
void double2float(double x)
{
	int s = 0;
	if (x < 0)
	{
		s = 1;
		x = -x;
	}

	int e = log2(x);
	if (e < -126)
	{
		RD0 = 0;
		return;
	}
	else if (e > 127)
	{
		if(s==0)
			RD0 = 0x7fffffff;
		else
			RD0 = 0xffffffff;
		return;
	}
	else if (log2(x) < 0)
		e -= 1;

	x = x / (pow(2, e)) - 1;
	RD0 = x * 8388608;
	e += 127;
	RD0 = RD0.m_data + (s << 31) + (e << 23);

}

////////////////////////////////////////////////////////
//  名称:
//      Add_Float
//  功能:
//      浮点加法(单数)
//  参数:
//      1.RD0:加数0(out)
//		2.RD1:加数1
//  备注:
//      本系统浮点数为IEEE754格式的浮点数中的规约形式的32位浮点数，
//		即非规约形式的浮点数和极值不在我系统表示范围内。（详见百度百科）
////////////////////////////////////////////////////////
Sub_AutoField Add_Float
{
	double a,b;
	float2double(RD0.m_data, a);
	float2double(RD1.m_data, b);

	a += b;
	double2float(a);

	Return_AutoField(0);
}


////////////////////////////////////////////////////////
//  名称:
//      Add_Float_Dual
//  功能:
//      浮点加法(双序列)
//  参数:
//      1.RA0:加数0
//		2.RA1:加数1
//		3.RD0:序列长度
//		4.RD1:输出序列地址
//  备注:
//      本系统浮点数为IEEE754格式的浮点数中的规约形式的32位浮点数，
//		即非规约形式的浮点数和极值不在我系统表示范围内。（详见百度百科）
////////////////////////////////////////////////////////
Sub_AutoField Add_Float_Dual
{
	push(RA2);
	RA2 = RD1;

	int len = RD0.m_data;

	for (int i = 0; i < len; i++)
	{
		RD0 = GET_M(RA0 + i * MMU_BASE);
		RD1 = GET_M(RA1 + i * MMU_BASE);
		call_AutoField Add_Float;
		SET_M(RA2 + i * MMU_BASE, RD0);
	}

	pop(RA2);
	Return_AutoField(0);
}

////////////////////////////////////////////////////////
//  名称:
//      Add_Float_Const
//  功能:
//      浮点加法(序列与常数)
//  参数:
//      1.RA0:加数
//		2.RD1:常数
//		3.RD0:序列长度
//		4.RA1:输出序列地址
//  备注:
//      本系统浮点数为IEEE754格式的浮点数中的规约形式的32位浮点数，
//		即非规约形式的浮点数和极值不在我系统表示范围内。（详见百度百科）
////////////////////////////////////////////////////////
Sub_AutoField Add_Float_Const
{
	int len = RD0.m_data;

	for (int i = 0; i < len; i++)
	{
		RD0 = GET_M(RA0 + i * MMU_BASE);
		call_AutoField Add_Float;
		SET_M(RA1 + i * MMU_BASE, RD0);
	}


	Return_AutoField(0);
}

////////////////////////////////////////////////////////
//  名称:
//      Sub_Float
//  功能:
//      浮点减法(单数)
//  参数:
//      1.RD0:被减数(out)
//		2.RD1:减数
//  备注:
//      本系统浮点数为IEEE754格式的浮点数中的规约形式的32位浮点数，
//		即非规约形式的浮点数和极值不在我系统表示范围内。（详见百度百科）
////////////////////////////////////////////////////////
Sub_AutoField Sub_Float
{
	double a,b;
	float2double(RD0.m_data, a);
	float2double(RD1.m_data, b);

	a -= b;
	double2float(a);

	Return_AutoField(0);
}
////////////////////////////////////////////////////////
//  名称:
//      Sub_Float_Dual
//  功能:
//      浮点减法(双序列)
//  参数:
//      1.RA0:被减数
//		2.RA1:减数
//		3.RD0:序列长度
//		4.RD1:输出序列地址
//  备注:
//      本系统浮点数为IEEE754格式的浮点数中的规约形式的32位浮点数，
//		即非规约形式的浮点数和极值不在我系统表示范围内。（详见百度百科）
////////////////////////////////////////////////////////
Sub_AutoField Sub_Float_Dual
{
	push(RA2);
	RA2 = RD1;

	int len = RD0.m_data;

	for (int i = 0; i < len; i++)
	{
		RD0 = GET_M(RA0 + i * MMU_BASE);
		RD1 = GET_M(RA1 + i * MMU_BASE);
		call_AutoField Sub_Float;
		SET_M(RA2 + i * MMU_BASE, RD0);
	}

	pop(RA2);
	Return_AutoField(0);
}


////////////////////////////////////////////////////////
//  名称:
//      Sub_Float_Const
//  功能:
//      浮点减法(序列减常数)
//  参数:
//      1.RA0:被减数
//		2.RD1:常数
//		3.RD0:序列长度
//		4.RA1:输出序列地址
//  备注:
//      本系统浮点数为IEEE754格式的浮点数中的规约形式的32位浮点数，
//		即非规约形式的浮点数和极值不在我系统表示范围内。（详见百度百科）
////////////////////////////////////////////////////////
Sub_AutoField Sub_Float_Const
{
	int len = RD0.m_data;

	for (int i = 0; i < len; i++)
	{
		RD0 = GET_M(RA0 + i * MMU_BASE);
		call_AutoField Sub_Float;
		SET_M(RA1 + i * MMU_BASE, RD0);
	}


	Return_AutoField(0);
}

////////////////////////////////////////////////////////
//  名称:
//      Mul_Float
//  功能:
//      浮点乘法(单数)
//  参数:
//      1.RD0:乘数0(out)
//		2.RD1:乘数1
//  备注:
//      本系统浮点数为IEEE754格式的浮点数中的规约形式的32位浮点数，
//		即非规约形式的浮点数和极值不在我系统表示范围内。（详见百度百科）
////////////////////////////////////////////////////////
Sub_AutoField Mul_Float
{
	double a,b;
	float2double(RD0.m_data, a);
	float2double(RD1.m_data, b);

	a = a * b;
	double2float(a);
	Return_AutoField(0);
}
////////////////////////////////////////////////////////
//  名称:
//      Mul_Float_Dual
//  功能:
//      浮点乘法(双序列)
//  参数:
//      1.RA0:乘数0地址
//		2.RA1:乘数1地址
//		3.RD0:序列长度
//		4.RD1:输出序列地址
//  备注:
//      本系统浮点数为IEEE754格式的浮点数中的规约形式的32位浮点数，
//		即非规约形式的浮点数和极值不在我系统表示范围内。（详见百度百科）
////////////////////////////////////////////////////////
Sub_AutoField Mul_Float_Dual
{ 
	push(RA2);
	RA2 = RD1;

	int len = RD0.m_data;

	for (int i = 0; i < len; i++)
	{
		RD0 = GET_M(RA0 + i * MMU_BASE);
		RD1 = GET_M(RA1 + i * MMU_BASE);
		call_AutoField Mul_Float;
		SET_M(RA2 + i * MMU_BASE, RD0);
	}

	pop(RA2);
	Return_AutoField(0);
}


////////////////////////////////////////////////////////
//  名称:
//      Mul_Float_Const
//  功能:
//      浮点乘法(序列乘常数)
//  参数:
//      1.RA0:乘数(out)
//		2.RD1:常数
//		3.RD0:序列长度
//		4.RA1:输出序列地址
//  备注:
//      本系统浮点数为IEEE754格式的浮点数中的规约形式的32位浮点数，
//		即非规约形式的浮点数和极值不在我系统表示范围内。（详见百度百科）
////////////////////////////////////////////////////////
Sub_AutoField Mul_Float_Const
{
	int len = RD0.m_data;

	for (int i = 0; i < len; i++)
	{
		RD0 = GET_M(RA0 + i * MMU_BASE);
		call_AutoField Mul_Float;
		SET_M(RA1 + i * MMU_BASE, RD0);
	}


	Return_AutoField(0);
}


////////////////////////////////////////////////////////
//  名称:
//      Div_Float
//  功能:
//      浮点除法(单数)
//  参数:
//      1.RD0:被除数(out)
//		2.RD1:除数
//  备注:
//      本系统浮点数为IEEE754格式的浮点数中的规约形式的32位浮点数，
//		即非规约形式的浮点数和极值不在我系统表示范围内。（详见百度百科）
////////////////////////////////////////////////////////
Sub_AutoField Div_Float
{
	double a,b;
	float2double(RD0.m_data, a);
	float2double(RD1.m_data, b);

	a = a / b;
	double2float(a);

	Return_AutoField(0);
}
////////////////////////////////////////////////////////
//  名称:
//      Div_Float_Dual
//  功能:
//      浮点除法(双序列)
//  参数:
//      1.RA0:被除数地址
//		2.RA1:除数地址
//		3.RD0:序列长度
//		4.RD1:输出序列地址
//  备注:
//      本系统浮点数为IEEE754格式的浮点数中的规约形式的32位浮点数，
//		即非规约形式的浮点数和极值不在我系统表示范围内。（详见百度百科）
////////////////////////////////////////////////////////
Sub_AutoField Div_Float_Dual
{
	push(RA2);
	RA2 = RD1;

	int len = RD0.m_data;

	for (int i = 0; i < len; i++)
	{
		RD0 = GET_M(RA0 + i * MMU_BASE);
		RD1 = GET_M(RA1 + i * MMU_BASE);
		call_AutoField Div_Float;
		SET_M(RA2 + i * MMU_BASE, RD0);
	}

	pop(RA2);
	Return_AutoField(0);
}


////////////////////////////////////////////////////////
//  名称:
//      Div_Float_Const
//  功能:
//      浮点除法(序列除常数)
//  参数:
//      1.RA0:被除数地址
//		2.RD1:常数
//		3.RD0:序列长度
//		4.RA1:输出序列地址
//  备注:
//      本系统浮点数为IEEE754格式的浮点数中的规约形式的32位浮点数，
//		即非规约形式的浮点数和极值不在我系统表示范围内。（详见百度百科）
////////////////////////////////////////////////////////
Sub_AutoField Div_Float_Const
{
	int len = RD0.m_data;

	for (int i = 0; i < len; i++)
	{
		RD0 = GET_M(RA0 + i * MMU_BASE);
		call_AutoField Div_Float;
		SET_M(RA1 + i * MMU_BASE, RD0);
	}


	Return_AutoField(0);
}


////////////////////////////////////////////////////////
//  名称:
//      Recip_Float
//  功能:
//      浮点求倒数(单数)
//  参数:
//      1.RD0:数据(out)
//  备注:
//      本系统浮点数为IEEE754格式的浮点数中的规约形式的32位浮点数，
//		即非规约形式的浮点数和极值不在我系统表示范围内。（详见百度百科）
////////////////////////////////////////////////////////
Sub_AutoField Recip_Float
{
	double a;
	float2double(RD0.m_data, a);

	a = 1 / a;
	double2float(a);
	
	Return_AutoField(0);
}
////////////////////////////////////////////////////////
//  名称:
//      Recip_Float_Seq
//  功能:
//      浮点求倒数(序列)
//  参数:
//      1.RA0:数据地址
//		2.RA1:输出序列地址
//		3.RD0:序列长度
//  备注:
//      本系统浮点数为IEEE754格式的浮点数中的规约形式的32位浮点数，
//		即非规约形式的浮点数和极值不在我系统表示范围内。（详见百度百科）
////////////////////////////////////////////////////////
Sub_AutoField Recip_Float_Seq
{
	int len = RD0.m_data;

	for (int i = 0; i < len; i++)
	{
		RD0 = GET_M(RA0 + i * MMU_BASE);
		call_AutoField Recip_Float;
		SET_M(RA1 + i * MMU_BASE, RD0);
	}


	Return_AutoField(0);
}

////////////////////////////////////////////////////////
//  名称:
//      Sqrt_Float
//  功能:
//      浮点求开平方(单数)
//  参数:
//      1.RD0:数据(out)
//  备注:
//      本系统浮点数为IEEE754格式的浮点数中的规约形式的32位浮点数，
//		即非规约形式的浮点数和极值不在我系统表示范围内。（详见百度百科）
////////////////////////////////////////////////////////
Sub_AutoField Sqrt_Float
{
	double a;
	float2double(RD0.m_data, a);

	a = sqrt(a);
	double2float(a);

	Return_AutoField(0);
}
////////////////////////////////////////////////////////
//  名称:
//      Sqrt_Float_Seq
//  功能:
//      浮点求开平方(序列)
//  参数:
//      1.RA0:数据地址
//		2.RA1:输出序列地址
//		3.RD0:序列长度
//  备注:
//      本系统浮点数为IEEE754格式的浮点数中的规约形式的32位浮点数，
//		即非规约形式的浮点数和极值不在我系统表示范围内。（详见百度百科）
////////////////////////////////////////////////////////
Sub_AutoField Sqrt_Float_Seq
{
	int len = RD0.m_data;

	for (int i = 0; i < len; i++)
	{
		RD0 = GET_M(RA0 + i * MMU_BASE);
		call_AutoField Recip_Float;
		SET_M(RA1 + i * MMU_BASE, RD0);
	}


	Return_AutoField(0);
}

////////////////////////////////////////////////////////
//  名称:
//      Log2_Float
//  功能:
//      浮点求log2(单数)
//  参数:
//      1.RD0:数据(out)
//  备注:
//      本系统浮点数为IEEE754格式的浮点数中的规约形式的32位浮点数，
//		即非规约形式的浮点数和极值不在我系统表示范围内。（详见百度百科）
////////////////////////////////////////////////////////
Sub_AutoField Log2_Float
{
	double a;
	float2double(RD0.m_data, a);

	a = log2(a);
	double2float(a);

	Return_AutoField(0);
}
////////////////////////////////////////////////////////
//  名称:
//      Log2_Float_Seq
//  功能:
//      浮点求log2(序列)
//  参数:
//      1.RA0:数据地址
//		2.RA1:输出序列地址
//		3.RD0:序列长度
//  备注:
//      本系统浮点数为IEEE754格式的浮点数中的规约形式的32位浮点数，
//		即非规约形式的浮点数和极值不在我系统表示范围内。（详见百度百科）
////////////////////////////////////////////////////////
Sub_AutoField Log2_Float_Seq
{
	int len = RD0.m_data;

	for (int i = 0; i < len; i++)
	{
		RD0 = GET_M(RA0 + i * MMU_BASE);
		call_AutoField Log2_Float;
		SET_M(RA1 + i * MMU_BASE, RD0);
	}


	Return_AutoField(0);
}

////////////////////////////////////////////////////////
//  名称:
//      Log10_Float
//  功能:
//      浮点求log10(单数)
//  参数:
//      1.RD0:数据(out)
//  备注:
//      本系统浮点数为IEEE754格式的浮点数中的规约形式的32位浮点数，
//		即非规约形式的浮点数和极值不在我系统表示范围内。（详见百度百科）
////////////////////////////////////////////////////////
Sub_AutoField Log10_Float
{
	double a;
	float2double(RD0.m_data, a);

	a = log10(a);
	double2float(a);

	Return_AutoField(0);
}
////////////////////////////////////////////////////////
//  名称:
//      Log10_Float_Seq
//  功能:
//      浮点求log10(序列)
//  参数:
//      1.RA0:数据地址
//		2.RA1:输出序列地址
//		3.RD0:序列长度
//  备注:
//      本系统浮点数为IEEE754格式的浮点数中的规约形式的32位浮点数，
//		即非规约形式的浮点数和极值不在我系统表示范围内。（详见百度百科）
////////////////////////////////////////////////////////
Sub_AutoField Log10_Float_Seq
{
	int len = RD0.m_data;

	for (int i = 0; i < len; i++)
	{
		RD0 = GET_M(RA0 + i * MMU_BASE);
		call_AutoField Log10_Float;
		SET_M(RA1 + i * MMU_BASE, RD0);
	}


	Return_AutoField(0);
}

////////////////////////////////////////////////////////
//  名称:
//      Ln_Float
//  功能:
//      浮点求ln(单数)
//  参数:
//      1.RD0:数据(out)
//  备注:
//      本系统浮点数为IEEE754格式的浮点数中的规约形式的32位浮点数，
//		即非规约形式的浮点数和极值不在我系统表示范围内。（详见百度百科）
////////////////////////////////////////////////////////
Sub_AutoField Ln_Float
{
	double a;
	float2double(RD0.m_data, a);

	a = log(a);
	double2float(a);

	Return_AutoField(0);
}
////////////////////////////////////////////////////////
//  名称:
//      Ln_Float_Seq
//  功能:
//      浮点求ln(序列)
//  参数:
//      1.RA0:数据地址
//		2.RA1:输出序列地址
//		3.RD0:序列长度
//  备注:
//      本系统浮点数为IEEE754格式的浮点数中的规约形式的32位浮点数，
//		即非规约形式的浮点数和极值不在我系统表示范围内。（详见百度百科）
////////////////////////////////////////////////////////
Sub_AutoField Ln_Float_Seq
{
	int len = RD0.m_data;

	for (int i = 0; i < len; i++)
	{
		RD0 = GET_M(RA0 + i * MMU_BASE);
		call_AutoField Ln_Float;
		SET_M(RA1 + i * MMU_BASE, RD0);
	}


	Return_AutoField(0);
}


////////////////////////////////////////////////////////
//  名称:
//      Abs_Float
//  功能:
//      浮点求绝对值(单数)
//  参数:
//      1.RD0:数据(out)
//  备注:
//      本系统浮点数为IEEE754格式的浮点数中的规约形式的32位浮点数，
//		即非规约形式的浮点数和极值不在我系统表示范围内。（详见百度百科）
////////////////////////////////////////////////////////
Sub_AutoField Abs_Float
{
	double a;
	float2double(RD0.m_data, a);

	a = fabs(a);
	double2float(a);

	Return_AutoField(0);
}
////////////////////////////////////////////////////////
//  名称:
//      Abs_Float_Seq
//  功能:
//      浮点求绝对值(序列)
//  参数:
//      1.RA0:数据地址
//		2.RA1:输出序列地址
//		3.RD0:序列长度
//  备注:
//      本系统浮点数为IEEE754格式的浮点数中的规约形式的32位浮点数，
//		即非规约形式的浮点数和极值不在我系统表示范围内。（详见百度百科）
////////////////////////////////////////////////////////
Sub_AutoField Abs_Float_Seq
{
	int len = RD0.m_data;

	for (int i = 0; i < len; i++)
	{
		RD0 = GET_M(RA0 + i * MMU_BASE);
		call_AutoField Abs_Float;
		SET_M(RA1 + i * MMU_BASE, RD0);
	}

	Return_AutoField(0);
}

////////////////////////////////////////////////////////
//  名称:
//      Neg_Float
//  功能:
//      浮点求负(单数)
//  参数:
//      1.RD0:数据(out)
//  备注:
//      本系统浮点数为IEEE754格式的浮点数中的规约形式的32位浮点数，
//		即非规约形式的浮点数和极值不在我系统表示范围内。（详见百度百科）
////////////////////////////////////////////////////////
Sub_AutoField Neg_Float
{
	double a;
	float2double(RD0.m_data, a);

	a = -a;
	double2float(a);

	Return_AutoField(0);
}
////////////////////////////////////////////////////////
//  名称:
//      Neg_Float_Seq
//  功能:
//      浮点求负(序列)
//  参数:
//      1.RA0:数据地址
//		2.RA1:输出序列地址
//		3.RD0:序列长度
//  备注:
//      本系统浮点数为IEEE754格式的浮点数中的规约形式的32位浮点数，
//		即非规约形式的浮点数和极值不在我系统表示范围内。（详见百度百科）
////////////////////////////////////////////////////////
Sub_AutoField Neg_Float_Seq
{
	int len = RD0.m_data;

	for (int i = 0; i < len; i++)
	{
		RD0 = GET_M(RA0 + i * MMU_BASE);
		call_AutoField Neg_Float;
		SET_M(RA1 + i * MMU_BASE, RD0);
	}


	Return_AutoField(0);
}

////////////////////////////////////////////////////////
//  名称:
//      Power2_Float
//  功能:
//      浮点求2的指数(单数)
//  参数:
//      1.RD0:数据(out)
//  备注:
//      本系统浮点数为IEEE754格式的浮点数中的规约形式的32位浮点数，
//		即非规约形式的浮点数和极值不在我系统表示范围内。（详见百度百科）
////////////////////////////////////////////////////////
Sub_AutoField Power2_Float
{
	double a;
	float2double(RD0.m_data, a);

	a = pow(2, a);
	double2float(a);

	Return_AutoField(0);
}
////////////////////////////////////////////////////////
//  名称:
//      Power2_Float_Seq
//  功能:
//      浮点求2的指数(序列)
//  参数:
//      1.RA0:数据地址
//		2.RA1:输出序列地址
//		3.RD0:序列长度
//  备注:
//      本系统浮点数为IEEE754格式的浮点数中的规约形式的32位浮点数，
//		即非规约形式的浮点数和极值不在我系统表示范围内。（详见百度百科）
////////////////////////////////////////////////////////
Sub_AutoField Power2_Float_Seq
{
	int len = RD0.m_data;

	for (int i = 0; i < len; i++)
	{
		RD0 = GET_M(RA0 + i * MMU_BASE);
		call_AutoField Power2_Float;
		SET_M(RA1 + i * MMU_BASE, RD0);
	}


	Return_AutoField(0);
}


////////////////////////////////////////////////////////
//  名称:
//      Power10_Float
//  功能:
//      浮点求10的指数(单数)
//  参数:
//      1.RD0:数据(out)
//  备注:
//      本系统浮点数为IEEE754格式的浮点数中的规约形式的32位浮点数，
//		即非规约形式的浮点数和极值不在我系统表示范围内。（详见百度百科）
////////////////////////////////////////////////////////
Sub_AutoField Power10_Float
{
	double a;
	float2double(RD0.m_data, a);

	a = pow(10, a);
	double2float(a);

	Return_AutoField(0);
}

////////////////////////////////////////////////////////
//  名称:
//      Power10_Float_Seq
//  功能:
//      浮点求10的指数(序列)
//  参数:
//      1.RA0:数据地址
//		2.RA1:输出序列地址
//		3.RD0:序列长度
//  备注:
//      本系统浮点数为IEEE754格式的浮点数中的规约形式的32位浮点数，
//		即非规约形式的浮点数和极值不在我系统表示范围内。（详见百度百科）
////////////////////////////////////////////////////////
Sub_AutoField Power10_Float_Seq
{
	int len = RD0.m_data;

	for (int i = 0; i < len; i++)
	{
		RD0 = GET_M(RA0 + i * MMU_BASE);
		call_AutoField Power10_Float;
		SET_M(RA1 + i * MMU_BASE, RD0);
	}


	Return_AutoField(0);
}

////////////////////////////////////////////////////////
//  名称:
//      MultiSum_Float
//  功能:
//      浮点求乘累加(双序列)
//  参数:
//      1.RA0:数据
//      2.RA1:数据
//		3.RD0:序列长度,乘累加结果（out）
//  备注:
//      本系统浮点数为IEEE754格式的浮点数中的规约形式的32位浮点数，
//		即非规约形式的浮点数和极值不在我系统表示范围内。（详见百度百科）
////////////////////////////////////////////////////////
Sub_AutoField MultiSum_Float
{
	double a,b,c;
	int len = RD0.m_data;
	c = 0;
	for (int i = 0; i < len; i++)
	{
		RD0 = GET_M(RA0 + i * MMU_BASE);
		RD1 = GET_M(RA1 + i * MMU_BASE);

		float2double(RD0.m_data, a);
		float2double(RD1.m_data, b);
		c = c + a * b;
	}

	double2float(c);

	Return_AutoField(0);
}

////////////////////////////////////////////////////////
//  名称:
//      Max_Float
//  功能:
//      浮点求最大值(序列)
//  参数:
//      1.RA0:数据
//		2.RD0:序列长度,最大值结果（out）
//  备注:
//      本系统浮点数为IEEE754格式的浮点数中的规约形式的32位浮点数，
//		即非规约形式的浮点数和极值不在我系统表示范围内。（详见百度百科）
////////////////////////////////////////////////////////
Sub_AutoField Max_Float
{
	double a,b;
	int len = RD0.m_data;
	RD0 = GET_M(RA0) ;
	float2double(RD0.m_data, b);

	for (int i = 1; i < len; i++)
	{
		RD0 = GET_M(RA0 + i * MMU_BASE);
		float2double(RD0.m_data, a);
		if (a > b)
			b = a;
	}
	double2float(b);

	Return_AutoField(0);
}

////////////////////////////////////////////////////////
//  名称:
//      Min_Float
//  功能:
//      浮点求最小值(序列)
//  参数:
//      1.RA0:数据
//		2.RD0:序列长度,最小值结果（out）
//  备注:
//      本系统浮点数为IEEE754格式的浮点数中的规约形式的32位浮点数，
//		即非规约形式的浮点数和极值不在我系统表示范围内。（详见百度百科）
////////////////////////////////////////////////////////
Sub_AutoField Min_Float
{
	double a,b;
	int len = RD0.m_data;
	RD0 = GET_M(RA0);
	float2double(RD0.m_data, b);

	for (int i = 1; i < len; i++)
	{
		RD0 = GET_M(RA0 + i * MMU_BASE);
		float2double(RD0.m_data, a);
		if (a < b)
			b = a;
	}
	double2float(b);

	Return_AutoField(0);
}

////////////////////////////////////////////////////////
//  名称:
//      Mean_Float
//  功能:
//      浮点求平均值(序列)
//  参数:
//      1.RA0:数据
//		2.RD0:序列长度,平均值结果（out）
//  备注:
//      本系统浮点数为IEEE754格式的浮点数中的规约形式的32位浮点数，
//		即非规约形式的浮点数和极值不在我系统表示范围内。（详见百度百科）
////////////////////////////////////////////////////////
Sub_AutoField Mean_Float
{
	double a,b;
	int len = RD0.m_data;
	b = 0;

	for (int i = 1; i < len; i++)
	{
		RD0 = GET_M(RA0 + i * MMU_BASE);
		float2double(RD0.m_data, a);
		b = b + a;
	}
	b = b / len;
	double2float(b);

	Return_AutoField(0);
}


////////////////////////////////////////////////////////
//  名称:
//      Abs_Mean_Float
//  功能:
//      浮点求绝对值的平均值(序列)
//  参数:
//      1.RA0:数据
//		2.RD0:序列长度,平均值结果（out）
//  备注:
//      本系统浮点数为IEEE754格式的浮点数中的规约形式的32位浮点数，
//		即非规约形式的浮点数和极值不在我系统表示范围内。（详见百度百科）
////////////////////////////////////////////////////////
Sub_AutoField Abs_Mean_Float
{
	double a,b;
	int len = RD0.m_data;
	b = 0;

	for (int i = 1; i < len; i++)
	{
		RD0 = GET_M(RA0 + i * MMU_BASE);
		float2double(RD0.m_data, a);
		b = b + fabs(a);
	}
	b = b / len;
	double2float(b);

	Return_AutoField(0);
}
