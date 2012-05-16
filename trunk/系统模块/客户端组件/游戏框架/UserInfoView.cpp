#include "StdAfx.h"
#include "resource.h"
#include "GlobalOption.h"
#include "UserInfoView.h"
#include "GameFrameControl.h"
///////////////////////////////////////////////////////////////////////////

//�궨��
#define COLOR_VIEW_BACKGROUND						RGB(173,227,255)								//������ɫ
#define COLOR_PHOTO_FRAM							RGB(3,148,227)									//�߿���ɫ

#define PHOTO_FRAME_WIDTH							103												//ͼƬ��С
#define PHOTO_FRAME_HEIGHT							103												//ͼƬ��С
#define PHOTO_FRAME_EXCURSION_X						5												//ͼƬƫ��
#define PHOTO_FRAME_EXCURSION_Y						5												//ͼƬƫ��

#define IDC_EXCHANGE								101												//�һ���ť


//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CUserInfoView, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_EXCHANGE, OnBtnClickedExchange)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
//���캯��
CUserInfoView::CUserInfoView()
{
	//���ñ���
	m_pIUserFaceRes=NULL;
	m_pIClientKernel=NULL;
	m_pCurrentUserData=NULL;

	return;
}

//��������
CUserInfoView::~CUserInfoView()
{
}

//���ýӿ�
bool CUserInfoView::SetUserFaceRes(IUnknownEx * pIUnknownEx)
{
	ASSERT(pIUnknownEx!=NULL);
	m_pIUserFaceRes=(IUserFaceRes *)pIUnknownEx->QueryInterface(IID_IUserFaceRes,VER_IUserFaceRes);
	return (m_pIUserFaceRes!=NULL);
}

//������Դ
bool CUserInfoView::UpdateSkinResource()
{
	//��ȡ��Դ
	HINSTANCE hInstance=g_GlobalOption.m_PlatformResourceModule->GetResInstance();
	tagGameFrameImage &GameFrameImage=g_GlobalOption.m_GameFrameImage;

	m_ImageMaleFace.SetLoadInfo(GameFrameImage.uMaleFace, hInstance, false);
	m_ImageFemaleFace.SetLoadInfo(GameFrameImage.uFemaleFace, hInstance, false);
	m_btExchange.SetButtonImage(GameFrameImage.uBtExchangeCharm, hInstance, false);

	return true;
}

//�����û�
VOID CUserInfoView::SetUserViewInfo(tagUserData const* pCurrentUserData)
{
	//�����û�
	if (m_pCurrentUserData!=pCurrentUserData)
	{
		//���ñ���
		m_pCurrentUserData=pCurrentUserData;

		//���½���
		if (m_hWnd!=NULL) Invalidate(FALSE);
	}

	//���¿ؼ�
	UpdateControls();

	return;
}

//�滭����
VOID CUserInfoView::OnPaint()
{
	CPaintDC dc(this);

	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//��������
	CDC BufferDC;
	CBitmap ImageBuffer;
	BufferDC.CreateCompatibleDC(&dc);
	ImageBuffer.CreateCompatibleBitmap(&dc,rcClient.Width(),rcClient.Height());

	//���� DC
	BufferDC.SetBkMode(TRANSPARENT);
	BufferDC.SelectObject(&ImageBuffer);
	BufferDC.SetTextColor(RGB(0,0,0));

	//��䱳��
	BufferDC.FillSolidRect(&rcClient,COLOR_VIEW_BACKGROUND);

	//�滭�߿�
	CRect rcPhotoFram(PHOTO_FRAME_EXCURSION_X, PHOTO_FRAME_EXCURSION_Y, PHOTO_FRAME_EXCURSION_X+PHOTO_FRAME_WIDTH, PHOTO_FRAME_EXCURSION_Y+PHOTO_FRAME_HEIGHT);
	CPen BorderPen(PS_SOLID,2,COLOR_PHOTO_FRAM);
	CPen * pOldPen=BufferDC.SelectObject(&BorderPen);
	BufferDC.RoundRect(&rcPhotoFram,CPoint(0,00));
	BufferDC.SelectObject(pOldPen);

	//��������
	CFont Font;
	VERIFY(Font.CreateFont(12,0,0,0,FW_NORMAL,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH | FF_SWISS,TEXT("����")));             

	CFont *pOldFont = BufferDC.SelectObject(&Font);

	//�滭�û�
	if (m_pCurrentUserData!=NULL)
	{
		//��������
		TCHAR szString[256]=TEXT("");
		tagServerAttribute const* pGameAttribute=m_pIClientKernel->GetServerAttribute();

		//�滭��ʶ
		if ( m_pCurrentUserData->dwCustomFaceVer == 0 )
		{
			//���ô�С
			rcPhotoFram.DeflateRect(1, 1, 2, 2);

			if ( m_pCurrentUserData->cbGender != 2 )
			{
				CImageHandle ImageHandleMaleFace(&m_ImageMaleFace);
				m_ImageMaleFace.BitBlt(BufferDC.GetSafeHdc(), rcPhotoFram.left, rcPhotoFram.top);
			}
			else
			{
				CImageHandle ImageHandleFemaleFace(&m_ImageFemaleFace);
				m_ImageFemaleFace.BitBlt(BufferDC.GetSafeHdc(), rcPhotoFram.left, rcPhotoFram.top);
			}
		}
		//�û�ͷ��
		else
		{
			ASSERT(m_pIUserFaceRes!=NULL);
			m_pIUserFaceRes->DrawCustomBigFace(&BufferDC,rcPhotoFram.left+1, rcPhotoFram.top+1,m_pCurrentUserData->dwUserID,m_pCurrentUserData->dwCustomFaceVer);
		}

		//�û�����
		INT nUserInfoCuont = 6;
		const tagUserData *pMeUserData = m_pIClientKernel->GetMeUserInfo();
		if(pMeUserData!=NULL && m_pCurrentUserData->dwUserID==pMeUserData->dwUserID)
			nUserInfoCuont++;
		for (INT i=0;i<nUserInfoCuont;i++)
		{
			//λ�ö���
			CRect rcString;
			rcString.SetRect(PHOTO_FRAME_EXCURSION_X+PHOTO_FRAME_WIDTH+10,6+i*20,rcClient.Width()-5,6+i*20+15);

			TCHAR szString[128];

			//�滭��Ϣ
			switch (i)
			{
			case 0:		//�û��ʺ�
				{
					//�û��ʺ�
					_sntprintf(szString,CountArray(szString),TEXT("�û�����%s"),m_pCurrentUserData->szName);
					BufferDC.DrawText(szString,lstrlen(szString),&rcString,DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

					break;
				}
			case 1:
				{
					//�����Ϣ
					if ( m_pCurrentUserData->cbGender == 0 ) 
						_sntprintf(szString,CountArray(szString),TEXT("�Ա𣺱���"));
					else if ( m_pCurrentUserData->cbGender == 1 ) 
						_sntprintf(szString,CountArray(szString),TEXT("�Ա���"));
					else _sntprintf(szString,CountArray(szString),TEXT("�Ա�Ů"));
					BufferDC.DrawText(szString,lstrlen(szString),&rcString,DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

					break;
				}
			case 2:
				{
					BufferDC.SetTextColor(RGB(255,0,0));
					//�����Ϣ
					_sntprintf(szString,CountArray(szString),TEXT("������%ld"),m_pCurrentUserData->lLoveliness);
					BufferDC.DrawText(szString,lstrlen(szString),&rcString,DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

					BufferDC.SetTextColor(RGB(0,0,0));

					break;
				}
			case 3:
				{
					//�����Ϣ
					if ( pGameAttribute->wGameGenre == GAME_GENRE_GOLD )
						_sntprintf(szString,CountArray(szString),TEXT("��Ϸ�ң�%ld"),m_pCurrentUserData->lScore);
					else if (pGameAttribute->wGameGenre == GAME_GENRE_EDUCATE)
						_sntprintf(szString,CountArray(szString),TEXT("��ϰ�ң�%ld"),m_pCurrentUserData->lScore);
					else if (pGameAttribute->wGameGenre == GAME_GENRE_MATCH)
						_sntprintf(szString,CountArray(szString),TEXT("���ң�%ld"),m_pCurrentUserData->lScore);
					else
						_sntprintf(szString,CountArray(szString),TEXT("���֣�%ld"),m_pCurrentUserData->lScore);
					BufferDC.DrawText(szString,lstrlen(szString),&rcString,DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

					break;
				}
			case 4:
				{
					CGameFrameControl *pGameFrameControl= (CGameFrameControl *)GetParent();
					CUserListView &UserListView = pGameFrameControl->m_UserListView;

					//�����Ϣ
					_sntprintf(szString,CountArray(szString),TEXT("��Ϸ����%s"),
						UserListView.m_GameRankManagerHelper->GetLevelDescribe(m_pCurrentUserData->lScore));
					BufferDC.DrawText(szString,lstrlen(szString),&rcString,DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

					break;
				}
			case 5:
				{
					CGameFrameControl *pGameFrameControl= (CGameFrameControl *)GetParent();
					CUserListView &UserListView = pGameFrameControl->m_UserListView;

					//��Ա��Ϣ
					LPCTSTR pszMemberOrder[]={TEXT("�ǻ�Ա"),TEXT("�����Ա"),TEXT("�����Ա"),TEXT("�����Ա"),TEXT("�����Ա")};
					if (m_pCurrentUserData->cbMemberOrder>=CountArray(pszMemberOrder))break;

					//�����Ϣ
					_sntprintf(szString,CountArray(szString),TEXT("��Ա����%s"),pszMemberOrder[m_pCurrentUserData->cbMemberOrder]);
					BufferDC.DrawText(szString,lstrlen(szString),&rcString,DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

					break;
				}
			case 6:
				{
					//�����Ϣ
					_sntprintf(szString,CountArray(szString),TEXT("���д�%ld"),m_pCurrentUserData->lInsureScore);
					BufferDC.DrawText(szString,lstrlen(szString),&rcString,DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

					break;
				}
			}
		}

		//������Ŀ
		BufferDC.SetTextColor(RGB(255,0,0));
		CRect rcExchange;
		m_btExchange.GetWindowRect(rcExchange);
		ScreenToClient(rcExchange);
		CString strFlowerCount;
		strFlowerCount.Format(TEXT("%ld"), m_pCurrentUserData->lLoveliness);
		BufferDC.TextOut(rcExchange.right+4, rcExchange.top+4, strFlowerCount);
	}

	//�滭����
	dc.BitBlt(0,0,rcClient.Width(),rcClient.Height(),&BufferDC,0,0,SRCCOPY);

	//������Դ	
	BufferDC.SelectObject(pOldFont);
	BufferDC.DeleteDC();
	ImageBuffer.DeleteObject();	
	Font.DeleteObject();

	return;
}

//������Ϣ
INT CUserInfoView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	CRect rcCreate(0,0,0,0);
	m_btExchange.Create(NULL, WS_CHILD|WS_VISIBLE, rcCreate, this, IDC_EXCHANGE);
	m_btExchange.EnableWindow(FALSE);

	//������Դ
	UpdateSkinResource();

	return 0;
}

//�����ں�
void CUserInfoView::SetClientKernel(IClientKernel *pIClientKernel)
{
	m_pIClientKernel = pIClientKernel;
}

void CUserInfoView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	RectifyControl(cx, cy);
}

//��������
void CUserInfoView::RectifyControl(int nWidth, int nHeight)
{
	//����λ��
	HDWP hDwp = BeginDeferWindowPos(20);

	UINT uFlags = SWP_NOCOPYBITS | SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOZORDER;

	CRect rcButton;
	m_btExchange.GetWindowRect(rcButton);

	int nBtPhotoPosX = PHOTO_FRAME_EXCURSION_X + (PHOTO_FRAME_WIDTH-rcButton.Width()) / 2;
	DeferWindowPos(hDwp, m_btExchange, NULL,  10, nHeight- rcButton.Height() - 5, 0, 0, uFlags);

	EndDeferWindowPos(hDwp);

	return;
}

//�һ���Ϣ
void CUserInfoView::OnBtnClickedExchange()
{
	//�һ�����
	const tagUserData *pMeUserData = m_pIClientKernel->GetMeUserInfo();
	ShowCharmValueExchangeDlg(m_pIClientKernel,NULL,(tagUserData *)pMeUserData);

	return;
}

//���º���
void CUserInfoView::UpdateUserInfo()
{
	UpdateControls();
	InvalidateRect(NULL);
}

//���¿ؼ�
void CUserInfoView::UpdateControls()
{
	if ( m_pCurrentUserData == NULL ) return ;

	//�Լ��ж�
	tagUserData const *pMeUserData = m_pIClientKernel->GetMeUserInfo();

	if ( pMeUserData->dwUserID == m_pCurrentUserData->dwUserID && 1 <= pMeUserData->lLoveliness )
		m_btExchange.EnableWindow(TRUE);
	else
		m_btExchange.EnableWindow(FALSE);
}

//��ȡ����
tagUserData const *CUserInfoView::GetCurrentUser()
{
	return m_pCurrentUserData;
}

///////////////////////////////////////////////////////////////////////////