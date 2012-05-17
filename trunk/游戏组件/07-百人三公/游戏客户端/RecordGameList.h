#ifndef RECORD_GAME_LIST_HEAD_FILE
#define RECORD_GAME_LIST_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

//��Ϸ��¼
struct tagGameRecord
{
	//��Ϸ��Ϣ
	WORD							wDrawCount;							//��Ϸ����	
	BYTE							cbWinerSide;						//ʤ�����
	BYTE							cbPlayerPoint;						//�м��Ƶ�
	BYTE							cbBankerPoint;						//ׯ���Ƶ�	

	//�ҵ���Ϣ
	LONG							lGameScore;							//��Ϸ�ɼ�	
	LONG							lMyAddGold;							//��Ϸ��ע	

	//��ע����
	LONG							lDrawTieScore;						//��ƽ��ע
	LONG							lDrawBankerScore;					//��ׯ��ע
	LONG							lDrawPlayerScore;					//������ע
};

//////////////////////////////////////////////////////////////////////////

//��Ϸ��¼
class CGameRecord : public CSkinDialogEx
{
	//�ؼ�����
public:
	CSkinListCtrl					m_RecordGameList;					//��Ϸ�б�

	//��������
public:
	//���캯��
	CGameRecord();
	//��������
	virtual ~CGameRecord();

	//���غ���
protected:
	//�ؼ���
	virtual void DoDataExchange(CDataExchange * pDX);
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ����Ϣ
	virtual void OnOK() { return; }
	//ȡ����Ϣ
	virtual void OnCancel() { return; }

	//���ܺ���
public:
	//�����б�
	void FillGameRecourd(tagGameRecord & GameRecord);

	//��Ϣӳ��
protected:
	//λ�ñ仯
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif
