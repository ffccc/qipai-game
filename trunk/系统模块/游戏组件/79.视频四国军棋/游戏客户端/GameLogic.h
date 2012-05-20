#pragma once

#include "Stdafx.h"
#include <AFXTEMPL.H>

//////////////////////////////////////////////////////////////////////////

enum enCompareResult 
{
	enSmall , 
	enEqual , 
	enLarge ,
	enError
};

//�����Ϣ
class CNode  
{
public:
	//�������
	void ComputeDistance();	
	CNode();
	CNode(int x,int y);
	virtual ~CNode();

public:
	BYTE							bXPos ,								//�������
									bYPos;								//�������
public:
	BYTE							bToDesDis,							//Ŀ�����
									bToStartDis,						//��ʼ����
									bTotalDisDis;						//�ܹ�����

	CNode							*pParent;							//��һ���
};


//��Ϸ�߼���
class CGameLogic
{
	//���ױ���
public:


	//���̱���
public:
	tagChessItem					m_ChessItem[GAME_PLAYER][25];		//������Ϣ
	tagChessItem *					m_ChessBorad[17][17];				//������Ϣ
	CArray <POINT,POINT>			m_ptPathArray;						//����·��

	//��������
public:
	bool							m_bStop,							//ֹͣ��־
									m_bFind;							//�ҵ���־

	BYTE							m_bYDest,							//Ŀ���� 
									m_bXDest;							//Ŀ���� 

	CPtrList						m_ListCanPassNode,					//���߽��				
									m_ListHaveGoneNode;					//�߹����

	CNode							*m_pCurNode,						//��ǰ���					
									m_StartNode,						//��ʼ���
									*m_pEndNode;						//�������


	//��������
public:
	//���캯��
	CGameLogic();
	//��������
	virtual ~CGameLogic();

	//��Ϣ����
public:

	//������
public:

	//���ƺ���
public:
	//��������
	bool SwapChess(tagChessItem ChessItem[17][17] , BYTE bSourceX , BYTE bSourceY , BYTE bDesX , BYTE bDesY , bool bSwapChess=true)  ;


	//�߼�����
public:
	//�ܷ��ƶ�
	bool AbleMove(const BYTE bChessColor , const tagChessItem ChessItem[17][17] , const BYTE bXPos , const BYTE bYPos) ;
	//�Ƿ��Ӫ
	bool IsBarback(BYTE bXPos , BYTE bYPos) ;
	//�ƶ�����
	bool MoveChess(BYTE  const bPlayerCount , tagChessItem ChessItem[17][17] , const BYTE bSourceXPos , const BYTE bSourceYPos , const BYTE bDesXPos , const BYTE bDesYPos , bool bMove=true) ;
	//�Ƿ�з�
	bool IsEnemy(BYTE bPlayerCount , const tagChessItem ChessItem[17][17] , const BYTE bSourceXPos , const BYTE bSourceYPos , const BYTE bDesXPos , const BYTE bDesYPos) ;
	//�Ƿ����
	bool AbleTurn(BYTE const bXPos , BYTE const bYPos) ;
	//������·
	bool NotOnRail(BYTE const bXPos , BYTE const bYPos) ;
	//�Ƚ�����
	enCompareResult CompareChess(const tagChessItem ChessItem[17][17] , const BYTE bSourceXPos , const BYTE bSourceYPos , const BYTE bDesXPos , const BYTE bDesYPos) ;
	//�ڴ�Ӫ
	bool IsHeadquarters(BYTE bXPos , BYTE bYPos) ;
	//�ǽ�㴦
	bool NotOnNode(BYTE bXPos , BYTE bYPos) ;
	//�Ƿ�����
	bool IsOnBoard(BYTE bXPos , BYTE bYPos) ;
	//�ɷ���
    bool CanCarry(BYTE const bChessID , BYTE const bChessColor , const tagChessItem ChessItem[17][17]) ;
	//û������
	bool NoChess(BYTE const bChessColor , const tagChessItem ChessItem[17][17]) ;
	//��������
	CString IDToName(BYTE bChessID) ;

	//�������� 
public:
	//Ѱ��·��
	bool FindPath(POINT ptDest,POINT ptStart , tagChessItem ChessItem[17][17] , bool bMove=true);
	//������
	BYTE ComputeToStartDis(CNode &cellA,CNode &cellB);
	//�������
	void ClearList();	
	//������� 
	int ComputeNodeDis(int xdest,int ydest,int xorgin,int yorgin);
	//��С���
	void GetMinDisNode(CPtrList *list,POSITION &pos);
	//Ѱ��λ��
	void FinPos(CPtrList &list,int x,int y,POSITION &pos);
	//��ʼ���
	void Init(POINT ptDest,POINT ptStart , tagChessItem ChessItem[17][17]);

	//�ڲ�����
private:
};

//////////////////////////////////////////////////////////////////////////
