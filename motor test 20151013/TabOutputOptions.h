#pragma once


// CTabOutputOptions 对话框

class CTabOutputOptions : public CDialogEx
{
	DECLARE_DYNAMIC(CTabOutputOptions)

public:
	CTabOutputOptions(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTabOutputOptions();
	BOOL OnInitDialog();
// 对话框数据
	enum { IDD = TAB_OUTPUT_OPTIONS };
	CComboBoxEx	mComboTorque;
	CComboBoxEx	mComboForce;
	CStatic	mMessageTorque;
	CStatic	mMessageForce;
	CButton	mCheckboxTempComp;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
