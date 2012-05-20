#include "StdAfx.h"
#include "Resource.h"
#include "GameLogic.h"
#include "ChessBorad.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

#define CHESS_WIDTH					25									//���ӿ��
#define CHESS_HEIGHT				25									//���Ӹ߶�

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CChessBorad, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CChessBorad::CChessBorad()
{
	//���ñ���
	m_bShowChessID=false;
	m_bDoubleMouse=false;
	m_cbSelectMode=SM_NO_SELECT;

	//��������
	m_cbXCursor=255;
	m_cbYCursor=255;

	//��Ŀ����
	m_bCountInfo=false;
	memset(m_cbCountInfo,0,sizeof(m_cbCountInfo));

	//״̬����
	m_cbXPosFocus=255;
	m_cbYPosFocus=255;
	memset(m_ChessInfo,0,sizeof(m_ChessInfo));

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
	//���ñ���
	m_cbXPosFocus=255;
	m_cbYPosFocus=255;
	m_bCountInfo=false;
	memset(m_ChessInfo,0,sizeof(m_ChessInfo));
	memset(m_cbCountInfo,0,sizeof(m_cbCountInfo));

	//���½���
	Invalidate(FALSE);

	return;
}

//�����Ŀ
void CChessBorad::CleanCountInfo()
{
	//��Ŀ����
	if (m_bCountInfo==true)
	{
		//���ñ���
		m_bCountInfo=false;
		memset(m_cbCountInfo,0,sizeof(m_cbCountInfo));	

		//���½���
		if (GetSafeHwnd()!=NULL) Invalidate();
	}

	return;
}

//��������
void CChessBorad::CleanChess(WORD wDeadChess[], WORD wDeadChessCount)
{
	//���ñ���
	m_cbXPosFocus=255;
	m_cbYPosFocus=255;

	//��Ŀ����
	if (m_bCountInfo==true)
	{
		m_bCountInfo=false;
		memset(m_cbCountInfo,0,sizeof(m_cbCountInfo));	
	}

	//���ñ���
	BYTE cbXPos,cbYPos;
	for (WORD wIndex=0;wIndex<wDeadChessCount;wIndex++)
	{
		cbXPos=LOBYTE(wDeadChess[wIndex]);
		cbYPos=HIBYTE(wDeadChess[wIndex]);
		ASSERT((cbXPos<19)&&(cbYPos<19));
		m_ChessInfo[cbXPos][cbYPos].cbFlag=FALSE;
		m_ChessInfo[cbXPos][cbYPos].wIdentifier=0;
		m_ChessInfo[cbXPos][cbYPos].cbColor=NO_CHESS;
	}

	//���½���
	if (GetSafeHwnd()!=NULL) Invalidate(FALSE);

	return;
}

//���õ�Ŀ
void CChessBorad::SetChessCountInfo(BYTE cbCountInfo[19][19])
{
	//���ñ���
	m_bCountInfo=true;
	CopyMemory(m_cbCountInfo,cbCountInfo,sizeof(m_cbCountInfo));

	//���½���
	if (GetSafeHwnd()!=NULL) Invalidate(FALSE);

	return;
}

//��������
void CChessBorad::SetChess(BYTE cbXPos, BYTE cbYPos, BYTE cbColor, WORD wIdentifier)
{
	//Ч�����
	ASSERT(cbXPos<19);
	ASSERT(cbYPos<19);

	//��Ŀ����
	if (m_bCountInfo==true)
	{
		//���ñ���
		m_bCountInfo=false;
		memset(m_cbCountInfo,0,sizeof(m_cbCountInfo));	

		//���½���
		if (GetSafeHwnd()!=NULL) Invalidate();
	}

	//���ñ���
	m_ChessInfo[cbXPos][cbYPos].cbColor=cbColor;
	m_ChessInfo[cbXPos][cbYPos].wIdentifier=wIdentifier;

	//��������
	UpdateBoradPos(cbXPos,cbYPos);

	return;
}

//��������
void CChessBorad::SetChessBorad(const BYTE cbChessBorad[19][19], const WORD wIdentifier[19][19])
{
	//�����Ϣ
	m_cbXCursor=255;
	m_cbYCursor=255;
	m_cbXPosFocus=255;
	m_cbYPosFocus=255;
	m_bCountInfo=false;
	memset(m_ChessInfo,0,sizeof(m_ChessInfo));
	memset(m_cbCountInfo,0,sizeof(m_cbCountInfo));

	//���ñ���
	for (BYTE cbXPos=0;cbXPos<19;cbXPos++)
	{
		for (BYTE cbYPos=0;cbYPos<19;cbYPos++)
		{
			m_ChessInfo[cbXPos][cbYPos].cbColor=cbChessBorad[cbXPos][cbYPos];
			m_ChessInfo[cbXPos][cbYPos].wIdentifier=wIdentifier[cbXPos][cbYPos];
		}
	}

	//���½���
	if (GetSafeHwnd()!=NULL) Invalidate(FALSE);

	return;
}

//��ʾ����
void CChessBorad::ShowChessID(bool bShowChessID)
{
	if (m_bShowChessID!=bShowChessID)
	{
		m_bShowChessID=bShowChessID;
		if (GetSafeHwnd()!=NULL) Invalidate(FALSE);
	}

	return;
}

//˫������
void CChessBorad::SetDoubleMouse(bool bDoubleMouse)
{
	m_bDoubleMouse=bDoubleMouse;
	return;
}

//������Ӧ
void CChessBorad::SetSelectMode(BYTE cbSelectMode)
{
	//״̬Ч��
	if (m_cbSelectMode==cbSelectMode) return;

	//��Ӧ����
	m_cbSelectMode=cbSelectMode;
	if (m_cbSelectMode==SM_NO_SELECT)
	{
		//���ñ���
		BYTE cbXCursor=m_cbXCursor;
		BYTE cbYCursor=m_cbYCursor;
		m_cbXCursor=255,m_cbYCursor=255;

		//������ͼ
		SetCursor(LoadCursor(NULL,IDC_ARROW));
		if ((cbXCursor!=255)&&(cbYCursor!=255)) UpdateBoradPos(cbXCursor,cbYCursor);
	}

	return;
}

//���ý���
void CChessBorad::SetChessFocus(BYTE cbXPos, BYTE cbYPos)
{
	//�������
	BYTE cbXFocus=m_cbXPosFocus;
	BYTE cbYFocus=m_cbYPosFocus;

	//���ñ���
	m_cbXPosFocus=cbXPos;
	m_cbYPosFocus=cbYPos;

	//������ͼ
	if ((cbXFocus!=255)&&(cbYFocus!=255)) UpdateBoradPos(cbXFocus,cbYFocus);
	if ((m_cbXPosFocus!=255)&&(m_cbYPosFocus!=255)) UpdateBoradPos(m_cbXPosFocus,m_cbYPosFocus);

	return;
}

//��������
void CChessBorad::SetChessBorad(const WORD wBlackChess[], BYTE cbBlackCount, const WORD wWhiteChess[], BYTE cbWhiteCount)
{
	//��������
	m_cbXPosFocus=255;
	m_cbYPosFocus=255;
	memset(m_ChessInfo,0,sizeof(m_ChessInfo));

	//��������
	BYTE cbXPos=255,cbYPos=255;
	for (BYTE i=0;i<cbBlackCount;i++)
	{
		cbXPos=LOBYTE(wBlackChess[i]);
		cbYPos=HIBYTE(wBlackChess[i]);
		m_ChessInfo[cbXPos][cbYPos].cbFlag=FALSE;
		m_ChessInfo[cbXPos][cbYPos].wIdentifier=i*2+1;
		m_ChessInfo[cbXPos][cbYPos].cbColor=BLACK_CHESS;
	}
	for (BYTE i=0;i<cbWhiteCount;i++)
	{
		cbXPos=LOBYTE(wWhiteChess[i]);
		cbYPos=HIBYTE(wWhiteChess[i]);
		m_ChessInfo[cbXPos][cbYPos].cbFlag=FALSE;
		m_ChessInfo[cbXPos][cbYPos].wIdentifier=i*2+2;
		m_ChessInfo[cbXPos][cbYPos].cbColor=WHITE_CHESS;
	}

	//���½���
	Invalidate(FALSE);

	return;
}

//��ʾʤ��
void CChessBorad::ShowWinChessLine(BYTE cbXFirstPos, BYTE cbYFirstPos, BYTE cbXEndPos, BYTE cbYEndPos)
{
	//Ч�����
	ASSERT((cbXEndPos<19)&&(cbYEndPos<19));
	ASSERT((cbXFirstPos<19)&&(cbYFirstPos<19));

	//���ñ�־
	BYTE cbXPos=cbXFirstPos;
	BYTE cbYPos=cbYFirstPos;
	do
	{
		//���ñ���
		ASSERT(m_ChessInfo[cbXPos][cbYPos].cbFlag!=NO_CHESS);
		m_ChessInfo[cbXPos][cbYPos].cbFlag=TRUE;

		//�˳��ж�
		if ((cbXPos==cbXEndPos)&&(cbYPos==cbYEndPos)) break;

		//��������
		if (cbXEndPos>cbXPos) cbXPos++;
		else if (cbXEndPos<cbXPos) cbXPos--;

		//��������
		if (cbYEndPos>cbYPos) cbYPos++;
		else if (cbYEndPos<cbYPos) cbYPos--;

	} while (true);

	//���½���
	Invalidate(NULL);
		
	return;
}

//��������
void CChessBorad::UpdateBoradPos(BYTE cbXPos, BYTE cbYPos)
{
	//Ч�����
	ASSERT(cbXPos!=255);
	ASSERT(cbYPos!=255);

	//����λ��
	CRect rcChess;
	rcChess.left=GetChessXPos(cbXPos)-1;
	rcChess.top=GetChessYPos(cbYPos)-1;
	rcChess.right=rcChess.left+SPACE_BORAD_X+2;
	rcChess.bottom=rcChess.top+SPACE_BORAD_Y+2;

	//ˢ�½���
	if (GetSafeHwnd()!=NULL) InvalidateRect(&rcChess);

	return;
}

//ѡ���ж�
bool CChessBorad::EfficacySelect(BYTE cbXPos, BYTE cbYPos)
{
	//Ч�����
	ASSERT(cbXPos!=255);
	ASSERT(cbYPos!=255);

	//ѡ���ж�
	BYTE cbColor=m_ChessInfo[cbXPos][cbYPos].cbColor;
	if (m_cbSelectMode==SM_NO_SELECT) return false;
	if ((m_cbSelectMode&SM_NULL_CHESS)&&(cbColor==NO_CHESS)) return true;
	if ((m_cbSelectMode&SM_BLACK_CHESS)&&(cbColor==BLACK_CHESS)) return true;
	if ((m_cbSelectMode&SM_WHITE_CHESS)&&(cbColor==WHITE_CHESS)) return true;

	return false;
}

//��������
void CChessBorad::SwitchMousePos(const CPoint & MousePoint, BYTE & cbXPos, BYTE & cbYPos)
{
	if ((MousePoint.x>EXCURSION_X)&&(MousePoint.x<(EXCURSION_X+19*SPACE_BORAD_X))
		&&(MousePoint.y>EXCURSION_Y)&&(MousePoint.y<(EXCURSION_Y+19*SPACE_BORAD_Y)))
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
	BufferDC.SelectObject(CSkinResourceManager::GetDefaultFont());

	//�滭����
	CImageHandle BoradHandle(&m_ImageBorad);
	m_ImageBorad.BitBlt(BufferDC,0,0);

	//�滭����
	CRect rcChessID;
	TCHAR szBuffer[16];
	CImageHandle ChessHandle(&m_ImageChess);
	int nPosImage=0,nXBoradPos=0,nYBoradPos=0;
	for (BYTE cbXPos=0;cbXPos<19;cbXPos++)
	{
		for (BYTE cbYPos=0;cbYPos<19;cbYPos++)
		{
			if (m_ChessInfo[cbXPos][cbYPos].cbColor!=NO_CHESS)
			{
				//�滭����
				nXBoradPos=GetChessXPos(cbXPos);
				nYBoradPos=GetChessYPos(cbYPos);
				nPosImage=(m_ChessInfo[cbXPos][cbYPos].cbColor-1)*CHESS_WIDTH;
				if (m_ChessInfo[cbXPos][cbYPos].cbFlag==TRUE) nPosImage+=CHESS_WIDTH*2;
				else if ((cbXPos==m_cbXPosFocus)&&(cbYPos==m_cbYPosFocus)) nPosImage+=CHESS_WIDTH*2;
				m_ImageChess.AlphaDrawImage(&BufferDC,nXBoradPos,nYBoradPos,CHESS_WIDTH,CHESS_HEIGHT,nPosImage,0,RGB(255,0,255));

				//�滭����
				if (m_bShowChessID==true)
				{
					//����λ��
					rcChessID.left=nXBoradPos;
					rcChessID.top=nYBoradPos;
					rcChessID.right=rcChessID.left+CHESS_WIDTH;
					rcChessID.bottom=rcChessID.top+CHESS_HEIGHT;

					//�����Ϣ
					_snprintf(szBuffer,sizeof(szBuffer),TEXT("%d"),m_ChessInfo[cbXPos][cbYPos].wIdentifier);
					BufferDC.SetTextColor((m_ChessInfo[cbXPos][cbYPos].cbColor==WHITE_CHESS)?RGB(0,0,0):RGB(255,255,255));
					BufferDC.DrawText(szBuffer,lstrlen(szBuffer),&rcChessID,DT_SINGLELINE|DT_VCENTER|DT_CENTER);
				}
			}
			else if (m_bCountInfo==true)
			{
				//��Ŀ����
				nXBoradPos=GetChessXPos(cbXPos);
				nYBoradPos=GetChessYPos(cbYPos);
				if (m_cbCountInfo[cbXPos][cbYPos]==CT_BLACE_CHESS) nPosImage=CHESS_WIDTH*4;
				else if (m_cbCountInfo[cbXPos][cbYPos]==CT_WHITE_CHESS) nPosImage=CHESS_WIDTH*5;
				else if (m_cbCountInfo[cbXPos][cbYPos]==(CT_BLACE_CHESS|CT_WHITE_CHESS)) nPosImage=CHESS_WIDTH*6;
				else continue;
				m_ImageChess.AlphaDrawImage(&BufferDC,nXBoradPos,nYBoradPos,CHESS_WIDTH,CHESS_HEIGHT,nPosImage,0,RGB(255,0,255));
			}
			
			//�滭���
			if ((cbXPos==m_cbXCursor)&&(cbYPos==m_cbYCursor))
			{
				nXBoradPos=GetChessXPos(m_cbXCursor);
				nYBoradPos=GetChessYPos(m_cbYCursor);
				CImageHandle FrameHandle(&m_ImageFrame);
				m_ImageFrame.AlphaDrawImage(&BufferDC,nXBoradPos,nYBoradPos,RGB(255,0,255));
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
	if (m_bDoubleMouse==true) return;
	if (m_cbSelectMode==SM_NO_SELECT) return;
	if ((m_cbXCursor==255)||(m_cbYCursor==255)) return;
	if (EfficacySelect(m_cbXCursor,m_cbYCursor)==false) return;

	//������Ϣ
	GetParent()->PostMessage(WM_HIT_CHESS_BORAD,m_cbXCursor,m_cbYCursor);

	return;
}

//���˫��
void CChessBorad::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	__super::OnLButtonDblClk(nFlags, point);

	//״̬�ж�
	if (m_bDoubleMouse==false) return;
	if (m_cbSelectMode==SM_NO_SELECT) return;
	if ((m_cbXCursor==255)||(m_cbYCursor==255)) return;
	if (EfficacySelect(m_cbXCursor,m_cbYCursor)==false) return;

	//������Ϣ
	GetParent()->PostMessage(WM_HIT_CHESS_BORAD,m_cbXCursor,m_cbYCursor);

	return;
}

//�����Ϣ
BOOL CChessBorad::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	if (m_cbSelectMode!=SM_NO_SELECT)
	{
		//��ȡ���
		CPoint MousePoint;
		GetCursorPos(&MousePoint);
		ScreenToClient(&MousePoint);

		//ת������
		BYTE cbXPos=255,cbYPos=255;
		SwitchMousePos(MousePoint,cbXPos,cbYPos);

		//�滭���
		if ((m_cbXCursor!=cbXPos)||(m_cbYCursor!=cbYPos))
		{
			//���ù��
			if ((m_cbXCursor!=255)&&(m_cbYCursor!=255)) UpdateBoradPos(m_cbXCursor,m_cbYCursor);
			if ((cbXPos!=255)&&(cbYPos!=255)&&(EfficacySelect(cbXPos,cbYPos)==true)) 
			{
				m_cbXCursor=cbXPos;
				m_cbYCursor=cbYPos;
				UpdateBoradPos(m_cbXCursor,m_cbYCursor);
			}
			else
			{
				m_cbXCursor=255;
				m_cbYCursor=255;
			}
		}

		//λ���ж�
		if ((m_cbXCursor!=255)&&(m_cbYCursor!=255))
		{
			SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_ENABLE)));
			return TRUE;
		}
		else 
		{
			SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_DISABLE)));
			return TRUE;
		}
	}
	
	return __super::OnSetCursor(pWnd,nHitTest,uMessage);
}

//////////////////////////////////////////////////////////////////////////
