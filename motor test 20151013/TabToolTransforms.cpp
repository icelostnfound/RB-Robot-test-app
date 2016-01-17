// TabToolTransforms.cpp : 实现文件
//

#include "stdafx.h"
#include "motor test 20151013.h"
#include "TabToolTransforms.h"
#include "afxdialogex.h"
#include "PopToolTransform.h"

// CTabToolTransforms 对话框

IMPLEMENT_DYNAMIC(CTabToolTransforms, CDialogEx)

CTabToolTransforms::CTabToolTransforms(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTabToolTransforms::IDD, pParent)
{

}

CTabToolTransforms::~CTabToolTransforms()
{
}

void CTabToolTransforms::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabToolTransforms)
	DDX_Control(pDX, LIST_TRANSFORMS, mListTransforms);
	DDX_Control(pDX, BUTTON_REMOVE_TRANSFORM, mButtonRemoveTransform);
	DDX_Control(pDX, BUTTON_REMOVE_ALL_TRANSFORMS, mButtonRemoveAllTransforms);
	DDX_Control(pDX, BUTTON_EDIT_TRANSFORM, mButtonEditTransform);
	DDX_Control(pDX, BUTTON_ADD_TRANSFORM, mButtonAddTransform);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTabToolTransforms, CDialogEx)
	ON_BN_CLICKED(BUTTON_ADD_TRANSFORM, &CTabToolTransforms::OnBnClickedAddTransform)
	ON_BN_CLICKED(BUTTON_EDIT_TRANSFORM, &CTabToolTransforms::OnBnClickedEditTransform)
	ON_BN_CLICKED(BUTTON_REMOVE_TRANSFORM, &CTabToolTransforms::OnBnClickedRemoveTransform)
	ON_BN_CLICKED(BUTTON_REMOVE_ALL_TRANSFORMS, &CTabToolTransforms::OnBnClickedRemoveAllTransforms)
END_MESSAGE_MAP()


// CTabToolTransforms 消息处理程序
void CTabToolTransforms::SetFTWrapper(FTWrapper *wrapper)
{
	this->wrapper = wrapper;
	this->calibration = wrapper->GetActiveCalibration();
	this->tools = calibration.GetToolTransforms();
	//	updateList();
}
/*更新工具的列表转变为主动校准 update the list of tool transforms for the active calibration */
void CTabToolTransforms::updateList()
{
	long num = tools.GetCount();
	mListTransforms.ResetContent();
	for (short i=0; i<(short)(num); i++) {
		_ToolTransform tool = tools.GetItem(COleVariant((short)(i+1),VT_I2));
		CString axis = tool.GetAxis();
		double value = tool.GetValue();
		CString units = tool.GetUnits();
		CString f;
		f.Format("%s: %2.4f %s", axis, value, units);
		mListTransforms.InsertString(i, f);
	}
}

BOOL CTabToolTransforms::OnInitDialog()
{
	CDialog::OnInitDialog();

	updateList();

	return TRUE;
}

void CTabToolTransforms::OnBnClickedAddTransform()
{
	// TODO: 在此添加控件通知处理程序代码
	CPopToolTransform popup(&tools);
	popup.DoModal();
	updateList();
}


void CTabToolTransforms::OnBnClickedEditTransform()
{
	// TODO: 在此添加控件通知处理程序代码
	int index = mListTransforms.GetCurSel();
	if (index != LB_ERR) {
		_ToolTransform tool = tools.GetItem(COleVariant((short)(index+1),VT_I2));
		CPopToolTransform popup(&tool);
		popup.DoModal();
		updateList();
	}
}


void CTabToolTransforms::OnBnClickedRemoveTransform()
{
	// TODO: 在此添加控件通知处理程序代码
	int index = mListTransforms.GetCurSel();
	if (index != LB_ERR) {
		// delete the transform with this index
		tools.Remove(COleVariant((short)(index+1), VT_I2));
		updateList();
	}
}



void CTabToolTransforms::OnBnClickedRemoveAllTransforms()
{
	// TODO: 在此添加控件通知处理程序代码
	tools.RemoveAll();
	updateList();
}
