// TabStartup.cpp : 实现文件
//

#include "stdafx.h"
#include "motor test 20151013.h"
#include "TabStartup.h"
#include "afxdialogex.h"


// CTabStartup 对话框

IMPLEMENT_DYNAMIC(CTabStartup, CDialogEx)

CTabStartup::CTabStartup(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTabStartup::IDD, pParent)
{

}

CTabStartup::~CTabStartup()
{
}

void CTabStartup::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, EDIT_CONFIG_PATH, mEditConfigPath);
	DDX_Control(pDX, CHECK_LOAD_CONFIG, mCheckLoadConfig);
	DDX_Control(pDX, BUTTON_LOAD_CONFIG, mButtonLoadConfig);
}


BEGIN_MESSAGE_MAP(CTabStartup, CDialogEx)
	ON_BN_CLICKED(BUTTON_LOAD_CONFIG, &CTabStartup::OnBnClickedLoadConfig)
	ON_BN_CLICKED(CHECK_LOAD_CONFIG, &CTabStartup::OnCheckLoadConfig)
END_MESSAGE_MAP()


// CTabStartup 消息处理程序

CString ChooseConfigFile()
{
	CString filename;
	CString filter = "ATI DAQ F/T Configuration Files (*.cfg)|*.cfg||";
	CFileDialog fileMenu(true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter, NULL);
	fileMenu.DoModal();
	filename = fileMenu.GetPathName();
	//	if (fileMenu.GetFileExt() != "cfg") {
	//		// bad cal file
	//	}
	return filename;
}
void CTabStartup::OnBnClickedLoadConfig()
{
	// TODO: 在此添加控件通知处理程序代码
	CString filename = ChooseConfigFile();
	if (filename != "") {
		mEditConfigPath.SetWindowText(filename);
	}
}



void CTabStartup::OnCheckLoadConfig()
{
	// TODO: 在此添加控件通知处理程序代码
	bool enable = mCheckLoadConfig.GetCheck() ? true : false;
	mButtonLoadConfig.EnableWindow(enable);
	mEditConfigPath.EnableWindow(enable);
}
BOOL CTabStartup::OnInitDialog()
{
	CDialog::OnInitDialog();
	OnCheckLoadConfig();
	return TRUE;
}
