#pragma once
#include "motor test 20151013.h"

// CTabGeneral �Ի���

class CTabGeneral : public CDialogEx
{
	DECLARE_DYNAMIC(CTabGeneral)
	/* grant CProgramOptionsDlg access to private members */
	friend class CProgramOptionsDlg;
public:
	CTabGeneral(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTabGeneral();
	// Dialog Data
	//{{AFX_DATA(CTabGeneral)
	enum { IDD = TAB_GENERAL };
	CButton	m_checkAutoload;
	CEdit	mEditTimeInterval;
	CButton	mCheckThermistor;
	CButton	mCheckBeep;
	//}}AFX_DATA
// �Ի�������

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
