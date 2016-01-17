
// motor test 20151013Dlg.h : 头文件
//

#pragma once
//Adlink head file////////
#include "APS168.h"
#include "ErrorCodeDef.h"
#include "APS_define.h"
#include "type_def.h"
//////////////////////////
#include "Motion.h"
#include"ForceChartDlg.h"
// Cmotortest20151013Dlg 对话框
class Cmotortest20151013Dlg : public CDialogEx
{
// 构造
private:
	 CForceChartDlg *m_ForceChartDlg;
public:
	Cmotortest20151013Dlg(CWnd* pParent = NULL);	// 标准构造函数
// 对话框数据
	enum { IDD = IDD_MOTORTEST20151013_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP()
public:
	#define YES (1)
    #define NO  (0)
    #define ON  (1)
    #define OFF (0)
	afx_msg void OnBnClickedInitialButton();
	I32 v_card_name;
    I32 v_board_id;         
    I32 v_channel;           
    I32 v_total_axis;        
    I32 v_is_card_initialed;
    //////////////////////////////////////////////////////////////////////////////////
	I32 Is_thread_creat;
	void Function_Result( I32 Ret );
	I32 Is_CARD_INITIAL(void);
	I32 Is_axis_err(I32 Axis_ID);
	double average(double sum[4]);
	int get_bit(int num,int index);
	afx_msg void OnBnClickedOnButton();
	afx_msg void OnBnClickedOffButton();
	afx_msg void OnBnClickedP2pButton4();
	double m_Position;
	double m_Vs;
	double m_Vm;
	double m_Ve;
	double m_Acc;
	double m_Dec;
	double m_SFac;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedResetButton();
	double m_Position1;
	double m_Vs1;
	double m_Vm1;
	double m_Ve1;
	double m_Acc1;
	double m_Dec1;
	double m_SFac1;
	double G[100];
	double R[100];
	double target1;
	double target2;
	int times;
	int times3;
	int ptp[2];
	double sum1;
	double sum2;
	afx_msg void OnBnClickedLeftshouderButton();
	afx_msg void OnBnClickedRightearButton();
	afx_msg void OnBnClickedForcechartButton();
	afx_msg void OnBnClickedLearnbButton();
	afx_msg void OnBnClickedLearneButton2();
	afx_msg void OnBnClickedMButton();
	afx_msg void OnBnClickedRelayopen();
	afx_msg void OnBnClickedRelayclose();
};
