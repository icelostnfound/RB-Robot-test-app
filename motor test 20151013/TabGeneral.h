#pragma once
#include "motor test 20151013.h"

// CTabGeneral 对话框

class CTabGeneral : public CDialogEx
{
	DECLARE_DYNAMIC(CTabGeneral)
	/* grant CProgramOptionsDlg access to private members */
	friend class CProgramOptionsDlg;
public:
	CTabGeneral(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTabGeneral();
	// Dialog Data
	//{{AFX_DATA(CTabGeneral)
	enum { IDD = TAB_GENERAL };
	CButton	m_checkAutoload;
	CEdit	mEditTimeInterval;
	CButton	mCheckThermistor;
	CButton	mCheckBeep;
	//}}AFX_DATA
// 对话框数据

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
