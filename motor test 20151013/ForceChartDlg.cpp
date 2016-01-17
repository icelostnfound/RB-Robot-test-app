// ForceChartDlg.cpp : ʵ���ļ�
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
extern  BOOL m_flag;
extern  BOOL m_flag0;
#include"NIDAQmx.h"
#include <fstream>
// CForceChartDlg �Ի���

IMPLEMENT_DYNAMIC(CForceChartDlg, CDialogEx)
VOID ThreadFun(LPVOID pParam);
VOID ThreadFun0(LPVOID pParam);
BOOL m_flag0 = TRUE;
BOOL m_flag = TRUE;
double G0[100]={0};
double  RD[5]={0};
double Time=0;
CForceChartDlg::CForceChartDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CForceChartDlg::IDD, pParent)
	
	
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
	ON_BN_CLICKED(IDC_Read_BUTTON, &CForceChartDlg::OnBnClickedReadButton)
	ON_BN_CLICKED(IDC_StopR_BUTTON, &CForceChartDlg::OnBnClickedStoprButton)
	ON_BN_CLICKED(IDC_TXT_BUTTON, &CForceChartDlg::OnBnClickedTxtButton)
END_MESSAGE_MAP()


// CForceChartDlg ��Ϣ�������
void CForceChartDlg::receive_data()
{
	 
	for (int i=0;i<4;i++)
	{
		DAQ("Dev1",i);
		RD[i]=G0[5];
	}	
		
}
//�߳�1��������
VOID ThreadFun(LPVOID pParam)
{
	 //TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//�����1����/ʱ��ͼ
	CString str1;
	CString str2;
	CString str3;
	CString str4;
	CString str5;
	CString strTime;
	CSeries lineseries1;
	CSeries lineseries2;
	clock_t start,finish;//����洢��ʼ�ڽ�����ʱ��ı���
	m_flag = TRUE;
	CTime time;
    CForceChartDlg *dlg=(CForceChartDlg*)pParam;
	while (m_flag)
	{
	start=clock();//�洢��ʼʱ��
	 dlg->receive_data();
	 time = CTime::GetCurrentTime();
	strTime=time.Format("%H:%M:%S");//��ʾ��ǰʱ��
	str1.Format(_T("%8.3f"),RD[0]);
	str2.Format(_T("%8.3f"),RD[1]);
	str3.Format(_T("%8.3f"),RD[2]*196);
	str4.Format(_T("%8.3f"),RD[3]*196);
	::SetDlgItemText(dlg->m_hWnd,IDC_Time_EDIT,strTime);
	::SetDlgItemText(dlg->m_hWnd,IDC_AA_EDIT, str1);
	::SetDlgItemText(dlg->m_hWnd,IDC_SA_EDIT, str2);
	::SetDlgItemText(dlg->m_hWnd,IDC_M1F_EDIT, str3);
	::SetDlgItemText(dlg->m_hWnd,IDC_M2F_EDIT, str4);
	//�����2����/ʱ��ͼ
	lineseries1=(CSeries)dlg->m_M1FChart.Series(0);
	lineseries1.AddXY(Time,RD[2]*196,NULL,0);
	//((CScroll)m_M1FChart.get_Axis())
	lineseries2=(CSeries)dlg->m_M2FChart.Series(0);
	lineseries2.AddXY(Time,RD[3]*196,NULL,0);
	Sleep(100);
    Time+=0.11;
    finish=clock();//�洢����ʱ��
	//���ѭ������ʱ��
	if(Time==0.11)
	   { 
			str4.Format(_T("%ld"),finish-start);
			::SetDlgItemText(dlg->m_hWnd,IDC_LOOP_EDIT,str4);

		}
	}
	
	ExitThread(1);
}

VOID ThreadFun0(LPVOID pParam)
{
	 CForceChartDlg *dlg=(CForceChartDlg*)pParam;
	 m_flag0 = TRUE;
	 while (m_flag0)
	 {
	  dlg->receive_data();
	  Sleep(100);
	 }
	 ExitThread(1);
}
void CForceChartDlg::OnBnClickedReadButton()
{
	hThread=CreateThread(NULL,
		0,
		(LPTHREAD_START_ROUTINE)ThreadFun,
		this,
		0,
&ThreadID);
	GetDlgItem(IDC_StopR_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_Read_BUTTON)->EnableWindow(FALSE);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
void CForceChartDlg::Cramps()
{
	hThread0=CreateThread(NULL,
		0,
		(LPTHREAD_START_ROUTINE)ThreadFun0,
		this,
		0,
		&ThreadID0);
}

void CForceChartDlg::OnBnClickedStoprButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_flag = false;
	GetDlgItem(IDC_StopR_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_Read_BUTTON)->EnableWindow(TRUE);
}

BOOL CForceChartDlg::DAQ(CString changeDeviceName, short changeFirstChannel)
{
	TaskHandle  taskHandle = 0;
	int32       read;
	CString cstrChannelList;
	cstrChannelList.Format( _T("/ai%d"), changeFirstChannel );
	DAQmxCreateTask("AAencoderTask", &taskHandle);
	DAQmxCreateAIVoltageChan(taskHandle, changeDeviceName+cstrChannelList, "AAencoderChannel", DAQmx_Val_RSE, 0, 5, DAQmx_Val_Volts, NULL);
	//UpdateData(TRUE);
	DAQmxCfgSampClkTiming(taskHandle, "OnboardClock", 100, DAQmx_Val_Rising, DAQmx_Val_ContSamps, 20);
	//UpdateData(TRUE);
	DAQmxStartTask(taskHandle);
	DAQmxReadAnalogF64(taskHandle, 10, 0.2, DAQmx_Val_GroupByChannel, G0, 10, &read, NULL);
	DAQmxStopTask(taskHandle);
	DAQmxClearTask(taskHandle);
	return TRUE;
}
void CForceChartDlg::OnBnClickedTxtButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//unsigned int c = 0, l = 0;

	////	CFile file("data.txt",CFile::modeCreate|CFile::modeWrite);
	//ofstream ofs;  //�������������ļ�
	//ofs.open("D:\rehibition robot data\\data.txt", ofstream::app | ofstream::out);
	//
	//for (c = 0; c <m_col; c++)
	//	{
	//		for (l = 0; l < 10; l++)
	//		{
	//			if (c == 0)
	//			{
	//				//������ǰ����������ͷ�������� position = [
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
