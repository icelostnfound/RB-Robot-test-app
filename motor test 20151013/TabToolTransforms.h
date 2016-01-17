#pragma once
#include "atidaqft.h"	// Added by ClassView
#include "FTWrapper.h"

// CTabToolTransforms 对话框

class CTabToolTransforms : public CDialogEx
{
	DECLARE_DYNAMIC(CTabToolTransforms)

public:
	CTabToolTransforms(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTabToolTransforms();
	BOOL OnInitDialog();
	void SetFTWrapper(FTWrapper* wrapper);
// 对话框数据
	enum { IDD = TAB_TOOL_TRANSFORMS };
	CListBox	mListTransforms;
	CButton	mButtonRemoveTransform;
	CButton	mButtonRemoveAllTransforms;
	CButton	mButtonEditTransform;
	CButton	mButtonAddTransform;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	/* update the list of tool transforms for the active calibration */
	void updateList();
	/* the list of Tool Transforms for the active Calibration */
	_ToolTransforms tools;
	/* the active Calibration */
	_Calibration calibration;
	/* a pointer to the FTWrapper that hides much of the complexity of the _FTSensor class */
	FTWrapper* wrapper;
public:
	afx_msg void OnBnClickedAddTransform();
	afx_msg void OnBnClickedEditTransform();
	afx_msg void OnBnClickedRemoveTransform();
	afx_msg void OnBnClickedRemoveAllTransforms();
};
