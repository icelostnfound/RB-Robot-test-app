// LoginDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "motor test 20151013.h"
#include "LoginDlg.h"
#include "afxdialogex.h"


// CLoginDlg �Ի���

IMPLEMENT_DYNAMIC(CLoginDlg, CDialogEx)

CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLoginDlg::IDD, pParent)
	, m_strUser(_T(""))
	, m_strPasswd(_T(""))
{

}

CLoginDlg::~CLoginDlg()
{
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_StrUser_EDIT, m_strUser);
	DDX_Text(pDX, IDC_strPasswd_EDIT, m_strPasswd);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialogEx)
END_MESSAGE_MAP()


// CLoginDlg ��Ϣ�������
