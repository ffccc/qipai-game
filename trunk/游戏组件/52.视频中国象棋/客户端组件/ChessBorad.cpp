#include "StdAfx.h"
#include "GameClient.h"
#include "ChessBorad.h"

//////////////////////////////////////////////////////////////////////////

//λ�ö���
#define EXCURSION_X					16									//ƫ������
#define EXCURSION_Y					15									//ƫ������
#define SPACE_BORAD_X				57									//��϶��С
#define SPACE_BORAD_Y				57									//��϶��С

//λ�ö���
#define EXCURSION_X_S				10									//ƫ������
#define EXCURSION_Y_S				8									//ƫ������
#define SPACE_BORAD_X_S				45									//��϶��С
#define SPACE_BORAD_Y_S				45									//��϶��С

//////////////////////////////////////////////////////////////////////////

//���캯��
CChessBorad::CChessBorad()
{
	//���ñ���
	m_bPositively=false;
	m_bManualView=false;
	m_bWaitGameRule=false;
	m_cbUserColor=CHESS_BLACK;

	//��������
	m_cbXPosStart=0xFF;
	m_cbYPosStart=0xFF;
	m_cbXPosSelect=0xFF;
	m_cbYPosSelect=0xFF;
	m_cbXPosTerminal=0xFF;
	m_cbYPosTerminal=0xFF;

	//���ñ���
	m_SizeBorad.SetSize(0,0);
	m_SizeChess.SetSize(0,0);
	m_BenchmarkPos.SetPoint(0,0);

	//���̱���
	ZeroMemory(m_ChessItemPtr,sizeof(m_ChessItemPtr));

	//������Դ
	m_ImageWaitGameRule.SetLoadInfo(IDB_WAIT_GAME_RULE,AfxGetInstanceHandle());

	return;
}

//��������
CChessBorad::~CChessBorad()
{
}

//�滭����
VOID CChessBorad::DrawChessBorad(CDC * pDC)
{
	//������Դ
	CImageHandle HandleChessFrame(&m_ImageChessFrame);
	CImageHandle HandleChessBorad(&m_ImageChessBorad);

	//�滭����
	m_ImageChessBorad.BitBlt(pDC->m_hDC,m_BenchmarkPos.x,m_BenchmarkPos.y);

	//���ױ�־
	if (m_bManualView==true)
	{
		//������Դ
		CPngImage ImageManualFlag;
		ImageManualFlag.LoadImage(AfxGetInstanceHandle(),TEXT("MANUAL_FLAG"));

		//�滭��־
		INT nImageWidth=ImageManualFlag.GetWidth();
		INT nImageHeight=ImageManualFlag.GetHeight();
		ImageManualFlag.DrawImage(pDC,m_BenchmarkPos.x+(m_SizeBorad.cx-nImageWidth)/2,m_BenchmarkPos.y+m_SizeBorad.cy/2+108);
		ImageManualFlag.DrawImage(pDC,m_BenchmarkPos.x+(m_SizeBorad.cx-nImageWidth)/2,m_BenchmarkPos.y+m_SizeBorad.cy/2-nImageHeight-120);
	}

	//�滭����
	for (BYTE cbXPos=0;cbXPos<COUNT_X;cbXPos++)
	{
		for (BYTE cbYPos=0;cbYPos<COUNT_Y;cbYPos++)
		{
			//����λ��
			INT nXBoradPos=GetChessXPos(cbXPos);
			INT nYBoradPos=GetChessYPos(cbYPos);
			tagChessItem * pChessItem=m_ChessItemPtr[cbXPos][cbYPos];

			//�滭����
			if (pChessItem!=NULL)
			{
				INT nXPosImage=(pChessItem->cbChess-1)*m_SizeChess.cx;
				INT nYPosImage=(pChessItem->cbColor-1)*m_SizeChess.cy;
				m_ImageChessItem.DrawImage(pDC,nXBoradPos+(m_XBoradSpace-m_SizeChess.cx)/2,
					nYBoradPos+(m_YBoradSpace-m_SizeChess.cy)/2,m_SizeChess.cx,m_SizeChess.cy,nXPosImage,nYPosImage);
			}

			//�滭���
			bool bFrame=false;
			if ((cbXPos==m_cbXPosStart)&&(cbYPos==m_cbYPosStart)) bFrame=true;
			if ((cbXPos==m_cbXPosSelect)&&(m_cbYPosSelect==cbYPos)) bFrame=true;
			if ((cbXPos==m_cbXPosTerminal)&&(cbYPos==m_cbYPosTerminal)) bFrame=true;
			if (bFrame==true) m_ImageChessFrame.AlphaDrawImage(pDC,nXBoradPos,nYBoradPos,RGB(255,0,255));
		}
	}

	//�ȴ���־
	if (m_bWaitGameRule==true)
	{
		CImageHandle HandleWaitGameRule(&m_ImageWaitGameRule);
		INT nXPos=(m_SizeBorad.cx-m_ImageWaitGameRule.GetWidth())/2;
		INT nYPos=(m_SizeBorad.cy-m_ImageWaitGameRule.GetHeight())/2;
		m_ImageWaitGameRule.AlphaDrawImage(pDC,m_BenchmarkPos.x+nXPos,m_BenchmarkPos.y+nYPos,RGB(255,0,255));
	}

	return;
}

//����ģʽ
VOID CChessBorad::SetChessBoradMode(bool bSmallMode)
{
	//������Դ
	if (bSmallMode==false)
	{
		//���ñ���
		m_XExcursion=EXCURSION_X;
		m_YExcursion=EXCURSION_Y;
		m_XBoradSpace=SPACE_BORAD_X;
		m_YBoradSpace=SPACE_BORAD_Y;

		//������Դ
		m_ImageChessItem.DestroyImage();
		m_ImageChessItem.LoadImage(AfxGetInstanceHandle(),TEXT("CHESS_ITEM"));
		m_ImageChessFrame.SetLoadInfo(IDB_CHESS_FRAME,AfxGetInstanceHandle());
		m_ImageChessBorad.SetLoadInfo(IDB_CHESS_BORAD,AfxGetInstanceHandle());

		//��ȡ��С
		CImageHandle HandleChessBorad(&m_ImageChessBorad);
		m_SizeBorad.SetSize(m_ImageChessBorad.GetWidth(),m_ImageChessBorad.GetHeight());
		m_SizeChess.SetSize(m_ImageChessItem.GetWidth()/7,m_ImageChessItem.GetHeight()/2);
	}
	else
	{
		//���ñ���
		m_XExcursion=EXCURSION_X_S;
		m_YExcursion=EXCURSION_Y_S;
		m_XBoradSpace=SPACE_BORAD_X_S;
		m_YBoradSpace=SPACE_BORAD_Y_S;

		//������Դ
		m_ImageChessItem.DestroyImage();
		m_ImageChessItem.LoadImage(AfxGetInstanceHandle(),TEXT("CHESS_ITEM_S"));
		m_ImageChessFrame.SetLoadInfo(IDB_CHESS_FRAME_S,AfxGetInstanceHandle());
		m_ImageChessBorad.SetLoadInfo(IDB_CHESS_BORAD_S,AfxGetInstanceHandle());

		//��ȡ��С
		CImageHandle HandleChessBorad(&m_ImageChessBorad);
		m_SizeBorad.SetSize(m_ImageChessBorad.GetWidth(),m_ImageChessBorad.GetHeight());
		m_SizeChess.SetSize(m_ImageChessItem.GetWidth()/7,m_ImageChessItem.GetHeight()/2);
	}

	return;
}

//�������
VOID CChessBorad::CleanChessItem()
{
	//��������
	m_cbXPosStart=0xFF;
	m_cbYPosStart=0xFF;
	m_cbXPosSelect=0xFF;
	m_cbYPosSelect=0xFF;
	m_cbXPosTerminal=0xFF;
	m_cbYPosTerminal=0xFF;

	//���ñ���
	m_cbUserColor=CHESS_BLACK;
	ZeroMemory(m_ChessItemPtr,sizeof(m_ChessItemPtr));

	return;
}

//��������
VOID CChessBorad::SetChessBorad(tagChessItem * ChessItemPtr[COUNT_X][COUNT_Y])
{
	//��������
	CopyMemory(m_ChessItemPtr,ChessItemPtr,sizeof(m_ChessItemPtr));

	return;
}

//��������
VOID CChessBorad::SelectChessItem(BYTE cbXPos, BYTE cbYPos)
{
	//���ñ���
	m_cbXPosSelect=cbXPos;
	m_cbYPosSelect=cbYPos;

	return;
}

//����·��
VOID CChessBorad::SetMoveChessTrace(BYTE cbXPosStart, BYTE cbYPosStart, BYTE cbXPosTerminal, BYTE cbYPosTerminal)
{
	//���ñ���
	m_cbXPosStart=cbXPosStart;
	m_cbYPosStart=cbYPosStart;
	m_cbXPosTerminal=cbXPosTerminal;
	m_cbYPosTerminal=cbYPosTerminal;

	return;
}

//�����
VOID CChessBorad::OnEventLMouseDown(CPoint Point)
{
	//״̬�ж�
	if ((m_bPositively==false)||(m_bManualView==true)) return;

	//��ȡλ��
	INT nXCursor=Point.x-m_BenchmarkPos.x;
	INT nYCursor=Point.y-m_BenchmarkPos.y;

	//ת������
	BYTE cbXPos=INVALID_POS,cbYPos=INVALID_POS;
	SwitchMousePos(nXCursor,nYCursor,cbXPos,cbYPos);

	//������Ϣ
	if ((cbXPos!=INVALID_POS)&&(cbYPos!=INVALID_POS)) 
	{
		//����ת��
		if (m_cbUserColor==CHESS_WHITE)
		{
			cbXPos=COUNT_X-cbXPos-1;
			cbYPos=COUNT_Y-cbYPos-1;
		}

		//��ѯ�ӿ�
		//CGlobalUnits * pGlobalUnits=CGlobalUnits::GetGlobalUnits();
		//IGameFrameEngine * pIGameFrameEngine=(IGameFrameEngine *)pGlobalUnits->QueryGlobalModule(MODULE_GAME_FRAME_ENGINE,IID_IGameFrameEngine,VER_IGameFrameEngine);

		//���ƽ���
		//ASSERT(pIGameFrameEngine!=NULL);
		/*if (pIGameFrameEngine!=NULL)*/ AfxGetMainWnd()->SendMessage(WM_HIT_CHESS_BORAD,cbXPos,cbYPos);
	}

	return;
}

//��ȡ����
INT CChessBorad::GetChessXPos(BYTE cbXPos) 
{ 
	if (m_cbUserColor==CHESS_WHITE) cbXPos=COUNT_X-cbXPos-1;
	return m_BenchmarkPos.x+m_XExcursion+cbXPos*m_XBoradSpace; 
}

//��ȡ����
INT CChessBorad::GetChessYPos(BYTE cbYPos)
{ 
	if (m_cbUserColor==CHESS_WHITE) cbYPos=COUNT_Y-cbYPos-1;
	return m_BenchmarkPos.y+m_XExcursion+cbYPos*m_YBoradSpace; 
}

//ת������
VOID CChessBorad::SwitchMousePos(INT nXCursor, INT nYCursor, BYTE & cbXIndex, BYTE & cbYIndex)
{
	//λ�ü��
	if ((nXCursor>m_XExcursion)&&(nXCursor<(m_XExcursion+COUNT_X*m_XBoradSpace))
		&&(nYCursor>m_XExcursion)&&(nYCursor<(m_XExcursion+COUNT_Y*m_YBoradSpace)))
	{
		//����λ��
		cbXIndex=(BYTE)((nXCursor-m_XExcursion)/m_XBoradSpace);
		cbYIndex=(BYTE)((nYCursor-m_XExcursion)/m_YBoradSpace);
	}
	else
	{
		//���ý��
		cbXIndex=0xFF;
		cbYIndex=0xFF;
	}

	return;
}

//////////////////////////////////////////////////////////////////////////
