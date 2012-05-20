#include "StdAfx.h"
#include "Resource.h"
#include "ChessBorad.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

//λ�ö���
#define EXCURSION_X					18									//ƫ������
#define EXCURSION_Y					15									//ƫ������
#define CHESS_WIDTH					37									//���ӿ��
#define CHESS_HEIGHT				46									//���Ӹ߶�
#define SPACE_BORAD_X				56									//��϶��С
#define SPACE_BORAD_Y				56									//��϶��С

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CChessBorad, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CChessBorad::CChessBorad()
{
	CString strFile,strTemp;
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	strFile.Format("log\\%sCGameClientDlg.log",strTime);

	strTemp.Format("into CGameFrameDlg()");
	WriteLog(strFile, strTemp);

	//���ñ���
	m_bPositively=false;
	m_bShowChessName=false;
	m_cbUserColor=BLACK_CHESS;

	//��������
	m_cbXPosStart=255;
	m_cbYPosStart=255;
	m_cbXPosSelect=255;
	m_cbYPosSelect=255;
	m_cbXPosTerminal=255;
	m_cbYPosTerminal=255;

	//״̬����
	memset(m_ChessBorad,0,sizeof(m_ChessBorad));

	//������Դ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageChess.SetLoadInfo(IDB_CHESS,hInstance);
	m_ImageFrame.SetLoadInfo(IDB_CHESS_FRAME,hInstance);
	m_ImageBorad.SetLoadInfo(IDB_CHESS_BOARD,hInstance);

	//��ȡ��С
	CImageHandle BoradHandle(&m_ImageBorad);
	m_BoradSize.cx=m_ImageBorad.GetWidth();
	m_BoradSize.cy=m_ImageBorad.GetHeight();

	return;
}

//��������
CChessBorad::~CChessBorad()
{
}

//�������
void CChessBorad::CleanChess()
{
	//��������
	m_cbXPosStart=255;
	m_cbYPosStart=255;
	m_cbXPosSelect=255;
	m_cbYPosSelect=255;
	m_cbXPosTerminal=255;
	m_cbYPosTerminal=255;

	//���ñ���
	m_cbUserColor=BLACK_CHESS;
	memset(m_ChessBorad,0,sizeof(m_ChessBorad));

	//���½���
	Invalidate(FALSE);

	return;
}

//��������
void CChessBorad::SetSelectChess(BYTE cbXPos, BYTE cbYPos)
{
	//���ñ���
	m_cbXPosSelect=cbXPos;
	m_cbYPosSelect=cbYPos;

	//���½���
	Invalidate(FALSE);

	return;
}

//��������
void CChessBorad::SetChessBorad(tagChessItem * ChessBorad[8][8])
{
	//��������
	m_cbXPosSelect=255;
	m_cbYPosSelect=255;

	//��������
	CopyMemory(m_ChessBorad,ChessBorad,sizeof(m_ChessBorad));

	//���½���
	Invalidate(FALSE);

	return;
}

//������ɫ
void CChessBorad::SetChessColor(BYTE cbUserColor)
{
	if (m_cbUserColor!=cbUserColor)
	{
		m_cbUserColor=cbUserColor;
		if (GetSafeHwnd()!=NULL) Invalidate(FALSE);
	}

	return;
}

//������Ӧ
void CChessBorad::SetPositively(bool bPositively)
{
	m_bPositively=bPositively;
	return;
}

//��ʾ����
void CChessBorad::ShowChessName(bool bShowChessName)
{
	if (m_bShowChessName!=bShowChessName)
	{
		m_bShowChessName=bShowChessName;
		Invalidate(FALSE);
	}

	return;
}

//�������
void CChessBorad::SetChessFrame(BYTE cbXPosStart, BYTE cbYPosStart, BYTE cbXPosTerminal, BYTE cbYPosTerminal)
{
	//���ñ���
	m_cbXPosStart=cbXPosStart;
	m_cbYPosStart=cbYPosStart;
	m_cbXPosTerminal=cbXPosTerminal;
	m_cbYPosTerminal=cbYPosTerminal;

	//���½���
	Invalidate(FALSE);

	return;
}

//��ȡ����
int CChessBorad::GetChessXPos(BYTE cbXPos) 
{ 
	if (m_cbUserColor==WHITE_CHESS) cbXPos=7-cbXPos;
	return EXCURSION_X+cbXPos*SPACE_BORAD_X; 
}

//��ȡ����
int CChessBorad::GetChessYPos(BYTE cbYPos)
{ 
	if (m_cbUserColor==WHITE_CHESS) cbYPos=7-cbYPos;
	return EXCURSION_Y+cbYPos*SPACE_BORAD_Y; 
}

//��������
void CChessBorad::UpdateBoradPos(BYTE cbXPos, BYTE cbYPos)
{
	//Ч�����
	ASSERT(cbXPos!=255);
	ASSERT(cbYPos!=255);

	//ת������
	if (m_cbUserColor==WHITE_CHESS)
	{
		cbXPos=7-cbXPos;
		cbYPos=7-cbYPos;
	}

	//����λ��
	CRect rcChess;
	rcChess.top=GetChessYPos(cbYPos)-1;
	rcChess.left=GetChessXPos(cbXPos)-1;
	rcChess.right=rcChess.left+SPACE_BORAD_X+2;
	rcChess.bottom=rcChess.top+SPACE_BORAD_Y+2;

	//ˢ�½���
	InvalidateRect(&rcChess);

	return;
}

//��������
void CChessBorad::SwitchMousePos(const CPoint & MousePoint, BYTE & cbXPos, BYTE & cbYPos)
{
	if ((MousePoint.x>EXCURSION_X)&&(MousePoint.x<(EXCURSION_X+8*SPACE_BORAD_X))
		&&(MousePoint.y>EXCURSION_Y)&&(MousePoint.y<(EXCURSION_Y+8*SPACE_BORAD_Y)))
	{
		cbXPos=(BYTE)((MousePoint.x-EXCURSION_X)/SPACE_BORAD_X);
		cbYPos=(BYTE)((MousePoint.y-EXCURSION_Y)/SPACE_BORAD_Y);
	}
	else
	{
		cbXPos=255;
		cbYPos=255;
	}

	return;
}

//�ػ�����
void CChessBorad::OnPaint()
{
	CPaintDC dc(this);

	//��ȡ��С
	CRect rcClient;
	GetClientRect(&rcClient);	

	//��������ͼ
	CDC BufferDC;
	CBitmap BufferBmp;
	BufferDC.CreateCompatibleDC(&dc);
	BufferBmp.CreateCompatibleBitmap(&dc,rcClient.Width(),rcClient.Height());
	BufferDC.SelectObject(&BufferBmp);

	//���� DC
	BufferDC.SetBkMode(TRANSPARENT);
	BufferDC.SetTextColor(RGB(200,0,0));
	BufferDC.SelectObject(CSkinAttribute::m_DefaultFont);

	//�滭����
	CImageHandle BoradHandle(&m_ImageBorad);
	m_ImageBorad.BitBlt(BufferDC,0,0);

	//��������
	LPCTSTR pszChessName[]={TEXT("��"),TEXT("��"),TEXT("��"),TEXT("��"),TEXT("��"),TEXT("ʿ")};

	//�滭����
	CImageHandle FrameHandle(&m_ImageFrame);
	CImageHandle ChessHandle(&m_ImageChess);
	int nXPosImage=0,nYPosImage=0,nXBoradPos=0,nYBoradPos=0;
	for (BYTE cbXPos=0;cbXPos<8;cbXPos++)
	{
		for (BYTE cbYPos=0;cbYPos<8;cbYPos++)
		{
			//����λ��
			nXBoradPos=GetChessXPos(cbXPos);
			nYBoradPos=GetChessYPos(cbYPos);
			tagChessItem * pChessItem=m_ChessBorad[cbXPos][cbYPos];

			//�滭����
			if (pChessItem!=NULL)
			{
				nXPosImage=(pChessItem->cbChess-1)*CHESS_WIDTH;
				nYPosImage=(pChessItem->cbColor-1)*CHESS_HEIGHT;
				m_ImageChess.AlphaDrawImage(&BufferDC,nXBoradPos+(SPACE_BORAD_X-CHESS_WIDTH)/2,
					nYBoradPos+(SPACE_BORAD_Y-CHESS_HEIGHT)/2,CHESS_WIDTH,CHESS_HEIGHT,
					nXPosImage,nYPosImage,RGB(255,0,255));
			}

			//�滭���
			bool bFrame=((cbXPos==m_cbXPosStart)&&(cbYPos==m_cbYPosStart));
			if (bFrame==false) bFrame=((cbXPos==m_cbXPosSelect)&&(m_cbYPosSelect==cbYPos));
			if (bFrame==false) bFrame=((cbXPos==m_cbXPosTerminal)&&(cbYPos==m_cbYPosTerminal));
			if (bFrame==true) m_ImageFrame.AlphaDrawImage(&BufferDC,nXBoradPos,nYBoradPos,RGB(255,0,255));

			//�滭����
			if ((m_bShowChessName==true)&&(pChessItem!=NULL))
			{
				LPCTSTR pszTitle=pszChessName[pChessItem->cbChess-1];
				BufferDC.SetTextColor(((cbXPos+cbYPos)%2==1)?RGB(255,255,255):RGB(200,0,0));
				BufferDC.TextOut(nXBoradPos+5,nYBoradPos+5,pszTitle,lstrlen(pszTitle));
			}
		}
	}

	//�滭����
	dc.BitBlt(0,0,rcClient.Width(),rcClient.Height(),&BufferDC,0,0,SRCCOPY);

	//������Դ
	BufferBmp.DeleteObject();
	BufferDC.DeleteDC();

	return;
}

//������Ϣ
int CChessBorad::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//���ñ���
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);

	return 0;
}

//�����Ϣ
void CChessBorad::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags, Point);

	//״̬�ж�
	if (m_bPositively==false) return;

	//��ȡ���
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	ScreenToClient(&MousePoint);

	//ת������
	BYTE cbXPos=255,cbYPos=255;
	SwitchMousePos(MousePoint,cbXPos,cbYPos);

	//������Ϣ
	if ((cbXPos!=255)&&(cbYPos!=255)) 
	{
		if (m_cbUserColor==WHITE_CHESS) 
		{
			cbXPos=7-cbXPos;
			cbYPos=7-cbYPos;
		}
		GetParent()->PostMessage(WM_HIT_CHESS_BORAD,cbXPos,cbYPos);
	}

	return;
}

//////////////////////////////////////////////////////////////////////////
