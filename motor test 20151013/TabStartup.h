#pragma once


// CTabStartup �Ի���

class CTabStartup : public CDialogEx
{
	DECLARE_DYNAMIC(CTabStartup)

public:
	CTabStartup(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTabStartup();
	BOOL OnInitDialog();
// �Ի�������
	enum { IDD = TAB_STARTUP };
	CEdit	mEditConfigPath;
	CButton	mCheckLoadConfig;
	CButton	mButtonLoadConfig;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedLoadConfig();
	afx_msg void OnCheckLoadConfig();
};
