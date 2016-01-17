#pragma once

#include "atidaqft.h"
// CPopToolTransform 对话框

class CPopToolTransform : public CDialogEx
{
	DECLARE_DYNAMIC(CPopToolTransform)

public:
	CPopToolTransform(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPopToolTransform();
	BOOL OnInitDialog();
	/* This constructor is used for editing an existing Tool Transform */
	CPopToolTransform(_ToolTransform* tool, CWnd* pParent = NULL);
	/* This constructor is used for creating a new Tool Transform */
	CPopToolTransform(_ToolTransforms* tools, CWnd* pParent = NULL);
// 对话框数据
	enum { IDD = IDD_TOOL_TRANSFORM };
	CEdit	mEditValue;
	CComboBoxEx	mComboUnits;
	CComboBoxEx	mComboAxis;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	/* a pointer to the list of Tool Transforms, NULL if editing */
	_ToolTransforms* tools;
	/* a pointer to an existing Tool Transform, NULL if creating new */
	_ToolTransform* tool;
	/* this boolean tracks whether this object is editing (true) or creating (false) a tool transform */
	bool edit;
	/* common code to both constructors */
	void AFX_DATA_INIT();
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnCbnSelchangeAxis();
};
