
// GetFileInfo.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CGetFileInfoApp: 
// �йش����ʵ�֣������ GetFileInfo.cpp
//

class CGetFileInfoApp : public CWinApp
{
public:
	CGetFileInfoApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CGetFileInfoApp theApp;