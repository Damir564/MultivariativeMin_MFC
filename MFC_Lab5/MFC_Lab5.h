
// MFC_Lab5.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CMFCLab5App:
// See MFC_Lab5.cpp for the implementation of this class
//

class CMFCLab5App : public CWinApp
{
public:
	CMFCLab5App();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CMFCLab5App theApp;
