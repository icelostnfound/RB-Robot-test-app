// ProgramOpDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "motor test 20151013.h"
#include "ProgramOpDlg.h"
#include "afxdialogex.h"


// CProgramOpDlg 对话框

IMPLEMENT_DYNAMIC(CProgramOpDlg, CDialogEx)

CProgramOpDlg::CProgramOpDlg(FTWrapper* wrapper,CWnd* pParent /*=NULL*/)
	: CDialogEx(CProgramOpDlg::IDD, pParent)
{
	this->wrapper = wrapper;
	beep = false;
	if (wrapper->GetStatus() & FTWrapper::CALFILE_LOADED) {
		thermistor = wrapper->GetActiveCalibration().GetTempCompAvailable() ? true : false;
	}
	else {
		thermistor = true;
	}
	timer = 100;
	DAQSys* daq = wrapper->getDAQSys();
	device = daq->getDeviceName();
	firstChannel = daq->getFirstChannel();
	scanRate = (int)(daq->getScanRate());
	loadConfig = false;
	configFile = "";
}
void CProgramOpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRAM_OPTIONS_TAB, mProgramOptionsTab);
	DDX_Control(pDX, FOLDER, mFolder);
}


BEGIN_MESSAGE_MAP(CProgramOpDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CProgramOpDlg::OnBnClickedOk)
	ON_NOTIFY(TCN_SELCHANGE, IDC_PROGRAM_OPTIONS_TAB, &CProgramOpDlg::OnSelchangeProgramOptionsTab)
	ON_BN_CLICKED(IDCANCEL, &CProgramOpDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CProgramOpDlg 消息处理程序
//字符串型转成整型函数
int convertStringToInt(CString str, int min, int max, int old)
{
	for (int i=0; i<str.GetLength(); i++) {
		if (str.GetAt(i)>'9' || str.GetAt(i)<'0') {
			if (i != 0 || str.GetAt(0) == '-') {
				return old;
			}
		}
	}
	int t = atoi(str);
	if (t<min) {
		return min;
	}
	if (t>max) {
		return max;
	}
	return t;
}


void CProgramOpDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	/*we use the ? : operators here because we'll get compile 
		warnings if we just try to set bools to int values */
	beep = mTabGeneral.mCheckBeep.GetCheck() ? true : false;
	thermistor = mTabGeneral.mCheckThermistor.GetCheck() ? true : false;
	autoLoad = mTabGeneral.m_checkAutoload.GetCheck() ? true : false;

	CString temp;
	mTabGeneral.mEditTimeInterval.GetWindowText(temp);
	timer = convertStringToInt(temp, 50, 1000, timer);

	mTabNIDAQ.mEditDeviceNumber.GetWindowText(device);
	
	mTabNIDAQ.mEditFirstChannelIndex.GetWindowText(temp);
	firstChannel = convertStringToInt(temp, 0, 1024, firstChannel);
	mTabNIDAQ.mEditScanRate.GetWindowText(temp);
	scanRate = convertStringToInt(temp, 100, 10000, scanRate);

	loadConfig = mTabStartup.mCheckLoadConfig.GetCheck() ? true : false;
	mTabStartup.mEditConfigPath.GetWindowText(configFile);

	try {
		wrapper->getDAQSys()->inited( device, (short)firstChannel, (double)scanRate );
	}
	catch (DAQException* error) {
		// TODO: error handling code
		// change ignored
	}
	CDialogEx::OnOK();
}

BOOL CProgramOpDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	DAQSys* daq = wrapper->getDAQSys();
	device = daq->getDeviceName();
	firstChannel = daq->getFirstChannel();
	scanRate = (int)(daq->getScanRate());
	loadConfig = false;
	configFile = "";
	TCITEM tcGeneral;
	tcGeneral.mask = TCIF_TEXT;
	tcGeneral.pszText = "General";
	mProgramOptionsTab.InsertItem(0, &tcGeneral);
	mTabGeneral.Create(TAB_GENERAL, &mFolder);
	mTabGeneral.EnableWindow(true);

	TCITEM tcNI_DAQ;
	tcNI_DAQ.mask = TCIF_TEXT;
	tcNI_DAQ.pszText = "NI-DAQ";
	mProgramOptionsTab.InsertItem(1, &tcNI_DAQ);
	mTabNIDAQ.Create(TAB_NIDAQ, &mFolder);
	mTabNIDAQ.EnableWindow(true);

#if START_UP
	TCITEM tcStartup;
	tcStartup.mask = TCIF_TEXT;
	tcStartup.pszText = "Startup";
	mProgramOptionsTab.InsertItem(2, &tcStartup);
#endif
	mTabStartup.Create(TAB_STARTUP, &mFolder);
	mTabStartup.EnableWindow(true);

	mTabGeneral.ShowWindow(true);

	mTabGeneral.mCheckBeep.SetCheck(beep);
	mTabGeneral.mCheckThermistor.SetCheck(thermistor);
	mTabGeneral.m_checkAutoload.SetCheck( autoLoad );

	CString temp;
	temp.Format("%d", timer);
	mTabGeneral.mEditTimeInterval.SetWindowText(temp);


	mTabNIDAQ.mEditDeviceNumber.SetWindowText(device);
	temp.Format("%d", firstChannel);
	mTabNIDAQ.mEditFirstChannelIndex.SetWindowText(temp);
	temp.Format("%d", scanRate);
	mTabNIDAQ.mEditScanRate.SetWindowText(temp);

	mTabStartup.mCheckLoadConfig.SetCheck(loadConfig);
	mTabStartup.OnCheckLoadConfig();
	mTabStartup.mEditConfigPath.SetWindowText(configFile);

	return TRUE;  // return TRUE  unless you set the focus to a control
}
void CProgramOpDlg::OnSelchangeProgramOptionsTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	switch(mProgramOptionsTab.GetCurSel()) {
	case 0:
		mTabGeneral.ShowWindow(true);
		mTabNIDAQ.ShowWindow(false);
		mTabStartup.ShowWindow(false);
		mTabGeneral.SetForegroundWindow();
		break;
	case 1:
		mTabGeneral.ShowWindow(false);
		mTabNIDAQ.ShowWindow(true);
		mTabStartup.ShowWindow(false);
		mTabNIDAQ.SetForegroundWindow();
		break;
	case 2:
		mTabGeneral.ShowWindow(false);
		mTabNIDAQ.ShowWindow(false);
		mTabStartup.ShowWindow(true);
		mTabStartup.SetForegroundWindow();
		break;
	default:
		;
	};
	*pResult = 0;
}


void CProgramOpDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}
