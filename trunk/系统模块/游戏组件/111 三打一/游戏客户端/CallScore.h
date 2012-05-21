#ifndef CALL_SCORE_HEAD_FILE
#define CALL_SCORE_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"

#define IDM_LAND_SCORE				(WM_USER+104)						//�з���Ϣ

//////////////////////////////////////////////////////////////////////////

//������ͼ��
class CCallScore : public CWnd
{
	/*
	CSkinButton                    m_Score120;            //120��
	CSkinButton                    m_Score115;            //115��
	CSkinButton                    m_Score110;            //110��
	CSkinButton                    m_Score105;            //105��
	CSkinButton                    m_Score100;            //100��
	CSkinButton                    m_Score95;             //95��
	CSkinButton                    m_Score90;             //90��
	CSkinButton                    m_Score85;             //85��
	*/
	CSkinButton                    m_Score80;             //80��
	CSkinButton                    m_Score75;             //75��
	CSkinButton                    m_Score70;             //70��
	CSkinButton                    m_Score65;             //65��
	CSkinButton                    m_Score60;             //60��
	CSkinButton                    m_Score55;             //55��
	CSkinButton                    m_Score50;             //50��
	CSkinButton                    m_Score45;             //45��
	CSkinButton                    m_Score40;             //40��
	CSkinButton                    m_Score35;             //35��
	CSkinButton                    m_Score30;             //30��
	CSkinButton                    m_Score25;             //25��
	CSkinButton                    m_Score20;             //20��
	CSkinButton                    m_Score15;             //15��
	CSkinButton                    m_Score10;             //10��
	CSkinButton                    m_Score5;              //5��
	CSkinButton					   m_btGiveUpScore;		  //����

	CSkinImage					   m_ImageScoreBack;			  //����ͼ��

	//�з���Ϣ
public:
	BYTE						   m_cbPriScore;				//�ϴνз�
	
	//��������
public:
	//���캯��
	CCallScore();

	//�ػ�����
	void ReloadView();

	//��������
	virtual ~CCallScore();

	
	//��Ϣ����
protected:
	//�ػ�����
	afx_msg void OnPaint();
	//������Ϣ
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	//�з�OK��ť
	afx_msg void OnCallScore80();
	//�з�OK��ť
	afx_msg void OnCallScore75();
	//�з�OK��ť
	afx_msg void OnCallScore70();
	//�з�OK��ť
	afx_msg void OnCallScore65();
	//�з�OK��ť
	afx_msg void OnCallScore60();
	//�з�OK��ť
	afx_msg void OnCallScore55();
	//�з�OK��ť
	afx_msg void OnCallScore50();
	//�з�OK��ť
	afx_msg void OnCallScore45();
	//�з�OK��ť
	afx_msg void OnCallScore40();
	//�з�OK��ť
	afx_msg void OnCallScore35();
	//�з�OK��ť
	afx_msg void OnCallScore30();
	//�з�OK��ť
	afx_msg void OnCallScore25();
	//�з�OK��ť
	afx_msg void OnCallScore20();
	//�з�OK��ť
	afx_msg void OnCallScore15();
	//�з�OK��ť
	afx_msg void OnCallScore10();
	//�з�OK��ť
	afx_msg void OnCallScore5();
	//������ť
	afx_msg void OnGiveUpScore();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif