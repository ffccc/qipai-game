#include "Stdafx.h"
#include "Resource.h"
#include "VideoServiceControl.h"

//////////////////////////////////////////////////////////////////////////

//���ڱ�ʶ
#define IDC_VIDEO_SNED				100									//���ʹ���
#define IDC_VIDEO_RCEC				100									//���մ���
#define IDC_VIDEO_SR				100									//��Ƶ����

//��ť��ʶ
#define IDC_FLOWER					103									//�ͻ���ť
#define IDC_VIDEO					104									//��Ƶ��ť
#define IDC_AUDIO					105									//��Ƶ��ť
#define IDC_CLONE					106									//��¡��ť
#define IDC_MORE					107									//���ఴť


//λ�ñ�ʶ
#define X_SMALL_POS					10									//��ʼ����
#define Y_SMALL_POS					4									//��ʼ����
#define X_LARGE_POS					8									//��ʼ����
#define Y_LARGE_POS					5									//��ʼ����

//��ȱ�ʶ
#define W_SMALL_POS					80									//�������
#define H_SMALL_POS					60									//�߶�����
#define W_LARGE_POS					160									//�������
#define H_LARGE_POS					120									//�߶�����

//////////////////////////////////////////////////////////////////////////

//�˵�����
#define	IDM_UM_COPY_NAME			WM_USER+100							//��������
#define	IDM_UM_SET_CHAT				WM_USER+101							//��������
#define	IDM_UM_CANECL_CHAT			WM_USER+102							//ȡ������
#define	IDM_UM_ALLOW_LOOKON			WM_USER+103							//�Թۿ���
#define	IDM_UM_ENJOIN_LOOKON		WM_USER+104							//�Թۿ���
#define	IDM_UM_KICK_TABLE_USER		WM_USER+105							//�����û�


//�ʻ��˵�
#define	IDM_UM_FLOWER				WM_USER+306							//��������


//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CVideoServiceControl, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_FLOWER,OnBnClickedFlower)
	ON_BN_CLICKED(IDC_VIDEO,OnBnClickedVideo)
	ON_BN_CLICKED(IDC_AUDIO,OnBnClickedAudio)
	ON_BN_CLICKED(IDC_CLONE,OnBnClickedClone)
	ON_BN_CLICKED(IDC_MORE,OnBnClickedMore)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CVideoServiceControl, CWnd)
	ON_EVENT(CVideoServiceControl, IDC_VIDEO_SR, 202, OnEventVideoEnable, VTS_NONE)
	ON_EVENT(CVideoServiceControl, IDC_VIDEO_SR, 201, VolumeOutAV, VTS_PI4)
END_EVENTSINK_MAP()

//////////////////////////////////////////////////////////////////////////

//��̬����
CVideoServiceManager * CVideoServiceManager::m_pVideoServiceManager=NULL;//�������
HINSTANCE CVideoServiceManager::m_hResInstance=NULL;
tagVideoFrameImage CVideoServiceManager::m_VideoFrameImage;
tagPropertyMenuImage CVideoServiceManager::m_PropertyMenuImage;

//////////////////////////////////////////////////////////////////////////

//���캯��
CVideoServiceControl::CVideoServiceControl()
{
	//�ں˱���
	m_bInitVideo=false;

	//���Ʊ���
	m_bSmallMode=false;
	m_bLocalMode=false;
	m_bEnableVideo=true;
	m_bEnableAudio=true;

	//״̬����
	m_fVolumePercent=0.0f;

	//�豸��־
	m_bVidicon=false;
	m_bMicrophone=true;
	m_bOccupy=true;

	//�û���Ϣ
	m_dwUserID=0L;
	ZeroMemory(m_szAccounts,sizeof(m_szAccounts));

	//�ͻ�����
	m_dwTargerUserID = 0;
	m_pUserData=NULL;

	return;
}

//��������
CVideoServiceControl::~CVideoServiceControl()
{
	//�ͷſؼ�
	if ( m_FlowerEffectControl.m_hWnd != NULL )
		m_FlowerEffectControl.DestroyWindow();
}

//��Ϣ����
BOOL CVideoServiceControl::PreTranslateMessage(MSG * pMsg)
{
	//��ʾ��Ϣ
	if (m_ToolTipCtrl.m_hWnd!=NULL)
	{
		m_ToolTipCtrl.RelayEvent(pMsg);
	}

	return __super::PreTranslateMessage(pMsg);
}

//��Сģʽ
bool CVideoServiceControl::SetSmallMode(bool bSmallMode)
{
	//���ñ���
	if (m_bSmallMode!=bSmallMode)
	{
		//���ñ���
		m_bSmallMode=bSmallMode;

		//��������
		RectifyControl();
	}

	return true;
}

//�����û�
bool CVideoServiceControl::SetVideoUserInfo(DWORD dwUserID, LPCTSTR pszAccounts)
{
	//���ñ���
	m_dwUserID=dwUserID;
	lstrcpyn(m_szAccounts,pszAccounts,CountArray(m_szAccounts));

	//ˢ�´���
	if (m_hWnd!=NULL) Invalidate(FALSE);

	return true;
}

//������Ƶ
bool CVideoServiceControl::InitVideoService(bool bLocalMode, bool bSmallMode)
{
	//Ч��״̬
	ASSERT((m_hWnd!=NULL)&&(m_bInitVideo==false));
	if ((m_hWnd==NULL)||(m_bInitVideo==true)) return false;

	//��ȡ����
	ASSERT(CVideoServiceManager::GetInstance()!=NULL);
	CVideoServiceManager * pVideoServiceManager=CVideoServiceManager::GetInstance();

	//�Թ��ж�
	if ( pVideoServiceManager != NULL && pVideoServiceManager->m_pIClientKernel != NULL && pVideoServiceManager->m_pIClientKernel->IsLookonMode() ) return false;

	//���ñ���
	m_bInitVideo=true;
	m_bSmallMode=bSmallMode;
	m_bLocalMode=bLocalMode;

	//������Դ
	UpdateSkinResource();

	//��������
	if (m_bLocalMode==true)
	{
		//������Ƶ
		CRect rcCreate(0,0,0,0);
		m_VideoSend.Create(NULL,NULL,WS_CHILD,rcCreate,this,IDC_VIDEO_SNED);

		//�����豸
		if ((m_VideoSend.m_hWnd!=NULL)&&(m_VideoSend.HasVideoDrv()!=FALSE))
		{
			//���ñ���
			m_bVidicon=true;

			//�����豸
			m_VideoSend.SetVideoMode(0);
			m_VideoSend.SetVideoRate(0);
		}
	}
	else
	{
		//������Ƶ
		CRect rcCreate(0,0,0,0);
		m_VideoRece.Create(NULL,NULL,WS_CHILD,rcCreate,this,IDC_VIDEO_RCEC);
		m_VideoRece.SetAudioCache(200);
		m_VideoRece.SetVideoCache(200);
	}

	//��������
	RectifyControl();

	//��������	
	if (m_bSmallMode==true)
	{
		//����ͼƬ
		CImageHandle HandleFrame(&m_ImageSmallFrame);
		CImageHandle ImageVolume(&m_ImageSmallVolume);

		m_rcPaintVolume.left = (HandleFrame->GetWidth()-ImageVolume->GetWidth())/2;
		m_rcPaintVolume.top = H_SMALL_POS+8;
		m_rcPaintVolume.right = m_rcPaintVolume.left + ImageVolume->GetWidth();
		m_rcPaintVolume.bottom = m_rcPaintVolume.top + ImageVolume->GetHeight();
	}
	else
	{
		//����ͼƬ
		CImageHandle HandleFrame(&m_ImageLargeFrame);
		CImageHandle ImageVolume(&m_ImageLargeVolume);

		m_rcPaintVolume.left = (HandleFrame->GetWidth()-ImageVolume->GetWidth())/2;;
		m_rcPaintVolume.top = H_LARGE_POS+10;
		m_rcPaintVolume.right = m_rcPaintVolume.left + ImageVolume->GetWidth();
		m_rcPaintVolume.bottom = m_rcPaintVolume.top + ImageVolume->GetHeight();
	}
	return true;
}

//��ʾ��Ƶ
bool CVideoServiceControl::OnShowLocalVD(bool bShow)
{
	//��ʾ����
	if ( bShow )
	{
		if ( m_bLocalMode )
		{
			//��ʾ��Ƶ
			m_VideoSend.OpenSend();

			return true;
		}
	}
	else
	{
		if ( m_bLocalMode )
		{
			//�ر���Ƶ
			m_VideoSend.CloseSend();
			m_VideoSend.ShowWindow(SW_HIDE);
			
			return true;
		}
	}

	return false;
}

//��������
bool CVideoServiceControl::OnEventGameReset()
{
	//���ñ���
	m_dwUserID=0L;
	ZeroMemory(m_szAccounts,sizeof(m_szAccounts));

	//���ÿؼ�
	if (m_VideoRece.m_hWnd!=NULL)
	{
		m_VideoRece.CloseRece();
		m_VideoRece.ShowWindow(SW_HIDE);
	}

	//�ر�ͨ��
	if (m_VideoSend.m_hWnd!=NULL)
	{
	}

	return true;
}

//�û��뿪
bool CVideoServiceControl::OnEventUserLeave(DWORD dwRemoteUserID)
{
	//״̬Ч��
	ASSERT(m_bInitVideo==true);
	if (m_bInitVideo==false) return false;

	//����ģʽ
	if (m_bLocalMode==true)
	{
		//״̬Ч��
		if (m_VideoSend.m_hWnd==NULL) return false;

		//ɾ��ͨ��
		m_VideoSend.DeleteP2PSocket(dwRemoteUserID);

		return true;
	}

	//Զ��ģʽ
	if ((m_bLocalMode==false)&&(m_dwUserID==dwRemoteUserID))
	{
		//״̬Ч��
		if (m_VideoRece.m_hWnd==NULL) return false;

		//�ر���Ƶ
		m_VideoRece.CloseRece();

		//���ش���
		if (m_VideoRece.m_hWnd!=NULL) m_VideoRece.ShowWindow(SW_HIDE);

		//�����û�
		SetVideoUserInfo(0L,TEXT(""));

		return true;
	}

	return false;
}

//�û�����
bool CVideoServiceControl::OnEventUserEnter(DWORD dwRemoteUserID)
{
	//״̬Ч��
	ASSERT(m_bInitVideo==true);
	if (m_bInitVideo==false) return false;

	//����ģʽ
	if (m_bLocalMode==true)
	{
		//��ȡ����
		ASSERT(CVideoServiceManager::GetInstance()!=NULL);
		CVideoServiceManager * pVideoServiceManager=CVideoServiceManager::GetInstance();

		//����ͨ��
		unsigned char cRet = m_VideoSend.InsertP2PSocket(pVideoServiceManager->m_dwLocalUserID,dwRemoteUserID,pVideoServiceManager->m_dwVideoServer,
			pVideoServiceManager->m_wVideoPort);

		//�ɹ��ж�
		if ( cRet == 0 )
		{
			//��ʾ��Ϣ
			TCHAR szMsg[] = TEXT("����Ƶ����������ʧ�ܣ��������ܽ�����Ƶ��");
			IMessageProxy * pIMessageProxy = pVideoServiceManager->m_pIMessageProxy;
			if ( pIMessageProxy != NULL ) pIMessageProxy->InsertSystemString(szMsg, lstrlen(szMsg));

			//TCHAR szServerInfo[125];
			//BYTE *pByte=(BYTE *)&pVideoServiceManager->m_dwVideoServer;
			//_sntprintf(szServerInfo,CountArray(szServerInfo),TEXT("%d.%d.%d.%d,%ld"),pByte[0],pByte[1],pByte[2],pByte[3],pVideoServiceManager->m_wVideoPort);
			//pIMessageProxy->InsertSystemString(szServerInfo, lstrlen(szServerInfo));

			return false;
		}

		//������Ϣ
		WORD wNatPort=0,wLocalPort=0;
		DWORD dwNatAddr=0L,dwLocalAddr=0L;
		if (m_VideoSend.m_hWnd!=NULL) m_VideoSend.GetP2PInfo(dwRemoteUserID,&dwLocalAddr,&wLocalPort,&dwNatAddr,&wNatPort);

		//Ч������
 		if ((wNatPort==0L)||(dwNatAddr==0L)) return false;
		if ((wLocalPort==0L)||(dwLocalAddr==0L)) return false;

		//��������
		if ((pVideoServiceManager!=NULL)&&(pVideoServiceManager->m_pIClientKernel!=NULL))
		{
			//��������
			CMD_C_VideoConfig VideoConfig;
			ZeroMemory(&VideoConfig,sizeof(VideoConfig));

			//���ñ���
			VideoConfig.wNatPort=wNatPort;
			VideoConfig.dwNatAddr=dwNatAddr;
			VideoConfig.wLocalPort=wLocalPort;
			VideoConfig.dwLocalAddr=dwLocalAddr;
			VideoConfig.dwRemoteUserID=dwRemoteUserID;

			//��������
			IClientKernel * pIClientKernel=pVideoServiceManager->m_pIClientKernel;
			pIClientKernel->SendSocketData(MDM_GF_VIDEO,SUB_GF_C_VIDEO_CONFIG,&VideoConfig,sizeof(VideoConfig));

			return true;
		}

		//�������
		ASSERT(FALSE);

		return false;
	}

	return true;
}

//�û�����
bool CVideoServiceControl::OnEventUserConfig(DWORD dwRemoteUserID, tagVideoNetwordInfo * pVideoNetwordInfo)
{
	//״̬Ч��
	ASSERT(m_bInitVideo==true);
	if (m_bInitVideo==false) return false;

	//��ȡ����
	ASSERT(CVideoServiceManager::GetInstance()!=NULL);
	CVideoServiceManager * pVideoServiceManager=CVideoServiceManager::GetInstance();

	//Զ��ģʽ
	if ((m_bLocalMode==false)&&(m_dwUserID==dwRemoteUserID))
	{
		//����Ч��
		ASSERT(m_VideoRece.m_hWnd!=NULL);
		if (m_VideoRece.IsWindowVisible()) return true;

		//����Ƶ
		m_VideoRece.OpenRece();

		//����ͨ��
		unsigned char cRet = m_VideoRece.CreateP2P(pVideoServiceManager->m_dwLocalUserID,dwRemoteUserID,pVideoServiceManager->m_dwVideoServer,
			pVideoServiceManager->m_wVideoPort,pVideoNetwordInfo->dwLocalAddr,pVideoNetwordInfo->wLocalPort,pVideoNetwordInfo->dwNatAddr,
			pVideoNetwordInfo->wNatPort);

		//�ɹ��ж�
		if ( cRet == 0 )
		{
			//��ʾ��Ϣ
			TCHAR szMsg[] = TEXT("����Ƶ����������ʧ�ܣ��������ܽ�����Ƶ��");
			IMessageProxy * pIMessageProxy = pVideoServiceManager->m_pIMessageProxy;
			if ( pIMessageProxy != NULL ) pIMessageProxy->InsertSystemString(szMsg, lstrlen(szMsg));

			return false;
		}

		return true;
	}

	//�������
	ASSERT(FALSE);

	return false;
}

//��������
VOID CVideoServiceControl::RectifyControl()
{
	//�����ж�
	if (m_hWnd==NULL) return;

	//��������
	if (m_bInitVideo==true)
	{
		//������Դ
		CImageHandle HandleFrame;
		if (m_bSmallMode==true) HandleFrame.AttachResource(&m_ImageSmallFrame);
		if (m_bSmallMode==false) HandleFrame.AttachResource(&m_ImageLargeFrame);

		//��������
		INT nFrameWidth=HandleFrame->GetWidth();
		INT nFrameHeight=HandleFrame->GetHeight();
		SetWindowPos(NULL,0,0,nFrameWidth,nFrameHeight,SWP_NOZORDER|SWP_NOMOVE|SWP_NOCOPYBITS);

		//�ƶ��ؼ�
		if (m_bSmallMode==true)
		{
			if (m_VideoSend.m_hWnd!=NULL) m_VideoSend.SetWindowPos(NULL,X_SMALL_POS,Y_SMALL_POS,W_SMALL_POS,H_SMALL_POS,SWP_NOZORDER|SWP_NOACTIVATE);
			if (m_VideoRece.m_hWnd!=NULL) m_VideoRece.SetWindowPos(NULL,X_SMALL_POS,Y_SMALL_POS,W_SMALL_POS,H_SMALL_POS,SWP_NOZORDER|SWP_NOACTIVATE);
		}
		else
		{
			if (m_VideoSend.m_hWnd!=NULL) m_VideoSend.SetWindowPos(NULL,X_LARGE_POS,Y_LARGE_POS,W_LARGE_POS,H_LARGE_POS,SWP_NOZORDER|SWP_NOACTIVATE);
			if (m_VideoRece.m_hWnd!=NULL) m_VideoRece.SetWindowPos(NULL,X_LARGE_POS,Y_LARGE_POS,W_LARGE_POS,H_LARGE_POS,SWP_NOZORDER|SWP_NOACTIVATE);
		}

		//��������
		INT nSpace=m_bSmallMode?1:10;
		INT nXCenterPos=HandleFrame->GetWidth()/2;
		INT nYPos=m_bSmallMode?H_SMALL_POS+13:H_LARGE_POS+18;
		const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;

		//��ťλ��
		CRect rcButton;
		m_btFlower.GetWindowRect(&rcButton);

		//�ƶ���ť
		HDWP hDwp=BeginDeferWindowPos(32);
		DeferWindowPos(hDwp,m_btFlower,NULL,nXCenterPos-rcButton.Width()*5/2-nSpace*2,nYPos,0,0,uFlags);
		DeferWindowPos(hDwp,m_btVideo,NULL,nXCenterPos-rcButton.Width()*3/2-nSpace,nYPos,0,0,uFlags);
		DeferWindowPos(hDwp,m_btAudio,NULL,nXCenterPos-rcButton.Width()/2,nYPos,0,0,uFlags);
		DeferWindowPos(hDwp,m_btClone,NULL,nXCenterPos+rcButton.Width()/2+nSpace,nYPos,0,0,uFlags);
		DeferWindowPos(hDwp,m_btMore,NULL,nXCenterPos+rcButton.Width()*3/2+nSpace*2,nYPos,0,0,uFlags);

		////�ʻ�����
		//if ( m_FlowerEffectControl.m_hWnd != NULL )
		//{
		//	if ( m_bSmallMode == true )
		//	{
		//		CRect rcFlowerEffect;
		//		rcFlowerEffect.SetRect(X_SMALL_POS,Y_SMALL_POS,W_SMALL_POS+10,H_SMALL_POS+2);
		//		DeferWindowPos(hDwp,m_FlowerEffectControl,NULL,rcFlowerEffect.left,rcFlowerEffect.top,rcFlowerEffect.Width(),rcFlowerEffect.Height(),SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS);
		//	}
		//	else
		//	{
		//		CRect rcFlowerEffect;
		//		rcFlowerEffect.SetRect(X_LARGE_POS,Y_LARGE_POS,W_LARGE_POS+5,H_LARGE_POS+5);
		//		DeferWindowPos(hDwp,m_FlowerEffectControl,NULL,rcFlowerEffect.left,rcFlowerEffect.top,rcFlowerEffect.Width(),rcFlowerEffect.Height(),SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS);
		//	}
		//}

		EndDeferWindowPos(hDwp);
	}
	else
	{
		//��С����
		SetWindowPos(NULL,0,0,0,0,SWP_NOZORDER|SWP_NOMOVE|SWP_NOCOPYBITS);
	}

	//�ػ�����
	Invalidate(TRUE);

	return;
}

//�ػ�����
VOID CVideoServiceControl::OnPaint()
{
	CPaintDC dcPaint(this);

	//�����ж�
	ASSERT(m_bInitVideo==true);
	if (m_bInitVideo==false) return;

	//��ȡ��С
	CRect rcClient;
	GetClientRect(&rcClient);

	//�ڴ�DC
	CDC dcBuffer;
	dcBuffer.CreateCompatibleDC(&dcPaint);
	CBitmap BufferBmp;
	BufferBmp.CreateCompatibleBitmap(&dcPaint, rcClient.Width(), rcClient.Height());
	dcBuffer.SelectObject(&BufferBmp);

	//�����ж�
	CRect rcClipBox;
	dcPaint.GetClipBox(rcClipBox);
	if ( rcClipBox != m_rcPaintVolume )
	{
		//�滭����
		if (m_bSmallMode==true)
		{
			CImageHandle HandleFrame(&m_ImageSmallFrame);
			m_ImageSmallFrame.BitBlt(dcBuffer,0,0);

			//����ͼƬ
			CImageHandle ImageVolume(&m_ImageSmallVolume);

			INT nXPos=(HandleFrame->GetWidth()-m_ImageSmallVolume.GetWidth())/2;
			INT nYPos=H_SMALL_POS+8;
			m_ImageSmallVolume.BitBlt(dcBuffer,nXPos, nYPos);
		}
		else
		{
			CImageHandle HandleFrame(&m_ImageLargeFrame);
			m_ImageLargeFrame.BitBlt(dcBuffer,0,0);

			//����ͼƬ
			CImageHandle ImageVolume(&m_ImageLargeVolume);

			INT nXPos=(HandleFrame->GetWidth()-m_ImageLargeVolume.GetWidth())/2;
			INT nYPos=H_LARGE_POS+10;
			m_ImageLargeVolume.BitBlt(dcBuffer,nXPos, nYPos);
		}

		//��Ƶ��ʶ
		if (m_bVidicon==false)
		{
			if (m_bSmallMode==true)
			{
				CImageHandle HandlePhoto(&m_ImageSmallPhoto);
				m_ImageSmallPhoto.BitBlt(dcBuffer,X_SMALL_POS+(W_SMALL_POS-m_ImageSmallPhoto.GetWidth())/2,Y_SMALL_POS+(H_SMALL_POS-m_ImageSmallPhoto.GetHeight())/2);
			}
			else
			{
				CImageHandle HandlePhoto(&m_ImageLargePhoto);
				m_ImageLargePhoto.BitBlt(dcBuffer,X_LARGE_POS+(W_LARGE_POS-m_ImageLargePhoto.GetWidth())/2,Y_LARGE_POS+(H_LARGE_POS-m_ImageLargePhoto.GetHeight())/2);
			}
		}

		//�ж�ͷ��
		CVideoServiceManager *pVideoServiceManager = CVideoServiceManager::GetInstance();
		ASSERT(pVideoServiceManager!=NULL);
		IClientKernel *pIClientKernel = pVideoServiceManager->m_pIClientKernel;
		IUserFaceRes *pIUserFaceRes = pVideoServiceManager->m_pIUserFaceRes;
		if ( pIClientKernel != NULL  && pIUserFaceRes!=NULL)
		{
			//�滭ͷ��
			const tagUserData * pUserData = pIClientKernel->SearchUserItem(m_dwUserID);
			if(pUserData!=NULL)
			{
				CRect rect;
				if(m_bSmallMode)
				{
					rect.left = 10;
					rect.right = rcClient.Width()-rect.left*2;
					rect.top = 4;
					rect.bottom = m_rcPaintVolume.top-7;
				}
				else
				{
					rect.left = 7;
					rect.right = rcClient.Width()-rect.left*2;
					rect.top = 5;
					rect.bottom = m_rcPaintVolume.top-9;
				}
				if(pUserData->dwCustomFaceVer!=0)
				{
					pIUserFaceRes->DrawCustomBigFace(&dcBuffer,rect.left,rect.top,pUserData->dwUserID,pUserData->dwCustomFaceVer,
					rect.right,rect.bottom);
				}
				else if(pUserData->cbGender==1)
				{
					CImageHandle ImageFemale1(&m_ImageSmallDefMale);
					CImageHandle ImageFemale2(&m_ImageLargeDefMale);
					if(m_bSmallMode)m_ImageSmallDefMale.AlphaDrawImage(&dcBuffer,rect.left,rect.top,m_ImageSmallDefMale.GetWidth(),m_ImageSmallDefMale.GetHeight(),0,0,RGB(255,0,255));
					if(!m_bSmallMode)m_ImageLargeDefMale.AlphaDrawImage(&dcBuffer,rect.left,rect.top,m_ImageLargeDefMale.GetWidth(),m_ImageLargeDefMale.GetHeight(),0,0,RGB(255,0,255));
				}
				else 
				{
					CImageHandle ImageFemale1(&m_ImageSmallDefFemale);
					CImageHandle ImageFemale2(&m_ImageLargeDefFemale);
					if(m_bSmallMode)m_ImageSmallDefFemale.AlphaDrawImage(&dcBuffer,rect.left,rect.top,m_ImageSmallDefFemale.GetWidth(),m_ImageSmallDefFemale.GetHeight(),0,0,RGB(255,0,255));
					if(!m_bSmallMode)m_ImageLargeDefFemale.AlphaDrawImage(&dcBuffer,rect.left,rect.top,m_ImageLargeDefFemale.GetWidth(),m_ImageLargeDefFemale.GetHeight(),0,0,RGB(255,0,255));
				}
			}
		}

		dcPaint.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &dcBuffer, 0, 0, SRCCOPY);
	}
	else
	{
		if (m_bSmallMode==true)
		{
			//����ͼƬ
			CImageHandle ImageVolume(&m_ImageSmallVolume);
			CImageHandle ImageVolumeEx(&m_ImageSmallVolumeEx);

			m_ImageSmallVolume.BitBlt(dcBuffer,m_rcPaintVolume.left, m_rcPaintVolume.top);
			m_ImageSmallVolumeEx.BitBlt(dcBuffer,m_rcPaintVolume.left, m_rcPaintVolume.top,int(m_rcPaintVolume.Width()*m_fVolumePercent),m_rcPaintVolume.Height(),0,0);
		}
		else
		{
			//����ͼƬ
			CImageHandle ImageVolume(&m_ImageLargeVolume);
			CImageHandle ImageVolumeEx(&m_ImageLargeVolumeEx);

			m_ImageLargeVolume.BitBlt(dcBuffer,m_rcPaintVolume.left, m_rcPaintVolume.top);
			m_ImageLargeVolumeEx.BitBlt(dcBuffer,m_rcPaintVolume.left, m_rcPaintVolume.top,int(m_rcPaintVolume.Width()*m_fVolumePercent),m_rcPaintVolume.Height(),0,0);
		}

		dcPaint.BitBlt(rcClipBox.left, rcClipBox.top, rcClipBox.Width(), rcClipBox.Height(), &dcBuffer, rcClipBox.left, rcClipBox.top, SRCCOPY);
	}

	//������Դ
	dcBuffer.DeleteDC();
	BufferBmp.DeleteObject();	

	return;
}

//�滭����
BOOL CVideoServiceControl::OnEraseBkgnd(CDC * pDC)
{
	Invalidate(FALSE);
	UpdateWindow();

	return TRUE;
}

//��������
INT CVideoServiceControl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//�޸Ĵ�������
	ModifyStyle(0,WS_CLIPCHILDREN);

	//������ť
	CRect rcCreate(0,0,0,0);
	m_btMore.Create(TEXT(""),WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_MORE);
	m_btVideo.Create(TEXT(""),WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_VIDEO);
	m_btAudio.Create(TEXT(""),WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_AUDIO);
	m_btClone.Create(TEXT(""),WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_CLONE);
	m_btFlower.Create(TEXT(""),WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_FLOWER);

	//������ʾ
	m_ToolTipCtrl.Create(this);
	m_ToolTipCtrl.Activate(TRUE);
	m_ToolTipCtrl.AddTool(&m_btMore, TEXT("�鿴��Ϣ"));
	m_ToolTipCtrl.AddTool(&m_btVideo, TEXT("��ֹ��Ƶ"));
	m_ToolTipCtrl.AddTool(&m_btAudio, TEXT("��ֹ����"));
	m_ToolTipCtrl.AddTool(&m_btClone, TEXT("������Ƶ"));
	m_ToolTipCtrl.AddTool(&m_btFlower, TEXT("�����ʻ�"));

	//�����ؼ�
	m_FlowerEffectControl.CreateControl(this);

	//��������
	RectifyControl();

	return 0;
}

//�ͻ���ť
VOID CVideoServiceControl::OnBnClickedFlower()
{
	//��ȡĿ��
	m_dwTargerUserID = GetVideoUserID();
	if ( m_dwTargerUserID == 0 ) return;

	//�˵�����
	UINT uCommands[FLOWER_COUNT] ;
	for ( WORD wCmdIdx = 0; wCmdIdx < FLOWER_COUNT; ++wCmdIdx )
		uCommands[wCmdIdx] = IDM_UM_FLOWER + wCmdIdx;

	//�˵�λͼ
	CBitmap MenuBmps[FLOWER_COUNT];
	AfxSetResourceHandle(CVideoServiceManager::m_hResInstance);
	tagPropertyMenuImage & PropertyMenuImage=CVideoServiceManager::m_PropertyMenuImage;

	//����λͼ
	for(WORD i=0;i<FLOWER_COUNT;i++)
	{
		MenuBmps[i].LoadBitmap( PropertyMenuImage.uMenuGift[i] );
	}

	AfxSetResourceHandle(GetModuleHandle(NULL));
    
    //�����˵�
	CMenu GiftMenu;
	GiftMenu.CreatePopupMenu();

	//���ò˵�
	for ( UINT uMenuItem = 0; uMenuItem < FLOWER_COUNT; ++uMenuItem )
	{
		//�˵���Ϣ
		CString strMenuItem = g_FlowerDescribe[uMenuItem].szName;
		CString strGiftPay;
		strGiftPay.Format("%ld", CPropertyBar::m_FlowerInfo[uMenuItem].lPrice);
		strMenuItem += strGiftPay;

		//���Ӳ˵�
		GiftMenu.AppendMenu( 0, uCommands[uMenuItem], strMenuItem);
		GiftMenu.SetMenuItemBitmaps(uCommands[uMenuItem], MF_BYCOMMAND,&MenuBmps[uMenuItem], &MenuBmps[uMenuItem] );
	}	

	//��ȡλ��
	CPoint MousePoint;
	GetCursorPos(&MousePoint);

	GiftMenu.TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON,MousePoint.x,MousePoint.y,this);

	//�ͷ���Դ
	for ( INT nBmpIdx = 0; nBmpIdx < FLOWER_COUNT; ++nBmpIdx ) MenuBmps[nBmpIdx].DeleteObject();
	GiftMenu.DestroyMenu();

	return;
}

//�˵�����
BOOL CVideoServiceControl::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//��ȡ����
	UINT uCmd = LOWORD(wParam);

	if ( IDM_UM_FLOWER <= uCmd && uCmd <= IDM_UM_FLOWER + FLOWER_COUNT )
	{
		//����ת��
		UINT uBaseCmd = IDM_UM_FLOWER;
		UINT uFlowerIdx = uCmd - uBaseCmd;

		//������Ϣ
		CString strMsg;
		LONG lValue = CPropertyBar::m_FlowerInfo[uFlowerIdx].lPrice;
		strMsg.Format(TEXT("��ѡ�������ͣ�%s�ݣ�������������п۳�[%ld]��Ϸ�ҡ�\n�����ǡ�����\n������ȡ����β���"),g_FlowerDescribe[uFlowerIdx].szName, lValue, lValue );
		if(IDYES!=MessageBox(strMsg,TEXT("��ܰ��ʾ"),MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2))
			return TRUE;

		//�����ж�
		DWORD dwRcvUserID = GetVideoUserID();
		if ( dwRcvUserID == 0 || m_dwTargerUserID != dwRcvUserID )
		{
			MessageBox(strMsg,TEXT("��ܰ��ʾ"), MB_ICONINFORMATION );
			return TRUE;
		}

		//��ȡ����
		ASSERT(CVideoServiceManager::GetInstance()!=NULL);
		CVideoServiceManager * pVideoServiceManager=CVideoServiceManager::GetInstance();
		IClientKernel *pIClientKernel = pVideoServiceManager->m_pIClientKernel;
		ASSERT( pIClientKernel != NULL );

		//��������
		CMD_GF_Gift Gift;
		ZeroMemory( &Gift, sizeof( Gift ) );

		Gift.cbSendLocation = LOCATION_GAME_ROOM;
		Gift.wFlowerCount=1;
		Gift.dwRcvUserID = GetVideoUserID();
		Gift.dwSendUserID = pVideoServiceManager->m_dwLocalUserID;
		Gift.wGiftID = uFlowerIdx;

		//������Ϣ
		pIClientKernel->SendSocketData( MDM_GF_PRESENT,SUB_GF_FLOWER,&Gift, sizeof(Gift));

		return TRUE;
	}

	//��ȡ����
	ASSERT(CVideoServiceManager::GetInstance()!=NULL);
	CVideoServiceManager * pVideoServiceManager=CVideoServiceManager::GetInstance();
	ASSERT( pVideoServiceManager != NULL ) ;

	//��������
	IClientKernel * pIClientKernel=pVideoServiceManager->m_pIClientKernel;
	ASSERT( pIClientKernel != NULL ) ;

	switch (LOWORD(wParam))
	{
	case IDM_UM_COPY_NAME:		//��������
		{
			if ( m_pUserData == NULL ) return TRUE;

			//Ѱ�����
			if ((m_pUserData==NULL)||(m_pUserData->dwUserID==0)) return TRUE;

			//�򿪼��а�
			if (OpenClipboard()==FALSE) return TRUE;
			if (EmptyClipboard()==FALSE) 
			{
				CloseClipboard();
				return TRUE;
			}

			//��������
			HANDLE hData=GlobalAlloc(GMEM_MOVEABLE|GMEM_ZEROINIT,NAME_LEN);
			if (hData==NULL) 
			{
				CloseClipboard();
				return TRUE;
			}
			LPTSTR szMemName=(LPTSTR)GlobalLock(hData);
			lstrcpy(szMemName,m_pUserData->szName);
			SetClipboardData(CF_TEXT,hData);
			GlobalUnlock(hData);
			CloseClipboard();

			return TRUE;
		}
	case IDM_UM_KICK_TABLE_USER:	//�����û�
		{
			//Ѱ�����
			if ((m_pUserData==NULL)||(m_pUserData->dwUserID==0)) return TRUE;

			//������Ϣ
			CMD_GF_KickTableUser KickTableUser;
			KickTableUser.dwUserID=m_pUserData->dwUserID;
			pIClientKernel->SendSocketData(MDM_GF_FRAME,SUB_GF_KICK_TABLE_USER,&KickTableUser,sizeof(KickTableUser));

			return TRUE;
		}
	}

	return __super::OnCommand(wParam,lParam);
}

//��Ƶ��ť
VOID CVideoServiceControl::OnBnClickedVideo()
{
	//ռ���ж�
	if ( m_bOccupy ) return ;

	//���ñ���
	m_bEnableVideo=!m_bEnableVideo;

	//���ð�ť
	HINSTANCE hInstance=CVideoServiceManager::m_hResInstance;
	tagVideoFrameImage & VideoFrameImage=CVideoServiceManager::m_VideoFrameImage;
	if (!m_bSmallMode) m_btVideo.SetButtonImage((m_bEnableVideo)?VideoFrameImage.uBtBigSee:VideoFrameImage.uBtBigNosee,hInstance,false);
	if (m_bSmallMode) m_btVideo.SetButtonImage((m_bEnableVideo)?VideoFrameImage.uBtSmallSee:VideoFrameImage.uBtSmallNosee,hInstance,false);

	if ( m_bEnableVideo ) m_ToolTipCtrl.AddTool(&m_btVideo, TEXT("��ֹ��Ƶ"));
	else m_ToolTipCtrl.AddTool(&m_btVideo, TEXT("������Ƶ"));

	//�����豸
	if (m_VideoSend.m_hWnd!=NULL)
	{
		m_VideoSend.EnableVideo(m_bEnableVideo);
		m_VideoSend.ShowWindow(m_bEnableVideo?SW_SHOW:SW_HIDE);

		if ( m_bLocalMode )
		{
			//����Ƶ
			if ( m_bEnableVideo )
			{
				//��ȡ����
				ASSERT(CVideoServiceManager::GetInstance()!=NULL);
				CVideoServiceManager * pVideoServiceManager=CVideoServiceManager::GetInstance();

				//��������
				IClientKernel * pIClientKernel=pVideoServiceManager->m_pIClientKernel;
				if ( pIClientKernel != NULL ) pIClientKernel->SendSocketData(MDM_GF_VIDEO,SUB_GF_C_VIDEO_OPEN);
			}
			else
			{
				//��ȡ����
				ASSERT(CVideoServiceManager::GetInstance()!=NULL);
				CVideoServiceManager * pVideoServiceManager=CVideoServiceManager::GetInstance();

				//��������
				IClientKernel * pIClientKernel=pVideoServiceManager->m_pIClientKernel;
				if ( pIClientKernel != NULL ) pIClientKernel->SendSocketData(MDM_GF_VIDEO,SUB_GF_C_VIDEO_CLOSE);
			}
		}
	}

	//�����豸
	if (m_VideoRece.m_hWnd!=NULL)
	{
		m_VideoRece.EnableVideo(m_bEnableVideo);
		m_VideoRece.ShowWindow(m_bEnableVideo?SW_SHOW:SW_HIDE);
	}

	//�ػ��ؼ�
	Invalidate(FALSE);

	return;
}

//�ر���Ƶ
bool CVideoServiceControl::OnEventCloseVideo()
{
	if ( m_bLocalMode && m_VideoSend.m_hWnd != NULL ) m_VideoSend.ShowWindow( SW_HIDE );
	else if ( m_VideoRece.m_hWnd != NULL ) m_VideoRece.ShowWindow( SW_HIDE );

	//ʹ�ܰ�ť
	m_btVideo.EnableWindow(FALSE);
	m_btAudio.EnableWindow(FALSE);
	m_btClone.EnableWindow(FALSE);

	return true;
}

//����Ƶ
bool CVideoServiceControl::OnEventOpenVideo()
{
	if ( m_bLocalMode && m_VideoSend.m_hWnd != NULL ) m_VideoSend.ShowWindow( SW_SHOW );
	else if ( m_VideoRece.m_hWnd != NULL ) m_VideoRece.ShowWindow( SW_SHOW );

	//���ð�ť
	m_btVideo.EnableWindow(TRUE);
	m_btAudio.EnableWindow(TRUE);
	m_btClone.EnableWindow(TRUE);
	m_btFlower.EnableWindow(TRUE);

	return true;
}

//�����ʻ�
bool CVideoServiceControl::OnEventFlower(UINT uFlowerEffectID)
{
	//�Ϸ��ж�
	if ( m_FlowerEffectControl.m_hWnd == NULL ) return false;

	//��ʾ����
	m_FlowerEffectControl.StartFlowerEffect(uFlowerEffectID);
	m_FlowerEffectControl.ShowWindow(SW_SHOW);
	m_FlowerEffectControl.BringWindowToTop();

	return true;
}

//������Դ
bool CVideoServiceControl::UpdateSkinResource()
{
	//������Դ
	HINSTANCE hResInstance=CVideoServiceManager::m_hResInstance;
	ASSERT(hResInstance!=NULL);
	if(hResInstance==NULL)return false;
	tagVideoFrameImage & VideoFrameImage=CVideoServiceManager::m_VideoFrameImage;

	m_ImageSmallPhoto.SetLoadInfo(VideoFrameImage.uSmallPhoto,hResInstance,false);
	m_ImageLargePhoto.SetLoadInfo(VideoFrameImage.uBigPhoto,hResInstance,false);
	m_ImageSmallFrame.SetLoadInfo(VideoFrameImage.uSmallFrame,hResInstance,false);
	m_ImageLargeFrame.SetLoadInfo(VideoFrameImage.uBigFrame,hResInstance,false);

	m_ImageLargeVolume.SetLoadInfo(VideoFrameImage.uBigVolume,hResInstance,false);
	m_ImageLargeVolumeEx.SetLoadInfo(VideoFrameImage.uBigVolumeEx,hResInstance,false);
	m_ImageSmallVolumeEx.SetLoadInfo(VideoFrameImage.uSmallVolumeEx,hResInstance,false);
	m_ImageSmallVolume.SetLoadInfo(VideoFrameImage.uSmallVolumeEx,hResInstance,false);	
	m_ImageSmallDefFemale.SetLoadInfo(VideoFrameImage.uSmallFemale,hResInstance,false);
	m_ImageSmallDefMale.SetLoadInfo(VideoFrameImage.uSmallMale,hResInstance,false);
	m_ImageLargeDefFemale.SetLoadInfo(VideoFrameImage.uBigFemale,hResInstance,false);
	m_ImageLargeDefMale.SetLoadInfo(VideoFrameImage.uBigMale,hResInstance,false);

	//���ð�ť
	m_btMore.SetButtonImage(m_bSmallMode?VideoFrameImage.uBtSmallMore:VideoFrameImage.uBtBigMore,hResInstance,false);
	m_btClone.SetButtonImage(m_bSmallMode?VideoFrameImage.uBtSmallResore:VideoFrameImage.uBtBigResore,hResInstance,false);
	m_btFlower.SetButtonImage(m_bSmallMode?VideoFrameImage.uBtSmallFlower:VideoFrameImage.uBtBigFlower,hResInstance,false);

	if (!m_bSmallMode) m_btVideo.SetButtonImage((m_bEnableVideo)?VideoFrameImage.uBtBigSee:VideoFrameImage.uBtBigNosee,hResInstance,false);
	if (m_bSmallMode) m_btVideo.SetButtonImage((m_bEnableVideo)?VideoFrameImage.uBtSmallSee:VideoFrameImage.uBtSmallNosee,hResInstance,false);
	if (!m_bSmallMode) m_btAudio.SetButtonImage((m_bEnableAudio)?VideoFrameImage.uBtBigTalk:VideoFrameImage.uBtBigNotalk,hResInstance,false);
	if (m_bSmallMode) m_btAudio.SetButtonImage((m_bEnableAudio)?VideoFrameImage.uBtSmallTalk:VideoFrameImage.uBtSmallNotalk,hResInstance,false);

	return true;
}

//��Ƶ��ť
VOID CVideoServiceControl::OnBnClickedAudio()
{
	//���ñ���
	m_bEnableAudio=!m_bEnableAudio;

	//���ð�ť
	HINSTANCE hInstance=CVideoServiceManager::m_hResInstance;
	tagVideoFrameImage & VideoFrameImage=CVideoServiceManager::m_VideoFrameImage;
	if (!m_bSmallMode) m_btAudio.SetButtonImage((m_bEnableAudio)?VideoFrameImage.uBtBigTalk:VideoFrameImage.uBtBigNotalk,hInstance,false);
	if (m_bSmallMode) m_btAudio.SetButtonImage((m_bEnableAudio)?VideoFrameImage.uBtSmallTalk:VideoFrameImage.uBtSmallNotalk,hInstance,false);

	//�����豸
	if (m_VideoSend.m_hWnd!=NULL) m_VideoSend.EnableAudio(m_bEnableAudio);
	if (m_VideoRece.m_hWnd!=NULL) m_VideoRece.EnableAudio(m_bEnableAudio);

	if ( m_bEnableAudio ) m_ToolTipCtrl.AddTool(&m_btAudio, TEXT("��ֹ����"));
	else m_ToolTipCtrl.AddTool(&m_btAudio, TEXT("��������"));

	return;
}

//��¡��ť
VOID CVideoServiceControl::OnBnClickedClone()
{
	//ռ���ж�
	if ( m_bOccupy ) return ;

	//����ģʽ
	if (m_bLocalMode==true)
	{
		ASSERT(m_VideoSend.m_hWnd!=NULL);
		if (m_VideoSend.m_hWnd!=NULL) m_VideoSend.ScaleVideo();
	}

	//Զ��ģʽ
	if (m_bLocalMode==false)
	{
		ASSERT(m_VideoRece.m_hWnd!=NULL);
		if (m_VideoRece.m_hWnd!=NULL) m_VideoRece.ScaleVideo();
	}

	return;
}

//���ఴť
VOID CVideoServiceControl::OnBnClickedMore()
{
	//��ȡλ��
	CPoint MousePoint;
	GetCursorPos(&MousePoint);

	//��ȡ����
	ASSERT(CVideoServiceManager::GetInstance()!=NULL);
	CVideoServiceManager * pVideoServiceManager=CVideoServiceManager::GetInstance();
	ASSERT( pVideoServiceManager != NULL ) ;

	//��������
	IClientKernel * pIClientKernel=pVideoServiceManager->m_pIClientKernel;
	ASSERT( pIClientKernel != NULL ) ;

	//��ȡ�û�
	TCHAR szBuffer[512]=TEXT("");
	m_pUserData=pIClientKernel->SearchUserItem(m_dwUserID);
	if(m_pUserData==NULL) return;

	//Ԥ�ȼ���
	double dFleeRate=0.0;
	LONG lPlayCount=m_pUserData->lWinCount+m_pUserData->lLostCount+m_pUserData->lDrawCount+m_pUserData->lFleeCount;
	if (m_pUserData->lFleeCount>0)
	{
		dFleeRate=(double)(m_pUserData->lFleeCount*100)/(double)lPlayCount;
		if (dFleeRate<0.01) dFleeRate=0.0;
	}

	//����˵�
	CMenu UserInfoMenu;
	UserInfoMenu.CreatePopupMenu();

	//���ܲ˵�
	UserInfoMenu.AppendMenu(0,IDM_UM_COPY_NAME,TEXT("�����û���"));
	UserInfoMenu.AppendMenu(MF_SEPARATOR);

	//�û���ν
	LPCTSTR pszMemberOrder[]={TEXT("��ͨ��Ա"),TEXT("�м���Ա"),TEXT("�߼����")};
	if ((m_pUserData->cbMemberOrder>0)&&(m_pUserData->cbMemberOrder<CountArray(pszMemberOrder)))
	{
		_snprintf(szBuffer,sizeof(szBuffer),TEXT("�û�����%s [ %s ]"),m_pUserData->szName,pszMemberOrder[m_pUserData->cbMemberOrder]);
	}
	else _snprintf(szBuffer,sizeof(szBuffer),TEXT("�û�����%s"),m_pUserData->szName);
	UserInfoMenu.AppendMenu(0,0,szBuffer);

	//�û� ID
	_snprintf(szBuffer,sizeof(szBuffer),TEXT("��Ϸ ID��%ld"),m_pUserData->dwGameID);
	UserInfoMenu.AppendMenu(0,0,szBuffer);

	//�û�����
	_snprintf(szBuffer,sizeof(szBuffer),TEXT("����ֵ��%ld"),m_pUserData->lExperience);
	UserInfoMenu.AppendMenu(0,0,szBuffer);

	//�û�����
	_snprintf(szBuffer,sizeof(szBuffer),TEXT("���֣�%ld  �ܾ�����%ld  �����ʣ�%5.2f%%"),m_pUserData->lScore,lPlayCount,dFleeRate);
	UserInfoMenu.AppendMenu(0,0,szBuffer);

	//��ȡ�û�
	const tagUserData * pMeUserData=pIClientKernel->GetMeUserInfo();

	//���˲˵�
	if (pMeUserData->dwUserID!=m_pUserData->dwUserID)
	{
		//���Ʋ˵�
		bool bEnable=(pMeUserData->cbMemberOrder>0);
		if ((bEnable==true)&&(m_pUserData->cbMemberOrder>pMeUserData->cbMemberOrder)) bEnable=false;
		if ((bEnable==true)&&(m_pUserData->cbMasterOrder>0)&&(pMeUserData->cbMasterOrder==0)) bEnable=false;

		//����˵�
		_snprintf(szBuffer,sizeof(szBuffer),TEXT("�� [ %s ] �߳���Ϸ����"),m_pUserData->szName);
		UserInfoMenu.AppendMenu((bEnable==true)?MF_ENABLED:MF_GRAYED,IDM_UM_KICK_TABLE_USER,szBuffer);
	}

	//��ʾ�˵�
	UserInfoMenu.TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON,MousePoint.x,MousePoint.y,this);

	return;
}

//ʹ��֪ͨ
VOID CVideoServiceControl::OnEventVideoEnable()
{
	//ռ������
	m_bOccupy = false;

	//����ģʽ
	if (m_VideoSend.m_hWnd!=NULL) m_VideoSend.ShowWindow(SW_SHOW);
	if (m_VideoRece.m_hWnd!=NULL) m_VideoRece.ShowWindow(SW_SHOW);

	return;
}

//��Ƶ����
void CVideoServiceControl::VolumeOutAV(unsigned long *Volume)
{
	//��ȡ����
	ASSERT(CVideoServiceManager::GetInstance()!=NULL);
	CVideoServiceManager * pVideoServiceManager=CVideoServiceManager::GetInstance();

	//�������
	m_fVolumePercent = *Volume / 500.f;
	if ( m_fVolumePercent > 1 ) m_fVolumePercent=1.0f;

	//���½���
	InvalidateRect(&m_rcPaintVolume);
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CVideoServiceManager::CVideoServiceManager()
{
	//��Ƶ��Ϣ
	m_wVideoPort=0;
	m_dwVideoServer=0L;
	m_dwLocalUserID=0L;
	ZeroMemory( m_bOffLine, sizeof( m_bOffLine ) );

	//�������
	m_pIClientKernel=NULL;
	m_pIUserFaceRes =NULL;
	m_pIMessageProxy=NULL;
	ZeroMemory(m_VideoServiceControl,sizeof(m_VideoServiceControl));

	//���ö���
	ASSERT(m_pVideoServiceManager==NULL);
	if (m_pVideoServiceManager==NULL) m_pVideoServiceManager=this;

	return;
}

//��������
CVideoServiceManager::~CVideoServiceManager()
{
}

//���ýӿ�
bool CVideoServiceManager::SetClientKernel(IUnknownEx * pIUnknownEx)
{
	//���ýӿ�
	if (pIUnknownEx!=NULL)
	{
		//��ѯ�ӿ�
		ASSERT(QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IClientKernel)!=NULL);
		m_pIClientKernel=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IClientKernel);

		//�ɹ��ж�
		if (m_pIClientKernel==NULL) return false;
	}
	else m_pIClientKernel=NULL;

	return true;
}

//���ýӿ�
bool CVideoServiceManager::SetUserFaceRes(IUnknownEx * pIUnknownEx)
{
	ASSERT(pIUnknownEx!=NULL);
	m_pIUserFaceRes=(IUserFaceRes *)pIUnknownEx->QueryInterface(IID_IUserFaceRes,VER_IUserFaceRes);
	return (m_pIUserFaceRes!=NULL);
}

//������Ϣ
bool CVideoServiceManager::SetVideoServiceInfo(DWORD dwLoaclUserID, DWORD dwVideoServer, WORD wVideoPort)
{
	//���ñ���
	m_wVideoPort=wVideoPort;
	m_dwVideoServer=dwVideoServer;
	m_dwLocalUserID=dwLoaclUserID;

	return true;
}

//���ö���
bool CVideoServiceManager::SetVideoServiceControl(WORD wVideoIndex, CVideoServiceControl * pVideoServiceControl)
{
	//��ȡ����
	ASSERT(wVideoIndex<MAX_CHAIR);
	if (wVideoIndex>=MAX_CHAIR) return false;

	//���ö���
	m_VideoServiceControl[wVideoIndex]=pVideoServiceControl;

	return true;
}

//������Դ
void CVideoServiceManager::SetSkinResource(HINSTANCE hResInstance,tagVideoFrameImage *pVideoFrameImage,tagPropertyMenuImage *pPropertyMenuImage)
{
	//��Դ����
	m_hResInstance=hResInstance;
	CopyMemory(&m_VideoFrameImage,pVideoFrameImage,sizeof(m_VideoFrameImage));
	CopyMemory(&m_PropertyMenuImage,pPropertyMenuImage,sizeof(m_PropertyMenuImage));

	return ;
}

//��������
bool CVideoServiceManager::OnEventGameReset()
{
	//��������
	for (WORD i=0;i<MAX_CHAIR;i++)
	{
		if (m_VideoServiceControl[i]!=NULL)
		{
			m_VideoServiceControl[i]->OnEventGameReset();
		}
	}

	return true;
}

//�û��뿪
bool CVideoServiceManager::OnEventUserLeave(WORD wVideoIndex, DWORD dwRemoteUserID)
{
	//�¼�֪ͨ
	ASSERT(wVideoIndex<MAX_CHAIR);
	if (wVideoIndex>=MAX_CHAIR) return false;

	//Ч�����
	ASSERT(m_VideoServiceControl[wVideoIndex]!=NULL);
	ASSERT(m_VideoServiceControl[wVideoIndex]->GetVideoUserID()==dwRemoteUserID);

	//�������
	for (WORD i=0;i<MAX_CHAIR;i++)
	{
		if (m_VideoServiceControl[i]!=NULL)
		{
			DWORD dwVideoUserID=m_VideoServiceControl[i]->GetVideoUserID();
			if ((dwVideoUserID==m_dwLocalUserID)||(dwVideoUserID==dwRemoteUserID))
			{
				m_VideoServiceControl[i]->OnEventUserLeave(dwRemoteUserID);
			}
		}
	}

	return true;
}

//�û�����
bool CVideoServiceManager::OnEventUserEnter(WORD wVideoIndex, DWORD dwRemoteUserID)
{
	//�¼�֪ͨ
	ASSERT(wVideoIndex<MAX_CHAIR);
 	if (wVideoIndex>=MAX_CHAIR) return false;

	//Ч�����
	ASSERT(m_VideoServiceControl[wVideoIndex]!=NULL);
	ASSERT(m_VideoServiceControl[wVideoIndex]->GetVideoUserID()==0L);

	//�����û�
	if (m_VideoServiceControl[wVideoIndex]!=NULL)
	{
		m_VideoServiceControl[wVideoIndex]->SetVideoUserInfo(dwRemoteUserID,TEXT("��Ƶ�û�"));
	}

	//�����û�
	if (dwRemoteUserID!=m_dwLocalUserID)
	{
		for (WORD i=0;i<MAX_CHAIR;i++)
		{
			if (m_VideoServiceControl[i]!=NULL)
			{
				DWORD dwVideoUserID=m_VideoServiceControl[i]->GetVideoUserID();
				if (dwVideoUserID==m_dwLocalUserID)
					if ( m_VideoServiceControl[i]->OnEventUserEnter(dwRemoteUserID) )
					{
						//���ñ���
						m_bOffLine[wVideoIndex] = false;
						break;
					}
			}
		}
	}

	return true;
}

//�û�����
bool CVideoServiceManager::OnEventUserConfig(WORD wVideoIndex, DWORD dwRemoteUserID, tagVideoNetwordInfo * pVideoNetwordInfo)
{
	//�¼�֪ͨ
	ASSERT(wVideoIndex<MAX_CHAIR);
	if (wVideoIndex>=MAX_CHAIR) return false;

	//Ч�����
	ASSERT(m_VideoServiceControl[wVideoIndex]!=NULL);
	ASSERT(m_VideoServiceControl[wVideoIndex]->GetVideoUserID()==dwRemoteUserID);

	//�����û�
	if (m_VideoServiceControl[wVideoIndex]!=NULL)
	{
		m_VideoServiceControl[wVideoIndex]->OnEventUserConfig(dwRemoteUserID, pVideoNetwordInfo);
	}

	return true;
}

//��ʾ��Ƶ
bool CVideoServiceManager::OnShowLocalVD(bool bShow)
{
	for (WORD i=0;i<MAX_CHAIR;i++)
	{
		if (m_VideoServiceControl[i]!=NULL)
		{
			m_VideoServiceControl[i]->OnShowLocalVD(bShow);
		}
	}

	return false;
}

//�����û�
bool CVideoServiceManager::OnUpdateUserItem( WORD wVideoIndex, BYTE cbUserStatus, DWORD dwRemoteUserID )
{
	//����Ч��
	ASSERT(wVideoIndex<MAX_CHAIR);
	ASSERT(m_VideoServiceControl[wVideoIndex]!=NULL);

	//����Ч��
	if (wVideoIndex>=MAX_CHAIR) return false;
	if (m_VideoServiceControl[wVideoIndex]==NULL) return false;	

	if (cbUserStatus==US_OFFLINE )
	{
		//���ñ���
		m_bOffLine[wVideoIndex]=true;
		return OnEventUserLeave(wVideoIndex,dwRemoteUserID);
	}
	else if (cbUserStatus==US_PLAY)
	{
		//�����ж�
		if (m_bOffLine[wVideoIndex]) OnEventUserEnter(wVideoIndex,dwRemoteUserID);

	}

	return false;
}

//�ر���Ƶ
bool CVideoServiceManager::OnEventCloseVideo(WORD wVideoIndex)
{
	//�Ϸ���֤
	ASSERT( wVideoIndex < MAX_CHAIR );
	ASSERT( m_VideoServiceControl[wVideoIndex] != NULL );
	if ( wVideoIndex >= MAX_CHAIR ) return false;
	if ( m_VideoServiceControl[wVideoIndex] == NULL ) return false;

	m_VideoServiceControl[wVideoIndex]->OnEventCloseVideo();

	return true;
}

//����Ƶ
bool CVideoServiceManager::OnEventOpenVideo(WORD wVideoIndex)
{
	//�Ϸ���֤
	ASSERT( wVideoIndex < MAX_CHAIR );
	ASSERT( m_VideoServiceControl[wVideoIndex] != NULL );
	if ( wVideoIndex >= MAX_CHAIR ) return false;
	if ( m_VideoServiceControl[wVideoIndex] == NULL ) return false;

	m_VideoServiceControl[wVideoIndex]->OnEventOpenVideo();
	
	return true;
}

//�����ʻ�
bool CVideoServiceManager::OnEventFlower(WORD wVideoIndex, UINT uFlowerEffectID)
{
	//�Ϸ���֤
	ASSERT( wVideoIndex < MAX_CHAIR );
	ASSERT( m_VideoServiceControl[wVideoIndex] != NULL );
	if ( wVideoIndex >= MAX_CHAIR ) return false;
	if ( m_VideoServiceControl[wVideoIndex] == NULL ) return false;

	return m_VideoServiceControl[wVideoIndex]->OnEventFlower(uFlowerEffectID);
}

//���ýӿ�
bool CVideoServiceManager::SetMessageProxy(IUnknownEx * pIUnknownEx)
{
	ASSERT(pIUnknownEx!=NULL);
	m_pIMessageProxy=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IMessageProxy);
	ASSERT(m_pIMessageProxy!=NULL);
	return (m_pIMessageProxy!=NULL);
}

//////////////////////////////////////////////////////////////////////////
