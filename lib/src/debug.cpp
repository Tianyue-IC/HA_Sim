#include "debug.h"

CData_io debug_data = CData_io("", "./user/data/debug_data_out.bin");     // ����IO���ʼ��
CData_io debug_steam = CData_io("", "./user/data/debug_steam_out.bin");     // ����IO���ʼ��
CData_io debug_vector = CData_io("", "./user/data/debug_vector_out.bin");     // ����IO���ʼ��


//////////////////////////////////////////////////////////////////////////
//  ����:
//      Export_Sound_16bit
//  ����:
//      ��16bit��ʽ����������Ƶ
//  ����:
//      1.RD0:�����׵�ַ
//      2.RD1:���ݳ���(��λ���ֽڣ�����4��������)
//  ����ֵ��
//      ��
//////////////////////////////////////////////////////////////////////////
Sub_AutoField Export_Sound_16bit
{
    RA0 = RD0;
    debug_steam.outData_Steam(RA0, RD1.m_data);
    Return_AutoField(0);
}


//////////////////////////////////////////////////////////////////////////
//  ����:
//      Export_Data_32bit
//  ����:
//      ��32bit��ʽ����������������
//  ����:
//      1.RD0:�����׵�ַ
//      2.RD1:���ݳ���(��λ���ֽڣ�����4��������)
//  ����ֵ��
//      ��
//////////////////////////////////////////////////////////////////////////
Sub_AutoField Export_Data_32bit
{

    RA0 = RD0;
    debug_data.outData_Data32(RA0, RD1.m_data);

    Return_AutoField(0);
}

//////////////////////////////////////////////////////////////////////////
//  ����:
//      Export_Vector_32bit
//  ����:
//      ��32bit��ʽ�����ؼ�����(����)��
//  ����:
//      1.RD0:�����׵�ַ
//      2.RD1:���ݳ���(��λ���ֽڣ�����4��������)
//  ����ֵ��
//      ��
//////////////////////////////////////////////////////////////////////////
Sub_AutoField Export_Vector_32bit
{
    RA0 = RD0;
    debug_data.outData_Data32(RA0, RD1.m_data);

    Return_AutoField(0);
}