
#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "Resource.h"
#include "BJGameDlg.h"

//////////////////////////////////////////////////////////////////////////

//Ӧ�ó�����
class CBJApp : public CGameFrameApp
{
	//��������
public:
	//���캯��
	CBJApp();
	//��������
	virtual ~CBJApp();

	//�̳к���
public:
	//����������
	virtual CGameFrameDlg * GetGameFrameDlg() { return new CBJGameDlg; }
	
};

//Ӧ�ó������
extern CBJApp theApp;

