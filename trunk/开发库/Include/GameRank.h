#ifndef GAMERANK_HEAD_FILE
#define GAMERANK_HEAD_FILE

//////////////////////////////////////////////////////////////////////////

//ƽ̨ͷ�ļ�
#include "Template.h"

//ȫ��ͷ�ļ�
#include "GlobalDef.h"

//////////////////////////////////////////////////////////////////////////
//�����궨��

//��������
#ifndef GAMERANK_CLASS
	#ifdef  GAMERANK_DLL
		#define GAMERANK_CLASS _declspec(dllexport)
	#else
		#define GAMERANK_CLASS _declspec(dllimport)
	#endif
#endif

//ģ�鶨��
#ifdef _DEBUG
	#define GAMERANK_DLL_NAME	TEXT("GameRankD.dll")					//��� DLL ����
#else
	#define GAMERANK_DLL_NAME	TEXT("GameRank.dll")					//��� DLL ����
#endif

//////////////////////////////////////////////////////////////////////////

#define VER_IGameRankManager INTERFACE_VERSION(1,1)
static const GUID IID_IGameRankManager={0x850e5617,0x4eab,0x4f2e,0xae,0x11,0xef,0xa3,0x22,0x65,0xe0,0xc2};

//��Ϸ�ȼ��ӿ�
interface IGameRankManager : public IUnknownEx
{
	//��Ϣ�ӿ�
public:
	//�����ж�
	virtual bool __cdecl IsLoadLevel()=NULL;
	//�ȼ���Ŀ
	virtual WORD __cdecl GetGameLevelCount()=NULL;

	//���ýӿ�
public:
	//���صȼ�
	virtual bool __cdecl LoadGameLevel(LPCTSTR pszDirectory, WORD wGameGenre=GAME_GENRE_SCORE)=NULL;
	//�ͷŵȼ�
	virtual bool __cdecl UnLoadGameLevel()=NULL;

	//�����ӿ�
public:
	//�ȼ�����
	virtual LPCTSTR __cdecl GetLevelDescribe(LONGLONG lScore)=NULL;

	//���ݽӿ�
public:
	//��ȡ�ȼ�
	virtual WORD __cdecl GetGameLevelItem(tagLevelItem LevelItem[], WORD wMaxCount)=NULL;
	//���õȼ�
	virtual bool __cdecl SetGameLevelItem(tagLevelItem LevelItem[], WORD wItemCount)=NULL;
};

//////////////////////////////////////////////////////////////////////////

//��ϵ��������
class CGameRankManagerHelper : public CTempldateHelper<IGameRankManager>
{
	//��������
public:
	//���캯��
	CGameRankManagerHelper(void) : CTempldateHelper<IGameRankManager>(IID_IGameRankManager,
		VER_IGameRankManager,GAMERANK_DLL_NAME,TEXT("CreateGameRankManager")) { }
};

//////////////////////////////////////////////////////////////////////////


#endif