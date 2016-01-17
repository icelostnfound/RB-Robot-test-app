#pragma once
#include "m1f_tchart.h"


// CForceChartDlg 对话框

class CForceChartDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CForceChartDlg)

public:
	CForceChartDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CForceChartDlg();

// 对话框数据
	enum { IDD = IDD_ForceChart_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
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
