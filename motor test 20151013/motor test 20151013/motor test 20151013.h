
// motor test 20151013.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// Cmotortest20151013App:
// �йش����ʵ�֣������ motor test 20151013.cpp
//

class Cmotortest20151013App : public CWinApp
{
public:
	Cmotortest20151013App();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern Cmotortest20151013App theApp;