#pragma once


// CTabNIDAQ 对话框

class CTabNIDAQ : public CDialogEx
{
	DECLARE_DYNAMIC(CTabNIDAQ)

public:
	CTabNIDAQ(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTabNIDAQ();

// 对话框数据
	enum { IDD = TAB_NIDAQ };
	CEdit	mEditDeviceNumber;
	CEdit	mEditScanRate;
	CEdit	mEditFirstChannelIndex;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
