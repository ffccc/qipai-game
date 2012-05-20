#ifndef APPLY_USER_LIST_HEAD_FILE
#define APPLY_USER_LIST_HEAD_FILE

#pragma once

#include "Stdafx.h"


//////////////////////////////////////////////////////////////////////////
//
struct tagApplyUser
{
	//�����Ϣ
	DWORD							dwUserID;							//���ID��
	TCHAR							szUserName[NAME_LEN];				//����ʺ�
	__int64							iUserScore;							//��ҽ��
};

//////////////////////////////////////////////////////////////////////////

//��Ϸ��¼
class CApplyUser 
{
	//�ؼ�����
private:
	struct tagApplyBankerUser
	{
		int							nSortNo;
		DWORD						dwUserID;							//���ID��
		__int64						iUserScore;							//��ҽ��
		TCHAR						szUserName[NAME_LEN];				//����ʺ�
	};


	tagApplyBankerUser  m_BankerUserArray[GAME_PLAYER];	//������ׯ����б�
	int  m_nApplyCount;									//��ǰ������ׯ������
	int	 m_nPos;										//��д��ǰ��λ��
	//��������
public:
	//���캯��
	CApplyUser();
	//��������
	virtual ~CApplyUser();

	//���ܺ���
public:
	//�����б�
	void InserUser(tagApplyUser & ApplyUser);
	//ɾ���б�
	void DeleteUser(tagApplyUser & ApplyUser);
	//�����б�
	void UpdateUser( tagApplyUser & ApplyUser );
	//����б�
	void ClearAll();
	CString FormatNumber(__int64 i64);
	int GetApplayBankerUserCount(){return m_nApplyCount;}
	void ShowApplayBankerUser(CDC * pDC, CRect * pSortNoRect, CRect * pNameRect, CRect * pScoreRect);
	int ScrollUp();
	int ScrollDown();
};

//////////////////////////////////////////////////////////////////////////

#endif
