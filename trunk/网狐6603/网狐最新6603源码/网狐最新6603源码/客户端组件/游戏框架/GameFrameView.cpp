#include "StdAfx.h"
#include "Resource.h"
#include "GlobalUnits.h"
#include "GameFrameView.h"
#include "GameFrameEngine.h"

//////////////////////////////////////////////////////////////////////////////////
//��̬����

CGameFrameView * CGameFrameView::m_pGameFrameView=NULL;					//��Ϸ��ͼ

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameFrameView, CWnd)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGameFrameView::CGameFrameView()
{
	//λ�ñ���
	ZeroMemory(&m_ptClock,sizeof(m_ptClock));
	ZeroMemory(&m_ptReady,sizeof(m_ptReady));
	ZeroMemory(&m_ptAvatar,sizeof(m_ptAvatar));
	ZeroMemory(&m_ptNickName,sizeof(m_ptNickName));

	//�û�����
	ZeroMemory(m_wUserClock,sizeof(m_wUserClock));
	ZeroMemory(m_pIClientUserItem,sizeof(m_pIClientUserItem));

	//����ӿ�
	m_pIStringMessage=NULL;
	m_pIGameLevelParser=NULL;

	//���ñ���
	ASSERT(m_pGameFrameView==NULL);
	if (m_pGameFrameView==NULL) m_pGameFrameView=this;

	//ע�����
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
	pGlobalUnits->RegisterGlobalModule(MODULE_GAME_FRAME_VIEW,QUERY_ME_INTERFACE(IUnknownEx));

	return;
}

//��������
CGameFrameView::~CGameFrameView()
{
	//���ñ���
	ASSERT(m_pGameFrameView==this);
	if (m_pGameFrameView==this) m_pGameFrameView=NULL;

	return;
}

//�ӿڲ�ѯ
VOID * CGameFrameView::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IGameFrameView,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IGameFrameView,Guid,dwQueryVer);
	return NULL;
}

//���ھ��
HWND CGameFrameView::GetGameViewHwnd()
{
	return m_hWnd;
}

//��������
bool CGameFrameView::CreateGameViewWnd(CWnd * pParentWnd, UINT nID)
{
	//Ч��״̬
	ASSERT(m_hWnd==NULL);
	if (m_hWnd!=NULL) return false;

	//��ȡ����
	ASSERT(CGlobalUnits::GetInstance()!=NULL);
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();

	//��������
	CRect rcCreate(0,0,0,0);
	Create(NULL,NULL,WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,rcCreate,pParentWnd,nID);

	//��ѯ�ӿ�
	m_pIGameLevelParser=(IGameLevelParser *)pGlobalUnits->QueryGlobalModule(MODULE_GAME_LEVEL_PARSER,IID_IGameLevelParser,VER_IGameLevelParser);

	return true;
}

//�л�״̬
VOID CGameFrameView::SwitchToReadyStatus()
{
	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//���ý���
	ResetGameView();

	//�û�����
	ZeroMemory(m_wUserClock,sizeof(m_wUserClock));
	ZeroMemory(m_pIClientUserItem,sizeof(m_pIClientUserItem));

	//���½���
	InvalidGameView(0,0,rcClient.Width(),rcClient.Height());

	return;
}

//��ȡʱ��
WORD CGameFrameView::GetUserClock(WORD wChairID)
{
	//Ч�����
	ASSERT(wChairID<MAX_CHAIR);
	if (wChairID>=MAX_CHAIR) return 0;

	//��ȡʱ��
	return m_wUserClock[wChairID];
}

//����ʱ��
VOID CGameFrameView::SetUserClock(WORD wChairID, WORD wUserClock)
{
	//���ñ���
	if (wChairID==INVALID_CHAIR)
	{
		for (WORD i=0;i<MAX_CHAIR;i++)
		{
			m_wUserClock[i]=wUserClock;
		}
	}
	else
	{
		m_wUserClock[wChairID]=wUserClock;
	}

	//���½���
	CRect rcClient;
	GetClientRect(&rcClient);
	InvalidGameView(0,0,rcClient.Width(),rcClient.Height());

	return;
}

//��ȡ�û�
IClientUserItem * CGameFrameView::GetClientUserItem(WORD wChairID)
{
	//Ч�����
	ASSERT(wChairID<MAX_CHAIR);
	if (wChairID>=MAX_CHAIR) return NULL;

	//��ȡ�û�
	return m_pIClientUserItem[wChairID];
}

//�����û�
VOID CGameFrameView::SetClientUserItem(WORD wChairID, IClientUserItem * pIClientUserItem)
{
	//Ч�����
	ASSERT(wChairID<CountArray(m_pIClientUserItem));
	if (wChairID>=CountArray(m_pIClientUserItem)) return;

	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//���ñ���
	m_pIClientUserItem[wChairID]=pIClientUserItem;

	//���½���
	InvalidGameView(0,0,rcClient.Width(),rcClient.Height());

	return;
}

//��������
bool CGameFrameView::PlayGameSound(LPCTSTR pszSoundName)
{
	//��������
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
	if (pGlobalUnits!=NULL) pGlobalUnits->PlayGameSound(pszSoundName);

	return true;
}

//��������
bool CGameFrameView::PlayGameSound(HINSTANCE hInstance, LPCTSTR pszSoundName)
{
	//��������
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
	if (pGlobalUnits!=NULL) pGlobalUnits->PlayGameSound(hInstance,pszSoundName);

	return true;
}

//��ȡ�ȼ�
LPCTSTR CGameFrameView::GetLevelDescribe(IClientUserItem * pIClientUserItem)
{
	//Ч��״̬
	ASSERT(m_pIGameLevelParser!=NULL);
	if (m_pIGameLevelParser==NULL) return TEXT("");
	
	//��ȡ�ȼ�
	return m_pIGameLevelParser->GetLevelDescribe(pIClientUserItem);
}

//������Ϣ
LRESULT CGameFrameView::SendEngineMessage(UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	//��������
	ASSERT(CGameFrameEngine::GetInstance()!=NULL);
	CGameFrameEngine * pGameFrameEngine=CGameFrameEngine::GetInstance();

	//����Ч��
	ASSERT(IsWindow(pGameFrameEngine->GetSafeHwnd()));
	if (IsWindow(pGameFrameEngine->GetSafeHwnd())==FALSE) return 0L;

	//������Ϣ
	return pGameFrameEngine->SendMessage(uMessage,wParam,lParam);
}

//Ͷ����Ϣ
LRESULT CGameFrameView::PostEngineMessage(UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	//��������
	ASSERT(CGameFrameEngine::GetInstance()!=NULL);
	CGameFrameEngine * pGameFrameEngine=CGameFrameEngine::GetInstance();

	//����Ч��
	ASSERT(IsWindow(pGameFrameEngine->GetSafeHwnd()));
	if (IsWindow(pGameFrameEngine->GetSafeHwnd())==FALSE) return 0L;

	//������Ϣ
	return pGameFrameEngine->PostMessage(uMessage,wParam,lParam);
}

//λ�ñ仯
VOID CGameFrameView::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType, cx, cy);
	
	//�����ؼ�
	RectifyControl(cx,cy);

	//���½���
	InvalidGameView(0,0,cx,cy);

	return;
}

//������Ϣ
INT CGameFrameView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//���ñ���
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);

	//��������
	ASSERT(CGlobalUnits::GetInstance()!=NULL);
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();

	//���ýӿ�
	m_pIGameLevelParser=pGlobalUnits->m_GameLevelParserModule.GetInterface();

	return 0;
}

//�����Ϣ
VOID CGameFrameView::OnRButtonUp(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags, Point);

	//��ʾ�˵�
	for (WORD i=0;i<MAX_CHAIR;i++)
	{
		if (m_pIClientUserItem[i]!=NULL)
		{
			//λ���ж�
			if (Point.x<=m_ptAvatar[i].x) continue;
			if (Point.y<=m_ptAvatar[i].y) continue;
			if (Point.x>=(m_ptAvatar[i].x+FACE_CX)) continue;
			if (Point.y>=(m_ptAvatar[i].y+FACE_CY)) continue;

			//��ѯ�ӿ�
			/*IGameFrameControl * pIGameFrameControl=NULL;
			pIGameFrameControl=(IGameFrameControl *)g_GlobalUnits.QueryGlobalModule(MODULE_GAME_FRAME_CONTROL,IID_IGameFrameControl,VER_IGameFrameControl);

			//��ʾ�˵�
			ASSERT(pIGameFrameControl!=NULL);
			if (pIGameFrameControl!=NULL) pIGameFrameControl->TrackUserItemMenu(m_pIClientUserItem[i]);*/

			break;
		}
	}

	return;
}

//�����Ϣ
VOID CGameFrameView::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags, Point);

	//���ý���
	SetFocus();

	return;
}

//////////////////////////////////////////////////////////////////////////////////
