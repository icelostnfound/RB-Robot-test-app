// ArmEncoderDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "motor test 20151013.h"
#include "motor test 20151013Dlg.h"
#include "ArmEncoderDlg.h"
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
#define Sample_Vel_Time 50//速度采样间隔
#define Sample_Pic_Time 10//画图采样间隔
#define M_PI  3.14159265358979323846
#define Pul_To_Degree 0.045// 360/2000——新买增量式编码器脉冲数转换成角度值得系数

VOID CALLBACK OnMediaTimer(UINT wTimerID,UINT msg,DWORD dwUser,DWORD dw1,DWORD dw2);
// CArmEncoderDlg 对话框

UINT PAccuracy;// 定义分辨率
MMRESULT Ptimer_ID;
double Enco_Arm_Pos(0);

double Enco_Arm_Vel(0);

double Pos_Sta_Arm=0;
double Vel_Sta_Arm=0;
double Vel_Error=0;
double Pos_Error=0;
double i=0;
extern double Posi_Angle_Arm;
extern double Vel_Real_AA;
extern std::vector <double> Vel_Arm_Array;

extern std::vector <double> Pos_Arm_Array;
extern std::vector <double> Vel_Arm_Motor;
extern std::vector <double> Time_Array;
CString strTime1;
CMutex m_mutex;
Cmotortest20151013Dlg *m_pDlg=NULL;
CArmEncoderDlg *A_pDlg=NULL;
IMPLEMENT_DYNAMIC(CArmEncoderDlg, CDialogEx)

CArmEncoderDlg::CArmEncoderDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CArmEncoderDlg::IDD, pParent)
	
	, Pos_Target(0)
	, Vel_Target(0)
{
	A_pDlg=this;
	
}

CArmEncoderDlg::~CArmEncoderDlg()
{
	timeKillEvent(Ptimer_ID);
	timeEndPeriod(PAccuracy);
	delete m_pDlg;
}

void CArmEncoderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, TCHART_ARM_ENCODER, m_Tchart);
	DDX_Text(pDX, EDIT_ARM_POST, Pos_Target);
	DDX_Text(pDX, EDIT_ARM_VELT, Vel_Target);
}


BEGIN_MESSAGE_MAP(CArmEncoderDlg, CDialogEx)
	ON_BN_CLICKED(BUTTON_BEGAIN, &CArmEncoderDlg::OnBnBegain)
	ON_BN_CLICKED(BUTTON_STOP, &CArmEncoderDlg::OnBnStop)
	ON_BN_CLICKED(BUTTON_SAVE, &CArmEncoderDlg::OnBnClickedSave)
	ON_BN_CLICKED(RADIO_ELBOW, &CArmEncoderDlg::OnBnClickedElbow)
	ON_BN_CLICKED(RADIO_SHOUL, &CArmEncoderDlg::OnBnClickedShoul)
END_MESSAGE_MAP()


// CArmEncoderDlg 消息处理程序
VOID CALLBACK OnMediaTimer(UINT wTimerID,UINT msg,DWORD dwUser,DWORD dw1,DWORD dw2)
{
	CString str;
	CString str1;
	CString str2;
	CString str3;
	CSeries lineseries1;
	CSeries lineseries2;
	CSeries lineseries3;
	CSeries lineseries4;
	CSeries lineseries5;
	CSeries lineseries6;
	//double Motor_Pos=0;
	lineseries1=(CSeries)A_pDlg->m_Tchart.Series(0);
	lineseries2=(CSeries)A_pDlg->m_Tchart.Series(1);
	lineseries3=(CSeries)A_pDlg->m_Tchart.Series(2);
	lineseries4=(CSeries)A_pDlg->m_Tchart.Series(3);
	lineseries5=(CSeries)A_pDlg->m_Tchart.Series(4);
	lineseries6=(CSeries)A_pDlg->m_Tchart.Series(5);
	/*APS_get_position_f(0, &Motor_Pos);*/
	//Enco_Arm_Pos=Motor_Pos*Pul_To_Degree ;
	m_mutex.Lock();
	Enco_Arm_Pos=Posi_Angle_Arm;
	Enco_Arm_Vel=Vel_Real_AA;
	m_mutex.Unlock();

	Pos_Sta_Arm=A_pDlg->Pos_Target*sin(i/1000*M_PI);
	Vel_Sta_Arm=A_pDlg->Vel_Target*sin(i/1000*M_PI);

	 Pos_Error=Pos_Sta_Arm-Enco_Arm_Pos;
	 Vel_Error=Vel_Sta_Arm-Enco_Arm_Vel;
	str.Format(_T("%8.5f"),Pos_Sta_Arm);
	str1.Format(_T("%8.5f"),Enco_Arm_Pos);
	str2.Format(_T("%8.5f"),Vel_Sta_Arm);
	str3.Format(_T("%8.5f"),Enco_Arm_Vel);
	::SetDlgItemText(A_pDlg->m_hWnd,EDIT_ARM_POSS,str);
	::SetDlgItemText(A_pDlg->m_hWnd,EDIT_ARM_POSR,str1);
	::SetDlgItemText(A_pDlg->m_hWnd,EDIT_ARM_VELS,str2);
	::SetDlgItemText(A_pDlg->m_hWnd,EDIT_ARM_VELR,str3);
	 if (i<=1000)
	 {
	 
	 lineseries1.AddXY((float)i*Sample_Pic_Time*0.001,Pos_Sta_Arm,NULL,0);
	 lineseries2.AddXY((float)i*Sample_Pic_Time*0.001,Enco_Arm_Pos,NULL,0);
	 lineseries3.AddXY((float)i*Sample_Pic_Time*0.001,Vel_Sta_Arm,NULL,0);
	 lineseries4.AddXY((float)i*Sample_Pic_Time*0.001,Enco_Arm_Vel,NULL,0);
	 lineseries5.AddXY((float)i*Sample_Pic_Time*0.001,Pos_Error,NULL,0);
	 lineseries6.AddXY((float)i*Sample_Pic_Time*0.001,Vel_Error,NULL,0);
	 i++;
	}
	 else
	 {
		 timeKillEvent(Ptimer_ID);
		 timeEndPeriod(PAccuracy);
	 }
}
void CArmEncoderDlg:: MediaTimer()
{
	TIMECAPS tc;
	bool com_statu=TRUE;
	UpdateData(TRUE);//更新控件关联的成员变量（value型）
	// 利用函数timeGetDevCaps取出系统分辨率的取值范围, 如果无错则继续; 
	if (timeGetDevCaps(&tc,sizeof(TIMECAPS))==TIMERR_NOERROR)
	{
		PAccuracy = min(max(tc.wPeriodMin,1),tc.wPeriodMax);//分辨率的值不能超出系统的取值范围 
		timeBeginPeriod(PAccuracy);// 调用timeBeginPeriod函数设置定时器的分辨率
		// 设置定时器  
		//com_statu=onCommSet();//打开串口
		Ptimer_ID=timeSetEvent(Sample_Pic_Time,PAccuracy,(LPTIMECALLBACK)OnMediaTimer,DWORD(1),TIME_PERIODIC);
		m_pDlg->OnBnClickedResetButton();//编码器数值归零
	}
}
VOID CArmEncoderDlg::OnClearAllSeries()
{
	for (int j=0;j<m_Tchart.get_SeriesCount();j++)
	{
		((CSeries)m_Tchart.Series(j)).Clear();
	}
	Enco_Arm_Pos=0;

	 Enco_Arm_Vel=0;

	Pos_Sta_Arm=0;
	 Vel_Sta_Arm=0;
	 Vel_Error=0;
	 Pos_Error=0;
	 i=0;
}

void CArmEncoderDlg::OnBnBegain()
{
	// TODO: 在此添加控件通知处理程序代码
	OnClearAllSeries();
	MediaTimer();

}


void CArmEncoderDlg::OnBnStop()
{
	// TODO: 在此添加控件通知处理程序代码
	timeKillEvent(Ptimer_ID);
	timeEndPeriod(PAccuracy);
}


void CArmEncoderDlg::OnBnClickedSave()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str1(" ");
	CString str2(" ");
	CSeries lineseries[7];
	for (int j=0;j<m_Tchart.get_SeriesCount();j++)
	{
		lineseries[j]=(CSeries)m_Tchart.Series(j);
	}
	OnClearAllSeries();
	UpdateData(TRUE);
	CString szFilePath="C:\\ArmEncoder.txt";
	CStdioFile file;
	CString str_i;
	double Time_i=0;
	int len_vector=0;
	len_vector=min(Vel_Arm_Array.size(),Pos_Arm_Array.size(),Time_Array.size(),Vel_Arm_Motor.size());
	file.Open(szFilePath,CFile::modeCreate|CFile::modeWrite);
	file.WriteString("t \t");
	file.WriteString("_t \t\t");
	file.WriteString("Vel \t\t");
	file.WriteString("Pos \r\n");
	for (double i=0;i< len_vector;i++)
	{
		Time_i=i*Sample_Vel_Time*0.001;
		strTime1.Format(_T("%8.5f"),Time_Array[i]);//窗口显示实时采样周期
		str1.Format( _T( "%f"), Vel_Arm_Array[i] );//窗口显示实时速度
		str2.Format( _T( "%f"), Pos_Arm_Array[i] );//窗口显示实时位置
		str_i.Format("%d",i);

		//(i<=300)?Pos_Sta_Arm=Pos_Target*sin(((double)i/300)*M_PI):Pos_Sta_Arm=0;
		Vel_Sta_Arm=Vel_Target*sin(i/len_vector*M_PI);
		if (Time_i<=2.5)
		{
			//Pos_Sta_Arm=Pos_Target*pow(i,2)/10000;
			Pos_Sta_Arm=Pos_Target*pow(Time_i,2)/25;
		}
		if (Time_i>2.5 && Time_i<=5)
		{
			//Pos_Sta_Arm=Pos_Target*(i/100-0.25);
		    Pos_Sta_Arm=Pos_Target*(Time_i/5-0.25);

		}
		if (Time_i>5 && Time_i<=7.5)
		{
			//Pos_Sta_Arm=Pos_Target*(-0.25+i/100-pow(i-100,2)/10000);
			Pos_Sta_Arm=Pos_Target*(-0.25+Time_i/5-pow(Time_i-5,2)/25);
		}
		if (Time_i>7.5 && Time_i<=10)
		{
			Pos_Sta_Arm=Pos_Target;

		}
		if (Time_i>10 && Time_i<=12.5)
		{
			//Pos_Sta_Arm=Pos_Target*(1-pow(i-200,2)/10000);
			Pos_Sta_Arm=Pos_Target*(1-pow(Time_i-10,2)/25);
		}
		if (Time_i>12.5 && Time_i<=15)
		{
			//Pos_Sta_Arm=Pos_Target*(3.25-i/100);
			Pos_Sta_Arm=Pos_Target*(3.25-Time_i/5);
		}
		if (Time_i>15 && Time_i<=17.5)
		{
			//Pos_Sta_Arm=Pos_Target*(3.25-i/100+pow(i-300,2)/10000);
			Pos_Sta_Arm=Pos_Target*(3.25-Time_i/5+pow(Time_i-15,2)/25);
		}
		if (Time_i>17.5)
		{
			Pos_Sta_Arm=0;
		}
		Pos_Error=Pos_Sta_Arm-Pos_Arm_Array[i];
		Vel_Error=Vel_Sta_Arm-Vel_Arm_Array[i];
		lineseries[0].AddXY((float)Time_i,Pos_Sta_Arm,NULL,0);
		lineseries[1].AddXY((float)Time_i,Pos_Arm_Array[i],NULL,0);
		lineseries[2].AddXY((float)Time_i,Vel_Sta_Arm,NULL,0);
		lineseries[3].AddXY((float)Time_i,Vel_Arm_Array[i],NULL,0);
		lineseries[4].AddXY((float)Time_i,Pos_Error,NULL,0);
		lineseries[5].AddXY((float)Time_i,Vel_Error,NULL,0);
		lineseries[6].AddXY((float)Time_i,Vel_Arm_Motor[i],NULL,0);
		file.WriteString(str_i +"\t");
		file.WriteString(strTime1+"ms\t");
		file.WriteString(str1+"\t");
		file.WriteString(str2);
		file.WriteString("\r\n");
	}
	file.Close();
}


void CArmEncoderDlg::OnBnClickedElbow()
{
	// TODO: 在此添加控件通知处理程序代码
	Pos_Target=75;
	Vel_Target=0;
	UpdateData(FALSE);
}


void CArmEncoderDlg::OnBnClickedShoul()
{
	// TODO: 在此添加控件通知处理程序代码
	Pos_Target=65;
	Vel_Target=0;
	UpdateData(FALSE);
}
