#pragma once
#include "type.h"

class CFir_base
{
public:
    void FirFilterFrame(int bank, int* dataBuf, int dataLen);
    void setPara(int bank_id, short* paraArray);

private:
    static const int BANK_NUM = 4;                              // ֧�ֶ���ϵ��������BANK
    static const int LEVEL = 16;                                // һ���˲��������ļ���
    int a_bank[BANK_NUM][LEVEL] = { 0 };                        // para a
    int xBuf_bank[BANK_NUM][LEVEL] = { 0 };                     // xdata buffer
    int Multi24_16x24(int Multi24_0, int Multi24_1);
    double convertPara_hard2normal(short input);
};

extern CFir_base fir1;
