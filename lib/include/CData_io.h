#pragma once
#include "reg.h"
#include <iostream>


// ���಻����Ӳ��������һ����
// ģ��Ӳ��������������������
class CData_io
{
public:

    CData_io(char fileIn[], char fileOut[]);                // ��ʼ��
    int getData_1Frame(CReg RAx, int frameLen);             // ȡһ֡����
    void outData_1Frame(CReg RAx, int frameLen);            // ���һ֡����
    void outData_Data32(CReg RAx, int bLen);                // ���������Ϣ(����)
    void CData_io::outData_Steam(CReg RAx, int bLen);       // ���������Ϣ(����)

    int loop = 0;                                           // ͳ�ƴ�ѭ������
private:
    int* inBuf;                                             // �������ݻ���
    char fileInPath[256];                                   // �����ļ�·��
    char fileOutPath[256];                                  // ����ļ�·��
    int inBufNum;                                           // �������ݸ���
    int inBufIndex;                                         // �������ݻ��浱ǰλ��
    FILE* fp_out;


    int readFile(char file[]);
    int readFileBin(char file[]);
    int readFileBin2(char file[]);
    int writeFile(char file[], int* buf, int len);
    int writeFileBin(char file[], int* buf, int len);

    int writeFileBin2(char file[], int* buf, int len);
    int writeFileBin_data32(int* buf, int len);
    short limit(int data);
    void createFile(char file[]);
};

