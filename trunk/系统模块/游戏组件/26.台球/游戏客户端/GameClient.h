// GameClient.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������
#include "GameClientDlg.h"


// CGameClientApp:
// �йش����ʵ�֣������ GameClient.cpp
//

class CGameClientApp : public CGameFrameApp
{
public:
	CGameClientApp();
	virtual ~CGameClientApp();

public:
	//����������
	virtual CGameFrameDlg * GetGameFrameDlg() 
	{ 
		return new CGameClientDlg; 
	}
};

extern CGameClientApp theApp;
