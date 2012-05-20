#include "StdAfx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//�����Ϣ
CNode::CNode()
{
	bXPos		 = 0 ;
	bYPos		 = 0 ;
	bToDesDis    = 0 ;	
	bToStartDis  = 0 ;
	bTotalDisDis = 0 ;
	pParent		 = NULL ;
}

CNode::CNode(int x,int y)
{
	bXPos	= x ;
	bYPos	= y ;	
	pParent = NULL;
}

CNode::~CNode()
{

}

//�������
void CNode::ComputeDistance()
{
	BYTE bXDis = 0 ,
		 bYDis = 0 ;

	bXDis = abs(bXPos - pParent->bXPos);
	bYDis = abs(bYPos - pParent->bYPos);	

	//�ԽǾ���
	if (1==bXDis && 1==bYDis)
	{
		bToStartDis = 2 + pParent->bToStartDis ;
	}
	//һ������
	else
		bToStartDis = 1 + pParent->bToStartDis;

	//�ܹ�����
	bTotalDisDis=bToStartDis+bToDesDis;

	return;
}

///////////////////////////////////////////////////////////////////////

//���캯��
CGameLogic::CGameLogic()
{
	//��������

	//���̱���										
	memset(m_ChessItem,0,sizeof(m_ChessItem));
	memset(m_ChessBorad,0,sizeof(m_ChessBorad));

	return;
}

//��������
CGameLogic::~CGameLogic()
{
}

//�ܷ��ƶ�
bool CGameLogic::AbleMove(const BYTE bChessColor , const tagChessItem ChessItem[17][17] , const BYTE bXPos , const BYTE bYPos)
{
	//���Լ���
	if(bChessColor!=ChessItem[bXPos][bYPos].bColor)		return false ;

	//����λ��
	if(CHESS_NULL==ChessItem[bXPos][bYPos].bChessID)	return false ;

	//�ڴ�Ӫ
	if(true==IsHeadquarters(bXPos , bYPos))				return false ;

	//��������
	if(CHESS_DI_LEI==ChessItem[bXPos][bYPos].bChessID)	return false ;

	return true ;
}

//�ƶ�����
bool CGameLogic::MoveChess(BYTE  const bPlayerCount , tagChessItem ChessItem[17][17] , const BYTE bSourceXPos , const BYTE bSourceYPos , const BYTE bDesXPos , const BYTE bDesYPos , bool bMove)
{
	//�Լ�����
	if(ChessItem[bSourceXPos][bSourceYPos].bColor==ChessItem[bDesXPos][bDesYPos].bColor) return false ;

	//��������
	if(CHESS_NULL==ChessItem[bSourceXPos][bSourceYPos].bChessID) return false ;

	//���ǵз�
	if(CHESS_NULL!=ChessItem[bDesXPos][bDesYPos].bChessID)
        if(false==IsEnemy(bPlayerCount , ChessItem , bSourceXPos , bSourceYPos , bDesXPos , bDesYPos)) return false ;

	//�ǽ�㴦
	if(true==NotOnNode(bDesXPos , bDesYPos)) return false ;

	//������Ӫ
	if(true==IsBarback(bDesXPos , bDesYPos) && CHESS_NULL!=ChessItem[bDesXPos][bDesYPos].bChessID) return false ;

	//��������
	if(false==IsOnBoard(bSourceXPos,bSourceYPos) || false==IsOnBoard(bDesXPos , bDesYPos)) return false ;

	if(true==bMove) m_ptPathArray.RemoveAll() ;

	int	  sign;
	POINT point ;

	//���ǹ���
	if (CHESS_GONG_BING!=ChessItem[bSourceXPos][bSourceYPos].bChessID)
	{
		//ֻ��һ��
		if ((abs(bSourceYPos-bDesYPos)==1 && abs(bSourceXPos-bDesXPos)==0) || (abs(bSourceXPos-bDesXPos)==1 && abs(bSourceYPos-bDesYPos)==0))
		{
			//����λ��
			if (true==bMove)
			{	
				point.x = bDesXPos ;
				point.y = bDesYPos ;
				m_ptPathArray.Add(point);

				point.x = bSourceXPos ;
				point.y = bSourceYPos ;
				m_ptPathArray.Add(point);

			}
			return true ;
		}
		//�Ƿ����
		else if ( AbleTurn(bSourceXPos , bSourceYPos) && AbleTurn(bDesXPos , bDesYPos) && abs(bSourceYPos-bDesYPos)==1 && abs(bSourceXPos-bDesXPos)==1 )
		{
			//����λ��
			if (true==bMove)
			{
				point.x = bDesXPos ;
				point.y = bDesYPos ;
				m_ptPathArray.Add(point);

				point.x = bSourceXPos ;
				point.y = bSourceYPos ;
				m_ptPathArray.Add(point);
			}
			return true ;
		}
		//���һ��
		else if (0==bSourceXPos || 16==bSourceXPos || 0==bSourceYPos || 16 ==bSourceYPos)
		{
			if ( (bSourceXPos == bDesXPos && abs(bSourceYPos-bDesYPos) == 1 ) || (bSourceYPos == bDesYPos && abs(bSourceXPos-bDesXPos) == 1) )			 
			{
				//����λ��
				if (true==bMove)
				{
					point.x = bDesXPos ;
					point.y = bDesYPos ;
					m_ptPathArray.Add(point);

					point.x = bSourceXPos ;
					point.y = bSourceYPos ;
					m_ptPathArray.Add(point);
				}
				return true ;
			}
			return false ;
		}
		//���һ��
		else if (0 == bDesXPos || 16 == bDesXPos)
		{
			if (abs(bSourceXPos-bDesXPos)==1 && bSourceYPos == bDesYPos)
			{
				//����λ��
				if (true==bMove)
				{
					point.x = bDesXPos ;
					point.y = bDesYPos ;
					m_ptPathArray.Add(point);

					point.x = bSourceXPos ;
					point.y = bSourceYPos ;
					m_ptPathArray.Add(point);
				}
				return true;
			}
			
			return false;
		}
		else if (0==bDesYPos || 16==bDesYPos)
		{
			if (abs(bSourceYPos - bDesYPos)==1 && bSourceXPos==bDesXPos)
			{
				//����λ��
				if (true==bMove)
				{
					point.x = bDesXPos ;
					point.y = bDesYPos ;
					m_ptPathArray.Add(point);

					point.x = bSourceXPos ;
					point.y = bSourceYPos ;
					m_ptPathArray.Add(point);
				}
				return true ;
			}
			return false ;
		}
		//������·
		else if ( NotOnRail(bSourceXPos , bSourceYPos) )
		{
			if ( abs(bSourceYPos - bDesYPos) > 1 || abs(bSourceXPos-bDesXPos) > 1 )
				return false;
			else if (IsBarback(bSourceXPos , bSourceYPos))
			{
				//����λ��
				if (true==bMove)
				{
					point.x = bDesXPos ;
					point.y = bDesYPos ;
					m_ptPathArray.Add(point);

					point.x = bSourceXPos ;
					point.y = bSourceYPos ;
					m_ptPathArray.Add(point);
				}
				return true ;
			}
			//ֻ��һ��
			else if (((bSourceYPos == bDesYPos) && abs(bSourceXPos-bDesXPos)==1) || ((bSourceXPos==bDesXPos) && abs(bSourceYPos - bDesYPos)==1) )
			{
				//����λ��
				if (true==bMove)
				{
					point.x = bDesXPos ;
					point.y = bDesYPos ;
					m_ptPathArray.Add(point);

					point.x = bSourceXPos ;
					point.y = bSourceYPos ;
					m_ptPathArray.Add(point);
				}
				return true ;
			}
			return false ;
		} 
		//������·
		else if ( NotOnRail(bDesXPos , bDesYPos) )
		{
			if ( abs(bSourceYPos - bDesYPos) > 1 || abs(bSourceXPos-bDesXPos) > 1 )
				return false;
			else if (IsBarback(bDesXPos , bDesYPos))				
			{
				//����λ��
				if (true==bMove)
				{
					point.x = bDesXPos ;
					point.y = bDesYPos ;
					m_ptPathArray.Add(point);

					point.x = bSourceXPos ;
					point.y = bSourceYPos ;
					m_ptPathArray.Add(point);
				}
				return true ;
			}
			else if (((bSourceYPos == bDesYPos) && abs(bSourceXPos-bDesXPos)==1) || ((bSourceXPos==bDesXPos) && abs(bSourceYPos - bDesYPos)==1) )
			{
				//����λ��
				if (true==bMove)
				{
					point.x = bDesXPos ;
					point.y = bDesYPos ;
					m_ptPathArray.Add(point);

					point.x = bSourceXPos ;
					point.y = bSourceYPos ;
					m_ptPathArray.Add(point);
				}
				return true ;
			}
			return false ;
		} 
		else if ( (5==bSourceXPos || 11==bSourceXPos ) && (7==bSourceYPos || 9==bSourceYPos) )
		{	
			if (bSourceXPos==bDesXPos)
			{
				if(bDesYPos<bSourceYPos)
				{
					//�м���
					for(int y=bDesYPos ; y<=bSourceYPos ; ++y)
					{
						//����λ��
						if (true==bMove)
						{
							point.x = bSourceXPos ;
							point.y = y ;
							m_ptPathArray.Add(point);
						}

						if(y!=bDesYPos && y!=bSourceYPos)
                            if(CHESS_NULL!=ChessItem[bSourceXPos][y].bChessID)
							{
								//������
								m_ptPathArray.RemoveAll() ;
								return false ;
							}
					}
	
				}
				else if(bDesYPos>bSourceYPos)
				{
					for(int y=bDesYPos ; y>=bSourceYPos ; --y)
					{
						//����λ��
						if(true==bMove)
						{
							point.x = bSourceXPos ;
							point.y = y ;
							m_ptPathArray.Add(point);
						}
						if(y!=bDesYPos && y!=bSourceYPos)
							if(CHESS_NULL!=ChessItem[bSourceXPos][y].bChessID)
							{
								//������
								m_ptPathArray.RemoveAll() ;
								return false ;
							}
					}
				}
				return true ;
			}
			return false ;
		}
		else if ( (11==bSourceYPos || 5==bSourceYPos ) && (7==bSourceXPos || 9==bSourceXPos)  )
		{
			if (bSourceYPos == bDesYPos)
			{
				//����λ��
				if (bDesXPos<bSourceXPos)
				{

					for (int x=bDesXPos ; x<=bSourceXPos ; x++)
					{
						//����λ��
						if(true==bMove)
						{
							point.y=bSourceYPos;
							point.x=x;
							m_ptPathArray.Add(point);
						}
						if(x!=bDesXPos && x!=bSourceXPos)
							if(CHESS_NULL!=ChessItem[x][bSourceYPos].bChessID)
							{
								//������
								m_ptPathArray.RemoveAll() ;
								return false ;
							}
					}					
				}
				else if (bDesXPos>bSourceXPos)
				{					
					for (int x=bDesXPos ; x>=bSourceXPos && 255!=x ; x--)
					{
						//����λ��
						if(true==bMove)
						{
							point.y=bSourceYPos;
							point.x=x;
							m_ptPathArray.Add(point);
						}
						if(x!=bDesXPos && x!=bSourceXPos)
							if(CHESS_NULL!=ChessItem[x][bSourceYPos].bChessID)
							{
								//������
								m_ptPathArray.RemoveAll() ;
								return false ;
							}
					}					
				}

				return true ;
			}
			
			return false ;
		}
		else if (bSourceXPos==bDesXPos && bDesYPos>0 && bDesYPos<16 )
		{
			if ( bDesYPos > bSourceYPos )
			{				
				for (int y=bDesYPos ; y>=bSourceYPos ; y--)
				{
					if (true==bMove)
					{
						point.x=bSourceXPos;
						point.y=y;
						m_ptPathArray.Add(point);
					}

					if(y!=bDesYPos && y!=bSourceYPos)
                        if (CHESS_NULL!=ChessItem[bSourceXPos][y].bChessID)
						{
							//������
							m_ptPathArray.RemoveAll() ;
							return false ;
						}
				}
				return true ;
			}
			else if (bDesYPos < bSourceYPos)
			{
				for (int y=bDesYPos ; y<=bSourceYPos ; y++)
				{
					if (true==bMove)
					{
						point.x=bSourceXPos;
						point.y=y;
						m_ptPathArray.Add(point);
					}
					if(y!=bDesYPos && y!=bSourceYPos)
						if (CHESS_NULL!=ChessItem[bSourceXPos][y].bChessID)
						{
							//������
							m_ptPathArray.RemoveAll() ;
							return false ;
						}
				}
				return true ;
			}
		}
		else if (bSourceYPos==bDesYPos && bDesXPos>0 && bDesXPos<16 )
		{
			if ( bDesXPos > bSourceXPos )
			{
				for (int x=bDesXPos ; x>=bSourceXPos ; x--)
				{
					if (true==bMove)
					{
						point.y=bSourceYPos;
						point.x=x;
						m_ptPathArray.Add(point);
					}
					if(x!=bDesXPos && x!=bSourceXPos)
                        if (CHESS_NULL!=ChessItem[x][bSourceYPos].bChessID)
						{
							m_ptPathArray.RemoveAll() ;
							return false ;
						}						
				}
				return true;
			}
			else if (bDesXPos < bSourceXPos)
			{
				for (int x=bDesXPos ; x<=bSourceXPos ; x++)
				{
					if (true==bMove)
					{
						point.y=bSourceYPos;
						point.x=x;
						m_ptPathArray.Add(point);
					}
					if(x!=bDesXPos && x!=bSourceXPos)
						if (CHESS_NULL!=ChessItem[x][bSourceYPos].bChessID)
						{
							m_ptPathArray.RemoveAll() ;
							return false ;
						}					
				}				
				return true ;
			}
		}
		//���ӹ���
		else if ( (bSourceXPos==6  && bSourceYPos>=bDesYPos && bDesXPos<=bSourceXPos && bDesYPos==10 ) ||	//��ת����
				  (bSourceXPos==6  && bSourceYPos<=bDesYPos && bDesXPos<=bSourceXPos && bDesYPos==6  ) ||	//��ת����
				  (bSourceXPos==10 && bSourceYPos>=bDesYPos && bDesXPos>=bSourceXPos && bDesYPos==10 ) ||	//��ת����
				  (bSourceXPos==10 && bSourceYPos<=bDesYPos && bDesXPos>=bSourceXPos && bDesYPos==6  ) )	//��ת����
		{
			int i,j,k;
			
			//ת������
			if ( bSourceYPos!=bDesYPos && ((bDesXPos>6 && bDesXPos<10 )|| ((bDesXPos==6 || bDesXPos==10) && bSourceXPos!=bDesXPos))) 
				return false ;

			//�������
			if (bDesXPos<6)
			{	
				i=5;
				k=abs(bDesXPos-i);
			}
			//�������
			if (bDesXPos>10 )
			{
				i=11;
				k=abs(bDesXPos-i);
			}
			if (k==0)
				sign=0;
			else
				sign=(bDesXPos-i)/abs(bDesXPos-i);
			if (true==bMove)
			{
				point.x = bDesXPos ;
				point.y = bDesYPos ;
				m_ptPathArray.Add(point);
			}
			i=bDesXPos;
			//��������
			for (j=0;j<k;j++)
			{				
				i=i-sign;
				if (true==bMove)
				{
					point.x=i;
					point.y=bDesYPos;
					m_ptPathArray.Add(point);
				}
				if (i>=0 && i<17 && CHESS_NULL!=ChessItem[i][bDesYPos].bChessID)
				{
					m_ptPathArray.RemoveAll() ;
					return false ;
				}

			}			

			if (bSourceYPos<6)
			{
				i=5;
				k=abs(bSourceYPos-i);
			}
			if (bSourceYPos>10)
			{
				i=11;			
				k=abs(bSourceYPos-i);
			}
			if (k==0)
				sign=0;
			else
				sign=abs(bSourceYPos-i)/(bSourceYPos-i);

			//�������� 
			for (j=0;j<k;j++)
			{
				if (true==bMove)
				{
					point.x=bSourceXPos;
					point.y=i;
					m_ptPathArray.Add(point);
				}
				if (i>=0 && i<17 && CHESS_NULL!=ChessItem[bSourceXPos][i].bChessID)
				{
					m_ptPathArray.RemoveAll() ;
					return false ;
				}
				i=i+sign;

			}
			if (true==bMove)
			{
				point.x = bSourceXPos ;
				point.y = bSourceYPos ;
				m_ptPathArray.Add(point);
			}
			return true ;
		}
		//���ӹ���
		else if ( (bSourceYPos==6  && bSourceYPos>=bDesYPos && bDesXPos<=bSourceXPos && bDesXPos==10 ) ||	//��ת����
				  (bSourceYPos==6  && bSourceYPos>=bDesYPos	&& bDesXPos>=bSourceXPos && bDesXPos==6 )  ||	//��ת����
				  (bSourceYPos==10 && bSourceYPos<=bDesYPos	&& bDesXPos<=bSourceXPos && bDesXPos==10 ) ||	//��ת����
				  (bSourceYPos==10 && bSourceYPos<=bDesYPos	&& bDesXPos>=bSourceXPos && bDesXPos==6  ) )	//��ת����
		{
			int i,j,k;

			//ת������ 
			if (bSourceXPos!=bDesXPos && ((bDesYPos>=6 && bDesYPos<=10) ||((bDesYPos==6 || bDesYPos==10) && bDesYPos!=bSourceYPos)))
				return false ;
			if (bDesYPos<6)
			{
				i=5;
				k=abs(bDesYPos-i);
			}
			if (bDesYPos>10)
			{
				i=11;		
				k=abs(bDesYPos-i);
			}
			if (k==0)
				sign=0;
			else
				sign=(bDesYPos-i)/abs(bDesYPos-i);
			if (true==bMove)
			{
				point.x = bDesXPos ;
				point.y = bDesYPos ;
				m_ptPathArray.Add(point);
			}
			i=bDesYPos;
			//��������
			for (j=0;j<k;j++)
			{	
				i=i-sign;
				if (true==bMove)
				{
					point.y=i;
					point.x=bDesXPos ;
					m_ptPathArray.Add(point);
				}
				if (i>=0 && i<17 && CHESS_NULL!=ChessItem[bDesXPos][i].bChessID)
				{
					m_ptPathArray.RemoveAll() ;
					return false;
				}

			}			
			if (bSourceXPos<6)
			{
				i=5;
				k=abs(bSourceXPos-i);
			}

			if (bSourceXPos>10)
			{
				i=11;			
				k=abs(bSourceXPos-i);
			}

			//��������
			for (j=0;j<k;j++)
			{	
				if (true==bMove)
				{
					point.y=bSourceYPos;
					point.x=i;
					m_ptPathArray.Add(point);
				}
				if (i>=0 && i<17 && CHESS_NULL!=ChessItem[i][bSourceYPos].bChessID)
				{
					m_ptPathArray.RemoveAll() ;
					return false ;
				}

				i=i+abs(bSourceXPos-i)/(bSourceXPos-i);

			}
			if (true==bMove)
			{
				point.x = bSourceXPos ;
				point.y = bSourceYPos ;
				m_ptPathArray.Add(point);
			}
			return true;

		}
	}
	//��������
	else
	{	
		//���һ��
		if (bSourceXPos == 0 || bSourceXPos == 16 || bSourceYPos == 0 || bSourceYPos == 16)
		{			
				//ֻ��һ��
			if ( (bSourceXPos == bDesXPos && abs(bSourceYPos-bDesYPos) == 1 ) || (bSourceYPos == bDesYPos && abs(bSourceXPos-bDesXPos) == 1) )			
			{
				//��������
				if (true==bMove)
				{
					point.x = bDesXPos ;
					point.y = bDesYPos ;
					m_ptPathArray.Add(point);

					point.x = bSourceXPos ;
					point.y = bSourceYPos ;
					m_ptPathArray.Add(point);
				}
				return true ;
			}
			return false ;
		}
		else if (bDesXPos==0 || bDesXPos==16)
		{			
			
			if (abs(bSourceXPos-bDesXPos)==1 && bSourceYPos==bDesYPos)
			{
				if (true==bMove)
				{
					point.x = bDesXPos ;
					point.y = bDesYPos ;
					m_ptPathArray.Add(point);

					point.x = bSourceXPos ;
					point.y = bSourceYPos ;
					m_ptPathArray.Add(point);
				}
				return true ;
			}
			return false ;
		}
		else if (bDesYPos==0 || bDesYPos==16)
		{			
			if (abs(bSourceYPos-bDesYPos)==1 && bSourceXPos==bDesXPos)
			{
				if (true==bMove)
				{
					point.x = bDesXPos ;
					point.y = bDesYPos ;
					m_ptPathArray.Add(point);

					point.x = bSourceXPos ;
					point.y = bSourceYPos ;
					m_ptPathArray.Add(point);
				}
				return true;
			}
			return false;
		}	
		//�ƶ�һ��
		else if (( abs(bSourceXPos-bDesXPos)==1 && abs(bSourceYPos-bDesYPos)==0 ) || (abs(bSourceYPos-bDesYPos)==1 && abs(bSourceXPos-bDesXPos)==0) )
		{			
			if (true==bMove)
			{
				point.x = bDesXPos ;
				point.y = bDesYPos ;
				m_ptPathArray.Add(point);

				point.x = bSourceXPos ;
				point.y = bSourceYPos ;
				m_ptPathArray.Add(point);
			}
			return true ;

		}
		//������·
		else if ( NotOnRail(bSourceXPos , bSourceYPos) )
		{			
			if (true==bMove)
			{
				point.x = bDesXPos ;
				point.y = bDesYPos ;
				m_ptPathArray.Add(point);

				point.x = bSourceXPos ;
				point.y = bSourceYPos ;
				m_ptPathArray.Add(point);
			}
			if ( abs(bSourceYPos-bDesYPos) > 1 || abs(bSourceXPos-bDesXPos) > 1 )
			{
				m_ptPathArray.RemoveAll() ;
				return false ;
			}
			else if (IsBarback(bSourceXPos,bSourceYPos))				
			{
				return true ;
			}
			else if (((bSourceYPos==bDesYPos) && abs(bSourceXPos-bDesXPos)==1) || ((bSourceXPos==bDesXPos) && abs(bSourceYPos-bDesYPos)==1) )
			{
				return true ;
			}
			else
			{
				m_ptPathArray.RemoveAll() ;
				return false ;
			}
		} 
		//������·
		else if ( NotOnRail(bDesXPos , bDesYPos) )
		{			
			if (true==bMove)
			{
				point.x = bDesXPos ;
				point.y = bDesYPos ;
				m_ptPathArray.Add(point);

				point.x = bSourceXPos ;
				point.y = bSourceYPos ;
				m_ptPathArray.Add(point);
			}
			if ( abs(bSourceYPos-bDesYPos) > 1 || abs(bSourceXPos-bDesXPos) > 1 )
			{
				m_ptPathArray.RemoveAll() ;
				return false ;
			}
			else if (IsBarback(bDesXPos , bDesYPos))				
			{
				return true ;
			}
			else if (((bSourceYPos==bDesYPos) && abs(bSourceXPos-bDesXPos)==1) || ((bSourceXPos==bDesXPos) && abs(bSourceYPos-bDesYPos)==1) )
			{
				return true ;
			}
			else
			{
				m_ptPathArray.RemoveAll() ;
				return false ;
			}
		} 
		POINT ptDest , ptStart ;
		ptDest.x  = bDesXPos ;
		ptDest.y  = bDesYPos ;
		ptStart.x = bSourceXPos ;
		ptStart.y = bSourceYPos ;

		//����Ŀ��
		tagChessItem TargetChessItem=ChessItem[bDesXPos][bDesYPos] ;
		ChessItem[bDesXPos][bDesYPos].bChessID = CHESS_NULL ;

		bool bFind = FindPath(ptDest , ptStart , ChessItem , bMove) ;
		//�ָ�Ŀ��
		ChessItem[bDesXPos][bDesYPos] = TargetChessItem ;

		return bFind ;
	}

	return false ;
}

//��������
bool CGameLogic::SwapChess(tagChessItem ChessItem[17][17] , BYTE bSourceX , BYTE bSourceY , BYTE bDesX , BYTE bDesY , bool bSwapChess) 
{
	//��������
	if (CHESS_NULL!=ChessItem[bSourceX][bSourceY].bChessID &&
		CHESS_NULL!=ChessItem[bDesX][bDesY].bChessID &&
		ChessItem[bSourceX][bSourceY].bColor==ChessItem[bDesX][bDesY].bColor)
	{			
		if ((CHESS_JUN_QI==ChessItem[bSourceX][bSourceY].bChessID ||
			 CHESS_JUN_QI==ChessItem[bDesX][bDesY].bChessID)      &&
			 bSourceY!=bDesY)
		{
			return false;
		}

		//���ھ�Ӫ
		if ((CHESS_JUN_QI==ChessItem[bSourceX][bSourceY].bChessID ||
			CHESS_JUN_QI==ChessItem[bDesX][bDesY].bChessID)       &&
			(abs(bSourceX-bDesX)!=2))
		{
			return false;
		}

		//����λ��
		if (CHESS_DI_LEI==ChessItem[bSourceX][bSourceY].bChessID &&	bDesY<15)
		{
			return false;			
		}

		//����λ��
		if (CHESS_DI_LEI==ChessItem[bDesX][bDesY].bChessID && bSourceY<15 )
		{			
			return false;
		}

		//ը��λ��
		if (CHESS_ZHA_DAN==ChessItem[bSourceX][bSourceY].bChessID && bDesY<12 )
		{
			return	false ;
		}

		//ը��λ��
		if (CHESS_ZHA_DAN==ChessItem[bDesX][bDesY].bChessID	&& bSourceY<12 )
		{
			return false;
		}

		if(true==bSwapChess)
		{
			//��������
			tagChessItem tmpChess ;

			tmpChess = ChessItem[bSourceX][bSourceY] ;
			ChessItem[bSourceX][bSourceY] = ChessItem[bDesX][bDesY] ;
			ChessItem[bDesX][bDesY] = tmpChess ;
		}
		return true;

	}
	else
		return false ;

	return false ;
}

//�Ƿ��Ӫ
bool CGameLogic::IsBarback(BYTE bXPos , BYTE bYPos)
{
	ASSERT(bXPos>=0 && bXPos<=16 && bYPos>=0 && bYPos<=16) ;
	if(bXPos<0 || bXPos>16 || bYPos<0 || bYPos>16) return false ;

	if(7==bXPos  && (2==bYPos  || 4==bYPos)  || 8==bXPos  && 3==bYPos  || 9==bXPos  && (2==bYPos  || 4==bYPos)  || //��0���
	   2==bXPos  && (7==bYPos  || 9==bYPos)  || 3==bXPos  && 8==bYPos  || 4==bXPos  && (7==bYPos  || 9==bYPos)  || //��1���
	   7==bXPos  && (12==bYPos || 14==bYPos) || 8==bXPos  && 13==bYPos || 9==bXPos  && (12==bYPos || 14==bYPos) || //��2���
	   12==bXPos && (7==bYPos  || 9==bYPos)  || 13==bXPos && 8==bYPos  || 14==bXPos && (7==bYPos  || 9==bYPos)     //��3���
	   )
	   return true ;

	return false ;
}

//�Ƿ�з�
bool CGameLogic::IsEnemy(BYTE bPlayerCount , const tagChessItem ChessItem[17][17] , const BYTE bSourceXPos , const BYTE bSourceYPos , const BYTE bDesXPos , const BYTE bDesYPos)
{
	if(ChessItem[bSourceXPos][bSourceYPos].bColor == ChessItem[bDesXPos][bDesYPos].bColor) return false ;

	if(4==bPlayerCount)
	{
		if((2+ChessItem[bSourceXPos][bSourceYPos].bColor)%4==ChessItem[bDesXPos][bDesYPos].bColor || (2+ChessItem[bSourceXPos][bSourceYPos].bColor)==ChessItem[bDesXPos][bDesYPos].bColor)
			return false ;
	}

	return true ;
}

//�Ƿ����
bool CGameLogic::AbleTurn(BYTE const bXPos , BYTE const bYPos)
{
	if ( (bXPos==6   &&  bYPos==5  ) ||
		 (bXPos==5   &&  bYPos==6  ) ||
		 (bXPos==10  &&  bYPos==5  ) ||
		 (bXPos==11  &&  bYPos==6  ) ||
		 (bXPos==5   &&  bYPos==10 ) ||
		 (bXPos==6   &&  bYPos==11 ) ||
		 (bXPos==10  &&  bYPos==11 ) ||
		 (bXPos==11  &&  bYPos==10 ) )
	{
		return true;
	}
	return false;
}

//������·
bool CGameLogic::NotOnRail(BYTE const bXPos , BYTE const bYPos)
{
	if ((bXPos>=2  && bXPos<=4  && bYPos>=7 && bYPos<=9) ||
		(bYPos>=2  && bYPos<=4  && bXPos>=7 && bXPos<=9) ||
		(bXPos>=12 && bXPos<=14 && bYPos>=7 && bYPos<=9) ||
		(bYPos>=12 && bYPos<=14 && bXPos>=7 && bXPos<=9) )
        return true ;

	return false ;
}

//�Ƚ�����
enCompareResult CGameLogic::CompareChess(const tagChessItem ChessItem[17][17] , const BYTE bSourceXPos , const BYTE bSourceYPos , const BYTE bDesXPos , const BYTE bDesYPos)
{
	if(CHESS_NULL==ChessItem[bSourceXPos][bSourceYPos].bChessID) 
		return enError ;

	else if(CHESS_NULL==ChessItem[bDesXPos][bDesYPos].bChessID)
		return enLarge ;
	
	else if (CHESS_ZHA_DAN==ChessItem[bDesXPos][bDesYPos].bChessID)
		return enEqual;

	else if (CHESS_ZHA_DAN==ChessItem[bSourceXPos][bSourceYPos].bChessID)
		return enEqual;

	else if(CHESS_DI_LEI==ChessItem[bDesXPos][bDesYPos].bChessID && CHESS_GONG_BING!=ChessItem[bSourceXPos][bSourceYPos].bChessID)
		return enSmall ;

	else if (CHESS_GONG_BING==ChessItem[bSourceXPos][bSourceYPos].bChessID && CHESS_DI_LEI==ChessItem[bDesXPos][bDesYPos].bChessID)
		return enLarge;

	if ((ChessItem[bDesXPos][bDesYPos].bChessID	> ChessItem[bSourceXPos][bSourceYPos].bChessID))
		return enSmall;

	else if (ChessItem[bDesXPos][bDesYPos].bChessID	== ChessItem[bSourceXPos][bSourceYPos].bChessID)
		return enEqual;

	else if(ChessItem[bSourceXPos][bSourceYPos].bChessID > ChessItem[bDesXPos][bDesYPos].bChessID)
		return enLarge ;

	return enError ;
}

//�ڴ�Ӫ
bool CGameLogic::IsHeadquarters(BYTE bXPos , BYTE bYPos)
{
	if((7==bXPos || 9==bXPos)  && (0==bYPos || 16==bYPos) ||
	   (0==bXPos || 16==bXPos) && (7==bYPos || 9==bYPos))
	   return true ;

	return false ;
}

//�ǽ�㴦
bool CGameLogic::NotOnNode(BYTE bXPos , BYTE bYPos) 
{
	if ( (bXPos==6  &&  bYPos==9 ) ||
		 (bXPos==6  &&  bYPos==7 ) ||
		 (bXPos==7  &&  bYPos==6 ) ||
		 (bXPos==7  &&  bYPos==8 ) ||
		 (bXPos==7  &&  bYPos==10) ||
		 (bXPos==9  &&  bYPos==6 ) ||
		 (bXPos==9  &&  bYPos==8 ) ||
		 (bXPos==9  &&  bYPos==10) ||
		 (bXPos==8  &&  bYPos==7 ) ||
		 (bXPos==8  &&  bYPos==9 ) ||
		 (bXPos==10 &&  bYPos==7 ) ||
		 (bXPos==10 &&  bYPos==9 ) ||
		 (bXPos==7  &&  bYPos==7 ) || 
		 (bXPos==9  &&  bYPos==7 ) ||
		 (bXPos==7  &&  bYPos==9 ) ||
		 (bXPos==9  &&  bYPos==9))
	{
		return true;
	}
	return false;
}

//�Ƿ�����
bool CGameLogic::IsOnBoard(BYTE bXPos , BYTE bYPos)
{
	if( (bXPos>=6  && bXPos<=10 && bYPos>=0  && bYPos<=5)  ||
		(bXPos>=11 && bXPos<=16 && bYPos>=6  && bYPos<=10) ||
		(bXPos>=6  && bXPos<=10 && bYPos>=11 && bYPos<=16) ||
		(bXPos>=0  && bXPos<=5  && bYPos>=6  && bYPos<=10) ||
		(bXPos>=6  && bXPos<=10 && bYPos>=6  && bYPos<=10  && !NotOnNode(bXPos,bYPos))
		)
		return true ;
	return false ;
}

//�ɷ���
bool CGameLogic::CanCarry(BYTE const bChessID , BYTE const bChessColor , const tagChessItem ChessItem[17][17])
{
	BYTE bMinChessID=bChessID ;
	for(BYTE x=0 ; x<17 ; ++x)
		for(BYTE y=0 ; y<17 ; ++y)
		{
			if(bChessColor==ChessItem[x][y].bColor)
				if(CHESS_JUN_QI!=ChessItem[x][y].bChessID && CHESS_DI_LEI!=ChessItem[x][y].bChessID && CHESS_ZHA_DAN!=ChessItem[x][y].bChessID && CHESS_NULL!=ChessItem[x][y].bChessID && !IsHeadquarters(x,y))
					if(bMinChessID>ChessItem[x][y].bChessID)
						bMinChessID=ChessItem[x][y].bChessID ;
		}
	return bMinChessID==bChessID ;
}

//û������
bool CGameLogic::NoChess(BYTE const bChessColor , const tagChessItem ChessItem[17][17]) 
{
	for(BYTE x=0 ; x<17 ; ++x)
		for(BYTE y=0 ; y<17 ; ++y)
		{
			if(bChessColor==ChessItem[x][y].bColor && CHESS_NULL!=ChessItem[x][y].bChessID && !IsHeadquarters(x,y) && CHESS_DI_LEI!=ChessItem[x][y].bChessID)
				return false ;
		}
	return true ;
}



//��������
CString CGameLogic::IDToName(BYTE bChessID) 
{
	switch(bChessID)
	{
	case CHESS_SI_LING :
		return "˾��" ;

	case CHESS_JUN_ZHANG :
		return "����" ;

	case CHESS_SHI_ZHANG :
		return "ʦ��" ;

	case CHESS_LV_ZHANG :
		return "�ó�" ;

	case CHESS_TUAN_ZHANG :
		return "�ų�" ;

	case CHESS_YING_ZHANG :
		return "Ӫ��" ;

	case CHESS_LIAN_ZHANG :
		return "����" ;

	case CHESS_PAI_ZHANG :
		return "�ų�" ;

	case CHESS_GONG_BING :
		return "����";

	case CHESS_ZHA_DAN :
		return "ը��" ;

	case CHESS_DI_LEI :
		return "����" ;

	case CHESS_JUN_QI :
		return "����";

	case CHESS_NULL :
		return "û��";

	default:
		return "����" ;
	}
}








////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//��������


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Ѱ��·��
bool CGameLogic::FindPath(POINT ptDest,POINT ptStart , tagChessItem ChessItem[17][17] , bool bMove)
{
	//�ĸ�����
	Init(ptDest,ptStart,ChessItem);

	int nXDirection[8]={1,1,0,-1,-1,-1,0,1};
	int nYDirection[8]={0,-1,-1,-1,0,1,1,1};
	int x,y;
	CString strOut,strBuffer;
	POSITION pTemp;	
	m_bStop=false;

	while (!m_bStop)
	{	
		pTemp=NULL;

		//��С���
		GetMinDisNode(&m_ListCanPassNode,pTemp);							
		if (!pTemp)			
		{
			m_bStop=true;
			m_bFind=false;
			ClearList();
			return m_bFind;	
		}

		m_pCurNode=(CNode * )m_ListCanPassNode.GetAt(pTemp);

		m_ListCanPassNode.RemoveAt(pTemp);
		m_ListHaveGoneNode.AddTail((void *)m_pCurNode);

		int	 count=0;
		BOOL bEmpty;

		for (int i=0 ; i<8 ; i++)
		{

			x = m_pCurNode->bXPos + nXDirection[i] ;
			y = m_pCurNode->bYPos + nYDirection[i] ;


			if (x<0 || y<0 || x>=17 || y>=17)
				continue;
			//��������
			if(! ((x>=6  && x<=10 && y>=1  && y<=5)  ||
				  (x>=11 && x<=15 && y>=6  && y<=10) ||
				  (x>=6  && x<=10 && y>=11 && y<=15) ||
				  (x>=1  && x<=5  && y>=6  && y<=10) ||
				  (x>=6  && x<=10 && y>=6  && y<=10)
				))
				continue ;

			//������·
			if(true==NotOnRail(x,y)) 
				continue ;

			//��������
			if((7==x && 7==y) || (7==x && 9==y) || (9==x && 7==y) || (9==x && 9==y))
				continue ;

			//��Ӫλ��
			if(IsBarback(x,y))
				continue ;

			//��������
			if (CHESS_NULL!=ChessItem[x][y].bChessID)
				continue;
			   
			//б�ߣ������
			if ( i%2!=0 && (!AbleTurn(x,y) || !AbleTurn(m_pCurNode->bXPos,m_pCurNode->bYPos) ) )
				continue;

			//��������
			if  ((6==x  && (7==y || 9==y)  && 0==i)	||	//�����ߵ��ǽ��
				((7==x  || 9==x) && 10==y  && 2==i)	||	//�����ߵ��ǽ��
				(10==x  && (7==y || 9==y)  && 4==i)	||	//�����ߵ��ǽ��
				((7==x  || 9==x) && 6==y   && 6==i))	//�����ߵ��ǽ��
				continue;

			if ((5==x  && (7==y  || 9==y ) && 4==i)||	//�ӷǽ��������
				((7==x || 9==x)  && 5==y   && 2==i)||	//�ӷǽ��������
				(11==x && (7==y  || 9==y)  && 0==i)||	//�ӷǽ��������
				((7==x || 9==x)  && 11==y  && 6==i))	//�ӷǽ��������
				continue;

			bEmpty=m_ListCanPassNode.IsEmpty();

			POSITION pInOpen,pInClose;
			pInOpen=pInClose=NULL;

			FinPos(m_ListCanPassNode,x,y,pInOpen);

			count=(int)(m_ListHaveGoneNode.GetCount());		
			if (0!=count)
			{
				FinPos(m_ListHaveGoneNode,x,y,pInClose);
			}

			//�Ѿ��߹�
			if (pInClose)
			{
				continue;
			}	

			//û���߹�
			if ( pInOpen==NULL)
			{					
				CNode * pNewCell=new CNode(x,y);

				pNewCell->pParent=m_pCurNode;				
				pNewCell->bToDesDis=ComputeNodeDis(m_bXDest,m_bYDest,x,y);
				pNewCell->ComputeDistance();
				m_ListCanPassNode.AddHead((void *)pNewCell);				

			}		
			else 
			{
				CNode * pExist=(CNode *)m_ListCanPassNode.GetAt(pInOpen);				
				BYTE tempG=ComputeToStartDis(*pExist,*m_pCurNode);
				//���¾��� 
				if (pExist->bToStartDis>tempG )
				{					
					pExist->pParent=m_pCurNode;
					pExist->bToStartDis=tempG;
					pExist->bTotalDisDis=pExist->bToStartDis+pExist->bToDesDis;	
				}
			}
			//�ҵ�Ŀ��
			if (x==m_bXDest && y==m_bYDest )					
			{
				m_bStop=true;
				m_bFind=true;
				m_pEndNode=(CNode *)m_ListCanPassNode.GetHead();

				if(true==bMove)
				{
					m_ptPathArray.RemoveAll() ;
					POINT point ;

					//����·��
					for (CNode *pNode=m_pEndNode ; pNode!=NULL ; pNode=pNode->pParent)
					{
						point.x = pNode->bXPos ;
						point.y = pNode->bYPos ;
						m_ptPathArray.Add(point) ;
					}
				}

				//�������
				ClearList() ;

				return m_bFind;	
			}
		}
	}
	//�������
	ClearList() ;

	return m_bFind;
}

//������
BYTE CGameLogic::ComputeToStartDis(CNode &cellA, CNode &cellB)
{
	BYTE bXPos , bYPos ;

	bXPos = abs(cellA.bXPos - cellB.bXPos);
	bYPos = abs(cellA.bYPos - cellB.bYPos);

	//�ԽǾ���
	if (1==bXPos && 1==bYPos)
	{
		return 2+cellB.bToStartDis;
	}
	//һ������
	else	
		return 1+cellB.bToStartDis;
}

//�������
void CGameLogic::ClearList()
{
	CNode * pTemp;
	POSITION posTemp=m_ListCanPassNode.GetHeadPosition();
	for (posTemp;posTemp!= NULL; )
	{
		pTemp=(CNode *)m_ListCanPassNode.GetNext(posTemp);		
		if (!pTemp)
			delete pTemp;
	}
	for (posTemp=m_ListHaveGoneNode.GetHeadPosition(); posTemp!= NULL; )
	{
		pTemp=(CNode *)m_ListHaveGoneNode.GetNext(posTemp);		
		if (!pTemp)
			delete pTemp;
	}
	m_ListCanPassNode.RemoveAll();
	m_ListHaveGoneNode.RemoveAll();
}

//������� 
int CGameLogic::ComputeNodeDis(int xdest, int ydest,int xorgin,int yorgin)
{
	BYTE bXPos , bYPos , bTotalDis ;

	bXPos = abs(xdest - xorgin);
	bYPos = abs(ydest - yorgin);

	//�ܹ�����
	bTotalDis = (bXPos+bYPos);	

	return bTotalDis;
}

//��С���
void CGameLogic::GetMinDisNode(CPtrList *plist, POSITION &pos)
{
	CNode *pMin,*pTemp;
	if (plist->IsEmpty())
	{
		pos=NULL;
		return;
	}

	pMin=(CNode * )plist->GetHead();
	POSITION posTemp=plist->GetTailPosition();

	if (pMin)
	{
		for (pTemp=pMin; posTemp!= NULL; pTemp=(CNode * )plist->GetPrev(posTemp))
		{				
			if (pTemp->bTotalDisDis <= pMin->bTotalDisDis)
			{
				pMin=pTemp;	
				continue;
			}		
		}
		pos=plist->Find((void *)pMin);
	}
	else
		pos=NULL;	

	pMin=NULL;
	pTemp=NULL;

}

//Ѱ��λ��
void CGameLogic::FinPos(CPtrList &list,int x,int y,POSITION &pos)
{
	CNode * pCell;
	POSITION pTemp;
	for (pTemp=list.GetHeadPosition();pTemp!= NULL;)
	{
		pCell=(CNode * )list.GetNext(pTemp);
		if (pCell->bXPos==x && pCell->bYPos==y)
		{
			pos=list.Find((void *)pCell);			
			break;
		}		
	}	
	return;
}

//��ʼ���
void CGameLogic::Init(POINT ptDest, POINT ptStart , tagChessItem ChessItem[17][17])
{
	int  tx[8]={1, 1, 0, -1,-1,-1, 0, 1};
	int  ty[8]={0,-1,-1, -1, 0, 1, 1, 1};
	BYTE x,y;
	CNode * pNew;
	ClearList();	

	m_bXDest=(BYTE)(ptDest.x);
	m_bYDest=(BYTE)(ptDest.y);

	m_StartNode=CNode(ptStart.x,ptStart.y);
	m_StartNode.bToStartDis=0;			
	m_StartNode.pParent=NULL;

	//���㲽��
	m_StartNode.bTotalDisDis=ComputeNodeDis(m_bXDest,m_bYDest,m_StartNode.bXPos,m_StartNode.bYPos);

	////////////////////////////////////////////////////////

	m_bStop=false;
	m_bFind=false;	

	for (int i=0;i<8;i++)
	{
		x=m_StartNode.bXPos+tx[i];
		y=m_StartNode.bYPos+ty[i];		

		if (x<0 || y<0 || x>=17 || y>=17)
			continue;
		//��������
		if(! ((x>=6  && x<=10 && y>=1  && y<=5)  ||
			  (x>=11 && x<=15 && y>=6  && y<=10) ||
			  (x>=6  && x<=10 && y>=11 && y<=15) ||
			  (x>=1  && x<=5  && y>=6  && y<=10) ||
			  (x>=6  && x<=10 && y>=6  && y<=10)
			))
			continue ;

		//������·
		if(true==NotOnRail(x,y)) 
			continue ;

		//��������
		if((7==x && 7==y) || (7==x && 9==y) || (9==x && 7==y) || (9==x && 9==y))
			continue ;

		//��Ӫλ��
		if(IsBarback(x,y))
			continue ;

		//��������
		if (CHESS_NULL!=ChessItem[x][y].bChessID)
			continue;

		//��������,������������
		if (i%2!=0 && (!AbleTurn(x,y) || !AbleTurn((BYTE)(ptStart.x) , (BYTE)(ptStart.y)) ))
			continue;

		if  ((6==x  && (7==y || 9==y)  && 0==i)	||	//�����ߵ��ǽ��
			((7==x  || 9==x) && 10==y  && 2==i)	||	//�����ߵ��ǽ��
			(10==x  && (7==y || 9==y)  && 4==i)	||	//�����ߵ��ǽ��
			((7==x  || 9==x) && 6==y   && 6==i))	//�����ߵ��ǽ��
			continue;

		if ((5==x  && (7==y || 9==y ) && 4==i) ||	//�ӷǽ��������
			((7==x || 9==x)  && 5==y   && 2==i)||	//�ӷǽ��������
			(11==x && (7==y  || 9==y)  && 0==i)||	//�ӷǽ��������
			((7==x || 9==x)  && 11==y  && 6==i))	//�ӷǽ��������
			continue;

		pNew=new CNode(x,y);						
		pNew->pParent=&m_StartNode;
		pNew->bToDesDis=ComputeNodeDis(m_bXDest,m_bYDest,x,y);
		pNew->ComputeDistance();			
		m_ListCanPassNode.AddTail((void *)pNew);
		pNew=NULL;		
	}
	m_ListHaveGoneNode.AddTail((void *)&m_StartNode);
}
//////////////////////////////////////////////////////////////////////////
