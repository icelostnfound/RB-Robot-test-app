// TabNIDAQ.cpp : 实现文件
//

#include "stdafx.h"
#include "motor test 20151013.h"
#include "TabNIDAQ.h"
#include "afxdialogex.h"


// CTabNIDAQ 对话框

IMPLEMENT_DYNAMIC(CTabNIDAQ, CDialogEx)

CTabNIDAQ::CTabNIDAQ(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTabNIDAQ::IDD, pParent)
{

}

CTabNIDAQ::~CTabNIDAQ()
{
}

void CTabNIDAQ::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, EDIT_DEVICE_NUMBER, mEditDeviceNumber);
	DDX_Control(pDX, EDIT_SCAN_RATE, mEditScanRate);
	DDX_Control(pDX, EDIT_FIRST_CHANNEL_INDEX, mEditFirstChannelIndex);
}


BEGIN_MESSAGE_MAP(CTabNIDAQ, CDialogEx)
END_MESSAGE_MAP()


// CTabNIDAQ 消息处理程序
