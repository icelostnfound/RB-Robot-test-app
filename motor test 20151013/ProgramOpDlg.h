#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "TabGeneral.h"
#include "TabNIDAQ.h"
#include "TabStartup.h"
#include "FTWrapper.h"

// CProgramOpDlg 对话框

class CProgramOpDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CProgramOpDlg)

public:
	BOOL OnInitDialog();
	CProgramOpDlg(FTWrapper* wrapper, CWnd* pParent = NULL);   // standard constructor
	/* returns the setting for whether the program should beep to indicate A/D saturation */
	bool getBeep() { return beep; }
	/* sets whether the program should beep to indicate saturation */
	void setBeep( bool newBeep ) { beep = newBeep; }
	/* returns the setting to use for the timer (that calls for a new scan and updates the screen) */
	int getTimer() { return timer; }
	/* sets the timer that updates the screen */
	void setTimer(int newTime) { timer = newTime; }
	/* returns whether the Thermistor readout should be displayed */
	bool isThermistorVisible() { return thermistor; }
	/* sets whether the thermistor readout should be displayed */
	void setThermistorVisible( bool newTherm ) { thermistor = newTherm; }
	/* sets whether we auto-load a configuration at program startup */
	void setAutoLoad( bool newAutoLoad ) { autoLoad = newAutoLoad; }
	/* returns whether we auto-load a configuration at program startup */
	bool getAutoLoad( ) { return autoLoad; }
// 对话框数据
	enum { IDD = IDD_PROGRAM_OPTIONS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl mProgramOptionsTab;
	CStatic mFolder;
private:
	/*whether or not to auto-load a configuration at startup*/
	bool autoLoad;
	/* a pointer to the FTWrapper that hides much of the complexity of the _FTSensor class */
	FTWrapper* wrapper;
	/* this variable holds the scan rate for the DAQ */
	int scanRate;
	/* this variable holds the first channel index for the DAQ */
	int firstChannel;
	/* this variable holds the device name of the DAQ */
	CString device;
	/* this is an object representing one of the tab frames - General Options */
	CTabGeneral mTabGeneral;
	/* this is an object representing one of the tab frames - NI-DAQ Options */
	CTabNIDAQ mTabNIDAQ;
	/* this is an object representing one of the tab frames - Startup Options */
	CTabStartup mTabStartup;
	/* this variable tracks whether the user wants a beep to indicate A/D saturation */
	bool beep;
	/* this variable tracks whether the user wants the thermistor value displayed */
	bool thermistor;
	/* this variable tracks how often the user wanst the DAQ to be scanned and the voltages updated */
	int timer;
	/* this variable records the location of a default config file */
	CString configFile;
	/* this variable tracks whether the user wants the default config file loaded */
	bool loadConfig;
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnSelchangeProgramOptionsTab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCancel();
};
