#ifndef D3D_SOUND_BUFFER_HEAD_FILE
#define D3D_SOUND_BUFFER_HEAD_FILE

#pragma once

#include "GameEngineHead.h"

//////////////////////////////////////////////////////////////////////////////////

//声音信息
struct tagWaveDataInfo
{
	DWORD							dwWaveSize;							//数据大小
	LPVOID							pcbWaveData;						//数据指针
	WAVEFORMATEX					WaveFormatEx;						//声音格式
};

//////////////////////////////////////////////////////////////////////////////////

//声音缓冲
class GAME_ENGINE_CLASS CD3DSoundBuffer
{
	//友元定义
	friend class CD3DSoundThread;

	//内核变量
protected:
	HANDLE							m_hEventSound;						//事件对象
	IDirectSoundBuffer *			m_pIDirectSoundBuffer;				//缓冲对象

	//函数定义
public:
	//构造函数
	CD3DSoundBuffer();
	//析构函数
	virtual ~CD3DSoundBuffer();

	//播放管理
public:
	//停止播放
	bool Stop();
	//播放声音
	bool PlaySound(DWORD dwStartPosition, bool bRepetition);

	//加载管理
public:
	//重置声音
	bool ResetResource();
	//加载声音
	bool LoadSoundResource(LPCTSTR pszFileName);
	//加载声音
	bool LoadSoundResource(HINSTANCE hInstance, LPCTSTR pszResource, LPCTSTR pszTypeName);

	//功能函数
private:
	//填充缓冲
	bool FillSoundBuffer(VOID * pSoundData, DWORD dwSoundSize);
	//声音属性
	bool GetWaveDataInfo(LPVOID pDataBuffer, tagWaveDataInfo & WaveDataInfo);

	//创建缓冲
private:
	//创建缓冲
	bool CreateSoundBuffer(tagWaveDataInfo & WaveDataInfo, DWORD dwFlags);
	//创建缓冲
	bool CreateSoundBuffer(INT nChannels, INT nSecs, DWORD nSamplesPerSec, WORD wBitsPerSample, DWORD dwFlags);
};

//////////////////////////////////////////////////////////////////////////////////

#endif