#ifndef DLG_SEARCH_USER_HEAD_FILE
#define DLG_SEARCH_USER_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "ServerViewItem.h"

//////////////////////////////////////////////////////////////////////////////////

//��������
struct tagNativeListData
{
	CServerViewItem *				pServerViewItem;					//����ӿ�
	IClientUserItem *				pIClientUserItem;					//�û�ָ��
	IGameLevelParser *				pIGameLevelParser;					//�ȼ��ӿ�
};

//Զ������
struct tagRemoteListData
{
	//�û���Ϣ
	DWORD							dwUserID;							//�û���ʶ
	DWORD							dwGameID;							//��Ϸ��ʶ
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�

	//������Ϣ
	BYTE							cbGender;							//�û��Ա�
	BYTE							cbMemberOrder;						//��Ա�ȼ�
	BYTE							cbMasterOrder;						//����ȼ�

	//λ����Ϣ
	WORD							wKindID;							//���ͱ�ʶ
	WORD							wServerID;							//�����ʶ
	TCHAR							szGameKind[LEN_KIND];				//����λ��
	TCHAR							szGameServer[LEN_SERVER];			//����λ��
};

//���鶨��
typedef CWHArray<tagNativeListData *> CNativeListDataArray;				//��������
typedef CWHArray<tagRemoteListData *> CRemoteListDataArray;				//Զ������

//////////////////////////////////////////////////////////////////////////////////

//�����б�
class CUserListNative : public CUserListControl
{
	//��������
protected:
	CNativeListDataArray			m_NativeListDataActive;				//��������
	CNativeListDataArray			m_NativeListDataBuffer;				//��������

	//��������
public:
	//���캯��
	CUserListNative();
	//��������
	virtual ~CUserListNative();

	//���غ���
protected:
	//�ؼ���
	virtual VOID PreSubclassWindow();

	//���غ���
protected:
	//�û�����
	virtual IClientUserItem * GetClientUserItem(VOID * pItemData);
	//�����ַ�
	virtual VOID ConstructString(VOID * pItemData, WORD wColumnIndex, LPTSTR pszString, WORD wMaxCount);

	//���뺯��
public:
	//��������
	VOID InsertUserItem(IClientUserItem * pIClientUserItem, CServerViewItem * pServerViewItem);
	//��������
	VOID UpdateUserItem(IClientUserItem * pIClientUserItem, CServerViewItem * pServerViewItem);
	//ɾ������
	VOID DeleteUserItem(IClientUserItem * pIClientUserItem, CServerViewItem * pServerViewItem);

	//��������
protected:
	//��ȡ����
	tagNativeListData * CreateNativeListData();
	//ɾ������
	VOID DeleteNativeListData(tagNativeListData * pNativeListData);
};

//////////////////////////////////////////////////////////////////////////////////

//Զ���б�
class CUserListRemote : public CSkinListCtrl
{
	//�б���Ϣ
protected:
	WORD							m_wColumnCount;						//�б���Ŀ
	BYTE							m_cbDataDescribe[MAX_COLUMN];		//��������

	//��Դ����
protected:
	CImageList						m_ImageUserStatus;					//״̬λͼ

	//��������
protected:
	CRemoteListDataArray			m_RemoteListDataActive;				//��������
	CRemoteListDataArray			m_RemoteListDataBuffer;				//��������

	//��������
public:
	//���캯��
	CUserListRemote();
	//��������
	virtual ~CUserListRemote();

	//���غ���
protected:
	//�ؼ���
	virtual VOID PreSubclassWindow();

	//���غ���
protected:
	//��������
	virtual INT SortItemData(LPARAM lParam1, LPARAM lParam2, WORD wColumnIndex, bool bAscendSort);
	//�����ַ�
	virtual VOID ConstructString(VOID * pItemData, WORD wColumnIndex, LPTSTR pszString, WORD wMaxCount);
	//�滭����
	virtual VOID DrawCustomItem(CDC * pDC, LPDRAWITEMSTRUCT lpDrawItemStruct, CRect & rcSubItem, INT nColumnIndex);

	//���뺯��
public:
	//��������
	VOID InsertUserItem(tagUserRemoteInfo * pUserRemoteInfo, LPCTSTR pszKindName);

	//��������
protected:
	//��ȡ����
	tagRemoteListData * CreateRemoteListData();
	//ɾ������
	VOID DeleteRemoteListData(tagRemoteListData * pRemoteListData);
};

//////////////////////////////////////////////////////////////////////////////////

//�����û�
class CDlgSearchUser : public CSkinDialog, public CMissionItem
{
	//��������
protected:
	DWORD							m_dwGameID;							//��Ϸ��ʶ
	TCHAR							m_szNickName[LEN_NICKNAME];			//�û��ǳ�

	//��ť����
protected:
	CSkinButton						m_btOk;								//������ť
	CSkinButton						m_btCancel;							//�رհ�ť

	//�ؼ�����
protected:
	CSkinEdit						m_edGameID;							//��Ϸ��ʶ
	CSkinEdit						m_edNickName;						//�û��ǳ�
	CSkinComboBox					m_ComSearchArea;					//�����б�

	//ģʽ��ť
protected:
	CButton							m_SearchRemote;						//Զ��ģʽ
	CButton							m_SearchByGameID;					//��ʶģʽ

	//�ؼ�����
protected:
	CStatic							m_StaticImage;						//ͼ��ؼ�
	CUserListRemote					m_UserListRemote;					//�û��б�
	CUserListNative					m_UserListNative;					//�û��б�

	//�������
protected:
	CMissionManager					m_MissionManager;					//�������

	//��̬����
protected:
	static CDlgSearchUser *			m_pDlgSearchUser;					//����ָ��

	//��������
public:
	//���캯��
	CDlgSearchUser();
	//��������
	virtual ~CDlgSearchUser();

	//���غ���
protected:
	//�ؼ���
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//��������
	virtual BOOL OnInitDialog();
	//ȷ������
	virtual VOID OnOK();

	//���غ���
protected:
	//�滭��Ϣ
	virtual VOID OnDrawClientArea(CDC * pDC, INT nWidth, INT nHeight);

	//�����¼�
private:
	//�����¼�
	virtual bool OnEventMissionLink(INT nErrorCode);
	//�ر��¼�
	virtual bool OnEventMissionShut(BYTE cbShutReason);
	//��ȡ�¼�
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);

	//�¼�����
public:
	//����ر�
	VOID OnViewItemDelete(CServerViewItem * pServerViewItem);
	//�û�ɾ��
	VOID OnUserItemDelete(IClientUserItem * pIClientUserItem, CServerViewItem * pServerViewItem);
	//�û�����
	VOID OnUserItemUpdate(IClientUserItem * pIClientUserItem, CServerViewItem * pServerViewItem);

	//��������
protected:
	//�����ؼ�
	VOID RectifyControl(INT nWidth, INT nHeight);

	//��̬����
public:
	//��ȡ����
	static CDlgSearchUser * GetInstance() { return m_pDlgSearchUser; }

	//��Ϣ����
protected:
	//ʱ����Ϣ
	VOID OnTimer(UINT_PTR nIDEvent);

	//��Ϣ����
protected:
	//Զ�̲���
	VOID OnBnClickedSearchRemote();
	//��ʶģʽ
	VOID OnBnClickedSearchByGameID();
	//�Ҽ��б�
	VOID OnNMRclickNativeList(NMHDR * pNMHDR, LRESULT * pResult);
	//˫���б�
	VOID OnNMDblclkNativeList(NMHDR * pNMHDR, LRESULT * pResult);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif