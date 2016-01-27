
// motor test 20151013Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "motor test 20151013.h"
#include "motor test 20151013Dlg.h"
#include "ForceChartDlg.h"
#include "LoginDlg.h"
#include "SixAxisSensorDlg.h"
#include "ArmEncoderDlg.h"
#include "ShoulderEncoderDlg.h"
#include "afxdialogex.h"
//Adlink head file////////
#include "APS168.h"
#include "ErrorCodeDef.h"
#include "APS_define.h"
#include "type_def.h"
#include "NIDAQmx.h"
#include"CAxes.h"
#include"CAxis.h"
#include"CSeries.h"
#include"CTChart.h"
#include"CScroll.h"
#include <math.h>
#include<vector>
#include "Windows.h"
#include "mmsystem.h"
#pragma comment(lib,"winmm.lib")
//////////////////////////
extern  BOOL m_flag1;
extern  BOOL m_flag2;
extern  BOOL m_flag3;
extern  BOOL m_flag4;
extern  BOOL m_flag5;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define M_PI  3.14159265358979323846
#define Value_Posit_RightEar 114400
#define Value_Vel_RightEar 100000
#define Value_Posit_LeftShoulder 179642
#define Value_Vel_LeftShoulder 140524
#define Unit_Convert 3413.33    //60*1.25*16384/360 传动系数，60-减数比，16384-一圈脉冲数，2pi-圆周
#define Sample_Vel_Time 50//速度采样间隔
#define Pul_To_Degree 0.045// 360/2000——新买增量式编码器脉冲数转换成角度值得系数
#define KP_A 12500 
#define KI_A 500
//#define FF_A 30
#define KP_S 5625 
#define KI_S 250 
#define FF_S 12500

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
VOID ThreadFun1(LPVOID pParam);
VOID ThreadFun2(LPVOID pParam);
VOID ThreadFun3(LPVOID pParam);
VOID ThreadFun4(LPVOID pParam);
VOID ThreadFun5(LPVOID pParam);
//多媒体定时器回调函数-被动模式控制周期
VOID  CALLBACK OnTimeContr(UINT wTimerID,UINT msg,DWORD dwUser,DWORD dw1,DWORD dw2);
//多媒体定时器回调函数-采集编码器数据并输出速度
VOID  CALLBACK OnTimeFunc(UINT wTimerID,UINT msg,DWORD dwUser,DWORD dw1,DWORD dw2);
BOOL m_flag1 = TRUE;
BOOL m_flag2 = TRUE;
BOOL m_flag3 = TRUE;
BOOL m_flag4 = TRUE;
BOOL m_flag5 = TRUE;
BOOL PID_flag=true;
BOOL PID_Prio=TRUE;
int times=0;
int times3=0;

double sum1;
double sum2;
double target_Pos_Arm=0;
double target_Pos_Shoul=0;
double target1=0;
double target2=0;
double G[10]={0};
double Max_B_W[10]={0};
double Max_B_T[10]={0};
double R[10]={0};
double Max_A_W[10]={0};
double Max_A_T[10]={0};
double Range_EMG_A=0;
double Range_EMG_B=0;
double EMG[10]={0};
double EAFW[100]={0};
double EAFR[5]={0};
double Pos_init=0;
F64 tmp = 0; 
double Posi_Angle_Arm=0;//
double Posi_Angle_Shoul=0;//
F64 m_VEROR=0;
F64 m_VEROR1=0;
double  m_Pos_S[3]={0};
double  m_Pos_A[3]={0};
int AS_i=0;
double Vel_Real_AA=0;
double Vel_Real_SA=0;
double init_P=0;
F64 Vel_Buffer[4]={0};
F64 Vel_Buffer_S[4]={0};
double Pos_Comd_Motor=0;
double Pos_Comd_MS=0;
double Pos_Comd=0;
double Pos_Comd_Arm=0;
double  Pos_Comd_Shoul=0;
double Pos_Error[3]={0};
double Pos_Error_S[3]={0};
double Pos_PID=0;
double Pos_PID_S=0;
double T_C=50;//控制周期，单位毫秒
F64 Vel_Average=0;
BYTE rxdata1[1024]={0};
CMutex g_mutex;//互斥锁
UINT wAccuracy;// 定义分辨率
MMRESULT Mtimer_ID;
MMRESULT Vtimer_ID;
double exe_time_Vel=0;
double FT_i=0;
int Vel_i=0;
std::vector<double>Vel_Arm_Array;
std::vector<double>Vel_Shoul_Array;
std::vector<double>Pos_Arm_Array;
std::vector<double>Vel_Arm_Motor;
std::vector<double>Vel_Shoul_Motor;
std::vector<double>Pos_Shoul_Array;
std::vector<double>Time_Array;
double Motor_Statu_Vel=0;
double Motor_Statu_Vel1=0;
CString strTime;
Cmotortest20151013Dlg *g_pDlg=NULL;
LARGE_INTEGER start_t,stop_t;
LONG Timer_Out[2]={0};
int Timer_i=0;
LARGE_INTEGER freq;
//double Vel_Array[5000]={0};
//double Pos_Array[5000]={0};
//int Sum_i=0;
CForceChartDlg m_ForceChartDlg;

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
	, m_Position(89821)
	, m_Vs(0)
	, m_Vm(4000)
	, m_Ve(0)
	, m_Acc(10000)
	, m_Dec(10000)
	, m_SFac(0.5)
	, m_Position1(140524)
	, m_Vs1(0)
	, m_Vm1(10000)
	, m_Ve1(0)
	, m_Acc1(10000)
	, m_Dec1(10000)
	, m_SFac1(0.5)
	,m_FTT(0)
	, m_FTTEDIT(0)
	, KP_E(4)
	, KI_E(0.3)
	, KD_E(0)
	, FF_E(30)
	, m_VelShoul(0)
	, m_VelArm(0)
{
	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
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
	DDX_Text(pDX, EDIT_FTT_Z, m_FTTEDIT);
	DDX_Control(pDX, IDC_TCHART2, m_FTVEL);
	DDX_Control(pDX, IDC_MSCOMM1, m_comm);
	DDX_Text(pDX, EDIT_INPUT_KP, KP_E);
	DDX_Text(pDX, EDIT_INPUT_KI, KI_E);
	DDX_Text(pDX, EDIT_INPUT_KD, KD_E);
	DDX_Text(pDX, EDIT_FEEDFORD, FF_E);
	DDX_Text(pDX, EDIT_VEL_MOT2, m_VelShoul);
	DDX_Text(pDX, EDIT_VEL_MOT1, m_VelArm);
}

BEGIN_MESSAGE_MAP(Cmotortest20151013Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_ON_BUTTON, &Cmotortest20151013Dlg::OnBnClickedOnButton)
	ON_BN_CLICKED(IDC_P2P_BUTTON4, &Cmotortest20151013Dlg::OnBnClickedP2pButton4)
	ON_BN_CLICKED(IDC_Reset_BUTTON, &Cmotortest20151013Dlg::OnBnClickedResetButton)
	ON_BN_CLICKED(IDC_LeftShouder_BUTTON, &Cmotortest20151013Dlg::OnBnClickedLeftshouderButton)
	ON_BN_CLICKED(IDC_RightEar_BUTTON, &Cmotortest20151013Dlg::OnBnClickedRightearButton)
	ON_BN_CLICKED(IDC_LearnB_BUTTON, &Cmotortest20151013Dlg::OnBnClickedLearnbButton)
	ON_BN_CLICKED(IDC_LearnE_BUTTON2, &Cmotortest20151013Dlg::OnBnClickedLearneButton2)
	ON_BN_CLICKED(IDC_M_BUTTON, &Cmotortest20151013Dlg::OnBnClickedMButton)
	ON_BN_CLICKED(IDC_RelayClose, &Cmotortest20151013Dlg::OnBnClickedRelayclose)
	ON_COMMAND(ID_TOOLS_SIX, &Cmotortest20151013Dlg::OnToolsSix)
	ON_COMMAND(ID_TOOLS_ENCODER, &Cmotortest20151013Dlg::OnToolsEncoder)
	ON_BN_CLICKED(BUTTON_VEL_MOT1, &Cmotortest20151013Dlg::OnBnClickedVelMot1)
	ON_BN_CLICKED(BUTTON_VEL_MOT2, &Cmotortest20151013Dlg::OnBnClickedVelMot2)
	ON_BN_CLICKED(Button_VEL_STOP, &Cmotortest20151013Dlg::OnBnClickedVelStop)
	ON_BN_CLICKED(BUTTON_VEL_START, &Cmotortest20151013Dlg::OnStartWithSensor)
	ON_WM_TIMER()
	ON_WM_COPYDATA()
	ON_BN_CLICKED(IDC_HOME_BUTTON, &Cmotortest20151013Dlg::OndHomeButton)
	ON_BN_CLICKED(BUTTON_PID_CONTROL, &Cmotortest20151013Dlg::OnPidControl)
	ON_BN_CLICKED(IDC_RADIO_POSITION, &Cmotortest20151013Dlg::OnRadioPosition)
	ON_BN_CLICKED(IDC_RADIO_V_SINGlE, &Cmotortest20151013Dlg::OnRadioVSingle)
	ON_BN_CLICKED(IDC_RADIO_SIX, &Cmotortest20151013Dlg::OnRadioSix)
	ON_BN_CLICKED(IDC_RADIO_PID, &Cmotortest20151013Dlg::OnRadioPid)
	ON_BN_CLICKED(IDC_RADIO_EMG, &Cmotortest20151013Dlg::OnRadioEmg)
	ON_BN_CLICKED(IDC_BUTTON_QUIT, &Cmotortest20151013Dlg::OnButtonQuit)
	ON_BN_CLICKED(BUTTON_PID_SAVE, &Cmotortest20151013Dlg::OnPidSaveData)
	ON_BN_CLICKED(BUTTON_VEL_MODE, &Cmotortest20151013Dlg::OnBnVelMode)
	ON_COMMAND(ID_EQUIPMENTS_ARMENCODER, &Cmotortest20151013Dlg::OnEquipmentsArmencoder)
	ON_COMMAND(ID_Menu32802, &Cmotortest20151013Dlg::OnMenuShoulE)
END_MESSAGE_MAP()


// Cmotortest20151013Dlg 消息处理程序
BOOL Cmotortest20151013Dlg::Login()
{
	while(1)
	 {/////
	  //CLoginDlg myDlg;
	  int nReturn = myDlg.DoModal();
	  if(nReturn==IDCANCEL)
		  {
		   return FALSE;
		  }
	  if(nReturn==IDOK)
		  {
		   CString strUser = myDlg.m_strUser;
		   CString strPasswd = myDlg.m_strPasswd;
		   if( !(strUser==_T("admin") && strPasswd==_T("admin")) )   //如果用户/密码不是admin/admin的话，则报错，提示重新输入，直至正确为止;
			   {
				if(IDOK==MessageBox(_T("用户ID或密码错误！\n  请重新输入！")))
				 continue;
				else
				 return FALSE;
			   }
		   else
			break;
	      }
  
	 }/////-while-end
	return FALSE;
}
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
	
	g_pDlg=this;
	OnBnClickedInitialButton();
	mt_flag=TRUE;
	relay_flag=TRUE;
	for(int i=0;i<2;i++)
		ptp[i]=0;
	//Reci_Vel_Arm();//开启多媒体定时器
	RelaySwitchOpen();//开启继电器
	
	//用户登录 Login();
	//开启线程1-输出电机状态参数，实时监测返回机制是否启动
	hThread1=CreateThread(NULL,
		0,
		(LPTHREAD_START_ROUTINE)ThreadFun1,
		this,
		0,
&ThreadID1);
//	//开启线程4-实时输出两个角位移传感器，两个拉力传感器的值
	hThread4=CreateThread(NULL,
		0,
		(LPTHREAD_START_ROUTINE)ThreadFun4,
		this,
		0,
		&ThreadID4);
	/*hThread5=CreateThread(NULL,
	0,
	(LPTHREAD_START_ROUTINE)ThreadFun5,
	this,
	0,
	&ThreadID5);*/
	///////////////////////////////////////////////////////
	
	//痉挛检测机制-开启数据采集线程////////////////////////////////////
	//m_ForceChartDlg.Cramps();
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
//检测一个字符是不是十六进制字符，若是返回相应的值，否则返回0x10
char HexChar(char c)
{
	if ((c >= '0') && (c <= '9'))
		return c - 0x30;
	else if ((c >= 'A') && (c <= 'F'))
		return c - 'A' + 10;
	else if ((c >= 'a') && (c <= 'f'))
		return c - 'a' + 10;
	else return 0x10;
}
//将一个字符串作为十六进制串转化为一个字节数组，字节间可用空格分隔，返回转换后的字节数组长度，同时字节数组长度自动设置。
int Str2Hex(CString str, CByteArray &data)
{
	int t, t1;
	int rlen = 0, len = str.GetLength();
	data.SetSize(len / 2);
	for (int i = 0; i<len;)
	{
		char l, h = str[i];
		if (h == ' ')
		{
			i++;
			continue;
		}
		i++;
		if (i >= len)break;
		l = str[i];
		t = HexChar(h);
		t1 = HexChar(l);
		if ((t == 16) || (t1 == 16))
			break;
		else t = t * 16 + t1;
		i++;
		data[rlen] = (char)t;
		rlen++;
	}
	data.SetSize(rlen);
	return rlen;
}
BOOL Cmotortest20151013Dlg::onCommSet()
{
	CString str;
	BOOL open_statu=TRUE;
	str.Format(_T ("%d,n,8,1"),115200);
	open_statu=m_comm.get_PortOpen();
	if (!open_statu)
	{
		m_comm.put_CommPort(5); //选择默认串口com5 
		m_comm.put_InputMode(1);   //设置输入方式为二进制方式 
		m_comm.put_Settings(str);  //波特率为115200,无校验，8数据位，1个停止位 
		m_comm.put_InputLen(0);  //设置当前接收区数据长度为0,即：接收串口所有的数据
		m_comm.put_InBufferSize(32000);
		m_comm.put_OutBufferSize(32000);
		m_comm.put_RThreshold(1);   //缓冲区一个字符引发事件  
		m_comm.put_RTSEnable(1);   //设置RT允许 
		m_comm.put_PortOpen(true);//打开串口 
		
	} 
	else
	{
		m_comm.put_PortOpen(false);
	}
	return(open_statu);
}
//继电器开启函数
void Cmotortest20151013Dlg::RelaySwitchOpen()
{
	TaskHandle  taskHandle=0;
	uInt8       open[2]={1,1};
	DAQmxCreateTask("open",&taskHandle);
	DAQmxCreateDOChan(taskHandle,"Dev1/port1/line0:1","",DAQmx_Val_ChanForAllLines);
	DAQmxStartTask(taskHandle);
	DAQmxWriteDigitalLines(taskHandle,1,1,10.0,DAQmx_Val_GroupByChannel,open,NULL,NULL);
	DAQmxStopTask(taskHandle);
	DAQmxClearTask(taskHandle);
}
//继电器关闭函数
void Cmotortest20151013Dlg::RelaySwitchClose()
{
	TaskHandle  taskHandle=0;
	uInt8       close[2]={0,0};
	DAQmxCreateTask("close",&taskHandle);
	DAQmxCreateDOChan(taskHandle,"Dev1/port1/line0:1","",DAQmx_Val_ChanForAllLines);
	DAQmxStartTask(taskHandle);
	DAQmxWriteDigitalLines(taskHandle,1,1,10.0,DAQmx_Val_GroupByChannel, close,NULL,NULL);
	DAQmxStopTask(taskHandle);
	DAQmxClearTask(taskHandle);
}
//电机运动函数， Position1为电机一目标位置 Position2为电机二目标位置
void Cmotortest20151013Dlg::MoveToPosition(F64 Position1,F64 Position2 )
{
	I32 Axis_ID = 0;
	if(Is_CARD_INITIAL() == NO ) return;
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
		, Position1  // F64 Position
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
		, Position2   // F64 Position
		, 0     // F64 Vs
		, 10000 // F64 Vm
		,0     // F64 Ve
		, 10000 // F64 Acc
		, 10000 // F64 Dec
		, 0.5 // F64 SFac
		, 0   // ASYNCALL *Wait
		);
}
void Cmotortest20151013Dlg::LeftshouderMove()//摸左肩运动开始
{
	MoveToPosition(Value_Posit_LeftShoulder,Value_Vel_LeftShoulder );	
}
void Cmotortest20151013Dlg::LeftshouderBack()//摸左肩结束，开始返回
{
	MoveToPosition(-Value_Posit_LeftShoulder,-Value_Vel_LeftShoulder );	
}
void Cmotortest20151013Dlg::RightearMove()//摸右耳运动开始
{
	MoveToPosition(Value_Posit_RightEar,Value_Vel_RightEar );
}
void Cmotortest20151013Dlg::RightearBack()//摸右耳结束，开始返回
{
	MoveToPosition(-Value_Posit_RightEar,-Value_Vel_RightEar );
}
//线程函数-线程1-输出电机状态参数，实时监测返回机制是否启动
void ThreadFun1(LPVOID pParam)
{
	I32 Axis_ID = 2;
    F64 tmp = 0;
    I32 sts = 0;
	CString str=0;
	Cmotortest20151013Dlg *dlg=(Cmotortest20151013Dlg*)pParam;
	
	while(m_flag1)
	{
		//轴1的详细参数
	   APS_get_command_f(Axis_ID, &tmp);
		str.Format(_T( "%f"), tmp );
        ::SetDlgItemText(dlg->m_hWnd, EDIT_cmd_0 , str );  

        APS_get_position_f(Axis_ID, &tmp);
		str.Format( _T( "%f"), tmp );
        ::SetDlgItemText(dlg->m_hWnd, EDIT_fb_0 , str ); 

        APS_get_target_position_f(Axis_ID, &tmp);
		str.Format( _T( "%f"), tmp );
       ::SetDlgItemText(dlg->m_hWnd, EDIT_tg_0 , str );
                
        APS_get_error_position_f(Axis_ID, &tmp);
		str.Format( _T( "%f"), tmp );
       ::SetDlgItemText(dlg->m_hWnd, EDIT_err_0 , str );
                
        APS_get_command_velocity_f(Axis_ID, &tmp);
		str.Format( _T( "%f"), tmp );
        ::SetDlgItemText(dlg->m_hWnd, EDIT_cv_0 , str );
                
        APS_get_feedback_velocity_f(Axis_ID, &tmp);
		str.Format( _T( "%f"), tmp );
        ::SetDlgItemText(dlg->m_hWnd, EDIT_fv_0 , str );
                
        sts = APS_motion_io_status(Axis_ID);
		str.Format( _T("0x%x"), sts );
        ::SetDlgItemText(dlg->m_hWnd, EDIT_mio_0 , str );
                
        sts = APS_motion_status(Axis_ID);
		str.Format( _T("0x%x"), sts );
        ::SetDlgItemText(dlg->m_hWnd, EDIT_msts_0, str );
		///////////////////////////////////
		//电机1运动状态
		/*str.Format( _T("%d"), dlg->get_bit(sts,0) );
		::SetDlgItemText(dlg->m_hWnd, IDC_CSTP_EDIT , str );

		str.Format( _T("%d"), dlg->get_bit(sts,4) );
		::SetDlgItemText(dlg->m_hWnd, IDC_DIR_EDIT , str );

		str.Format( _T("%d"), dlg->get_bit(sts,5) );
		::SetDlgItemText(dlg->m_hWnd, IDC_MDN_EDIT , str );

		str.Format( _T("%d"), dlg->get_bit(sts,15) );
		::SetDlgItemText(dlg->m_hWnd, IDC_JOG_EDIT , str );

		str.Format( _T("%d"),dlg->get_bit(sts,16) );
		::SetDlgItemText(dlg->m_hWnd, IDC_ASTP_EDIT , str );

		str.Format( _T("%d"), dlg->get_bit(sts,18) );
		::SetDlgItemText(dlg->m_hWnd, IDC_PRED_EDIT , str );

		str.Format( _T("%d"), dlg->get_bit(sts,19) );
		::SetDlgItemText(dlg->m_hWnd, IDC_POSTD_EDIT , str );*/
		////////////////////////////////////////////
		//轴2的详细参数
        APS_get_command_f(Axis_ID+1, &tmp);
		str.Format(_T( "%f"), tmp );
        ::SetDlgItemText(dlg->m_hWnd, EDIT_cmd_1 , str );      
        APS_get_position_f(Axis_ID+1, &tmp);
		str.Format( _T( "%f"), tmp );
        ::SetDlgItemText(dlg->m_hWnd, EDIT_fb_1 , str );
                
        APS_get_target_position_f(Axis_ID+1, &tmp);
		str.Format( _T( "%f"), tmp );
       ::SetDlgItemText(dlg->m_hWnd, EDIT_tg_1 , str );
                
        APS_get_error_position_f(Axis_ID+1, &tmp);
		str.Format( _T( "%f"), tmp );
        ::SetDlgItemText(dlg->m_hWnd, EDIT_err_1 , str );
                
        APS_get_command_velocity_f(Axis_ID+1, &tmp);
		str.Format( _T( "%f"), tmp );
        ::SetDlgItemText(dlg->m_hWnd, EDIT_cv_1 , str );
                
        APS_get_feedback_velocity_f(Axis_ID+1, &tmp);
		str.Format( _T( "%f"), tmp );
        ::SetDlgItemText(dlg->m_hWnd, EDIT_fv_1 , str );
                
        sts = APS_motion_io_status(Axis_ID+1);
		str.Format( _T("0x%x"), sts );
        ::SetDlgItemText(dlg->m_hWnd, EDIT_mio_1 , str );
                
        sts = APS_motion_status(Axis_ID+1);
		str.Format( _T("0x%x"), sts );
        ::SetDlgItemText(dlg->m_hWnd, EDIT_msts_1, str );
		////////////////////////////////////////////
		//电机2运动状态
		/*str.Format( _T("%d"), dlg->get_bit(sts,0) );
		::SetDlgItemText(dlg->m_hWnd, IDC_CSTP_EDIT2 , str );

		str.Format( _T("%d"), dlg->get_bit(sts,4) );
		::SetDlgItemText(dlg->m_hWnd,IDC_DIR_EDIT2 , str );

		str.Format( _T("%d"), dlg->get_bit(sts,5) );
		::SetDlgItemText(dlg->m_hWnd, IDC_MDN_EDIT2 , str );

		str.Format( _T("%d"), dlg->get_bit(sts,15) );
		::SetDlgItemText(dlg->m_hWnd, IDC_JOG_EDIT2 , str );

		str.Format( _T("%d"),dlg->get_bit(sts,16) );
		::SetDlgItemText(dlg->m_hWnd, IDC_ASTP_EDIT2 , str );

		str.Format( _T("%d"), dlg->get_bit(sts,18) );
		::SetDlgItemText(dlg->m_hWnd, IDC_PRED_EDIT2 , str );

		str.Format( _T("%d"), dlg->get_bit(sts,19) );
		::SetDlgItemText(dlg->m_hWnd, IDC_POSTD_EDIT2 , str );*/
		//痉挛检测机制-检测，过载反应////////////////////////////////////
		/*if((m_ForceChartDlg.RD[3]*196)>500)
		{
		APS_stop_move(0);
		APS_stop_move(1);
		}*/
        ////////////////////////////////////////////
		//摸左肩返回模式////////////////////////////////
		if(dlg->ptp[0]==1)
		{
			if((dlg->GetDlgItemInt(EDIT_fb_0)<-89500)&&(dlg->GetDlgItemInt(EDIT_fb_1)<=-140000))
			{
				if(dlg->GetDlgItemInt( IDC_MDN_EDIT)&&dlg->GetDlgItemInt(IDC_MDN_EDIT2 ))
				{
					///////摸左肩电机返回模式启动////////////////////
					dlg->LeftshouderBack();
                 dlg->ptp[0]=0;
				}
			}
		 }
		////////////////////////////////////////////
		//摸右耳返回模式////////////////////////////////
		if(dlg->ptp[1]==1)
		{
			if((dlg->GetDlgItemInt(EDIT_fb_0)<=-114000)&&(dlg->GetDlgItemInt(EDIT_fb_1)<=-9500))
			{
				if(dlg->GetDlgItemInt( IDC_MDN_EDIT)&&dlg->GetDlgItemInt(IDC_MDN_EDIT2 ))
				{
					///////摸右耳电机返回模式启动////////////////////
					dlg->RightearBack();
					 dlg->ptp[1]=0;
				}
			}
		}
		Sleep(100);
		
	}
	return;

}
//线程函数-线程2-学习模式//////////////////////////////
void ThreadFun2(LPVOID pParam)
{
    CString str1;
	CString str2;
	CSeries lineseries1;
	CSeries lineseries2;
	int Max_i=0;
	int Mode_i=0;
	Cmotortest20151013Dlg *dlg=(Cmotortest20151013Dlg*)pParam;
	m_flag2=TRUE;
	lineseries1=(CSeries)dlg->m_FTVEL.Series(0);
	lineseries2=(CSeries)dlg->m_FTVEL.Series(1);
	AfxMessageBox(_T("学习开始，开始屈肘！"));
	while(m_flag2)
	{
	//采集EMG1口数据-胸肌
		if (times==10)
		{
			times=0;
			
			for (int i=0;i<10;i++)
			{
				if (G[i]>G[i+1])
				{
					G[i+1]=G[i];
				}
				if (R[i]>R[i+1])
				{
					R[i+1]=R[i];
				}
			}
			if (Mode_i==0)
			{
				Max_B_W[Max_i]=G[9];
				Max_A_W[Max_i]=R[9];
			}
			else
			{
				Max_B_T[Max_i]=G[9];
				Max_A_T[Max_i]=R[9];
			}
			
			Max_i++;
		}
		if (Max_i==10)
		{
			if (Mode_i==0)
			{
		AfxMessageBox(_T("屈肘结束，开始摸肩！"));
			}
			Max_i=0;
			Mode_i++;
		}
		if (Mode_i==2)
		{
			AfxMessageBox(_T("摸肩结束，即将退出学习！"));
			m_flag2=FALSE;
			break;
		}
		dlg->DAQ_EAF("Dev4",0,1);
		G[times]=EAFW[0];
		str1.Format(_T("%8.6f"),EAFW[0]);
		::SetDlgItemText(dlg->m_hWnd,IDC_Read1_EDIT, str1);
		//采集EMG1口数据-肱二头肌
		//dlg->DAQ_EMG("Dev4",1);
		R[times]=EAFW[1];
		str2.Format(_T("%8.6f"),EAFW[1]);
		::SetDlgItemText(dlg->m_hWnd,IDC_Read2_EDIT, str2);
		/*lineseries1.AddXY((float)times*0.01,G[times],NULL,0);
		lineseries2.AddXY((float)times*0.01,R[times],NULL,0);*/
		times++;
		Sleep(100);
	}
	return;
}
//线程函数-线程3-肌肉触发模式//////////////////////////////
void ThreadFun3(LPVOID pParam)
{
    CString str1;
	CString str2;
	Cmotortest20151013Dlg *dlg=(Cmotortest20151013Dlg*)pParam;
	double test1=0;
	double test2=0;
	m_flag3=TRUE;
	while(m_flag3)
	{
        //采集EMG1口数据///////////////////////
		dlg->DAQ_EAF("Dev4",0,1);
		if (EAFW[1]>Range_EMG_A)
		{
			AfxMessageBox(_T("屈肘运动即将开始！"));
			dlg->OnBnClickedRightearButton();
			m_flag3=FALSE;
			break;
		}
		if (EAFW[0]>Range_EMG_B)
		{
			AfxMessageBox(_T("摸左肩运动即将开始！"));
			dlg->OnBnClickedLeftshouderButton();
			m_flag3=FALSE;
			break;
		}
	
		Sleep(100);
		}
		return;
}
//线程4-实时输出两个角位移传感器，两个拉力传感器的值
void ThreadFun4(LPVOID pParam)
{
	CString str1;
	CString str2;
	CString str3;
	CString str6;
	CString str_i;
	CString m_Adress;
	double  m_MF1;
	double m_MF2;
	double m_linear=0;
	double M_A=0;
	double M_S=0;
	CByteArray data;
	CTime time;
	int len =0;
	int i=0;
//	clock_t start,finish;clock_t start,finish;//定义存储开始于结束的时间的变量
	Cmotortest20151013Dlg *dlg=(Cmotortest20151013Dlg*)pParam;
	m_flag4 = TRUE;
	while (m_flag4)
	{
		
		APS_get_feedback_velocity_f(2, &M_A);
		APS_get_feedback_velocity_f(3, &M_S);
		/*if (PID_Prio)
		{
			if (Posi_Angle_Arm<-10||Posi_Angle_Arm>200||Posi_Angle_Shoul<-10||Posi_Angle_Shoul>200||M_A>200000||M_S>200000)
			{
				dlg->OnBnClickedVelStop();
				dlg->OnWarning();
			}
			else
			{
				dlg->OnWarning();
			}
		}*/
		//dlg->DAQ_EAF("Dev1",2,3);//数据采集卡采集角位移以及拉力信号
		//dlg->DAQ_EMG("Dev1",6);//采集腕部光电位移传感器信号
		m_linear=80-6*EMG[5];
		m_MF1=EAFW[0]*196;
		m_MF2=EAFW[1]*196;
		str1.Format(_T("%8.3f"),m_MF1);
		str2.Format(_T("%8.3f"),m_MF2);
		str3.Format(_T("%8.3f"),m_linear);
		::SetDlgItemText(dlg->m_hWnd,EDIT_MF1, str1);
		::SetDlgItemText(dlg->m_hWnd,EDIT_MF2, str2);
		::SetDlgItemText(dlg->m_hWnd,EDIT_LINEAR, str3);
		//保护模式-速度，力，位置
	}
	
	return;
}
//线程5-显示增量式编码器角度值并输出速度
void ThreadFun5(LPVOID pParam)
{
	CString str;
	CString str1;
	CSeries lineseries1;
	CSeries lineseries2;
	CSeries lineseries3;
	CSeries lineseries4;
	CSeries lineseries5;
	CSeries lineseries6;
	F64 Tmp_Motor_Pos=0;
	Cmotortest20151013Dlg *dlg=(Cmotortest20151013Dlg*)pParam;
	m_flag5=TRUE;
	int i=0;
	lineseries1=(CSeries)dlg->m_FTVEL.Series(0);
	lineseries2=(CSeries)dlg->m_FTVEL.Series(1);
	lineseries3=(CSeries)dlg->m_FTVEL.Series(2);
	lineseries4=(CSeries)dlg->m_FTVEL.Series(3);
	lineseries5=(CSeries)dlg->m_FTVEL.Series(4);
	lineseries6=(CSeries)dlg->m_FTVEL.Series(5);
	while (m_flag5)
	{ 
		g_mutex.Lock();
		str1.Format( _T( "%f"), Vel_Real_AA );//窗口显示实时速度
		::SetDlgItemText(dlg->m_hWnd, EDIT_AA_VEL ,str1 ); 

		str.Format( _T( "%f"), Posi_Angle_Arm );//窗口显示实时速度
		::SetDlgItemText(dlg->m_hWnd, EDIT_AA ,str ); 

		//lineseries1.AddXY((float)i*Sample_Vel_Time*0.001,Posi_Angle_Arm,NULL,0);
		lineseries2.AddXY((float)i*Sample_Vel_Time*0.001,Vel_Real_SA,NULL,0);
		

		//exe_time_Vel=1000*(stop_t.QuadPart-start_t.QuadPart)/freq.QuadPart;
		strTime.Format(_T("%8.5f"),exe_time_Vel);//窗口显示实时采样周期

		::SetDlgItemText(dlg->m_hWnd, IDC_EDIT_LOOP ,strTime );  
		g_mutex.Unlock();
		i++;
	}
	return;
}
//多媒体定时器的回调函数
VOID  CALLBACK OnTimeFunc(UINT wTimerID,UINT msg,DWORD dwUser,DWORD dw1,DWORD dw2)
{
	CString str1;
	CString str2;
	CString str_i;
	LONG freq_Time=0;
	double Tmp_Motor_Pos=0;
	double Tmp_Motor_Pos1=0;
	Cmotortest20151013Dlg *dlg=(Cmotortest20151013Dlg*)g_pDlg;
	g_mutex.Lock();
	QueryPerformanceFrequency(&freq);
	freq_Time=freq.QuadPart;
	APS_get_position_f(0, &Tmp_Motor_Pos);
	APS_get_position_f(1, &Tmp_Motor_Pos1);
	Posi_Angle_Arm=Tmp_Motor_Pos*Pul_To_Degree ;
	Posi_Angle_Shoul=Tmp_Motor_Pos1*Pul_To_Degree ;
	//将得到的角度值赋值传给画图窗口需要使用到的中间变量
	dlg->Arm_Pos_Dlg=Posi_Angle_Arm;
	dlg->Shoul_Pos_Dlg=Posi_Angle_Shoul;
	//输出速度
	if (AS_i==3)
	{
		AS_i=0;
	}
	if (Timer_i==2)
	{
		Timer_i=0;
	}
	m_Pos_A[AS_i]=Posi_Angle_Arm;//Posi_Angle_Arm为返回的肘部角度值
	m_Pos_S[AS_i]=Posi_Angle_Shoul;//Posi_Angle_Shoul为返回的肩部角度值
	//有位置求速度的算法
	switch (AS_i)
	{
	case 0:
		(m_Pos_A[1]==0)?Vel_Real_AA=0:
						Vel_Real_AA=(m_Pos_A[0]-m_Pos_A[1])/(Sample_Vel_Time*0.001*2);
		(m_Pos_S[1]==0)?Vel_Real_SA=0:
						Vel_Real_SA=(m_Pos_S[0]-m_Pos_S[1])/(Sample_Vel_Time*0.001*2);
		break;
	case 1:
		(m_Pos_A[2]==0)?Vel_Real_AA=0:
						Vel_Real_AA=(m_Pos_A[1]-m_Pos_A[2])/(Sample_Vel_Time*0.001*2);
		(m_Pos_S[2]==0)?Vel_Real_SA=0:
			            Vel_Real_SA=(m_Pos_S[1]-m_Pos_S[2])/(Sample_Vel_Time*0.001*2);
		break;
	case 2:
		Vel_Real_AA=(m_Pos_A[2]-m_Pos_A[0])/(Sample_Vel_Time*0.001*2);
		Vel_Real_SA=(m_Pos_S[2]-m_Pos_S[0])/(Sample_Vel_Time*0.001*2);
		break;
	}
	//将得到的速度值赋值传给画图窗口需要使用到的中间变量
	dlg->Arm_Vel_Dlg=Vel_Real_AA;
	dlg->Shoul_Vel_Dlg=Vel_Real_SA;
	///////////////////
	QueryPerformanceCounter(&start_t);
	Timer_Out[Timer_i]=start_t.QuadPart;
	if (Timer_i==0)
	{
		exe_time_Vel=1000*(Timer_Out[0]-Timer_Out[1])/freq_Time;
	}
	else
	{
	exe_time_Vel=1000*(Timer_Out[1]-Timer_Out[0])/freq_Time;
	}
	
	APS_get_feedback_velocity_f(2, &Motor_Statu_Vel);
	APS_get_feedback_velocity_f(3, &Motor_Statu_Vel1);

	Vel_Arm_Motor.push_back(Motor_Statu_Vel/0.045);
	Vel_Shoul_Motor.push_back(Motor_Statu_Vel1/0.045);
	Pos_Arm_Array.push_back(Posi_Angle_Arm);
	Pos_Shoul_Array.push_back(Posi_Angle_Shoul);
	Vel_Arm_Array.push_back(Vel_Real_AA);
	Vel_Shoul_Array.push_back(Vel_Real_SA);
	Time_Array.push_back(exe_time_Vel);

	if (Vel_i>2000)
	{
		Vel_i=0;
	}
	g_mutex.Unlock();
   Vel_i++;
   AS_i++;
   Timer_i++;
	return;
}
//多媒体定时器-用于绝对式编码器读取位移及速度，若不用绝对式编码器则不用
VOID Cmotortest20151013Dlg::Reci_Vel_Arm()
{
	TIMECAPS tc;
	bool com_statu=TRUE;
	// 利用函数timeGetDevCaps取出系统分辨率的取值范围, 如果无错则继续; 
	if (timeGetDevCaps(&tc,sizeof(TIMECAPS))==TIMERR_NOERROR)
	{
		wAccuracy = min(max(tc.wPeriodMin,1),tc.wPeriodMax);//分辨率的值不能超出系统的取值范围 
		timeBeginPeriod(wAccuracy);// 调用timeBeginPeriod函数设置定时器的分辨率
		 // 设置定时器  
		//com_statu=onCommSet();//打开串口
		Mtimer_ID=timeSetEvent(Sample_Vel_Time,wAccuracy,(LPTIMECALLBACK)OnTimeFunc,DWORD(1),TIME_PERIODIC);
		OnBnClickedResetButton();//编码器数值归零
	}
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
					//MessageBox(_T("Initial ok !"));
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
	
	/*hThread1=CreateThread(NULL,
	0,
	(LPTHREAD_START_ROUTINE)ThreadFun1,
	this,
	0,
	&ThreadID1);*/
	//配置电机的轴参数
	//for(int i=0;i<4;i++)
	//{
	//	// 配置逻辑参数
	//	APS_set_axis_param(i,PRA_EL_LOGIC,0);
	//	APS_set_axis_param(i,PRA_ORG_LOGIC,0);
	//	APS_set_axis_param(i,PRA_ALM_LOGIC,1);
	//	APS_set_axis_param(i,PRA_EZ_LOGIC,0);
	//	APS_set_axis_param(i,PRA_INP_LOGIC,1);
	//	APS_set_board_param(i,PRB_EMG_LOGIC,1);

	//}
	//速度模式下调节电机PID参数
	APS_set_axis_param(2,PRA_KP_GAIN,18461);
	APS_set_axis_param(2,PRA_KI_GAIN,36103);
	APS_set_axis_param(2,PRA_KD_GAIN,18893);
	
	APS_set_axis_param(3,PRA_KP_GAIN,13214);
	APS_set_axis_param(3,PRA_KI_GAIN,40897);
	APS_set_axis_param(3,PRA_KD_GAIN,17085);

	APS_set_axis_param(0,PRA_PLS_IPT_MODE,4);//编码器输入脉冲
	APS_set_axis_param(0,PRA_ENCODER_DIR,0);//配置编码器正负使能
	APS_set_axis_param(1,PRA_PLS_IPT_MODE,4);
	APS_set_axis_param(1,PRA_ENCODER_DIR,0);//配置编码器正负使能
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
	////关闭各线程
	m_flag1=FALSE;
	m_flag2=FALSE;
	m_flag3=FALSE;
	m_flag4=FALSE;
	m_flag5=FALSE;
	OnBnClickedResetButton();//编码器数值归零
	m_comm.put_PortOpen(false);
	timeKillEvent(Mtimer_ID);
	timeEndPeriod(wAccuracy);
	KillTimer(0);
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

	_itoa_s(Ret,Buff,10);     	
	strcat_s(MsgStr,Buff);   

	if (Ret != 0)
	{
		MessageBox(str,NULL,MB_OK); 
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
	average =sum/ 10;//求平均值
	return(average);
	
}
void Cmotortest20151013Dlg::OnBnClickedOnButton()
{
	// TODO: 在此添加控件通知处理程序代码
	I32 Axis_ID = 0;
	if (mt_flag)
	{
		
		for (Axis_ID = 2; Axis_ID < 4; Axis_ID++)
		APS_set_servo_on(Axis_ID, ON);
		mt_flag=FALSE;
		SetDlgItemText(IDC_ON_BUTTON,_T("电机关"));
	} 
	else
	{
		ptp[0]=0;
		ptp[1]=0;
		for (Axis_ID = 2; Axis_ID < 4; Axis_ID++)
		APS_set_servo_on(Axis_ID, OFF);
		mt_flag=TRUE;
		SetDlgItemText(IDC_ON_BUTTON,_T("电机开"));
	}
	
}

//点对点测试按钮
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
//重置按钮
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
	if (Is_axis_err(0) == YES) return;
	if (Is_axis_err(1) == YES) return;
	//PID_Prio=FALSE;
	if (PID_flag)
	{
		if (Posi_Angle_Arm>5||Posi_Angle_Shoul>5)
		{
			MessageBox(_T("机器人不在初始位置"));
			return;
		}
		//OnClearAllSeries();
		Vel_Arm_Array.clear();
		Pos_Arm_Array.clear();
		Vel_Shoul_Array.clear();
		Pos_Shoul_Array.clear();
		Time_Array.clear();
		RelaySwitchClose();
		SetTimer(3,50,NULL);
		SetDlgItemText(IDC_LeftShouder_BUTTON,_T("停止"));
		PID_flag=false;
	} 
	else
	{
		
		target_Pos_Arm=0;
		target_Pos_Shoul=0;
		Pos_Comd_Arm=0;
		Pos_Comd_Shoul=0;
		Pos_Comd_Motor=0;
		Pos_Comd_MS=0;
		target_Pos_Arm=0;
		FT_i=0;
		for (int i=0;i<3;i++)
		{
			Pos_Error[i]=0;
			Pos_Error_S[i]=0;
			Vel_Buffer[i]=0;
			Vel_Buffer_S[i]=0;
		}
		OnBnClickedVelStop();
		SetDlgItemText(IDC_LeftShouder_BUTTON,_T("摸左肩"));
		PID_flag=true;
	}
	/*LeftshouderMove();
	ptp[0]=1;*/
}

//摸右耳的消息处理函数
void Cmotortest20151013Dlg::OnBnClickedRightearButton()
{
	// TODO: 在此添加控件通知处理程序代码

	if( Is_CARD_INITIAL() == NO ) return;
	if (Is_axis_err(0) == YES) return;
	if (Is_axis_err(1) == YES) return;
	//PID_Prio=FALSE;
	if (PID_flag)
	{
		if (Posi_Angle_Arm>5||Posi_Angle_Shoul>5)
		{
			MessageBox(_T("机器人不在初始位置"));
			return;
		}
		Vel_Arm_Array.clear();
		Pos_Arm_Array.clear();
		Vel_Shoul_Array.clear();
		Pos_Shoul_Array.clear();
		Time_Array.clear();
		//OnClearAllSeries();
		RelaySwitchClose();
		SetTimer(2,50,NULL);
		SetDlgItemText(IDC_RightEar_BUTTON,_T("停止"));
		PID_flag=false;
	} 
	else
	{
		
		target_Pos_Arm=0;
		target_Pos_Shoul=0;
		Pos_Comd_Arm=0;
		Pos_Comd_Shoul=0;
		Pos_Comd_Motor=0;
		Pos_Comd_MS=0;
		target_Pos_Arm=0;
		FT_i=0;
		for (int i=0;i<3;i++)
		{
			Pos_Error[i]=0;
			Pos_Error_S[i]=0;
			Vel_Buffer[i]=0;
			Vel_Buffer_S[i]=0;
		}
		OnBnClickedVelStop();
		SetDlgItemText(IDC_RightEar_BUTTON,_T("摸耳朵"));
		PID_flag=true;
	}
	//RightearMove();
	//ptp[1]=1;
}
void Cmotortest20151013Dlg::OnBnClickedLearnbButton()
{
	// TODO: 在此添加控件通知处理程序代码
	OnClearAllSeries();
	hThread2=CreateThread(NULL,
	0,
	(LPTHREAD_START_ROUTINE)ThreadFun2,
	this,
	0,
	&ThreadID2);
	GetDlgItem(IDC_LearnE_BUTTON2)->EnableWindow(TRUE);
	GetDlgItem(IDC_LearnB_BUTTON)->EnableWindow(FALSE);
	times=0;
	m_flag3=FALSE;
}


void Cmotortest20151013Dlg::OnBnClickedLearneButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_flag2=FALSE;
	double Sum_A_W=0;
	double Sum_A_T=0;
	double Sum_B_W=0;
	double Sum_B_T=0;
	double Aver_A_W=0;
	double Aver_A_T=0;
	double Aver_B_T=0;
	double Aver_B_W=0;
	CString str1;
	CString str2;
	CString str3;
	CString str4;
	CString str5;
	CString str6;
	for(int i=0;i<10;i++)
	{
		Sum_A_T+=Max_A_T[i];
		Sum_A_W+=Max_A_W[i];
		Sum_B_W+=Max_B_W[i];
		Sum_B_T+=Max_B_T[i];
	}
	Aver_A_W=Sum_A_W/10;
	Aver_A_T=Sum_A_T/10;
	Aver_B_W=Sum_B_W/10;
	Aver_B_T=Sum_B_T/10;
	Range_EMG_A=(Aver_A_W+Aver_A_T)/2;
	Range_EMG_B=(Aver_B_W+Aver_B_T)/2;
	str1.Format(_T("%8.3f"), Aver_A_W);
	str2.Format(_T("%8.3f"),Aver_A_T);
	str3.Format(_T("%8.3f"), Aver_B_W);
	str4.Format(_T("%8.3f"),Aver_B_T);
	str5.Format(_T("%8.3f"), Range_EMG_A);
	str6.Format(_T("%8.3f"),Range_EMG_B);
	SetDlgItemText(EDIT_EMG1_UPPER, str1);
	SetDlgItemText(EDIT_EMG1_DOWN, str2);
	SetDlgItemText(EDIT_EMG2_UPPER, str3);
	SetDlgItemText(EDIT_EMG2_DOWN, str4);
	SetDlgItemText(EDIT_EMG1_RANGE, str5);
	SetDlgItemText(EDIT_EMG2_RANGE, str6);
	/*GetDlgItem(IDC_LearnE_BUTTON2)->EnableWindow(FALSE);
	GetDlgItem(IDC_LearnB_BUTTON)->EnableWindow(TRUE);*/

}


void Cmotortest20151013Dlg::OnBnClickedMButton()
{
	// TODO: 在此添加控件通知处理程序代码
	hThread3=CreateThread(NULL,
		0,
		(LPTHREAD_START_ROUTINE)ThreadFun3,
		this,
		0,
&ThreadID3);

}


void Cmotortest20151013Dlg::OnBnClickedRelayclose()
{
	// TODO: 在此添加控件通知处理程序代码
	if (relay_flag)
	{
	RelaySwitchClose();//关闭继电器
	relay_flag=FALSE;
	SetDlgItemText(IDC_RelayClose,_T("离合松开"));
	} 
	else
	{
		uInt8       open[2]={1,1};
		RelaySwitchOpen();
		relay_flag=TRUE;
		SetDlgItemText(IDC_RelayClose,_T("离合吸合"));
	}
}
//六维力传感器参数对话框
void Cmotortest20151013Dlg::OnToolsSix()
{
	// TODO: 在此添加命令处理程序代码
	CSixAxisSensorDlg *m_SixAxisSensor=new CSixAxisSensorDlg;
	m_SixAxisSensor->Create(IDD_SIXAXIS_DIALOG,this);
	m_SixAxisSensor->ShowWindow(SW_SHOW);
	
}

//拉力传感器参数对话框
void Cmotortest20151013Dlg::OnToolsEncoder()
{
	// TODO: 在此添加命令处理程序代码
	CForceChartDlg *m_ForceChart=new CForceChartDlg;
	m_ForceChart->Create(IDD_ForceChart_DIALOG,this);
	m_ForceChart->ShowWindow(SW_SHOW);
}

//速度模式-电机1-启动函数
void Cmotortest20151013Dlg::OnBnClickedVelMot1()
{
	// TODO: 在此添加控件通知处理程序代码
	if( Is_CARD_INITIAL() == NO ) return;
	if (Is_axis_err(0) == YES) return;
	UpdateData(TRUE);
	F64 speed_1 =  m_VelArm;

	APS_set_axis_param_f( 2, PRA_STP_DEC, 10000.0 );
	APS_set_axis_param_f( 2, PRA_CURVE,      0.5 ); //Set acceleration rate
	APS_set_axis_param_f( 2, PRA_ACC,    50000.0 ); //Set acceleration rate
	APS_set_axis_param_f( 2, PRA_DEC,    50000.0 ); //Set deceleration rate

	//Check servo on or not
	if( !( ( APS_motion_io_status( 2 ) >> MIO_SVON ) & 1 ) )  
	{
		APS_set_servo_on( 2, ON ); 
		Sleep( 500 ); // Wait stable.
	}
	if (speed_1>0)
	{
		APS_vel( 2, 0, speed_1, 0 );
	} 
	else
	{
		APS_vel( 2, 1, -speed_1, 0 );
	}
	 
}

//速度模式-电机2-启动函数
void Cmotortest20151013Dlg::OnBnClickedVelMot2()
{
	// TODO: 在此添加控件通知处理程序代码

	if( Is_CARD_INITIAL() == NO ) return;
	if (Is_axis_err(1) == YES) return;
	UpdateData(TRUE);
	F64 speed_1 =  m_VelShoul;

	APS_set_axis_param_f( 3, PRA_STP_DEC, 10000.0 );
	APS_set_axis_param_f( 3, PRA_CURVE,      0.5 ); //Set acceleration rate
	APS_set_axis_param_f( 3, PRA_ACC,    50000.0 ); //Set acceleration rate
	APS_set_axis_param_f( 3, PRA_DEC,    50000.0 ); //Set deceleration rate

	//Check servo on or not
	if( !( ( APS_motion_io_status( 3 ) >> MIO_SVON ) & 1 ) )  
	{
		APS_set_servo_on( 3, ON ); 
		//Sleep( 500 ); // Wait stable.
	}
	if (speed_1>0)
	{
		APS_vel( 3, 0, speed_1, 0 ); 
	}
	else
	{
		APS_vel( 3, 1, -speed_1, 0 ); 
	}
	Reci_Vel_Arm();
}

//速度模式-电机-停止函数
void Cmotortest20151013Dlg::OnBnClickedVelStop()
{
	// TODO: 在此添加控件通知处理程序代码
	if( Is_CARD_INITIAL() == NO ) return;
	if (Is_axis_err(0) == YES) return;
	if (Is_axis_err(1) == YES) return;
	APS_set_axis_param_f( 0, PRA_STP_DEC, 50000.0 );
	APS_set_axis_param_f( 1, PRA_STP_DEC, 50000.0 );
	APS_stop_move( 2);
	APS_stop_move( 3 );
	APS_set_servo_on(2, OFF);
	APS_set_servo_on(3 ,OFF);
	RelaySwitchOpen();//关闭离合器
	KillTimer(0);
	KillTimer(1);
	KillTimer(2);
	KillTimer(3);
}


void Cmotortest20151013Dlg::OnStartWithSensor()
{
	// TODO: 在此添加控件通知处理程序代码
	if( Is_CARD_INITIAL() == NO ) return;
	if (Is_axis_err(0) == YES) return;
	if (Is_axis_err(1) == YES) return;
	SetTimer(0,100,NULL);
	
}


void Cmotortest20151013Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CString m_sixFt(" ");
	double  sixFt(0);		
	CSeries lineseries1;
	CSeries lineseries2;
	CSeries lineseries3;
	CSeries lineseries4;
	CSeries lineseries5;
	CString str(" ");
	CString str1(" ");
	CString str2;
    CString str3;
	CString str4;
	CString str5;
	double  Pos_Settimer_Arm(0);
	double  Pos_Settimer_Arm1(0);
	//定义存储开始于结束的时间的变量
	double exe_time;
	CString str6;
	CString str7;
	CString strTime1;
	//////////////////////////////////////////////////////////////////////////
	LARGE_INTEGER freq_Timer;
	LARGE_INTEGER start_Timer_t,stop_Timer_t;
	QueryPerformanceFrequency(&freq_Timer);
	QueryPerformanceCounter(&start_Timer_t);
	F64 ACC_1=8000000;
	GetDlgItemText(EDIT_FTT_Z,m_sixFt);//获取当前六维力传感器FZ数值
	sixFt=atof(m_sixFt);
	F64 speed_1 =  327*fabs(sixFt);//fabs()-求得浮点数绝对值
	F64 speed_2 =speed_1-0.1*(speed_1-m_VEROR);//加阻尼器
	switch (nIDEvent)
	{
	case 0:	//六轴运动模式//
			if( Is_CARD_INITIAL() == NO ) return;
			if (Is_axis_err(1) == YES) return;
			//绘图/////////////
			lineseries1=(CSeries)m_FTVEL.Series(0);
			lineseries2=(CSeries)m_FTVEL.Series(1);
			lineseries1.AddXY(FT_i,m_VEROR1-tmp,NULL,0);
			APS_set_axis_param_f( 1, PRA_STP_DEC, 10000.0 );
			APS_set_axis_param_f( 1, PRA_CURVE,      0.5 ); //Set acceleration rate
			APS_set_axis_param_f( 1, PRA_ACC,    ACC_1 ); //Set acceleration rate
			APS_set_axis_param_f( 1, PRA_DEC,    ACC_1 ); //Set deceleration rate

			//Check servo on or not
			if( !( ( APS_motion_io_status( 1 ) >> MIO_SVON ) & 1 ) )  
			{
				APS_set_servo_on( 1, ON ); 
				//Sleep( 500 ); // Wait stable.
			}

			APS_vel( 1, 0, speed_2, 0 ); 
			APS_get_feedback_velocity_f(1, &tmp);//反馈速度显示
			str.Format("%f",tmp);
			str1.Format("%f",speed_2);
			SetDlgItemText(EDIT_VEL_REA, str );
			SetDlgItemText(EDIT_VEL_STD, str1 );
			m_VEROR=speed_1;
			m_VEROR1=speed_2;
			FT_i++;
			UpdateData (TRUE);

		break;
	case 1://被动位置模式调试
		UpdateData(TRUE);
		g_mutex.Lock();
		Pos_Settimer_Arm=Posi_Angle_Arm;
		g_mutex.Unlock();
		if (FT_i<=500)
			{
				if (FT_i==0)
				 {
					 APS_set_axis_param_f( 2, PRA_STP_DEC, 10000.0 );
					 APS_set_axis_param_f( 2, PRA_CURVE,      0.5 ); //Set acceleration rate
					 APS_set_axis_param_f( 2, PRA_ACC,    400000.0 ); //Set acceleration rate
					 APS_set_axis_param_f( 2, PRA_DEC,    400000.0 ); //Set deceleration rate
					//Check servo on or not
					if( !( ( APS_motion_io_status( 2) >> MIO_SVON ) & 1 ) )  
					{
						APS_set_servo_on( 2, ON ); 
					}
					APS_vel(2, 0, 0, 0 );
				 }
				else
				 {
					if (FT_i<4)
					 {
						Vel_Buffer[(int)FT_i]=Pos_Settimer_Arm;
						target_Pos_Arm=Pos_Settimer_Arm;
					 }
					else
					 {
						Vel_Buffer[0]=Vel_Buffer[1];
						Vel_Buffer[1]=Vel_Buffer[2];
						Vel_Buffer[2]=Vel_Buffer[3];
						Vel_Buffer[3]=Pos_Settimer_Arm;
						target_Pos_Arm=(Vel_Buffer[0]+Vel_Buffer[1]+Vel_Buffer[2]+Vel_Buffer[3])/4;
					 }
					Pos_Comd=100*sin(((double)FT_i)/500*M_PI);
					Pos_Error[0]=Pos_Comd-target_Pos_Arm;;
					Pos_PID=Pos_PID+KP_E*(Pos_Error[0]-Pos_Error[1])+KI_E*Pos_Error[0]+KD_E*(Pos_Error[0]-2*Pos_Error[1]+Pos_Error[2]);
					Pos_Error[2]=Pos_Error[1];
					Pos_Error[1]=Pos_Error[0];
					Pos_Comd_Motor=(Pos_PID+FF_E)*625;
					APS_set_axis_param_f( 2, PRA_STP_DEC, 10000.0 );
					APS_set_axis_param_f( 2, PRA_CURVE,      0.5 ); //Set acceleration rate
					APS_set_axis_param_f( 2, PRA_ACC,    400000.0 ); //Set acceleration rate
					APS_set_axis_param_f( 2, PRA_DEC,    400000.0 ); //Set deceleration rate

					//Check servo on or not
					if( !( ( APS_motion_io_status( 2 ) >> MIO_SVON ) & 1 ) )  
					{
						APS_set_servo_on( 2, ON ); 
					}
					/*if (Pos_Comd_Motor>Value_Vel_RightEar)
					{
					Pos_Comd_Motor=Value_Vel_RightEar/10;
					}*/
					if (Pos_Comd_Motor>=0)
						APS_vel(2, 0,Pos_Comd_Motor, 0 );
					else
						APS_vel(2, 1, -1*Pos_Comd_Motor, 0 );
				}
				if (Posi_Angle_Arm>20)
				{
					PID_Prio=TRUE;
				}
					APS_get_feedback_velocity_f(2, &tmp);
					lineseries1=(CSeries)m_FTVEL.Series(0);
					lineseries2=(CSeries)m_FTVEL.Series(1);
					lineseries3=(CSeries)m_FTVEL.Series(2);
					lineseries4=(CSeries)m_FTVEL.Series(3);
					lineseries5=(CSeries)m_FTVEL.Series(4);
					lineseries1.AddXY((float)FT_i*T_C/1000,Pos_Comd,NULL,0);
					lineseries2.AddXY((float)FT_i*T_C/1000,target_Pos_Arm,NULL,0);
					lineseries3.AddXY((float)FT_i*T_C/1000,Pos_Comd_Motor/Unit_Convert,NULL,0);
					lineseries4.AddXY((float)FT_i*T_C/1000,Pos_Error[0],NULL,0);
					lineseries5.AddXY((float)FT_i*T_C/1000,tmp/Unit_Convert,NULL,0);
					QueryPerformanceCounter(&stop_Timer_t);
					exe_time=1000*(stop_Timer_t.QuadPart-start_Timer_t.QuadPart)/freq_Timer.QuadPart;
					strTime1.Format(_T("%f"),exe_time);
					SetDlgItemText(IDC_EDIT_CLOOP, strTime1);
					FT_i++;

			}
			else
			{
				OnWarning();
				OnBnClickedVelStop();
			}
		break;
	case 2://位置模式-摸耳朵
		Pos_Mode_Motor(75,10);
		QueryPerformanceCounter(&stop_Timer_t);
		exe_time=50+1000*(stop_Timer_t.QuadPart-start_Timer_t.QuadPart)/freq_Timer.QuadPart;
		strTime1.Format(_T("%f"),exe_time);
		SetDlgItemText(IDC_EDIT_CLOOP, strTime1);
		break;
	case 3://位置模式-摸左肩
		Pos_Mode_Motor(65,57);
		break;
}
	
	CDialogEx::OnTimer(nIDEvent);
}


BOOL Cmotortest20151013Dlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (pCopyDataStruct->cbData>0)
	{
		char recvData[20]={0};
		strncpy_s(recvData,(char*)pCopyDataStruct->lpData,pCopyDataStruct->cbData);
		SetDlgItemText(EDIT_FTT_Z, (char*)recvData );
	}
	
	return CDialogEx::OnCopyData(pWnd, pCopyDataStruct);
}
BOOL Cmotortest20151013Dlg::DAQ_EMG(CString changeDeviceName, short changeFirstChannel)
{
	TaskHandle  taskHandle = 0;
	int32       read;
	int status =0;
	CString cstrChannelList;
	cstrChannelList.Format( _T("/ai%d"), changeFirstChannel );
	status =DAQmxCreateTask("EMGtask", &taskHandle);
	status =DAQmxCreateAIVoltageChan(taskHandle, changeDeviceName+cstrChannelList, "EMGtaskChannel", DAQmx_Val_RSE, 0, 5, DAQmx_Val_Volts, NULL);
	//UpdateData(TRUE);
	status =DAQmxCfgSampClkTiming(taskHandle, "OboardClock", 2000, DAQmx_Val_Rising, DAQmx_Val_ContSamps, 20);
	//UpdateData(TRUE);
	status =DAQmxStartTask(taskHandle);
	status =DAQmxReadAnalogF64(taskHandle, 10, 0.2, DAQmx_Val_GroupByChannel, EMG, 40, &read, NULL);
	status =DAQmxStopTask(taskHandle);
	status =DAQmxClearTask(taskHandle);
	return TRUE;
}
BOOL Cmotortest20151013Dlg::DAQ_EAF(CString changeDeviceName, short changeFirstChannel,short changeLastChannel)
{
	TaskHandle  taskHandle = 0;
	int32       read;
		int status =0;
	CString cstrChannelList;
	cstrChannelList.Format( _T("/ai%d:%d"), changeFirstChannel,changeLastChannel );
	status =DAQmxCreateTask("AAencoderTask", &taskHandle);
	status =DAQmxCreateAIVoltageChan(taskHandle, changeDeviceName+cstrChannelList, "AAencoderChannel", DAQmx_Val_RSE, 0, 5, DAQmx_Val_Volts, NULL);
	//UpdateData(TRUE);
	status =DAQmxCfgSampClkTiming(taskHandle, "OnboardClock", 2000, DAQmx_Val_Rising, DAQmx_Val_ContSamps, 20);
	//UpdateData(TRUE);
	status =DAQmxStartTask(taskHandle);
	status =DAQmxReadAnalogF64(taskHandle, 10, 0.2, DAQmx_Val_GroupByScanNumber, EAFW, 40, &read, NULL);
	status =DAQmxStopTask(taskHandle);
	status =DAQmxClearTask(taskHandle);
	return TRUE;
}

void Cmotortest20151013Dlg::OndHomeButton()
{
	// TODO: 在此添加控件通知处理程序代码
	if( Is_CARD_INITIAL() == NO ) return;
	if (Is_axis_err(1) == YES) return;
	F64 speed_1 = 20000;
	

APS_set_axis_param_f( 3, PRA_STP_DEC, 10000.0 );
	APS_set_axis_param_f( 3, PRA_CURVE,      0.5 ); //Set acceleration rate
	APS_set_axis_param_f( 3, PRA_ACC,    50000.0 ); //Set acceleration rate
	APS_set_axis_param_f( 3, PRA_DEC,    50000.0 ); //Set deceleration rate

	//Check servo on or not
	if( !( ( APS_motion_io_status( 3 ) >> MIO_SVON ) & 1 ) )  
	{
		APS_set_servo_on( 3, ON ); 
		Sleep( 500 ); // Wait stable.
	}
	PID_Prio=TRUE;
	APS_vel( 3, 1, speed_1, 0 );
	
	

}

//清屏按钮的响应函数
void Cmotortest20151013Dlg::OnClearAllSeries()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int i=0;i<m_FTVEL.get_SeriesCount();i++)
	{
		((CSeries)m_FTVEL.Series(i)).Clear();
	}
	Posi_Angle_Arm=0;
	Vel_Real_AA=0;
	for (int i=0;i<4;i++)
	{
		Vel_Buffer[i]=0;
	}
	target_Pos_Arm=0;
	Pos_Comd_Motor=0;
	Pos_Error[0]=0;
	Pos_Error[1]=0;
	Pos_Error[2]=0;
	Pos_PID=0;
	AS_i=0;
	FT_i=0;
}


void Cmotortest20151013Dlg::OnPidControl()
{
	// TODO: 在此添加控件通知处理程序代码
	
	if( Is_CARD_INITIAL() == NO ) return;
	if (Is_axis_err(0) == YES) return;
	if (Is_axis_err(1) == YES) return;
	PID_Prio=FALSE;
	if (PID_flag)
	{
		
		if (Posi_Angle_Arm>13)
		{
		MessageBox(_T("机器人不在初始位置"));
		return;
		}
		OnClearAllSeries();
		Vel_Arm_Array.clear();
		Pos_Arm_Array.clear();
		Vel_Shoul_Array.clear();
		Pos_Shoul_Array.clear();
		Time_Array.clear();
		RelaySwitchClose();
		SetTimer(1,50,NULL);
		SetDlgItemText(BUTTON_PID_CONTROL,_T("停止"));
		PID_flag=false;
	} 
	else/*(PID_flag=false);*/
	{
		Pos_Comd=0;
		target_Pos_Arm=0;
		for (int i=0;i<3;i++)
		{
			Pos_Error[i]=0;
		}
		OnBnClickedVelStop();
		SetDlgItemText(BUTTON_PID_CONTROL,_T("PID运动"));
		PID_flag=true;
	}
	
}


BEGIN_EVENTSINK_MAP(Cmotortest20151013Dlg, CDialogEx)
	//ON_EVENT(Cmotortest20151013Dlg, IDC_MSCOMM1, 1, Cmotortest20151013Dlg::OnCommMscomm1, VTS_NONE)
END_EVENTSINK_MAP()

//采集ssi编码器的角度并输出速度
//void Cmotortest20151013Dlg::OnCommMscomm1()
//{
//	// TODO: 在此处添加消息处理程序代码
//	VARIANT variant_inp1;
//	COleSafeArray safearray_inp1;
//	LONG len1, k1; 
//	CString strtemp1;
//	CString strangle;
//	unsigned long BMQ_Val1;
//	if (m_comm.get_CommEvent() == 2)     //事件值为2表示接收缓冲区内有字符
//	{
//		variant_inp1 = m_comm.get_Input();   //读缓冲区
//		safearray_inp1 = variant_inp1;           //VARIANT型变量转换为ColeSafeArray型变量
//		len1 = safearray_inp1.GetOneDimSize(); //得到有效数据长度
//		for (k1 = 0; k1<len1; k1++)
//			safearray_inp1.GetElement(&k1, rxdata1 + k1);//转换为BYTE型数组
//		if (((rxdata1[0] & 0xFF) == 0xAA) && (((rxdata1[2] + rxdata1[3] + rxdata1[4] + rxdata1[5]) & 0xFF) == rxdata1[6]))              //数据帧校验通过
//		{
//			BMQ_Val1 = ((rxdata1[5] & 0xFF) | ((rxdata1[4] & 0xFF) << 8) | ((rxdata1[3] & 0xFF) << 16) | ((rxdata1[2] & 0xFF) << 24)) & 0xFFFFFFF; //数据拼接
//			g_mutex.Lock();
//			Posi_Angle_Arm = ((double)(BMQ_Val1 & 0xFFF) / 0xFFF)*360.0;    //将0x000-0xFFF的12位十六进制数据转换成0-360角度值
//		    g_mutex.Unlock();
//			//输出速度
//			if (AS_i==3)
//			{
//				AS_i=0;
//			}
//			g_mutex.Lock();
//			//m_A[AS_i]=Posi_Angle_Arm;//Posi_Angle_Arm为返回的角度值
//			g_mutex.Unlock();
//			switch (AS_i)
//			{
//			case 0:
//				if (m_AA[1]==0)
//				{
//					g_mutex.Lock();
//					Vel_Real_AA=0;
//					g_mutex.Unlock();
//				}
//				else
//				{
//					g_mutex.Lock();
//					Vel_Real_AA=(m_AA[0]-m_AA[1])/0.02;
//					g_mutex.Unlock();
//				}
//				break;
//			case 1:
//				if (m_AA[2]==0)
//				{
//					g_mutex.Lock();
//					Vel_Real_AA=(m_AA[1]-m_AA[0])/0.01;
//					g_mutex.Unlock();
//				}
//				else
//				{
//					g_mutex.Lock();
//					Vel_Real_AA=(m_AA[1]-m_AA[2])/0.02;
//					g_mutex.Unlock();
//				}
//
//				break;
//			case 2:
//				g_mutex.Lock();
//				Vel_Real_AA=(m_AA[2]-m_AA[0])/0.02;
//				g_mutex.Unlock();
//				break;
//			}
//			AS_i++;
//		}
//	}
	//UpdateData(FALSE);           //更新编辑框内容
//}
void Cmotortest20151013Dlg::OnRadioPosition()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_P2P_BUTTON4)->EnableWindow(TRUE);
	GetDlgItem(IDC_LeftShouder_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_RightEar_BUTTON)->EnableWindow(TRUE);
	////
	GetDlgItem(BUTTON_VEL_MOT1)->EnableWindow(FALSE);

	GetDlgItem(EDIT_VEL_MIX)->EnableWindow(FALSE);
	GetDlgItem(BUTTON_VEL_MOT2)->EnableWindow(FALSE);

	GetDlgItem(BUTTON_VEL_START)->EnableWindow(FALSE);

	GetDlgItem(BUTTON_PID_CONTROL)->EnableWindow(FALSE);
	GetDlgItem(BUTTON_PID_SAVE)->EnableWindow(FALSE);

	GetDlgItem(IDC_LearnB_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_M_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_LearnE_BUTTON2)->EnableWindow(FALSE);
}


void Cmotortest20151013Dlg::OnRadioVSingle()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_P2P_BUTTON4)->EnableWindow(FALSE);
	GetDlgItem(IDC_LeftShouder_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_RightEar_BUTTON)->EnableWindow(FALSE);

	GetDlgItem(BUTTON_VEL_MOT1)->EnableWindow(TRUE);

	GetDlgItem(EDIT_VEL_MIX)->EnableWindow(TRUE);
	GetDlgItem(BUTTON_VEL_MOT2)->EnableWindow(TRUE);

	GetDlgItem(BUTTON_VEL_START)->EnableWindow(FALSE);

	GetDlgItem(BUTTON_PID_CONTROL)->EnableWindow(FALSE);
	GetDlgItem(BUTTON_PID_SAVE)->EnableWindow(FALSE);

	GetDlgItem(IDC_LearnB_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_M_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_LearnE_BUTTON2)->EnableWindow(FALSE);

}


void Cmotortest20151013Dlg::OnRadioSix()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_P2P_BUTTON4)->EnableWindow(FALSE);
	GetDlgItem(IDC_LeftShouder_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_RightEar_BUTTON)->EnableWindow(FALSE);

	GetDlgItem(BUTTON_VEL_MOT1)->EnableWindow(FALSE);

	GetDlgItem(EDIT_VEL_MIX)->EnableWindow(FALSE);
	GetDlgItem(BUTTON_VEL_MOT2)->EnableWindow(FALSE);

	GetDlgItem(BUTTON_VEL_START)->EnableWindow(TRUE);

	GetDlgItem(BUTTON_PID_CONTROL)->EnableWindow(FALSE);
	GetDlgItem(BUTTON_PID_SAVE)->EnableWindow(FALSE);

	GetDlgItem(IDC_LearnB_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_M_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_LearnE_BUTTON2)->EnableWindow(FALSE);
}


void Cmotortest20151013Dlg::OnRadioPid()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_P2P_BUTTON4)->EnableWindow(FALSE);
	GetDlgItem(IDC_LeftShouder_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_RightEar_BUTTON)->EnableWindow(FALSE);

	GetDlgItem(BUTTON_VEL_MOT1)->EnableWindow(FALSE);

	GetDlgItem(EDIT_VEL_MIX)->EnableWindow(FALSE);
	GetDlgItem(BUTTON_VEL_MOT2)->EnableWindow(FALSE);

	GetDlgItem(BUTTON_VEL_START)->EnableWindow(FALSE);

	
	GetDlgItem(BUTTON_PID_CONTROL)->EnableWindow(TRUE);
	GetDlgItem(BUTTON_PID_SAVE)->EnableWindow(TRUE);

	GetDlgItem(IDC_LearnB_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_M_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_LearnE_BUTTON2)->EnableWindow(FALSE);
	
}


void Cmotortest20151013Dlg::OnRadioEmg()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_P2P_BUTTON4)->EnableWindow(FALSE);
	GetDlgItem(IDC_LeftShouder_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_RightEar_BUTTON)->EnableWindow(FALSE);

	GetDlgItem(BUTTON_VEL_MOT1)->EnableWindow(FALSE);

	GetDlgItem(EDIT_VEL_MIX)->EnableWindow(FALSE);
	GetDlgItem(BUTTON_VEL_MOT2)->EnableWindow(FALSE);

	GetDlgItem(BUTTON_VEL_START)->EnableWindow(FALSE);

	GetDlgItem(BUTTON_PID_SAVE)->EnableWindow(FALSE);
	GetDlgItem(BUTTON_PID_CONTROL)->EnableWindow(FALSE);

	GetDlgItem(IDC_LearnB_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_M_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_LearnE_BUTTON2)->EnableWindow(TRUE);
}
VOID Cmotortest20151013Dlg::OnWarning()
{
	if (Posi_Angle_Arm<0||Posi_Angle_Arm>85||tmp>200000)
	{
		GetDlgItem(IDC_STATIC_WARN)->ShowWindow(TRUE);
		CBitmap bitmap;
		bitmap.LoadBitmap(IDB_BITMAP2);
		CStatic *p=(CStatic*)GetDlgItem(IDC_STATIC_WARN);
		p->ModifyStyle(0xf,SS_BITMAP|SS_CENTERIMAGE);
		p->SetBitmap(bitmap);
	} 
	else
	{
		GetDlgItem(IDC_STATIC_WARN)->ShowWindow(FALSE);
	}
	
}

void Cmotortest20151013Dlg::OnButtonQuit()
{
	// TODO: 在此添加控件通知处理程序代码
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
	////关闭各线程
	m_flag1=FALSE;
	m_flag2=FALSE;
	m_flag3=FALSE;
	m_flag4=FALSE;
	m_flag5=FALSE;
	KillTimer(0);
	KillTimer(1);
	OnBnClickedResetButton();//编码器数值归零
	//m_comm.put_PortOpen(false);
	timeKillEvent(Mtimer_ID);
	timeEndPeriod(wAccuracy);
	CDialog::OnClose();
}
void Cmotortest20151013Dlg::Pos_Mode_Motor(double Pos_Angle_A,double Pos_Angle_S)
{
	double  Pos_Settimer_Arm(0);
	double  Pos_Settimer_Arm1(0);
	double FF_A=0;//3125;
	double Dead_band=1.0;
	static double Sign=0;
	double Time_S=0;
	//double FF_S=0;
	g_mutex.Lock();
	Pos_Settimer_Arm=Posi_Angle_Arm;
	Pos_Settimer_Arm1=Posi_Angle_Shoul;
	g_mutex.Unlock();
	Time_S=FT_i*0.05;
	if (FT_i<=500)
	{
		if (FT_i==0)
		{
			for (int i=2;i<4;i++)
			{
				APS_set_axis_param_f( i, PRA_STP_DEC, 10000.0 );
				APS_set_axis_param_f( i, PRA_CURVE,      0.5 ); //Set acceleration rate
				APS_set_axis_param_f( i, PRA_ACC,    300000.0 ); //Set acceleration rate
				APS_set_axis_param_f( i, PRA_DEC,    300000.0 ); //Set deceleration rate

				//Check servo on or not
				if( !( ( APS_motion_io_status( i ) >> MIO_SVON ) & 1 ) )  
				{
					APS_set_servo_on( i, ON ); 
				}
				APS_vel(i, 0, 0, 0 );
				APS_vel(i, 0, 0, 0 );
			}

		}
		else
		{
			if (FT_i<4)
			{
				Vel_Buffer[(int)FT_i]=Pos_Settimer_Arm;Vel_Buffer_S[(int)FT_i]=Pos_Settimer_Arm1;
				target_Pos_Arm=Pos_Settimer_Arm;target_Pos_Shoul=Pos_Settimer_Arm1;
			}
			else
			{
				Vel_Buffer[0]=Vel_Buffer[1];Vel_Buffer_S[0]=Vel_Buffer_S[1];
				Vel_Buffer[1]=Vel_Buffer[2];Vel_Buffer_S[1]=Vel_Buffer_S[2];
				Vel_Buffer[2]=Vel_Buffer[3];Vel_Buffer_S[2]=Vel_Buffer_S[3];
				Vel_Buffer[3]=Pos_Settimer_Arm;Vel_Buffer_S[3]=Pos_Settimer_Arm1;
				target_Pos_Arm=(Vel_Buffer[0]+Vel_Buffer[1]+Vel_Buffer[2]+Vel_Buffer[3])/4;
				target_Pos_Shoul=(Vel_Buffer_S[0]+Vel_Buffer_S[1]+Vel_Buffer_S[2]+Vel_Buffer_S[3])/4;
			}
			//肘部电机PID
			if (FT_i<=50)
			{
				Pos_Comd_Arm=Pos_Angle_A*pow(FT_i,2)/10000;
				Pos_Comd_Shoul=Pos_Angle_S*pow(FT_i,2)/10000;
			}
			if (FT_i>50 && FT_i<=100)
			{
				Pos_Comd_Arm=Pos_Angle_A*(FT_i/100-0.25);
				Pos_Comd_Shoul=Pos_Angle_S*(FT_i/100-0.25);
			}
			if (FT_i>100 && FT_i<=150)
			{
				Pos_Comd_Arm=Pos_Angle_A*(-0.25+FT_i/100-pow(FT_i-100,2)/10000);
				Pos_Comd_Shoul=Pos_Angle_S*(-0.25+FT_i/100-pow(FT_i-100,2)/10000);
			}
			if (FT_i>150 && FT_i<=200)
			{
				Pos_Comd_Arm=Pos_Angle_A;
				Pos_Comd_Shoul=Pos_Angle_S;
			}
			if (FT_i>200 && FT_i<=250)
			{
			Pos_Comd_Arm=Pos_Angle_A*(1-pow(FT_i-200,2)/10000);
			Pos_Comd_Shoul=Pos_Angle_S*(1-pow(FT_i-200,2)/10000);
			}
			if (FT_i>250 && FT_i<=300)
			{
			Pos_Comd_Arm=Pos_Angle_A*(3.25-FT_i/100);
			Pos_Comd_Shoul=Pos_Angle_S*(3.25-FT_i/100);
			}
			if (FT_i>300 && FT_i<=350)
			{
			Pos_Comd_Arm=Pos_Angle_A*(3.25-FT_i/100+pow(FT_i-300,2)/10000);
			Pos_Comd_Shoul=Pos_Angle_S*(3.25-FT_i/100+pow(FT_i-300,2)/10000);
			}

			if (FT_i>360)
			{
				Pos_Comd_Arm=0;
				Pos_Comd_Shoul=0;
			}
			//(FT_i<=300)?(Pos_Comd_Arm=Pos_Angle_A*sin(((double)FT_i)/300*M_PI)):(Pos_Comd_Arm=0);
			Pos_Error[0]=Pos_Comd_Arm-Pos_Settimer_Arm;
			if ((Pos_Error[0]>=Dead_band)&&(Pos_Error[1]<=Dead_band))
			{
				Sign=1;
			}
			else if((Pos_Error[0]<=-Dead_band)&&Pos_Error[1]>=-Dead_band)
			{
				Sign=-1;
			}
			Pos_PID=0;//Pos_PID+KP_A*(Pos_Error[0]-Pos_Error[1])+KI_A*Pos_Error[0]+0*(Pos_Error[0]-2*Pos_Error[1]+Pos_Error[2]);
			Pos_Error[2]=Pos_Error[1];
			Pos_Error[1]=Pos_Error[0];
			if (Time_S<=2.5)
			{
				Pos_Comd_Motor=(Pos_PID+Sign*FF_A)+(Time_S*Pos_Angle_A/12.5)*Unit_Convert;
			}
			if (Time_S>2.5&&Time_S<=5)
			{
				Pos_Comd_Motor=(Pos_PID+Sign*FF_A)+(Pos_Angle_A/5)*Unit_Convert;
			}
			if (Time_S>5&&Time_S<=7.5)
			{
				Pos_Comd_Motor=(Pos_PID+Sign*FF_A)+(Pos_Angle_A*(0.6-Time_S/12.5))*Unit_Convert;
			}
			if (Time_S>7.5&&Time_S<=10)
			{
				Pos_Comd_Motor=(Pos_PID+Sign*FF_A);
			}
			if (Time_S>10&&Time_S<=12.5)
			{
				Pos_Comd_Motor=(Pos_PID+Sign*FF_A)+(Pos_Angle_A*(-Time_S/12.5+0.8))*Unit_Convert;
			}
			if (Time_S>12.5&&Time_S<=15)
			{
				Pos_Comd_Motor=(Pos_PID+Sign*FF_A)-(Pos_Angle_A/5)*Unit_Convert;
			}
			if (Time_S>15&&Time_S<=17.5)
			{
				Pos_Comd_Motor=(Pos_PID+Sign*FF_A)+(Pos_Angle_A*(Time_S/12.5-1.4))*Unit_Convert;
			}
			//肩部电机PID
			
			Pos_Error_S[0]=Pos_Comd_Shoul-Pos_Settimer_Arm1;
			Pos_PID_S=Pos_PID_S+KP_S*(Pos_Error_S[0]-Pos_Error_S[1])+KI_S*Pos_Error_S[0]+0*(Pos_Error_S[0]-2*Pos_Error_S[1]+Pos_Error_S[2]);
			Pos_Error_S[2]=Pos_Error_S[1];
			Pos_Error_S[1]=Pos_Error_S[0];
			Pos_Comd_MS=Pos_PID_S+FF_S;

			APS_get_feedback_velocity_f(2, &Motor_Statu_Vel);
			APS_get_feedback_velocity_f(3, &Motor_Statu_Vel1);

			Vel_Arm_Motor.push_back(Motor_Statu_Vel/Unit_Convert);
			Vel_Shoul_Motor.push_back(Motor_Statu_Vel1/Unit_Convert);
			Pos_Arm_Array.push_back(target_Pos_Arm);
			Pos_Shoul_Array.push_back(target_Pos_Shoul);
			Vel_Arm_Array.push_back(Vel_Real_AA);
			Vel_Shoul_Array.push_back(Vel_Real_SA);
			Time_Array.push_back(exe_time_Vel);

			for (int i=2;i<4;i++)
			{
				APS_set_axis_param_f( i, PRA_STP_DEC, 10000.0 );
				APS_set_axis_param_f( i, PRA_CURVE,      0.5 ); //Set acceleration rate
				APS_set_axis_param_f( i, PRA_ACC,    300000.0 ); //Set acceleration rate
				APS_set_axis_param_f( i, PRA_DEC,    300000.0 ); //Set deceleration rate

				//Check servo on or not
				if( !( ( APS_motion_io_status( i ) >> MIO_SVON ) & 1 ) )  
				{
					APS_set_servo_on( i, ON ); 
				}
				APS_vel(i, 0, 0, 0 );
				APS_vel(i, 0, 0, 0 );
			}
			/*if (Pos_Comd_MS>Value_Vel_RightEar)
			{
				Pos_Comd_MS=Value_Vel_RightEar/10;
			}*/
			/*if (Pos_Comd_MS>=0)
			APS_vel(3, 0,Pos_Comd_MS, 0 );
			else
			APS_vel(3, 1, -1*Pos_Comd_MS, 0 );*/
			/*if (Pos_Comd_Motor>Value_Vel_RightEar)
			{
				Pos_Comd_Motor=Value_Vel_RightEar/10;
			}*/
			if (Pos_Comd_Motor>=0)
				APS_vel(2, 0,Pos_Comd_Motor, 0 );
			else
				APS_vel(2, 1, -1*Pos_Comd_Motor, 0 );

		}
		
		FT_i++;
	}
	else
	{
		OnBnClickedVelStop();
		return;
	}
}
//保存PID参数
void Cmotortest20151013Dlg::OnPidSaveData()
{
	// TODO: 在此添加控件通知处理程序代码
	CString KP(" ");
	CString KI(" ");
	CString KD(" ");
	CString FF(" ");
	CString str1(" ");
	CString str2(" ");
	CSeries lineseries1;
	CSeries lineseries2;
	CSeries lineseries3;
	lineseries1=(CSeries)m_FTVEL.Series(0);
	lineseries2=(CSeries)m_FTVEL.Series(1);
	lineseries3=(CSeries)m_FTVEL.Series(2);
	UpdateData(TRUE);
	GetDlgItemText(EDIT_INPUT_KP,KP);//获取对话框KP数值
	GetDlgItemText(EDIT_INPUT_KI,KI);//获取对话框KI数值
	GetDlgItemText(EDIT_INPUT_KD,KD);//获取对话框KD数值
	GetDlgItemText(EDIT_FEEDFORD,FF);//获取对话框KD数值
	CString szFilePath="C:\\VelPos.txt";
	CStdioFile file;
	CString str_i;
	file.Open(szFilePath,CFile::modeCreate|CFile::modeWrite);
	file.WriteString("t \t");
	file.WriteString("_t \t\t");
	file.WriteString("Vel \t\t");
	file.WriteString("Pos \r\n");
	for (int i=0;i< Vel_Arm_Array.size();i++)
	{
		strTime.Format(_T("%8.5f"),Time_Array[i]);//窗口显示实时采样周期
		str1.Format( _T( "%f"), Vel_Arm_Array[i] );//窗口显示实时速度
		str2.Format( _T( "%f"), Pos_Arm_Array[i] );//窗口显示实时位置
		str_i.Format("%d",i);
		if (i==0)
		{
			lineseries3.AddXY((float)i*Sample_Vel_Time*0.001,0,NULL,0);
		}
		else
		{
			lineseries3.AddXY((float)i*Sample_Vel_Time*0.001,Time_Array[i],NULL,0);
		}
		lineseries1.AddXY((float)i*Sample_Vel_Time*0.001,Pos_Arm_Array[i],NULL,0);
		lineseries2.AddXY((float)i*Sample_Vel_Time*0.001,Vel_Arm_Array[i],NULL,0);
		
		
		file.WriteString(str_i +"\t");
		file.WriteString(strTime+"ms\t");
		file.WriteString(str1+"\t");
		file.WriteString(str2);
		file.WriteString("\r\n");
	}
    file.Close();

}


void Cmotortest20151013Dlg::OnBnVelMode()
{
	// TODO: 在此添加控件通知处理程序代码

}


void Cmotortest20151013Dlg::OnEquipmentsArmencoder()
{
	// TODO: 在此添加命令处理程序代码
	CArmEncoderDlg *m_ArmEncoder=new CArmEncoderDlg;
	m_ArmEncoder->Create(DIALOG_ARM_ENCODER,this);
	m_ArmEncoder->ShowWindow(SW_SHOW);
	//delete m_ArmEncoder;
}


void Cmotortest20151013Dlg::OnMenuShoulE()
{
	// TODO: 在此添加命令处理程序代码
	CShoulderEncoderDlg *m_ShoulEncoder=new CShoulderEncoderDlg;
	m_ShoulEncoder->Create(DIALOG_SHOUL_ENCODER,this);
	m_ShoulEncoder->ShowWindow(SW_SHOW);
	//delete m_ShoulEncoder;
}
