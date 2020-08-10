#pragma once
#include "type.h"


// �������������1/256
struct STC_CFG_IIR
{
    bool b2048;             // true:��������*1/2,g_receip��1024~2048����ʹ��  false:��Ӱ���������
    int segNum;             // 00:4�� 01:3�� 10:2�� 11:1�� ,ÿһ�ο�����4������
    bool bB3A3X2;           // true:����ʱb3��a3��2  false:����ʱb3��a3����2
    bool b1024;             // true:��������*1/4,g_receip��256~1024����ʹ��    false:��Ӱ���������
    bool bX2Add;            // true:���Ա���2�� false:���Ա���0��
    bool bX1Add;            // true:���Ա���1�� false:���Ա���0��
    bool bX05Add;           // true:���Ա���0.5�� false:���Ա���0��
    bool bX025Add;          // true:���Ա���0.25�� false:���Ա���0��
    int Xbase;              // ����Ļ������� 1 or 2 or 4
};


class CIir_base
{
public:
    int setPara(int bank_id, short*paraArray);  // paraArray��ʽ��[b11-b15 a12-a15 CFG_IIR, b21-b25 a22-a25 CFG_IIR...],һ��10����
    int iirFilterFrame(int bank, int* dataBuf, int dataLen);

private:
    static const int BANK_NUM = 4;                              // ֧�ֶ���ϵ��������BANK
    static const int ORDER_ADD1 = 5;                            // �˲�������+1
    static const int LEVEL = 4;                                 // һ���˲��������ļ���
    double a_bank[BANK_NUM][LEVEL * ORDER_ADD1] = { 0 };        // para a
    double b_bank[BANK_NUM][LEVEL * ORDER_ADD1] = { 0 };        // para b
    double gain_bank[BANK_NUM] = { 0 };                         // ���ǰͳһ��������
    double xBuf_bank[BANK_NUM][LEVEL * ORDER_ADD1] = { 0 };        // xdata buffer
    double yBuf_bank[BANK_NUM][LEVEL * ORDER_ADD1] = { 0 };        // ydata buffer

    int MultiHLS(int a24, int b16);
    int multi32_8(int a24, int b16);
    int iirFilterLevels(int* dataBuf, int dataLen, int bank_id, int levelNum);                                        // �༶iir
    int iirFilter1Level(double* a, double* b, double* xBufEx, double* yBufEx, int* dataBuf, int dataLen);     // 1��iir

    void calGain(WORD CFG_IIR, double& gain, bool& bB3A3X2);
    void convert(WORD CFG_IIR, STC_CFG_IIR& stcCfg);
    double convertPara_hard2normal(short input, bool type);
};

extern CIir_base iir1;
