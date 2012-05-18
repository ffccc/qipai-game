
#include "stdafx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameLogic::CGameLogic()
{
	m_LastPisa.nCount=0;
	m_LastPisa.nChairId=0;
	m_bFirst=FALSE;
}

//��������
CGameLogic::~CGameLogic()
{
}

int CGameLogic::StartANewGame()
{
	for(int i=0;i<GAME_PLAYER;i++)
		m_PisaArray[i].RemoveAll();
	//��ʼ����
	struct tagPisa tmp;
	srand(GetTickCount());
	int Index=0;
	int tmpindex;
#ifdef _TWICE
	int time;
#endif
	for(int row=1;row<=13;row++)
	{
		for(int line=1;line<=4;line++)
		{
#ifdef _TWICE
			for(time=0;time<2;time++)
			{
#endif
			while(1)
			{
				tmpindex=rand()%GAME_PLAYER;
				if (m_PisaArray[tmpindex].GetCount()<PISACOUNT)
					break;
			}
			tmp.row=uncompare[row];
			tmp.line=line;
			if (uncompare[row]==3 && line==4)//����3
				Index=tmpindex;
			m_PisaArray[tmpindex].Add(tmp);
#ifdef _TWICE
			}
#endif
		}
	}
#ifdef _TWICE
	for(time=0;time<2;time++)
	{
		//С��
		while(1)
		{
			tmpindex=rand()%GAME_PLAYER;
			if (m_PisaArray[tmpindex].GetCount()<PISACOUNT)
				break;
		}
		tmp.line=1;
		tmp.row=14;
		m_PisaArray[tmpindex].Add(tmp);
	}
	for(time=0;time<2;time++)
	{
		//����
		while(1)
		{
			tmpindex=rand()%GAME_PLAYER;
			if (m_PisaArray[tmpindex].GetCount()<PISACOUNT)
				break;
		}
		tmp.line=2;
		tmp.row=14;
		m_PisaArray[tmpindex].Add(tmp);
	}
#endif
	m_LastPisa.nCount=0;
	m_LastPisa.nChairId=0;
	m_bFirst=TRUE;
	return Index;
}

void CGameLogic::GetInitPisa(int nIndex,struct tagSetInitPisa *pPisa)
{
	memset(pPisa,0,sizeof(tagSetInitPisa));
	pPisa->nOwnerIndex=nIndex;
	ASSERT(m_PisaArray[nIndex].GetCount()>=PISACOUNT);
	for(int i=0;i<PISACOUNT;i++)
		pPisa->tPisa[i]=m_PisaArray[nIndex].GetAt(i);
	return;
}

void CGameLogic::EmptyLastCardInfo()
{
	m_LastPisa.nCount=0;
}

BOOL CGameLogic::IsUserBegin(int index)
{
	//��������һ�γ��Ƽ�¼����
	if (m_LastPisa.nCount!=0 && m_LastPisa.nChairId==index)
		return TRUE;
	return FALSE;
}

BOOL CGameLogic::IsRegular(const struct tagSetOutPisa *pisa)
{
	if (pisa->nCount==1) return TRUE;
	else if (pisa->nCount==2)
	{
		//�Ƿ�Ϊ��
		if (pisa->tOutPisa[0].row == pisa->tOutPisa[1].row)
			return TRUE;
		else
			_tcscpy(m_chErrorMsg,_T("�������Ʋ��Ƕԣ�"));
	}
	else if (pisa->nCount==3)
	{
		//�Ƿ�Ϊ��ͬ��
		if (pisa->tOutPisa[0].row == pisa->tOutPisa[1].row && pisa->tOutPisa[0].row == pisa->tOutPisa[2].row)
			return TRUE;
		else
			_tcscpy(m_chErrorMsg,_T("�������Ʋ�����ͬ�ţ�"));
	}
	else
	{
		int Count;
		struct tagPisa PisaPoint;
		BOOL bSameColor;
		//���������ը��
		if (IsBomb(pisa,&Count,&PisaPoint))
		{
			TRACE("����ը������\n");
			return TRUE;
		}
		//���������ͬ����
		if (IsContinue3(pisa,&PisaPoint))
		{
			TRACE("����ͬ���Ź���\n");
			return TRUE;
		}
		//�������������
		if (IsContinue2(pisa,&PisaPoint))
		{
			TRACE("�������Թ���\n");
			return TRUE;
		}
		//���������˳��
		if (IsStraight(pisa,&PisaPoint,&bSameColor))
		{
			TRACE("����˳�ӹ���\n");
			return TRUE;
		}
		//���������������
		if (Is3And2(pisa,&PisaPoint))
		{
			TRACE("��������������\n");
			return TRUE;
		}
		//��������Ǻ���
		if (IsButterfly(pisa,&PisaPoint))
		{
			TRACE("���Ϻ�������\n");
			return TRUE;
		}
	}
	_tcscpy(m_chErrorMsg,_T("�������Ʋ��Ϸ�����"));
	return FALSE;
}

BOOL CGameLogic::IsBiggerThanLastCard(const struct tagSetOutPisa *pisa)
{
	//�жϳ����Ƿ�Ϸ�Ҫ��
	if (!IsRegular(pisa))
		return FALSE;
	_tcscpy(m_chErrorMsg,_T("�������Ʋ�����"));
	if (m_bFirst)
	{
		BOOL m_bFound3=FALSE;
		//����ǵ�һ�γ��ƣ���Ҫ����ǲ��Ǻ��к���3
		for(int i=0;i<pisa->nCount;i++)
			if (pisa->tOutPisa[i].row==3 && pisa->tOutPisa[i].line==4)//����3
			{
				m_bFound3=TRUE;
				break;
			}
		if (!m_bFound3)
		{
			_tcscpy(m_chErrorMsg,_T("��һ�γ��Ʊ��뺬�к���3��"));
			return FALSE;
		}
	}
	//�������һ�γ��Ƽ�¼
	if (m_LastPisa.nCount!=0)
	{
		int Count,Count1;
		struct tagPisa PisaPoint;
		struct tagPisa PisaPoint1;
		BOOL bIsBomb=IsBomb(pisa,&Count,&PisaPoint);
		BOOL bIsBomb1=IsBomb(&m_LastPisa,&Count1,&PisaPoint1);
		//�����ҳ�����ը���������ϼҳ��Ĳ���ը����ֱ�ӷ���TRUE
		if (bIsBomb && !bIsBomb1)
			return TRUE;
		//�����ҳ��Ĳ���ը���������ϼҳ�����ը����ֱ�ӷ���TRUE
		if (!bIsBomb && bIsBomb1)
			return FALSE;
		//���������Ҷ���ը��
		if (bIsBomb && bIsBomb1)
		{
			TRACE("ը���Ƚϣ�Count:%d,%d, Pisa:%d,%d:%d,%d��\n",Count,Count1,PisaPoint.row,PisaPoint.line,PisaPoint1.row,PisaPoint1.line);
			//��ҳ���������ը��
			if (PisaPoint.row==14) return TRUE;
			//�ϼҳ���������ը��
			if (PisaPoint1.row==14) return FALSE;
			//�����϶�
			if (Count>Count1)
				return TRUE;
			//�����ϴ�
			else if (Count==Count1 && compare[PisaPoint.row]>compare[PisaPoint1.row])
				return TRUE;
			//��ʽ�ϴ�
			else if (Count==Count1 && PisaPoint.row==PisaPoint1.row && PisaPoint.line>PisaPoint1.line)
				return TRUE;
			return FALSE;
		}
		//�жϳ������Ƿ���ͬ
		if (pisa->nCount==m_LastPisa.nCount && pisa->nCount<=PISACOUNT)
		{
			if(pisa->nCount==1)
			{
				//�����ϴ�
				if (compare[pisa->tOutPisa[0].row] > compare[m_LastPisa.tOutPisa[0].row])
					return TRUE;
				//��ɫ�ϴ�
				else if (pisa->tOutPisa[0].row == m_LastPisa.tOutPisa[0].row &&
					pisa->tOutPisa[0].line > m_LastPisa.tOutPisa[0].line)
					return TRUE;
				TRACE("�����ƱȽ����\n");
			}
			else if (pisa->nCount==2)
			{
				//�����ϴ�
				if (compare[pisa->tOutPisa[0].row] > compare[m_LastPisa.tOutPisa[0].row])
					return TRUE;
				//��ɫ�ϴ�
				else if (pisa->tOutPisa[0].row == m_LastPisa.tOutPisa[0].row &&
					max(pisa->tOutPisa[0].line,pisa->tOutPisa[1].line) > max(m_LastPisa.tOutPisa[0].line,m_LastPisa.tOutPisa[1].line))
					return TRUE;
				TRACE("2���ƱȽ����\n");
			}
			else if (pisa->nCount==3)
			{
				//�����ϴ�
				if (compare[pisa->tOutPisa[0].row] > compare[m_LastPisa.tOutPisa[0].row])
					return TRUE;
				//��ɫ�ϴ�
				else if (pisa->tOutPisa[0].row == m_LastPisa.tOutPisa[0].row &&
					(max(pisa->tOutPisa[0].line,max(pisa->tOutPisa[1].line,pisa->tOutPisa[2].line)) >
					max(m_LastPisa.tOutPisa[0].line,max(m_LastPisa.tOutPisa[1].line,m_LastPisa.tOutPisa[2].line))))
					return TRUE;
				TRACE("3���ƱȽ����\n");
			}
			else
			{
				//������������
/*				if (IsBomb(pisa,&Count,&PisaPoint))
				{
					TRACE("��ҳ�����ը����\n");
					//�����ը��
					if (IsBomb(&m_LastPisa,&Count1,&PisaPoint1))
					{
						TRACE("ը���Ƚϣ�Count:%d,%d, Pisa:%d,%d:%d,%d��\n",Count,Count1,PisaPoint.row,PisaPoint.line,PisaPoint1.row,PisaPoint1.line);
						//�����ϴ�
						if (compare[PisaPoint.row]>compare[PisaPoint1.row])
							return TRUE;
						//��ʽ�ϴ�
						else if (PisaPoint.row==PisaPoint1.row && PisaPoint.line>PisaPoint1.line)
							return TRUE;
					}
					else
						_tcscpy(m_chErrorMsg,_T("����������ը���������ϼҳ��Ĳ���ը����"));
				}*/
				//����ͬ�ţ����ԣ�˳�Ӷ�����������ͬ������ֻ��Ҫ�жϵ����Ϳ�����
				if (IsContinue3(pisa,&PisaPoint))
				{
					TRACE("��ҳ�������ͬ�ţ�\n");
					//���������ͬ��
					if (IsContinue3(&m_LastPisa,&PisaPoint1))
					{
						TRACE("�����ͬ������ƣ�%d,%d,�ϼ�����ƣ�%d,%d��\n",PisaPoint.line,PisaPoint.row,PisaPoint1.line,PisaPoint1.row);
						//�����ϴ�
						if (compare[PisaPoint.row] > compare[PisaPoint1.row])
							return TRUE;
						//��ɫ�ϴ�
						else if (PisaPoint.row == PisaPoint1.row && PisaPoint.line > PisaPoint1.line)
							return TRUE;
					}
					else
						_tcscpy(m_chErrorMsg,_T("������������ͬ�ţ������ϼҳ��Ĳ�����ͬ�ţ�"));
				}
				if (IsContinue2(pisa,&PisaPoint))
				{
					TRACE("��ҳ��������ԣ�\n");
					//���������
					if (IsContinue2(&m_LastPisa,&PisaPoint1))
					{
						TRACE("�����������ƣ�%d,%d,�ϼ�����ƣ�%d,%d��\n",PisaPoint.line,PisaPoint.row,PisaPoint1.line,PisaPoint1.row);
						//�����ϴ�
						if (compare[PisaPoint.row] > compare[PisaPoint1.row])
							return TRUE;
						//��ɫ�ϴ�
						else if (PisaPoint.row == PisaPoint1.row && PisaPoint.line > PisaPoint1.line)
							return TRUE;
					}
					else
						_tcscpy(m_chErrorMsg,_T("�������������ԣ������ϼҳ��Ĳ������ԣ�"));
				}
				BOOL bSameColor;
				BOOL bSameColor1;
				if (IsStraight(pisa,&PisaPoint,&bSameColor))
				{
					TRACE("��ҳ�����˳�ӣ�\n");
					//�����˳��
					if (IsStraight(&m_LastPisa,&PisaPoint1,&bSameColor1))
					{
						TRACE("���˳������ƣ�(!%d) %d,%d,�ϼ�����ƣ�(!%d) %d,%d��\n",bSameColor,PisaPoint.line,PisaPoint.row,bSameColor1,PisaPoint1.line,PisaPoint1.row);
						//�����ͬ��˳������֮ǰ�Ĳ���ͬ��˳
						if (bSameColor>bSameColor1)
							return TRUE;
						//�����ϴ�
						if (compare[PisaPoint.row] > compare[PisaPoint1.row])
							return TRUE;
						//��ɫ�ϴ�
						else if (PisaPoint.row == PisaPoint1.row && PisaPoint.line > PisaPoint1.line)
							return TRUE;
					}
					else
						_tcscpy(m_chErrorMsg,_T("����������˳�ӣ������ϼҳ��Ĳ���˳�ӣ�"));
				}
				//���������������
				if (Is3And2(pisa,&PisaPoint))
				{
					TRACE("��ҳ�������������\n");
					//�����������
					if (Is3And2(&m_LastPisa,&PisaPoint1))
					{
						TRACE("�������������ƣ�%d,%d,�ϼ�����ƣ�%d,%d��\n",PisaPoint.line,PisaPoint.row,PisaPoint1.line,PisaPoint1.row);
						//�����ϴ�
						if (compare[PisaPoint.row] > compare[PisaPoint1.row])
							return TRUE;
						//��ɫ�ϴ�
						else if (PisaPoint.row == PisaPoint1.row && PisaPoint.line > PisaPoint1.line)
							return TRUE;
					}
					else
						_tcscpy(m_chErrorMsg,_T("�����������������������ϼҳ��Ĳ�����������"));
				}
				//��������Ǻ���
				if (IsButterfly(pisa,&PisaPoint))
				{
					TRACE("��ҳ����Ǻ�����\n");
					//�����������
					if (Is3And2(&m_LastPisa,&PisaPoint1))
					{
						TRACE("��Һ�������ƣ�%d,%d,�ϼ�����ƣ�%d,%d��\n",PisaPoint.line,PisaPoint.row,PisaPoint1.line,PisaPoint1.row);
						//�����ϴ�
						if (compare[PisaPoint.row] > compare[PisaPoint1.row])
							return TRUE;
						//��ɫ�ϴ�
						else if (PisaPoint.row == PisaPoint1.row && PisaPoint.line > PisaPoint1.line)
							return TRUE;
					}
					else
						_tcscpy(m_chErrorMsg,_T("���������Ǻ����������ϼҳ��Ĳ��Ǻ�����"));
				}
			}
		}
		else
		{
/*			//�����������ͬ����Ҫ�Ƚ��ǲ���ը��
			if (IsBomb(pisa,&Count,&PisaPoint) && IsBomb(&m_LastPisa,&Count1,&PisaPoint1))
			{
			}
			else*/
			{
				_tcscpy(m_chErrorMsg,_T("���������������ϼҲ�ͬ�����Ҳ���ը����"));
			}
		}
	}
	else//û�г��Ƽ�¼����ǰ����ͨ��
		return TRUE;
	return FALSE;
}

char *CGameLogic::GetCompareError()
{
	return m_chErrorMsg;
}

BOOL CGameLogic::SaveLastCard(const struct tagSetOutPisa *pisa)
{
	//�ڶ�Ӧ��ҵĳ�ʼ�������Ƴ���Ӧ�����
	m_PisaArray[pisa->nChairId].RemoveAt(0,pisa->nCount);
	memcpy(&m_LastPisa,pisa,sizeof(tagSetOutPisa));
	m_bFirst=FALSE;
	return (m_PisaArray[pisa->nChairId].GetCount()!=0);
}

int CGameLogic::GetPlayerLeftCard(int nIndex)
{
	int count=(int)m_PisaArray[nIndex].GetCount();
	//һ��û��������4�������Ʋ���1/4������3�������Ʋ���1/2������ 2��
	if (count>=PISACOUNT) return count*4;
	else if (count>=PISACOUNT-PISACOUNT/4) return count*3;
	else if (count>=PISACOUNT-PISACOUNT/2) return count*2;
	return count;
}

//�ж������Ƿ�Ϊ���ԣ����ص�PisaCount�������ֵ
BOOL CGameLogic::IsContinue2(const struct tagSetOutPisa *pisa,struct tagPisa *PisaPoint)
{
	if (!_IsContinue(pisa,PisaPoint,2)) return FALSE;
	//����Ҫ3������
	if (pisa->nCount<6) return FALSE;
	for(int i=0;i<pisa->nCount;i++)
	{
		//2���ܳ�����������
		if (pisa->tOutPisa[i].row==2) return FALSE;
	}
	return TRUE;
}

//�ж������Ƿ�Ϊ����ͬ�ţ����ص�PisaCount�������ֵ
BOOL CGameLogic::IsContinue3(const struct tagSetOutPisa *pisa,struct tagPisa *PisaPoint)
{
	if (!_IsContinue(pisa,PisaPoint,3)) return FALSE;
	//����Ҫ3������ͬ������
	if (pisa->nCount<9) return FALSE;
	for(int i=0;i<pisa->nCount;i++)
	{
		//2���ܳ���������ͬ����
		if (pisa->tOutPisa[i].row==2) return FALSE;
	}
	return TRUE;
}

//�ж������Ƿ�Ϊ˳�ӣ����ص�PisaCount�������ֵ��bSameColor��ʾ�Ƿ�Ϊͬ��˳
BOOL CGameLogic::IsStraight(const struct tagSetOutPisa *pisa,struct tagPisa *PisaPoint,BOOL *bSameColor)
{
	if (!_IsContinue(pisa,PisaPoint,1)) return FALSE;
	//˳�ӱ���5������
	if (pisa->nCount<5) return FALSE;
	for(int i=0;i<pisa->nCount;i++)
	{
		//2���ܳ�����˳����
		if (pisa->tOutPisa[i].row==2) return FALSE;
	}
	int line=pisa->tOutPisa[0].line;
	*bSameColor=TRUE;
	for(int i=1;i<pisa->nCount;i++)
	{
		if (pisa->tOutPisa[i].line!=line)
		{
			*bSameColor=FALSE;
			break;
		}
	}
	return TRUE;
}

//�ж������Ƿ�Ϊ�����������ص�PisaCount�������ֵ
BOOL CGameLogic::Is3And2(const struct tagSetOutPisa *pisa,struct tagPisa *PisaPoint)
{
	//������������5�ŵ�
	if (pisa->nCount!=5) return FALSE;
	//��ʼ���źͽ�β���ű���һ��
	if (pisa->tOutPisa[0].row!=pisa->tOutPisa[1].row ||
		pisa->tOutPisa[3].row!=pisa->tOutPisa[4].row)
		return FALSE;
	if (pisa->tOutPisa[2].row==pisa->tOutPisa[3].row)
	{
		PisaPoint->row=pisa->tOutPisa[2].row;
		PisaPoint->line=max(pisa->tOutPisa[2].line,max(pisa->tOutPisa[3].line,pisa->tOutPisa[4].line));
		return TRUE;
	}
	else if (pisa->tOutPisa[2].row==pisa->tOutPisa[1].row)
	{
		PisaPoint->row=pisa->tOutPisa[2].row;
		PisaPoint->line=max(pisa->tOutPisa[2].line,max(pisa->tOutPisa[1].line,pisa->tOutPisa[0].line));
		return TRUE;
	}
	return FALSE;
}

//�ж������Ƿ�Ϊ�������ͣ����ص�PisaCount�������ֵ
BOOL CGameLogic::IsButterfly(const struct tagSetOutPisa *pisa,struct tagPisa *PisaPoint)
{
	//����������15��
	if (pisa->nCount!=15) return FALSE;
	struct tagSetOutPisa p1,p2;
	struct tagPisa Pisa,Pisa1;
	//���������ŵ���ǰ
	memset(&p1,0,sizeof(p1));
	memset(&p2,0,sizeof(p2));
	p1.nCount=9;
	p2.nCount=6;
	for(int i=0;i<15;i++)
	{
		if (i<9)
			p1.tOutPisa[i]=pisa->tOutPisa[i];
		else
			p2.tOutPisa[i-9]=pisa->tOutPisa[i];
	}
	if (_IsContinue(&p1,&Pisa,3) && _IsContinue(&p2,&Pisa1,2))
	{
		//�Ǻ�������
		memcpy(PisaPoint,&Pisa,sizeof(Pisa));
		return TRUE;
	}
	//���������ŵ��ں�
	memset(&p1,0,sizeof(p1));
	memset(&p2,0,sizeof(p2));
	p1.nCount=6;
	p2.nCount=9;
	for(int i=0;i<15;i++)
	{
		if (i<6)
			p1.tOutPisa[i]=pisa->tOutPisa[i];
		else
			p2.tOutPisa[i-6]=pisa->tOutPisa[i];
	}
	if (_IsContinue(&p1,&Pisa,2) && _IsContinue(&p2,&Pisa1,3))
	{
		//�Ǻ�������
		memcpy(PisaPoint,&Pisa1,sizeof(Pisa));
		return TRUE;
	}
	return FALSE;
}

//�ж������Ƿ�Ϊը�������ص�Count�����������ص�PisaCount�������ֵ
BOOL CGameLogic::IsBomb(const struct tagSetOutPisa *pisa,int *Count,struct tagPisa *PisaPoint)
{
	int row=pisa->tOutPisa[0].row;
	int maxline=pisa->tOutPisa[0].line;
	for(int i=1;i<pisa->nCount;i++)
	{
		if (pisa->tOutPisa[i].row!=row)
			return FALSE;
		if (pisa->tOutPisa[i].line>maxline)
			maxline=pisa->tOutPisa[i].line;
	}
	if (pisa->nCount<4) return FALSE;
	*Count=pisa->nCount;
	PisaPoint->line=maxline;
	PisaPoint->row=row;
	return TRUE;
}

BOOL CGameLogic::_IsContinue(const struct tagSetOutPisa *pisa,struct tagPisa *PisaPoint,int offest)
{
	//������������offest
	if (pisa->nCount%offest!=0)
	{
		TRACE("�����ƱȽϺ�����������������offest!\n");
		return FALSE;
	}
	//������е�ֵ�Ƿ�Ϸ�
	int NotNeighborCount=0;
	for(int i=0;i<pisa->nCount;i+=offest)
	{
		//����ǶԻ��������ŵģ�������ڵ����Ƿ�ͬ��
		if (offest>1)
		{
			for(int j=i+1;j<i+offest;j++)
				if (pisa->tOutPisa[i].row != pisa->tOutPisa[j].row)
				{
					TRACE("�����ƱȽϺ����������Ʋ�ͬ��[%d]:%d,[%d]:%d��\n",i,pisa->tOutPisa[i].row,j,pisa->tOutPisa[j].row);
					return FALSE;
				}
		}
		//����������Ƿ���������
		if (i!=0)
			if (!IsNeighbor(pisa->tOutPisa[i].row,pisa->tOutPisa[i-1].row))
			{
				NotNeighborCount++;
				TRACE("�����ƱȽϺ���������%d�β������ڣ�[%d]:%d,[%d]:%d��\n",NotNeighborCount,i-1,pisa->tOutPisa[i-1].row,i,pisa->tOutPisa[i].row);
				//����ϲ���������������ɵģ�Ӧ���˳�
				if (pisa->tOutPisa[i].row==pisa->tOutPisa[i-1].row)
					return FALSE;
			}
	}
	BOOL bEnd=FALSE;
	//�Ƚ����һ���ƺ͵�һ�����Ƿ�����,ֻ������2��3֮������
	if (!IsNeighbor(pisa->tOutPisa[0].row,pisa->tOutPisa[pisa->nCount-1].row))
	{
		TRACE("�����ƱȽϺ��������������ƺ͵�һ���Ʋ����ڣ�[%d]:%d,[%d]:%d��\n",0,pisa->tOutPisa[0].row,pisa->nCount-1,pisa->tOutPisa[pisa->nCount-1].row);
		NotNeighborCount++;
	}
	else
		bEnd=TRUE;
	//���������ǿ�������ػ���,���������ж����������������������Σ���˵����������
	if (NotNeighborCount>=2)
	{
		TRACE("�����ƱȽϺ��������治����������%d�β����ڣ�\n",NotNeighborCount);
		return FALSE;
	}
	if (bEnd)
	{
		//˵������ͷβ���ӵģ���Ϊ��Ҫ�ж��Ƿ���K��3
		if (IsBothHave3AndK(pisa))
		{
			TRACE("�����ƱȽϺ�����������ͷβ���ӣ����Һ���K��3��\n");
			return FALSE;
		}
	}
	GetMaxPoint(pisa,PisaPoint);
	return TRUE;
}

BOOL CGameLogic::IsBothHave3AndK(const struct tagSetOutPisa *pisa)
{
	BOOL bFound3=FALSE;
	BOOL bFoundK=FALSE;
	for(int i=0;i<pisa->nCount;i++)
	{
		if (pisa->tOutPisa[i].row==3)
			bFound3=TRUE;
		else if (pisa->tOutPisa[i].row==13)
			bFoundK=TRUE;
	}
	return (bFound3 && bFoundK);
}

void CGameLogic::GetMaxPoint(const struct tagSetOutPisa *pisa,struct tagPisa *PisaPoint)
{
	struct tagPisa Max;//���ֵ
	struct tagPisa No12Max;//û��A��2�����ֵ
	BOOL bFound3=FALSE;
	Max.line=0;
	Max.row=0;
	No12Max.line=0;
	No12Max.row=0;
	for(int i=0;i<pisa->nCount;i++)
	{
		int value=compare[pisa->tOutPisa[i].row];
		if (pisa->tOutPisa[i].row==3)
			bFound3=TRUE;
		if (value<12 && compare[No12Max.row]<value)
		{
			No12Max.row=pisa->tOutPisa[i].row;
			No12Max.line=pisa->tOutPisa[i].line;
		}
		if (compare[Max.row]<value)
		{
			Max.row=pisa->tOutPisa[i].row;
			Max.line=pisa->tOutPisa[i].line;
		}
	}
	if (bFound3)
		memcpy(PisaPoint,&No12Max,sizeof(No12Max));
	else
		memcpy(PisaPoint,&Max,sizeof(Max));
}

BOOL CGameLogic::IsNeighbor(int index,int index2)
{
	if (index>index2 && index-index2==1) return TRUE;
	if (index<index2 && index2-index==1) return TRUE;
	if ((index==13 && index2==1) ||
		(index==1 && index2==13)) return TRUE;
	return FALSE;
}
