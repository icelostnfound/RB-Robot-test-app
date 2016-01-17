#pragma once
#include "m1f_tchart.h"


// CForceChartDlg �Ի���

class CForceChartDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CForceChartDlg)

public:
	CForceChartDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CForceChartDlg();

// �Ի�������
	enum { IDD = IDD_ForceChart_DIALOG };
	CEdit m_ct;

protected:
	HANDLE hThread;
	DWORD ThreadID;
	HANDLE hThread0;
	DWORD ThreadID0;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()
public:
	CM1f_tchart m_M1FChart;
	CM1f_tchart m_M2FChart;
	//static DWORD ThreadFun(LPVOID pParam);
	void receive_data();
	void Cramps();
	BOOL DAQ(CString changeDeviceName, short changeFirstChannel);
	afx_msg void OnBnClickedReadButton();
	afx_msg void OnBnClickedStoprButton();
	afx_msg void OnBnClickedTxtButton();
	
	
	
};
