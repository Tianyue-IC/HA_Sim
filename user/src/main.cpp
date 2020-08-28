#include "main.h"



using namespace std;

// ȫ�ֱ���
char File_In[] = "./user/data/data_in.bin";		// ������Ƶ����·��
char File_Out[] = "./user/data/data_out.bin";	// �����Ƶ����·��

CData_io data_io = CData_io(File_In, File_Out);     // ����IO���ʼ��



int main()
{

	// ϵͳ��ʼ��
	sys_init();

	cout << "start prj" << endl;

	// ===================��ʱ����===================
	test_mem_oper();
	//test_simple_oper();
	//test_conditional();
	//test_float_model();


	// ===================�û�������====================
	// �㷨��������(��ʱ��˥��һ��Ϊ�������޸�)
	RSP -= MMU_BASE;
	RA2 = RSP;


	while (1)
	{
		// ��ADCȡһ֡���ݴ���RN_GRAM_IN
		RD0 = RN_GRAM_IN;
		RA0 = RD0;
		if (data_io.getData_1Frame(RA0, FRAME_LEN_Word) != FRAME_LEN_Word)
		{
			cout << "end prj" << endl;
			break;
		}

		//// ���*0.5
		//RD0 = 0x40004000;
		//RD1 = FRAME_LEN_DWord;
		//send_para(RA0);
		//send_para(RD0);
		//send_para(RA0);
		//send_para(RD1);
		//call_AutoField MAC_MultiConst16;

		//M[RA2] = data_io.loop;
		//RD0 = RA2;
		//RD1 = 4;
		//call_AutoField Export_Data_32bit;

		//// ���ݴ���
		//// ��ͨ�˲�
		//RD0 = RN_GRAM_IN;
		//RA0 = RD0;
		//RA1 = RD0;
		//RD0 = FRAME_LEN_DWord;
		//call_AutoField _IIR_PATH3_HP;

		// test SingleSerSquare32
		RD0 = RN_GRAM_IN;
		RA0 = RD0;
		RD0 = RN_GRAM1;
		RA1 = RD0;
		RD0 = FRAME_LEN_DWord;
		call_AutoField SingleSerSquare32;

		RD0 = RA1;
		RD1 = FRAME_LEN_DWord*4;
		call_AutoField Export_Data_32bit;

		//RD0 = RN_GRAM_OUT;
		//RD1 = 64;
		//call_AutoField Export_Sound_16bit;



		// RN_GRAM_OUT���һ֡���ݵ�DAC
		RD0 = RN_GRAM_OUT;
		RA0 = RD0;
		data_io.outData_1Frame(RA0, FRAME_LEN_Word);


	}


	return 0;
}

void sys_init()
{
	// �����벻�����޸�
	CVersion ver111;		// ��ʾ��汾��Ϣ
	CMemBase::init();		// �ڴ��ʼ����RSP��ʼ��

	// ����Ϊ�û�������
	RD0 = RSP;
	RD1 = RN_GLOBAL_VAR_TOTAL_LEN_B;
	RD0 -= RD1;
	RSP = RD0;
	RA4 = RD0;				// RA4ָ��ȫ�ֱ�����ʼ��ַ

	// ����HPϵ��
	call_AutoField IIR_PATH3_HPInit_HP2;

}


