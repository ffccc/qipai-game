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
private:
	//CGameFrameDlg					* m_pGameFrame;					//��ܴ���
	//CSkinRecordFile					m_SkinRecordFile;				//��������

	//��������
public:
	//���캯��
	CGameClientApp();
	//��������
	virtual ~CGameClientApp();

	//���غ���
public:
	//��ʼ������
	//virtual BOOL InitInstance();
	//�˳���Ϣ
	//virtual int ExitInstance();

	//�̳к���
public:
	//����������
	virtual CGameFrameDlg * GetGameFrameDlg() { return new CGameClientDlg; }
	//DECLARE_MESSAGE_MAP()
};

//Ӧ�ó������
extern CGameClientApp theApp;

//////////////////////////////////////////////////////////////////////////
