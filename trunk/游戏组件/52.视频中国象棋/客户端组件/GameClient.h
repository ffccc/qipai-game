#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "Resource.h"
#include "GameClientDlg.h"

//////////////////////////////////////////////////////////////////////////

//Ӧ�ó�����
class CGameClientApp : public CGameFrameApp
{
	//��������
public:
	//���캯��
	CGameClientApp();
	//��������
	virtual ~CGameClientApp();

	//�̳к���
public:
	//����������
	virtual CGameFrameDlg * GetGameFrameDlg() { return new CGameClientDlg; }
};

//Ӧ�ó������
extern CGameClientApp theApp;

//��Ƶ����
#ifdef VIDEO_GAME
extern CVideoServiceManager g_VideoServiceManager;
#endif
//////////////////////////////////////////////////////////////////////////
