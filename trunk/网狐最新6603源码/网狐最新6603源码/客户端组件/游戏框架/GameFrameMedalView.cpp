#include "StdAfx.h"
#include "Resource.h"
#include "GameFrameMedalView.h"

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameFrameMedalView, CWnd)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGameFrameMedalView::CGameFrameMedalView()
{
	//���ñ���
	m_dwUserMedal=0L;

	return;
}

//��������
CGameFrameMedalView::~CGameFrameMedalView()
{
}

//���ý���
VOID CGameFrameMedalView::SetUserMedal(DWORD dwUserMedal)
{
	//���ñ���
	if (m_dwUserMedal!=dwUserMedal)
	{
		//���ñ���
		m_dwUserMedal=dwUserMedal;

		//���½���
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW);
	}
}

//�滭����
BOOL CGameFrameMedalView::OnEraseBkgnd(CDC * pDC)
{
	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//��������
	CImage ImageBuffer;
	ImageBuffer.Create(rcClient.Width(),rcClient.Height(),32);

	//���� DC
	CImageDC BufferDC(ImageBuffer);
	CDC * pBufferDC=CDC::FromHandle(BufferDC);

	//���û���
	pBufferDC->SetBkMode(TRANSPARENT);
	pBufferDC->SetTextAlign(TA_LEFT|TA_TOP);
	pBufferDC->SetTextColor(RGB(250,250,250));
	pBufferDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

	//������Դ
	CImage ImageUserMedalL;
	CImage ImageUserMedalM;
	CImage ImageUserMedalR;
	ImageUserMedalL.LoadFromResource(GetModuleHandle(GAME_FRAME_DLL_NAME),IDB_USER_MEDAL_L);
	ImageUserMedalM.LoadFromResource(GetModuleHandle(GAME_FRAME_DLL_NAME),IDB_USER_MEDAL_M);
	ImageUserMedalR.LoadFromResource(GetModuleHandle(GAME_FRAME_DLL_NAME),IDB_USER_MEDAL_R);

	//��������
	INT nXStartPos=ImageUserMedalL.GetWidth();
	INT nXConcludePos=rcClient.Width()-ImageUserMedalR.GetWidth();

	//�滭���
	ImageUserMedalL.BitBlt(BufferDC,0,0);
	ImageUserMedalR.BitBlt(BufferDC,rcClient.Width()-ImageUserMedalR.GetWidth(),0);

	//�滭�м�
	for (INT nXPos=nXStartPos;nXPos<nXConcludePos;nXPos+=ImageUserMedalM.GetWidth())
	{
		INT nCXDraw=__min(ImageUserMedalM.GetWidth(),nXConcludePos-nXPos);
		ImageUserMedalM.BitBlt(BufferDC,nXPos,0,nCXDraw,ImageUserMedalM.GetHeight(),0,0);
	}

	//������Դ
	CPngImage ImageCount;
	CPngImage ImageStringL;
	CPngImage ImageStringR;
	ImageCount.LoadImage(GetModuleHandle(GAME_FRAME_DLL_NAME),TEXT("MEDAL_COUNT"));
	ImageStringL.LoadImage(GetModuleHandle(GAME_FRAME_DLL_NAME),TEXT("MEDAL_STRING_L"));
	ImageStringR.LoadImage(GetModuleHandle(GAME_FRAME_DLL_NAME),TEXT("MEDAL_STRING_R"));

	//��ȡ��С
	CSize SizeCount;
	CSize SizeStringL;
	CSize SizeStringR;
	SizeCount.SetSize(ImageCount.GetWidth()/10L,ImageCount.GetHeight());
	SizeStringL.SetSize(ImageStringL.GetWidth(),ImageStringL.GetHeight());
	SizeStringR.SetSize(ImageStringR.GetWidth(),ImageStringR.GetHeight());

	//���콱��
	TCHAR szUserMedal[16]=TEXT("");
	_sntprintf(szUserMedal,CountArray(szUserMedal),TEXT("%ld"),m_dwUserMedal);

	//��������
	INT nNumCount=lstrlen(szUserMedal);
	INT nNumStart=(rcClient.Width()-nNumCount*SizeCount.cx)/2+10;

	//�滭�ַ�
	ImageStringL.DrawImage(pBufferDC,nNumStart-SizeStringL.cx-3,4);
	ImageStringR.DrawImage(pBufferDC,nNumStart+nNumCount*SizeCount.cx+3,4);

	//�滭����
	for (INT i=0;i<nNumCount;i++)
	{
		INT nXImage=(szUserMedal[i]-TEXT('0'))*SizeCount.cx;
		ImageCount.DrawImage(pBufferDC,nNumStart+i*SizeCount.cx,7,SizeCount.cx,SizeCount.cy,nXImage,0);
	}

	//�滭����
	pDC->BitBlt(0,0,rcClient.Width(),rcClient.Height(),pBufferDC,0,0,SRCCOPY);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////
