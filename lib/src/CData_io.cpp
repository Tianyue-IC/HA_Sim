#include "CData_io.h"
#include <iostream>
#include "memory.h"

int CData_io::* inBuf = NULL;

CData_io::CData_io(char fileIn[], char fileOut[])
{
    inBufIndex = 0;

    strcpy(fileInPath, fileIn);
    strcpy(fileOutPath, fileOut);
    readFile(fileInPath);
    createFile(fileOutPath);
}

// ��inBufȡһ֡����(��32)�����ȡ���ʧ�ܷ���0����ȷ����ȡ�������ݸ���
// ���ݽ�������[n+1|n]����ռ��RAxָ���frameLen/2��DWORD
int CData_io::getData_1Frame(CReg RAx, int frameLen)
{
    int dwLen = frameLen / 2;

    int* frame = new int[dwLen];
    if (frame == NULL)
    {
        return 0;
    }

    if (inBufNum < (inBufIndex + dwLen))
    {
        delete[] frame;
        return 0;
    }

    for (int i = 0; i < dwLen; i++)
    {
        frame[i] = inBuf[inBufIndex + i];
    }
    inBufIndex += dwLen;

    // frame -> RAx
    for (int i = 0; i < dwLen; i++)
    {
        SET_M(RAx + i * MMU_BASE, frame[i]);
    }


    delete[] frame;

    loop++;
    return frameLen;
}


// ��RAxȡһ֡���ݣ�д�뵽����ļ���
// RAx������Ϊ��������[n+1|n]��ռ��frameLen/2��DWORD��ÿ��DWORD��������������
void CData_io::outData_1Frame(CReg RAx, int frameLen)
{
    int dwLen = frameLen / 2;

    int* frame = new int[dwLen];
    if (frame == NULL)
    {
        return;
    }    

    // frame <- RAx
    for (int i = 0; i < dwLen; i++)
    {
        CReg tempRD = GET_M(RAx + i * MMU_BASE);
        frame[i] = tempRD.m_data;
    }

    // д���ļ�
    writeFile(fileOutPath, frame, dwLen);

    delete[] frame;
    return;
}

// ��RAxȡbLen�ֽ����ݣ�˳��д�뵽����ļ���
void CData_io::outData_Data32(CReg RAx, int bLen)
{
    int dwLen = bLen / 4;

    int* frame = new int[dwLen];
    if (frame == NULL)
    {
        return;
    }

    // frame <- RAx
    for (int i = 0; i < dwLen; i++)
    {
        CReg tempRD = GET_M(RAx + i * MMU_BASE);
        frame[i] = tempRD.m_data;
    }

    // д���ļ�
    writeFileBin_data32(frame, dwLen);

    delete[] frame;
    return;
}

// ��RAxȡbLen�ֽ����ݣ�Ӳ�����ո�ʽ[n+1|n]������Ϊ˳�����[n|n+1]��ʽд�뵽����ļ���
void CData_io::outData_Steam(CReg RAx, int bLen)
{
    outData_1Frame(RAx, bLen / 2);
}



// ��ȡ�ļ����������ݵ�inBuf
int CData_io::readFile(char file[])
{
    if (strlen(file) <= 1 )
    {
        return 0;
    }


    // ��ȡ�ļ���׺, .bin .txt .wav��
    char* pTail = strrchr(file, '.');
    if (strcmp(pTail, ".bin") == 0)
    {
        return readFileBin2(file);
    }
    else
    {
        return 0;
    }


    

    return 0;
}

// ��ȡ.bin�ļ�, ���ݸ�ʽΪ��Ƶ���ݽ������У�ÿ��������Ϊ2�ֽڣ���˸�ʽ
// ��ȡ��������������ɢ���У���һ��DWORDһ����
// ���ض�ȡ��
int CData_io::readFileBin(char file[])
{
    int len = 0;

    // ȡ�ļ���С
    struct _stat info;
    _stat(file, &info);
    int size = info.st_size;

    // ���ļ�
    FILE* fp = fopen(file, "rb");
    if (fp == NULL)
    {
        return len;
    }
    
    // inBuf����ռ�
    int num = size / 2;
    inBuf = new int[num];
    if (inBuf == NULL)
    {
        return len;
    }
    inBufNum = num;

    // ���ļ����ݵ�inbuf
    unsigned char byte2[2];
    while (!feof(fp))
    {
        fread(byte2, 1, 2, fp);        
        unsigned char tempByte = byte2[0];
        byte2[0] = byte2[1];
        byte2[1] = tempByte;
        short tempShort = *(short*)byte2;
        inBuf[len++] = (int)tempShort;
    }
    fclose(fp);


    return len;
}


// ��ȡ.bin�ļ�, ���ݸ�ʽΪ��Ƶ���ݽ������У�ÿ��������Ϊ2�ֽڣ���˸�ʽ
// ��ȡ�����������ǽ������У���һ��DWORD��������[n+1 | n]
// ���ض�ȡ��
int CData_io::readFileBin2(char file[])
{
    int len = 0;

    // ȡ�ļ���С
    struct _stat info;
    _stat(file, &info);
    int size = info.st_size;

    // ���ļ�
    FILE* fp = fopen(file, "rb");
    if (fp == NULL)
    {
        return len;
    }

    // inBuf����ռ�
    int num = size / 4;
    inBuf = new int[num];
    if (inBuf == NULL)
    {
        return len;
    }
    inBufNum = num;

    // ���ļ����ݵ�inbuf
    unsigned char byte4[4];
    while (!feof(fp))
    {
        fread(byte4, 1, 4, fp);
        BYTE* pTemp = (BYTE*)&inBuf[len++];
        pTemp[3] = byte4[2];
        pTemp[2] = byte4[3];
        pTemp[1] = byte4[0];
        pTemp[0] = byte4[1];
    }
    fclose(fp);


    return len;
}

// ��bufд���ļ���д�뷽ʽ��append
int CData_io::writeFile(char file[], int *buf, int len)
{
    // ��ȡ�ļ���׺, .bin .txt .wav��
    char* pTail = strrchr(file, '.');
    if (strcmp(pTail, ".bin") == 0)
    {
        return writeFileBin2(file, buf, len);
    }
    else
    {
        return 0;
    }

    return 0;
}




// ��bufд���ļ���д�뷽ʽ��append, ���ݸ�ʽΪ��Ƶ���ݽ������У�ÿ��������Ϊ2�ֽڣ���˸�ʽ
int CData_io::writeFileBin(char file[], int* buf, int len)
{
    int retlen = 0;
    short temp;
    unsigned char tempByte[2];

    // ���ļ�
    FILE* fp = fopen(file, "ab");
    if (fp == NULL)
    {
        return retlen;
    }

    for (int i = 0; i < len; i++)
    {
        temp = limit(buf[i]);
        tempByte[0] = *((unsigned char*)(&temp) + 1);
        tempByte[1] = *((unsigned char*)(&temp) + 0);

        fwrite(tempByte, 1, 2, fp);
        retlen += 2;
    }
    

    fclose(fp);

    return retlen;
}

// buf�ĸ�ʽΪӲ�����ո�ʽ[n+1|n]��ÿ��������Ϊ2�ֽڣ���˸�ʽ
// ��bufд���ļ���д�뷽ʽ��append, ���ݸ�ʽΪ��Ƶ���ݽ�������[n|n+1]��ÿ��������Ϊ2�ֽڣ���˸�ʽ
// ����д���ļ��Ĳ�������
int CData_io::writeFileBin2(char file[], int* buf, int len)
{
    int retlen = 0;
    unsigned char tempByte[4];

    // ���ļ�
    FILE* fp = fp_out;
    if (fp == NULL)
    {
        return retlen;
    }

    for (int i = 0; i < len; i++)
    {
        BYTE* pTemp = (BYTE*)&buf[i];
        tempByte[0] = pTemp[1];
        tempByte[1] = pTemp[0];
        tempByte[2] = pTemp[3];
        tempByte[3] = pTemp[2];

        fwrite(tempByte, 1, 4, fp);
        retlen += 2;
    }


    //fclose(fp);

    return retlen;
}


// ��buf����˳��д���ļ���ÿ����ռ���ֽڴ�˸�ʽ��д�뷽ʽ��append,
// ����д���ļ����ֽ���
int CData_io::writeFileBin_data32(int* buf, int len)
{
    int retlen = 0;
    unsigned char tempByte[4];

    // ���ļ�
    FILE* fp = fp_out;
    if (fp == NULL)
    {
        return retlen;
    }

    for (int i = 0; i < len; i++)
    {
        BYTE* pTemp = (BYTE*)&buf[i];
        tempByte[0] = pTemp[3];
        tempByte[1] = pTemp[2];
        tempByte[2] = pTemp[1];
        tempByte[3] = pTemp[0];

        fwrite(tempByte, 1, 4, fp);
        retlen += 4;
    }

    return retlen;
}


// intתshort�����ҽ���ֵ��Χ������ -32767��32767
short CData_io::limit(int data)
{
    short rst;

    if (data > 32767)
    {
        rst = 32767;
    }
    else if (data < -32767)
    {
        rst = -32767;
    }
    else
    {
        rst = (short)data;
    }
    return rst;
}

void CData_io::createFile(char file[])
{
    fp_out = NULL;

    if (strlen(file) <= 0)
    {
        return;
    }

    FILE* fp = fopen(file, "wb");
    if (fp == NULL)
    {
        return;
    }
    
    fp_out = fp;

}



