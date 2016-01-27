// ShoulderEncoderDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "motor test 20151013.h"
#include "ShoulderEncoderDlg.h"
#include "afxdialogex.h"
#include"CAxes.h"
#include"CAxis.h"
#include"CSeries.h"
#include"CTChart.h"
#include"CScroll.h"
#include <math.h>
#include "Windows.h"
#include "mmsystem.h"
#include <vector>
#pragma comment(lib,"winmm.lib")

#define Sample_Pic_Time 50//画图采样间隔
#define M_PI  3.14159265358979323846
#define Pul_To_Degree 0.045// 360/2000——新买增量式编码器脉冲数转换成角度值得系数

extern std::vector<double>Vel_Shoul_Array;
extern std::vector<double>Pos_Shoul_Array;
extern std::vector<double>Vel_Shoul_Motor;
extern std::vector <double> Time_Array;
double Enco_Shoul_Pos(0);
double Enco_Shoul_Vel(0);
double Pos_Sta_Shoul=0;
double Vel_Sta_Shoul=0;

double Vel_Error_Shoul=0;
double Pos_Error_Shoul=0;
// CShoulderEncoderDlg 对话框

IMPLEMENT_DYNAMIC(CShoulderEncoderDlg, CDialogEx)

CShoulderEncoderDlg::CShoulderEncoderDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CShoulderEncoderDlg::IDD, pParent)
	
	, Pos_S_Target(0)
	, Vel_S_Target(0)
{

}

CShoulderEncoderDlg::~CShoulderEncoderDlg()
{
}

void CShoulderEncoderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, TCHART_SHOUL_ENCODER, m_TSChart);
	DDX_Text(pDX, EDIT_SHOUL_POST, Pos_S_Target);
	DDX_Text(pDX, EDIT_SHOUL_VELT, Vel_S_Target);
}


BEGIN_MESSAGE_MAP(CShoulderEncoderDlg, CDialogEx)
	ON_BN_CLICKED(RADIO_S_ELBOW, &CShoulderEncoderDlg::OnBnClickedSElbow)
	ON_BN_CLICKED(RADIO_S_T, &CShoulderEncoderDlg::OnBnClickedST)
	ON_BN_CLICKED(BUTTON_SAVE_PIC, &CShoulderEncoderDlg::OnBnSavePic)
END_MESSAGE_MAP()


// CShoulderEncoderDlg 消息处理程序


void CShoulderEncoderDlg::OnBnClickedSElbow()
{
	// TODO: 在此添加控件通知处理程序代码
	Pos_S_Target=10;
	Vel_S_Target=0;
	UpdateData(FALSE);
}


void CShoulderEncoderDlg::OnBnClickedST()
{
	// TODO: 在此添加控件通知处理程序代码
	Pos_S_Target=57;
	Vel_S_Target=0;
	UpdateData(FALSE);
}


void CShoulderEncoderDlg::OnBnSavePic()
{
	// TODO: 在此添加控件通知处理程序代码
	CSeries lineseries[7];
	for (int j=0;j<m_TSChart.get_SeriesCount();j++)
	{
		lineseries[j]=(CSeries)m_TSChart.Series(j);
	}
	UpdateData(TRUE);
	CString szFilePath="C:\\ShoulEncoder.txt";
	CStdioFile file;
	CString str_i;
	CString strTime_shoul;
	CString str1;
	CString str2;
	int len_vector=0;
	len_vector=min(Vel_Shoul_Array.size(),Pos_Shoul_Array.size(),Time_Array.size(),Vel_Shoul_Motor.size());
	file.Open(szFilePath,CFile::modeCreate|CFile::modeWrite);
	file.WriteString("t \t");
	file.WriteString("_t \t\t");
	file.WriteString("Vel \t\t");
	file.WriteString("Pos \r\n");
	for (double i=0;i< len_vector;i++)
	{
		strTime_shoul.Format(_T("%8.5f"),Time_Array[i]);//窗口显示实时采样周期
		str1.Format( _T( "%f"), Vel_Shoul_Array[i] );//窗口显示实时速度
		str2.Format( _T( "%f"), Pos_Shoul_Array[i] );//窗口显示实时位置
		str_i.Format("%d",i);

		if (i<=50)
		{
			Pos_Sta_Shoul=Pos_S_Target*pow(i,2)/10000;
			
		}
		if (i>50 && i<=100)
		{
			Pos_Sta_Shoul=Pos_S_Target*(i/100-0.25);
			
		}
		if (i>100 && i<=150)
		{
			Pos_Sta_Shoul=Pos_S_Target*(-0.25+i/100-pow(i-100,2)/10000);
			
		}
		if (i>150 && i<=200)
		{
			Pos_Sta_Shoul=Pos_S_Target;
			
		}
		if (i>200 && i<=250)
		{
			Pos_Sta_Shoul=Pos_S_Target*(1-pow(i-200,2)/10000);
			
		}
		if (i>250 && i<=300)
		{
			Pos_Sta_Shoul=Pos_S_Target*(3.25-i/100);
			
		}
		if (i>300 && i<=350)
		{
			Pos_Sta_Shoul=Pos_S_Target*(3.25-i/100+pow(i-300,2)/10000);
			
		}
		if (i>350)
		{
			Pos_Sta_Shoul=0;
		}
		//(i<=300)?Pos_Sta_Shoul=Pos_S_Target*sin(((double)i/300)*M_PI):Pos_Sta_Shoul=0;
		Vel_Sta_Shoul=Vel_S_Target*sin(((double)i/len_vector)*M_PI);

		Pos_Error_Shoul=Pos_Sta_Shoul-Pos_Shoul_Array[i];
		Vel_Error_Shoul=Vel_Sta_Shoul-Vel_Shoul_Array[i];

		lineseries[0].AddXY((float)i*Sample_Pic_Time*0.001,Pos_Sta_Shoul,NULL,0);
		lineseries[1].AddXY((float)i*Sample_Pic_Time*0.001,Pos_Shoul_Array[i],NULL,0);
		lineseries[2].AddXY((float)i*Sample_Pic_Time*0.001,Vel_Sta_Shoul,NULL,0);
		lineseries[3].AddXY((float)i*Sample_Pic_Time*0.001,Vel_Shoul_Array[i],NULL,0);
		lineseries[4].AddXY((float)i*Sample_Pic_Time*0.001,Pos_Error_Shoul,NULL,0);
		lineseries[5].AddXY((float)i*Sample_Pic_Time*0.001,Vel_Error_Shoul,NULL,0);
		lineseries[6].AddXY((float)i*Sample_Pic_Time*0.001,Vel_Shoul_Motor[i],NULL,0);
		file.WriteString(str_i +"\t");
		file.WriteString(strTime_shoul+"ms\t");
		file.WriteString(str1+"\t");
		file.WriteString(str2);
		file.WriteString("\r\n");
	}
	file.Close();
}
