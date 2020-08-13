#include "fir.h"
#include "fir_base.h"




////////////////////////////////////////////////////////
//  ����:
//      FIR_PATH3_HPInit_HP1
//  ����:
//      ����fir�˲���ϵ��
//  ����:
//      ��
//  ����ֵ:
//      ��
////////////////////////////////////////////////////////
Sub_AutoField FIR_PATH3_HPInit_HP1
{
    RSP -= 20 * MMU_BASE;
    RA0 = RSP;

    // firϵ��
    #define a0          0x862a
    #define a1          0x03de
    #define a2          0x0d2a
    #define a3          0x80be
    #define a4          0x9942
    #define a5          0x8c12
    #define a6          0x393e
    #define a7          0x7fff
    #define a8          0x7fff
    #define a9          0x393e  
    #define a10         0x8c12  
    #define a11         0x9942  
    #define a12         0x80be  
    #define a13         0x0d2a  
    #define a14         0x03de  
    #define a15         0x862a  
    #define c0          0x0000  


    //���������˲���ϵ��
    RD0 = a0;
    M[RA0++] = RD0;
    RD0 = a1;
    M[RA0++] = RD0;
    RD0 = a2;
    M[RA0++] = RD0;
    RD0 = a3;
    M[RA0++] = RD0;
    RD0 = a4;
    M[RA0++] = RD0;
    RD0 = a5;
    M[RA0++] = RD0;
    RD0 = a6;
    M[RA0++] = RD0;
    RD0 = a7;
    M[RA0++] = RD0;
    RD0 = a8;
    M[RA0++] = RD0;
    RD0 = a9;
    M[RA0++] = RD0;
    RD0 = a10;
    M[RA0++] = RD0;
    RD0 = a11;
    M[RA0++] = RD0;
    RD0 = a12;
    M[RA0++] = RD0;
    RD0 = a13;
    M[RA0++] = RD0;
    RD0 = a14;
    M[RA0++] = RD0;
    RD0 = a15;
    M[RA0++] = RD0;
    RD0 = c0;
    M[RA0++] = RD0;
    M[RA0++] = RD0;
    M[RA0++] = RD0;
    M[RA0++] = RD0;

    RA0 = RSP;
    RD0 = 0;                        
    call_AutoField FIR1_SetPara;



    RSP += 20 * MMU_BASE;
    Return_AutoField(0 * MMU_BASE);
}

////////////////////////////////////////////////////////
//  ��������:
//      FIR1_SetPara
//  ��������:
//      FIR1����ϵ����ģ������firϵ������
//  �β�:
//      1.RA0:ϵ������
//      2.RD0:���õڼ���bank,ȡֵ��Χ0~3
//  ����ֵ:
//      ��
////////////////////////////////////////////////////////
Sub_AutoField FIR1_SetPara
{
    int bank_id = RD0.m_data;

    short para[20];

    for (int i = 0; i < 20; i++)
    {
        RD0 = M[RA0++];
        para[i] = RD0.m_data;
    }
    fir1.setPara(bank_id, para);

    Return_AutoField(0 * MMU_BASE);
}



////////////////////////////////////////////////////////
//  ��������:
//      FIR1_Filter
//  ��������:
//      FIR�˲�
//  �β�:
//      1.RA0:�����������У���24bit��Ч
//      2.RA1:����������У���24bit��Ч
//      3.RD1:DWORD����
//      4.RD0:���õڼ���bank,ȡֵ��Χ0~3
//  ����ֵ:
//      ��
////////////////////////////////////////////////////////
Sub_AutoField FIR1_Filter
{
    int bank_id = RD0.m_data;
    int len = RD1.m_data;

    int* data_in = new int[len];

    for (int i = 0; i < len; i ++)
    {
        RD0 = M[RA0++];
        data_in[i] = RD0.m_data;
    }
    fir1.FirFilterFrame(bank_id, data_in, len);
    for (int i = 0; i < len; i ++)
    {
        RD0.m_data = data_in[i];
        M[RA1++] = RD0;
    }

    delete[] data_in;
    Return_AutoField(0 * MMU_BASE);
}