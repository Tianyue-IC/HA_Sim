#include "memory.h"

int CMemBase::pTotalRam[BASERAM_SIZE_DWORD + GRAM_SIZE_DWORD + XRAM_SIZE_DWORD] = { 0 };
//int CMemBase::pGRam[GRAM_SIZE_DWORD] = { 0 };
//int CMemBase::pXRam[XRAM_SIZE_DWORD] = { 0 };

CMemBase M;


void m_w(int offset, int data)
{
    int* p = (int*)((unsigned int)CMemBase::pTotalRam + offset);
    *p = data;
}


void CMemBase::init()
{
    RSP = BASERAM_SIZE_DWORD*MMU_BASE;

}

// ����BASE_RAM�ڴ棬���ı�RSP,RSP��С
void CMemBase::malloc_local(int len)
{
    RSP -= len;
}

// �ͷ�BASE_RAM�ڴ棬�ı�RSP,RSP����
void CMemBase::release_local(int len)
{
    RSP += len;
}

// ��RAx��ָ����ڴ�
CReg CMemBase::M_R(CReg RAx)
{
    int* p = (int*)((unsigned int)pTotalRam + RAx.m_data);
    CReg result = *p;
    return result;
}

// дRAx��ָ����ڴ�
void CMemBase::M_W(CReg RAx, CReg RDx)
{
    m_w(RAx.m_data, RDx.m_data);
}


// ѹջRDx
void CMemBase::PUSH(CReg RDx)
{
    malloc_local(1 * MMU_BASE);
    M_W(RSP, RDx);
}


// ��ջ
void CMemBase::POP(CReg& RDx)
{
    RDx = M_R(RSP);
    release_local(1 * MMU_BASE);
}


// BASE_RAM�Ŀ���,lenΪ�ֽڳ���
void CMemBase::memcpy(CReg len, CReg src, CReg dst)
{
    int* psrc = (int*)((unsigned int)pTotalRam + src.m_data);
    int* pdst = (int*)((unsigned int)pTotalRam + dst.m_data);

    for (int i = 0; i < len.m_data / MMU_BASE; i++)
    {
        *(pdst + i) = *(psrc + i);
    }
}


////////////////////////////////////////////////////////
//  ����:
//      En_GRAM_To_CPU
//  ����:
//      ��GRAM����ΪCPU����ģʽ
//  ����:
//      1.RD0:��Ҫ���õ�GRAM��ַ
//  ����ֵ:
//      ��
////////////////////////////////////////////////////////
Sub_AutoField En_GRAM_To_CPU
{
    Return_AutoField(0);
}
////////////////////////////////////////////////////////
//  ����:
//      Dis_GRAM_To_CPU
//  ����:
//      ��GRAM��CPU�Ͽ�
//  ����:
//      ��
//  ����ֵ:
//      ��
////////////////////////////////////////////////////////
Sub_AutoField Dis_GRAM_To_CPU
{

Return_AutoField(0);
}
////////////////////////////////////////////////////////
//  ����:
//      En_AllRAM_To_CPU
//  ����:
//      ������GRAM��XRAM����ΪCPU����ģʽ
//  ����:
//      ��
//  ����ֵ:
//      ��
////////////////////////////////////////////////////////
Sub_AutoField En_AllRAM_To_CPU
{
Return_AutoField(0);
}
////////////////////////////////////////////////////////
//  ����:
//      En_AllGRAM_To_CPU
//  ����:
//      ������GRAM����ΪCPU����ģʽ
//  ����:
//      ��
//  ����ֵ:
//      ��
////////////////////////////////////////////////////////
Sub_AutoField En_AllGRAM_To_CPU
{
Return_AutoField(0);
}


////////////////////////////////////////////////////////
//  ����:
//      En_AllXRAM_To_CPU
//  ����:
//      ������XRAM����ΪCPU����ģʽ
//  ����:
//      ��
//  ����ֵ:
//      ��
////////////////////////////////////////////////////////
Sub_AutoField En_AllXRAM_To_CPU
{
Return_AutoField(0);
}

