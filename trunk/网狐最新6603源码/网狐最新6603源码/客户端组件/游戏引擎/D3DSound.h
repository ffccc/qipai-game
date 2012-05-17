#ifndef D3D_SOUND_HEAD_FILE
#define D3D_SOUND_HEAD_FILE

#include "GameEngineHead.h"
#include "D3DSoundBuffer.h"
#include "D3DSoundThread.h"

//////////////////////////////////////////////////////////////////////////////////

//��˵��
typedef CWHArray<CD3DSoundBuffer *> CD3DSoundBufferArray;

//////////////////////////////////////////////////////////////////////////////////

//��������
class GAME_ENGINE_CLASS CD3DSound
{
	//��Ԫ����
	friend class CD3DSoundBuffer;
	friend class CD3DSoundThread;

	//�ӿڱ���
protected:
	IDirectSound8 *					m_pIDirectSound;					//�豸����

	//�ٽ����
protected:
	CCriticalSection				m_CriticalSection;					//��������

	//�������
protected:
	CD3DSoundThread					m_D3DSoundThread;					//�����߳�
	CD3DSoundBufferArray			m_D3DSoundBufferActive;				//��������
	CD3DSoundBufferArray			m_D3DSoundBufferBuffer;				//��������

	//��̬����
public:
	static CD3DSound *				m_pD3DSound;						//����ָ��

	//��������
public:
	//���캯��
	CD3DSound();
	//��������
	virtual ~CD3DSound();

	//���ܺ���
public:
	//��������
	bool CreateD3DSound(HWND hWnd);

	//��������
public:
	//��������
	bool PlaySound(LPCTSTR pszFileName, bool bRepetition);
	//��������
	bool PlaySound(HINSTANCE hInstance, LPCTSTR pszResource, LPCTSTR pszTypeName, bool bRepetition);

	//�ڲ�����
private:
	//��ȡ����
	bool CreateSoundBuffer(CD3DSoundBuffer * & pD3DSoundBuffer);
	//ɾ������
	bool DeleteSoundBuffer(CD3DSoundBuffer * & pD3DSoundBuffer);

	//��̬����
public:
	//��ȡ����
	static CD3DSound * GetInstance() { return m_pD3DSound; }
};

//////////////////////////////////////////////////////////////////////////////////

#endif