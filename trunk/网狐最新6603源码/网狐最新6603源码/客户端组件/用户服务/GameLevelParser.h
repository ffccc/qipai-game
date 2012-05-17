#ifndef GAME_LEVEL_PARSER_HEAD_FILE
#define GAME_LEVEL_PARSER_HEAD_FILE

#pragma once

#include "UserServiceHead.h"

//////////////////////////////////////////////////////////////////////////////////

//�ȼ�����
struct tagLevelDescribe
{
	LONG							lLevelScore;						//�ȼ�����
	TCHAR							szLevelName[16];					//�ȼ�����
};

//�ȼ�����
typedef CWHArray<tagLevelDescribe *> CLevelDescribeArray;				//�ȼ�����	

//////////////////////////////////////////////////////////////////////////////////

//�ȼ�����
class USER_SERVICE_CLASS CGameLevelParser : public IGameLevelParser
{
	//��������
protected:
	TCHAR							m_szKindName[LEN_KIND];				//��Ϸ����

	//�������
protected:
	CLevelDescribeArray				m_LevelDescribeActive;				//�����
	static CLevelDescribeArray		m_LevelDescribeStorage;				//�洢����

	//��������
public:
	//���캯��
	CGameLevelParser();
	//��������
	virtual ~CGameLevelParser();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { delete this; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//���ýӿ�
public:
	//��Ϸ����
	virtual VOID SetKindName(LPCTSTR pszKindName);
	//��Ϸ����
	virtual LPCTSTR GetKindName() { return m_szKindName; }

	//�����ӿ�
public:
	//�ȼ�����
	virtual LPCTSTR GetLevelDescribe(SCORE lScore);
	//�ȼ�����
	virtual LPCTSTR GetLevelDescribe(IClientUserItem * pIClientUserItem);

	//���ݽӿ�
public:
	//��ȡ�ȼ�
	virtual WORD GetGameLevelItem(tagLevelItem LevelItem[], WORD wMaxCount);
	//���õȼ�
	virtual bool SetGameLevelItem(tagLevelItem LevelItem[], WORD wItemCount);
	//���صȼ�
	virtual bool LoadGameLevelItem(LPCTSTR pszKindName, LPCTSTR pszDirectory, WORD wGameGenre);

	//�ڲ�����
private:
	//Ĭ�ϵȼ�
	LPCTSTR GetDefaultLevel(SCORE lScore);
	//��ȡ����
	tagLevelDescribe * CreateLevelDescribe();
};

//////////////////////////////////////////////////////////////////////////////////

#endif