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

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()
public:
	CM1f_tchart m_M1FChart;
	CM1f_tchart m_M2FChart;
	double Time;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedReadButton();
	afx_msg void OnBnClickedStoprButton();
	afx_msg void OnBnClickedTxtButton();
};
