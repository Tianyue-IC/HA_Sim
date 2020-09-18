#include "reg.h"

int CReg::m_rq = 0;
oper_status CReg::m_oper_status;

extern void m_w(int offset, int data);


CReg::CReg()
{
    m_data = 0;
}

CReg::CReg(int data)
{
    m_data = data;
}

void CReg::operator = (signed data)
{
    if (isMemID())
    {
        m_w(this->m_offset, data);
        m_data = data;
        m_rq = m_data;
    }
    else
    {
        m_data = data;
        m_rq = m_data;
    }

    
}

void CReg::operator = (CReg data)
{
    if (isMemID())
    {
        m_w(this->m_offset, data.m_data);
        m_data = data.m_data;
    }
    else
    {
        m_data = data.m_data;
        m_rq = m_data;
    }
    
}

CReg CReg::operator+(signed data)
{
    CReg temp;
    temp.m_data = m_data + data;
    return temp;
}

CReg CReg::operator-(signed data)
{
    CReg temp;
    temp.m_data = m_data - data;
    return temp;
}

int CReg::operator-()
{
    return (-m_data);
}




void CReg::operator += (signed data)
{
    if (isMemID())
    {
        m_data += data;
        m_w(this->m_offset, m_data);
    }
    else
    {
        m_oper_status.SRQ_OverFlow = isAddOverflow(m_data, data);
        m_oper_status.SRQ_OverFlow_H = isAddOverflowH(m_data, data);

        m_data += data;
        m_rq = m_data;
    }

    

}

void CReg::operator += (CReg data)
{
    if (isMemID())
    {
        m_data += data.m_data;
        m_w(this->m_offset, m_data);
    }
    else
    {
        m_oper_status.SRQ_OverFlow = isAddOverflow(m_data, data.m_data);
        m_oper_status.SRQ_OverFlow_H = isAddOverflowH(m_data, data.m_data);

        m_data += data.m_data;
        m_rq = m_data;
    }
}

void CReg::operator -= (signed data)
{
    if (isMemID())
    {
        m_data -= data;
        m_w(this->m_offset, m_data);
    }
    else
    {

        m_oper_status.RQ_ForJudge = (DWORD)m_data == (DWORD)data ? 0 : ((DWORD)m_data > (DWORD)data ? 1 : (-1));
        m_oper_status.SRQ_ForJudge = m_data == data ? 0 : (m_data > data ? 1 : (-1));
        m_oper_status.SRQ_OverFlow = isAddOverflow(m_data, -data);
        m_oper_status.SRQ_OverFlow_H = isAddOverflowH(m_data, -data);

        m_data -= data;
        m_rq = m_data;
    }
}

void CReg::operator -= (CReg data)
{
    if (isMemID())
    {
        m_data -= data.m_data;
        m_w(this->m_offset, m_data);
    }
    else
    {
        m_oper_status.RQ_ForJudge = (DWORD)m_data == (DWORD)data.m_data ? 0 : ((DWORD)m_data > (DWORD)data.m_data ? 1 : (-1));
        m_oper_status.SRQ_ForJudge = m_data == data.m_data ? 0 : (m_data > data.m_data ? 1 : (-1));
        m_oper_status.SRQ_OverFlow = isAddOverflow(m_data, -data);
        m_oper_status.SRQ_OverFlow_H = isAddOverflowH(m_data, -data);

        m_data -= data.m_data;
        m_rq = m_data;
    }
}

void CReg::operator&=(signed data)
{
    m_data &= data;
}

void CReg::operator&=(CReg data)
{
    m_data &= data.m_data;
}

void CReg::operator|=(signed data)
{
    m_data |= data;
}

void CReg::operator|=(CReg data)
{
    m_data |= data.m_data;
}

void CReg::operator^=(signed data)
{
    m_data ^= data;
}

void CReg::operator^=(CReg data)
{
    m_data ^= data.m_data;
}

// ǰ��++,�����㣬�������Ľ������
CReg CReg::operator++()
{
    *this += 4;
    return *this;
}

// ����++,�����㣬������ǰ�Ľ������
CReg CReg::operator++(int)
{
    CReg temp = *this;
    if (isRaID())
    {
        *this += 4;
    }
    else
    {
        if (isMemID())
        {
            *this += 1;
            m_w(this->m_offset, this->m_data);
        }
        else
        {
            *this += 1;
        }
    }
    return temp;
}

CReg CReg::operator--()
{
    *this -= 4;
    return *this;

}

CReg CReg::operator--(int)
{
    CReg temp = *this;
    if (isRaID())
    {
        *this -= 4;
    }
    else
    {
        if (isMemID())
        {
            *this -= 1;
            m_w(this->m_offset, this->m_data);
        }
        else
        {
            *this -= 1;
        }
    }
    return temp;

}


int CReg::operator>>(signed data)
{
    return (m_data >> data);
}

int CReg::operator<<(signed data)
{
    return (m_data << data);;
}

void CReg::operator>>=(signed data)
{
    m_data >>= data;
}

void CReg::operator<<=(signed data)
{
    m_data <<= data;
}

int CReg::operator&(signed data)
{
    return m_data&data;
}

CReg CReg::operator+(CReg data)
{
    CReg temp;
    temp.m_data = m_data + data.m_data;
    return temp;
}

CReg CReg::operator>>(CReg data)
{
    CReg temp;
    temp.m_data = m_data >> data.m_data;
    return temp;
}

CReg CReg::operator<<(CReg data)
{
    CReg temp;
    temp.m_data = m_data << data.m_data;
    return temp;
}

bool CReg::operator>(signed data)
{

    return (m_data > data);
}

bool CReg::operator<(signed data)
{
    return (m_data < data);
}

bool CReg::operator==(signed data)
{
    return (m_data == data);
}

bool CReg::operator!=(signed data)
{
    return (m_data != data);
}

bool CReg::operator>=(signed data)
{
    return (m_data >= data);
}



// ˫�������־
bool CReg::isAddOverflow(int x, int y)
{
    int sum = x + y;
    return (x > 0 && y > 0 && sum < 0) || (x < 0 && y < 0 && sum > 0);
}

// ���������־
bool CReg::isAddOverflowH(int x, int y)
{
    int sum = x + y;
    return (x > 0 && y > 0 && sum < 0);
}