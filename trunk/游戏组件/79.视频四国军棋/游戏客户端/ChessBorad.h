#ifndef CHESS_BORAD_HEAD_FILE
#define CHESS_BORAD_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include <AFXTEMPL.H>


//////////////////////////////////////////////////////////////////////////
//�궨��

//λ�ö���
#define CHESS_WIDTH					30									//���ӿ��
#define CHESS_HEIGTH				30									//���Ӹ߶�
#define NODE_WIDTH					36									//�����
#define NODE_HEIGTH					36									//���߶�

#define ARROW_WIDTH					30									//��ͷ���
#define ARROW_HEIGTH				30									//��ͷ�߶�

//////////////////////////////////////////////////////////////////////////
//��־��Ϣ
struct tagFlagInfo
{
	bool							bSet;								//���ñ�־
	BYTE							bXPos ,								//��־����
									bYPos ;								//��־����
};

class CShowFlagDlg ;
class CGameClientView ;

//��Ϸ����
class CChessBorad
{
	//���ñ���
protected:
	BYTE							m_cbUserColor;						//�û���ɫ
	bool							m_bPositively;						//��Ӧ��־

public:
	static LONG						m_bExsursionX;						//����ƫ��
	static LONG						m_bExsursionY;						//����ƫ��
	CPoint							m_StartPoint;						//��־λ��
	BYTE							m_bSetFlagX ,						//��־λ��
									m_bSetFlagY ;						//��־λ��

	//��������
protected:
	CGameClientView					*m_pParent;							//����ָ��
	CArray <POINT,POINT>			m_ptPathArray;						//����·��
	tagFlagInfo						m_FlagInfo[17][17] ;				//���ñ�־

	//״̬����
protected:
	tagChessItem					m_ChessBorad[17][17];				//������Ϣ

	//��Դ����
protected:
	CSize							m_BoradSize;						//���̴�С
	CSkinImage						m_ImageChess;						//������Դ
	CSkinImage						m_ImageBorad;						//������Դ
	CSkinImage						m_ImageBackGround;					//������Դ
	CSkinImage						m_ImageArrow;						//��ͷ��Դ
	CSkinImage						m_ImageFlag;						//��־ͼ��

	//�ؼ�����
public:
	CShowFlagDlg					*m_pFlagImageDlg;					//��־����


	//��������
public:
	//���캯��
	CChessBorad();
	//��������
	virtual ~CChessBorad();

	//���ӿ���
public:
	//�������
	void CleanChess();
	//ѡ������
	void SetSelectChess(BYTE cbXPos, BYTE cbYPos);
	//��������
	void SetChess(const tagChessItem tagChessItems[17][17]) ;

	//״̬����
public:
	//���ø���
	void SetParent(CGameClientView *pWnd) ;
	//�ػ�����
	void DrawBoard(CDC *pDC);
	//����ƫ��
	void SetExcursionX(const LONG ExcursionX) ;
	//����ƫ��
	void SetExcursionY(const LONG ExcursionY) ;

	//״̬��ѯ
public:
	//��ȡ��С
	const CSize & GetChessBoradSize() { return m_BoradSize; }

	//��������
public:
	//��ȡ����
	int GetChessXPos(BYTE cbXPos);
	//��ȡ����
	int GetChessYPos(BYTE cbYPos);
	//ת������
	void SwitchMousePos(const CPoint & MousePoint, BYTE & cbXPos, BYTE & cbYPos);
	//����·��
	void SetPath(const CArray <POINT,POINT> *pPtPath) ;
	//�滭��ͷ
	void DrawArrow(CDC *pDC) ;
	//���ñ�־
	void SetFlagImage(const BYTE bFlagXPos , const BYTE bFlagYPos , bool bSet=true) ;
	//��ʾ��־
	void ShowFlagImage(CPoint point , bool bShow) ;
	//��־����
	void SetFlaChessPos(BYTE bXPos , BYTE bYPos) ;
	//�����־
	void ClearFlagImage(BYTE bXPos , BYTE bYPos) ;
	//��ת��־
	void ReverseFlagImage() ;
	//�ƶ���־
	void MoveFlagImage(BYTE bSourceX , BYTE bSourceY , BYTE bTargetX , BYTE bTargetY , bool bSet) ;

	//��Ϣӳ��
protected:

};

//////////////////////////////////////////////////////////////////////////

#endif