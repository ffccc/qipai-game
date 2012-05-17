#ifndef SCORE_CONTROL_HEAD_FILE
#define SCORE_CONTROL_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "CardControl.h"

//////////////////////////////////////////////////////////////////////////

//������Ϣ
struct tagScoreInfo
{
	//�����˿�
	BYTE							cbCardCount;							//�˿���Ŀ
	BYTE							cbCardData[MAX_COUNT];					//�˿�����

	//�û���Ϣ
	WORD							wBankerUser;							//ׯ���û�
	TCHAR							szUserName[GAME_PLAYER][NAME_LEN];		//�û�����

	//������Ϣ
	WORD							wProvideUser;							//��Ӧ�û�
	BYTE							cbProvideCard;							//��Ӧ�˿�
	LONG							lGameScore[GAME_PLAYER];				//��Ϸ����

	//���ƽ��
	DWORD							dwChiHuKind[GAME_PLAYER];				//��������
	DWORD							dwChiHuRight[GAME_PLAYER];				//��������
};

//�����Ϣ
struct tagWeaveInfo
{
	BYTE							cbWeaveCount;							//�����Ŀ
	BYTE							cbCardCount[4];							//�˿���Ŀ
	BYTE							cbPublicWeave[4];						//�������
	BYTE							cbCardData[4][4];						//����˿�
};

//////////////////////////////////////////////////////////////////////////

//���ֿؼ�
class CScoreControl : public CWnd
{
	//��������
protected:
	tagScoreInfo					m_ScoreInfo;							//������Ϣ

	//�ؼ�����
protected:
	BYTE							m_cbWeaveCount;							//�����Ŀ
	CWeaveCard						m_WeaveCard[4];							//����˿�
	CSkinButton						m_btCloseScore;							//�رհ�ť

	//��Դ����
protected:
	CSkinImage						m_ImageWinLose;							//ʤ��ʧ��
	CImage							m_ImageGameScore;						//������ͼ
	CSkinImage						m_ImageGameScoreFlag;					//������ͼ

	//��������
public:
	//���캯��
	CScoreControl();
	//��������
	virtual ~CScoreControl();

	//���ܺ���
public:
	//��λ����
	void RestorationData();
	//���û���
	void SetScoreInfo(const tagScoreInfo & ScoreInfo, const tagWeaveInfo & WeaveInfo);

	//��ť��Ϣ
protected:
	//�رհ�ť
	afx_msg void OnBnClickedClose();

	//��Ϣӳ��
protected:
	//�ػ�����
	afx_msg void OnPaint();
	//�滭����
	afx_msg BOOL OnEraseBkgnd(CDC * pDC);
	//��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint Point);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMove(int x, int y);
};

//////////////////////////////////////////////////////////////////////////

#endif