#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "Resource.h"
#include "GameClientDlg.h"

//////////////////////////////////////////////////////////////////////////

//Ӧ�ó�����
class CGameClientApp : public CWinApp
{
private:
	CGameClientDlg					* m_pGameFrame;					//��ܴ���
	CSkinRecordFile					m_SkinRecordFile;				//��������

	//��������
public:
	//���캯��
	CGameClientApp();
	//��������
	virtual ~CGameClientApp();

	//�̳к���
public:
	//����������
	virtual CGameClientDlg * GetGameFrameDlg() { return new CGameClientDlg; }

//���غ���
public:
	//��ʼ������
	virtual BOOL InitInstance();
	//�˳���Ϣ
	virtual int ExitInstance();
};

//Ӧ�ó������
extern CGameClientApp theApp;

//////////////////////////////////////////////////////////////////////////
