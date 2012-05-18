#include "StdAfx.h"
#include "GameClient.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////
//��ť��ʶ

//���ư�ť
#define IDC_START					100									//��ʼ��ť
#define IDC_REGRET					101									//���尴ť
#define IDC_PEACE					102									//��Ͱ�ť
#define IDC_GIVEUP					103									//���䰴ť
#define IDC_PRESERVE				104									//���水ť
#define IDC_MANUAL					105									//���װ�ť

//���װ�ť
#define IDC_MANUAL_BT_HEAD			200									//���װ�ť
#define IDC_MANUAL_BT_LAST			201									//���װ�ť
#define IDC_MANUAL_BT_OPEN			202									//���װ�ť
#define IDC_MANUAL_BT_NEXT			203									//���װ�ť
#define IDC_MANUAL_BT_TAIL			204									//���װ�ť

//�ؼ���ʶ
#define IDC_MANUAL_LIST				300									//���ױ�ʶ

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameView)

	//ϵͳ��Ϣ
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()

	//���ư�ť
	ON_BN_CLICKED(IDC_START, OnBnClickedStart)
	ON_BN_CLICKED(IDC_PEACE, OnBnClickedPeace)
	ON_BN_CLICKED(IDC_REGRET, OnBnClickedRegret)
	ON_BN_CLICKED(IDC_GIVEUP, OnBnClickedGiveUp)
	ON_BN_CLICKED(IDC_MANUAL, OnBnClickedManual)
	ON_BN_CLICKED(IDC_PRESERVE, OnBnClickedPreserve)

	//���װ�ť
	ON_BN_CLICKED(IDC_MANUAL_BT_HEAD, OnBnClickedHead)
	ON_BN_CLICKED(IDC_MANUAL_BT_LAST, OnBnClickedLast)
	ON_BN_CLICKED(IDC_MANUAL_BT_OPEN, OnBnClickedOpen)
	ON_BN_CLICKED(IDC_MANUAL_BT_NEXT, OnBnClickedNext)
	ON_BN_CLICKED(IDC_MANUAL_BT_TAIL, OnBnClickedTail)

	//�ؼ���Ϣ
	ON_LBN_SELCHANGE(IDC_MANUAL_LIST, OnLbnSelchangeManualList)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientView::CGameClientView()
{
	//��Ϸ����
	m_nFrameWidth = 0;
	m_nFrameHeight = 0;
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	ZeroMemory(&m_RuleStatusInfo,sizeof(m_RuleStatusInfo));
	ZeroMemory(&m_UserStatusInfo,sizeof(m_UserStatusInfo));

	//��������
	m_ChessBorad.SetChessBoradMode((GetSystemMetrics(SM_CXSCREEN)<=800));

	//�����Դ
	HINSTANCE hResInstance=AfxGetInstanceHandle();
	m_EncircleImageViewBack.ImageML.SetLoadInfo(IDB_VIEW_BACK_ML,hResInstance);
	m_EncircleImageViewBack.ImageMR.SetLoadInfo(IDB_VIEW_BACK_MR,hResInstance);
	m_EncircleImageViewBack.ImageTL.SetLoadInfo(IDB_VIEW_BACK_TL,hResInstance);
	m_EncircleImageViewBack.ImageTM.SetLoadInfo(IDB_VIEW_BACK_TM,hResInstance);
	m_EncircleImageViewBack.ImageTR.SetLoadInfo(IDB_VIEW_BACK_TR,hResInstance);
	m_EncircleImageViewBack.ImageBL.SetLoadInfo(IDB_VIEW_BACK_BL,hResInstance);
	m_EncircleImageViewBack.ImageBM.SetLoadInfo(IDB_VIEW_BACK_BM,hResInstance);
	m_EncircleImageViewBack.ImageBR.SetLoadInfo(IDB_VIEW_BACK_BR,hResInstance);

	//������Դ
	m_ImageViewFill.SetLoadInfo(IDB_VIEW_FILL,hResInstance);
	m_ImageClockNumber.SetLoadInfo(IDB_CLOCK_NUMBER,hResInstance);
	m_ImageControlBack.SetLoadInfo(IDB_CONTROL_BACK,hResInstance);
	m_ImageUserInfoFlag.SetLoadInfo(IDB_USER_INFO_FLAG,hResInstance);
	m_ImageUserInfoView.SetLoadInfo(IDB_USER_INFO_VIEW,hResInstance);
	m_ImageChessManualT.SetLoadInfo(IDB_CHESS_MANUAL_T,hResInstance);
	m_ImageChessManualM.SetLoadInfo(IDB_CHESS_MANUAL_M,hResInstance);
	m_ImageChessManualB.SetLoadInfo(IDB_CHESS_MANUAL_B,hResInstance);

	//��ȡ��С
	CImageHandle HandleControlBack(&m_ImageControlBack);
	CImageHandle HandleUserViewInfo(&m_ImageUserInfoView);
	CImageHandle HandleChessManualM(&m_ImageChessManualM);

	//��ȡ��С
	m_nManualWidth=m_ImageChessManualM.GetWidth();
	m_ChessBorad.GetControlSize(m_SizeChessBorad);
	m_SizeControlView.SetSize(m_ImageControlBack.GetWidth(),m_ImageControlBack.GetHeight());
	m_SizeUserInfoView.SetSize(m_ImageUserInfoView.GetWidth(),m_ImageUserInfoView.GetHeight());

	//��ܴ�С
	//CSkinAide::GetEncircleInfo(m_EncircleInfoViewBack,m_EncircleImageViewBack);

	return;
}

//��������
CGameClientView::~CGameClientView()
{
}

//��Ϣ����
BOOL CGameClientView::PreTranslateMessage(MSG * pMsg)
{
	//��ʾ��Ϣ
	if (m_ToolTipCtrl.m_hWnd!=NULL) 
	{
		m_ToolTipCtrl.RelayEvent(pMsg);
	}

	return __super::PreTranslateMessage(pMsg);
}

//���ý���
void CGameClientView::ResetGameView()
{
	//__super::ResetGameView();

	//��Ϸ����
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	ZeroMemory(&m_RuleStatusInfo,sizeof(m_RuleStatusInfo));
	ZeroMemory(&m_UserStatusInfo,sizeof(m_UserStatusInfo));

	//���ư�ť
	m_btStart.EnableWindow(FALSE);
	m_btPeace.EnableWindow(FALSE);
	m_btRegret.EnableWindow(FALSE);
	m_btGiveUp.EnableWindow(FALSE);
	m_btPreserve.EnableWindow(FALSE);

	//���װ�ť
	m_btManualHead.EnableWindow(FALSE);
	m_btManualLast.EnableWindow(FALSE);
	m_btManualNext.EnableWindow(FALSE);
	m_btManualTail.EnableWindow(FALSE);

	//��Ϸ�ؼ�
	m_ManualList.ResetContent();
	m_ScoreControl.RestorationData();

	//�رմ���
	if (m_DlgRuleOption.m_hWnd!=NULL) m_DlgRuleOption.DestroyWindow();
	if (m_UserRequestPeace.m_hWnd!=NULL) m_UserRequestPeace.DestroyWindow();
	if (m_UserRequestRegret.m_hWnd!=NULL) m_UserRequestRegret.DestroyWindow();
	if (m_UserHint.m_hWnd!=NULL)m_UserHint.DestroyWindow();

	//��������
	m_ChessBorad.SetPositively(false);
	m_ChessBorad.SetManualView(false);
	m_ChessBorad.SetWaitGameRule(false);
	m_ChessBorad.SelectChessItem(INVALID_POS,INVALID_POS);
	m_ChessBorad.SetMoveChessTrace(INVALID_POS,INVALID_POS,INVALID_POS,INVALID_POS);

	return ;
}

//�����ؼ�
VOID CGameClientView::RectifyGameView(INT nWidth, INT nHeight)
{
	if(m_nFrameWidth<400) m_nFrameWidth = nWidth;
	if(m_nFrameHeight<200) m_nFrameHeight = nHeight;

	//��������.
	m_EncircleInfoViewBack.nBBorder = 13;
	m_EncircleInfoViewBack.nLBorder = 176;
	m_EncircleInfoViewBack.nRBorder = 46;
	m_EncircleInfoViewBack.nTBorder = 14;
	INT nXBorderArea=m_EncircleInfoViewBack.nLBorder+m_EncircleInfoViewBack.nRBorder;
	INT nYBorderArea=m_EncircleInfoViewBack.nTBorder+m_EncircleInfoViewBack.nBBorder;
	bool bChessManual=((nWidth-m_EncircleInfoViewBack.nLBorder-m_EncircleInfoViewBack.nRBorder)>(m_SizeChessBorad.cx+m_nManualWidth));

	//����λ��
	INT nXChessBorad=nWidth-m_SizeChessBorad.cx-25;
	if(nWidth>m_nFrameWidth)nXChessBorad = m_nFrameWidth-m_SizeChessBorad.cx-25;
	INT nYChessBorad=nHeight/2-332;
	m_ChessBorad.SetBenchmarkPos(nXChessBorad,nYChessBorad);

	//����λ��
	m_ptControlView.y=nHeight-m_EncircleInfoViewBack.nBBorder-m_SizeControlView.cy;
	m_ptControlView.x=nXChessBorad;

	//�û�λ��
	m_ptUserInfoView.x=-5+m_nXBorder;
	m_ptUserInfoView.y=m_EncircleInfoViewBack.nTBorder+nYChessBorad+(m_SizeChessBorad.cy-m_SizeUserInfoView.cy)/2;

#ifdef VIDEO_GAME
	m_ptUserInfoView.y=m_EncircleInfoViewBack.nTBorder+nYChessBorad+(m_SizeChessBorad.cy-m_SizeUserInfoView.cy)/2+53;
#endif

	//����λ��
	if ((bChessManual==true)&&(nWidth>m_nFrameWidth))
	{
		m_rcChessManual.top=m_EncircleInfoViewBack.nTBorder+10;
		m_rcChessManual.bottom=nHeight-m_EncircleInfoViewBack.nBBorder+10;
		m_rcChessManual.left=nWidth-nWidth/2+m_nFrameWidth/2-m_nManualWidth/2-m_EncircleInfoViewBack.nRBorder+35;
		m_rcChessManual.right=nWidth-nWidth/2+m_nFrameWidth/2+m_nManualWidth/2-m_EncircleInfoViewBack.nRBorder+35;
	}
	else m_rcChessManual.SetRect(0,0,0,0);

	//ͷ��λ��
	m_ptFace[0].x = m_ptUserInfoView.x+24;
	m_ptFace[0].y = m_ptUserInfoView.y+22;
	m_ptFace[1].x =	m_ptUserInfoView.x+24;
	m_ptFace[1].y = m_ptUserInfoView.y+312;

	//�ʺ�λ��
	m_ptName[0].x = m_ptUserInfoView.x+70;
	m_ptName[0].y = m_ptUserInfoView.y+86;
	m_ptName[1].x =	m_ptUserInfoView.x+70;
	m_ptName[1].y = m_ptUserInfoView.y+377;

	//׼��λ��
	m_ptReady[0].x = 220;
	m_ptReady[0].y = nHeight/2-50;
	m_ptReady[1].x = nWidth/2;
	m_ptReady[1].y = nHeight-220;

	//���ư�ť
	CRect rcButton;
	m_btStart.GetWindowRect(&rcButton);

	//���װ�ť
	CRect rcManualOpen;
	CRect rcManualControl;
	m_btManualOpen.GetWindowRect(&rcManualOpen);
	m_btManualHead.GetWindowRect(&rcManualControl);

	//λ�ü���
	INT nXControlBorad=30;
	INT nYControlExcursion=12;
	INT nXButton=m_ptControlView.x+nXControlBorad;
	INT nYButton=m_ptControlView.y+nYControlExcursion;
	INT nXButtonSpace=(m_SizeControlView.cx-rcButton.Width()*6-2*nXControlBorad)/7;

	//λ�ü���
	INT nYManualPos=m_rcChessManual.bottom-65;
	INT nXManualPos=m_rcChessManual.left+m_rcChessManual.Width()/2-rcManualOpen.Width()/2-rcManualControl.Width()*2;

	//�ƶ��ؼ�
	HDWP hDwp=BeginDeferWindowPos(42);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;

	//���ư�ť
	DeferWindowPos(hDwp,m_btStart,NULL,nXButton+nXButtonSpace,nYButton,0,0,uFlags);
	DeferWindowPos(hDwp,m_btRegret,NULL,nXButton+rcButton.Width()+nXButtonSpace*2,nYButton,0,0,uFlags);
	DeferWindowPos(hDwp,m_btPeace,NULL,nXButton+rcButton.Width()*2+nXButtonSpace*3,nYButton,0,0,uFlags);
	DeferWindowPos(hDwp,m_btGiveUp,NULL,nXButton+rcButton.Width()*3+nXButtonSpace*4,nYButton,0,0,uFlags);
	DeferWindowPos(hDwp,m_btPreserve,NULL,nXButton+rcButton.Width()*4+nXButtonSpace*5,nYButton,0,0,uFlags);
	DeferWindowPos(hDwp,m_btManual,NULL,nXButton+rcButton.Width()*5+nXButtonSpace*6,nYButton,0,0,uFlags);

	//���װ�ť
	DeferWindowPos(hDwp,m_btManualHead,NULL,nXManualPos,nYManualPos-rcManualControl.Height()/2,0,0,uFlags);
	DeferWindowPos(hDwp,m_btManualLast,NULL,nXManualPos+rcManualControl.Width(),nYManualPos-rcManualControl.Height()/2,0,0,uFlags);
	DeferWindowPos(hDwp,m_btManualOpen,NULL,nXManualPos+rcManualControl.Width()*2,nYManualPos-rcManualOpen.Height()/2,0,0,uFlags);
	DeferWindowPos(hDwp,m_btManualNext,NULL,nXManualPos+rcManualControl.Width()*2+rcManualOpen.Width(),nYManualPos-rcManualControl.Height()/2,0,0,uFlags);
	DeferWindowPos(hDwp,m_btManualTail,NULL,nXManualPos+rcManualControl.Width()*3+rcManualOpen.Width(),nYManualPos-rcManualControl.Height()/2,0,0,uFlags);

	//�ƶ�����
	DeferWindowPos(hDwp,m_ManualList,NULL,m_rcChessManual.left+24,m_rcChessManual.top+93,m_rcChessManual.Width()-45,
		m_rcChessManual.Height()-188,SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS);

	//�ƶ��ɼ�
	CRect rcScoreControl;
	m_ScoreControl.GetWindowRect(&rcScoreControl);
	DeferWindowPos(hDwp,m_ScoreControl,NULL,m_EncircleInfoViewBack.nLBorder+nXChessBorad+(m_SizeChessBorad.cx-rcScoreControl.Width())/2-150,(nHeight-rcScoreControl.Height())*2/5,0,0,uFlags);

	//�ƶ�����
	//CRect rcScoreControl1;
	//m_UserRequestRegret.GetWindowRect(&rcScoreControl1);
	//m_UserRequestRegret.MoveWindow(rcScoreControl1.left-80,rcScoreControl1.top,rcScoreControl1.Width(),rcScoreControl1.Height());

#ifdef VIDEO_GAME
	//��Ƶ����
	DeferWindowPos(hDwp,m_DlgVideoService[0],NULL,5,2,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_DlgVideoService[1],NULL,5,nHeight/2+270,0,0,uFlags|SWP_NOSIZE);
#endif

	//�����ƶ�
	EndDeferWindowPos(hDwp);

	//�����ؼ�	
	SetFlowerControlInfo(1,m_ptFace[1].x,m_ptFace[1].y
		,BIG_FACE_WIDTH, BIG_FACE_HEIGHT, 30);
	SetFlowerControlInfo(0,m_ptFace[0].x,m_ptFace[0].y
		,BIG_FACE_WIDTH, BIG_FACE_HEIGHT, 30);

	return;
}

//�滭����
VOID CGameClientView::DrawGameView(CDC * pDC, INT nWidth, INT nHeight)
{
	//�滭����
	CRect rcViewBack(0,0,nWidth,nHeight);
	DrawViewImage(pDC,m_ImageViewFill,enMode_Spread);
	//CSkinAide::DrawEncircleFrame(pDC,rcViewBack,m_EncircleImageViewBack);

	//�û���Ϣ
	CImageHandle HandleUserViewInfo(&m_ImageUserInfoView);
	CImageHandle HandleUserViewFlag(&m_ImageUserInfoFlag);
	m_ImageUserInfoView.BitBlt(pDC->m_hDC,m_ptUserInfoView.x,m_ptUserInfoView.y);

	//�滭�û�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��ȡ�û�
		const tagUserData * pUserData=GetUserInfo(i);

		//���Դ���
		//tagUserData Obj;
		//Obj.wFaceID = 5;
		//_sntprintf( Obj.szName,sizeof(Obj.szName),TEXT("�û�������"));
		//Obj.cbUserStatus = US_SIT;
		//Obj.lScore = 5693;
		//pUserData = &Obj;

		if (pUserData==NULL) continue;

		//�û�ʱ��
		WORD wTimes = GetUserTimer(i);

		//���λ��
		CRect rcAccounts(m_ptName[i].x,m_ptName[i].y,m_ptName[i].x+100,m_ptName[i].y+13);
		CRect rcLevelDescribe(m_ptName[i].x,m_ptName[i].y+20,m_ptName[i].x+100,m_ptName[i].y+13+20);

		//�û��ȼ�
		TCHAR pszLevelDescribe[LEN_ACCOUNTS] = TEXT("");
		GetLevelDescribe(pUserData->lScore,pszLevelDescribe);

		//�滭�û�
		pDC->SetTextColor(RGB(20,20,20));
		DrawUserFace(pDC,pUserData,m_ptFace[i].x+7,m_ptFace[i].y+7,pUserData->cbUserStatus==US_OFFLINE);
		if(wTimes>0)DrawUserTimer(pDC,m_ptFace[i].x+85,m_ptFace[i].y+28,wTimes,99L);
		pDC->DrawText(pUserData->szName,lstrlen(pUserData->szName),rcAccounts,DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER);
		pDC->DrawText(pszLevelDescribe,lstrlen(pszLevelDescribe),rcLevelDescribe,DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER);

		//�滭��־
		if ((m_wBankerUser!=INVALID_CHAIR)&&(pUserData->cbUserStatus!=US_SIT))
		{
			//��ȡ��С
			INT nImageWidht=m_ImageUserInfoFlag.GetWidth()/4;
			INT nImageHeight=m_ImageUserInfoFlag.GetHeight();

			//�û���־
			INT nXImagePos=(m_wBankerUser==i)?0:nImageWidht;
			if (m_wCurrentUser==i) nXImagePos+=2*nImageWidht;
			m_ImageUserInfoFlag.BitBlt(pDC->m_hDC,m_ptUserInfoView.x+77,m_ptUserInfoView.y+8+i*291,nImageWidht,nImageHeight,nXImagePos,0);
		}

		//��Ϸʱ��
		DrawUserClock(pDC,m_ptName[i].x,m_ptName[i].y+42,m_UserStatusInfo[i].wDrawTimeCount);
		DrawUserClock(pDC,m_ptName[i].x,m_ptName[i].y+62,m_UserStatusInfo[i].wStepTimeCount);
		DrawUserClock(pDC,m_ptName[i].x,m_ptName[i].y+82,m_UserStatusInfo[i].wSecondTimeCount);
	}

	//������Ϣ
	INT nXPos=m_ptUserInfoView.x+70;
	INT nYPos=m_ptUserInfoView.y+218;
	DrawUserClock(pDC,nXPos,nYPos,m_RuleStatusInfo.wDrawTimeCount);
	DrawUserClock(pDC,nXPos,nYPos+20,m_RuleStatusInfo.wStepTimeCount);
	DrawUserClock(pDC,nXPos,nYPos+40,m_RuleStatusInfo.wSecondTimeCount);

	//�滭����
	m_ChessBorad.DrawChessBorad(pDC);

	//���Ƶ�ͼ
	CImageHandle HandleControlBack(&m_ImageControlBack);
	m_ImageControlBack.BitBlt(pDC->m_hDC,m_ptControlView.x,m_ptControlView.y);

	//�滭����
	if (m_rcChessManual.Width()>0)
	{
		//������Դ
		CImageHandle HandleChessManualT(&m_ImageChessManualT);
		CImageHandle HandleChessManualM(&m_ImageChessManualM);
		CImageHandle HandleChessManualB(&m_ImageChessManualB);

		//����ͷβ
		m_ImageChessManualT.BitBlt(pDC->m_hDC,m_rcChessManual.left,m_rcChessManual.top);
		m_ImageChessManualB.BitBlt(pDC->m_hDC,m_rcChessManual.left,m_rcChessManual.bottom-m_ImageChessManualB.GetHeight());

		//���׵ײ�
		INT nYStartPos=m_rcChessManual.top+m_ImageChessManualT.GetHeight();
		INT nYConcludePos=m_rcChessManual.bottom-m_ImageChessManualB.GetHeight();
		for (INT nYPos=nYStartPos;nYPos<nYConcludePos;nYPos+=m_ImageChessManualM.GetHeight())
		{
			INT nDrawHeight=__min(m_ImageChessManualM.GetHeight(),nYConcludePos-nYPos);
			m_ImageChessManualM.BitBlt(pDC->m_hDC,m_rcChessManual.left,nYPos,m_nManualWidth,nDrawHeight,0,0);
		}
	}

	return;
}

//ׯ���û�
bool CGameClientView::SetBankerUser(WORD wBankerUser)
{
	if (m_wBankerUser!=wBankerUser)
	{
		//���ñ���
		m_wBankerUser=wBankerUser;

		//���½���
		UpdateGameView(NULL);
	}

	return true;
}

//��ǰ�û�
bool CGameClientView::SetCurrentUser(WORD wCurrentUser)
{
	if (m_wCurrentUser!=wCurrentUser)
	{
		//���ñ���
		m_wCurrentUser=wCurrentUser;

		//���½���
		UpdateGameView(NULL);
	}

	return true;
}

//���ù���
bool CGameClientView::SetGameRuleInfo(const tagGameRuleInfo & GameRuleInfo)
{
	//���ñ���
	m_RuleStatusInfo.wDrawTimeCount=GameRuleInfo.wRuleDrawTime;
	m_RuleStatusInfo.wStepTimeCount=GameRuleInfo.wRuleStepTime;
	m_RuleStatusInfo.wSecondTimeCount=GameRuleInfo.wRuleSecondTime;

	//����λ��
	CRect rcUserInfoView;
	rcUserInfoView.top=m_ptUserInfoView.y;
	rcUserInfoView.left=m_ptUserInfoView.x;
	rcUserInfoView.right=rcUserInfoView.left+m_SizeUserInfoView.cx;
	rcUserInfoView.bottom=rcUserInfoView.top+m_SizeUserInfoView.cy;

	//���½���
	UpdateGameView(&rcUserInfoView);

	return true;
}

//�û�״̬
bool CGameClientView::SetUserStatusInfo(WORD wChairID, const tagStatusInfo & UserStatusInfo)
{
	//���ñ���
	m_UserStatusInfo[wChairID]=UserStatusInfo;

	//����λ��
	CRect rcUserInfoView;
	rcUserInfoView.top=m_ptUserInfoView.y;
	rcUserInfoView.left=m_ptUserInfoView.x;
	rcUserInfoView.right=rcUserInfoView.left+m_SizeUserInfoView.cx;
	rcUserInfoView.bottom=rcUserInfoView.top+m_SizeUserInfoView.cy;

	//���½���
	UpdateGameView(&rcUserInfoView);

	return true;
}

//ѡ������
bool CGameClientView::SelectChessItem(BYTE cbXPos, BYTE cbYPos)
{
	//��������
	m_ChessBorad.SelectChessItem(cbXPos,cbYPos);

	//����λ��
	CPoint BoardPoint;
	m_ChessBorad.GetBenchmarkPos(BoardPoint);

	//����λ��
	CRect rcBorad;
	rcBorad.SetRect(BoardPoint.x,BoardPoint.y,BoardPoint.x+m_SizeChessBorad.cx,BoardPoint.y+m_SizeChessBorad.cy);

	//���½���
	UpdateGameView(&rcBorad);

	return true;
}

//����·��
bool CGameClientView::SetMoveChessTrace(BYTE cbXPosStart, BYTE cbYPosStart, BYTE cbXPosTerminal, BYTE cbYPosTerminal)
{
	//��������
	m_ChessBorad.SetMoveChessTrace(cbXPosStart,cbYPosStart,cbXPosTerminal,cbYPosTerminal);

	//����λ��
	CPoint BoardPoint;
	m_ChessBorad.GetBenchmarkPos(BoardPoint);

	//����λ��
	CRect rcBorad;
	rcBorad.SetRect(BoardPoint.x,BoardPoint.y,BoardPoint.x+m_SizeChessBorad.cx,BoardPoint.y+m_SizeChessBorad.cy);

	//���½���
	UpdateGameView(&rcBorad);

	return true;
}

//�û�ʱ��
VOID CGameClientView::DrawUserClock(CDC * pDC, INT nXPos, INT nYPos, WORD wClockCount)
{
	//������Դ
	CImageHandle HandleClockNumber(&m_ImageClockNumber);
	INT nImageWidth=m_ImageClockNumber.GetWidth()/11;
	INT nImageHeight=m_ImageClockNumber.GetHeight();

	//�滭ʱ��
	WORD wHour=wClockCount/3600L;
	m_ImageClockNumber.BitBlt(pDC->m_hDC,nXPos,nYPos,nImageWidth,nImageHeight,wHour/10*nImageWidth,0);
	m_ImageClockNumber.BitBlt(pDC->m_hDC,nXPos+nImageWidth,nYPos,nImageWidth,nImageHeight,wHour%10*nImageWidth,0);

	//�滭ð��
	m_ImageClockNumber.BitBlt(pDC->m_hDC,nXPos+nImageWidth*2,nYPos,nImageWidth,nImageHeight,10*nImageWidth,0);

	//�滭����
	WORD wMinute=(wClockCount-wHour*3600)/60;
	m_ImageClockNumber.BitBlt(pDC->m_hDC,nXPos+nImageWidth*3,nYPos,nImageWidth,nImageHeight,wMinute/10*nImageWidth,0);
	m_ImageClockNumber.BitBlt(pDC->m_hDC,nXPos+nImageWidth*4,nYPos,nImageWidth,nImageHeight,wMinute%10*nImageWidth,0);

	//�滭ð��
	m_ImageClockNumber.BitBlt(pDC->m_hDC,nXPos+nImageWidth*5,nYPos,nImageWidth,nImageHeight,10*nImageWidth,0);

	//�滭����
	WORD wSecond=wClockCount%60L;
	m_ImageClockNumber.BitBlt(pDC->m_hDC,nXPos+nImageWidth*6,nYPos,nImageWidth,nImageHeight,wSecond/10*nImageWidth,0);
	m_ImageClockNumber.BitBlt(pDC->m_hDC,nXPos+nImageWidth*7,nYPos,nImageWidth,nImageHeight,wSecond%10*nImageWidth,0);

	return;
}

//�û��ȼ�
TCHAR *CGameClientView::GetLevelDescribe(LONG lUserScore,TCHAR tcUserInfo[32])
{
	WORD wSize = sizeof(tcUserInfo)*LEN_ACCOUNTS;
	if(lUserScore<30) _sntprintf(tcUserInfo,wSize,TEXT("������ͯ"));
	else if(lUserScore<80) _sntprintf(tcUserInfo,wSize,TEXT("������ͯ"));
	else if(lUserScore<160) _sntprintf(tcUserInfo,wSize,TEXT("һ����ͯ"));
	else if(lUserScore<280) _sntprintf(tcUserInfo,wSize,TEXT("�ż���ʿ"));
	else if(lUserScore<450) _sntprintf(tcUserInfo,wSize,TEXT("�˼���ʿ"));
	else if(lUserScore<680) _sntprintf(tcUserInfo,wSize,TEXT("�߼���ʿ"));
	else if(lUserScore<980) _sntprintf(tcUserInfo,wSize,TEXT("������ʿ"));
	else if(lUserScore<1360) _sntprintf(tcUserInfo,wSize,TEXT("�弶��ʿ"));
	else if(lUserScore<1830) _sntprintf(tcUserInfo,wSize,TEXT("�ļ���ʿ"));
	else if(lUserScore<2400) _sntprintf(tcUserInfo,wSize,TEXT("������ʿ"));
	else if(lUserScore<3880) _sntprintf(tcUserInfo,wSize,TEXT("������ʿ"));
	else if(lUserScore<4810) _sntprintf(tcUserInfo,wSize,TEXT("һ����ʿ"));
	else if(lUserScore<5880) _sntprintf(tcUserInfo,wSize,TEXT("��������"));
	else if(lUserScore<7100) _sntprintf(tcUserInfo,wSize,TEXT("��������"));
	else if(lUserScore<8480) _sntprintf(tcUserInfo,wSize,TEXT("һ������"));
	else if(lUserScore<10030) _sntprintf(tcUserInfo,wSize,TEXT("������ʦ"));
	else if(lUserScore<11760) _sntprintf(tcUserInfo,wSize,TEXT("������ʦ"));
	else if(lUserScore<13680) _sntprintf(tcUserInfo,wSize,TEXT("һ����ʦ"));
	else if(lUserScore<15800) _sntprintf(tcUserInfo,wSize,TEXT("���Ҵ�ʦ"));
	else _sntprintf(tcUserInfo,wSize,TEXT("�ؼ���ʦ"));

	//LevelItem1=������ͯ
	//LevelItem2=������ͯ,30
	//LevelItem3=һ����ͯ, 80
	//LevelItem4=�ż���ʿ, 160
	//LevelItem5=�˼���ʿ, 280
	//LevelItem6=�߼���ʿ, 450
	//LevelItem7=������ʿ, 680
	//LevelItem8=�弶��ʿ, 980
	//LevelItem9=�ļ���ʿ, 1360
	//LevelItem10=������ʿ,1830
	//LevelItem11=������ʿ,2400
	//LevelItem12=һ����ʿ,3880
	//LevelItem13=��������,4810
	//LevelItem14=��������,5880
	//LevelItem15=һ������,7100
	//LevelItem16=������ʦ,8480
	//LevelItem17=������ʦ,10030
	//LevelItem18=һ����ʦ,11760
	//LevelItem19=���Ҵ�ʦ,13680
	//LevelItem20=�ؼ���ʦ,15800

	return tcUserInfo;
}

//��ʼ��ť
VOID CGameClientView::OnBnClickedStart()
{
	//������Ϣ
	AfxGetMainWnd()->SendMessage(IDM_START,0,0);

	return;
}

//��Ͱ�ť
VOID CGameClientView::OnBnClickedPeace()
{
	//������Ϣ
	AfxGetMainWnd()->SendMessage(IDM_PEACE,0,0);

	return;
}

//���尴ť
VOID CGameClientView::OnBnClickedRegret()
{
	//������Ϣ
	AfxGetMainWnd()->SendMessage(IDM_REGRET,0,0);

	return;
}

//���䰴ť
VOID CGameClientView::OnBnClickedGiveUp()
{
	//������Ϣ
	AfxGetMainWnd()->SendMessage(IDM_GIVEUP,0,0);

	return;
}

//���װ�ť
VOID CGameClientView::OnBnClickedManual()
{
	//������Ϣ
	AfxGetMainWnd()->SendMessage(IDM_MANUAL,0,0);

	return;
}

//���水ť
VOID CGameClientView::OnBnClickedPreserve()
{
	//������Ϣ
	AfxGetMainWnd()->SendMessage(IDM_PRESERVE,0,0);

	return;
}

//���װ�ť
VOID CGameClientView::OnBnClickedHead()
{
	//������Ϣ
	WORD wStepCount=0;
	AfxGetMainWnd()->SendMessage(IDM_ORIENTATION_MANUAL,wStepCount,0);

	return;
}

//���װ�ť
VOID CGameClientView::OnBnClickedLast()
{
	//��ȡѡ��
	INT nCurrentSel=m_ManualList.GetCurSel();

	//������Ϣ
	if (nCurrentSel!=LB_ERR)
	{
		WORD wStepCount=nCurrentSel;
		AfxGetMainWnd()->SendMessage(IDM_ORIENTATION_MANUAL,wStepCount,0);
	}

	return;
}

//���װ�ť
VOID CGameClientView::OnBnClickedOpen()
{
	//��ʾ��Ϣ
	//CInformation Information(this);
	//Information.ShowMessageBox(TEXT("��ʾ"),TEXT("�����Ǵ򿪡��๦�����׹��������İ�ť�����ǽ���δ���������汾���ṩ�˹��ܣ�"));

	return;
}

//���װ�ť
VOID CGameClientView::OnBnClickedNext()
{
	//��ȡѡ��
	INT nCurrentSel=m_ManualList.GetCurSel();

	//������Ϣ
	WORD wStepCount=(nCurrentSel==LB_ERR)?1:(nCurrentSel+2);
	AfxGetMainWnd()->SendMessage(IDM_ORIENTATION_MANUAL,wStepCount,0);

	return;
}

//���װ�ť
VOID CGameClientView::OnBnClickedTail()
{
	//������Ϣ
	WORD wStepCount=m_ManualList.GetCount();
	AfxGetMainWnd()->SendMessage(IDM_ORIENTATION_MANUAL,wStepCount,0);

	return;
}

//ѡ��ı�
VOID CGameClientView::OnLbnSelchangeManualList()
{
	//��ȡѡ��
	INT nCurrentSel=m_ManualList.GetCurSel();

	//������Ϣ
	WORD wStepCount=nCurrentSel+1;
	AfxGetMainWnd()->SendMessage(IDM_ORIENTATION_MANUAL,wStepCount,0);

	return;
}

//��������
INT CGameClientView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//�����ؼ�
	CRect rcCreate(0,0,0,0);
	m_ScoreControl.Create(NULL,NULL,WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,rcCreate,this,100);
	m_ManualList.Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|LBS_NOINTEGRALHEIGHT|LBS_NOTIFY,rcCreate,this,IDC_MANUAL_LIST);

	//���ÿؼ�
	m_UserRequestPeace.m_cbRequestType=REQUEST_PEACE;
	m_UserRequestRegret.m_cbRequestType=REQUEST_REGRET;
	//m_ManualList.SetFont(&CSkinResourceManager::GetDefaultFont());

	//���ư�ť
	m_btManual.Create(NULL,WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS,rcCreate,this,IDC_MANUAL);
	m_btStart.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED|WS_CLIPSIBLINGS,rcCreate,this,IDC_START);
	m_btPeace.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED|WS_CLIPSIBLINGS,rcCreate,this,IDC_PEACE);
	m_btRegret.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED|WS_CLIPSIBLINGS,rcCreate,this,IDC_REGRET);
	m_btGiveUp.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED|WS_CLIPSIBLINGS,rcCreate,this,IDC_GIVEUP);
	m_btPreserve.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED|WS_CLIPSIBLINGS,rcCreate,this,IDC_PRESERVE);

	//���װ�ť
	m_btManualOpen.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_MANUAL_BT_OPEN);
	m_btManualHead.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_MANUAL_BT_HEAD);
	m_btManualLast.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_MANUAL_BT_LAST);
	m_btManualNext.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_MANUAL_BT_NEXT);
	m_btManualTail.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_MANUAL_BT_TAIL);

	//���ư�ť
	HINSTANCE hResInstance=AfxGetInstanceHandle();
	m_btStart.SetButtonImage(IDB_BT_START,hResInstance,false);
	m_btPeace.SetButtonImage(IDB_BT_PEACE,hResInstance,false);
	m_btManual.SetButtonImage(IDB_BT_MANUAL,hResInstance,false);
	m_btGiveUp.SetButtonImage(IDB_BT_GIVEUP,hResInstance,false);
	m_btRegret.SetButtonImage(IDB_BT_REGRET,hResInstance,false);
	m_btPreserve.SetButtonImage(IDB_BT_PRESERVE,hResInstance,false);

	//���װ�ť
	m_btManualHead.SetButtonImage(IDB_MANUAL_BT_HEAD,hResInstance,false);
	m_btManualNext.SetButtonImage(IDB_MANUAL_BT_NEXT,hResInstance,false);
	m_btManualTail.SetButtonImage(IDB_MANUAL_BT_TAIL,hResInstance,false);
	m_btManualOpen.SetButtonImage(IDB_MANUAL_BT_OPEN,hResInstance,false);
	m_btManualLast.SetButtonImage(IDB_MANUAL_BT_LAST,hResInstance,false);

	//������ʾ
	m_ToolTipCtrl.Create(this);
	m_ToolTipCtrl.Activate(TRUE);

	//������ʾ
	m_ToolTipCtrl.AddTool(&m_btStart,TEXT("��ʼ"));
	m_ToolTipCtrl.AddTool(&m_btPeace,TEXT("���"));
	m_ToolTipCtrl.AddTool(&m_btManual,TEXT("����"));
	m_ToolTipCtrl.AddTool(&m_btGiveUp,TEXT("����"));
	m_ToolTipCtrl.AddTool(&m_btRegret,TEXT("����"));
	m_ToolTipCtrl.AddTool(&m_btPreserve,TEXT("����"));

	//������ʾ
	m_ToolTipCtrl.AddTool(&m_btManualHead,TEXT("��ǰ"));
	m_ToolTipCtrl.AddTool(&m_btManualLast,TEXT("ǰһ��"));
	m_ToolTipCtrl.AddTool(&m_btManualOpen,TEXT("������"));
	m_ToolTipCtrl.AddTool(&m_btManualNext,TEXT("��һ��"));
	m_ToolTipCtrl.AddTool(&m_btManualTail,TEXT("���"));

#ifdef VIDEO_GAME
	//������Ƶ
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//������Ƶ
		m_DlgVideoService[i].Create(NULL,NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,200+i);
		m_DlgVideoService[i].InitVideoService(i==1,i==1);

		//������Ƶ
		g_VideoServiceManager.SetVideoServiceControl(i,&m_DlgVideoService[i]);
	}
#endif
	//��������
	//CDlgUserRequest * pDlgUserHint=&m_UserRequestRegret;
	//if (pDlgUserHint->m_hWnd==NULL) pDlgUserHint->Create(IDD_USER_REQUEST,this);

	////��ʾ����
	//pDlgUserHint->ShowWindow(SW_SHOW);
	//pDlgUserHint->SetForegroundWindow();

	////��������
	//CDlgHint * pDlgUserHint=&m_UserHint;
	//if (pDlgUserHint->m_hWnd==NULL) pDlgUserHint->Create(IDD_USER_HINT,this);

	//////��ʾ����
	//pDlgUserHint->ShowWindow(SW_SHOW);
	//pDlgUserHint->SetForegroundWindow();

	//m_ScoreControl.ShowWindow(5);
	return 0;
}

//�����Ϣ
VOID CGameClientView::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags, Point);

	//�������
	m_ChessBorad.OnEventLMouseDown(Point);

	return;
}

//�����Ϣ
VOID CGameClientView::OnLButtonDblClk(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDblClk(nFlags, Point);

	//������Ϣ
	WORD wStepCount=m_ManualList.GetCount();
	AfxGetMainWnd()->SendMessage(IDM_ORIENTATION_MANUAL,wStepCount,0);

	return;
}

//////////////////////////////////////////////////////////////////////////
