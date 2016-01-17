#pragma once


// CTabStartup 对话框

class CTabStartup : public CDialogEx
{
	DECLARE_DYNAMIC(CTabStartup)

public:
	CTabStartup(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTabStartup();
	BOOL OnInitDialog();
// 对话框数据
	enum { IDD = TAB_STARTUP };
	CEdit	mEditConfigPath;
	CButton	mCheckLoadConfig;
	CButton	mButtonLoadConfig;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedLoadConfig();
	afx_msg void OnCheckLoadConfig();
};
