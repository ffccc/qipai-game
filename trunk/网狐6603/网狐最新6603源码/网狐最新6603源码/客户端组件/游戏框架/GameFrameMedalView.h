#ifndef GAME_FRAME_MEDAL_HEAD_FILE
#define GAME_FRAME_MEDAL_HEAD_FILE

#pragma once

#include "GameFrameHead.h"

//////////////////////////////////////////////////////////////////////////////////

//�û���Ϣ
class GAME_FRAME_CLASS CGameFrameMedalView : public CWnd
{
	//��������
protected:
	DWORD							m_dwUserMedal;						//�û�����

	//��������
public:
	//���캯��
	CGameFrameMedalView();
	//��������
	virtual ~CGameFrameMedalView();

	//���ܺ���
public:
	//���ý���
	VOID SetUserMedal(DWORD dwUserMedal);

	//��Ϣӳ��
protected:
	//�滭����
	BOOL OnEraseBkgnd(CDC * pDC);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif