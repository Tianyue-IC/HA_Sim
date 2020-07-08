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

// 从inBuf取一帧数据，如果取完或失败返回0，正确返回数据个数
int CData_io::getData_1Frame(CReg RAx, int frameLen)
{
    int* frame = new int[frameLen];
    if (frame == NULL)
    {
        return 0;
    }

    if (inBufNum < (inBufIndex + frameLen))
    {
        delete[] frame;
        return 0;
    }

    for (int i = 0; i < frameLen; i++)
    {
        frame[i] = inBuf[inBufIndex + i];
    }
    inBufIndex += frameLen;

    // frame -> RAx
    for (int i = 0; i < frameLen; i++)
    {
        SET_M(RAx + i * MMU_BASE, frame[i]);
    }


    delete[] frame;
    return frameLen;
}


// 从RAx取一帧数据，写入到输出文件中
void CData_io::outData_1Frame(CReg RAx, int frameLen)
{
    int* frame = new int[frameLen];
    if (frame == NULL)
    {
        return;
    }    

    // frame <- RAx
    for (int i = 0; i < frameLen; i++)
    {
        CReg tempRD = GET_M(RAx + i * MMU_BASE);
        frame[i] = tempRD.m_data;
    }

    // 写入文件
    writeFile(fileOutPath, frame, frameLen);

    delete[] frame;
    return;
}

// 读取文件的数据内容到inBuf
int CData_io::readFile(char file[])
{
    // 读取文件后缀, .bin .txt .wav等
    char* pTail = strrchr(file, '.');
    if (strcmp(pTail, ".bin") == 0)
    {
        return readFileBin(file);
    }
    else
    {
        return 0;
    }


    

    return 0;
}

// 读取.bin文件, 数据格式为音频数据紧密排列，每个采样点为2字节，大端格式
// 返回读取的
int CData_io::readFileBin(char file[])
{
    int len = 0;

    // 取文件大小
    struct _stat info;
    _stat(file, &info);
    int size = info.st_size;

    // 打开文件
    FILE* fp = fopen(file, "rb");
    if (fp == NULL)
    {
        return len;
    }
    
    // inBuf申请空间
    int num = size / 2;
    inBuf = new int[num];
    if (inBuf == NULL)
    {
        return len;
    }
    inBufNum = num;

    // 读文件内容到inbuf
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

// 将buf写入文件，写入方式是append
int CData_io::writeFile(char file[], int *buf, int len)
{
    // 读取文件后缀, .bin .txt .wav等
    char* pTail = strrchr(file, '.');
    if (strcmp(pTail, ".bin") == 0)
    {
        return writeFileBin(file, buf, len);
    }
    else
    {
        return 0;
    }

    return 0;
}

// 将buf写入文件，写入方式是append, 数据格式为音频数据紧密排列，每个采样点为2字节，大端格式
int CData_io::writeFileBin(char file[], int* buf, int len)
{
    int retlen = 0;
    short temp;
    unsigned char tempByte[2];

    // 打开文件
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

// int转short，并且将数值范围限制在 -32767到32767
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
    if (strlen(file) <= 0)
    {
        return;
    }

    FILE* fp = fopen(file, "wb");
    if (fp == NULL)
    {
        return;
    }
    fclose(fp);

}