#pragma once
#include "cpu.h"
#include "memory.h"



//MAC函数与用户层交际部分，采用类硬件写法
#define SingleSerSquare SingleSerSquare()
#define MultiSum MultiSum()
#define MAC_MultiConst16 MAC_MultiConst16()
#define MultiConstH16L16 MultiConstH16L16()
#define MultiVector2 MultiVector2()
#define ModulationToZero ModulationToZero()
#define DualSequMulti_Q0 DualSequMulti_Q0()
#define WienerMultiConst24_DivQ7_LMT WienerMultiConst24_DivQ7_LMT()
#define WienerSeqMulti_DivQ7 WienerSeqMulti_DivQ7()
#define ModulationToZero2 ModulationToZero2()
#define SingleSerSquareL SingleSerSquareL()
#define MAC_MultiConst16_Q2207 MAC_MultiConst16_Q2207()


extern Sub_AutoField SingleSerSquare;

extern Sub_AutoField MultiSum;

extern Sub_AutoField MAC_MultiConst16;

extern Sub_AutoField MultiConstH16L16;

extern Sub_AutoField MultiVector2;

extern Sub_AutoField ModulationToZero;

extern Sub_AutoField DualSequMulti_Q0;

extern Sub_AutoField WienerMultiConst24_DivQ7_LMT;

extern Sub_AutoField WienerSeqMulti_DivQ7;

extern Sub_AutoField ModulationToZero2;

extern Sub_AutoField SingleSerSquareL;

extern Sub_AutoField MAC_MultiConst16_Q2207;

