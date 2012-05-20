#ifndef CALL_SCORE_HEAD_FILE
#define CALL_SCORE_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"

#define IDM_LAND_SCORE				(WM_USER+104)						//�з���Ϣ
#define IDM_LAND_MULTIPLES			(WM_USER+105)						//�ӱ���Ϣ

//////////////////////////////////////////////////////////////////////////

//������ͼ��
class CCallScore : public CWnd
{
	//÷��
	CSkinButton                    m_CallC1;             
	CSkinButton                    m_CallC2;            
	CSkinButton                    m_CallC3;             
	CSkinButton                    m_CallC4;            
	CSkinButton                    m_CallC5;            
	CSkinButton                    m_CallC6;            
	CSkinButton                    m_CallC7;           

	//����
	CSkinButton                    m_CallD1;             
	CSkinButton                    m_CallD2;            
	CSkinButton                    m_CallD3;             
	CSkinButton                    m_CallD4;            
	CSkinButton                    m_CallD5;            
	CSkinButton                    m_CallD6;            
	CSkinButton                    m_CallD7;           

	//����
	CSkinButton                    m_CallH1;             
	CSkinButton                    m_CallH2;            
	CSkinButton                    m_CallH3;             
	CSkinButton                    m_CallH4;            
	CSkinButton                    m_CallH5;            
	CSkinButton                    m_CallH6;            
	CSkinButton                    m_CallH7;           

	//����
	CSkinButton                    m_CallS1;             
	CSkinButton                    m_CallS2;            
	CSkinButton                    m_CallS3;             
	CSkinButton                    m_CallS4;            
	CSkinButton                    m_CallS5;            
	CSkinButton                    m_CallS6;            
	CSkinButton                    m_CallS7;           

	//NT
	CSkinButton                    m_CallN1;             
	CSkinButton                    m_CallN2;            
	CSkinButton                    m_CallN3;             
	CSkinButton                    m_CallN4;            
	CSkinButton                    m_CallN5;            
	CSkinButton                    m_CallN6;            
	CSkinButton                    m_CallN7;           

	//����
	CSkinButton                    m_CallB1;             
	CSkinButton                    m_CallB2;            
	
    //����
	CSkinButton					   m_btGiveUpScore;		 

	CSkinImage					   m_ImageScoreBack;			  //����ͼ��

	//�з���Ϣ
public:
	BYTE						   m_cbLandScore;				//�����з�
	BYTE						   m_cbLandColor;				//������ɫ
	BYTE						   m_cbMultiples;				//��Ϸ����
	bool						   m_bDbl;						//�ӱ�
	bool						   m_bReDbl;					//�ټӱ�
	//��������
public:
	//���캯��
	CCallScore();

	//�ػ�����
	void ReloadView();

	//���ƱȽ�
	bool CompareCallScore(BYTE bColor,BYTE bValue);

	//��������
	virtual ~CCallScore();

	
	//��Ϣ����
protected:
	//�ػ�����
	afx_msg void OnPaint();
	//������Ϣ
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	//�з�OK��ť
	afx_msg void OnCallC1();
	afx_msg void OnCallC2();
	afx_msg void OnCallC3();
	afx_msg void OnCallC4();
	afx_msg void OnCallC5();
	afx_msg void OnCallC6();
	afx_msg void OnCallC7();

    afx_msg void OnCallD1();
	afx_msg void OnCallD2();
	afx_msg void OnCallD3();
	afx_msg void OnCallD4();
	afx_msg void OnCallD5();
	afx_msg void OnCallD6();
	afx_msg void OnCallD7();

	afx_msg void OnCallH1();
	afx_msg void OnCallH2();
	afx_msg void OnCallH3();
	afx_msg void OnCallH4();
	afx_msg void OnCallH5();
	afx_msg void OnCallH6();
	afx_msg void OnCallH7();

	afx_msg void OnCallS1();
	afx_msg void OnCallS2();
	afx_msg void OnCallS3();
	afx_msg void OnCallS4();
	afx_msg void OnCallS5();
	afx_msg void OnCallS6();
	afx_msg void OnCallS7();

	afx_msg void OnCallN1();
	afx_msg void OnCallN2();
	afx_msg void OnCallN3();
	afx_msg void OnCallN4();
	afx_msg void OnCallN5();
	afx_msg void OnCallN6();
	afx_msg void OnCallN7();

	afx_msg void OnCallB1();
	afx_msg void OnCallB2();
	
	//������ť
	afx_msg void OnGiveUpScore();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif