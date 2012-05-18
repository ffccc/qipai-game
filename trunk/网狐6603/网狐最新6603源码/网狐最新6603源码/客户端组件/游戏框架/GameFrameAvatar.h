#ifndef GAME_FRAME_AVATAR_HEAD_FILE
#define GAME_FRAME_AVATAR_HEAD_FILE

#pragma once

#include "GameFrameHead.h"

//////////////////////////////////////////////////////////////////////////////////

//��Ϸͷ��
class GAME_FRAME_CLASS CGameFrameAvatar
{
	//��Դ����
protected:
	WORD							m_wD3DLineCount;					//ͷ����Ŀ
	WORD							m_wD3DFullCount;					//ͷ����Ŀ
	CD3DTexture						m_D3DTextureAvatar;					//ͷ������

	//��̬����
protected:
	static CGameFrameAvatar *		m_pGameFrameAvatar;					//����ӿ�

	//��������
public:
	//���캯��
	CGameFrameAvatar();
	//��������
	virtual ~CGameFrameAvatar();

	//���ýӿ�
public:
	//���ú���
	VOID Initialization(CD3DDevice * pD3DDevice);

	//�滭����
public:
	//�滭ͷ��
	VOID DrawUserAvatar(CDC * pDC, INT nXPos, INT nYPos, WORD wFaceID);
	//�滭ͷ��
	VOID DrawUserAvatar(CDC * pDC, INT nXPos, INT nYPos, IClientUserItem * pIClientUserItem);

	//�滭����
public:
	//�滭ͷ��
	VOID DrawUserAvatar(CD3DDevice * pD3DDevice, INT nXPos, INT nYPos, WORD wFaceID);
	//�滭ͷ��
	VOID DrawUserAvatar(CD3DDevice * pD3DDevice, INT nXPos, INT nYPos, IClientUserItem * pIClientUserItem);

	//��̬����
public:
	//��ȡʵ��
	static CGameFrameAvatar * GetInstance() { return m_pGameFrameAvatar; }
};

//////////////////////////////////////////////////////////////////////////////////

#endif