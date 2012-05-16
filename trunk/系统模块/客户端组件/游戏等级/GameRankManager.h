#ifndef GAME_LEVEL_PARSER_HEAD_FILE
#define GAME_LEVEL_PARSER_HEAD_FILE

#pragma once

#include "GameRank.h"

//////////////////////////////////////////////////////////////////////////

//�ȼ�����
struct tagLevelDescribe
{
	LONG							lLevelScore;						//�ȼ�����
	TCHAR							szLevelName[16];					//�ȼ�����
};

//�ȼ�����
typedef CArrayTemplate<tagLevelDescribe *> CLevelDescribeArray;			//�ȼ�����	

//////////////////////////////////////////////////////////////////////////

//�ȼ�����
class CGameRankManager : public IGameRankManager
{
	//״̬����
protected:
	bool							m_bLoadLevel;						//���ر�־

	//�������
protected:
	CLevelDescribeArray				m_LevelDescribeActive;				//�����
	static CLevelDescribeArray		m_LevelDescribeStorage;				//�洢����

	//��������
public:
	//���캯��
	CGameRankManager();
	//��������
	virtual ~CGameRankManager();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID __cdecl Release() { delete this; }
	//�ӿڲ�ѯ
	virtual VOID * __cdecl QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//��Ϣ�ӿ�
public:
	//�����ж�
	virtual bool __cdecl IsLoadLevel() { return m_bLoadLevel; }
	//�ȼ���Ŀ
	virtual WORD __cdecl GetGameLevelCount() { return (WORD)m_LevelDescribeActive.GetCount(); }

	//���ýӿ�
public:
	//���صȼ�
	virtual bool __cdecl LoadGameLevel(LPCTSTR pszDirectory, WORD wGameGenre=GAME_GENRE_SCORE);
	//�ͷŵȼ�
	virtual bool __cdecl UnLoadGameLevel();

	//�����ӿ�
public:
	//�ȼ�����
	virtual LPCTSTR __cdecl GetLevelDescribe(LONGLONG lScore);

	//���ݽӿ�
public:
	//��ȡ�ȼ�
	virtual WORD __cdecl GetGameLevelItem(tagLevelItem LevelItem[], WORD wMaxCount);
	//���õȼ�
	virtual bool __cdecl SetGameLevelItem(tagLevelItem LevelItem[], WORD wItemCount);

	//�ڲ�����
private:
	//Ĭ�ϵȼ�
	LPCTSTR GetDefaultLevel(LONGLONG lScore);
	//��ȡ����
	tagLevelDescribe * CreateLevelDescribe();
};

//////////////////////////////////////////////////////////////////////////

#endif