#pragma once
#include "motor test 20151013Dlg.h"
#include "m1f_tchart.h"

// CArmEncoderDlg 对话框

class CArmEncoderDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CArmEncoderDlg)

public:
	CArmEncoderDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CArmEncoderDlg();

// 对话框数据
	enum { IDD = DIALOG_ARM_ENCODER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	VOID OnClearAllSeries();
	VOID MediaTimer();
	CM1f_tchart m_Tchart;
	afx_msg void OnBnBegain();
	afx_msg void OnBnStop();
	double Pos_Target;
	double Vel_Target;
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedElbow();
	afx_msg void OnBnClickedShoul();
};
