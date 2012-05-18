#include "StdAfx.h"
#include "historyrecord.h"

CHistoryRecord::CHistoryRecord(void)
{
    memset(m_Chess , 0 , sizeof(m_Chess)) ;
	RemoveAllFromList();
}

CHistoryRecord::~CHistoryRecord(void)
{
}

//�������
void CHistoryRecord::AddToList(SaveChessStruct Chess)			
{
	m_list.AddTail(Chess);		
}

//��ʼ����
void CHistoryRecord::Init()
{
	memset(m_Chess , 0 , sizeof(m_Chess)) ;
	RemoveAllFromList();
}

//ɾ������
void CHistoryRecord::RemoveAllFromList()
{
	m_list.RemoveAll();
}

//��ȡ����
bool CHistoryRecord::GetFirstFromList(SaveChessStruct &ChessStruct)
{
	m_pos=m_list.GetHeadPosition();
	if(!m_pos)
	{
		return false ;
	}
	ChessStruct = m_list.GetNext(m_pos);
	return true ;
}

//��ȡ����
bool CHistoryRecord::GetNextFromList(SaveChessStruct &ChessStruct)
{
	if(!m_pos)
	{
		return false ;
	}
	SaveChessStruct save;
	save=m_list.GetNext(m_pos);

	ChessStruct= save ;
	return true ;
}

//��ȡ����
bool  CHistoryRecord::GetPrevFromList(SaveChessStruct &ChessStruct)
{	
	if(!m_pos)
	{
		//������
		return false ;
	}
	SaveChessStruct save;
	save=m_list.GetPrev(m_pos);

	ChessStruct = save ;
	return true ;
}

//��ȡ����
bool CHistoryRecord::GetOneFromList(short num , SaveChessStruct &ChessStruct)
{
	SaveChessStruct save;
	POSITION pos;
	pos=m_list.GetHeadPosition();
	for(int i=0;i<num;i++)
	{
		if(!pos) return false ;

		save=m_list.GetNext(pos);
	}
	ChessStruct = save ;
	return true ;
}

//���沼��
void CHistoryRecord::SavePlacement(const tagChessItem Chess[17][17])
{
    CopyMemory(m_Chess , Chess , sizeof(m_Chess)) ;
}



//����ת��
void CHistoryRecord::ClientToServer(const BYTE bChessColor , const BYTE bRealClientX , const BYTE bRealClientY , BYTE &bServerX , BYTE &bServerY) 
{
	switch(bChessColor) 
	{
	case CHESS_COLOR_GREEN:	//��ɫ����
		bServerX = 16-bRealClientX ;
		bServerY = 16-bRealClientY ;
		break;

	case CHESS_COLOR_BLACK:	//��ɫ����
		bServerX = bRealClientY ;
		bServerY = 16-bRealClientX ;
		break;

	case CHESS_COLOR_BLUE:	//��ɫ����
		bServerX = bRealClientX ;
		bServerY = bRealClientY ;
		break;

	case CHESS_COLOR_RED:	//��ɫ����
		bServerX = 16-bRealClientY ;
		bServerY = bRealClientX ;
		break;

	default:
#ifdef _DEBUG
		AfxMessageBox("CHistoryRecord::ClientToServer:��������") ;
#endif
		bServerX = 255 ;
		bServerY = 255 ;
	}
}

//��ȡ����
LONG CHistoryRecord::GetStepCount() 
{
	return (LONG)(m_list.GetCount()) ;
}
