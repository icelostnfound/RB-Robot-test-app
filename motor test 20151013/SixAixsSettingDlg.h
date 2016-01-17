#pragma once
#include "FTWrapper.h"
#include "TabOutputOptions.h"
#include "TabToolTransforms.h"
// CSixAixsSettingDlg 对话框

class CSixAixsSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSixAixsSettingDlg)

public:
	CSixAixsSettingDlg(FTWrapper* wrapper, CWnd* pParent = NULL);
// 对话框数据
	enum { IDD = IDD_SIXAIXS_SETTINGS };
	CStatic	mPalette;
	CTabCtrl	mSettingsTab;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	/* a pointer to the FTWrapper that hides much of the complexity of the _FTSensor class */
	FTWrapper* wrapper;
	/* this variable records the index for the list of torque units */
	int torque;
	/* this variable records the index for the list of force units */
	int force;
	/* this is an object representing one of the tab frames - Tool Transforms */
	CTabToolTransforms mToolTransforms;
	/* this is an object representing one of the tab frames - Output Options */
	CTabOutputOptions mOutputOptions;
	BOOL OnInitDialog();
public:
	afx_msg void OnTcnSelchangeSettingsTab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
