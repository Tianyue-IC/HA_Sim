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
	//test_float_model();


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

		//// 逐点*0.5
		//RD0 = 0x40004000;
		//RD1 = FRAME_LEN_DWord;
		//send_para(RA0);
		//send_para(RD0);
		//send_para(RA0);
		//send_para(RD1);
		//call_AutoField MAC_MultiConst16;



		// 数据处理
		// 高通滤波
		RD0 = RN_GRAM_IN;
		RA0 = RD0;
		RA1 = RD0;
		RD0 = FRAME_LEN_DWord;
		call_AutoField _IIR_PATH3_HP;





		// RN_GRAM_OUT输出一帧数据到DAC
		RD0 = RN_GRAM_OUT;
		RA0 = RD0;
		data_io.outData_1Frame(RA0, FRAME_LEN_Word);


	}


	return 0;
}

void sys_init()
{
	// 这块代码不允许修改
	CVersion ver111;		// 显示库版本信息
	CMemBase::init();		// 内存初始化，RSP初始化

	// 以下为用户代码区
	RD0 = RSP;
	RD1 = RN_GLOBAL_VAR_TOTAL_LEN_B;
	RD0 -= RD1;
	RSP = RD0;
	RA4 = RD0;				// RA4指向全局变量起始地址

	// 配置HP系数
	call_AutoField IIR_PATH3_HPInit_HP2;

}


