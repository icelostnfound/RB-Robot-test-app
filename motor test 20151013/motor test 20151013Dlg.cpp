
// motor test 20151013Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "motor test 20151013.h"
#include "motor test 20151013Dlg.h"
#include "ForceChartDlg.h"
#include "LoginDlg.h"
#include "SixAxisSensorDlg.h"
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
#define Unit_Convert 3413.33    //60*1.25*16384/360 ����ϵ����60-�����ȣ�16384-һȦ��������2pi-Բ��
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
VOID ThreadFun1(LPVOID pParam);
VOID ThreadFun2(LPVOID pParam);
VOID ThreadFun3(LPVOID pParam);
VOID ThreadFun4(LPVOID pParam);
VOID ThreadFun5(LPVOID pParam);
BOOL m_flag1 = TRUE;
BOOL m_flag2 = TRUE;
BOOL m_flag3 = TRUE;
BOOL m_flag4 = TRUE;
BOOL m_flag5 = TRUE;
BOOL PID_flag=true;
int times=0;
int times3=0;
double sum1;
double sum2;
double target1=0;
double target2=0;
double G[100]={0};
double R[100]={0};
double EMG[10]={0};
double EAFW[100]={0};
double EAFR[5]={0};
F64 tmp = 0;
double Posi_Angle_Arm=0;//
F64 m_VEROR=0;
F64 m_VEROR1=0;
double  m_AA[3]={0};
double  m_SA[3]={0};
int AS_i=0;
double Vel_Real_AA=0;
double init_P=0;
F64 Vel_Real_SA=0;
F64 Vel_Buffer[4]={0};
double Pos_Comd_Motor=0;
double Pos_Comd_Arm=0;
double Pos_Error[3]={0};
double Pos_PID=0;
double T_C=50;//�������ڣ���λ����
F64 Vel_Average=0;
BYTE rxdata1[1024]={0};
//int Sum_i=0;
CForceChartDlg m_ForceChartDlg;

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// Cmotortest20151013Dlg �Ի���



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
END_MESSAGE_MAP()


// Cmotortest20151013Dlg ��Ϣ�������
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
   if( !(strUser==_T("admin") && strPasswd==_T("admin")) )   //����û�/���벻��admin/admin�Ļ����򱨴���ʾ�������룬ֱ����ȷΪֹ;
   {
    if(IDOK==MessageBox(_T("�û�ID���������\n  ���������룡")))
     continue;
    else
     return FALSE;
   }
   else
    break;
  }
  
 }/////-while-end
}
BOOL Cmotortest20151013Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	
	OnBnClickedInitialButton();
	mt_flag=TRUE;
	relay_flag=TRUE;
	bool com_statu=TRUE;
	com_statu=onCommSet();//�򿪴���
	for(int i=0;i<2;i++)
		ptp[i]=0;
	FT_i=0;
	RelaySwitchOpen();//�����̵���
	//�û���¼ Login();
	//�����߳�1-������״̬������ʵʱ��ⷵ�ػ����Ƿ�����
	hThread1=CreateThread(NULL,
		0,
		(LPTHREAD_START_ROUTINE)ThreadFun1,
		this,
		0,
&ThreadID1);
	//�����߳�4-ʵʱ���������λ�ƴ�����������������������ֵ
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
	
	//���μ�����-�������ݲɼ��߳�////////////////////////////////////
	//m_ForceChartDlg.Cramps();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void Cmotortest20151013Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR Cmotortest20151013Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
//���һ���ַ��ǲ���ʮ�������ַ������Ƿ�����Ӧ��ֵ�����򷵻�0x10
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
//��һ���ַ�����Ϊʮ�����ƴ�ת��Ϊһ���ֽ����飬�ֽڼ���ÿո�ָ�������ת������ֽ����鳤�ȣ�ͬʱ�ֽ����鳤���Զ����á�
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
		m_comm.put_CommPort(5); //ѡ��Ĭ�ϴ���com5 
		m_comm.put_InputMode(1);   //�������뷽ʽΪ�����Ʒ�ʽ 
		m_comm.put_Settings(str);  //������Ϊ115200,��У�飬8����λ��1��ֹͣλ 
		m_comm.put_InputLen(0);  //���õ�ǰ���������ݳ���Ϊ0,�������մ������е�����
		m_comm.put_InBufferSize(32000);
		m_comm.put_OutBufferSize(32000);
		m_comm.put_RThreshold(1);   //������һ���ַ������¼�  
		m_comm.put_RTSEnable(1);   //����RT���� 
		m_comm.put_PortOpen(true);//�򿪴��� 
		
	} 
	else
	{
		m_comm.put_PortOpen(false);
	}
	return(open_statu);
}
//�̵�����������
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
//�̵����رպ���
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
//����˶������� Position1Ϊ���һĿ��λ�� Position2Ϊ�����Ŀ��λ��
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
void Cmotortest20151013Dlg::LeftshouderMove()//������˶���ʼ
{
	MoveToPosition(Value_Posit_LeftShoulder,Value_Vel_LeftShoulder );	
}
void Cmotortest20151013Dlg::LeftshouderBack()//������������ʼ����
{
	MoveToPosition(-Value_Posit_LeftShoulder,-Value_Vel_LeftShoulder );	
}
void Cmotortest20151013Dlg::RightearMove()//���Ҷ��˶���ʼ
{
	MoveToPosition(Value_Posit_RightEar,Value_Vel_RightEar );
}
void Cmotortest20151013Dlg::RightearBack()//���Ҷ���������ʼ����
{
	MoveToPosition(-Value_Posit_RightEar,-Value_Vel_RightEar );
}
//�̺߳���-�߳�1-������״̬������ʵʱ��ⷵ�ػ����Ƿ�����
void ThreadFun1(LPVOID pParam)
{
	I32 Axis_ID = 0;
    F64 tmp = 0;
    I32 sts = 0;
	CString str=0;
	Cmotortest20151013Dlg *dlg=(Cmotortest20151013Dlg*)pParam;
	while(m_flag1)
	{
		//��1����ϸ����
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
		//���1�˶�״̬
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
		//��2����ϸ����
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
		//���2�˶�״̬
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
		//���μ�����-��⣬���ط�Ӧ////////////////////////////////////
		/*if((m_ForceChartDlg.RD[3]*196)>500)
		{
		APS_stop_move(0);
		APS_stop_move(1);
		}*/
        ////////////////////////////////////////////
		//����緵��ģʽ////////////////////////////////
		if(dlg->ptp[0]==1)
		{
			if((dlg->GetDlgItemInt(EDIT_fb_0)<-89500)&&(dlg->GetDlgItemInt(EDIT_fb_1)<=-140000))
			{
				if(dlg->GetDlgItemInt( IDC_MDN_EDIT)&&dlg->GetDlgItemInt(IDC_MDN_EDIT2 ))
				{
					///////�����������ģʽ����////////////////////
					dlg->LeftshouderBack();
                 dlg->ptp[0]=0;
				}
			}
		 }
		////////////////////////////////////////////
		//���Ҷ�����ģʽ////////////////////////////////
		if(dlg->ptp[1]==1)
		{
			if((dlg->GetDlgItemInt(EDIT_fb_0)<=-114000)&&(dlg->GetDlgItemInt(EDIT_fb_1)<=-9500))
			{
				if(dlg->GetDlgItemInt( IDC_MDN_EDIT)&&dlg->GetDlgItemInt(IDC_MDN_EDIT2 ))
				{
					///////���Ҷ��������ģʽ����////////////////////
					dlg->RightearBack();
					 dlg->ptp[1]=0;
				}
			}
		}
		Sleep(100);
		
	}
	return;

}
//�̺߳���-�߳�2-ѧϰģʽ//////////////////////////////
void ThreadFun2(LPVOID pParam)
{
    CString str1;
	CString str2;
	Cmotortest20151013Dlg *dlg=(Cmotortest20151013Dlg*)pParam;
	m_flag2=TRUE;
	while(m_flag2)
	{
	//�ɼ�EMG1������
		dlg->DAQ_EMG("Dev1",4);
		G[times]=EMG[5];
		str1.Format(_T("%8.3f"),EMG[5]);
		::SetDlgItemText(dlg->m_hWnd,IDC_Read1_EDIT, str1);
		dlg->DAQ_EMG("Dev1",5);
		R[times]=EMG[5];
		str2.Format(_T("%8.3f"),EMG[5]);
		::SetDlgItemText(dlg->m_hWnd,IDC_Read1_EDIT, str2);
		times++;
		Sleep(100);
	}
	return;
}
//�̺߳���-�߳�3-���ⴥ��ģʽ//////////////////////////////
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
        //�ɼ�EMG1������///////////////////////
		dlg->DAQ_EMG("Dev1",4);
		for(int i=0;i<100;i++)
		{
			sum1+=EMG[i];
		}
		if (times3==3)
		{
			test1=sum1/300;
			str1.Format(_T("%8.3f"),test1);
			::SetDlgItemText(dlg->m_hWnd,IDC_Test1_EDIT, str1);
			sum1=0;
		}
		
		//�ɼ�EMG2������///////////////////////////
		dlg->DAQ_EMG("Dev1",5);
		for(int i=0;i<100;i++)
		{
			sum2+=EMG[i];
		}
		if (times3==3)
		{
			test2=sum2/300;
			str1.Format(_T("%8.3f"),test2);
			::SetDlgItemText(dlg->m_hWnd,IDC_Test2_EDIT, str2);
			sum2=0;
			times3=0;
		}
		times3++;
		if(((test2>=(target2/**2/3*/))&&(test2!=0))&&(test1>=(target1*1/2)))
		 {
			m_flag3=FALSE;
			dlg->LeftshouderMove();//�����������
			dlg->ptp[0]=1;
			AfxMessageBox(_T("����"));
			return;
		 }
		if(test1>=(target1*2/3)&&(test2<=target2*2/3))
		 {
	    	m_flag3=FALSE;
			dlg->RightearMove();//���Ҷ��������
			dlg->ptp[1]=1;
			AfxMessageBox(_T("����"));	
			return;
		 }
		Sleep(100);
		}
		return;
}
//�߳�4-ʵʱ���������λ�ƴ�����������������������ֵ
void ThreadFun4(LPVOID pParam)
{
	CString str1;
	CString str2;
	CString str3;
	CString str4;
	CString str5;
	CString str6;
	CString strTime;
	CString m_Adress;
	double  m_MF1;
	double m_MF2;
	double m_linear=0;
	CByteArray data;
	CTime time;
	int len =0;
	clock_t start,finish;//clock_t start,finish;//����洢��ʼ�ڽ�����ʱ��ı���
	Cmotortest20151013Dlg *dlg=(Cmotortest20151013Dlg*)pParam;
	m_flag4 = TRUE;
	m_Adress=_T("02");
	len = Str2Hex(m_Adress, data);
	while (m_flag4)
	{
		start=clock();//�洢��ʼʱ��
		dlg->DAQ_EAF("Dev1",2,3);//���ݲɼ����ɼ���λ���Լ������ź�
		dlg->DAQ_EMG("Dev1",6);//�ɼ��󲿹��λ�ƴ������ź�
		m_linear=80-6*EMG[5];
		m_MF1=EAFW[0]*196;
		m_MF2=EAFW[1]*196;
		str1.Format(_T("%8.3f"),m_MF1);
		str2.Format(_T("%8.3f"),m_MF2);
		str3.Format(_T("%8.3f"),m_linear);
		::SetDlgItemText(dlg->m_hWnd,EDIT_MF1, str1);
		::SetDlgItemText(dlg->m_hWnd,EDIT_MF2, str2);
		::SetDlgItemText(dlg->m_hWnd,EDIT_LINEAR, str3);
		//����ģʽ-�ٶȣ�����λ��
		APS_get_feedback_velocity_f(0, &tmp);
		if (Posi_Angle_Arm<6||Posi_Angle_Arm>85||tmp>150000)
		{
			dlg->OnBnClickedVelStop();

		} 

		//if (m_AA[AS_i]<1.7||m_AA[AS_i]>3.32||m_SA[AS_i]<2.9||m_SA[AS_i]>4.36)//����������̻�λ������ͣ��
		//{
		//	APS_stop_move( 0 );
		//	APS_stop_move( 1 );
		//	APS_set_servo_on(0, OFF);
		//	APS_set_servo_on(1, OFF);
		//	dlg->KillTimer(1);
		//}
		//���ѭ������ʱ��
		finish=clock();//�洢����ʱ��
		strTime.Format(_T("%d"),finish-start);
		::SetDlgItemText(dlg->m_hWnd,IDC_EDIT_LOOP, strTime);
		
	}
	return;
}
void ThreadFun5(LPVOID pParam)
{
	
	
	return;
}
void Cmotortest20151013Dlg::OnBnClickedInitialButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	//ʹ�ø������������
	I32 Axis_ID = 0;
	for (Axis_ID = 0; Axis_ID < v_total_axis; Axis_ID++)
	{
		APS_set_command_f(Axis_ID, 0.0);
		APS_set_position_f(Axis_ID, 0.0);
	}	
	hThread1=CreateThread(NULL,
		0,
		(LPTHREAD_START_ROUTINE)ThreadFun1,
		this,
		0,
		&ThreadID1);
	//���õ���������
	for(int i=0;i<2;i++)
	{
		// �����߼�����
		APS_set_axis_param(i,PRA_EL_LOGIC,0);
		APS_set_axis_param(i,PRA_ORG_LOGIC,0);
		APS_set_axis_param(i,PRA_ALM_LOGIC,1);
		APS_set_axis_param(i,PRA_EZ_LOGIC,0);
		APS_set_axis_param(i,PRA_INP_LOGIC,1);
		APS_set_board_param(i,PRB_EMG_LOGIC,1);
		////////////////////////
		//APS_set_axis_param(0,PRA_PLS_IPT_MODE,0);//������������ģʽ
		APS_set_axis_param(i,PRA_PLS_OPT_MODE,0);//���������������������ģʽ
		APS_set_axis_param(i,PRA_ENCODER_DIR,0);//���ñ���������ʹ��

	}
	//�ٶ�ģʽ�µ��ڵ��PID����
	APS_set_axis_param(0,PRA_KP_GAIN,18309);
	APS_set_axis_param(0,PRA_KI_GAIN,35528);
	APS_set_axis_param(0,PRA_KD_GAIN,18883);
	APS_set_axis_param(1,PRA_KP_GAIN,17577);
	APS_set_axis_param(1,PRA_KI_GAIN,32879);
	APS_set_axis_param(1,PRA_KD_GAIN,18801);
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
	////�رո��߳�
	m_flag1=FALSE;
	m_flag2=FALSE;
	m_flag3=FALSE;
	m_flag4=FALSE;
	m_flag5=FALSE;
	m_comm.put_PortOpen(false);
	m_comm.put_PortOpen(false);
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
	average =sum/ 10;//��ƽ��ֵ
	return(average);
	
}
void Cmotortest20151013Dlg::OnBnClickedOnButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	I32 Axis_ID = 0;
	if (mt_flag)
	{
		
		for (Axis_ID = 0; Axis_ID < 2; Axis_ID++)
		APS_set_servo_on(Axis_ID, ON);
		mt_flag=FALSE;
		SetDlgItemText(IDC_ON_BUTTON,_T("�����"));
	} 
	else
	{
		ptp[0]=0;
		ptp[1]=0;
		for (Axis_ID = 0; Axis_ID < 2; Axis_ID++)
		APS_set_servo_on(Axis_ID, OFF);
		mt_flag=TRUE;
		SetDlgItemText(IDC_ON_BUTTON,_T("�����"));
	}
	
}

//��Ե���԰�ť
void Cmotortest20151013Dlg::OnBnClickedP2pButton4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
//���ð�ť
void Cmotortest20151013Dlg::OnBnClickedResetButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	I32 Axis_ID = 0;

    for (Axis_ID = 0; Axis_ID < v_total_axis; Axis_ID++)
    {
		APS_set_command_f(Axis_ID, 0.0);
        APS_set_position_f(Axis_ID, 0.0);
    }	
}
//��������Ϣ������
void Cmotortest20151013Dlg::OnBnClickedLeftshouderButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if( Is_CARD_INITIAL() == NO ) return;
	I32 Axis_ID;
	LeftshouderMove();
	ptp[0]=1;
}

//���Ҷ�����Ϣ������
void Cmotortest20151013Dlg::OnBnClickedRightearButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if( Is_CARD_INITIAL() == NO ) return;
	I32 Axis_ID;
	RightearMove();
	ptp[1]=1;
}
void Cmotortest20151013Dlg::OnBnClickedLearnbButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_flag2=FALSE;
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
	GetDlgItem(IDC_LearnE_BUTTON2)->EnableWindow(FALSE);
	GetDlgItem(IDC_LearnB_BUTTON)->EnableWindow(TRUE);

}


void Cmotortest20151013Dlg::OnBnClickedMButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	hThread3=CreateThread(NULL,
		0,
		(LPTHREAD_START_ROUTINE)ThreadFun3,
		this,
		0,
&ThreadID3);

}


void Cmotortest20151013Dlg::OnBnClickedRelayclose()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (relay_flag)
	{
	RelaySwitchClose();//�رռ̵���
	relay_flag=FALSE;
	SetDlgItemText(IDC_RelayClose,_T("����ɿ�"));
	} 
	else
	{
		uInt8       open[2]={1,1};
		RelaySwitchOpen();
		relay_flag=TRUE;
		SetDlgItemText(IDC_RelayClose,_T("�������"));
	}
}
//��ά�������������Ի���
void Cmotortest20151013Dlg::OnToolsSix()
{
	// TODO: �ڴ���������������
	CSixAxisSensorDlg *m_SixAxisSensor=new CSixAxisSensorDlg;
	m_SixAxisSensor->Create(IDD_SIXAXIS_DIALOG,this);
	m_SixAxisSensor->ShowWindow(SW_SHOW);
	
}

//���������������Ի���
void Cmotortest20151013Dlg::OnToolsEncoder()
{
	// TODO: �ڴ���������������
	CForceChartDlg *m_ForceChart=new CForceChartDlg;
	m_ForceChart->Create(IDD_ForceChart_DIALOG,this);
	m_ForceChart->ShowWindow(SW_SHOW);
}

//�ٶ�ģʽ-���1-��������
void Cmotortest20151013Dlg::OnBnClickedVelMot1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if( Is_CARD_INITIAL() == NO ) return;
	if (Is_axis_err(0) == YES) return;
	F64 speed_1 =  GetDlgItemInt( EDIT_VEL_MOT1);

	APS_set_axis_param_f( 0, PRA_STP_DEC, 10000.0 );
	APS_set_axis_param_f( 0, PRA_CURVE,      0.5 ); //Set acceleration rate
	APS_set_axis_param_f( 0, PRA_ACC,    50000.0 ); //Set acceleration rate
	APS_set_axis_param_f( 0, PRA_DEC,    50000.0 ); //Set deceleration rate

	//Check servo on or not
	if( !( ( APS_motion_io_status( 0 ) >> MIO_SVON ) & 1 ) )  
	{
		APS_set_servo_on( 0, ON ); 
		Sleep( 500 ); // Wait stable.
	}

	APS_vel( 0, 0, speed_1, 0 ); 
}

//�ٶ�ģʽ-���2-��������
void Cmotortest20151013Dlg::OnBnClickedVelMot2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if( Is_CARD_INITIAL() == NO ) return;
	if (Is_axis_err(1) == YES) return;
	F64 speed_1 =  GetDlgItemInt( EDIT_VEL_MOT2);

	APS_set_axis_param_f( 1, PRA_STP_DEC, 10000.0 );
	APS_set_axis_param_f( 1, PRA_CURVE,      0.5 ); //Set acceleration rate
	APS_set_axis_param_f( 1, PRA_ACC,    50000.0 ); //Set acceleration rate
	APS_set_axis_param_f( 1, PRA_DEC,    50000.0 ); //Set deceleration rate

	//Check servo on or not
	if( !( ( APS_motion_io_status( 1 ) >> MIO_SVON ) & 1 ) )  
	{
		APS_set_servo_on( 1, ON ); 
		//Sleep( 500 ); // Wait stable.
	}

	APS_vel( 1, 0, speed_1, 0 ); 
}

//�ٶ�ģʽ-���-ֹͣ����
void Cmotortest20151013Dlg::OnBnClickedVelStop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if( Is_CARD_INITIAL() == NO ) return;
	if (Is_axis_err(0) == YES) return;
	if (Is_axis_err(1) == YES) return;
	APS_set_axis_param_f( 0, PRA_STP_DEC, 50000.0 );
	APS_set_axis_param_f( 1, PRA_STP_DEC, 50000.0 );
	APS_stop_move( 0 );
	APS_stop_move( 1 );
	APS_set_servo_on(1, OFF);
	APS_set_servo_on(0 ,OFF);
	KillTimer(0);
	KillTimer(1);
}


void Cmotortest20151013Dlg::OnStartWithSensor()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if( Is_CARD_INITIAL() == NO ) return;
	if (Is_axis_err(0) == YES) return;
	if (Is_axis_err(1) == YES) return;
	SetTimer(0,100,NULL);
	
}


void Cmotortest20151013Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CString m_sixFt(" ");
	double  sixFt(0);		
	CSeries lineseries1;
	CSeries lineseries2;
	CSeries lineseries3;
	CSeries lineseries4;
	CSeries lineseries5;
	CString str(" ");
	CString str1(" ");
	CString KP_S(" ");
	CString KI_S(" ");
	CString KD_S(" ");
	CString str2;
    CString str3;
	CString str4;
	CString str5;
	double KP(0);
	double KI(0);
	double KD(0);
	double Pos_init=0;
	//����洢��ʼ�ڽ�����ʱ��ı���
	LARGE_INTEGER freq;
	LARGE_INTEGER start_t,stop_t;
	double exe_time;
	CString str6;
	CString str7;
	CString strTime;
	//////////////////////////////////////////////////////////////////////////
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&start_t);
	Pos_init=Posi_Angle_Arm;
	F64 ACC_1=8000000;
	GetDlgItemText(EDIT_FTT_Z,m_sixFt);//��ȡ��ǰ��ά��������FZ��ֵ
	sixFt=atof(m_sixFt);
	F64 speed_1 =  327*fabs(sixFt);//fabs()-��ø���������ֵ
	F64 speed_2 =speed_1-0.1*(speed_1-m_VEROR);//��������
	switch (nIDEvent)
	{
	case 0:
			if( Is_CARD_INITIAL() == NO ) return;
			if (Is_axis_err(1) == YES) return;
			//��ͼ/////////////
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
			APS_get_feedback_velocity_f(1, &tmp);//�����ٶ���ʾ
			str.Format("%f",tmp);
			str1.Format("%f",speed_2);
			SetDlgItemText(EDIT_VEL_REA, str );
			SetDlgItemText(EDIT_VEL_STD, str1 );
			m_VEROR=speed_1;
			m_VEROR1=speed_2;
			FT_i++;
			UpdateData (TRUE);

		break;
	case 1:
		if (AS_i==3){
		AS_i=0;}
		GetDlgItemText(EDIT_INPUT_KP,KP_S);//��ȡ�Ի���KP��ֵ
		KP=atof(KP_S);
		GetDlgItemText(EDIT_INPUT_KI,KI_S);//��ȡ�Ի���KI��ֵ
		KI=atof(KI_S);
		GetDlgItemText(EDIT_INPUT_KD,KD_S);//��ȡ�Ի���KD��ֵ
		KD=atof(KD_S);
		m_AA[AS_i]=Posi_Angle_Arm/**M_PI/180*/;//tmp1Ϊ���صĽǶ�ֵ
		str2.Format(_T("%8.3f"),m_AA[AS_i]);
		::SetDlgItemText(m_hWnd,EDIT_AA, str2);
		switch (AS_i)
			{
			case 0:
				if(FT_i==0){
					Vel_Real_AA=0;}
				else{
					Vel_Real_AA=(m_AA[0]-m_AA[1])/0.11;
				}
				break;
			case 1:
				if (FT_i<=1)
				{
					Vel_Real_AA=(m_AA[1]-m_AA[0])/0.055;
				} 
				else
				{
					Vel_Real_AA=(m_AA[1]-m_AA[2])/0.11;
				}
				break;
			case 2:Vel_Real_AA=(m_AA[2]-m_AA[0])/0.11;
				break;
			}
		str4.Format(_T("%f"),Vel_Real_AA);
		str5.Format(_T("%f"),Vel_Real_SA);
		::SetDlgItemText(m_hWnd,EDIT_AA_VEL, str4);
		::SetDlgItemText(m_hWnd,EDIT_SA_VEL, str5);
		if (FT_i<=300)
			{
				if (FT_i==0)
				 {
					APS_set_axis_param_f( 0, PRA_SF, 0.5      );
					APS_set_axis_param_f( 0, PRA_ACC, 10000.0 );
					APS_set_axis_param_f( 0, PRA_DEC, 10000.0 );
					APS_set_axis_param_f( 0, PRA_VM,  1000.0   );
					//Check servo on or not
					if( !( ( APS_motion_io_status( 0 ) >> MIO_SVON ) & 1 ) )  
					{
						APS_set_servo_on( 0, ON ); 
					}
					APS_ptp_all( 
						0  // I32 Axis_ID
						, OPT_RELATIVE // I32 Option
						, 0 // F64 Position
						, 0     // F64 Vs
						, 10000 // F64 Vm
						, 0     // F64 Ve
						, 10000 // F64 Acc
						, 10000// F64 Dec
						, 0.5 // F64 SFac
						, 0   // ASYNCALL *Wait
						);
				 }
				else
				 {
					if (FT_i<4)
					 {
						Vel_Buffer[FT_i]=Posi_Angle_Arm;//Vel_Real_AA;
						target1=Posi_Angle_Arm;//Vel_Real_AA;
					 }
					else
					 {
						Vel_Buffer[0]=Vel_Buffer[1];
						Vel_Buffer[1]=Vel_Buffer[2];
						Vel_Buffer[2]=Vel_Buffer[3];
						Vel_Buffer[3]=Posi_Angle_Arm;//Vel_Real_AA;
						target1=(Vel_Buffer[0]+Vel_Buffer[1]+Vel_Buffer[2]+Vel_Buffer[3])/4;
					 }
				
					Pos_Comd_Arm=85*sin(((double)FT_i)/300*M_PI)+Pos_init;
					Pos_Error[0]=Pos_Comd_Arm-target1;
					Pos_PID=Pos_PID+KP*(Pos_Error[0]-Pos_Error[1])+KI*Pos_Error[0]+KD*(Pos_Error[0]-2*Pos_Error[1]+Pos_Error[2]);
					Pos_Error[2]=Pos_Error[1];
					Pos_Error[1]=Pos_Error[0];
					Pos_Comd_Motor=Pos_PID*Unit_Convert/*/10*/;
					APS_set_axis_param_f( 0, PRA_SF, 0.5      );
					APS_set_axis_param_f( 0, PRA_ACC, 10000.0 );
					APS_set_axis_param_f( 0, PRA_DEC, 10000.0 );
					APS_set_axis_param_f( 0, PRA_VM,  1000.0   );
					//Check servo on or not
					if( !( ( APS_motion_io_status( 0 ) >> MIO_SVON ) & 1 ) )  
					{
						APS_set_servo_on( 0, ON ); 
					}
					/*if (Pos_Comd_Motor>Value_Vel_RightEar)
					{
					Pos_Comd_Motor=Value_Vel_RightEar/10;
					}*/
					/*if (Vel_Comd>=0)
					APS_vel(0, 0,Vel_Comd, 0 );
					else
					APS_vel(0, 1, -1*Vel_Comd, 0 );*/
					// Start a relative p to p move
					APS_ptp_all( 
						0  // I32 Axis_ID
						, OPT_RELATIVE // I32 Option
						, Pos_Comd_Motor // F64 Position
						, 0     // F64 Vs
						, 10000 // F64 Vm
						, 0     // F64 Ve
						, 10000 // F64 Acc
						, 10000// F64 Dec
						, 0.5 // F64 SFac
						, 0   // ASYNCALL *Wait
						);
				}
					APS_get_feedback_velocity_f(0, &tmp);
					lineseries1=(CSeries)m_FTVEL.Series(0);
					lineseries2=(CSeries)m_FTVEL.Series(1);
					lineseries3=(CSeries)m_FTVEL.Series(2);
					lineseries4=(CSeries)m_FTVEL.Series(3);
					lineseries5=(CSeries)m_FTVEL.Series(4);
					lineseries1.AddXY((float)FT_i*T_C/1000,Pos_Comd_Arm,NULL,0);
					lineseries2.AddXY((float)FT_i*T_C/1000,target1,NULL,0);
					lineseries3.AddXY((float)FT_i*T_C/1000,Pos_Comd_Motor/Unit_Convert,NULL,0);
					lineseries4.AddXY((float)FT_i*T_C/1000,Pos_Error[0],NULL,0);
					lineseries5.AddXY((float)FT_i*T_C/1000,tmp/Unit_Convert,NULL,0);
					QueryPerformanceCounter(&stop_t);
					exe_time=50+1000*(stop_t.QuadPart-start_t.QuadPart)/freq.QuadPart;
					strTime.Format(_T("%f"),exe_time);
					SetDlgItemText(IDC_EDIT_CLOOP, strTime);
					FT_i++;
					AS_i++;

			}
			else
			{
				KillTimer(1);
			}
		break;
	}
	
	CDialogEx::OnTimer(nIDEvent);
}


BOOL Cmotortest20151013Dlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
	status =DAQmxCreateTask("AAencoderTask", &taskHandle);
	status =DAQmxCreateAIVoltageChan(taskHandle, changeDeviceName+cstrChannelList, "AAencoderChannel", DAQmx_Val_RSE, 0, 10, DAQmx_Val_Volts, NULL);
	//UpdateData(TRUE);
	status =DAQmxCfgSampClkTiming(taskHandle, "OnboardClock", 2000, DAQmx_Val_Rising, DAQmx_Val_ContSamps, 20);
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if( Is_CARD_INITIAL() == NO ) return;
	if (Is_axis_err(1) == YES) return;
	F64 speed_1 = 20000;
	APS_set_axis_param_f( 0, PRA_STP_DEC, 10000.0 );
	APS_set_axis_param_f( 0, PRA_CURVE,      0.5 ); //Set acceleration rate
	APS_set_axis_param_f( 0, PRA_ACC,    50000.0 ); //Set acceleration rate
	APS_set_axis_param_f( 0, PRA_DEC,    50000.0 ); //Set deceleration rate

	//Check servo on or not
	if( !( ( APS_motion_io_status( 0 ) >> MIO_SVON ) & 1 ) )  
	{
		APS_set_servo_on( 0, ON ); 
		Sleep( 500 ); // Wait stable.
	}

	APS_vel( 0, 1, speed_1, 0 );
}

//������ť����Ӧ����
void Cmotortest20151013Dlg::OnClearAllSeries()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	for (int i=0;i<m_FTVEL.get_SeriesCount();i++)
	{
		((CSeries)m_FTVEL.Series(i)).Clear();
	}
	for (int i=0;i<3;i++)
	{
		m_AA[i]=0;
	}
	Posi_Angle_Arm=0;
	Vel_Real_AA=0;
	for (int i=0;i<4;i++)
	{
		Vel_Buffer[i]=0;
	}
	target1=0;
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	if( Is_CARD_INITIAL() == NO ) return;
	if (Is_axis_err(0) == YES) return;
	if (Is_axis_err(1) == YES) return;
	if (PID_flag)
	{
		OnClearAllSeries();
		SetTimer(1,50,NULL);
		SetDlgItemText(BUTTON_PID_CONTROL,_T("ֹͣ"));
		PID_flag=false;
	} 
	else/*(PID_flag=false);*/
	{
		OnBnClickedVelStop();
		SetDlgItemText(BUTTON_PID_CONTROL,_T("PID�˶�"));
		PID_flag=true;
	}
	
}


BEGIN_EVENTSINK_MAP(Cmotortest20151013Dlg, CDialogEx)
	ON_EVENT(Cmotortest20151013Dlg, IDC_MSCOMM1, 1, Cmotortest20151013Dlg::OnCommMscomm1, VTS_NONE)
END_EVENTSINK_MAP()


void Cmotortest20151013Dlg::OnCommMscomm1()
{
	// TODO: �ڴ˴������Ϣ����������
	VARIANT variant_inp1;
	COleSafeArray safearray_inp1;
	LONG len1, k1;
	 //����BYTE���� 
	CString strtemp1;
	CString strangle;
	
	unsigned long BMQ_Val1;
	if (m_comm.get_CommEvent() == 2)     //�¼�ֵΪ2��ʾ���ջ����������ַ�
	{
		variant_inp1 = m_comm.get_Input();   //��������
		safearray_inp1 = variant_inp1;           //VARIANT�ͱ���ת��ΪColeSafeArray�ͱ���
		len1 = safearray_inp1.GetOneDimSize(); //�õ���Ч���ݳ���
		for (k1 = 0; k1<len1; k1++)
			safearray_inp1.GetElement(&k1, rxdata1 + k1);//ת��ΪBYTE������
		if (((rxdata1[0] & 0xFF) == 0xAA) && (((rxdata1[2] + rxdata1[3] + rxdata1[4] + rxdata1[5]) & 0xFF) == rxdata1[6]))              //����֡У��ͨ��
		{
			BMQ_Val1 = ((rxdata1[5] & 0xFF) | ((rxdata1[4] & 0xFF) << 8) | ((rxdata1[3] & 0xFF) << 16) | ((rxdata1[2] & 0xFF) << 24)) & 0xFFFFFFF; //����ƴ��

			Posi_Angle_Arm = ((double)(BMQ_Val1 & 0xFFF) / 0xFFF)*360.0;    //��0x000-0xFFF��12λʮ����������ת����0-360�Ƕ�ֵ
		}
	}
	UpdateData(FALSE);           //���±༭������
}
void Cmotortest20151013Dlg::OnRadioPosition()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetDlgItem(IDC_P2P_BUTTON4)->EnableWindow(TRUE);
	GetDlgItem(IDC_LeftShouder_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_RightEar_BUTTON)->EnableWindow(TRUE);
}


void Cmotortest20151013Dlg::OnRadioVSingle()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetDlgItem(BUTTON_VEL_MOT1)->EnableWindow(TRUE);
	GetDlgItem(Button_VEL_STOP)->EnableWindow(TRUE);
	GetDlgItem(EDIT_VEL_MIX)->EnableWindow(TRUE);
	GetDlgItem(BUTTON_VEL_MOT2)->EnableWindow(TRUE);
}


void Cmotortest20151013Dlg::OnRadioSix()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetDlgItem(BUTTON_VEL_START)->EnableWindow(TRUE);
}


void Cmotortest20151013Dlg::OnRadioPid()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetDlgItem(BUTTON_PID_CONTROL)->EnableWindow(TRUE);
}


void Cmotortest20151013Dlg::OnRadioEmg()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetDlgItem(IDC_LearnB_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_M_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_LearnE_BUTTON2)->EnableWindow(TRUE);
}


void Cmotortest20151013Dlg::OnButtonQuit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	////�رո��߳�
	m_flag1=FALSE;
	m_flag2=FALSE;
	m_flag3=FALSE;
	m_flag4=FALSE;
	m_flag5=FALSE;
	KillTimer(0);
	KillTimer(1);
	CDialog::OnClose();
}
