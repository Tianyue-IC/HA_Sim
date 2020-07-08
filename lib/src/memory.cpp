#include "memory.h"

int CMemBase::pTotalRam[BASERAM_SIZE_DWORD + GRAM_SIZE_DWORD + XRAM_SIZE_DWORD] = { 0 };
//int CMemBase::pGRam[GRAM_SIZE_DWORD] = { 0 };
//int CMemBase::pXRam[XRAM_SIZE_DWORD] = { 0 };

void CMemBase::init()
{
    RSP = BASERAM_SIZE_DWORD*MMU_BASE;

}

// 申请BASE_RAM内存，将改变RSP,RSP减小
void CMemBase::malloc_local(int len)
{
    RSP -= len;
}

// 释放BASE_RAM内存，改变RSP,RSP增大
void CMemBase::release_local(int len)
{
    RSP += len;
}

// 读RAx所指向的内存
CReg CMemBase::M_R(CReg RAx)
{
    int* p = (int*)((unsigned int)pTotalRam + RAx.m_data);
    CReg result = *p;
    return result;
}

// 写RAx所指向的内存
void CMemBase::M_W(CReg RAx, CReg RDx)
{
    int* p = (int*)((unsigned int)pTotalRam + RAx.m_data);
    *p = RDx.m_data;
}

// 压栈RDx
void CMemBase::PUSH(CReg RDx)
{
    malloc_local(1 * MMU_BASE);
    M_W(RSP, RDx);
}


// 退栈
void CMemBase::POP(CReg& RDx)
{
    RDx = M_R(RSP);
    release_local(1 * MMU_BASE);
}


// BASE_RAM的拷贝,len为字节长度
void CMemBase::memcpy(CReg len, CReg src, CReg dst)
{
    int* psrc = (int*)((unsigned int)pTotalRam + src.m_data);
    int* pdst = (int*)((unsigned int)pTotalRam + dst.m_data);

    for (int i = 0; i < len.m_data / MMU_BASE; i++)
    {
        *(pdst + i) = *(psrc + i);
    }
}

// BASE_RAM的拷贝,len为字节长度
//void CMemBase::memcpy_gx(int len, unsigned int src, unsigned int dst)
//{
//    int* psrc = (int*)((unsigned int)pTotalRam + src);
//    int* pdst = (int*)((unsigned int)pTotalRam + dst);
//
//    for (int i = 0; i < len / MMU_BASE; i++)
//    {
//        *(pdst + i) = *(psrc + i);
//    }
//}
