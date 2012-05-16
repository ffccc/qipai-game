#include "Stdafx.h"
#include "GamePlaza.h"
#include "GlobalUnits.h"
#include "RoomViewItem.h"
#include "PlazaViewItem.h"
#include "Zip.h"


//////////////////////////////////////////////////////////////////////////

#define		IDC_WEB_BBS			100
#define		IDC_WEB_BEFORE		101
#define		IDC_WEB_BRUSH		102
#define		IDC_WEB_HOME		103
#define		IDC_WEB_NEXT		104
#define		IDC_WEB_STOP		105

BEGIN_MESSAGE_MAP(CPlazaViewItem, CDialog)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_WEB_BBS, OnBnClickedBBS)
	ON_BN_CLICKED(IDC_WEB_HOME, OnBnClickedHOME)
	ON_BN_CLICKED(IDC_WEB_NEXT, OnBnClickedNEXT)
	ON_BN_CLICKED(IDC_WEB_STOP, OnBnClickedSTOP)
	ON_BN_CLICKED(IDC_WEB_BRUSH, OnBnClickedMBRUSH)
	ON_BN_CLICKED(IDC_WEB_BEFORE, OnBnClickedBEFORE)
	ON_COMMAND(IDM_LOGON_PLAZA,		OnCommandLogon)
	ON_COMMAND(IDM_CONNECT_SERVER,	OnCommandConnect)
	ON_COMMAND(IDM_CANCEL_CONNECT,	OnCommandCancelConnect)
	ON_MESSAGE(WM_DOWN_LOAD_FACE, OnDownloadFace)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////


CPlazaViewItem::CPlazaViewItem() : CDialog(IDD_GAME_PLAZA)
{
	//���ñ���
	m_bInitDialog	= false;
	m_bLogonPlaza	= false;
	m_bDownloadConnect=false;
	m_bStartDownloadFace=false;
	m_pHtmlBrower	= NULL;

	return ;
}

CPlazaViewItem::~CPlazaViewItem()
{
}

//�ؼ���
void CPlazaViewItem::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
}

BOOL CPlazaViewItem::PreTranslateMessage(MSG* pMsg)
{
	//��ʾ��Ϣ
	if (m_ToolTipCtrl.m_hWnd!=NULL) 
	{
		m_ToolTipCtrl.RelayEvent(pMsg);
	}

	return __super::PreTranslateMessage(pMsg);
}

//��ʼ������
BOOL CPlazaViewItem::OnInitDialog()
{
	__super::OnInitDialog();

	//�����ؼ�
	m_pHtmlBrower=new CHtmlBrower;
	m_pHtmlBrower->Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CRect(0,0,0,0),this,20,NULL);

	//���Ұ��
	m_pHtmlBrower->Navigate(szStationPage);

	//������ť
	m_WEB_BEFORE.Create(NULL,WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,IDC_WEB_BEFORE);
	m_WEB_BRUSH.Create(NULL,WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,IDC_WEB_BRUSH);
	m_WEB_HOME.Create(NULL,WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,IDC_WEB_HOME);
	m_WEB_NEXT.Create(NULL,WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,IDC_WEB_NEXT);
	m_WEB_STOP.Create(NULL,WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,IDC_WEB_STOP);

	//������Դ
	UpdateSkinResource();

	//��ʾ�ؼ�
	m_ToolTipCtrl.Create(this);
	m_ToolTipCtrl.Activate(TRUE);
	m_ToolTipCtrl.AddTool(&m_WEB_BEFORE,TEXT("����"));
	m_ToolTipCtrl.AddTool(&m_WEB_NEXT,TEXT("ǰ��"));
	m_ToolTipCtrl.AddTool(&m_WEB_STOP,TEXT("ֹͣ"));
	m_ToolTipCtrl.AddTool(&m_WEB_HOME,TEXT("��ҳ"));
	m_ToolTipCtrl.AddTool(&m_WEB_BRUSH,TEXT("ˢ��"));

	//��¼�㳡
	SendLogonMessage();

	//���ñ���
	m_bInitDialog=true;

	return TRUE;
}

//�ӿڲ�ѯ
void * __cdecl CPlazaViewItem::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IViewItem,Guid,dwQueryVer);
	QUERYINTERFACE(ITCPSocketSink,Guid,dwQueryVer);
	QUERYINTERFACE(IPlazaViewItem,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IViewItem,Guid,dwQueryVer);
	return NULL;
}

//����������Ϣ
bool __cdecl CPlazaViewItem::OnEventTCPSocketLink(WORD wSocketID, INT nErrorCode)
{
	//������
	if (nErrorCode!=0)
	{
		g_GlobalAttemper.DestroyStatusWnd(this);
		ShowMessageBox(TEXT("��¼����������ʧ�ܣ����Ժ����Ի�������վ���棡"),MB_ICONINFORMATION);
		SendLogonMessage();
		return true;
	}

	//���͵�¼��
	if (m_bLogonPlaza==false)
	{
		m_DlgLogon.SendLogonPacket(m_ClientSocket.GetInterface());
		g_GlobalAttemper.ShowStatusMessage(TEXT("������֤�û���¼��Ϣ..."),this);
	}

	//�Զ���ͷ��
	CGameFrame *pGameFrame = (CGameFrame *)AfxGetMainWnd() ;
	CDlgCustomFace &DlgCustomFace = pGameFrame->m_DlgCustomFace;
	if ( DlgCustomFace.m_hWnd != NULL && DlgCustomFace.IsWindowVisible() )
	{
		DlgCustomFace.SendData();
	}

	//�����ж�
	if ( m_bDownloadConnect )
	{
		//���ñ���
		m_bDownloadConnect = false;

		//Ͷ������
		tagDownloadInfo &DownloadInfo = m_DownloadInfoArrary[0];

		CMD_GP_DownloadFace DownloadFace;
		DownloadFace.dwUserID = DownloadInfo.dwUserID;
		m_ClientSocket->SendData(MDM_GP_USER, SUB_GP_USER_DOWNLOAD_FACE, &DownloadFace, sizeof(DownloadFace));
	}

	return true;
}

//�����ȡ��Ϣ
bool __cdecl CPlazaViewItem::OnEventTCPSocketRead(WORD wSocketID, CMD_Command Command, VOID * pData, WORD wDataSize)
{
	switch (Command.wMainCmdID)
	{
	case MDM_GP_LOGON:				//��¼��Ϣ
		{
			return OnSocketMainLogon(Command,pData,wDataSize);
		}
	case MDM_GP_SERVER_LIST:		//�б���Ϣ
		{
			return OnSocketMainServerList(Command,pData,wDataSize);
		}
	case MDM_GP_SYSTEM:				//ϵͳ��Ϣ
		{
			return OnSocketMainSystem(Command,pData,wDataSize);
		}
	case MDM_GP_USER:				//�û���Ϣ
		{
			return OnSocketMainUser(Command,pData,wDataSize);
		}
	}

	return true;
}

//����ر���Ϣ
bool __cdecl CPlazaViewItem::OnEventTCPSocketShut(WORD wSocketID, BYTE cbShutReason)
{
	if (m_bLogonPlaza==false)
	{
		if (cbShutReason!=SHUT_REASON_NORMAL)
		{
			g_GlobalAttemper.DestroyStatusWnd(this);
			ShowMessageBox(TEXT("��¼����������ʧ�ܣ����Ժ����Ի�������վ���棡"),MB_ICONINFORMATION);
			SendLogonMessage();
		}
	}

	//�Զ���ͷ��
	CGameFrame *pGameFrame = (CGameFrame *)AfxGetMainWnd() ;
	CDlgCustomFace &DlgCustomFace = pGameFrame->m_DlgCustomFace;
	if ( DlgCustomFace.m_hWnd != NULL ) DlgCustomFace.SetStatus(enOperateStatus_NULL);

	//�ͷ��ڴ�
	if ( m_CustomFace.pFaceData != NULL ) m_CustomFace.Clear();

	return true;
}

//��¼��Ϣ
bool CPlazaViewItem::OnSocketMainLogon(CMD_Command Command, void * pData, WORD wDataSize)
{
	switch (Command.wSubCmdID)
	{
	case SUB_GP_LOGON_SUCCESS:		//��¼�ɹ�
		{
			//Ч�����
			ASSERT(wDataSize>=sizeof(CMD_GP_LogonSuccess));
			if (wDataSize<sizeof(CMD_GP_LogonSuccess)) return false;

			//������Ϣ
			tagGlobalUserData & UserData=g_GlobalUnits.GetGolbalUserData();
			CMD_GP_LogonSuccess * pLogonSuccess=(CMD_GP_LogonSuccess *)pData;
			UserData.wFaceID=pLogonSuccess->wFaceID;
			UserData.cbGender=pLogonSuccess->cbGender;
			UserData.cbMember=pLogonSuccess->cbMember;
			UserData.dwUserID=pLogonSuccess->dwUserID;
			UserData.dwGameID=pLogonSuccess->dwGameID;
			UserData.dwExperience=pLogonSuccess->dwExperience;
			UserData.dwCustomFaceVer=pLogonSuccess->dwCustomFaceVer;

			//��չ��Ϣ
			void * pDataBuffer=NULL;
			tagDataDescribe DataDescribe;
			CRecvPacketHelper RecvPacket(pLogonSuccess+1,wDataSize-sizeof(CMD_GP_LogonSuccess));
			while (true)
			{
				pDataBuffer=RecvPacket.GetData(DataDescribe);
				if (DataDescribe.wDataDescribe==DTP_NULL) break;
				switch (DataDescribe.wDataDescribe)
				{
				case DTP_USER_ACCOUNTS:		//�û��ʻ�
					{
						ASSERT(pDataBuffer!=NULL);
						ASSERT(DataDescribe.wDataSize>0);
						ASSERT(DataDescribe.wDataSize<=sizeof(UserData.szAccounts));
						if (DataDescribe.wDataSize<=sizeof(UserData.szAccounts))
						{
							CopyMemory(UserData.szAccounts,pDataBuffer,DataDescribe.wDataSize);
							UserData.szAccounts[CountArray(UserData.szAccounts)-1]=0;
						}
						break;
					}
				case DTP_USER_PASS:			//�û�����
					{
						ASSERT(pDataBuffer!=NULL);
						ASSERT(DataDescribe.wDataSize>0);
						ASSERT(DataDescribe.wDataSize<=sizeof(UserData.szPassWord));
						if (DataDescribe.wDataSize<=sizeof(UserData.szPassWord))
						{
							CopyMemory(UserData.szPassWord,pDataBuffer,DataDescribe.wDataSize);
							UserData.szPassWord[CountArray(UserData.szPassWord)-1]=0;
						}
						break;
					}
				case DTP_UNDER_WRITE:		//����ǩ��
					{
						ASSERT(pDataBuffer!=NULL);
						ASSERT(DataDescribe.wDataSize>0);
						ASSERT(DataDescribe.wDataSize<=sizeof(UserData.szUnderWrite));
						if (DataDescribe.wDataSize<=sizeof(UserData.szUnderWrite))
						{
							CopyMemory(UserData.szUnderWrite,pDataBuffer,DataDescribe.wDataSize);
							UserData.szUnderWrite[CountArray(UserData.szUnderWrite)-1]=0;
						}
						break;
					}
				case DTP_USER_GROUP_NAME:	//��������
					{
						ASSERT(pDataBuffer!=NULL);
						ASSERT(DataDescribe.wDataSize>0);
						ASSERT(DataDescribe.wDataSize<=sizeof(UserData.szGroupName));
						if (DataDescribe.wDataSize<=sizeof(UserData.szGroupName))
						{
							CopyMemory(UserData.szGroupName,pDataBuffer,DataDescribe.wDataSize);
							UserData.szGroupName[CountArray(UserData.szGroupName)-1]=0;
						}
						break;
					}
				case DTP_STATION_PAGE:		//��Ϸ��վ
					{
						ASSERT(pDataBuffer!=NULL);
						if (pDataBuffer!=NULL) 
						{
							g_GlobalUnits.SetStationPage((LPCTSTR)pDataBuffer);
							m_pHtmlBrower->Navigate(g_GlobalUnits.GetStationPage());
						}
						break;
					}
				default: { ASSERT(FALSE); }
				}
			}

			//������ʾ
			g_GlobalAttemper.ShowStatusMessage(TEXT("���ڶ�ȡ�������б���Ϣ..."),this);

			return true;
		}
	case SUB_GP_LOGON_ERROR:		//��¼ʧ��
		{
			//Ч�����
			CMD_GP_LogonError *pLogonError = (CMD_GP_LogonError *)pData;
			ASSERT(wDataSize>=(sizeof(CMD_GP_LogonError)-sizeof(pLogonError->szErrorDescribe)));
			if (wDataSize<(sizeof(CMD_GP_LogonError)-sizeof(pLogonError->szErrorDescribe))) return false;

			//�ر�����
			g_GlobalAttemper.DestroyStatusWnd(this);
			m_ClientSocket->CloseSocket();

			//��ʾ��Ϣ
			WORD wDescribeSize=wDataSize-(sizeof(CMD_GP_LogonError)-sizeof(pLogonError->szErrorDescribe));
			if (wDescribeSize>0)
			{
				pLogonError->szErrorDescribe[wDescribeSize-1]=0;
				ShowMessageBox(pLogonError->szErrorDescribe,MB_ICONINFORMATION);
			}

			//���͵�¼
			SendLogonMessage();

			return true;
		}
	case SUB_GP_LOGON_FINISH:		//��¼���
		{
			//�ر���ʾ
			g_GlobalAttemper.DestroyStatusWnd(this);

			//չ������
			INT_PTR nIndex=0;
			CListType * pListType=NULL;
			do
			{
				pListType=g_GlobalUnits.m_ServerListManager.EnumTypeItem(nIndex++);
				if (pListType==NULL) break;
				g_GlobalUnits.m_ServerListManager.ExpandListItem(pListType);
			} while (true);

			//չ���б�
			nIndex=0;
			CListInside * pListInside=NULL;
			do
			{
				pListInside=g_GlobalUnits.m_ServerListManager.EnumInsideItem(nIndex++);
				if (pListInside==NULL) break;
				g_GlobalUnits.m_ServerListManager.ExpandListItem(pListInside);
			} while (true);

			//��¼��Ϣ
			m_bLogonPlaza=true;
			m_DlgLogon.OnLogonSuccess();
			m_pHtmlBrower->EnableBrowser(true);

			//��¼��Ϣ
			g_GlobalUnits.WriteUserCookie();

			//��ʾͷ��
			((CGameFrame*)AfxGetMainWnd())->m_UserInfoView.ShowUserInfo(true);

			//�Զ����ж�
			tagGlobalUserData &GlobalUserData = g_GlobalUnits.GetGolbalUserData();
			if ( GlobalUserData.dwCustomFaceVer!=0)
			{
				//ͷ������
				CString strDirName = CString(g_GlobalUnits.GetWorkDirectory()) + TEXT("\\CustomFace");
				CString strFileName;
				strFileName.Format(TEXT("\\%ld_%d.bmp"), GlobalUserData.dwUserID, GlobalUserData.dwCustomFaceVer);

				//��ȡ�ļ�
				CImage FaceImage;
				HRESULT hResult = FaceImage.Load(strDirName + strFileName);
				if (SUCCEEDED(hResult))
				{
					//�ر�����
					m_ClientSocket->CloseSocket();

					FaceImage.Destroy();
				}
				//����ͷ��
				else
				{
					PostMessage(WM_DOWN_LOAD_FACE, LPARAM(GlobalUserData.dwCustomFaceVer), WPARAM(GlobalUserData.dwUserID));
				}
			}
			else
			{
				//�ر�����
				m_ClientSocket->CloseSocket();
			}

			return true;
		}
	}

	return true;
}

//��Ϸ�б���Ϣ
bool CPlazaViewItem::OnSocketMainServerList(CMD_Command Command, void * pData, WORD wDataSize)
{
	ASSERT(Command.wMainCmdID==MDM_GP_SERVER_LIST);
	
	switch (Command.wSubCmdID)
	{
	case SUB_GP_LIST_TYPE:			//������Ϣ
		{
			//Ч�����
			ASSERT(wDataSize%sizeof(tagGameType)==0);
			if (wDataSize%sizeof(tagGameType)!=0) return false;

			//������Ϣ
			tagGameType * pGameType=(tagGameType *)pData;
			WORD wItemCount=wDataSize/sizeof(tagGameType);
			g_GlobalUnits.m_ServerListManager.InsertTypeItem(pGameType,wItemCount);

			return true;
		}
	case SUB_GP_LIST_KIND:			//������Ϣ
		{
			//Ч�����
			ASSERT(wDataSize%sizeof(tagGameKind)==0);
			if (wDataSize%sizeof(tagGameKind)!=0) return false;

			//������Ϣ
			tagGameKind * pGameKind=(tagGameKind *)pData;
			WORD wItemCount=wDataSize/sizeof(tagGameKind);
			g_GlobalUnits.m_ServerListManager.InsertKindItem(pGameKind,wItemCount);

			return true;
		}
	case SUB_GP_LIST_STATION:		//վ����Ϣ
		{
			//Ч�����
			ASSERT(wDataSize%sizeof(tagGameStation)==0);
			if (wDataSize%sizeof(tagGameStation)!=0) return false;

			//������Ϣ
			tagGameStation * pGameStation=(tagGameStation *)pData;
			WORD wItemCount=wDataSize/sizeof(tagGameStation);
			g_GlobalUnits.m_ServerListManager.InsertStationItem(pGameStation,wItemCount);

			return true;
		}
	case SUB_GP_LIST_SERVER:		//����������
		{
			//Ч�����
			ASSERT(wDataSize%sizeof(tagGameServer)==0);
			if (wDataSize%sizeof(tagGameServer)!=0) return false;

			//������Ϣ
			tagGameServer * pGameServer=(tagGameServer *)pData;
			WORD wItemCount=wDataSize/sizeof(tagGameServer);
			g_GlobalUnits.m_ServerListManager.InsertServerItem(pGameServer,wItemCount);

			return true;
		}
	case SUB_GP_LIST_FINISH:		//�б������
		{
			//��������
			INT_PTR nIndex=0;
			DWORD dwAllOnLineCount=0L;
			CListKind * pListKind=NULL;
			do
			{
				pListKind=g_GlobalUnits.m_ServerListManager.EnumKindItem(nIndex++);
				if (pListKind==NULL) break;
				dwAllOnLineCount+=pListKind->GetItemInfo()->dwOnLineCount;
			} while (true);
			g_GlobalUnits.m_ServerListManager.UpdateGameOnLineCount(dwAllOnLineCount);

			return true;
		}
	case SUB_GP_LIST_CONFIG:		//�б�����
		{
			//Ч�����
			ASSERT(wDataSize%sizeof(CMD_GP_ListConfig)==0);
			if (wDataSize%sizeof(CMD_GP_ListConfig)!=0) return false;

			//������Ϣ
			CMD_GP_ListConfig * pListConfig=(CMD_GP_ListConfig *)pData;
			((CGameFrame*)AfxGetMainWnd())->m_ServerItemView.ShowOnLineCount(pListConfig->bShowOnLineCount?true:false);

			return true;
		}
	}

	return true;
}

//ϵͳ��Ϣ
bool CPlazaViewItem::OnSocketMainSystem(CMD_Command Command, void * pData, WORD wDataSize)
{
	ASSERT(Command.wMainCmdID==MDM_GP_SYSTEM);
	switch (Command.wSubCmdID)
	{
	case SUB_GP_VERSION:			//�汾��Ϣ
		{
			//Ч�����
			ASSERT(wDataSize>=sizeof(CMD_GP_Version));
			if (wDataSize<sizeof(CMD_GP_Version)) return false;

			//��Ϣ����
			CMD_GP_Version * pVersion=(CMD_GP_Version *)pData;
			if (pVersion->bAllowConnect)
			{
				TCHAR szMessage[]=TEXT("��Ϸ�����汾�Ѿ����������ڵİ汾�����Լ���ʹ�ã������Ƿ����������°汾��");
				
				int iResult=ShowInformationEx(szMessage,0,MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON1,TEXT("��Ϸ�㳡"));
				if (iResult==IDYES)
				{
					g_GlobalAttemper.DestroyStatusWnd(this);
					m_ClientSocket->CloseSocket();
					tagGlobalUserData & GlobalUserData=g_GlobalUnits.GetGolbalUserData();
					memset(&GlobalUserData,0,sizeof(GlobalUserData));
					g_GlobalAttemper.DownLoadClient(TEXT("��Ϸ�㳡"),0,true);
				}
			}
			else
			{
				g_GlobalAttemper.DestroyStatusWnd(this);
				m_ClientSocket->CloseSocket();
				TCHAR szMessage[]=TEXT("��Ϸ�����汾�Ѿ����������ڵİ汾�����Լ���ʹ�ã������Ƿ����������°汾��");
				
				int iResult=ShowInformationEx(szMessage,0,MB_ICONSTOP|MB_YESNO|MB_DEFBUTTON1,TEXT("��Ϸ�㳡"));
				if(iResult != IDYES)
				{
					tagGlobalUserData & GlobalUserData=g_GlobalUnits.GetGolbalUserData();
					memset(&GlobalUserData,0,sizeof(GlobalUserData));
					AfxGetMainWnd()->PostMessage(WM_CLOSE);
				}
				else 
				{
					g_GlobalAttemper.DownLoadClient(TEXT("��Ϸ�㳡"),0,true);
				}
			}

			return true;
		}
	}
	return true;
}

//�û���Ϣ
bool CPlazaViewItem::OnSocketMainUser(CMD_Command Command, void * pBuffer, WORD wDataSize)
{
	ASSERT(Command.wMainCmdID == MDM_GP_USER);
	switch(Command.wSubCmdID)
	{
	case SUB_GP_USER_DOWNLOAD_FACE:			//����ͷ��
		{
			//����ת��
			CMD_GP_DownloadFaceSuccess *pDownloadFaceSuccess = (CMD_GP_DownloadFaceSuccess*)pBuffer;

			//������֤
			WORD wSendSize = WORD(pDownloadFaceSuccess->dwCurrentSize + sizeof(CMD_GP_DownloadFaceSuccess) - sizeof(pDownloadFaceSuccess->bFaceData));
			ASSERT(wDataSize == wSendSize);
			if ( wDataSize != wSendSize ) return false;

			//��һ���ж�
			if ( m_CustomFace.pFaceData == NULL )
			{
				if ( m_CustomFace.pFaceData != NULL ) delete[] m_CustomFace.pFaceData;
				m_CustomFace.pFaceData = new BYTE[pDownloadFaceSuccess->dwToltalSize];
				if ( m_CustomFace.pFaceData == NULL ) return true;
				m_CustomFace.dwFaceSize = pDownloadFaceSuccess->dwToltalSize;
				m_CustomFace.dwUserID = pDownloadFaceSuccess->dwUserID;
			}

			//��������
			CopyMemory(m_CustomFace.pFaceData+m_CustomFace.dwCurrentSize, pDownloadFaceSuccess->bFaceData, pDownloadFaceSuccess->dwCurrentSize);
			m_CustomFace.dwCurrentSize += pDownloadFaceSuccess->dwCurrentSize;

			//�������
			if ( m_CustomFace.dwFaceSize == m_CustomFace.dwCurrentSize )
			{
				tagDownloadInfo &DownloadInfo = m_DownloadInfoArrary[0];

				//����Ŀ¼
				CString strDirName = CString(g_GlobalUnits.GetWorkDirectory()) + TEXT("\\CustomFace");
				CreateDirectory(strDirName, NULL) ;

				//д���ļ�
				CFile fileFace;
				CString strZipFileName, strBmpFileName;
				strZipFileName.Format(TEXT("\\%ld_%d.zip"), DownloadInfo.dwUserID, DownloadInfo.bFaceVer);
				strBmpFileName.Format(TEXT("\\%ld_%d.bmp"), DownloadInfo.dwUserID, DownloadInfo.bFaceVer);
				if ( fileFace.Open(strDirName + strZipFileName, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary) )
				{
					fileFace.Write(m_CustomFace.pFaceData, m_CustomFace.dwFaceSize);
					fileFace.Close();

					//��ѹ�ļ�
					CUnzip oUnzip( strDirName + strZipFileName );
					oUnzip.SwapSize( strDirName + strBmpFileName );

					//ɾ���ļ�
					CFile::Remove(strDirName + strZipFileName);
				}

				//�Լ��ж�
				tagGlobalUserData &GlobalUserData = g_GlobalUnits.GetGolbalUserData();
				if ( GlobalUserData.dwUserID ==  DownloadInfo.dwUserID )
				{
					//���½���
					((CGameFrame*)AfxGetMainWnd())->m_UserInfoView.UpdateUserInfo();
				}

				//���ñ���
				m_CustomFace.Clear();

				//ɾ��Ԫ��
				m_DownloadInfoArrary.RemoveAt(0);

				//�����ж�
				if ( 0 < m_DownloadInfoArrary.GetCount() )
				{
					//״̬�ж�
					if ( m_ClientSocket->GetSocketStatus() != SOCKET_STATUS_CONNECT )
					{
						//��������
						if ( ! ConnectServer() ) return true;

						//���ñ�ʶ
						m_bDownloadConnect = true;
					}

					//Ͷ������
					tagDownloadInfo &DownloadInfo = m_DownloadInfoArrary[0];

					CMD_GP_DownloadFace DownloadFace;
					DownloadFace.dwUserID = DownloadInfo.dwUserID;
					m_ClientSocket->SendData(MDM_GP_USER, SUB_GP_USER_DOWNLOAD_FACE, &DownloadFace, sizeof(DownloadFace));
				}
				else
				{
					//�ر�����
					m_ClientSocket->CloseSocket();

					//���ñ���
					m_bStartDownloadFace=false;
				}
			}
			return true;
		}
	case SUB_GP_UPLOAD_FACE_RESULT:		//�ϴ����
		{
			ASSERT(sizeof(CMD_GP_UploadFaceResult) == wDataSize);
			if ( sizeof(CMD_GP_UploadFaceResult) != wDataSize) return true;

			//��ʾ��Ϣ
			CMD_GP_UploadFaceResult *pUploadFaceResult = (CMD_GP_UploadFaceResult*)pBuffer;
			ShowMessageBox(pUploadFaceResult->szDescribeMsg, MB_ICONINFORMATION);

			//�ɹ��ж�
			if ( pUploadFaceResult->bOperateSuccess )
			{
				//ͷ��Ŀ¼
				CString strDirName = CString(g_GlobalUnits.GetWorkDirectory()) + TEXT("\\CustomFace");
				CString strOldFileName;
				strOldFileName = TEXT("\\MyFace.bmp");

				//�����ļ�
				tagGlobalUserData &GlobalUserData = g_GlobalUnits.GetGolbalUserData();
				CString strNemFileName;
				strNemFileName.Format(TEXT("\\%ld_%d.bmp"), GlobalUserData.dwUserID, pUploadFaceResult->dwFaceVer);

				//ɾ���ļ�
				try
				{
				CFile::Remove(strDirName + strNemFileName);
				}
				catch(...){}

				//ɾ���ļ�
				CString strOldFile;
				strOldFile.Format(TEXT("\\%ld_%ld.bmp"), GlobalUserData.dwUserID, GlobalUserData.dwCustomFaceVer);
				try
				{
					CFile::Remove(strDirName + strOldFile);
				}
				catch(...){}

				try
				{
					CFile::Rename(strDirName + strOldFileName, strDirName + strNemFileName);

					//���ð汾��
					GlobalUserData.dwCustomFaceVer = pUploadFaceResult->dwFaceVer;
					ASSERT(GlobalUserData.dwCustomFaceVer!=0);

					//Ͷ����Ϣ
					for ( INT nIndex = 0; nIndex < MAX_SERVER; nIndex++ ) 
					{
						CRoomViewItem *pRoomViewItem = ((CGameFrame*)AfxGetMainWnd())->m_pRoomViewItem[nIndex];
						if ( pRoomViewItem == NULL ) continue;
						pRoomViewItem->SendMessage(WM_UPDATE_FACE, GlobalUserData.dwCustomFaceVer, GlobalUserData.dwUserID);
					}
				}
				catch(...)
				{
					ShowMessageBox(TEXT("ͷ���ļ�����ʧ�ܣ���Ҫ���µ�¼�ſ�����ʾ��ͷ��"), MB_ICONINFORMATION);
				}

				//���½���
				((CGameFrame*)AfxGetMainWnd())->m_UserInfoView.UpdateUserInfo();

				//���ý���
				CGameFrame *pGameFrame = (CGameFrame *)AfxGetMainWnd() ;
				CDlgCustomFace &DlgCustomFace = pGameFrame->m_DlgCustomFace;
				if ( DlgCustomFace.m_hWnd != NULL && DlgCustomFace.IsWindowVisible() )
				{
					DlgCustomFace.UpdateControls();
					DlgCustomFace.SetFocus();
				}
			}

			//�ر�����
			m_ClientSocket->CloseSocket();

			return true;
		}
	case SUB_GP_DELETE_FACE_RESULT:		//ɾ�����
		{
			ASSERT(sizeof(CMD_GP_DeleteFaceResult) == wDataSize);
			if ( sizeof(CMD_GP_DeleteFaceResult) != wDataSize) return true;

			//��ʾ��Ϣ
			CMD_GP_DeleteFaceResult *pDeleteFaceResult = (CMD_GP_DeleteFaceResult*)pBuffer;
			ShowMessageBox(pDeleteFaceResult->szDescribeMsg, MB_ICONINFORMATION);

			//�ɹ��ж�
			if ( pDeleteFaceResult->bOperateSuccess )
			{
				//��ȡ��Ϣ
				tagGlobalUserData &GlobalUserData = g_GlobalUnits.GetGolbalUserData();

				//�ļ�Ŀ¼
				CString strDirName = CString(g_GlobalUnits.GetWorkDirectory()) + TEXT("\\CustomFace");

				//�ļ�����
				CString strFileName;
				strFileName.Format(TEXT("\\%ld_%d.bmp"), GlobalUserData.dwUserID, pDeleteFaceResult->dwFaceVer);

				//ɾ���ļ�
				try
				{
				CFile::Remove(strDirName + strFileName);
				}
				catch(...){}
				
				//���½���
				((CGameFrame*)AfxGetMainWnd())->m_UserInfoView.UpdateUserInfo();

				//���ý���
				CGameFrame *pGameFrame = (CGameFrame *)AfxGetMainWnd() ;
				CDlgCustomFace &DlgCustomFace = pGameFrame->m_DlgCustomFace;
				if ( DlgCustomFace.m_hWnd != NULL && DlgCustomFace.IsWindowVisible() )
				{
					DlgCustomFace.UpdateControls();
					DlgCustomFace.SetFocus();
				}
				
				//Ͷ����Ϣ
				for ( INT nIndex = 0; nIndex < MAX_SERVER; nIndex++ ) 
				{
					CRoomViewItem *pRoomViewItem = ((CGameFrame*)AfxGetMainWnd())->m_pRoomViewItem[nIndex];
					if ( pRoomViewItem == NULL ) continue;
					pRoomViewItem->SendMessage(WM_UPDATE_FACE, GlobalUserData.dwCustomFaceVer, GlobalUserData.dwUserID);
				}
			}
			
			//�ر�����
			m_ClientSocket->CloseSocket();

			return true;
		}
	case SUB_GP_MODIFY_INDIVIDUAL_RESULT:	//�޸Ľ��
		{
			return true;
		}
	default:
		{
			ASSERT(FALSE);
			return false;
		}
	}
	return true;
}

//��¼��Ϣ
bool __cdecl CPlazaViewItem::SendLogonMessage()
{
	m_bLogonPlaza=false;
	((CGameFrame*)AfxGetMainWnd())->m_UserInfoView.ShowUserInfo(false);
	PostMessage(WM_COMMAND,IDM_LOGON_PLAZA,0);
	return true;
}

//������Ϣ
bool __cdecl CPlazaViewItem::SendConnectMessage()
{
	PostMessage(WM_COMMAND,IDM_CONNECT_SERVER,0);
	return true;
}

//��ʾ��ҳ
void CPlazaViewItem::WebBrowse(LPCTSTR lpszUrl, bool bForceBrowse)
{
	if (m_pHtmlBrower->GetSafeHwnd())
	{
		m_pHtmlBrower->Navigate(lpszUrl);
		if (bForceBrowse==true) 
		{
			m_pHtmlBrower->EnableBrowser(true);
		}
	}
	return;
}

//��ʾ��Ϣ
int CPlazaViewItem::ShowMessageBox(LPCTSTR pszMessage, UINT nType)
{				
	int nResult=ShowInformationEx(pszMessage,0,nType,TEXT("��Ϸ����"));

	return nResult;
}

//��������
bool CPlazaViewItem::ConnectServer()
{
	//�Ϸ��ж�
	if ( m_ClientSocket.GetInterface() == NULL ) 
	{
		return false;
	}

	//��ַ����
	CRegKey RegServer;
	LPCTSTR pszServerIP=m_DlgLogon.GetLogonServer();
	TCHAR szRegServer[256]=TEXT(""),szServerAddr[64]=TEXT("");
	_snprintf(szRegServer,sizeof(szRegServer),TEXT("%s\\%s"),REG_LOGON_SERVER,pszServerIP);

	if (RegServer.Open(HKEY_CURRENT_USER,szRegServer,KEY_READ)==ERROR_SUCCESS)
	{
		TCHAR szReadData[1024]=TEXT("");
		DWORD dwReadData=0L,dwDataType=0L,dwDataSize=sizeof(szReadData);
		LONG lErrorCode=RegServer.QueryValue(TEXT("ServerAddr"),&dwDataType,szReadData,&dwDataSize);
		if (lErrorCode==ERROR_SUCCESS) 
		{
			CXOREncrypt::CrevasseData(szReadData,szServerAddr,sizeof(szServerAddr));
			pszServerIP=szServerAddr;
		}
	}

	//���ӷ�����
	try
	{
		//���ӷ�����
		m_ClientSocket->CloseSocket();
		if (m_ClientSocket->Connect(pszServerIP,PORT_LOGON_SERVER)!=ERROR_SUCCESS)
		{
			throw TEXT("���������Ӵ��󣬿��������ϵͳ��û�гɹ����������磡");
		}
	}
	catch (LPCTSTR pszError)
	{
		UNREFERENCED_PARAMETER(pszError);
		return false;
	}

	return true;
}

//������Դ
bool CPlazaViewItem::UpdateSkinResource()
{
	//������Դ
	tagPlazaViewImage & PlazaViewImage=g_GlobalUnits.m_PlazaViewImage;
	HINSTANCE hInstance=g_GlobalUnits.m_PlatformResourceModule->GetResInstance();

	//��������
	m_EncircleWeb.ImageML.SetLoadInfo(PlazaViewImage.uBrowseML,hInstance);
	m_EncircleWeb.ImageMR.SetLoadInfo(PlazaViewImage.uBrowseMR,hInstance);
	m_EncircleWeb.ImageTL.SetLoadInfo(PlazaViewImage.uBrowseTL,hInstance);
	m_EncircleWeb.ImageTM.SetLoadInfo(PlazaViewImage.uBrowseTM,hInstance);
	m_EncircleWeb.ImageTR.SetLoadInfo(PlazaViewImage.uBrowseTR,hInstance);
	m_EncircleWeb.ImageBL.SetLoadInfo(PlazaViewImage.uBrowseBL,hInstance);
	m_EncircleWeb.ImageBM.SetLoadInfo(PlazaViewImage.uBrowseBM,hInstance);
	m_EncircleWeb.ImageBR.SetLoadInfo(PlazaViewImage.uBrowseBR,hInstance);

	//��ȡ��Ϣ
	CSkinAide::GetEncircleInfo(m_ImageInfoWeb,m_EncircleWeb);

	//���ð�ť
	m_WEB_BEFORE.SetButtonImage(PlazaViewImage.uBtWebLast,hInstance,false);
	m_WEB_BRUSH.SetButtonImage(PlazaViewImage.uBtWebReload,hInstance,false);
	m_WEB_HOME.SetButtonImage(PlazaViewImage.uBtWebHome,hInstance,false);
	m_WEB_NEXT.SetButtonImage(PlazaViewImage.uBtWebNext,hInstance,false);
	m_WEB_STOP.SetButtonImage(PlazaViewImage.uBtWebStop,hInstance,false);

	return true;
}

//λ����Ϣ
void CPlazaViewItem::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	//״̬�ж�
	if (m_bInitDialog==false) return;
	if ((cx==0)||(cy==0)) return;

	//��������
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS;

	//�ƶ��ؼ�
	HDWP hDwp=BeginDeferWindowPos(32);
	DeferWindowPos(hDwp,m_pHtmlBrower->GetSafeHwnd(),NULL,m_ImageInfoWeb.nLBorder,m_ImageInfoWeb.nTBorder,
		cx-m_ImageInfoWeb.nLBorder-m_ImageInfoWeb.nRBorder,cy-m_ImageInfoWeb.nTBorder-m_ImageInfoWeb.nBBorder,uFlags);

	//������ť
	CRect rcButton;
	m_WEB_BEFORE.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_WEB_BEFORE,NULL,10,3,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_WEB_NEXT,NULL,10+rcButton.Width(),3,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_WEB_BRUSH,NULL,10+rcButton.Width()*2,3,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_WEB_STOP,NULL,10+rcButton.Width()*3,3,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_WEB_HOME,NULL,10+rcButton.Width()*4,3,0,0,uFlags|SWP_NOSIZE);

	EndDeferWindowPos(hDwp);

	return;
}

//��ʾ��Ϣ
void CPlazaViewItem::OnShowWindow(BOOL bShow, UINT nStatus)
{
	__super::OnShowWindow(bShow, nStatus);

	//���ñ���
	if (bShow) 
	{
		UpdateTitleText();
	}

	return;
}

//������Ϣ
LRESULT CPlazaViewItem::OnDownloadFace(WPARAM wParam, LPARAM lParam)
{
	DWORD dwUserID = (DWORD)lParam;
	ASSERT(dwUserID != 0);
	BYTE bFaceVer = (BYTE)wParam;

	//���Ҵ���
	bool bFind = false;
	for ( INT_PTR nIndex = 0; nIndex < m_DownloadInfoArrary.GetCount(); nIndex++ )
	{
		tagDownloadInfo &DownloadInfo = m_DownloadInfoArrary[nIndex];
		if ( DownloadInfo.dwUserID == dwUserID )DownloadInfo.bReference++;
	}

	if ( !bFind )
	{
		tagDownloadInfo DownloadInfo;
		ZeroMemory(&DownloadInfo, sizeof(DownloadInfo));
		DownloadInfo.bFaceVer = bFaceVer;
		DownloadInfo.dwUserID = dwUserID;
		DownloadInfo.bReference = 1;

		//�������
		INT_PTR nCount = m_DownloadInfoArrary.GetCount();
		m_DownloadInfoArrary.SetAtGrow(nCount, DownloadInfo);
	}

	//��������
	if (m_bStartDownloadFace==false)
	{
		m_bStartDownloadFace=true;

		//״̬�ж�
		if ( m_ClientSocket->GetSocketStatus() != SOCKET_STATUS_CONNECT )
		{
			//��������
			if ( ! ConnectServer() ) return FALSE;

			//���ñ�ʶ
			m_bDownloadConnect = true;
		}

		//Ͷ������
		CMD_GP_DownloadFace DownloadFace;
		DownloadFace.dwUserID = dwUserID;
		m_ClientSocket->SendData(MDM_GP_USER, SUB_GP_USER_DOWNLOAD_FACE, &DownloadFace, sizeof(DownloadFace));
	}

	return TRUE;
}

//�滭��Ϣ
void CPlazaViewItem::OnPaint()
{
	CPaintDC dc(this);

	//�滭����
	DrawWebFrame(&dc);

	return;
}

//�滭����
BOOL CPlazaViewItem::OnEraseBkgnd(CDC * pDC)
{
	Invalidate(FALSE);
	UpdateWindow();
	return TRUE;
}

//��¼�㳡
void CPlazaViewItem::OnCommandLogon()
{
	//������¼��
	if (m_DlgLogon.m_hWnd==NULL) 
	{
		m_DlgLogon.Create(IDD_LOGON,this);
	}

	//��ʾ��¼��
	m_DlgLogon.ShowWindow(SW_SHOW);
	m_DlgLogon.SetActiveWindow();
	m_DlgLogon.SetForegroundWindow();

	return;
}

//���ӷ�����
void CPlazaViewItem::OnCommandConnect()
{
	//�������
	if (m_ClientSocket.GetInterface()==NULL)
	{
		try
		{
			IUnknownEx * pIUnknownEx=(IUnknownEx *)QueryInterface(IID_IUnknownEx,VER_IUnknownEx);
			if (m_ClientSocket.CreateInstance()==false) 
			{
				throw TEXT("�����������ʧ��");
			}
			if (m_ClientSocket->SetTCPSocketSink(pIUnknownEx)==false) 
			{
				throw TEXT("��������ص��ӿ�����ʧ��");
			}
		}
		catch (...)
		{
			ShowMessageBox(TEXT("�����������ʧ�ܣ�������������Ϸ������"),MB_ICONSTOP);
			PostMessage(WM_COMMAND,IDM_LOGON_PLAZA,0);
			return;
		}
	}

	//��ַ����
	CRegKey RegServer;
	LPCTSTR pszServerIP=m_DlgLogon.GetLogonServer();
	TCHAR szRegServer[256]=TEXT(""),szServerAddr[64]=TEXT("");
	_snprintf(szRegServer,sizeof(szRegServer),TEXT("%s\\%s"),REG_LOGON_SERVER,pszServerIP);

	if (RegServer.Open(HKEY_CURRENT_USER,szRegServer,KEY_READ)==ERROR_SUCCESS)
	{
		TCHAR szReadData[1024]=TEXT("");
		DWORD dwReadData=0L,dwDataType=0L,dwDataSize=sizeof(szReadData);
		LONG lErrorCode=RegServer.QueryValue(TEXT("ServerAddr"),&dwDataType,szReadData,&dwDataSize);
		if (lErrorCode==ERROR_SUCCESS) 
		{
			CXOREncrypt::CrevasseData(szReadData,szServerAddr,sizeof(szServerAddr));
			pszServerIP=szServerAddr;
		}
	}

	//���ӷ�����
	try
	{
		//���ӷ�����
		m_ClientSocket->CloseSocket();
		
		//�����ж�
		if ( m_DlgLogon.EnableProxy() == true )
		{
			//��ȡ����
			enProxyServerType ProxyServerType;
			tagProxyServerInfo ProxyServerInfo;
			m_DlgLogon.GetProxyInfo(ProxyServerType, ProxyServerInfo);	

			//���ô���
			m_ClientSocket->SetProxyServerInfo(ProxyServerType,ProxyServerInfo);
		}

		if (m_ClientSocket->Connect(pszServerIP,PORT_LOGON_SERVER)!=CONNECT_SUCCESS)
		{
			throw TEXT("���������Ӵ��󣬿��������ϵͳ��û�гɹ����������磡");
		}
	}
	catch (LPCTSTR pszError)
	{
		ShowMessageBox(pszError,MB_ICONINFORMATION);
		SendLogonMessage();
		return;
	}

	//��ʾ��ʾ��
	CString strMessage=TEXT("�������ӷ����������Ժ�...");
	g_GlobalAttemper.ShowStatusMessage(strMessage,this);
	
	return;
}

//ȡ������
void CPlazaViewItem::OnCommandCancelConnect()
{
	g_GlobalAttemper.DestroyStatusWnd(this);
	m_ClientSocket->CloseSocket();
	SendLogonMessage();
	return;
}

//���±���
void CPlazaViewItem::UpdateTitleText()
{
	CString strTitle;
	strTitle.LoadString(AfxGetInstanceHandle(),IDS_MAIN_TITLE);
	AfxGetMainWnd()->SetWindowText(strTitle);

	return;
}

//�滭��ҳ
void CPlazaViewItem::DrawWebFrame(CDC * pDC)
{
	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//�滭��ҳ
	CSkinAide::DrawEncircleFrame(pDC,CRect(0,0,rcClient.Width(),rcClient.Height()),m_EncircleWeb);

	return;
}

afx_msg void CPlazaViewItem::OnBnClickedBEFORE()
{
	m_pHtmlBrower->GoBack();
}
afx_msg void CPlazaViewItem::OnBnClickedNEXT()
{
	m_pHtmlBrower->GoForward();
}
afx_msg void CPlazaViewItem::OnBnClickedMBRUSH()
{
	m_pHtmlBrower->Refresh();
}
afx_msg void CPlazaViewItem::OnBnClickedSTOP()
{
	m_pHtmlBrower->Stop();
}
afx_msg void CPlazaViewItem::OnBnClickedHOME()
{
	m_pHtmlBrower->Navigate(g_GlobalUnits.GetStationPage());
}
afx_msg void CPlazaViewItem::OnBnClickedBBS()
{
	m_pHtmlBrower->Navigate("http://221.234.25.72");
}

//-----------------------------------------------
//					the end
//-----------------------------------------------
