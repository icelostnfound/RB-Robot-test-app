#pragma once


// CTabOutputOptions �Ի���

class CTabOutputOptions : public CDialogEx
{
	DECLARE_DYNAMIC(CTabOutputOptions)

public:
	CTabOutputOptions(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTabOutputOptions();
	BOOL OnInitDialog();
// �Ի�������
	enum { IDD = TAB_OUTPUT_OPTIONS };
	CComboBoxEx	mComboTorque;
	CComboBoxEx	mComboForce;
	CStatic	mMessageTorque;
	CStatic	mMessageForce;
	CButton	mCheckboxTempComp;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
