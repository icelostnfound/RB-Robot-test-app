#pragma once


// CTabNIDAQ �Ի���

class CTabNIDAQ : public CDialogEx
{
	DECLARE_DYNAMIC(CTabNIDAQ)

public:
	CTabNIDAQ(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTabNIDAQ();

// �Ի�������
	enum { IDD = TAB_NIDAQ };
	CEdit	mEditDeviceNumber;
	CEdit	mEditScanRate;
	CEdit	mEditFirstChannelIndex;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
