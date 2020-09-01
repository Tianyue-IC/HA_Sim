#pragma once
#include "type.h"

class CReg
{
public:

    // ����
    CReg();
    CReg(int data);
    CReg(int data, int ID)
    {
        m_data = data;
        m_ID = ID;
    }

    // ��ֵ
    void operator = (signed data);      // RDx = ������
    void operator = (CReg data);        // RDx = RD0

    // ˫Ŀ����
    CReg operator +(signed data);       // RDy = RDx + ������
    CReg operator -(signed data);       // RDy = RDx - ������

    // ����󸲸�ԭֵ
    void operator += (signed data);
    void operator += (CReg data);
    void operator -= (signed data);
    void operator -= (CReg data);
    void operator &= (signed data);
    void operator &= (CReg data);
    void operator |= (signed data);
    void operator |= (CReg data);
    void operator ^= (signed data);
    void operator ^= (CReg data);

    CReg operator ++ ();
    CReg operator ++ (int);
    CReg operator -- ();
    CReg operator -- (int);


    // �߼�����
    bool operator >  (signed data);
    bool operator <  (signed data);
    bool operator ==  (signed data);
    bool operator !=  (signed data);
    bool operator >=  (signed data);


    // �����(�û���������)
    int operator -();                   // -RDx
    int operator & (signed data);       // RDx & ������
    CReg operator +(CReg data);         // RDz = RDx + RDy
    CReg operator >>(CReg data);        // RDz = RDx >> RDy
    CReg operator <<(CReg data);        // RDz = RDx << RDy
    int operator >> (signed data);
    int operator << (signed data);
    void operator >>= (signed data);
    void operator <<= (signed data);



public:
    int m_data;
    static const int ID_MEM = 0x40;     // ����Ĵ���

    bitField_union GetBitField()
    {
        bitField_union bf;
        bf.data = m_data;
        return bf;
    }

    void SetByBitField(bitField_union bf)
    {
        m_data = bf.data;
    }

    static bitField_union GetBitFieldRQ()
    {
        bitField_union bf;
        bf.data = m_rq;
        return bf;
    }


    static oper_status GetOperStatus()
    {
        return m_oper_status;
    }

    // ��������ID_MEM�Ĵ���
    void setMemOffset(int offset)
    {
        m_offset = offset;
    }

private:
    static int m_rq;
    int m_offset;
    int m_ID = 0xffffffff;                // ID��RD,RA����ݱ�ʶ,��RD0~RD15��IDΪ0~15,RA0~RA7��IDΪ24~31
    static oper_status m_oper_status;

    static bool isAddOverflow(int x, int y);

    static bool isAddOverflowH(int x, int y);

    bool isMemID()
    {
        return (m_ID == ID_MEM);
    }

    bool isRdID()
    {
        return (m_ID >= 0 && m_ID <= 15);
    }

    bool isRaID()
    {
        return (m_ID >= 24 && m_ID <= 31);
    }
};