#pragma once

#ifndef __AFXWIN_H__
#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "Resource.h"
#include "GameClientDlg.h"

//////////////////////////////////////////////////////////////////////////

//Ӧ�ó�����
class CParrentApp : public CWinApp
{
	//��������
private:
	CGameParrentDlg					* m_pGameFrame;					//��ܴ���
	CSkinRecordFile					m_SkinRecordFile;				//��������

	//��������
public:
	//���캯��
	CParrentApp();
	//��������
	virtual ~CParrentApp();

	//���غ���
public:
	//��ʼ������
	virtual BOOL InitInstance();
	//�˳���Ϣ
	virtual int ExitInstance();

	//�̳к���
public:
	//����������
	virtual CGameParrentDlg * GetGameFrameDlg()=NULL;

	DECLARE_MESSAGE_MAP()
};

//Ӧ�ó������
//extern CParrentApp theApp;

//////////////////////////////////////////////////////////////////////////
