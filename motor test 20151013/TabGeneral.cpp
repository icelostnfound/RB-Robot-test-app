// TabGeneral.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "motor test 20151013.h"
#include "TabGeneral.h"
#include "afxdialogex.h"


// CTabGeneral �Ի���

IMPLEMENT_DYNAMIC(CTabGeneral, CDialogEx)

CTabGeneral::CTabGeneral(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTabGeneral::IDD, pParent)
{

}

CTabGeneral::~CTabGeneral()
{
}

void CTabGeneral::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECKAUTOLOAD, m_checkAutoload);
	DDX_Control(pDX, EDIT_TIME_INTERVAL, mEditTimeInterval);
	DDX_Control(pDX, CHECK_THERMISTOR, mCheckThermistor);
	DDX_Control(pDX, CHECK_BEEP, mCheckBeep);
}


BEGIN_MESSAGE_MAP(CTabGeneral, CDialogEx)
END_MESSAGE_MAP()


// CTabGeneral ��Ϣ�������
