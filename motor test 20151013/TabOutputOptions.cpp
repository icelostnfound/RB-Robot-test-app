// TabOutputOptions.cpp : 实现文件
//

#include "stdafx.h"
#include "motor test 20151013.h"
#include "TabOutputOptions.h"
#include "afxdialogex.h"


// CTabOutputOptions 对话框

IMPLEMENT_DYNAMIC(CTabOutputOptions, CDialogEx)

CTabOutputOptions::CTabOutputOptions(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTabOutputOptions::IDD, pParent)
{

}

CTabOutputOptions::~CTabOutputOptions()
{
}

void CTabOutputOptions::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, COMBOBOX_TORQUE, mComboTorque);
	DDX_Control(pDX, COMBOBOX_FORCE, mComboForce);
	DDX_Control(pDX, CHECK_TEMP_COMP, mCheckboxTempComp);
}


BEGIN_MESSAGE_MAP(CTabOutputOptions, CDialogEx)
END_MESSAGE_MAP()


// CTabOutputOptions 消息处理程序
//combox box单位选择函数，分为Lb,"N","KN,"g","kg","in-lb","ft-lb", and so on
BOOL CTabOutputOptions::OnInitDialog()
{
	CDialog::OnInitDialog();
	CString t;

	COMBOBOXEXITEM item;
	item.mask = CBEIF_TEXT;
	item.iItem = 0;
	item.pszText = "lb";
	mComboForce.InsertItem(&item);
	//	mComboForce.InsertString(0,"lb");
	item.iItem = 1;
	item.pszText = "N";
	mComboForce.InsertItem(&item);
	//	mComboForce.InsertString(1, "N");
	item.iItem = 2;
	item.pszText = "kN";
	mComboForce.InsertItem(&item);
	//	mComboForce.InsertString(2, "kN");
	item.iItem = 3;
	item.pszText = "g";
	mComboForce.InsertItem(&item);
	//	mComboForce.InsertString(3, "g");
	item.iItem = 4;
	item.pszText = "kg";
	mComboForce.InsertItem(&item);
	//	mComboForce.InsertString(4, "kg");
	//	mComboForce.ShowDropDown(true);

	item.iItem = 0;
	item.pszText = "in-lb";
	mComboTorque.InsertItem(&item);
	//	mComboTorque.InsertString(0, "in-lb");
	item.iItem = 1;
	item.pszText = "ft-lb";
	mComboTorque.InsertItem(&item);
	//	mComboTorque.InsertString(1, "ft-lb");
	item.iItem = 2;
	item.pszText = "Nm";
	mComboTorque.InsertItem(&item);
	//	mComboTorque.InsertString(2, "Nm");
	item.iItem = 3;
	item.pszText = _T("Nmm");
	mComboTorque.InsertItem(&item);
	//	mComboTorque.InsertString(3, "Nmm");
	item.iItem = 4;
	item.pszText = _T("kgcm");
	mComboTorque.InsertItem(&item);
	//	mComboTorque.InsertString(4, "kgcm");
	//	mComboTorque.ShowDropDown(true);

	return TRUE;
}
