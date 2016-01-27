#pragma once
#include "m1f_tchart.h"


// CShoulderEncoderDlg �Ի���

class CShoulderEncoderDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CShoulderEncoderDlg)

public:
	CShoulderEncoderDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CShoulderEncoderDlg();

// �Ի�������
	enum { IDD = DIALOG_SHOUL_ENCODER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CM1f_tchart m_TSChart;
	double Pos_S_Target;
	double Vel_S_Target;
	afx_msg void OnBnClickedSElbow();
	afx_msg void OnBnClickedST();
	afx_msg void OnBnSavePic();
};
