#include "FFT_F.h"
#include "memory.h"
#include <math.h>
#include <stdio.h>

#define _CRT_FUNCTIONS_REQUIRED

extern void fft(double pr[], double pi[], int n, double fr[], double fi[]);

////////////////////////////////////////////////////////
//  函数名称:
//      FFT_fix64
//  函数功能:
//      FFT运算
//  形参:
//      1.RD0:输入序列指针，复数格式
//      2.RD1:输出序列指针，复数格式
//  返回值:
//      RD0:增益系数
////////////////////////////////////////////////////////
Sub_AutoField FFT_fix64
{
	double pr[64], pi[64], fr[64], fi[64], x;

	RA0 = RD0;
	RA1 = RD1;

	for (int i = 0; i <= 63; i++)
	{
		RD0 = GET_M(RA0 + i * MMU_BASE);
        x = floor(RD0.m_data / 65536);
        x = x / 32768;
        pr[i] = x;
        x = *(short*)(&RD0.m_data);					//低16bit 
        x = x / 32768;
        pi[i] = x;
    }
	fft(pr, pi, 64, fr, fi);    //fft

   //限幅判溢出
    int k = RD0.m_data;
  //  double max0 = 0;
  //  double max1 = 0;
  //  for (int i = 0; i < 64; i++)
  //  {
  //      if (fabs(fr[i]) > fabs(max0))
  //          max0 = fabs(fr[i]);
  //      if (fabs(fi[i]) > fabs(max1))
  //          max1 = fabs(fi[i]);
  //  }
  //  if (fabs(max0) < fabs(max1))
  //      max0 = max1;
  //  if (max0 < 1)
  //  {
  //      k = 0;
  //  }
  //  else
  //  {
  //      k = log2(max0) + 1;
  //  }


  //  int l = 1 << k;
  //  for (int i = 0; i < 64; i++)
  //  {
  //      fr[i] = fr[i] / l;
  //      fi[i] = fi[i] / l;
  //  }


    for (int i = 0; i <= 63; i++)
    {
        x = fr[i];
		x = x * 32768;
        RD0 = x;
        RD0 <<= 16;
        x = fi[i];
        x = x * 32768;
        RD1 = x;
        RD0 += RD1;
        SET_M(RA1 + i * MMU_BASE, RD0);

    }
    RD0 = k;
    Return_AutoField(0);

}

//////////////////////////////////////////////////////////
////  函数名称:
////      FFT_32X128
////  函数功能:
////      128点宽位FFT运算
////  形参:
////      1.RD0:输入序列指针，32bit格式，顺序为R0;I0;R1;I1...
////  返回值:
////      RD0:增益系数
//////////////////////////////////////////////////////////
//Sub_AutoField FFT_Fast128
//{
//    double pr[128], pi[128], fr[128], fi[128], x;
//
//    RA0 = RD0;
//
//    for (int i = 0; i < 128; i++)
//    {
//        RD0 = M[RA0++];
//        x = RD0.m_data;
//        x = x / 32768;
//        pr[i] = x;
//        RD0 = M[RA0++];
//        x = RD0.m_data;
//        x = x / 32768;
//        pi[i] = x;
//    }
//    fft(pr, pi, 128, fr, fi);    //fft
//
//    //限幅判溢出
//    int k = 0;
//    double max0 = 0;
//    double max1 = 0;
//    for (int i = 0; i < 128; i++)
//    {
//        if (fabs(fr[i]) > fabs(max0))
//            max0 = fabs(fr[i]);
//        if (fabs(fi[i]) > fabs(max1))
//            max1 = fabs(fi[i]);
//    }
//    if (fabs(max0) < fabs(max1))
//        max0 = max1;
//    if (max0<65536)
//    {
//        k = 0;
//    }
//    else
//    {
//    k = log2(max0/65536) + 1;
//    }
//
//    
//    int l = 1 << k;
//    for (int i = 0; i < 128; i++)
//    {
//        fr[i] = fr[i] / l;
//        fi[i] = fi[i] / l;
//    }
//
//
//    for (int i = 0; i < 128; i++)
//    {
//        x = fr[i];
//        x = x * 32768;
//        RD0 = x;
//        M[RA1++] = RD0;
//
//        x = fi[i];
//        x = x * 32768;
//        RD0 = x;
//        M[RA1++] = RD0;
//    }
//    RD0 = k;
//    Return_AutoField(0);
//
//}


////////////////////////////////////////////////////////
//  函数名称:
//      FFT_Fast128
//  函数功能:
//      128点FFT运算，采用128点专用加速器
//  形参:
//      1.RD0:输入序列指针，复数格式
//      2.RD1:输出序列指针，复数格式
//  返回值:
//      RD0:增益系数
////////////////////////////////////////////////////////
Sub_AutoField FFT_Fast128
{
    double pr[128], pi[128], fr[128], fi[128], x;

    RA0 = RD0;
    RA1 = RD1;

    for (int i = 0; i < 128; i++)
    {
        RD0 = GET_M(RA0 + i * MMU_BASE);
		x = floor(RD0.m_data / 65536);
        x = floor(x / 2);
        x = x / 32768;
        pr[i] = x;
        x = *(short*)(&RD0.m_data);					//低16bit 
        x = floor(x / 2);
        x = x / 32768;
        pi[i] = x;
    }
    fft(pr, pi, 128, fr, fi);    //fft

    //限幅判溢出
    int k = RD0.m_data;
    double max0 = 0;
    double max1 = 0;
    for (int i = 0; i < 128; i++)
    {
        if (fabs(fr[i]) > fabs(max0))
            max0 = fabs(fr[i]);
        if (fabs(fi[i]) > fabs(max1))
            max1 = fabs(fi[i]);
    }
    if (fabs(max0) < fabs(max1))
        max0 = max1;
    if (max0<1)
    {
        k = 0;
    }
    else
    {
    k = log2(max0) + 1;
    }

    
    int l = 1 << k;
    for (int i = 0; i < 128; i++)
    {
        fr[i] = fr[i] / l;
        fi[i] = fi[i] / l;
    }


    for (int i = 0; i < 128; i++)
    {
        x = fr[i];
        x = x * 32768;
        RD0 = x;
        RD0 <<= 16;
        x = fi[i];
        x = x * 32768;
        RD1 = x;
        RD1 &= 0xffff;
        RD0 += RD1;
        SET_M(RA1 + i * MMU_BASE, RD0);

    }
    RD0 = k;
    Return_AutoField(0);

}
/////////////////////////////////////////////////////////////////////////////
//  名称:
//      fft
//  功能:
//      fft计算
//  参数:
//      double pr[n]	存放n个采样输入的实部，返回离散傅里叶变换的摸
//      double pi[n]	存放n个采样输入的虚部
//      double fr[n]	返回离散傅里叶变换的n个实部
//      double fi[n]	返回离散傅里叶变换的n个虚部
//      int n	        采样点数
//      int k	        满足n = 2^k 
//  返回值值:
//      无
//////////////////////////////////////////////////////////////////////////////
void fft(double pr[], double pi[], int n, double fr[], double fi[])
{
    RD0 = 1;
	int k = log2(n);
    int it, m, is, i, j, nv, l0;
    double p, q, s, vr, vi, poddr, poddi;

    //FILE* fo = fopen("Out.txt", "w");//打印中间量

    for (it = 0; it <= n - 1; it++)  //将pr[0]和pi[0]循环赋值给fr[]和fi[]
    {
        m = it;
        is = 0;
        for (i = 0; i <= k - 1; i++)
        {
            j = m / 2;
            is = 2 * is + (m - 2 * j);
            m = j;
        }
        fr[it] = pr[is];
        fi[it] = pi[is];
    }

  //  fprintf(fo, "//逆序结果\r");
  //  for (int qq = 0; qq < 128; qq++)
  //  {
  //      int x = fr[qq] * 32768;
  //      int y = fi[qq] * 32768;
		//x = (x << 16) + (y & 0xffff);
  //      fprintf(fo, "%08x\r", x);
  //  }


    pr[0] = 1.0;
    pi[0] = 0.0;
    p = 6.283185306 / (1.0 * n);
    pr[1] = cos(p); //将w=e^-j2pi/n用欧拉公式表示
    pi[1] = -sin(p);

    for (i = 2; i <= n - 1; i++)  //计算pr[]，系数
    {
        p = pr[i - 1] * pr[1];
        q = pi[i - 1] * pi[1];
        s = (pr[i - 1] + pi[i - 1]) * (pr[1] + pi[1]);
        pr[i] = p - q; pi[i] = s - p - q;
    }

    //fprintf(fo, "//系数表\r");
    //for (int qq = 0; qq < 128; qq++)
    //{
    //    int x = pr[qq] * 32768;
    //    int y = pi[qq] * 32768;
    //    x = (x << 16) + (y & 0xffff);
    //    fprintf(fo, "%08x\r", x);
    //}

    for (it = 0; it <= n - 2; it = it + 2)//第一轮蝶形
    {
        vr = fr[it];
        vi = fi[it];
        fr[it] = vr + fr[it + 1];
        fi[it] = vi + fi[it + 1];
        fr[it + 1] = vr - fr[it + 1];
        fi[it + 1] = vi - fi[it + 1];
    }
    //fprintf(fo, "//第一轮蝶形\r");
    //for (int qq = 0; qq < 128; qq++)
    //{
    //    int x = fr[qq] * 32768;
    //    int y = fi[qq] * 32768;
    //    x = (x << 16) + (y & 0xffff);
    //    fprintf(fo, "%08x\r", x);
    //}

    m = n / 2;
    nv = 2;
    for (l0 = k - 2; l0 >= 0; l0--) //蝴蝶操作
    {
        m = m / 2;
        nv = 2 * nv;
        for (it = 0; it <= (m - 1) * nv; it = it + nv)
            for (j = 0; j <= (nv / 2) - 1; j++)
            {
                p = pr[m * j] * fr[it + j + nv / 2];
                int xx = p * 32768;//涉及xx的注释仅用作与硬件机制相同，但会降低精度，同matlab结果验证时注释掉xx
                p = xx;//xx
                p = p / 32768;//xx
                q = pi[m * j] * fi[it + j + nv / 2];
                xx = q * 32768;//xx
                q = xx;//xx
                q = q / 32768;//xx
                s = pr[m * j] + pi[m * j];
                s = s * (fr[it + j + nv / 2] + fi[it + j + nv / 2]);
                xx = s * 32768;//xx
                s = xx;//xx
                s = s / 32768;//xx
                poddr = p - q;
                poddi = s - p - q;
                fr[it + j + nv / 2] = fr[it + j] - poddr;
                fi[it + j + nv / 2] = fi[it + j] - poddi;
                fr[it + j] = fr[it + j] + poddr;
                fi[it + j] = fi[it + j] + poddi;
            }
        //fprintf(fo, "//第%d轮蝶形\r", 7-l0);
        //for (int qq = 0; qq < 128; qq++)
        //{
        //    int x = fr[qq] * 32768;
        //    int y = fi[qq] * 32768;
        //    x = (x << 16) + (y & 0xffff);
        //    fprintf(fo, "%08x\r", x);
        //}
    
    //更符合硬件模式的判溢出机制
    //    int order = 0;
    //    for (int ii = 0; ii < n; ii++)
    //    {
    //        if (fabs(fr[ii]) >= 0.25)
    //            order = 1;
    //        if (fabs(fi[ii]) >= 0.25)
    //            order = 1;
    //    }
    //    if (order == 1)
    //    {
    //        for (int ii = 0; ii < n; ii++)
    //        {
				//fr[ii] = fr[ii] / 2;
				//fi[ii] = fi[ii] / 2;
    //            RD0++;
    //        }
    //    }
    }
    for (i = 0; i <= n - 1; i++)
    {
        pr[i] = sqrt(fr[i] * fr[i] + fi[i] * fi[i]);  //幅值计算
    }
    //fclose(fo);

    return;
}
