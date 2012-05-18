#ifndef D3D_SOUND_BUFFER_HEAD_FILE
#define D3D_SOUND_BUFFER_HEAD_FILE

#pragma once

#include "GameEngineHead.h"

//////////////////////////////////////////////////////////////////////////////////

//������Ϣ
struct tagWaveDataInfo
{
	DWORD							dwWaveSize;							//���ݴ�С
	LPVOID							pcbWaveData;						//����ָ��
	WAVEFORMATEX					WaveFormatEx;						//������ʽ
};

//////////////////////////////////////////////////////////////////////////////////

//��������
class GAME_ENGINE_CLASS CD3DSoundBuffer
{
	//��Ԫ����
	friend class CD3DSoundThread;

	//�ں˱���
protected:
	HANDLE							m_hEventSound;						//�¼�����
	IDirectSoundBuffer *			m_pIDirectSoundBuffer;				//�������

	//��������
public:
	//���캯��
	CD3DSoundBuffer();
	//��������
	virtual ~CD3DSoundBuffer();

	//���Ź���
public:
	//ֹͣ����
	bool Stop();
	//��������
	bool PlaySound(DWORD dwStartPosition, bool bRepetition);

	//���ع���
public:
	//��������
	bool ResetResource();
	//��������
	bool LoadSoundResource(LPCTSTR pszFileName);
	//��������
	bool LoadSoundResource(HINSTANCE hInstance, LPCTSTR pszResource, LPCTSTR pszTypeName);

	//���ܺ���
private:
	//��仺��
	bool FillSoundBuffer(VOID * pSoundData, DWORD dwSoundSize);
	//��������
	bool GetWaveDataInfo(LPVOID pDataBuffer, tagWaveDataInfo & WaveDataInfo);

	//��������
private:
	//��������
	bool CreateSoundBuffer(tagWaveDataInfo & WaveDataInfo, DWORD dwFlags);
	//��������
	bool CreateSoundBuffer(INT nChannels, INT nSecs, DWORD nSamplesPerSec, WORD wBitsPerSample, DWORD dwFlags);
};

//////////////////////////////////////////////////////////////////////////////////

#endif