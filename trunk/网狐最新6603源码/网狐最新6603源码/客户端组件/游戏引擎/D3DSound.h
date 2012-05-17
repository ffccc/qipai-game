#ifndef D3D_SOUND_HEAD_FILE
#define D3D_SOUND_HEAD_FILE

#include "GameEngineHead.h"
#include "D3DSoundBuffer.h"
#include "D3DSoundThread.h"

//////////////////////////////////////////////////////////////////////////////////

//类说明
typedef CWHArray<CD3DSoundBuffer *> CD3DSoundBufferArray;

//////////////////////////////////////////////////////////////////////////////////

//声音对象
class GAME_ENGINE_CLASS CD3DSound
{
	//友元定义
	friend class CD3DSoundBuffer;
	friend class CD3DSoundThread;

	//接口变量
protected:
	IDirectSound8 *					m_pIDirectSound;					//设备对象

	//临界变量
protected:
	CCriticalSection				m_CriticalSection;					//锁定对象

	//组件变量
protected:
	CD3DSoundThread					m_D3DSoundThread;					//声音线程
	CD3DSoundBufferArray			m_D3DSoundBufferActive;				//声音缓冲
	CD3DSoundBufferArray			m_D3DSoundBufferBuffer;				//声音缓冲

	//静态变量
public:
	static CD3DSound *				m_pD3DSound;						//对象指针

	//函数定义
public:
	//构造函数
	CD3DSound();
	//析构函数
	virtual ~CD3DSound();

	//功能函数
public:
	//创建环境
	bool CreateD3DSound(HWND hWnd);

	//声音播放
public:
	//播放声音
	bool PlaySound(LPCTSTR pszFileName, bool bRepetition);
	//播放声音
	bool PlaySound(HINSTANCE hInstance, LPCTSTR pszResource, LPCTSTR pszTypeName, bool bRepetition);

	//内部函数
private:
	//获取对象
	bool CreateSoundBuffer(CD3DSoundBuffer * & pD3DSoundBuffer);
	//删除对象
	bool DeleteSoundBuffer(CD3DSoundBuffer * & pD3DSoundBuffer);

	//静态函数
public:
	//获取对象
	static CD3DSound * GetInstance() { return m_pD3DSound; }
};

//////////////////////////////////////////////////////////////////////////////////

#endif