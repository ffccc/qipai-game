#pragma once

#include "Stdafx.h"
#include "UserManager.h"

//�궨��
#define	MAX_COLUMN						32									//�������
#define	IMGAE_SPACE						27									//��Ԫͼ���

//////////////////////////////////////////////////////////////////////////

//�û��б���ͼ
class CUserListView : public CSkinListCtrl
{
public:
	WORD								m_wGameGenre;						//��Ϸ����
	WORD								m_wKindID;							//���ͱ�ʶ

	//��Դ����
protected:
	UINT								m_uImageSpace;						//λͼ���
	BYTE								m_cbAscendSort;						//�����־
	static CImageList					m_StatusImage;						//״̬λͼ

	//�б���Ϣ
protected:
	WORD								m_wColumnCount;						//�б���Ŀ
	WORD								m_wDataDescribe[MAX_COLUMN];		//��������

	//�������
public:
	CGameRankManagerHelper				m_GameRankManager;					//��Ϸ�ȼ�

	//��������
public:
	//���캯��
	CUserListView();
	//��������
	virtual ~CUserListView();

	//���ú���
public:
	//��ʼ���б�
	bool InitUserListView();
	//��������Ϣ
	bool SetColumnDescribe(tagColumnItem ColumnItem[], WORD wColumnCount);

	//���ܺ���
public:
	//�����û�
	bool InsertUserItem(IUserItem * pIUserItem);
	//�����û�
	bool UpdateUserItem(IUserItem * pIUserItem);
	//ɾ���û�
	bool DeleteUserItem(IUserItem * pIUserItem);
	//�����ö�
	bool UpdataUserList();

	//�ڲ�����
protected:
	//��ȡλͼ
	int GetImageStation(IUserItem * pIUserItem);
	//��ȡλͼ
	int GetImageStation(tagUserData * pUserData);
	//��ȡ����
	int GetInsertStation(IUserItem * pIUserItem);
	//��ȡ�ַ�
	LPCTSTR GetDisplayText(IUserItem * pIUserItem, WORD wColumnPos);
	//���к���
	static int CALLBACK SortFun(LPARAM lParam1, LPARAM lParam2, LPARAM lParamList);

	//���غ���
public:
	//�滭����
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	//��Ϣӳ��
protected:
	//����б�
	afx_msg void OnColumnclick(NMHDR * pNMHDR, LRESULT * pResult);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
