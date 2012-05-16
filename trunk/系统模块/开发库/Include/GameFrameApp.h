#ifndef GAME_FRAME_APP_HEAD_FILE
#define GAME_FRAME_APP_HEAD_FILE

#pragma once

#include "GameFrame.h"
#include "GameFrameDlg.h"

//////////////////////////////////////////////////////////////////////////

//Ӧ�ó�����
class GAME_FRAME_CLASS CGameFrameApp : public CWinApp
{
	//��������
private:
	CGameFrameDlg					* m_pGameFrame;					//��ܴ���

	//��������
public:
	//���캯��
	CGameFrameApp();
	//��������
	virtual ~CGameFrameApp();

	//���غ���
public:
	//��ʼ������
	virtual BOOL InitInstance();
	//�˳���Ϣ
	virtual int ExitInstance();

	//�̳к���
public:
	//����������
	virtual CGameFrameDlg * GetGameFrameDlg()=NULL;

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif