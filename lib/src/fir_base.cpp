#include "fir_base.h"
#include <math.h>
#include "memory.h"
#include <string.h>
#include <stdio.h>


CFir_base fir1;



////////////////////////////////////////////////////////
//  ����:
//      Multi24_16x24
//  ����:
//      �˷�����ͨ��c����ģ��Ӳ��24λ�˷������ԡ����漰��MACӲ���ĳ˷�����ʹ�ñ����������
//  ����:
//      1.Multi24_0:��������ӦMACӲ���ڵ�RA0���У�16bit��ʽ
//      2.Multi24_1:��������ӦMACӲ���ڵ�RA1���У�24bit��ʽ
//  ����ֵֵ:
//      Multi24_0
////////////////////////////////////////////////////////
int CFir_base::Multi24_16x24(int Multi24_0, int Multi24_1)
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
	Multi24_0 = *(int*)(&x);					//��32bit 
	return Multi24_0;

}


// ��Ӳ����ʽ��ϵ��ת��Ϊ�ڲ��������õ�ϵ��a,��������Ӧ��bank��
// bank_id:ʹ����һ��bank��������,ȡֵ��Χ[0, BANK_NUM)
void CFir_base::setPara(int bank_id, short* paraArray)
{
	for (int i = 0; i < LEVEL; i++)
	{
		short a_hard = paraArray[i];
		double a_val = convertPara_hard2normal(a_hard);
		a_bank[bank_id][i] = a_val;

	}
}

//fir���ļ���
void CFir_base::FirFilterFrame(int bank, int* dataBuf, int dataLen)
{
	for (int i = 0; i < dataLen; i++)
	{
		long long s = 0;//���ۼ�ֵ
		for (int j = 0; j < LEVEL; j++)
		{
			xBuf_bank[bank][LEVEL - j - 1] = xBuf_bank[bank][LEVEL - j - 2];
		}
		xBuf_bank[bank][0] = dataBuf[i];
		//printf("0x%x\n", dataBuf[i]);
		for (int j = 0; j < LEVEL; j++)
		{
			s += Multi24_16x24(a_bank[bank][j], xBuf_bank[bank][j]);
			//int a = s >> 15;
			//printf("0x%x\n", a);
		}
		//printf("0x11223344\n");

		s = s >> 7;
		//�޷�
		if (s > 8388607)
			s = 8388607;
		else if (s < -8388608)
			s = -8388608;
		s = s & 0xffffff;
		dataBuf[i] = s;

	}
}
//  Ӳ����ʽתΪ��ͨ����ֵ
//  Ӳ����ʽ��{����λ������ֵ}
double CFir_base::convertPara_hard2normal(short input)
{
	bool sign = input >= 0 ? true : false;		// false:�� true:�Ǹ�
	short value = input & 0x7fff;


	if (!sign)
	{
		value = -value;
	}


	return value;
}





