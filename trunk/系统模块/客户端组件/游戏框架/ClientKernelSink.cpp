#include "StdAfx.h"
#include "GameFrameDlg.h"
#include "GlobalOption.h"
#include "GameFrameControl.h"
#include "ClientKernelSink.h"

//////////////////////////////////////////////////////////////////////////

//���캯��
CClientKernelSink::CClientKernelSink(CGameFrameDlg & GameFrameDlg) : m_GameFrameDlg(GameFrameDlg)
{
}

//��������
CClientKernelSink::~CClientKernelSink()
{
}

//�ӿڲ�ѯ
void * __cdecl CClientKernelSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IClientKernelSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IClientKernelSink,Guid,dwQueryVer);
	return NULL;
}

//��ȡ����
HWND __cdecl CClientKernelSink::GetFrameWnd()
{
	return m_GameFrameDlg.m_hWnd;
}

//����״̬
void __cdecl CClientKernelSink::ResetGameFrame()
{
	//���ñ���
	m_GameFrameDlg.m_bGameSceen=false;

	//���ý���
	m_GameFrameDlg.m_pGameFrameView->ResetData();
	m_GameFrameDlg.m_pGameFrameView->ResetGameView();
	m_GameFrameDlg.ResetGameFrame();
	m_GameFrameDlg.m_pGameFrameControl->ResetGameFrameControl();

	//������Ƶ
	CVideoServiceManager * pVideoServiceManager=CVideoServiceManager::GetInstance();
	if (pVideoServiceManager!=NULL) pVideoServiceManager->OnEventGameReset();

	return;
}

//�رմ���
void __cdecl CClientKernelSink::CloseGameFrame(bool bForceClose)
{
	if (m_GameFrameDlg.m_hWnd!=NULL) m_GameFrameDlg.PostMessage(WM_CLOSE,0,0);
	return;
}

//ʱ����Ϣ
bool __cdecl CClientKernelSink::OnEventTimer(WORD wChairID, UINT nElapse, UINT nTimerID)
{
	//��������
	WORD wViewChairID=m_GameFrameDlg.SwitchViewChairID(wChairID);
	m_GameFrameDlg.m_pGameFrameView->SetUserTimer(wViewChairID,nElapse);

	//�¼�֪ͨ
	return m_GameFrameDlg.OnTimerMessage(wChairID,nElapse,nTimerID);
}

//������Ϣ
bool __cdecl CClientKernelSink::OnEventSocket(const CMD_Command & Command, const void * pBuffer, WORD wDataSize)
{
	switch (Command.wMainCmdID)
	{
	case MDM_GF_FRAME:	//�����Ϣ
		{
			return m_GameFrameDlg.OnFrameMessage(Command.wSubCmdID,pBuffer,wDataSize);
		}
	case MDM_GF_GAME:	//��Ϸ��Ϣ
		{
			return m_GameFrameDlg.OnGameMessage(Command.wSubCmdID,pBuffer,wDataSize);
		}
	case MDM_GF_VIDEO:	//��Ƶ��Ϣ
		{
			switch (Command.wSubCmdID)
			{
			case SUB_GF_S_VIDEO_CONFIG:		//��Ƶ����
				{
					//Ч�����
					ASSERT(wDataSize==sizeof(CMD_S_VideoConfig));
					if (wDataSize!=sizeof(CMD_S_VideoConfig)) return false;

					//��Ϣ����
					CMD_S_VideoConfig * pVideoConfig=(CMD_S_VideoConfig *)pBuffer;

					//�����û�
					tagUserData * pRomterUserData=m_GameFrameDlg.m_ClientKernelHelper->SearchUserItem(pVideoConfig->dwLocalUserID);
					if (pRomterUserData==NULL) return true;

					//�û�Ч��
					ASSERT((m_GameFrameDlg.IsLookonMode()==false)&&(pRomterUserData->cbUserStatus!=US_LOOKON));
					if ((m_GameFrameDlg.IsLookonMode()==true)||(pRomterUserData->cbUserStatus==US_LOOKON)) return true;

					//��ȡ��Ƶ
					WORD wViewChairID=m_GameFrameDlg.SwitchViewChairID(pRomterUserData->wChairID);
					CVideoServiceManager * pVideoServiceManager=CVideoServiceManager::GetInstance();

					//��Ƶ����
					if (pVideoServiceManager!=NULL)
					{
						//��������
						tagVideoNetwordInfo VideoNetwordInfo;
						ZeroMemory(&VideoNetwordInfo,sizeof(VideoNetwordInfo));

						//���ñ���
						VideoNetwordInfo.wNatPort=pVideoConfig->wNatPort;
						VideoNetwordInfo.wLocalPort=pVideoConfig->wLocalPort;
						VideoNetwordInfo.dwNatAddr=pVideoConfig->dwNatAddr;
						VideoNetwordInfo.dwLocalAddr=pVideoConfig->dwLocalAddr;

						//��Ƶ����
						pVideoServiceManager->OnEventUserConfig(wViewChairID,pVideoConfig->dwLocalUserID,&VideoNetwordInfo);
					}

					return true;
				}
			case SUB_GF_S_VIDEO_CLOSE:		//�ر���Ƶ
				{
					//Ч�����
					ASSERT(wDataSize==sizeof(CMD_S_VideoClose));
					if (wDataSize!=sizeof(CMD_S_VideoClose)) return false;

					//��Ϣ����
					CMD_S_VideoClose * pVideoClose=(CMD_S_VideoClose *)pBuffer;

					//�����û�
					tagUserData const * pRomterUserData=m_GameFrameDlg.GetUserData(pVideoClose->wCloseUser);
					if (pRomterUserData==NULL) return true;

					//�û�Ч��
					ASSERT((m_GameFrameDlg.IsLookonMode()==false)&&(pRomterUserData->cbUserStatus!=US_LOOKON));
					if ((m_GameFrameDlg.IsLookonMode()==true)||(pRomterUserData->cbUserStatus==US_LOOKON)) return true;

					//��ȡ��Ƶ
					WORD wViewChairID=m_GameFrameDlg.SwitchViewChairID(pRomterUserData->wChairID);
					CVideoServiceManager * pVideoServiceManager=CVideoServiceManager::GetInstance();

					//��Ƶ����
					if (pVideoServiceManager!=NULL)
					{
						pVideoServiceManager->OnEventCloseVideo( wViewChairID );
					}
					return true;
				}
			case SUB_GF_S_VIDEO_OPEN:		//����Ƶ
				{
					//Ч�����
					ASSERT(wDataSize==sizeof(CMD_S_VideoOpen));
					if (wDataSize!=sizeof(CMD_S_VideoOpen)) return false;

					//��Ϣ����
					CMD_S_VideoOpen * pVideoOpen=(CMD_S_VideoOpen *)pBuffer;

					//�����û�
					tagUserData const * pRomterUserData=m_GameFrameDlg.GetUserData(pVideoOpen->wOpenUser);
					if (pRomterUserData==NULL) return true;

					//�û�Ч��
					ASSERT((m_GameFrameDlg.IsLookonMode()==false)&&(pRomterUserData->cbUserStatus!=US_LOOKON));
					if ((m_GameFrameDlg.IsLookonMode()==true)||(pRomterUserData->cbUserStatus==US_LOOKON)) return true;

					//��ȡ��Ƶ
					WORD wViewChairID=m_GameFrameDlg.SwitchViewChairID(pRomterUserData->wChairID);
					CVideoServiceManager * pVideoServiceManager=CVideoServiceManager::GetInstance();

					//��Ƶ����
					if (pVideoServiceManager!=NULL)
					{
						pVideoServiceManager->OnEventOpenVideo( wViewChairID );
					}
					return true;
				}
			}
		}
	}

	return false;
}

//������Ϣ
bool __cdecl CClientKernelSink::OnEventProcess(const IPC_Head & Head, const void * pBuffer, WORD wDataSize, HWND hWndSend)
{
	return false;
}

//��Ϸ״̬
bool __cdecl CClientKernelSink::OnEventGameScene(BYTE cbGameStation, bool bLookonOther, void * pBuffer, WORD wDataSize)
{
	//���ñ���
	m_GameFrameDlg.m_bGameSceen=true;

	return m_GameFrameDlg.OnGameSceneMessage(cbGameStation,bLookonOther,pBuffer,wDataSize);
}

//������Ϣ
bool __cdecl CClientKernelSink::OnUserChatMessage(tagUserData * pSendUserData, tagUserData * pRecvUserData, LPCTSTR pszChatMessage, COLORREF crColor)
{
	//��ʾ��Ϣ
	if (pRecvUserData==NULL) m_GameFrameDlg.m_MessageProxyHelper->InsertUserChat(pSendUserData->szName,pszChatMessage,crColor,MS_NORMAL);
	else m_GameFrameDlg.m_MessageProxyHelper->InsertUserChat(pSendUserData->szName,pRecvUserData->szName,pszChatMessage,crColor,MS_NORMAL);

	return true;
}

//ʱ�����
void __cdecl CClientKernelSink::OnEventTimerKilled(WORD wChairID, UINT nTimerID)
{
	//���ý���
	WORD wViewChairID=m_GameFrameDlg.SwitchViewChairID(wChairID);
	m_GameFrameDlg.m_pGameFrameView->SetUserTimer(wViewChairID,0);

	return;
}

//�Թ�״̬
void __cdecl CClientKernelSink::OnEventLookonChanged(bool bLookonUser, void * pBuffer, WORD wDataSize)
{
	//���ý���
	m_GameFrameDlg.OnLookonChanged(bLookonUser,pBuffer,wDataSize);

	return;
}

//�û�����
void __cdecl CClientKernelSink::OnEventUserEnter(tagUserData * pUserData, WORD wChairID, bool bLookonUser)
{
	//�����б�
	m_GameFrameDlg.m_pGameFrameControl->m_UserListView.m_wKindID=m_GameFrameDlg.m_ClientKernelHelper->GetServerAttribute()->wKindID;
	m_GameFrameDlg.m_pGameFrameControl->m_UserListView.m_wGameGenre=m_GameFrameDlg.m_ClientKernelHelper->GetServerAttribute()->wGameGenre;

	//�����б�
	m_GameFrameDlg.m_pGameFrameControl->InsertUserItem(pUserData);

	//���ý���
	if (bLookonUser==false)
	{
		WORD wViewChairID=m_GameFrameDlg.SwitchViewChairID(wChairID);
		m_GameFrameDlg.m_pGameFrameView->SetUserInfo(wViewChairID,pUserData);
	}

	//�����û�
	if (pUserData!=m_GameFrameDlg.m_ClientKernelHelper->GetMeUserInfo())
	{
		//�����û�
		int nItem=m_GameFrameDlg.m_pGameFrameControl->m_ChatObject.AddString(pUserData->szName);
		m_GameFrameDlg.m_pGameFrameControl->m_ChatObject.SetItemData(nItem,pUserData->dwUserID);

		//��ʾ��Ϣ
		if (m_GameFrameDlg.m_bGameSceen==true)
		{
			m_GameFrameDlg.m_MessageProxyHelper->InsertUserName(pUserData->szName);
			m_GameFrameDlg.m_MessageProxyHelper->InsertGeneralString(TEXT("������"),g_GlobalOption.m_crMsgNormal,0,true);
		}
	}

	//�¼�֪ͨ
	m_GameFrameDlg.OnEventUserEnter(pUserData,wChairID,bLookonUser);

	//��Ƶ֪ͨ
	if (bLookonUser==false)
	{
		//��ȡ��Ƶ
		CVideoServiceManager * pVideoServiceManager=CVideoServiceManager::GetInstance();

		//��Ƶ֪ͨ
		if (pVideoServiceManager!=NULL)
		{
			WORD wViewChairID=m_GameFrameDlg.SwitchViewChairID(wChairID);
			pVideoServiceManager->OnEventUserEnter(wViewChairID,pUserData->dwUserID);
		}
	}

	return;
}

//�û��뿪
void __cdecl CClientKernelSink::OnEventUserLeave(tagUserData * pUserData, WORD wChairID, bool bLookonUser)
{
	//�¼�֪ͨ
	m_GameFrameDlg.OnEventUserLeave(pUserData,wChairID,bLookonUser);

	//��Ƶ֪ͨ
	if ( ! bLookonUser )
	{
		WORD wViewChairID=m_GameFrameDlg.SwitchViewChairID(wChairID);
		CVideoServiceManager * pVideoServiceManager=CVideoServiceManager::GetInstance();
		if (pVideoServiceManager!=NULL) pVideoServiceManager->OnEventUserLeave(wViewChairID,pUserData->dwUserID);
	}

	//���ý���
	if (bLookonUser==false)
	{
		WORD wViewChairID=m_GameFrameDlg.SwitchViewChairID(wChairID);
		m_GameFrameDlg.m_pGameFrameView->SetUserInfo(wViewChairID,NULL);
	}

	//ɾ���б�
	m_GameFrameDlg.m_pGameFrameControl->DeleteUserItem(pUserData);

	//��ʾ��Ϣ
	if (m_GameFrameDlg.m_bGameSceen==true)
	{
		m_GameFrameDlg.m_MessageProxyHelper->InsertUserName(pUserData->szName);
		m_GameFrameDlg.m_MessageProxyHelper->InsertGeneralString(TEXT("�뿪��"),g_GlobalOption.m_crMsgNormal,0,true);
	}

	//ɾ���û�
	CComboBox * pChatObject=&m_GameFrameDlg.m_pGameFrameControl->m_ChatObject;
	int nCount=pChatObject->GetCount();
	for (int i=1;i<nCount;i++)
	{
		if (pChatObject->GetItemData(i)==pUserData->dwUserID)
		{
			if (pChatObject->GetCurSel()==i) pChatObject->SetCurSel(0);
			pChatObject->DeleteString(i);
			break;
		}
	}

	return;
}

//�û�����
void __cdecl CClientKernelSink::OnEventUserScore(tagUserData * pUserData, WORD wChairID, bool bLookonUser)
{
	//���ý���
	m_GameFrameDlg.m_pGameFrameControl->UpdateUserItem(pUserData);
	if (bLookonUser==false) m_GameFrameDlg.m_pGameFrameView->UpdateGameView(NULL);

	//�¼�֪ͨ
	m_GameFrameDlg.OnEventUserScore(pUserData,wChairID,bLookonUser);

	return;
}

//�û�״̬
void __cdecl CClientKernelSink::OnEventUserStatus(tagUserData * pUserData, WORD wChairID, bool bLookonUser)
{
	//���ý���
	m_GameFrameDlg.m_pGameFrameControl->UpdateUserItem(pUserData);
	if (bLookonUser==false) m_GameFrameDlg.m_pGameFrameView->UpdateGameView(NULL);

	//�¼�֪ͨ
	m_GameFrameDlg.OnEventUserStatus(pUserData,wChairID,bLookonUser);

	//��Ƶ֪ͨ
	tagUserData const * pMeUserData=m_GameFrameDlg.GetUserData(m_GameFrameDlg.GetMeChairID());
	if ((pMeUserData!=pUserData)&&((pUserData->cbUserStatus==US_OFFLINE)||(pUserData->cbUserStatus==US_PLAY)))
	{
		//����ת��
		WORD wViewChairID=m_GameFrameDlg.SwitchViewChairID(pUserData->wChairID);

		//�¼�֪ͨ
		CVideoServiceManager * pVideoServiceManager=CVideoServiceManager::GetInstance();
		if (pVideoServiceManager!=NULL) pVideoServiceManager->OnUpdateUserItem(wViewChairID,pUserData->cbUserStatus,pUserData->dwUserID);
	}

	return;
}

//��Ա�ȼ�
void __cdecl CClientKernelSink::OnEventUserMemberOrder(tagUserData * pUserData, WORD wChairID, bool bLookonUser)
{
	//���ý���
	m_GameFrameDlg.m_pGameFrameControl->UpdateUserItem(pUserData);
	if (bLookonUser==false) m_GameFrameDlg.m_pGameFrameView->UpdateGameView(NULL);

	return;
}

//��ʼ����
void __cdecl CClientKernelSink::OnEventInitProperty(tagPropertyInfo *pPropertyInfo, int nInfoCount)
{
	m_GameFrameDlg.OnEventInitProperty(pPropertyInfo, nInfoCount);

	return;
}

//��ʼ�ʻ�
void __cdecl CClientKernelSink::OnEventInitFlower(tagFlowerInfo *pFlowerInfo, int nInfoCount)
{
	m_GameFrameDlg.OnEventInitFlower(pFlowerInfo, nInfoCount);

	return;
}

//�ʻ���Ϣ
void __cdecl CClientKernelSink::OnEventFlower(const tagUserData * pSendUserData, const tagUserData * pRecvUserData, UINT uFlowerID, UINT uFlowerEffectID)
{
	//��������
	m_GameFrameDlg.PlayGameSound(GetModuleHandle(GAME_FRAME_DLL_NAME), TEXT("FLOWER"));

	//�Է�����
	m_GameFrameDlg.OnEventFlower(pSendUserData, pRecvUserData, uFlowerID, uFlowerEffectID+1);	

	//������
	if(pSendUserData->dwUserID!=pRecvUserData->dwUserID)
		m_GameFrameDlg.OnEventFlower(pSendUserData, pSendUserData, uFlowerID, uFlowerEffectID);	

	return;
}

//////////////////////////////////////////////////////////////////////////
