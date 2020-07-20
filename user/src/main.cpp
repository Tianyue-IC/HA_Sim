#include "main.h"




using namespace std;

// 全局变量
char File_In[] = "./user/data/data_in.bin";		// 输入音频数据路径
char File_Out[] = "./user/data/data_in.bin";	// 输出音频数据路径

CData_io data_io = CData_io("./user/data/data_in.bin", "./user/data/data_out.bin");     // 数据IO类初始化




int main()
{

	// 系统初始化
	sys_init();

	cout << "start prj" << endl;

	// ===================临时测试===================
	//test_mem_oper();
	//test_simple_oper();
	//test_conditional();
	test_float_model();


	// ===================用户代码区====================
	// 算法核心流程(以时域衰减一半为例，可修改)
	while (1)
	{
		// 从ADC取一帧数据存入RN_GRAM_IN
		RD0 = RN_GRAM_IN;
		RA0 = RD0;
		if (data_io.getData_1Frame(RA0, FRAME_LEN_Word) != FRAME_LEN_Word)
		{
			cout << "end prj" << endl;
			break;
		}


		// 数据处理
		// 逐点*0.5
		RD0 = 0x40004000;
		RD1 = FRAME_LEN_Word;
		send_para(RA0);
		send_para(RD0);
		send_para(RA0);
		send_para(RD1);
		call_AutoField MAC_MultiConst16;


		// RN_GRAM_OUT输出一帧数据到DAC
		RD0 = RN_GRAM_OUT;
		RA0 = RD0;
		data_io.outData_1Frame(RA0, FRAME_LEN_Word);


	}


	return 0;
}

void sys_init()
{
	// 本函数不允许修改
	CVersion ver111;		// 显示库版本信息
	CMemBase::init();		// 内存初始化
}


