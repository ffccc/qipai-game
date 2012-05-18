#include "StdAfx.h"
#include "Resource.h"
#include "ChessBorad.h"
#include ".\chessborad.h"
#include "GameClientView.h"
#include "GameClientDlg.h"
#include "GameLogic.h"
#include "ShowFlagDlg.h"

LONG								CChessBorad::m_bExsursionX;			//����ƫ��
LONG								CChessBorad::m_bExsursionY;			//����ƫ��

//////////////////////////////////////////////////////////////////////////

//���캯��
CChessBorad::CChessBorad()
{
	//���ñ���
	m_bPositively=false;
	m_cbUserColor=CHESS_COLOR_NO;

	//��������
	m_pParent=NULL ;
	memset(m_FlagInfo , 0 , sizeof(m_FlagInfo)) ;

	//״̬����
	memset(m_ChessBorad,0,sizeof(m_ChessBorad));

	//������Դ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageChess.SetLoadInfo(IDB_CHESS,hInstance);
	m_ImageBorad.SetLoadInfo(IDB_CHESS_BORAD,hInstance);
	m_ImageBackGround.SetLoadInfo(IDB_VIEW_BACK , hInstance) ;
	m_ImageArrow.SetLoadInfo(IDB_ARROW , hInstance) ;
	m_ImageFlag.SetLoadInfo(IDB_FLAG , hInstance) ;

	//��ȡ��С
	CImageHandle BoradHandle(&m_ImageBorad);
	m_BoradSize.cx=m_ImageBorad.GetWidth()+20;	//20�ǽ��ķ�֧�ĳ��ȵ�����
	m_BoradSize.cy=m_ImageBorad.GetHeight()+20;

	m_pFlagImageDlg = NULL ;
	m_bSetFlagX = 255 ;
	m_bSetFlagY = 255 ;


	return;
}

//��������
CChessBorad::~CChessBorad()
{
	if(m_pFlagImageDlg)
	{
		m_pFlagImageDlg->DestroyWindow() ;
		delete m_pFlagImageDlg ;
	}
}

//�������
void CChessBorad::CleanChess()
{
	//���ñ���
	m_cbUserColor=CHESS_COLOR_NO;
	memset(m_ChessBorad,0,sizeof(m_ChessBorad));
	memset(m_FlagInfo , 0 , sizeof(m_FlagInfo)) ;

	//���½���
	if(NULL!=m_pParent) m_pParent->UpdateGameView(NULL) ;

	return;
}

//��������
void CChessBorad::SetChess(const tagChessItem tagChessItems[17][17])
{
	CopyMemory(m_ChessBorad,tagChessItems,sizeof(tagChessItem)*17*17);

	//���½���
	if(NULL!=m_pParent) m_pParent->UpdateGameView(NULL) ;
}

//��ȡ����
int CChessBorad::GetChessXPos(BYTE cbXPos) 
{ 
	LONG RealBoardStartX = m_bExsursionX ;

    return cbXPos*NODE_WIDTH+RealBoardStartX; 
}

//��ȡ����
int CChessBorad::GetChessYPos(BYTE cbYPos)
{ 
	LONG RealBoardStartY = m_bExsursionY ;

	return cbYPos*NODE_HEIGTH+RealBoardStartY; 
}


//��������
void CChessBorad::SwitchMousePos(const CPoint & MousePoint, BYTE & cbXPos, BYTE & cbYPos)
{
	CPoint ClientPoint = MousePoint ;

	if(ClientPoint.x<=(m_bExsursionX-NODE_WIDTH/2+8+10) || ClientPoint.y<=(m_bExsursionY-NODE_HEIGTH/2+8+10) || ClientPoint.x>=(m_bExsursionX+m_BoradSize.cx+NODE_WIDTH/2-8-10) ||
		ClientPoint.y>=(m_bExsursionY+m_BoradSize.cy+NODE_HEIGTH/2-8-10))
	{
		cbXPos = 255 ;
		cbYPos = 255 ;
		return ;
	}

	LONG RealBoardStartX = m_bExsursionX ,	//����һ��Ϊ18�����أ����̵��������Ϊ8������,���ķ�֧�ĳ���Ϊ10
		 RealBoardStartY = m_bExsursionY ;

	cbXPos = (BYTE)((ClientPoint.x-RealBoardStartX)/NODE_WIDTH) ;
	cbYPos = (BYTE)((ClientPoint.y-RealBoardStartY)/NODE_HEIGTH);

	if((ClientPoint.x-RealBoardStartX) > (cbXPos+1)*NODE_WIDTH)  cbXPos++ ;
	if((ClientPoint.y-RealBoardStartY) > (cbYPos+1)*NODE_HEIGTH) cbYPos++ ;

	ASSERT(cbXPos>=0 && cbXPos<17 && cbYPos>=0 && cbYPos<17) ;
	if(cbXPos<0 || cbXPos>=17 || cbYPos<0 || cbYPos>=17) 
	{
		cbXPos = 255 ;
		cbYPos = 255 ;
		return ;
	}
	return;
}

//�ػ�����
void CChessBorad::DrawBoard(CDC *pDC)
{

	//����λͼ
	CRect rect ;
	m_pParent->GetClientRect(&rect) ;

	CDC BufferDC;
	CBitmap BufferBmp;
	BufferDC.CreateCompatibleDC(pDC);
	BufferBmp.CreateCompatibleBitmap(pDC,m_BoradSize.cx,m_BoradSize.cy);
	BufferDC.SelectObject(&BufferBmp);

	//�滭����
	CImageHandle BoradHandle(&m_ImageBorad);
	m_ImageBorad.AlphaDrawImage(&BufferDC,10,10,RGB(0,0,0));

	//�滭��ͷ
	DrawArrow(&BufferDC) ;

	//�滭����
	CImageHandle ChessHandle(&m_ImageChess) ;

	for(BYTE x=0 ; x<17 ; ++x)
		for(BYTE y=0 ; y<17 ; ++y)
		{
			if(CHESS_NO!=m_ChessBorad[x][y].bChessMode)
			{
				if(CHESS_BACK==m_ChessBorad[x][y].bChessMode)
                    m_ImageChess.BitBlt(BufferDC.GetSafeHdc() , x*NODE_WIDTH+(NODE_WIDTH-CHESS_WIDTH)/2 , y*NODE_HEIGTH+(NODE_HEIGTH-CHESS_HEIGTH)/2 , 
					CHESS_WIDTH , CHESS_HEIGTH , 0 , (m_ChessBorad[x][y].bColor-1)*CHESS_HEIGTH) ;
				else
					m_ImageChess.BitBlt(BufferDC.GetSafeHdc() , x*NODE_WIDTH+(NODE_WIDTH-CHESS_WIDTH)/2 , y*NODE_HEIGTH+(NODE_HEIGTH-CHESS_HEIGTH)/2 , 
					CHESS_WIDTH , CHESS_HEIGTH , m_ChessBorad[x][y].bChessID*CHESS_WIDTH , (m_ChessBorad[x][y].bColor-1)*CHESS_HEIGTH) ;
			}
		}

	//�滭��־
	CImageHandle FlagHandle(&m_ImageFlag) ;
    for(BYTE x=0 ; x<17 ; ++x)
		for(BYTE y=0 ; y<17 ; ++y)
		{
			if(true == m_FlagInfo[x][y].bSet)
			{
				if(255!=m_FlagInfo[x][y].bXPos && 255!=m_FlagInfo[x][y].bYPos)
					m_ImageFlag.AlphaDrawImage(&BufferDC , x*NODE_WIDTH+(NODE_WIDTH-FLAG_WIDTH)/2 , y*NODE_HEIGTH+(NODE_HEIGTH-FLAG_HEIGTH)/2+2 ,
					FLAG_WIDTH , FLAG_HEIGTH , m_FlagInfo[x][y].bXPos*FLAG_WIDTH+5 , m_FlagInfo[x][y].bYPos*FLAG_HEIGTH-8 , RGB(212 , 208 , 200)) ;

			}
		}


	//�滭����
	pDC->TransparentBlt(m_bExsursionX,m_bExsursionY,m_BoradSize.cx,m_BoradSize.cy,&BufferDC,0,0,m_BoradSize.cx,m_BoradSize.cy,RGB(0,0,0));

	//������Դ
	BufferBmp.DeleteObject();
	BufferDC.DeleteDC();

	return;
}

//����ƫ��
void CChessBorad::SetExcursionX(const LONG ExcursionX)
{
	m_bExsursionX = ExcursionX ;
}

//����ƫ��
void CChessBorad::SetExcursionY(const LONG ExcursionY)
{
	m_bExsursionY = ExcursionY ;
}


//���ø���
void CChessBorad::SetParent(CGameClientView *pWnd)
{
	m_pParent = pWnd ;
}

//����·��
void CChessBorad::SetPath(const CArray <POINT,POINT> *pPtPath)
{
	m_ptPathArray.RemoveAll() ;

	if (NULL!=pPtPath) m_ptPathArray.Copy(*pPtPath) ;

	//���½���
	if(NULL!=m_pParent) m_pParent->UpdateGameView(NULL) ;    
}


//�滭��ͷ
void CChessBorad::DrawArrow(CDC *pDC)
{
	if(TRUE==m_ptPathArray.IsEmpty()) return ;

	int nCount = (int)(m_ptPathArray.GetCount()) ;

    CImageHandle ArrowHandle(&m_ImageArrow) ;
	for (int i=0 ; i<nCount-1 ; i++)
	{
		POINT pt,ptParent;
		pt=m_ptPathArray[i];
		ptParent=m_ptPathArray[i+1];
		CGameLogic GameLogic ;

		if (true==GameLogic.NotOnNode((BYTE)(ptParent.x) , (BYTE)(ptParent.y)))
			continue;
		
		if (pt.x<ptParent.x && pt.y==ptParent.y)
			m_ImageArrow.AlphaDrawImage(pDC , ptParent.x*NODE_WIDTH+(NODE_WIDTH-ARROW_WIDTH)/2 , ptParent.y*NODE_HEIGTH+(NODE_HEIGTH-ARROW_HEIGTH)/2 , ARROW_WIDTH , ARROW_HEIGTH , 0 , 0 , RGB(255,0,255));
		else if ( pt.x<ptParent.x && pt.y<ptParent.y )
			m_ImageArrow.AlphaDrawImage(pDC , ptParent.x*NODE_WIDTH+(NODE_WIDTH-ARROW_WIDTH)/2 , ptParent.y*NODE_HEIGTH+(NODE_HEIGTH-ARROW_HEIGTH)/2 , ARROW_WIDTH , ARROW_HEIGTH , ARROW_WIDTH	  , 0 , RGB(255,0,255));
		else if (pt.x==ptParent.x && pt.y<ptParent.y)
			m_ImageArrow.AlphaDrawImage(pDC , ptParent.x*NODE_WIDTH+(NODE_WIDTH-ARROW_WIDTH)/2 , ptParent.y*NODE_HEIGTH+(NODE_HEIGTH-ARROW_HEIGTH)/2 , ARROW_WIDTH , ARROW_HEIGTH , ARROW_WIDTH*2 , 0 , RGB(255,0,255));
		else if (pt.x>ptParent.x && pt.y<ptParent.y)
			m_ImageArrow.AlphaDrawImage(pDC , ptParent.x*NODE_WIDTH+(NODE_WIDTH-ARROW_WIDTH)/2 , ptParent.y*NODE_HEIGTH+(NODE_HEIGTH-ARROW_HEIGTH)/2 , ARROW_WIDTH , ARROW_HEIGTH , ARROW_WIDTH*3 , 0 , RGB(255,0,255));
		else if (pt.x>ptParent.x && pt.y==ptParent.y)
			m_ImageArrow.AlphaDrawImage(pDC , ptParent.x*NODE_WIDTH+(NODE_WIDTH-ARROW_WIDTH)/2 , ptParent.y*NODE_HEIGTH+(NODE_HEIGTH-ARROW_HEIGTH)/2 , ARROW_WIDTH , ARROW_HEIGTH , ARROW_WIDTH*4 , 0 , RGB(255,0,255));
		else if (pt.x>ptParent.x && pt.y>ptParent.y )
			m_ImageArrow.AlphaDrawImage(pDC , ptParent.x*NODE_WIDTH+(NODE_WIDTH-ARROW_WIDTH)/2 , ptParent.y*NODE_HEIGTH+(NODE_HEIGTH-ARROW_HEIGTH)/2 , ARROW_WIDTH , ARROW_HEIGTH , ARROW_WIDTH*5 , 0 , RGB(255,0,255));
		else if (pt.x==ptParent.x && pt.y>ptParent.y)
			m_ImageArrow.AlphaDrawImage(pDC , ptParent.x*NODE_WIDTH+(NODE_WIDTH-ARROW_WIDTH)/2 , ptParent.y*NODE_HEIGTH+(NODE_HEIGTH-ARROW_HEIGTH)/2 , ARROW_WIDTH , ARROW_HEIGTH , ARROW_WIDTH*6 , 0 , RGB(255,0,255));
		else if (pt.x<ptParent.x && pt.y>ptParent.y)
			m_ImageArrow.AlphaDrawImage(pDC , ptParent.x*NODE_WIDTH+(NODE_WIDTH-ARROW_WIDTH)/2 , ptParent.y*NODE_HEIGTH+(NODE_HEIGTH-ARROW_HEIGTH)/2 , ARROW_WIDTH , ARROW_HEIGTH, ARROW_WIDTH*7 , 0 , RGB(255,0,255));
			

	}
}

//���ñ�־
void CChessBorad::SetFlagImage(const BYTE bFlagXPos , const BYTE bFlagYPos , bool bSet)
{
	//�����־
	if(255==m_bSetFlagX && 255==m_bSetFlagY && false==bSet)
	{
		for(BYTE x = 0 ; x<17 ; ++x)
			for(BYTE y = 0 ; y<17 ; ++y)
			{
				m_FlagInfo[x][y].bSet  = false ;
				m_FlagInfo[x][y].bXPos = 255 ;
				m_FlagInfo[x][y].bYPos = 255 ;
			}
	}
	//�����־
	else if(255!=m_bSetFlagX && 255!=m_bSetFlagY && false==bSet)
	{
		m_FlagInfo[m_bSetFlagX][m_bSetFlagY].bSet  = false ;
		m_FlagInfo[m_bSetFlagX][m_bSetFlagY].bXPos = 255 ;
		m_FlagInfo[m_bSetFlagX][m_bSetFlagY].bYPos = 255 ;
	}
	//���ñ�־
	else if(255!=m_bSetFlagX && 255!=m_bSetFlagY && true==bSet)
	{
		m_FlagInfo[m_bSetFlagX][m_bSetFlagY].bSet  = true ;
		m_FlagInfo[m_bSetFlagX][m_bSetFlagY].bXPos = bFlagXPos ;
		m_FlagInfo[m_bSetFlagX][m_bSetFlagY].bYPos = bFlagYPos ;
	}
	else
		AfxMessageBox("CChessBorad::SetFlagImage ����") ;

	//���½���
	if(NULL!=m_pParent) m_pParent->UpdateGameView(NULL) ; 
}

//��ʾ��־
void CChessBorad::ShowFlagImage(CPoint point , bool bShow) 
{
	//��������

	m_StartPoint.x = point.x ;//- m_bExsursionX ;
	m_StartPoint.y = point.y ;//- m_bExsursionY ;

	m_pParent->ClientToScreen(&m_StartPoint) ;

	if(true==bShow)
	{
		CImageHandle FlagHandle(&m_ImageFlag) ;
		CRect rect(0 , 0 , m_ImageFlag.GetWidth() , m_ImageFlag.GetHeight()) ;
		//�����ؼ�
		if(!m_pFlagImageDlg)
		{

			m_pFlagImageDlg = new CShowFlagDlg(this) ;
			m_pFlagImageDlg->Create(IDD_FLAGDLG , NULL) ; ;
		}
		m_pFlagImageDlg->MoveWindow(m_StartPoint.x , m_StartPoint.y , m_ImageFlag.GetWidth() , m_ImageFlag.GetHeight() , TRUE) ;
		m_pFlagImageDlg->BringWindowToTop() ;
		m_pFlagImageDlg->ShowWindow(SW_SHOW) ;
		//��ʾ�ؼ�
	}

	//���½���
	if(NULL!=m_pParent) m_pParent->UpdateGameView(NULL) ; 
}

//��־����
void CChessBorad::SetFlaChessPos(BYTE bXPos , BYTE bYPos) 
{
	m_bSetFlagX = bXPos ;
	m_bSetFlagY = bYPos ;
}

//�����־
void CChessBorad::ClearFlagImage(BYTE bXPos , BYTE bYPos)
{
	//���б�־
	if(255==bXPos && 255==bYPos) 
	{
		m_bSetFlagX = 255 ;
		m_bSetFlagY = 255 ;
		SetFlagImage(bXPos , bYPos , false) ;
	}
	//������־
	else
		SetFlagImage(bXPos , bYPos , false) ;
}

//��ת��־
void CChessBorad::ReverseFlagImage()
{
	for(BYTE x=0 ; x<8 ; ++x)
		for(BYTE y=0 ; y<17 ; ++y)
		{
			tagFlagInfo tmpInfo = m_FlagInfo[x][y] ;
			m_FlagInfo[x][y]    = m_FlagInfo[16-x][y] ;
			m_FlagInfo[16-x][y] = tmpInfo ;
		}
	//���½���
	if(NULL!=m_pParent) m_pParent->UpdateGameView(NULL) ; 
}

//�ƶ���־
void CChessBorad::MoveFlagImage(BYTE bSourceX , BYTE bSourceY , BYTE bTargetX , BYTE bTargetY , bool bSet)
{
	//ԭ��û��
	if(false==m_FlagInfo[bSourceX][bSourceY].bSet) return ;

	//�����־
	if(false==bSet)
	{
		m_FlagInfo[bSourceX][bSourceY].bSet  = false ;
		m_FlagInfo[bSourceX][bSourceY].bXPos = 255 ;
		m_FlagInfo[bSourceX][bSourceY].bYPos = 255 ;
	}
	//����λ��
	else
	{
		//�ƶ���־
		m_FlagInfo[bTargetX][bTargetY].bSet = true ;
		m_FlagInfo[bTargetX][bTargetY].bXPos = m_FlagInfo[bSourceX][bSourceY].bXPos ;
		m_FlagInfo[bTargetX][bTargetY].bYPos = m_FlagInfo[bSourceX][bSourceY].bYPos ;

		//ԭ����־
		m_FlagInfo[bSourceX][bSourceY].bSet  = false ;
		m_FlagInfo[bSourceX][bSourceY].bXPos = 255 ;
		m_FlagInfo[bSourceX][bSourceY].bYPos = 255 ;
	}

	//���½���
	if(NULL!=m_pParent) m_pParent->UpdateGameView(NULL) ;
}