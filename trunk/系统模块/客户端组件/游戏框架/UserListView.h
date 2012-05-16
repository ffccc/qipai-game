#ifndef USER_LIST_VIEW_HEAD_FILE
#define USER_LIST_VIEW_HEAD_FILE

#pragma once

#include "GameFrame.h"

//�궨��
#define	MAX_COLUMN						32									//�������
#define	IMGAE_SPACE						27									//��Ԫͼ���

//////////////////////////////////////////////////////////////////////////

//�û��б���ͼ
class GAME_FRAME_CLASS CUserListView : public CSkinListCtrl
{
public:
	WORD								m_wGameGenre;						//��Ϸ����
	WORD								m_wKindID;							//���ͱ�ʶ
	CGameRankManagerHelper				m_GameRankManagerHelper;			//��Ϸ�ȼ�

	//��Դ����
protected:
	static CImageList					m_StatusImage;						//״̬λͼ

	//�б���Ϣ
protected:
	BYTE								m_cbAscendSort;						//�����־
	WORD								m_wColumnCount;						//�б���Ŀ
	WORD								m_wDataDescribe[MAX_COLUMN];		//��������

	//��������
public:
	//���캯��
	CUserListView();
	//��������
	virtual ~CUserListView();

	//���ܺ���
public:
	//��ʼ���б�
	bool InitUserListView();
	//�����û�
	bool InsertUserItem(tagUserData * pUserData);
	//�����û�
	bool UpdateUserItem(tagUserData * pUserData);
	//ɾ���û�
	bool DeleteUserItem(tagUserData * pUserData);

	//���غ���
public:
	//�滭����
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	//�ڲ�����
protected:
	//��ȡλͼ
	int GetImageStation(tagUserData * pUserData);
	//��ȡ����
	int GetInsertStation(tagUserData * pUserData);
	//��д����
	void FillUserListItem(int iItem, tagUserData * pUserData);
	//��ȡ�ַ�
	LPCTSTR GetDisplayText(tagUserData * pUserData, WORD wColumnPos);
	//���к���
	static int CALLBACK SortFun(LPARAM lParam1, LPARAM lParam2, LPARAM lParamList);

	//��Ϣӳ��
protected:
	//����б�
	afx_msg void OnColumnclick(NMHDR * pNMHDR, LRESULT * pResult);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif