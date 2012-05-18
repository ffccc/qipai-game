#ifndef D3D_SOUND_THREAD_HEAD_FILE
#define D3D_SOUND_THREAD_HEAD_FILE

#pragma once

#include "GameEngineHead.h"

//////////////////////////////////////////////////////////////////////////////////

//��˵��
class CD3DSound;

//�����߳�
class GAME_ENGINE_CLASS CD3DSoundThread : public CWHThread
{
	//��������
protected:
	CD3DSound *						m_pD3DSound;						//��������

	//��������
public:
	//���캯��
	CD3DSoundThread();
	//��������
	virtual ~CD3DSoundThread();

	//�¼�����
protected:
	//�����¼�
	virtual bool OnEventThreadRun();

	//���ܺ���
public:
	//���ö���
	VOID SetD3DSound(CD3DSound * pD3DSound) { m_pD3DSound=pD3DSound; }
};

//////////////////////////////////////////////////////////////////////////////////

#endif