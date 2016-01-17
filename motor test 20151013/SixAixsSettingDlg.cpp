// SixAixsSettingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "motor test 20151013.h"
#include "SixAixsSettingDlg.h"
#include "afxdialogex.h"


// CSixAixsSettingDlg 对话框

IMPLEMENT_DYNAMIC(CSixAixsSettingDlg, CDialogEx)

CSixAixsSettingDlg::CSixAixsSettingDlg(FTWrapper* wrapper,CWnd* pParent /*=NULL*/)
	: CDialogEx(CSixAixsSettingDlg::IDD, pParent)
{
	this->wrapper = wrapper;
	force = 0;
	torque = 0;
}

void CSixAixsSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, PALETTE, mPalette);
	DDX_Control(pDX, IDC_SETTINGS_TAB, mSettingsTab);
}


BEGIN_MESSAGE_MAP(CSixAixsSettingDlg, CDialogEx)
	ON_NOTIFY(TCN_SELCHANGE, IDC_SETTINGS_TAB, &CSixAixsSettingDlg::OnTcnSelchangeSettingsTab)
	ON_BN_CLICKED(IDOK, &CSixAixsSettingDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSixAixsSettingDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CSixAixsSettingDlg 消息处理程序
BOOL CSixAixsSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	mToolTransforms.SetFTWrapper(wrapper);

	TCITEM tcOutputOptions;
	tcOutputOptions.mask = TCIF_TEXT;
	tcOutputOptions.pszText = "Output Options";
	mSettingsTab.InsertItem(0, &tcOutputOptions);
	mOutputOptions.Create(TAB_OUTPUT_OPTIONS, &mPalette);
	mOutputOptions.EnableWindow(true);

	TCITEM tcToolTransforms;
	tcToolTransforms.mask = TCIF_TEXT;
	tcToolTransforms.pszText = "Tool Transforms";
	mSettingsTab.InsertItem(1, &tcToolTransforms);
	mToolTransforms.Create(TAB_TOOL_TRANSFORMS, &mPalette);
	mToolTransforms.EnableWindow(true);

	mOutputOptions.ShowWindow(true);

	_Calibration calibration = wrapper->GetActiveCalibration();
	for (int i=0; i<mOutputOptions.mComboForce.GetCount(); i++) {
		CString forceUnits;
		mOutputOptions.mComboForce.GetLBText(i, forceUnits);
		if (calibration.GetForceUnits() == forceUnits) {
			force = i;
			break;
		}
	}
	for (int j=0; j<mOutputOptions.mComboTorque.GetCount(); j++) {
		CString torqueUnits;
		mOutputOptions.mComboTorque.GetLBText(j, torqueUnits);
		if (calibration.GetTorqueUnits() == torqueUnits) {
			torque = j;
			break;
		}
	}

	mOutputOptions.mCheckboxTempComp.SetCheck(wrapper->GetActiveCalibration().GetTempCompEnabled());
	mOutputOptions.mComboForce.SetCurSel(force);
	mOutputOptions.mComboTorque.SetCurSel(torque);

	return TRUE;
}



void CSixAixsSettingDlg::OnTcnSelchangeSettingsTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	switch(mSettingsTab.GetCurSel()) {
	case 0:
		mOutputOptions.ShowWindow(true);
		mToolTransforms.ShowWindow(false);
		mOutputOptions.SetForegroundWindow();
		break;
	case 1:
		mOutputOptions.ShowWindow(false);
		mToolTransforms.ShowWindow(true);
		mToolTransforms.SetForegroundWindow();
		break;
	default:
		;
	};
	*pResult = 0;
}


void CSixAixsSettingDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	_Calibration calibration = wrapper->GetActiveCalibration();
	calibration.SetTempCompEnabled(mOutputOptions.mCheckboxTempComp.GetCheck());
	force = mOutputOptions.mComboForce.GetCurSel();
	if (force != LB_ERR) {
		CString forceUnits;
		mOutputOptions.mComboForce.GetLBText(force, forceUnits);
		calibration.SetForceUnits(forceUnits);
	}
	torque = mOutputOptions.mComboTorque.GetCurSel();
	if (torque != LB_ERR) {
		CString torqueUnits;
		mOutputOptions.mComboTorque.GetLBText(torque, torqueUnits);
		calibration.SetTorqueUnits(torqueUnits);
	}
	CDialogEx::OnOK();
}


void CSixAixsSettingDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}
