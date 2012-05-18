#ifndef GAME_ENGINE_HEAD_HEAD_FILE
#define GAME_ENGINE_HEAD_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
//�����ļ�

//MFC �ļ�
#include <Afxmt.h>
#include <AtlBase.h>
#include <AfxTempl.h>
#include <MMSystem.h>

//D3D �ļ�
#include <D3D9.h>
#include <D3Dx9.h>
#include <DSound.h>

//OGL �ļ�
#include <GL/GL.h>		
#include <GL/GLU.h>
#include <GL/GLAux.h>

//ƽ̨�ļ�
#include "..\..\ȫ�ֶ���\Platform.h"
#include "..\..\�ͻ������\����ؼ�\SkinControlHead.h"

//////////////////////////////////////////////////////////////////////////////////
//��������

//��������
#ifndef GAME_ENGINE_CLASS
	#ifdef  GAME_ENGINE_DLL
		#define GAME_ENGINE_CLASS _declspec(dllexport)
	#else
		#define GAME_ENGINE_CLASS _declspec(dllimport)
	#endif
#endif

//ģ�鶨��
#ifndef _DEBUG
	#define GAME_ENGINE_DLL_NAME		TEXT("GameEngine.dll")			//�������
#else
	#define GAME_ENGINE_DLL_NAME		TEXT("GameEngineD.dll")			//�������
#endif

//////////////////////////////////////////////////////////////////////////////////
//�ṹ����

//��Դ��Ϣ
struct tagResourceInfo
{
	DWORD							dwSize;								//��Դ��С
	LPVOID							pcbBuffer;							//��Դ�ڴ�
};

//////////////////////////////////////////////////////////////////////////////////
//�����ļ�

#ifndef GAME_ENGINE_DLL

	//D3D ����
	#include "D3DFont.h"
	#include "D3DDirect.h"
	#include "D3DDevice.h"
	#include "D3DSprite.h"
	#include "D3DSurface.h"
	#include "D3DTexture.h"

	//OGL ����
	//#include "OGLFont.h"
	//#include "OGLDevice.h"
	//#include "OGLTexture.h"

	//�����ļ�
	#include "D3DSound.h"
	#include "D3DMusic.h"
	#include "D3DSoundBuffer.h"
	#include "D3DSoundThread.h"

	//��Ϸ����
	#include "LapseCount.h"
	#include "VirtualEngine.h"
	#include "VirtualWindow.h"
	#include "VirtualButton.h"

#endif

//////////////////////////////////////////////////////////////////////////////////

#endif