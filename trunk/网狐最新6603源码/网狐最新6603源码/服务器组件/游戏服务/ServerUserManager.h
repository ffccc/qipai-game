#ifndef SERVER_USER_MANAGER_HEAD_FILE
#define SERVER_USER_MANAGER_HEAD_FILE

#pragma once

#include "AfxTempl.h"
#include "GameServiceHead.h"

//////////////////////////////////////////////////////////////////////////////////

//�û���Ϣ
class GAME_SERVICE_CLASS CServerUserItem : public IServerUserItem, public IServerUserService
{
	//��Ԫ����
	friend class CServerUserManager;

	//���Ա���
protected:
	tagUserInfo						m_UserInfo;							//�û���Ϣ
	tagUserRule						m_UserRule;							//�û�����
	tagUserScore					m_ScoreFormer;						//��ǰ����

	//�����¼
protected:
	tagVariationInfo				m_RecordInfo;						//��¼��Ϣ
	tagVariationInfo				m_VariationInfo;					//�����Ϣ

	//���ӱ���
protected:
	DWORD							m_dwUserRight;						//�û�Ȩ��
	DWORD							m_dwMasterRight;					//����Ȩ��

	//�йܱ���
protected:
	SCORE	 						m_lTrusteeScore;					//�йܻ���
	SCORE							m_lRestrictScore;					//���ƻ���
	SCORE	 						m_lFrozenedScore;					//�������

	//��¼��Ϣ
protected:
	DWORD							m_dwLogonTime;						//��¼ʱ��
	DWORD							m_dwInoutIndex;						//��������

	//�ͻ�����
protected:
	bool							m_bMobileUser;						//�ֻ��û�
	bool							m_bAndroidUser;						//�����û�

	//��������
protected:
	bool							m_bClientReady;						//���ӱ�־
	bool							m_bModifyScore;						//�޸ı�־
	TCHAR							m_szLogonPass[LEN_PASSWORD];		//�û�����

	//ϵͳ����
protected:
	WORD							m_wBindIndex;						//������
	DWORD							m_dwClientAddr;						//���ӵ�ַ
	TCHAR							m_szMachineID[LEN_MACHINE_ID];		//������ʶ

	//����ӿ�
protected:
	IServerUserItemSink *			m_pIServerUserItemSink;				//�ص��ӿ�

	//��������
protected:
	//���캯��
	CServerUserItem();
	//��������
	virtual ~CServerUserItem();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { delete this; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//������Ϣ
public:
	//�������
	virtual WORD GetBindIndex() { return m_wBindIndex; }
	//�û���ַ
	virtual DWORD GetClientAddr() { return m_dwClientAddr; }
	//������ʶ
	virtual LPCTSTR GetMachineID() { return m_szMachineID; }

	//��¼��Ϣ
public:
	//��¼ʱ��
	virtual DWORD GetLogonTime() { return m_dwLogonTime; }
	//��¼����
	virtual DWORD GetInoutIndex() { return m_dwInoutIndex; }

	//�û���Ϣ
public:
	//�û���Ϣ
	virtual tagUserInfo * GetUserInfo() { return &m_UserInfo; }
	//�û�����
	virtual tagUserRule * GetUserRule() { return &m_UserRule; }

	//������Ϣ
public:
	//�û��Ա�
	virtual BYTE GetGender() { return m_UserInfo.cbGender; }
	//�û���ʶ
	virtual DWORD GetUserID() { return m_UserInfo.dwUserID; }
	//��Ϸ��ʶ
	virtual DWORD GetGameID() { return m_UserInfo.dwGameID; }
	//�û��ǳ�
	virtual LPCTSTR GetNickName() { return m_UserInfo.szNickName; }

	//״̬�ӿ�
public:
	//���Ӻ���
	virtual WORD GetTableID() { return m_UserInfo.wTableID; }
	//���Ӻ���
	virtual WORD GetChairID() { return m_UserInfo.wChairID; }
	//�û�״̬
	virtual BYTE GetUserStatus() { return m_UserInfo.cbUserStatus; }

	//Ȩ����Ϣ
public:
	//�û�Ȩ��
	virtual DWORD GetUserRight() { return m_dwUserRight; } 
	//����Ȩ��
	virtual DWORD GetMasterRight() { return m_dwMasterRight; } 

	//�ȼ���Ϣ
public:
	//�û�Ȩ��
	virtual BYTE GetMemberOrder() { return m_UserInfo.cbMemberOrder; } 
	//����Ȩ��
	virtual BYTE GetMasterOrder() { return m_UserInfo.cbMasterOrder; } 

	//������Ϣ
public:
	//�û�����
	virtual SCORE GetUserScore() { return m_UserInfo.lScore; }
	//�û��ɼ�
	virtual SCORE GetUserGrade() { return m_UserInfo.lGrade; }
	//�û�����
	virtual SCORE GetUserInsure() { return m_UserInfo.lInsure; }

	//�й���Ϣ
public:
	//�йܻ���
	virtual SCORE GetTrusteeScore() { return m_lTrusteeScore; }
	//�������
	virtual SCORE GetFrozenedScore() { return m_lFrozenedScore; }

	//������Ϣ
public:
	//�û�ʤ��
	virtual WORD GetUserWinRate();
	//�û�����
	virtual WORD GetUserLostRate();
	//�û�����
	virtual WORD GetUserDrawRate();
	//�û�����
	virtual WORD GetUserFleeRate();
	//��Ϸ����
	virtual DWORD GetUserPlayCount() { return m_UserInfo.dwWinCount+m_UserInfo.dwLostCount+m_UserInfo.dwDrawCount+m_UserInfo.dwFleeCount; }

	//Ч��ӿ�
public:
	//�Ա��ʺ�
	virtual bool ContrastNickName(LPCTSTR pszNickName);
	//�Ա�����
	virtual bool ContrastLogonPass(LPCTSTR pszPassword);

	//��Ϸ״̬
public:
	//����״̬
	virtual bool IsClientReady() { return m_bClientReady; };
	//��������
	virtual VOID SetClientReady(bool bClientReady) { m_bClientReady=bClientReady; }

	//�ֻ��û�
public:
	//����״̬
	virtual bool IsMobileUser() { return m_bMobileUser; }
	//���ÿ���
	virtual VOID SetMobileUser(bool bMobileUser) { m_bMobileUser=bMobileUser; }

	//�����û�
public:
	//����״̬
	virtual bool IsAndroidUser() { return m_bAndroidUser; }
	//���ÿ���
	virtual VOID SetAndroidUser(bool bAndroidUser) { m_bAndroidUser=bAndroidUser; }

	//��¼�ӿ�
public:
	//����ж�
	virtual bool IsVariation() { return m_bModifyScore; }
	//��ѯ��¼
	virtual bool QueryRecordInfo(tagVariationInfo & RecordInfo);
	//��ȡ���
	virtual bool DistillVariation(tagVariationInfo & VariationInfo);

	//����ӿ�
public:
	//����״̬
	virtual bool SetUserStatus(BYTE cbUserStatus, WORD wTableID, WORD wChairID);
	//д�����
    virtual bool WriteUserScore(SCORE lScore, SCORE lGrade, SCORE lRevenue, DWORD dwUserMedal, BYTE cbScoreType, DWORD dwPlayTimeCount);

	//����ӿ�
public:
	//�������
	virtual bool FrozenedUserScore(SCORE lScore);
	//�ⶳ����
	virtual bool UnFrozenedUserScore(SCORE lScore);

	//�����ӿ�
public:
	//�����
	virtual bool DetachBindStatus();
	//���в���
	virtual bool ModifyUserInsure(SCORE lScore, SCORE lInsure, SCORE lRevenue);
	//���ò���
	virtual bool SetUserParameter(DWORD dwClientAddr, WORD wBindIndex, TCHAR szMachineID[LEN_MACHINE_ID], bool bAndroidUser, bool bClientReady);

	//��������
private:
	//��������
	VOID ResetUserItem();
};

//////////////////////////////////////////////////////////////////////////////////

//�û�������
typedef CWHArray<CServerUserItem *> CServerUserItemArray;
typedef CMap<DWORD,DWORD,CServerUserItem *,CServerUserItem *> CServerUserItemMap;

//�û�������
class GAME_SERVICE_CLASS CServerUserManager : public IServerUserManager
{
	//�û�����
protected:
	CServerUserItemMap				m_UserIDMap;						//�û�����
	CServerUserItemArray			m_UserItemArray;					//�û�����
	CServerUserItemArray			m_UserItemStore;					//�洢�û�

	//����ӿ�
protected:
	IServerUserItemSink *			m_pIServerUserItemSink;				//�ص��ӿ�

	//��������
public:
	//���캯��
	CServerUserManager();
	//��������
	virtual ~CServerUserManager();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { return; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//���ú���
public:
	//���ýӿ�
	virtual bool SetServerUserItemSink(IUnknownEx * pIUnknownEx);

	//���Һ���
public:
	//ö���û�
	virtual IServerUserItem * EnumUserItem(WORD wEnumIndex);
	//�����û�
	virtual IServerUserItem * SearchUserItem(DWORD dwUserID);
	//�����û�
	virtual IServerUserItem * SearchUserItem(LPCTSTR pszNickName);

	//ͳ�ƺ���
public:
	//��������
	virtual DWORD GetUserItemCount() { return (DWORD)m_UserItemArray.GetCount(); }

	//������
public:
	//ɾ���û�
	virtual bool DeleteUserItem();
	//ɾ���û�
	virtual bool DeleteUserItem(IServerUserItem * pIServerUserItem);
	//�����û�
	virtual bool InsertUserItem(IServerUserItem * * pIServerUserResult, tagUserInfo & UserInfo, tagUserInfoPlus & UserInfoPlus);
};

//////////////////////////////////////////////////////////////////////////////////

#endif