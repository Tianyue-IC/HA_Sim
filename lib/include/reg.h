#pragma once
#include "type.h"

class CReg
{
public:

    // 构造
    CReg();
    CReg(int data);

    // 赋值
    void operator = (signed data);      // RDx = 立即数
    void operator = (CReg data);        // RDx = RD0

    // 双目运算
    CReg operator +(signed data);       // RDy = RDx + 立即数
    CReg operator -(signed data);       // RDy = RDx - 立即数

    // 运算后覆盖原值
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

    // 逻辑运算
    bool operator >  (signed data);
    bool operator <  (signed data);
    bool operator ==  (signed data);
    bool operator !=  (signed data);
    bool operator >=  (signed data);


    // 特殊的(用户程序不允许)
    int operator -();                   // -RDx
    int operator & (signed data);       // RDx & 立即数
    CReg operator +(CReg data);         // RDz = RDx + RDy
    CReg operator >>(CReg data);        // RDz = RDx >> RDy
    CReg operator <<(CReg data);        // RDz = RDx << RDy
    int operator >> (signed data);
    int operator << (signed data);
    void operator >>= (signed data);
    void operator <<= (signed data);



public:
    int m_data;

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


private:
    static int m_rq;
    static oper_status m_oper_status;

    static bool isAddOverflow(int x, int y);

    static bool isAddOverflowH(int x, int y);

};