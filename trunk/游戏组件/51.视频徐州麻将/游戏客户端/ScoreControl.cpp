#include "StdAfx.h"
#include "GameClient.h"
#include "ScoreControl.h"
#include ".\scorecontrol.h"

//////////////////////////////////////////////////////////////////////////

//��ť��ʶ
#define IDC_CLOSE_SCORE				100									//�رճɼ�

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CScoreControl, CWnd)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_CLOSE_SCORE, OnBnClickedClose)
	ON_WM_NCDESTROY()
	ON_WM_MOVE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CScoreControl::CScoreControl()
{
	//���ñ���
	m_cbWeaveCount=0;
	ZeroMemory(&m_ScoreInfo,sizeof(m_ScoreInfo));

	//������Դ
	HINSTANCE hResInstance=AfxGetInstanceHandle();
	m_ImageWinLose.SetLoadInfo(IDB_WIN_LOSE,hResInstance);
	m_ImageGameScore.LoadFromResource(hResInstance,IDB_GAME_SCORE);
	m_ImageGameScoreFlag.SetLoadInfo(IDB_GAME_SCORE_FLAG,hResInstance);

	//���ÿؼ�
	for (BYTE i=0;i<CountArray(m_WeaveCard);i++) m_WeaveCard[i].SetDirection(Direction_South);

	return;
}

//��������
CScoreControl::~CScoreControl()
{
}

//��λ����
void CScoreControl::RestorationData()
{
	//���ñ���
	m_cbWeaveCount=0;
	ZeroMemory(&m_ScoreInfo,sizeof(m_ScoreInfo));

	//���ش���
	if (m_hWnd!=NULL) ShowWindow(SW_HIDE);

	return;
}

//���û���
void CScoreControl::SetScoreInfo(const tagScoreInfo & ScoreInfo, const tagWeaveInfo & WeaveInfo)
{
	//���ñ���
	m_ScoreInfo=ScoreInfo;
	m_cbWeaveCount=WeaveInfo.cbWeaveCount;

	//��ϱ���
	for (BYTE i=0;i<m_cbWeaveCount;i++)
	{
		bool bPublicWeave=(WeaveInfo.cbPublicWeave[i]==TRUE);
		m_WeaveCard[i].SetCardData(WeaveInfo.cbCardData[i],WeaveInfo.cbCardCount[i]);
		m_WeaveCard[i].SetDisplayItem(true);
	}

	//��ʾ����
	ShowWindow(SW_SHOW);

	return;
}

//�رհ�ť
void CScoreControl::OnBnClickedClose()
{
	//���ش���
	RestorationData();

	return;
}

//�ػ�����
void CScoreControl::OnPaint()
{
	CPaintDC dc(this);

	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//��������
	CFont InfoFont;
	InfoFont.CreatePointFont(110,TEXT("����"),&dc);

	//��������
	CDC DCBuffer;
	CBitmap ImageBuffer;
	DCBuffer.CreateCompatibleDC(&dc);
	ImageBuffer.CreateCompatibleBitmap(&dc,rcClient.Width(),rcClient.Height());

	//���� DC
	DCBuffer.SetBkMode(TRANSPARENT);
	DCBuffer.SelectObject(&ImageBuffer);
	DCBuffer.SetTextColor(RGB(255,255,255));
	DCBuffer.SelectObject(CSkinResourceManager::GetDefaultFont());

	//������Դ
	CImageHandle HandleWinLose(&m_ImageWinLose);
	CImageHandle HandleGameScoreFlag(&m_ImageGameScoreFlag);

	//�滭����
	m_ImageGameScore.Draw(DCBuffer.GetSafeHdc(),0,0);

	//λ�ñ���
	int nCardSpace=5;
	int nItemWidth=g_CardResource.m_ImageTableBottom.GetViewWidth();
	int nTotalWidth=m_cbWeaveCount*(nItemWidth*3+nCardSpace)+nItemWidth*m_ScoreInfo.cbCardCount+nCardSpace;

	//����λ��
	int nYCardPos=110;
	int nXCardPos=(m_ImageGameScore.GetWidth()-nTotalWidth)/2;

	//�滭���
	for (BYTE i=0;i<m_cbWeaveCount;i++)
	{
		//�滭�˿�
		m_WeaveCard[i].DrawCardControl(&DCBuffer,nXCardPos,nYCardPos);

		//����λ��
		nXCardPos+=(nCardSpace+nItemWidth*3);
	}

	//�滭�˿�
	for (BYTE i=0;i<m_ScoreInfo.cbCardCount;i++)
	{
		//�滭�˿�
		int nXCurrentPos=nXCardPos;
		int nYCurrentPos=nYCardPos-g_CardResource.m_ImageTableBottom.GetViewHeight()-5;
		g_CardResource.m_ImageTableBottom.DrawCardItem(&DCBuffer,m_ScoreInfo.cbCardData[i],nXCurrentPos,nYCurrentPos);

		//����λ��
		nXCardPos+=nItemWidth;
		if ((i+2)==m_ScoreInfo.cbCardCount) nXCardPos+=nCardSpace;
	}

	//�滭����
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//�û�����
		if (m_ScoreInfo.dwChiHuKind[i]==CHK_NULL) continue;

		//������Ϣ
		DWORD dwCardKind[]={CHK_PENG_PENG,CHK_QI_XIAO_DUI,CHK_SHI_SAN_YAO};
		DWORD dwCardRight[]={CHR_DI,CHR_TIAN,CHR_QING_YI_SE,CHR_QIANG_GANG,CHK_QUAN_QIU_REN};

		//������Ϣ
		CString strCardInfo;
		LPCTSTR pszCardKind[]={TEXT("������"),TEXT("��С��"),TEXT("ʮ����")};
		LPCTSTR pszCardRight[]={TEXT("�غ�"),TEXT("���"),TEXT("��һɫ"),TEXT("�ܺ�"),TEXT("ȫ����")};

		//������Ϣ
		for (BYTE j=0;j<CountArray(dwCardKind);j++)
		{
			if (m_ScoreInfo.dwChiHuKind[i]&dwCardKind[j])
			{
				strCardInfo+=pszCardKind[j];
				strCardInfo+=TEXT("  ");
			}
		}

		//��Ȩ��Ϣ
		for (BYTE j=0;j<CountArray(dwCardRight);j++)
		{
			if (m_ScoreInfo.dwChiHuRight[i]&dwCardRight[j])
			{
				strCardInfo+=pszCardRight[j];
				strCardInfo+=TEXT("  ");
			}
		}

		//�滭��Ϣ
		CRect rcCardInfo(20,310,406,322);
		DCBuffer.DrawText(strCardInfo,rcCardInfo,DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);

		break;
	}

	//���� DC
	DCBuffer.SelectObject(InfoFont);
	DCBuffer.SetTextColor(RGB(255,255,255));

	//������Ϣ
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��������
		TCHAR szUserScore[16]=TEXT("");
		_sntprintf(szUserScore,CountArray(szUserScore),TEXT("%d"),m_ScoreInfo.lGameScore[i]);

		//λ�ü���
		CRect rcName(29,165+i*30,132,165+(i+1)*30);
		CRect rcScore(130,165+i*30,190,165+(i+1)*30);
		CRect rcStatus(220,165+i*30,260,165+(i+1)*30);

		//�滭��Ϣ
		UINT nFormat=DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER;
		DCBuffer.DrawText(szUserScore,lstrlen(szUserScore),&rcScore,nFormat|DT_CENTER);
		DCBuffer.DrawText(m_ScoreInfo.szUserName[i],lstrlen(m_ScoreInfo.szUserName[i]),&rcName,nFormat|DT_CENTER);

		//ׯ�ұ�־
		if (m_ScoreInfo.wBankerUser==i)
		{
			int nImageWidht=m_ImageGameScoreFlag.GetWidth();
			int nImageHeight=m_ImageGameScoreFlag.GetHeight();
			m_ImageGameScoreFlag.BlendDrawImage(&DCBuffer,310,168+i*30,RGB(255,0,255),240);
		}
		//�û�״̬
		if ((m_ScoreInfo.dwChiHuKind[i]!=0)&&((m_ScoreInfo.wProvideUser!=i)))
		DCBuffer.DrawText(TEXT("����"),lstrlen(TEXT("����")),&rcStatus,nFormat|DT_CENTER);

		//�û�״̬
		if ((m_ScoreInfo.wProvideUser==i)&&(m_ScoreInfo.dwChiHuKind[i]==0))
			DCBuffer.DrawText(TEXT("����"),lstrlen(TEXT("����")),&rcStatus,nFormat|DT_CENTER);

		//�û�״̬
		if ((m_ScoreInfo.dwChiHuKind[i]!=0)&&((m_ScoreInfo.wProvideUser==i)))
		DCBuffer.DrawText(TEXT("����"),lstrlen(TEXT("����")),&rcStatus,nFormat|DT_CENTER);



		//��Ӯ��־
		int nImageWidht=m_ImageWinLose.GetWidth()/3;
		int nImageHeight=m_ImageWinLose.GetHeight();

		//�滭��־
		int nImageExcursion=2*nImageWidht;
		if (m_ScoreInfo.lGameScore[i]>0L) nImageExcursion=0;
		if (m_ScoreInfo.lGameScore[i]<0L) nImageExcursion=nImageWidht;
		m_ImageWinLose.BlendDrawImage(&DCBuffer,390,169+i*29,nImageWidht,nImageHeight,nImageExcursion,0,RGB(255,0,255),240);
	}

	//�滭����
	dc.BitBlt(0,0,rcClient.Width(),rcClient.Height(),&DCBuffer,0,0,SRCCOPY);

	//������Դ
	DCBuffer.DeleteDC();
	InfoFont.DeleteObject();
	ImageBuffer.DeleteObject();

	return;
}

//�滭����
BOOL CScoreControl::OnEraseBkgnd(CDC * pDC)
{
	//���½���
	Invalidate(FALSE);
	UpdateWindow();

	return TRUE;
}

//��������
int CScoreControl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;


	//���ô���
	SetWindowPos(NULL,0,0,m_ImageGameScore.GetWidth(),m_ImageGameScore.GetHeight(),SWP_NOZORDER|SWP_NOMOVE);

	//������ť
	CRect rcCreate(0,0,0,0);
	m_btCloseScore.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_CLOSE_SCORE);
	m_btCloseScore.SetButtonImage(IDB_BT_SCORE_CLOSE,AfxGetInstanceHandle(),false);

	//������ť
	CRect rcClient;
	GetClientRect(&rcClient);
	m_btCloseScore.SetWindowPos(NULL,rcClient.Width()-31,3,0,0,SWP_NOSIZE|SWP_NOZORDER);

	return 0;
}

//�����Ϣ
void CScoreControl::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);

	//��Ϣģ��
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(Point.x,Point.y));

	return;
}

//////////////////////////////////////////////////////////////////////////


void CScoreControl::OnMove(int x, int y)
{
	__super::OnMove(x, y);

	//���½���
	Invalidate(FALSE);
	UpdateWindow();

}
