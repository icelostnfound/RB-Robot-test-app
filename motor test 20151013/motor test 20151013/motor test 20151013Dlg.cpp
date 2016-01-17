
// motor test 20151013Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "motor test 20151013.h"
#include "motor test 20151013Dlg.h"
#include "ForceChartDlg.h"
#include "afxdialogex.h"
//Adlink head file////////
#include "APS168.h"
#include "ErrorCodeDef.h"
#include "APS_define.h"
#include "type_def.h"
#include "NIDAQmx.h"
//////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Cmotortest20151013Dlg 对话框



Cmotortest20151013Dlg::Cmotortest20151013Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Cmotortest20151013Dlg::IDD, pParent)
	, m_Position(-89821)
	, m_Vs(0)
	, m_Vm(4000)
	, m_Ve(0)
	, m_Acc(10000)
	, m_Dec(10000)
	, m_SFac(0.5)
	, m_Position1(-140524)
	, m_Vs1(0)
	, m_Vm1(10000)
	, m_Ve1(0)
	, m_Acc1(10000)
	, m_Dec1(10000)
	, m_SFac1(0.5)
	,times(0)
	,times3(0)
	,target1(0)
	,target2(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cmotortest20151013Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, m_Position);
	DDX_Text(pDX, IDC_EDIT3, m_Vs);
	DDX_Text(pDX, IDC_EDIT4, m_Vm);
	DDX_Text(pDX, IDC_EDIT5, m_Ve);
	DDX_Text(pDX, IDC_EDIT6, m_Acc);
	DDX_Text(pDX, IDC_EDIT7, m_Dec);
	DDX_Text(pDX, IDC_EDIT8, m_SFac);
	DDX_Text(pDX, IDC_EDIT9, m_Position1);
	DDX_Text(pDX, IDC_EDIT11, m_Vs1);
	DDX_Text(pDX, IDC_EDIT10, m_Vm1);
	DDX_Text(pDX, IDC_EDIT12, m_Ve1);
	DDX_Text(pDX, IDC_EDIT13, m_Acc1);
	DDX_Text(pDX, IDC_EDIT14, m_Dec1);
	DDX_Text(pDX, IDC_EDIT15, m_SFac1);
}

BEGIN_MESSAGE_MAP(Cmotortest20151013Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_Initial_BUTTON, &Cmotortest20151013Dlg::OnBnClickedInitialButton)
	ON_BN_CLICKED(IDC_ON_BUTTON, &Cmotortest20151013Dlg::OnBnClickedOnButton)
	ON_BN_CLICKED(IDC_OFF_BUTTON, &Cmotortest20151013Dlg::OnBnClickedOffButton)
	ON_BN_CLICKED(IDC_P2P_BUTTON4, &Cmotortest20151013Dlg::OnBnClickedP2pButton4)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_Reset_BUTTON, &Cmotortest20151013Dlg::OnBnClickedResetButton)
	ON_BN_CLICKED(IDC_LeftShouder_BUTTON, &Cmotortest20151013Dlg::OnBnClickedLeftshouderButton)
	ON_BN_CLICKED(IDC_RightEar_BUTTON, &Cmotortest20151013Dlg::OnBnClickedRightearButton)
	ON_BN_CLICKED(IDC_ForceChart_BUTTON, &Cmotortest20151013Dlg::OnBnClickedForcechartButton)
	ON_BN_CLICKED(IDC_LearnB_BUTTON, &Cmotortest20151013Dlg::OnBnClickedLearnbButton)
	ON_BN_CLICKED(IDC_LearnE_BUTTON2, &Cmotortest20151013Dlg::OnBnClickedLearneButton2)
	ON_BN_CLICKED(IDC_M_BUTTON, &Cmotortest20151013Dlg::OnBnClickedMButton)
	ON_BN_CLICKED(IDC_RelayOpen, &Cmotortest20151013Dlg::OnBnClickedRelayopen)
	ON_BN_CLICKED(IDC_RelayClose, &Cmotortest20151013Dlg::OnBnClickedRelayclose)
END_MESSAGE_MAP()


// Cmotortest20151013Dlg 消息处理程序

BOOL Cmotortest20151013Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	SetTimer(1,10,NULL );
	for(int i=0;i<2;i++)
	ptp[i]=0;
	TaskHandle  taskHandle=0;
	uInt8       open[2]={1,1};
	DAQmxCreateTask("open",&taskHandle);
	DAQmxCreateDOChan(taskHandle,"Dev3/port1/line0:1","",DAQmx_Val_ChanForAllLines);
	DAQmxStartTask(taskHandle);
	DAQmxWriteDigitalLines(taskHandle,1,1,10.0,DAQmx_Val_GroupByChannel,open,NULL,NULL);
	DAQmxStopTask(taskHandle);
	DAQmxClearTask(taskHandle);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void Cmotortest20151013Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void Cmotortest20151013Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR Cmotortest20151013Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void Cmotortest20151013Dlg::OnBnClickedInitialButton()
{
	// TODO: 在此添加控件通知处理程序代码
	I32 boardID_InBits = 0;
			I32 mode           = 0;
			I32 ret          = 0;
			I32 i            = 0;
			I32 card_name    = 0;
			I32 tamp         = 0;
			I32 StartAxisID  = 0;
			I32 TotalAxisNum = 0;


			// UI protection
			if (v_is_card_initialed == YES)
			{
				MessageBox( _T("initial ok") );
				return;
			}

			// Card(Board) initial
			ret = APS_initial( &boardID_InBits, mode);

			if( ret == ERR_NoError )
			{
				for (i = 0; i < 16; i++)
				{
					tamp = (boardID_InBits >> i) & 1;

					if( tamp == 1 )
					{
						ret = APS_get_card_name(i, &card_name);

						if (   card_name == DEVICE_NAME_PCI_8258 
							|| card_name == DEVICE_NAME_AMP_82548 )
						{
							ret = APS_get_first_axisId(i, &StartAxisID, &TotalAxisNum);

							//----------------------------------------------------
							v_card_name         = card_name;
							v_board_id          = i;
							v_total_axis        = TotalAxisNum;
							v_is_card_initialed = YES;

							if      (v_total_axis == 4) v_channel = 2;
							else if (v_total_axis == 8) v_channel = 4;
							//----------------------------------------------------

							Is_thread_creat = NO;
							SetDlgItemInt(EDIT_1st_axis, 0);
						
							MessageBox(_T("Initial ok !"));

							//Show main form title text
							if (card_name == DEVICE_NAME_PCI_8258 && TotalAxisNum  == 8 ) AfxGetMainWnd()->SetWindowText(_T("PCI-8258 Basic Sample"));
							if (card_name == DEVICE_NAME_PCI_8258 && TotalAxisNum  == 4 ) AfxGetMainWnd()->SetWindowText(_T("PCI-8254 Basic Sample"));
							if (card_name == DEVICE_NAME_AMP_82548 && TotalAxisNum == 8 ) AfxGetMainWnd()->SetWindowText(_T("AMP-208C Basic Sample"));
							if (card_name == DEVICE_NAME_AMP_82548 && TotalAxisNum == 4 ) AfxGetMainWnd()->SetWindowText(_T("AMP-204C Basic Sample"));

							break;
						}
					}
				}

				if (v_board_id == -1)
				{
					v_is_card_initialed = NO;
				    MessageBox(_T("Board Id search error !"));
				}	
			}
			else
			{
				v_is_card_initialed = NO;
				MessageBox(_T("Initial fail"));
				Function_Result(ret);
			}
			//使得各电机参数归零
			I32 Axis_ID = 0;
			for (Axis_ID = 0; Axis_ID < v_total_axis; Axis_ID++)
			{
				APS_set_command_f(Axis_ID, 0.0);
				APS_set_position_f(Axis_ID, 0.0);
			 }	
			//配置电机1的轴参数
			// 配置逻辑参数
			APS_set_axis_param(0,PRA_EL_LOGIC,0);
			APS_set_axis_param(0,PRA_ORG_LOGIC,0);
			APS_set_axis_param(0,PRA_ALM_LOGIC,0);
			APS_set_axis_param(0,PRA_EZ_LOGIC,0);
			APS_set_axis_param(0,PRA_INP_LOGIC,1);
            APS_set_board_param(0,PRB_EMG_LOGIC,1);
			////////////////////////
			APS_set_axis_param(0,PRA_PLS_IPT_MODE,0);//配置脉冲输入模式
			APS_set_axis_param(0,PRA_PLS_OPT_MODE,0);//配置脉冲输出（编码器）模式
			APS_set_axis_param(0,PRA_ENCODER_DIR,1);//配置编码器正负使能
			//配置电机2的轴参数
			// 配置逻辑参数
			APS_set_axis_param(1,PRA_EL_LOGIC,0);
			APS_set_axis_param(1,PRA_ORG_LOGIC,0);
			APS_set_axis_param(1,PRA_ALM_LOGIC,0);
			APS_set_axis_param(1,PRA_EZ_LOGIC,0);
			APS_set_axis_param(1,PRA_INP_LOGIC,0);
            APS_set_board_param(1,PRB_EMG_LOGIC,1);
			////////////////////////
			APS_set_axis_param(1,PRA_PLS_IPT_MODE,5);//配置脉冲输入模式
			APS_set_axis_param(1,PRA_PLS_OPT_MODE,0);//配置脉冲输出（编码器）模式
			APS_set_axis_param(1,PRA_ENCODER_DIR,1);//配置编码器正负使能
			GetDlgItem(IDC_ON_BUTTON)->EnableWindow(TRUE);
			GetDlgItem(IDC_OFF_BUTTON)->EnableWindow(TRUE);
			GetDlgItem(IDC_Reset_BUTTON)->EnableWindow(TRUE);
            GetDlgItem(IDC_P2P_BUTTON4)->EnableWindow(TRUE);
			GetDlgItem(IDC_LeftShouder_BUTTON)->EnableWindow(TRUE);
			GetDlgItem(IDC_RightEar_BUTTON)->EnableWindow(TRUE);
			GetDlgItem(IDC_LearnB_BUTTON)->EnableWindow(TRUE);
			GetDlgItem(IDC_LearnE_BUTTON2)->EnableWindow(TRUE);
			GetDlgItem(IDC_M_BUTTON)->EnableWindow(TRUE);
			GetDlgItem(IDC_ForceChart_BUTTON)->EnableWindow(TRUE);
			
			////////////////////////
}
int Cmotortest20151013Dlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	v_card_name         = 0;
    v_board_id          = -1;
    v_channel           = 0;
    v_total_axis        = 0; 
    v_is_card_initialed = NO;

	Is_thread_creat = NO;


	return 0;
}
void  Cmotortest20151013Dlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default

			I32 Axis_ID  = 0;
            I32 ret      = 0;

            if (v_is_card_initialed == YES)
            {
                for (Axis_ID = 0; Axis_ID < v_total_axis; Axis_ID++)
                {
                    ret = APS_set_servo_on(Axis_ID, OFF);
                }

                ret = APS_close();
            }
    KillTimer(1);
	CDialog::OnClose();
}
void Cmotortest20151013Dlg::Function_Result( I32 Ret )
{
	char MsgStr[256]="Error Code = ";
	CString str;
    str.Format(_T("%s"),MsgStr);
    MessageBox(str);
    char Buff[256];

	_itoa_s(Ret,Buff,10);     //盢俱计Retぇ锣Θ﹃Buff,10秈ボ	
	strcat_s(MsgStr,Buff);   //盢﹃Buff钡﹃MsgStr礛肚MsgStr﹃把计

	if (Ret != 0)
	{
		MessageBox(str,NULL,MB_OK); //MessageBox(饼陪ボぇ﹃ , 夹肈 ,秙Α)	
	}
}
I32 Cmotortest20151013Dlg::Is_axis_err(I32 Axis_ID)
{
	if (Axis_ID < 0 || Axis_ID >= v_total_axis)
    {
		MessageBox(_T("User Input Axis_ID Error !"));
		return YES; 
	}
	else
		return NO; 
}
I32 Cmotortest20151013Dlg::Is_CARD_INITIAL(void)
{
	if (v_is_card_initialed == NO)
    {
		MessageBox(_T("Not initialed."));
        return NO;
    }
    else
    {
		return YES;
    }
}
int Cmotortest20151013Dlg::get_bit(int num,int index)
{
	return(num>>(index)&1);
}
double Cmotortest20151013Dlg::average(double G[100])
{
	double average;
	double  sum=0;
	for (int i = 0; i<10; i++)
	{
		sum = sum + G[i];
	}

	//求平均值
		average =sum/ 10;
		return(average);
	
}
void Cmotortest20151013Dlg::OnBnClickedOnButton()
{
	// TODO: 在此添加控件通知处理程序代码
	I32 Axis_ID = 0;
     for (Axis_ID = 0; Axis_ID < 2; Axis_ID++)
		APS_set_servo_on(Axis_ID, ON);
}


void Cmotortest20151013Dlg::OnBnClickedOffButton()
{
	// TODO: 在此添加控件通知处理程序代码
	ptp[0]=0;
	ptp[1]=0;
	I32 Axis_ID = 0;
    for (Axis_ID = 0; Axis_ID < 2; Axis_ID++)
	APS_set_servo_on(Axis_ID, OFF);
}


void Cmotortest20151013Dlg::OnBnClickedP2pButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	if( Is_CARD_INITIAL() == NO ) return;
	I32 Axis_ID;
	for (Axis_ID = 0; Axis_ID < 2; Axis_ID++)
	{
		if (Is_axis_err(Axis_ID) == YES) return;

	I32 ret = 0;
	// Config speed profile parameters.
	ret = APS_set_axis_param_f( Axis_ID, PRA_SF, 0.5      );
	ret = APS_set_axis_param_f( Axis_ID, PRA_ACC, 10000.0 );
	ret = APS_set_axis_param_f( Axis_ID, PRA_DEC, 10000.0 );
	ret = APS_set_axis_param_f( Axis_ID, PRA_VM,  1000.0   );

	//Check servo on or not
	if( !( ( APS_motion_io_status( Axis_ID ) >> MIO_SVON ) & 1 ) )  
	{
		ret = APS_set_servo_on( Axis_ID, ON ); 
		Sleep( 100 ); // Wait stable.
	}
	}
	// Start a relative p to p move
	UpdateData(TRUE);
	APS_ptp_all( 
		  0  // I32 Axis_ID
		, OPT_RELATIVE // I32 Option
		, m_Position   // F64 Position
		, m_Vs     // F64 Vs
		, m_Vm // F64 Vm
		, m_Ve     // F64 Ve
		, m_Acc // F64 Acc
		, m_Dec // F64 Dec
		, m_SFac // F64 SFac
		, 0   // ASYNCALL *Wait
	);
	APS_ptp_all( 
		  1  // I32 Axis_ID
		, OPT_RELATIVE // I32 Option
		, m_Position1   // F64 Position
		, m_Vs1     // F64 Vs
		, m_Vm1 // F64 Vm
		, m_Ve1     // F64 Ve
		, m_Acc1 // F64 Acc
		, m_Dec1 // F64 Dec
		, m_SFac1 // F64 SFac
		, 0   // ASYNCALL *Wait
	);
	ptp[0]=1;
	UpdateData(FALSE);
}


void Cmotortest20151013Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	I32 Axis_ID = 0;
    F64 tmp = 0;
    I32 sts = 0;
	CString str=0;
	int32       error = 0;
	TaskHandle  taskHandle = 0;
	int32       read;
	double G4[100]={0};
	double G5[100]={0};
    CString str1;
	CString str2;
	CString str3;
	
	double test1=0;
	double test2=0;
	//轴1的详细参数
	switch (nIDEvent)
	{
	case 1:APS_get_command_f(Axis_ID, &tmp);
		str.Format(_T( "%f"), tmp );
        SetDlgItemText( EDIT_cmd_0 , str );      
        APS_get_position_f(Axis_ID, &tmp);
		str.Format( _T( "%f"), tmp );
        SetDlgItemText( EDIT_fb_0 , str );
                
        APS_get_target_position_f(Axis_ID, &tmp);
		str.Format( _T( "%f"), tmp );
        SetDlgItemText( EDIT_tg_0 , str );
                
        APS_get_error_position_f(Axis_ID, &tmp);
		str.Format( _T( "%f"), tmp );
        SetDlgItemText( EDIT_err_0 , str );
                
        APS_get_command_velocity_f(Axis_ID, &tmp);
		str.Format( _T( "%f"), tmp );
        SetDlgItemText( EDIT_cv_0 , str );
                
        APS_get_feedback_velocity_f(Axis_ID, &tmp);
		str.Format( _T( "%f"), tmp );
        SetDlgItemText( EDIT_fv_0 , str );
                
        sts = APS_motion_io_status(Axis_ID);
		str.Format( _T("0x%x"), sts );
        SetDlgItemText( EDIT_mio_0 , str );
                
        sts = APS_motion_status(Axis_ID);
		str.Format( _T("0x%x"), sts );
        SetDlgItemText( EDIT_msts_0, str );
		///////////////////////////////////
		//电机1运动状态
		str.Format( _T("%d"), get_bit(sts,0) );
		SetDlgItemText( IDC_CSTP_EDIT , str );

		str.Format( _T("%d"), get_bit(sts,4) );
		SetDlgItemText( IDC_DIR_EDIT , str );

		str.Format( _T("%d"), get_bit(sts,5) );
		SetDlgItemText( IDC_MDN_EDIT , str );

		str.Format( _T("%d"), get_bit(sts,15) );
		SetDlgItemText( IDC_JOG_EDIT , str );

		str.Format( _T("%d"),get_bit(sts,16) );
		SetDlgItemText( IDC_ASTP_EDIT , str );

		str.Format( _T("%d"), get_bit(sts,18) );
		SetDlgItemText( IDC_PRED_EDIT , str );

		str.Format( _T("%d"), get_bit(sts,19) );
		SetDlgItemText( IDC_POSTD_EDIT , str );
		////////////////////////////////////////////
		//轴2的详细参数
        APS_get_command_f(Axis_ID+1, &tmp);
		str.Format(_T( "%f"), tmp );
        SetDlgItemText( EDIT_cmd_1 , str );      
        APS_get_position_f(Axis_ID+1, &tmp);
		str.Format( _T( "%f"), tmp );
        SetDlgItemText( EDIT_fb_1 , str );
                
        APS_get_target_position_f(Axis_ID+1, &tmp);
		str.Format( _T( "%f"), tmp );
        SetDlgItemText( EDIT_tg_1 , str );
                
        APS_get_error_position_f(Axis_ID+1, &tmp);
		str.Format( _T( "%f"), tmp );
        SetDlgItemText( EDIT_err_1 , str );
                
        APS_get_command_velocity_f(Axis_ID+1, &tmp);
		str.Format( _T( "%f"), tmp );
        SetDlgItemText( EDIT_cv_1 , str );
                
        APS_get_feedback_velocity_f(Axis_ID+1, &tmp);
		str.Format( _T( "%f"), tmp );
        SetDlgItemText( EDIT_fv_1 , str );
                
        sts = APS_motion_io_status(Axis_ID+1);
		str.Format( _T("0x%x"), sts );
        SetDlgItemText( EDIT_mio_1 , str );
                
        sts = APS_motion_status(Axis_ID+1);
		str.Format( _T("0x%x"), sts );
        SetDlgItemText( EDIT_msts_1, str );
    ////////////////////////////////////////////
   //电机2运动状态
		str.Format( _T("%d"), get_bit(sts,0) );
		SetDlgItemText( IDC_CSTP_EDIT2 , str );

		str.Format( _T("%d"), get_bit(sts,4) );
		SetDlgItemText( IDC_DIR_EDIT2 , str );

		str.Format( _T("%d"), get_bit(sts,5) );
		SetDlgItemText( IDC_MDN_EDIT2 , str );

		str.Format( _T("%d"), get_bit(sts,15) );
		SetDlgItemText( IDC_JOG_EDIT2 , str );

		str.Format( _T("%d"),get_bit(sts,16) );
		SetDlgItemText( IDC_ASTP_EDIT2 , str );

		str.Format( _T("%d"), get_bit(sts,18) );
		SetDlgItemText( IDC_PRED_EDIT2 , str );

		str.Format( _T("%d"), get_bit(sts,19) );
		SetDlgItemText( IDC_POSTD_EDIT2 , str );
		////////////////////////////////////////////
		//摸左肩返回模式////////////////////////////////
		if(ptp[0]==1)
		{
			if((GetDlgItemInt(EDIT_fb_0)<-89500)&&(GetDlgItemInt(EDIT_fb_1)<=-140000))
			{
				if(GetDlgItemInt( IDC_MDN_EDIT)&&GetDlgItemInt(IDC_MDN_EDIT2 ))
				{
					///////摸左肩电机返回模式启动////////////////////
					   if( Is_CARD_INITIAL() == NO ) return;
					 
					   for (Axis_ID = 0; Axis_ID < 2; Axis_ID++)
						{
							if (Is_axis_err(Axis_ID) == YES) return;
							I32 ret = 0;
							// Config speed profile parameters.
							ret = APS_set_axis_param_f( Axis_ID, PRA_SF, 0.5      );
							ret = APS_set_axis_param_f( Axis_ID, PRA_ACC, 10000.0 );
							ret = APS_set_axis_param_f( Axis_ID, PRA_DEC, 10000.0 );
							ret = APS_set_axis_param_f( Axis_ID, PRA_VM,  1000.0   );
							//Check servo on or not
							if( !( ( APS_motion_io_status( Axis_ID ) >> MIO_SVON ) & 1 ) )  
							{
								ret = APS_set_servo_on( Axis_ID, ON ); 
								Sleep( 100 ); // Wait stable.
							}
						}
						// Start a relative p to p move
						APS_ptp_all( 
									  0  // I32 Axis_ID
									, OPT_RELATIVE // I32 Option
									, 89821  // F64 Position
									, 0     // F64 Vs
									, 6250 // F64 Vm
									, 0     // F64 Ve
									, 10000 // F64 Acc
									, 10000// F64 Dec
									, 0.5 // F64 SFac
									, 0   // ASYNCALL *Wait
									);
						APS_ptp_all( 
									  1  // I32 Axis_ID
									, OPT_RELATIVE // I32 Option
									, 140524   // F64 Position
									, 0     // F64 Vs
									, 10000 // F64 Vm
									,0     // F64 Ve
									, 10000 // F64 Acc
									, 10000 // F64 Dec
									, 0.5 // F64 SFac
									, 0   // ASYNCALL *Wait
									);
					///////摸左肩电机返回模式启动////////////////////
                 ptp[0]=0;
				}
			}
		 }
		////////////////////////////////////////////
		//摸右耳返回模式////////////////////////////////
		if(ptp[1]==1)
		{
			if((GetDlgItemInt(EDIT_fb_0)<=-114000)&&(GetDlgItemInt(EDIT_fb_1)<=-9500))
			{
				if(GetDlgItemInt( IDC_MDN_EDIT)&&GetDlgItemInt(IDC_MDN_EDIT2 ))
				{
					///////摸右耳电机返回模式启动////////////////////
					if( Is_CARD_INITIAL() == NO ) return;
					
					for (Axis_ID = 0; Axis_ID < 2; Axis_ID++)
					{
						if (Is_axis_err(Axis_ID) == YES) return;

						I32 ret = 0;
						// Config speed profile parameters.
						ret = APS_set_axis_param_f( Axis_ID, PRA_SF, 0.5      );
						ret = APS_set_axis_param_f( Axis_ID, PRA_ACC, 10000.0 );
						ret = APS_set_axis_param_f( Axis_ID, PRA_DEC, 10000.0 );
						ret = APS_set_axis_param_f( Axis_ID, PRA_VM,  1000.0   );

						//Check servo on or not
						if( !( ( APS_motion_io_status( Axis_ID ) >> MIO_SVON ) & 1 ) )  
						{
							ret = APS_set_servo_on( Axis_ID, ON ); 
							Sleep( 100 ); // Wait stable.
						}
					}
					// Start a relative p to p move
					APS_ptp_all( 
						  0  // I32 Axis_ID
						, OPT_RELATIVE // I32 Option
						, 114400  // F64 Position
						, 0     // F64 Vs
						, 10000 // F64 Vm
						, 0     // F64 Ve
						, 10000 // F64 Acc
						, 10000// F64 Dec
						, 0.5 // F64 SFac
						, 0   // ASYNCALL *Wait
								);
					APS_ptp_all( 
						  1  // I32 Axis_ID
						, OPT_RELATIVE // I32 Option
						, 10000   // F64 Position
						, 0     // F64 Vs
						, 10000 // F64 Vm
						,0     // F64 Ve
						, 10000 // F64 Acc
						, 10000 // F64 Dec
						, 0.5 // F64 SFac
						, 0   // ASYNCALL *Wait
								);
					///////摸右耳电机返回模式启动////////////////////
					 ptp[1]=0;
				}
			}
		}
		////////////////////////////////////////////
		break;
		case 2:
		//采集EMG1口数据
		DAQmxCreateTask("LearningTask1", &taskHandle);
		DAQmxCreateAIVoltageChan(taskHandle, "Dev3/ai4", "LearningChannel1", DAQmx_Val_RSE, 0, 5, DAQmx_Val_Volts, NULL);
		UpdateData(TRUE);
		DAQmxCfgSampClkTiming(taskHandle, "OnboardClock", 100, DAQmx_Val_Rising, DAQmx_Val_ContSamps, 20);
		UpdateData(TRUE);
		DAQmxStartTask(taskHandle);
		DAQmxReadAnalogF64(taskHandle, 10, 0.2, DAQmx_Val_GroupByChannel, G4, 10, &read, NULL);
		DAQmxStopTask(taskHandle);
		DAQmxClearTask(taskHandle);
		//采集EMG5口数据
		DAQmxCreateTask("LearningTask2", &taskHandle);
		DAQmxCreateAIVoltageChan(taskHandle, "Dev3/ai5", "LearningChannel2", DAQmx_Val_RSE, 0, 5, DAQmx_Val_Volts, NULL);
		UpdateData(TRUE);
		DAQmxCfgSampClkTiming(taskHandle, "OnboardClock", 100, DAQmx_Val_Rising, DAQmx_Val_ContSamps, 20);
		UpdateData(TRUE);
		DAQmxStartTask(taskHandle);
		DAQmxReadAnalogF64(taskHandle, 10, 0.2, DAQmx_Val_GroupByChannel, G5, 10, &read, NULL);
		DAQmxStopTask(taskHandle);
		DAQmxClearTask(taskHandle);
		G[times]=G4[5];
		R[times]=G5[5];
		times++;
		str1.Format(_T("%8.3f"), G4[5]);
		str2.Format(_T("%8.3f"),G5[5]);
		SetDlgItemText(IDC_Read1_EDIT, str1);
		SetDlgItemText(IDC_Read2_EDIT, str2);

		break;
		case 3:
		//////采集EMG1口数据///////////////////////
		DAQmxCreateTask("LearningTask1", &taskHandle );
		DAQmxCreateAIVoltageChan(taskHandle, "Dev3/ai4", "LearningChannel1", DAQmx_Val_RSE, 0, 5, DAQmx_Val_Volts, NULL);
		UpdateData(TRUE);
		DAQmxCfgSampClkTiming(taskHandle, "OnboardClock", 1000, DAQmx_Val_Rising, DAQmx_Val_ContSamps, 100);
		UpdateData(TRUE);
		DAQmxStartTask(taskHandle);
		DAQmxReadAnalogF64(taskHandle, 100, 0.2, DAQmx_Val_GroupByChannel, G4, 100, &read, NULL);
		DAQmxStopTask(taskHandle);
		DAQmxClearTask(taskHandle);
		//采集EMG5口数据///////////////////////////
		DAQmxCreateTask("LearningTask2", &taskHandle);
		DAQmxCreateAIVoltageChan(taskHandle, "Dev3/ai5", "LearningChannel2", DAQmx_Val_RSE, 0, 5, DAQmx_Val_Volts, NULL);
		UpdateData(TRUE);
		DAQmxCfgSampClkTiming(taskHandle, "OnboardClock", 1000, DAQmx_Val_Rising, DAQmx_Val_ContSamps, 100);
		UpdateData(TRUE);
		DAQmxStartTask(taskHandle);
		DAQmxReadAnalogF64(taskHandle, 100, 0.2, DAQmx_Val_GroupByChannel, G5, 100, &read, NULL);
		DAQmxStopTask(taskHandle);
		DAQmxClearTask(taskHandle);
		//////////////////////////////////////////
		times3++;
		for(int i=0;i<100;i++)
		 {
			sum1+=G4[i];
			sum2+=G5[i];
		 }
		if(times3==3)
		{
		test1=sum1/300;
		test2=sum2/300;
		str1.Format(_T("%8.3f"), test1);
		str2.Format(_T("%8.3f"),test2);
		SetDlgItemText(IDC_Test1_EDIT, str1);
		SetDlgItemText(IDC_Test2_EDIT, str2);
		times3=0;
		sum1=0;
		sum2=0;
		}

		if(((test2>=(target2/**2/3*/))&&(test2!=0))&&(test1>=(target1*1/2)))
		 {
			   KillTimer(3);
		/////////////摸左肩电机启动///////////////
			   if( Is_CARD_INITIAL() == NO ) return;
			
			   for (Axis_ID = 0; Axis_ID < 2; Axis_ID++)
				{
					if (Is_axis_err(Axis_ID) == YES) return;
					I32 ret = 0;
					// Config speed profile parameters.
					ret = APS_set_axis_param_f( Axis_ID, PRA_SF, 0.5      );
					ret = APS_set_axis_param_f( Axis_ID, PRA_ACC, 10000.0 );
					ret = APS_set_axis_param_f( Axis_ID, PRA_DEC, 10000.0 );
					ret = APS_set_axis_param_f( Axis_ID, PRA_VM,  1000.0   );
					//Check servo on or not
					if( !( ( APS_motion_io_status( Axis_ID ) >> MIO_SVON ) & 1 ) )  
					{
						ret = APS_set_servo_on( Axis_ID, ON ); 
						Sleep( 100 ); // Wait stable.
					}
				}
				// Start a relative p to p move
				APS_ptp_all( 
							  0  // I32 Axis_ID
							, OPT_RELATIVE // I32 Option
							, -89821  // F64 Position
							, 0     // F64 Vs
							, 6250 // F64 Vm
							, 0     // F64 Ve
							, 10000 // F64 Acc
							, 10000// F64 Dec
							, 0.5 // F64 SFac
							, 0   // ASYNCALL *Wait
							);
				APS_ptp_all( 
							  1  // I32 Axis_ID
							, OPT_RELATIVE // I32 Option
							, -140524   // F64 Position
							, 0     // F64 Vs
							, 10000 // F64 Vm
							,0     // F64 Ve
							, 10000 // F64 Acc
							, 10000 // F64 Dec
							, 0.5 // F64 SFac
							, 0   // ASYNCALL *Wait
							);
			///////摸左肩电机启动////////////////////
				ptp[0]=1;
				MessageBox(_T("摸肩"));
				return;
		 }
		if(test1>=(target1*2/3)&&(test2<=target2*2/3))
		 {
				KillTimer(3);
				/////////////摸右耳电机启动///////////////
				if( Is_CARD_INITIAL() == NO ) return;
			
				for (Axis_ID = 0; Axis_ID < 2; Axis_ID++)
				{
					if (Is_axis_err(Axis_ID) == YES) return;

					I32 ret = 0;
					// Config speed profile parameters.
					ret = APS_set_axis_param_f( Axis_ID, PRA_SF, 0.5      );
					ret = APS_set_axis_param_f( Axis_ID, PRA_ACC, 10000.0 );
					ret = APS_set_axis_param_f( Axis_ID, PRA_DEC, 10000.0 );
					ret = APS_set_axis_param_f( Axis_ID, PRA_VM,  1000.0   );

					//Check servo on or not
					if( !( ( APS_motion_io_status( Axis_ID ) >> MIO_SVON ) & 1 ) )  
					{
						ret = APS_set_servo_on( Axis_ID, ON ); 
						Sleep( 100 ); // Wait stable.
					}
				}
				// Start a relative p to p move
				APS_ptp_all( 
					  0  // I32 Axis_ID
					, OPT_RELATIVE // I32 Option
					, -114400  // F64 Position
					, 0     // F64 Vs
					, 10000 // F64 Vm
					, 0     // F64 Ve
					, 10000 // F64 Acc
					, 10000// F64 Dec
					, 0.5 // F64 SFac
					, 0   // ASYNCALL *Wait
							);
				APS_ptp_all( 
					  1  // I32 Axis_ID
					, OPT_RELATIVE // I32 Option
					, -10000   // F64 Position
					, 0     // F64 Vs
					, 10000 // F64 Vm
					,0     // F64 Ve
					, 10000 // F64 Acc
					, 10000 // F64 Dec
					, 0.5 // F64 SFac
					, 0   // ASYNCALL *Wait
							);
				///////摸右耳电机启动////////////////////
				ptp[1]=1;
				MessageBox(_T("屈肘"));
				return;
		 }
				break;
		}	
	
	
	CDialogEx::OnTimer(nIDEvent);
}
void Cmotortest20151013Dlg::OnBnClickedResetButton()
{
	// TODO: 在此添加控件通知处理程序代码
	I32 Axis_ID = 0;

    for (Axis_ID = 0; Axis_ID < v_total_axis; Axis_ID++)
    {
		APS_set_command_f(Axis_ID, 0.0);
        APS_set_position_f(Axis_ID, 0.0);
    }	
}

//摸左肩的消息处理函数
void Cmotortest20151013Dlg::OnBnClickedLeftshouderButton()
{
	// TODO: 在此添加控件通知处理程序代码
	if( Is_CARD_INITIAL() == NO ) return;
	I32 Axis_ID;
	for (Axis_ID = 0; Axis_ID < 2; Axis_ID++)
	{
		if (Is_axis_err(Axis_ID) == YES) return;

	I32 ret = 0;
	// Config speed profile parameters.
	ret = APS_set_axis_param_f( Axis_ID, PRA_SF, 0.5      );
	ret = APS_set_axis_param_f( Axis_ID, PRA_ACC, 10000.0 );
	ret = APS_set_axis_param_f( Axis_ID, PRA_DEC, 10000.0 );
	ret = APS_set_axis_param_f( Axis_ID, PRA_VM,  1000.0   );

	//Check servo on or not
	if( !( ( APS_motion_io_status( Axis_ID ) >> MIO_SVON ) & 1 ) )  
	{
		ret = APS_set_servo_on( Axis_ID, ON ); 
		Sleep( 100 ); // Wait stable.
	}
	}
	// Start a relative p to p move
	APS_ptp_all( 
		  0  // I32 Axis_ID
		, OPT_RELATIVE // I32 Option
		, -89821  // F64 Position
		, 0     // F64 Vs
		, 6250 // F64 Vm
		, 0     // F64 Ve
		, 10000 // F64 Acc
		, 10000// F64 Dec
		, 0.5 // F64 SFac
		, 0   // ASYNCALL *Wait
	);
	APS_ptp_all( 
		  1  // I32 Axis_ID
		, OPT_RELATIVE // I32 Option
		, -140524   // F64 Position
		, 0     // F64 Vs
		, 10000 // F64 Vm
		,0     // F64 Ve
		, 10000 // F64 Acc
		, 10000 // F64 Dec
		, 0.5 // F64 SFac
		, 0   // ASYNCALL *Wait
	);
	ptp[0]=1;
}

//摸右耳的消息处理函数
void Cmotortest20151013Dlg::OnBnClickedRightearButton()
{
	// TODO: 在此添加控件通知处理程序代码
	if( Is_CARD_INITIAL() == NO ) return;
	I32 Axis_ID;
	for (Axis_ID = 0; Axis_ID < 2; Axis_ID++)
	{
		if (Is_axis_err(Axis_ID) == YES) return;

		I32 ret = 0;
		// Config speed profile parameters.
		ret = APS_set_axis_param_f( Axis_ID, PRA_SF, 0.5      );
		ret = APS_set_axis_param_f( Axis_ID, PRA_ACC, 10000.0 );
		ret = APS_set_axis_param_f( Axis_ID, PRA_DEC, 10000.0 );
		ret = APS_set_axis_param_f( Axis_ID, PRA_VM,  1000.0   );

		//Check servo on or not
		if( !( ( APS_motion_io_status( Axis_ID ) >> MIO_SVON ) & 1 ) )  
		{
			ret = APS_set_servo_on( Axis_ID, ON ); 
			Sleep( 100 ); // Wait stable.
		}
	}
	// Start a relative p to p move
	APS_ptp_all( 
		  0  // I32 Axis_ID
		, OPT_RELATIVE // I32 Option
		, -114400  // F64 Position
		, 0     // F64 Vs
		, 10000 // F64 Vm
		, 0     // F64 Ve
		, 10000 // F64 Acc
		, 10000// F64 Dec
		, 0.5 // F64 SFac
		, 0   // ASYNCALL *Wait
				);
	APS_ptp_all( 
		  1  // I32 Axis_ID
		, OPT_RELATIVE // I32 Option
		, -10000   // F64 Position
		, 0     // F64 Vs
		, 10000 // F64 Vm
		,0     // F64 Ve
		, 10000 // F64 Acc
		, 10000 // F64 Dec
		, 0.5 // F64 SFac
		, 0   // ASYNCALL *Wait
				);
	ptp[1]=1;
}

//拉力传感器绘图
void Cmotortest20151013Dlg::OnBnClickedForcechartButton()
{
	// TODO: 在此添加控件通知处理程序代码
	/*CForceChartDlg ForceChartDlg;
	ForceChartDlg.DoModal();*/
	//创建非模态对话框实例
	m_ForceChartDlg= new CForceChartDlg();
	m_ForceChartDlg->Create(IDD_ForceChart_DIALOG,this);
	//显示非模态对话框
	m_ForceChartDlg->ShowWindow(SW_SHOW);
}


void Cmotortest20151013Dlg::OnBnClickedLearnbButton()
{
	// TODO: 在此添加控件通知处理程序代码
	times=0;
	SetTimer(2,10,NULL );
	KillTimer(3);
}


void Cmotortest20151013Dlg::OnBnClickedLearneButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	KillTimer(2);
	
	double sum1=0;
	double sum2=0;
	for(int i=0;i<times;i++)
	{
		sum1+=G[i];
		sum2+=R[i];
	}
	target1=sum1/times;
	target2=sum2/times;
	CString str1;
	CString str2;
	str1.Format(_T("%8.3f"), target1);
	str2.Format(_T("%8.3f"),target2);
	SetDlgItemText(IDC_Threshold1_EDIT, str1);
	SetDlgItemText(IDC_Threshold2_EDIT, str2);

}


void Cmotortest20151013Dlg::OnBnClickedMButton()
{
	// TODO: 在此添加控件通知处理程序代码
	SetTimer(3,10,NULL );
}


void Cmotortest20151013Dlg::OnBnClickedRelayopen()
{
	// TODO: 在此添加控件通知处理程序代码
	TaskHandle  taskHandle=0;
	uInt8       open[2]={1,1};

	DAQmxCreateTask("open",&taskHandle);
	DAQmxCreateDOChan(taskHandle,"Dev3/port1/line0:1","",DAQmx_Val_ChanForAllLines);
	DAQmxStartTask(taskHandle);
	DAQmxWriteDigitalLines(taskHandle,1,1,10.0,DAQmx_Val_GroupByChannel,open,NULL,NULL);
	DAQmxStopTask(taskHandle);
	DAQmxClearTask(taskHandle);
	GetDlgItem(IDC_RelayOpen)->EnableWindow(FALSE);
	GetDlgItem(IDC_RelayClose)->EnableWindow(TRUE);
}


void Cmotortest20151013Dlg::OnBnClickedRelayclose()
{
	// TODO: 在此添加控件通知处理程序代码
	TaskHandle  taskHandle=0;
	uInt8       close[2]={0,0};

	DAQmxCreateTask("close",&taskHandle);
	DAQmxCreateDOChan(taskHandle,"Dev3/port1/line0:1","",DAQmx_Val_ChanForAllLines);
	DAQmxStartTask(taskHandle);
	DAQmxWriteDigitalLines(taskHandle,1,1,10.0,DAQmx_Val_GroupByChannel,close,NULL,NULL);
	DAQmxStopTask(taskHandle);
	DAQmxClearTask(taskHandle);
	GetDlgItem(IDC_RelayOpen)->EnableWindow(TRUE);
	GetDlgItem(IDC_RelayClose)->EnableWindow(FALSE);
}
