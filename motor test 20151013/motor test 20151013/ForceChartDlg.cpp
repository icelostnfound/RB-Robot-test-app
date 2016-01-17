// ForceChartDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "motor test 20151013.h"
#include "ForceChartDlg.h"
#include "afxdialogex.h"
//Teechart head file//////
#include"CAxes.h"
#include"CAxis.h"
#include"CSeries.h"
#include"CTChart0.h"
#include"CScroll.h"
//////////////////////////
#include"NIDAQmx.h"
#include <fstream>
// CForceChartDlg 对话框

IMPLEMENT_DYNAMIC(CForceChartDlg, CDialogEx)

CForceChartDlg::CForceChartDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CForceChartDlg::IDD, pParent)
	,Time(0)
{

}

CForceChartDlg::~CForceChartDlg()
{
}

void CForceChartDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_M1F_TCHART, m_M1FChart);
	DDX_Control(pDX, IDC_M2F_TCHART, m_M2FChart);
}


BEGIN_MESSAGE_MAP(CForceChartDlg, CDialogEx)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_Read_BUTTON, &CForceChartDlg::OnBnClickedReadButton)
	ON_BN_CLICKED(IDC_StopR_BUTTON, &CForceChartDlg::OnBnClickedStoprButton)
	ON_BN_CLICKED(IDC_TXT_BUTTON, &CForceChartDlg::OnBnClickedTxtButton)
END_MESSAGE_MAP()


// CForceChartDlg 消息处理程序


void CForceChartDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//画电机1拉力/时间图
	int32       error = 0;
	TaskHandle  taskHandle = 0;
	int32       read;
	double      G0[100]={0};
	double      G1[100]={0};
	double      G2[100]={0};
	double      G3[100]={0};
	//采集G0口数据
	/*********************************************/
	// DAQmx Configure Code
	/*********************************************/
	DAQmxCreateTask("AAencoderTask", &taskHandle);
	DAQmxCreateAIVoltageChan(taskHandle, "Dev3/ai0", "AAencoderChannel", DAQmx_Val_RSE, 0, 5, DAQmx_Val_Volts, NULL);
	UpdateData(TRUE);
	DAQmxCfgSampClkTiming(taskHandle, "OnboardClock", 100, DAQmx_Val_Rising, DAQmx_Val_ContSamps, 20);
	UpdateData(TRUE);
	/*********************************************/
	// DAQmx Start Code
	/*********************************************/
	DAQmxStartTask(taskHandle);

	/*********************************************/
	// DAQmx Read Code
	/*********************************************/
	DAQmxReadAnalogF64(taskHandle, 10, 0.2, DAQmx_Val_GroupByChannel, G0, 10, &read, NULL);
	DAQmxStopTask(taskHandle);
	DAQmxClearTask(taskHandle);
	//采集G1口数据
	DAQmxCreateTask("SAencoderTask", &taskHandle);
	DAQmxCreateAIVoltageChan(taskHandle, "Dev3/ai1", "SAencoderChannel", DAQmx_Val_RSE, 0, 5, DAQmx_Val_Volts, NULL);
	UpdateData(TRUE);
	DAQmxCfgSampClkTiming(taskHandle, "OnboardClock", 100, DAQmx_Val_Rising, DAQmx_Val_ContSamps, 20);
	UpdateData(TRUE);
	DAQmxStartTask(taskHandle);
	DAQmxReadAnalogF64(taskHandle, 10, 0.2, DAQmx_Val_GroupByChannel, G1, 10, &read, NULL);
	DAQmxStopTask(taskHandle);
	DAQmxClearTask(taskHandle);
	//采集G2口数据
	DAQmxCreateTask("M1FencoderTask", &taskHandle);
	DAQmxCreateAIVoltageChan(taskHandle, "Dev3/ai2", "M1FencoderChannel", DAQmx_Val_RSE, 0, 5, DAQmx_Val_Volts, NULL);
	UpdateData(TRUE);
	DAQmxCfgSampClkTiming(taskHandle, "OnboardClock", 100, DAQmx_Val_Rising, DAQmx_Val_ContSamps, 20);
	UpdateData(TRUE);
	DAQmxStartTask(taskHandle);
	DAQmxReadAnalogF64(taskHandle, 10, 0.2, DAQmx_Val_GroupByChannel, G2, 10, &read, NULL);
	DAQmxStopTask(taskHandle);
	DAQmxClearTask(taskHandle);
	//采集G3口数据
	DAQmxCreateTask("M2FencoderTask", &taskHandle);
	DAQmxCreateAIVoltageChan(taskHandle, "Dev3/ai3", "M2FencoderChannel", DAQmx_Val_RSE, 0, 5, DAQmx_Val_Volts, NULL);
	UpdateData(TRUE);
	DAQmxCfgSampClkTiming(taskHandle, "OnboardClock", 100, DAQmx_Val_Rising, DAQmx_Val_ContSamps, 20);
	UpdateData(TRUE);
	DAQmxStartTask(taskHandle);
	DAQmxReadAnalogF64(taskHandle, 10, 0.2, DAQmx_Val_GroupByChannel, G3, 10, &read, NULL);
	DAQmxStopTask(taskHandle);
	DAQmxClearTask(taskHandle);	
	//调试动作1
	/*if(G1[5]>=2.9)
	{
	     I32 Axis_ID = 0;
         for (Axis_ID = 0; Axis_ID < 2; Axis_ID++)
		 APS_set_servo_on(Axis_ID, OFF);
	}*/
	CString str1;
	CString str2;
	CString str3;
	CString str4;
	str1.Format(_T("%8.3f"), G0[5]);
	str2.Format(_T("%8.3f"),G1[5]);
	str3.Format(_T("%8.3f"), G2[5]*196);
	str4.Format(_T("%8.3f"),G3[5]*196);
	SetDlgItemText(IDC_AA_EDIT, str1);
	SetDlgItemText(IDC_SA_EDIT, str2);
	SetDlgItemText(IDC_M1F_EDIT, str3);
	SetDlgItemText(IDC_M2F_EDIT, str4);
	Time+=0.1;
	CSeries lineseries1;
	lineseries1=(CSeries)m_M1FChart.Series(0);
	lineseries1.AddXY(Time,G2[5]*196,NULL,0);
	//((CScroll)m_M1FChart.get_Axis())
	//画电机2拉力/时间图
	CSeries lineseries2;
	lineseries2=(CSeries)m_M2FChart.Series(0);
	lineseries2.AddXY(Time,G3[5]*196,NULL,0);
	CDialogEx::OnTimer(nIDEvent);
}


void CForceChartDlg::OnBnClickedReadButton()
{
	SetTimer(0,10,NULL );
	// TODO: 在此添加控件通知处理程序代码
}


void CForceChartDlg::OnBnClickedStoprButton()
{
	// TODO: 在此添加控件通知处理程序代码
	KillTimer(0);
}


void CForceChartDlg::OnBnClickedTxtButton()
{
	// TODO: 在此添加控件通知处理程序代码
	//unsigned int c = 0, l = 0;

	////	CFile file("data.txt",CFile::modeCreate|CFile::modeWrite);
	//ofstream ofs;  //创建保存数据文件
	//ofs.open("D:\rehibition robot data\\data.txt", ofstream::app | ofstream::out);
	//
	//for (c = 0; c <m_col; c++)
	//	{
	//		for (l = 0; l < 10; l++)
	//		{
	//			if (c == 0)
	//			{
	//				//在数据前面加上数组的头部，比如 position = [
	//				switch (l)
	//				{
	//				case 0:
	//				{
	//					ofs << "DP_Force" << '\t' ;
	//					break;
	//				}
	//				case 1:
	//				{
	//					ofs << "IP_Force " << '\t' ;
	//					break;
	//				}
	//				case 2:
	//				{
	//					ofs << "PP_Force" << '\t' ;
	//					break;
	//				}
	//				case 3:
	//				{
	//					ofs << "Thumb_Force " << '\t' ;
	//					break;
	//				}
	//				case 4:
	//				{
	//					ofs << "Position " << '\t' ;
	//					break;
	//				}
	//				case 5:
	//				{
	//					ofs << "Velocity " << '\t' ;
	//					break;
	//				}
	//				case 6:
	//				{
	//					ofs << "MotorTorque "<< '\t' ;
	//					break;
	//				}
	//				case 7:
	//				{
	//					ofs << "MCP_Angle " << '\t' ;
	//					break;
	//				}
	//				case 8:
	//				{
	//					ofs << "PIP_Angle "<< '\t' ;
	//					break;
	//				}
	//				case 9:
	//				{
	//					ofs << "DIP_Angle "<< '\t' <<  '\r' << '\n';
	//					break;
	//				}
	//			
	//				}
	//			}
	//			else
	//				if(l < 9)
	//					ofs << dataout[l][c] << '\t'<<  '\t';
	//			else
	//					ofs << dataout[l][c] << '\r' << '\n';
	//			}
	//	}
	//	ofs.close();
}
