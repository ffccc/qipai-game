#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "ChessBorad.h"

//////////////////////////////////////////////////////////////////////////

#define VER_MANUAL					1									//���װ汾

//�ļ�ͷ��Ϣ
struct tagChessManualHead
{
	WORD							wVersion;							//�汾����
	BYTE							cbChessColor;						//������ɫ
	DWORD							dwManualDate;						//����ʱ��
	DWORD							dwManualCount;						//������Ŀ
};

//////////////////////////////////////////////////////////////////////////

//���״���
class CChessManual : public CDialog
{
	//���ױ���
public:
	WORD							m_wChessStep;						//���岽��
	CArrayTemplate<tagChessManual>	m_ChessManualInfo;					//������Ϣ

	//�ؼ�����
public:
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
	CGameLogic *					m_pDlgGameLogic;					//��Ϸ�߼�

	//�ؼ�����
public:
	CSkinButton						m_btOpen;							//��ť�ؼ�
	CSkinButton						m_btPreserve;						//��ť�ؼ�
	CSkinButton						m_btReLoad;							//��ť�ؼ�
	CSkinButton						m_btFirst;							//��ť�ؼ�
	CSkinButton						m_btBefore;							//��ť�ؼ�
	CSkinButton						m_btNext;							//��ť�ؼ�
	CSkinButton						m_btLast;							//��ť�ؼ�
	CSkinButton						m_btCancel;							//��ť�ؼ�
	CListBox						m_ManualList;						//�����б�
	CChessBorad						m_ChessBorad;						//��Ϸ����

	//��Դ����
protected:
	CSkinImage						m_ImageBack;						//��ͼ��Դ

	//��������
public:
	//���캯��
	CChessManual();
	//��������
	virtual ~CChessManual();

	//���غ���
protected:
	//�ؼ���
	virtual void DoDataExchange(CDataExchange * pDX);
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ����Ϣ
	virtual void OnOK() { return; }
	//ȡ����Ϣ
	virtual void OnCancel() { DestroyWindow(); }

	//��������
protected:
	//���¿���
	void UpdateControl();
	//��������
	void UpdateManualList();

	//��Ϣ����
protected:
	//�ػ�����
	afx_msg void OnPaint();
	//�滭����
	afx_msg BOOL OnEraseBkgnd(CDC * pDC);
	//ѡ��ı�
	afx_msg void OnLbnSelchangeManualList();
	//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	//��ť��Ϣ
protected:
	//�򿪰�ť
	afx_msg void OnOpen();
	//���水ť
	afx_msg void OnPreserve();
	//ˢ�°�ť
	afx_msg void OnReLoad();
	//��ǰ��ť
	afx_msg void OnFirst();
	//���ְ�ť
	afx_msg void OnBefore();
	//���ְ�ť
	afx_msg void OnNext();
	//���ť
	afx_msg void OnLast();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
