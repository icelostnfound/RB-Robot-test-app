#pragma once
#include "atidaqft.h"
#include "FTWrapper.h"

// CSixAxisSensorInfoDlg �Ի���

class CSixAxisSensorInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSixAxisSensorInfoDlg)

public:
	CSixAxisSensorInfoDlg(FTWrapper* ft,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSixAxisSensorInfoDlg();

// �Ի�������
	enum { IDD = IDD_SENSOR_INFORMATION };
	CStatic	mLabelTemperatureCompensation;
	CStatic	mLabelOutputRange;
	CStatic	mLabelOutputPolarity;
	CStatic	mLabelOutputMode;
	CStatic	mLabelCalibrationDate;
	CListBox	mListCalibrations;
	CStatic	mLabelNumChannels;
	CStatic	mLabelFamily;
	CStatic	mLabelSerialNumber;
	CStatic	mLabelBodyStyle;
	//}}AFX_DATA
	CStatic	mLabelAxisMax[6];
	CStatic	mLabelAxis[6];
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
private:
	/* a method that is called when a new active calibration is to be displayed (or on startup) */
	void UpdateCalibrationInfo();
	_FTSensor* mFTSensor;
	/* a pointer to the FTWrapper that hides much of the complexity of the _FTSensor class */
	FTWrapper* mFTWrapper;
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnLbnSelchangeCalibrations();
};
