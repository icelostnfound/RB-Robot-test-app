// SixAxisSensorDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "motor test 20151013.h"
#include "SixAxisSensorDlg.h"
#include "afxdialogex.h"
#include "SixAixsSettingDlg.h"
#include "ProgramOpDlg.h"
#include "SixAxisSensorInfoDlg.h"
#include "DlgProxy.h" 
#include "ConfigureSaveData.h"
#include "RegAccess.h"
#include <direct.h>
#include  <fstream>
using namespace std;
#define PEAK_FT 20
#define PROGRESS_GRANALITY 1000
#define IbToN 4.44520552
#define inIbToNm 0.1130
// just a random number to use as a signal for the timer
// doesn't matter what it is, as long as they all use the same timer
#define TIMER_1 42
// CSixAxisSensorDlg 对话框

IMPLEMENT_DYNAMIC(CSixAxisSensorDlg, CDialogEx)

CSixAxisSensorDlg::CSixAxisSensorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSixAxisSensorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVisualCSampleDlg)
	m_display = -1;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pAutoProxy = NULL;
	dlgProgramOptions = NULL;
//((Cmotortest20151013App*)AfxGetApp())->m_Between=FTT;
	dlgSettings = NULL;
	
}

CSixAxisSensorDlg::~CSixAxisSensorDlg()
{
	// If there is an automation proxy for this dialog, set
	//  its back pointer to this dialog to NULL, so it knows
	//  the dialog has been deleted.
	if (m_pAutoProxy != NULL)
		m_pAutoProxy->m_pDialog = NULL;
	if (NULL != dlgProgramOptions)
		delete dlgProgramOptions;
	if (NULL != dlgSettings)
		delete dlgSettings;
}

void CSixAxisSensorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVisualCSampleDlg)
	//DDX_Control(pDX, IDC_BUTTONLOG, m_buttonLog);
	DDX_Control(pDX, IDC_BUTTONLOG, m_buttonLog);
	DDX_Control(pDX, LABEL_THERMISTOR, mLabelThermistor);
	DDX_Control(pDX, GROUP_TORQUE, mGroupTorque);
	DDX_Control(pDX, GROUP_FORCE, mGroupForce);
	DDX_Control(pDX, BUTTON_UNBIAS, mUnbias);
	DDX_Control(pDX, BUTTON_BIAS, mBias);
	DDX_Control(pDX, LABEL_MESSAGE, mLabelMessage);
	DDX_Radio(pDX, RADIO_VOLTAGES, m_display);
	//}}AFX_DATA_MAP
	//////////////////////////////
	DDX_Control(pDX, LABEL_FX, mLabels[0]);
	DDX_Control(pDX, LABEL_FY, mLabels[1]);
	DDX_Control(pDX, LABEL_FZ, mLabels[2]);
	DDX_Control(pDX, LABEL_TX, mLabels[3]);
	DDX_Control(pDX, LABEL_TY, mLabels[4]);
	DDX_Control(pDX, LABEL_TZ, mLabels[5]);
	DDX_Control(pDX, LABEL_FX_VAL, mValues[0]);
	DDX_Control(pDX, LABEL_FY_VAL, mValues[1]);
	DDX_Control(pDX, LABEL_FZ_VAL, mValues[2]);
	DDX_Control(pDX, LABEL_TX_VAL, mValues[3]);
	DDX_Control(pDX, LABEL_TY_VAL, mValues[4]);
	DDX_Control(pDX, LABEL_TZ_VAL, mValues[5]);
	DDX_Control(pDX, MAX_FX, mLabelMax[0]);
	DDX_Control(pDX, MAX_FY, mLabelMax[1]);
	DDX_Control(pDX, MAX_FZ, mLabelMax[2]);
	DDX_Control(pDX, MAX_TX, mLabelMax[3]);
	DDX_Control(pDX, MAX_TY, mLabelMax[4]);
	DDX_Control(pDX, MAX_TZ, mLabelMax[5]);
	DDX_Control(pDX, UNIT_FX, mUnit[0]);
	DDX_Control(pDX, UNIT_FY, mUnit[1]);
	DDX_Control(pDX, UNIT_FZ, mUnit[2]);
	DDX_Control(pDX, UNIT_TX, mUnit[3]);
	DDX_Control(pDX, UNIT_TY, mUnit[4]);
	DDX_Control(pDX, UNIT_TZ, mUnit[5]);



}


BEGIN_MESSAGE_MAP(CSixAxisSensorDlg, CDialogEx)
	ON_COMMAND(SETTINGS, &CSixAxisSensorDlg::OnToolsSettings)
	ON_COMMAND(SENSOR_INFORMATION, &CSixAxisSensorDlg::OnToolsSensorinformation)
	ON_COMMAND(PROGRAM_OPTIONS, &CSixAxisSensorDlg::OnToolsProgramoptions)
	ON_BN_CLICKED(BUTTON_BIAS, &CSixAxisSensorDlg::OnBnClickedBias)
	ON_BN_CLICKED(BUTTON_UNBIAS, &CSixAxisSensorDlg::OnBnClickedUnbias)
	ON_BN_CLICKED(RADIO_VOLTAGES, &CSixAxisSensorDlg::OnBnClickedVoltages)
	ON_COMMAND(IDC_BUTTONLOG, &CSixAxisSensorDlg::OnButtonlog)
	ON_BN_CLICKED(RADIO_FT, &CSixAxisSensorDlg::OnBnClickedFt)
	ON_WM_TIMER()
	ON_COMMAND(OPEN_CALIBRATION, &CSixAxisSensorDlg::OnCalibration)
	ON_COMMAND(EXIT, &CSixAxisSensorDlg::OnExit)
	ON_COMMAND(SAVE_CONFIGURATION, &CSixAxisSensorDlg::OnConfiguration)
	ON_COMMAND(OPEN_CONFIGURATION, &CSixAxisSensorDlg::OnOpenConfiguration)
END_MESSAGE_MAP()


// CSixAxisSensorDlg 消息处理程序
BOOL CSixAxisSensorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CustomInitialization();	
	t=0;


	/*set voltages to be default display*/
	m_display = 0;
	UpdateData(FALSE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}
void CSixAxisSensorDlg::OnClose() 
{
	if (CanExit())
		CDialog::OnClose();
}

BOOL CSixAxisSensorDlg::CanExit()
{
	// If the proxy object is still around, then the automation
	//  controller is still holding on to this application.  Leave
	//  the dialog around, but hide its UI.
	if (m_pAutoProxy != NULL)
	{
		ShowWindow(SW_HIDE);
		return FALSE;
	}

	return TRUE;
}
/* 代码的初始化部分的一个集中called during OnInitDialog - a centralized place for added startup code */
void CSixAxisSensorDlg::CustomInitialization()
{
	CMenu* menu = this->GetMenu();
	BOOL Stat=TRUE;
	// go ahead and turn off all of the menu operations that we don't want to allow unless certain conditions have been met
	// we can turn them back on as needed with the MF_ENABLED flag
	menu->EnableMenuItem(OPEN_CALIBRATION, MF_GRAYED);
	menu->EnableMenuItem(PROGRAM_OPTIONS, MF_GRAYED);
	menu->EnableMenuItem(SETTINGS, MF_GRAYED);
	menu->EnableMenuItem(SENSOR_INFORMATION, MF_GRAYED);
	menu->EnableMenuItem(OPEN_CONFIGURATION, MF_ENABLED);
	menu->EnableMenuItem(SAVE_CONFIGURATION, MF_GRAYED);
	Stat=mFTWrapper.GetStatus();
	if (mFTWrapper.GetStatus() & FTWrapper::DAQ_READY) {
		// DAQ loaded fine
		dlgProgramOptions = new CProgramOpDlg(&mFTWrapper, this);
		dlgSettings = new CSixAixsSettingDlg(&mFTWrapper, this);
		this->SetTimer(TIMER_1,dlgProgramOptions->getTimer(),NULL); // the timer is actually rounded up to the nearest 55 ms, so this will be 110 ms
		// if the DAQ is okay then we are ready to load a calibration file and/or make changes to the DAQ settings
		menu->EnableMenuItem(OPEN_CALIBRATION, MF_ENABLED);
		menu->EnableMenuItem(PROGRAM_OPTIONS, MF_ENABLED);
	}
	else {
		// something bad happened
		CString error = mFTWrapper.GetDAQErrorMessage();
		mLabelMessage.SetWindowText(error);
	}
	enableInputs();
	// Fonts are confusing.  I just wanted a larger, bolder text, but this is what I had to go through.
	// It would have been easier (if possible) to take the existing font and modify it, but I didn't see a way.
	CFont font;
	font.CreateFont(24,							// nHeight
					0,							// nWidth
					0,							// nEscapement
					0,							// nOrientation
					FW_BOLD,					// nWeight
					FALSE,						// bItalic
					FALSE,						// bUnderline
					0,							// cStrikeOut
					ANSI_CHARSET,				// nCharSet
					OUT_DEFAULT_PRECIS,			// nOutPrecision
					CLIP_DEFAULT_PRECIS,		// nClipPrecision
					DEFAULT_QUALITY,			// nQuality
					DEFAULT_PITCH | FF_SWISS,	// nPitchAndFamily
					_T("Arial"));					// lpszFacename	
	mLabelMessage.SetFont(&font, true);

	char autoLoadFile[200]; /* the file which contains the 
							configuration to auto-load */
	if ( 0 ==GetRegValue( _T("AUTOLOAD"), autoLoadFile, 200 ) )
	{
		/*Set the checkbox setting on the program options dialog*/
		dlgProgramOptions->setAutoLoad( true );
		mAutoLoadFilename = autoLoadFile;
		/*autoload the file*/
		loadConfiguration(autoLoadFile);
	}
	else
	{
	//	this->dlgProgramOptions->setAutoLoad( false );
		mAutoLoadFilename = "";
	}

	mLoggingEnabled = false;
}
/* 用于不断地进行屏幕更新，这个程序读取传入的电压并且更新在屏幕上的各个组成部分the heard and soul of the screen update, this procedure reads the voltages passed in and updates the various components on the screen */
void CSixAxisSensorDlg::UpdateGauges(double* readings)
{
	// ignore temperature (readings[6]) for now
	for (long i=0; i<6; i++) {
		t = readings[i];
		if (t>1e5) {
			t = 1e5;
		}
		if (t<-1e5) {
			t = -1e5;
		}
		int display = ftoi(t,(short)i);
		// clamp to the maximum saturation value
		if (display > PROGRESS_GRANALITY) {
			display = PROGRESS_GRANALITY;
			//			t = (show_raw_voltages)?PEAK_VOLTAGE:PEAK_FT;
		}
		// clamp to the minimum saturation value
		if (display < 0) {
			display = 0;
			//			t = -((show_raw_voltages)?PEAK_VOLTAGE:PEAK_FT);
		}
		CString str;
		if (i<=3)
		{
			str.Format("%4.4f",t*IbToN);
		}
		else
		{
str.Format("%4.4f",t*inIbToNm);
		}
		
		mValues[i].SetWindowText(str);
	}
	if (dlgProgramOptions->isThermistorVisible()) {
		CString str;
		str.Format("Thermistor: %4.4f V",readings[6]);
		mLabelThermistor.SetWindowText(str);
		//		mLabelThermistor.EnableWindow(true);
		//		mLabelThermistor.ShowWindow(true);
	}
	else {
		mLabelThermistor.SetWindowText("");
		//		mLabelThermistor.EnableWindow(false);
		//		mLabelThermistor.ShowWindow(false);
	}
}
void CSixAxisSensorDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	static bool alreadyScanning = false;	// using this boolean will help prevent 2 timer events from being processed at the same time
	if (nIDEvent == TIMER_1) {
		if (alreadyScanning == false) {
			alreadyScanning = true;
			try {
				double readings[7];
				int mDAQstatus;
				if (show_raw_voltages) {
					mDAQstatus = mFTWrapper.GetRawVoltages(readings);
				}
				else {
					mDAQstatus = mFTWrapper.GetForcesAndTorques(readings);
				}
				if (mDAQstatus) {
					if (dlgProgramOptions->getBeep()) {
						Beep(440,50);	//0x25 through 0x7FFF
					}
					mLabelMessage.SetWindowText("Saturation!");
				}
				else {
					mLabelMessage.SetWindowText("");
				}
				//利用WM_COPYDATA实现进程间通信
				FTT=readings[2]*IbToN;
				CString str;
				str.Format( _T("%f"), FTT);
				const char szDlgTiltle[]=_T("外骨骼测试程序");
				HWND hReceiveWindow=::FindWindow(NULL,szDlgTiltle);
				HWND hSendWindow=::FindWindow(NULL,"Six-Axis F/T Sensor");
				if(hReceiveWindow==NULL)
					return;
				COPYDATASTRUCT copyData;
				copyData.lpData=str.GetBuffer();
				copyData.cbData=str.GetLength();
				copyData.dwData=0;
				::SendMessage(hReceiveWindow,WM_COPYDATA,(WPARAM)hSendWindow,(LPARAM)&copyData);
				str.ReleaseBuffer();
				//利用WM_COPYDATA实现进程间通信
				UpdateGauges(readings);
				if ( mLoggingEnabled )
				{
					AddToLogFile(readings);
				}
				alreadyScanning = false;
			}
			catch(CUserException e)
			{
				const int N = 80;
				char errmsg[N];;
				e.GetErrorMessage(errmsg,N,NULL);
				alreadyScanning = false;
			}
			catch(char* e)
			{
				alreadyScanning = false;
			}
			catch(int e)
			{
				alreadyScanning = false;
			}
			catch (DAQException* e)
			{
				e->GetErrMsg();
				alreadyScanning = false;
			}
			catch(void* e)
			{
				alreadyScanning = false;
			}
			catch(...)
			{
				alreadyScanning = false;
			}
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}
/*将读取的浮点数转换成进度条显示 an internal function used to convert floats into progress bar readings */
int CSixAxisSensorDlg::ftoi(double f, short channel)
{
	if (show_raw_voltages) {
		double offset = 0.0;
		double voltageRange = mFTWrapper.getOutputRange();
		if (mFTWrapper.isBipolar()) {
			offset = voltageRange/2;
		}
		f = f+offset;				// align the signal so that 0 is the maximum negative signal
		f = f/(voltageRange);		// normalize the signal;
		f = f*PROGRESS_GRANALITY;	// scale the signal
	}
	else {
		double max_ft = PEAK_FT;
		/* mFTWrapper.GetActiveCalibrationIndex() should never return -1 here b/c a calfile has been loaded to get here (show_raw_voltages = false) */
		if (mFTWrapper.GetActiveCalibrationIndex() != -1) {
			channel++;
			max_ft = mFTWrapper.GetActiveCalibration().GetAxisMaxLoad(COleVariant(channel, VT_I2));
		}
		f = f+max_ft;				// align the signal so that 0 is the maximum negative signal
		f = f/(2*max_ft);			// normalize the signal;
		f = f*PROGRESS_GRANALITY;	// scale the signal
	}
	return (int)(f);			// convert the signal to int (this will truncate the signal, i.e., round down)
}

//六维力传感器设置对话框
void CSixAxisSensorDlg::OnToolsSettings()
{
	// TODO: 在此添加命令处理程序代码
	if (mFTWrapper.GetStatus() & FTWrapper::CALFILE_LOADED) {
		bool need_to_reset_timer = false;
		if (this->KillTimer(TIMER_1)) {
			need_to_reset_timer = true;
		}
		dlgSettings->DoModal();
		if (need_to_reset_timer) {
			this->SetTimer(TIMER_1,dlgProgramOptions->getTimer(),NULL); // the timer is actually rounded up to the nearest 55 ms, so this will be 110 ms
		}
	}

	/*非模态对话框
	CSixAixsSettingDlg *m_SixAixsSettingDlg= new CSixAixsSettingDlg;
	m_SixAixsSettingDlg->Create(IDD_SIXAIXS_SETTINGS,this);
	m_SixAixsSettingDlg->ShowWindow(SW_SHOW);*/
}

//六维力传感器传感器信息对话框
void CSixAxisSensorDlg::OnToolsSensorinformation()
{
	// TODO: 在此添加命令处理程序代码
	if (mFTWrapper.GetStatus() & FTWrapper::CALFILE_LOADED) {
		bool need_to_reset_timer = false;
		if (this->KillTimer(TIMER_1)) {
			need_to_reset_timer = true;
		}
		static CSixAxisSensorInfoDlg sensorInfo(&mFTWrapper, this);
		sensorInfo.DoModal();
		if (need_to_reset_timer) {
			this->SetTimer(TIMER_1,dlgProgramOptions->getTimer(),NULL); // the timer is actually rounded up to the nearest 55 ms, so this will be 110 ms
		}
	}
	//创建非模态对话框
	/*CSixAxisSensorInfoDlg *m_SixAxisSensorInfoDlg=new CSixAxisSensorInfoDlg;
	m_SixAxisSensorInfoDlg->Create(IDD_SENSOR_INFORMATION,this);
	m_SixAxisSensorInfoDlg->ShowWindow(SW_SHOW);*/
}

//六维力传感器传感器信息对话框
void CSixAxisSensorDlg::OnToolsProgramoptions()
{
	// TODO: 在此添加命令处理程序代码
	if (mFTWrapper.GetStatus() & FTWrapper::DAQ_READY) {
		bool need_to_reset_timer = false;
		if (this->KillTimer(TIMER_1)) {
			need_to_reset_timer = true;
		}
		dlgProgramOptions->DoModal();
		if ( dlgProgramOptions->getAutoLoad() )
		{
			if ( "" != mAutoLoadFilename )
			{
				//SetRegValue( "AUTOLOAD", (LPCTSTR)mAutoLoadFilename );
			}else
			{
				MessageBox( "No auto-load file name has been specified.  Please save this configuration to auto-load it at startup.", "No Auto-load File Specified" );
			}
		}else
		{
			DeleteRegValue( "AUTOLOAD" );
		}
		if (need_to_reset_timer) {
			this->SetTimer(TIMER_1,dlgProgramOptions->getTimer(),NULL); // the timer is actually rounded up to the nearest 55 ms, so this will be 110 ms
		}
	}	
	//创建非模态对话框
	/*CProgramOpDlg *m_ProgramOpDlg=new CProgramOpDlg;
	m_ProgramOpDlg->Create(IDD_PROGRAM_OPTIONS,this);
	m_ProgramOpDlg->ShowWindow(SW_SHOW);*/
}

/*电压偏置的按钮事件响应函数*/
void CSixAxisSensorDlg::OnBnClickedBias()
{
	// TODO: 在此添加控件通知处理程序代码
	mFTWrapper.Bias(true);
}

/*解除电压偏置的按钮事件响应函数*/
void CSixAxisSensorDlg::OnBnClickedUnbias()
{
	// TODO: 在此添加控件通知处理程序代码
	mFTWrapper.Bias(false);
}
/*打开校准文件的按钮事件响应函数*/
void CSixAxisSensorDlg::OnCalibration()
{
	// TODO: 在此添加命令处理程序代码
	if ((mFTWrapper.GetStatus() & FTWrapper::DAQ_READY) == 0) {
		return;
	}
	bool need_to_reset_timer = false;
	if (this->KillTimer(TIMER_1)) {
		need_to_reset_timer = true;
	}
	mFTWrapper.LoadCalFile("");
	//	mFTWrapper.LoadCalFile("I:/Engineering/Software/FT/DAQ FT Customer Software/Visual C Sample/input/dual.cal");
	CString filename = mFTWrapper.GetFTSensor()->GetCalFilePath();
	if (filename == "") {
		//		mLabelMessage.SetWindowText("Calibration File was not loaded");
	}
	else {
		//		mLabelMessage.SetWindowText("Loaded a new Calibration File: " + filename);
		CMenu* menu = this->GetMenu();
		menu->EnableMenuItem(SETTINGS, MF_ENABLED);
		menu->EnableMenuItem(SENSOR_INFORMATION, MF_ENABLED);
		//		need_to_reset_timer = true;
	}
	enableInputs();
	if (need_to_reset_timer) {
		this->SetTimer(TIMER_1,dlgProgramOptions->getTimer(),NULL); // the timer is actually rounded up to the nearest 55 ms, so this will be 110 ms
	}
}
/* 这个函数启用/禁用基于当前状态的各种控件 this procedure enables/disables various controls base on the current state */
void CSixAxisSensorDlg::enableInputs()
{
	bool visibility = (bool)(mFTWrapper.GetStatus() & FTWrapper::DAQ_READY);
	mBias.EnableWindow(visibility);
	mUnbias.EnableWindow(visibility);
	for (int i=0; i<6; i++) {
		mValues[i].EnableWindow(visibility);
		mLabelMax[i].EnableWindow(visibility);
		mLabels[i].EnableWindow(visibility);
	}
	visibility = visibility && !show_raw_voltages;
	mGroupForce.EnableWindow(visibility);
	mGroupTorque.EnableWindow(visibility);

	visibility = (bool)( (mFTWrapper.GetStatus() & FTWrapper::ALL_READY) == FTWrapper::ALL_READY );

	/*Enable F/T Display*/
	CButton FTButton;
	HWND buttonHWND;
	GetDlgItem(RADIO_FT, &buttonHWND);
	FTButton.Attach(buttonHWND);//附加映射到MDI客户端窗口
	FTButton.EnableWindow(visibility);
	FTButton.Detach();//分离 CWnd 对象的一个Windows句柄并返回处理。 

	/*enable save configuration menu item*/
	CMenu * menu = this->GetMenu();	
	menu->EnableMenuItem(SAVE_CONFIGURATION, visibility ? MF_ENABLED : MF_GRAYED);
}

void CSixAxisSensorDlg::OnBnClickedVoltages()
{
	// TODO: 在此添加控件通知处理程序代码
	int i;
	show_raw_voltages = true;
	for (i=0; i<6; i++) {
		mLabelMax[i].SetWindowText(_T("10.0000"));
		mUnit[i].SetWindowText(_T("V"));
	}
	mGroupForce.SetWindowText(_T(""));
	mGroupTorque.SetWindowText(_T(""));
	mLabels[0].SetWindowText(_T("G0"));
	mLabels[1].SetWindowText(_T("G1"));
	mLabels[2].SetWindowText(_T("G2"));
	mLabels[3].SetWindowText(_T("G3"));
	mLabels[4].SetWindowText(_T("G4"));
	mLabels[5].SetWindowText(_T("G5"));
	if ( mLoggingEnabled )
	{
		char changeMessage[] = "Data Type Changed to Gages\n";
		mLogFile.Write( changeMessage, strlen(changeMessage) );		
		WriteLogDataHeaders();
	}
}


void CSixAxisSensorDlg::OnBnClickedFt()
{
	// TODO: 在此添加控件通知处理程序代码
	int i;
	show_raw_voltages = false;
	for (i=0; i<6; i++) {
		if (i<=3)
		{
			mUnit[i].SetWindowText(_T("N"));
		}
		else
		{
			mUnit[i].SetWindowText(_T("N-m"));
		}
	}
	mLabelMax[0].SetWindowText(_T("580N"));
	mLabelMax[1].SetWindowText(_T("580N"));
	mLabelMax[2].SetWindowText(_T("1160N"));
	mLabelMax[3].SetWindowText(_T("20N-m"));
	mLabelMax[4].SetWindowText(_T("20N-m"));
	mLabelMax[5].SetWindowText(_T("20N-m"));
	mGroupForce.SetWindowText(_T("Force"));
	mGroupTorque.SetWindowText(_T("Torque"));
	mLabels[0].SetWindowText(_T("FX"));
	mLabels[1].SetWindowText(_T("FY"));
	mLabels[2].SetWindowText(_T("FZ"));
	mLabels[3].SetWindowText(_T("TX"));
	mLabels[4].SetWindowText(_T("TY"));
	mLabels[5].SetWindowText(_T("TZ"));
	if ( mLoggingEnabled )
	{
		char changeMessage[] = "Data Type Changed to Force/Torque\n";
		mLogFile.Write( changeMessage, strlen(changeMessage) );
		WriteLogDataHeaders();
	}
}

void CSixAxisSensorDlg::OnExit()
{
	// TODO: 在此添加命令处理程序代码
	CSixAxisSensorDlg::EndDialog(0);
}

//保存当前配置文件操作的事件响应函数
void CSixAxisSensorDlg::OnConfiguration()
{
	// TODO: 在此添加命令处理程序代码
	// When Saving a config, why not ask the user if they want to use that by default, i.e. change program options	
	
	CFileDialog fileChooser(FALSE, _T("*.ati"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "ATI Save Files (*.ati)|*.ati||", this);	
	char workingDirectory[200]; /*the working directory*/
	_getdcwd( _getdrive(), workingDirectory, 200 );
	fileChooser.m_ofn.lpstrInitialDir = workingDirectory;
	if (fileChooser.DoModal() != IDOK)
		return;
	
	ofstream fileStream; /* the file we're saving data to */
	fileStream.open( (LPCTSTR)fileChooser.GetPathName(), ios::binary );		

	ConfigureSaveData csd; /*holds the information we want saved, 
						with the exception of tool transforms */
	
	/*unfortunately, we're saving more than just the calibration
	information, so we can't use the loadscript and savescript methods
	of the ftsensor object*/

	csd.fileVersion = 1;	
	CString cstrDevName = 	mFTWrapper.getDAQSys()->getDeviceName();
	//csd.DAQDeviceName = mFTWrapper.getDAQSys()->getDeviceName();
	strcpy_s( csd.DAQDeviceName, cstrDevName.GetBuffer(100) );
	csd.DAQStartChannel = mFTWrapper.getDAQSys()->getFirstChannel();
	csd.DAQScanRate = mFTWrapper.getDAQSys()->getScanRate();	
	csd.screenTime = dlgProgramOptions->getTimer();
	csd.ADCBeep = dlgProgramOptions->getBeep();
	csd.showTherm = dlgProgramOptions->isThermistorVisible();
	strcpy_s( csd.calFilePath, (LPCTSTR)mFTWrapper.GetFTSensor()->GetCalFilePath() );	
	mFTWrapper.GetBiasVoltages( csd.biasVoltages );	
	strcpy_s( csd.forceUnits, (LPCTSTR)mFTWrapper.GetActiveCalibration().GetForceUnits() );
	strcpy_s( csd.torqueUnits, (LPCTSTR)mFTWrapper.GetActiveCalibration().GetTorqueUnits() );	
	csd.tempComp = 
		mFTWrapper.GetActiveCalibration().GetTempCompEnabled();	
	_ToolTransforms TTFs = 
		mFTWrapper.GetActiveCalibration().GetToolTransforms();
	csd.numTTFs = TTFs.GetCount();
	
	fileStream.write( (char *)&csd, sizeof(csd) );
	
	unsigned int i;
	/*precondition: all data besides tool transforms already
		written to file. TTFs initialized to the tool transform
		group being saved
	  postcondition: all tool transform data in TTFs will be written
		to file. i = number of tool transforms
	*/
	for (i = 0; i < csd.numTTFs; i++)
	{
		_ToolTransform ttf = TTFs.GetItem( COleVariant( (short)(i+1), VT_I2 ) );
		TTFSaveData ttfSave;
		strcpy_s( ttfSave.axis, (LPCTSTR)(ttf.GetAxis()) );
		strcpy_s( ttfSave.units, (LPCTSTR)(ttf.GetUnits()) );
		ttfSave.value = ttf.GetValue();
		
		fileStream.write( (char *)(&ttfSave), sizeof(TTFSaveData) );
	}

	fileStream.close();

	AskAboutAutoLoad( fileChooser.GetPathName() );
}

//打开配置文件操作的事件响应函数
void CSixAxisSensorDlg::OnOpenConfiguration()
{
	// TODO: 在此添加命令处理程序代码
	CFileDialog fileChooser(TRUE, _T("*.ati"), NULL, OFN_FILEMUSTEXIST, "ATI Files (*.ati)|*.ati||", this);
	char workingDirectory[200]; /*the working directory*/
	_getdcwd( _getdrive(), workingDirectory, 200 );
	fileChooser.m_ofn.lpstrInitialDir = workingDirectory; 
	if ( IDOK != fileChooser.DoModal() )
		return;

	loadConfiguration( (LPCTSTR)fileChooser.GetPathName() );

	AskAboutAutoLoad( fileChooser.GetPathName() );
}

/*loadConfiguration
loads a configuration, which consists of F/T settings as well
as program options.
arguments:  fileName - the file which contains the configuration
	data
*/
void CSixAxisSensorDlg::loadConfiguration(LPCTSTR fileName)
{
	bool need_to_reset_timer = false;
	if (this->KillTimer(TIMER_1)) {
		need_to_reset_timer = true;
	}

	ifstream fileStream; /*input file stream*/
	fileStream.open( fileName, ios::binary);

	/*unfortunately, we're saving more than just the calibration
	information, so we can't use the loadscript and savescript methods
	of the ftsensor object*/

	ConfigureSaveData csd;

	try
	{
		fileStream.read( (char *)&csd, sizeof(csd) );	

		mFTWrapper.getDAQSys()->inited( csd.DAQDeviceName,
			csd.DAQStartChannel, csd.DAQScanRate );	
		dlgProgramOptions->setTimer( csd.screenTime );
		dlgProgramOptions->setBeep( csd.ADCBeep );
		dlgProgramOptions->setThermistorVisible( csd.showTherm );
		mFTWrapper.LoadCalFile( csd.calFilePath );	
		mFTWrapper.SetBiasVoltages( csd.biasVoltages );		
		mFTWrapper.GetActiveCalibration().SetForceUnits( csd.forceUnits );
		mFTWrapper.GetActiveCalibration().SetTorqueUnits( csd.torqueUnits );	
		mFTWrapper.GetActiveCalibration().SetTempCompEnabled( csd.tempComp );
	
		_ToolTransforms TTFs = mFTWrapper.GetActiveCalibration().GetToolTransforms();
		unsigned int i;
		/*precondition: fileStream input is pointing to beginning of
			ttf section in file. TTFs is the tool transform collection
			of the sensor. csd contains the number of tool transforms
			in the file
		  postcondition:  all ttfs in file will be added to TTFs
		*/
		for (i = 0; i < csd.numTTFs; i++)
		{
			TTFSaveData ttfSave;
			fileStream.read ( (char*)(&ttfSave), sizeof(TTFSaveData) );
			TTFs.Add( ttfSave.axis, ttfSave.value, ttfSave.units );
		}	
		fileStream.close();

	} catch ( CException * e)
	{
		char caErrorMessage[2048];
		e->GetErrorMessage( caErrorMessage, 2048 );
		MessageBox( caErrorMessage, "Error Loading Configuration" );
		strcpy_s(csd.DAQDeviceName, "dev1" );
		strcpy_s(csd.torqueUnits, "N-m" );
		strcpy_s(csd.forceUnits, "N" );
		csd.DAQScanRate = 100;
		csd.DAQStartChannel = 0;
		csd.screenTime = 100;
		csd.ADCBeep = 1;
		csd.showTherm = true;
		mFTWrapper.getDAQSys()->inited(csd.DAQDeviceName, csd.DAQStartChannel,
			csd.DAQScanRate);
		dlgProgramOptions->setTimer( csd.screenTime );
		dlgProgramOptions->setBeep( csd.ADCBeep );
		dlgProgramOptions->setThermistorVisible( csd.showTherm );	
		
	}
	CMenu* menu = this->GetMenu();
	menu->EnableMenuItem(SETTINGS, MF_ENABLED);
	menu->EnableMenuItem(SENSOR_INFORMATION, MF_ENABLED);
	enableInputs();
	if (need_to_reset_timer) {
		this->SetTimer(TIMER_1,dlgProgramOptions->getTimer(),NULL); // the timer is actually rounded up to the nearest 55 ms, so this will be 110 ms
	}
}

/* asks whether or not to auto-load file at startup, writes value to registry */
void CSixAxisSensorDlg::AskAboutAutoLoad(CString filename)
{
	/*see if they want to load this configuration next time they
		start the application */
	int msgBoxResult = MessageBox(
		"Would you like to load this configuration the next time you start the demo?",
		"Auto-Load Configuration?",
		MB_YESNO );

	if ( IDYES == msgBoxResult )
	{
		/*SetRegValue( "AUTOLOAD", 
		(LPCTSTR)filename );*/
		mAutoLoadFilename = filename;
		dlgProgramOptions->setAutoLoad( true);
	}
}

//记录数据按钮的事件响应按钮
void CSixAxisSensorDlg::OnButtonlog()
{
	bool need_to_reset_timer = false;
	if (this->KillTimer(TIMER_1)) {
		need_to_reset_timer = true;
	}
	mLoggingEnabled = !mLoggingEnabled;
	if ( mLoggingEnabled )
	{
		CFileDialog fileChooser(FALSE, "*.log", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Log Files (*.log)|*.log||", this);	
		char workingDirectory[200]; /*the working directory*/
		_getdcwd( _getdrive(), workingDirectory, 200 );
		fileChooser.m_ofn.lpstrInitialDir = workingDirectory;
		if (fileChooser.DoModal() != IDOK)
			return;
		mLogFile.Open( (LPCTSTR)fileChooser.GetPathName(), 
			CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite);
		WriteLogDataHeaders();		
		m_buttonLog.SetWindowText( "Stop Logging" );
	}else{
		mLogFile.Close();
		m_buttonLog.SetWindowText( "Log Data" );
	}
	if (need_to_reset_timer) {
		this->SetTimer(TIMER_1,dlgProgramOptions->getTimer(),NULL); // the timer is actually rounded up to the nearest 55 ms, so this will be 110 ms				
	}
	UpdateData( FALSE );
}

/*将读到的数据存入记录文件里 adds the readings to the log file*/
void CSixAxisSensorDlg::AddToLogFile( double readings[] )
{
	CString dataString; /* the string written to file */	
	if ( dlgProgramOptions->isThermistorVisible() )
	{
		dataString.Format( "%-10.6f %-10.6f %-10.6f %-10.6f %-10.6f %-10.6f %-10.6f\n",
			readings[0], readings[1], readings[2], readings[3],
			readings[4], readings[5], readings[6]);
	}else
	{
		dataString.Format( "%-10.6f %-10.6f %-10.6f %-10.6f %-10.6f %-10.6f\n",
			readings[0], readings[1], readings[2], readings[3],
			readings[4], readings[5]);
	}
	mLogFile.Write( (LPCSTR)dataString, dataString.GetLength() );

}

/*writes field headers to log file (i.e. "G0..G6" or "Fx..Tz")*/
void CSixAxisSensorDlg::WriteLogDataHeaders()
{
	CString headerString; /* header line written to log file */
	if ( show_raw_voltages )
	{
		headerString.Format("%-10s %-10s %-10s %-10s %-10s %-10s",
			"G0", "G1", "G2", "G3", "G4", "G5");
	}else
	{
		headerString.Format("%-10s %-10s %-10s %-10s %-10s %-10s",
			_T("Fx"), _T("Fy"), _T("Fz"), _T("Tx"), _T("Ty"),_T( "Tz"));
	}
	if ( dlgProgramOptions->isThermistorVisible() )
	{
		headerString = headerString + " Thermistor\n";
	}else
	{
		headerString = headerString + "\n";
	}
	mLogFile.Write( (LPCTSTR)headerString, headerString.GetLength() );

}