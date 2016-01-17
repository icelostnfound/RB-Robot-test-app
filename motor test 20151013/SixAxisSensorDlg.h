#pragma once
#include "afxwin.h"
//{{AFX_INCLUDES()
#include "atidaqft.h"	// Added by ClassView
#include "FTWrapper.h"	// Added by ClassView
//}}AFX_INCLUDES
#include "ProgramOpDlg.h"
#include "SixAixsSettingDlg.h"

class CVisualCSampleDlgAutoProxy;
class DAQSys;
// CSixAxisSensorDlg 对话框

class CSixAxisSensorDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSixAxisSensorDlg);
	friend class CVisualCSampleDlgAutoProxy;

public:
	CSixAxisSensorDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSixAxisSensorDlg();
	friend class Cmotortest20151013Dlg;

// 对话框数据
	enum { IDD = IDD_SIXAXIS_DIALOG };
	//CButton	m_buttonLog;
	//CStatic	mLabelThermistor;
	CButton	mGroupTorque;
	CButton	mGroupForce;
	CButton	mUnbias;
	CButton	mBias;
	CStatic	mLabelMessage;
	int		m_display;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	CVisualCSampleDlgAutoProxy* m_pAutoProxy;
	HICON m_hIcon;
	//virtual BOOL OnInitDialog();

	BOOL CanExit();
	// Generated message map functions
	//{{AFX_MSG(CVisualCSampleDlg)
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnToolsSettings();
	afx_msg void OnToolsSensorinformation();
	afx_msg void OnToolsProgramoptions();
private :
	/*writes field headers to log file (i.e. "G0..G6" or "Fx..Tz")*/
	void WriteLogDataHeaders();
	/*adds the readings to the log file*/
	void AddToLogFile( double readings[] );
	/*whether or not we're currently logging data*/
	bool mLoggingEnabled;
	/*the file logged data is stored in*/
	CFile mLogFile;
	/* asks whether or not to auto-load file at startup, writes value to registry */
	void AskAboutAutoLoad( CString filename );
	/* the configuration file to auto-load at startup */
	CString mAutoLoadFilename;
	/* loads a configuration file */
	void loadConfiguration(LPCTSTR fileName);
	/* an object that handles selection of the Settings menu item */
	CSixAixsSettingDlg* dlgSettings;
	/* an object that handles selection of the Program Options menu item */
	CProgramOpDlg* dlgProgramOptions;
	//CProgramOpDlg  ProgramOptions;
	/* a pointer to the FTWrapper that hides much of the complexity of the _FTSensor class */
	FTWrapper mFTWrapper;
	/* this procedure enables/disables various controls base on the current state */
	void enableInputs();
	/* this bool tracks whether raw voltages or forces/torques should be displayed */
	bool show_raw_voltages;
	//	/* an internal function used to convert progress bar readings back to floats */
	//	double itof(int i);
	/* an internal function used to convert floats into progress bar readings */
	int ftoi(double f, short channel);
	/* a list of the static text controls used to display the voltages or forces/torques */
	//CStatic  mValues[6];
	/* a list of static text controls used to display the gauge id's of the F/T axes */
	CStatic	mLabels[6];
	/* a list of the progress bar controls that (poorly) displays the voltage reading */
	CStatic mLabelMax[6];
	CStatic mUnit[6];
	FTWrapper* m_FTWrapper;
	/* the heard and soul of the screen update, this procedure reads the voltages passed in and updates the various components on the screen */
	void UpdateGauges(double* readings);
	/* called during OnInitDialog - a centralized place for added startup code */
	
public:
	afx_msg void OnBnClickedBias();
	afx_msg void OnBnClickedUnbias();
	afx_msg void OnBnClickedVoltages();
	afx_msg void OnButtonlog();
	afx_msg void OnBnClickedFt();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnCalibration();
	afx_msg void OnExit();
	afx_msg void OnConfiguration();
	afx_msg void OnOpenConfiguration();
	virtual BOOL OnInitDialog();
	void CustomInitialization();
	double t;
	 double FTT;
	CStatic  mValues[6];
	CStatic mLabelThermistor;
	CButton m_buttonLog;
};
