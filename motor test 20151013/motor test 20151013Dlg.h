
// motor test 20151013Dlg.h : 头文件
//

#pragma once
//Adlink head file////////
#include "APS168.h"
#include "ErrorCodeDef.h"
#include "APS_define.h"
#include "type_def.h"
//////////////////////////
#include "Motion.h"
#include"ForceChartDlg.h"
#include "LoginDlg.h"
#include "SixAxisSensorDlg.h"
#include "CTChart0.h"
#include "m1f_tchart.h"
#include "mscomm1.h"
// Cmotortest20151013Dlg 对话框
class Cmotortest20151013Dlg : public CDialogEx
{
// 构造
private:
	 CForceChartDlg m_ForceChartDlg;
	 CLoginDlg myDlg;
	CSixAxisSensorDlg m_SixAxisSensorVEL;
public:
	Cmotortest20151013Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MOTORTEST20151013_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
// 实现
protected:
	HICON m_hIcon;
	HANDLE hThread1;
	DWORD ThreadID1;
	HANDLE hThread2;
	DWORD ThreadID2;
	HANDLE hThread3;
	DWORD ThreadID3;
	HANDLE hThread4;
	DWORD ThreadID4;
	HANDLE hThread5;
	DWORD ThreadID5;
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	
	afx_msg BOOL Login();
	DECLARE_MESSAGE_MAP()
public:
	#define YES (1)
    #define NO  (0)
    #define ON  (1)
    #define OFF (0)
	//////////////////////////////////////////////////////////////////
	I32 v_card_name;
    I32 v_board_id;         
    I32 v_channel;           
    I32 v_total_axis;        
    I32 v_is_card_initialed;
	I32 Is_thread_creat;
	double m_Position;
	double m_Vs;
	double m_Vm;
	double m_Ve;
	double m_Acc;
	double m_Dec;
	double m_SFac;
	double m_Position1;
	double m_Vs1;
	double m_Vm1;
	double m_Ve1;
	double m_Acc1;
	double m_Dec1;
	double m_SFac1;
	double FT[50];
	
	int m_FTT;
	int ptp[2];
	BOOL mt_flag;
	BOOL relay_flag;
    //////////////////////////////////////////////////////////////////
	void Function_Result( I32 Ret );
	I32 Is_CARD_INITIAL(void);
	I32 Is_axis_err(I32 Axis_ID);
	double average(double sum[4]);
	int get_bit(int num,int index);
	void MoveToPosition(F64 Position1,F64 Position2 );
	void LeftshouderMove();
	void LeftshouderBack();
	void RightearMove();
	void RightearBack();
	void RelaySwitchOpen();
	void RelaySwitchClose();
	VOID OnWarning();
	VOID Reci_Vel_Arm();
	BOOL DAQ_EMG(CString changeDeviceName, short changeFirstChannel);
	BOOL DAQ_EAF(CString changeDeviceName, short changeFirstChannel,short changeLastChannel);
	BOOL onCommSet();
	//////////////////////////////////////////////////////////////////
	afx_msg void OnBnClickedInitialButton();
	afx_msg void OnBnClickedOnButton();
	afx_msg void OnBnClickedP2pButton4();
	afx_msg void OnBnClickedResetButton();
	afx_msg void OnBnClickedLeftshouderButton();
	afx_msg void OnBnClickedRightearButton();
	afx_msg void OnBnClickedLearnbButton();
	afx_msg void OnBnClickedLearneButton2();
	afx_msg void OnBnClickedMButton();
	afx_msg void OnBnClickedRelayopen();
	afx_msg void OnBnClickedRelayclose();
	afx_msg void OnToolsSix();
	afx_msg void OnToolsEncoder();
	afx_msg void OnBnClickedVelMot1();
	afx_msg void OnBnClickedVelMot2();
	afx_msg void OnBnClickedVelStop();
	afx_msg void OnStartWithSensor();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	double m_FTTEDIT;
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg void OndHomeButton();
	CM1f_tchart m_FTVEL;
	afx_msg void OnClearAllSeries();
	afx_msg void OnPidControl();
	afx_msg void OnEnChangefv0();
	CMscomm1 m_comm;
	DECLARE_EVENTSINK_MAP()
	void OnCommMscomm1();
	
	afx_msg void OnRadioPosition();
	afx_msg void OnRadioVSingle();
	afx_msg void OnRadioSix();
	afx_msg void OnRadioPid();
	afx_msg void OnRadioEmg();
	afx_msg void OnButtonQuit();
	afx_msg void OnPidSaveData();
};
