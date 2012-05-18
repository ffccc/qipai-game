#ifndef DLG_CHESS_MANUAL_HEAD_FILE
#define DLG_CHESS_MANUAL_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "ChessBorad.h"
#include "ManualList.h"

//////////////////////////////////////////////////////////////////////////

#define VER_MANUAL_FILE				1									//���װ汾

//�ļ�ͷ��Ϣ
struct tagChessManualHead
{
	//������Ϣ
	WORD							wVersion;							//�汾����
	WORD							wItemCount;							//������Ŀ
	SYSTEMTIME						ManualDateTime;						//����ʱ��

	//��Ϸ��Ϣ
	WORD							wBankerUser;						//ׯ���û�
	TCHAR							szUserAccounts[2][LEN_ACCOUNTS];	//�û��ʺ�
};

//////////////////////////////////////////////////////////////////////////

//�����ļ�
class CChessManualFile
{
	//������Ϣ
protected:
	SYSTEMTIME						m_ManualDateTime;					//����ʱ��

	//��Ϸ��Ϣ
protected:
	WORD							m_wBankerUser;						//ׯ���û�
	TCHAR							m_szUserAccounts[2][LEN_ACCOUNTS];	//�û��ʺ�

	//��������
public:
	//���캯��
	CChessManualFile();
	//��������
	virtual ~CChessManualFile();

	//����ʱ��
public:
	//����ʱ��
	SYSTEMTIME GetManualDateTime() { return m_ManualDateTime; }
	//����ʱ��
	VOID SetManualDateTime(SYSTEMTIME & ManualDateTime) { m_ManualDateTime=ManualDateTime; }

	//ׯ����Ϣ
public:
	//ׯ���û�
	WORD GetBankerUser() { return m_wBankerUser; }
	//����ׯ��
	VOID SetBankerUser(WORD wBankerUser) { m_wBankerUser=wBankerUser; }

	//�û���Ϣ
public:
	//��ȡ�û�
	TCHAR * GetUserAccounts(WORD wChairID);
	//�����û�
	bool SetUserAccounts(WORD wChairID, TCHAR * pszAccounts);

	//���ܺ���
public:
	//��������
	bool LoadChessManual(LPCTSTR pszFileName, CChessManualArray & ChessManualArray);
	//��������
	bool SaveChessManual(LPCTSTR pszFileName, CChessManualArray & ChessManualArray);
};

//////////////////////////////////////////////////////////////////////////

//������ͼ
class CDlgChessManual : public CDialog
{
	//���̱���
protected:
	WORD							m_wStepCount;						//���岽��
	tagChessItem					m_ChessItemView[2][COUNT_CHESS];	//������Ϣ
	tagChessItem *					m_ChessBoradView[COUNT_X][COUNT_Y];	//������Ϣ

	//�������
protected:
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
	CChessBorad						m_ChessBorad;						//���̿ؼ�
	CManualList						m_ManualList;						//�����б�
	CToolTipCtrl					m_ToolTipCtrl;						//��ʾ�ؼ�
	CChessManualFile				m_ChessManualFile;					//�����ļ�

	//���ư�ť
protected:
	CSkinButton						m_btHide;							//���ذ�ť
	CSkinButton						m_btCancel;							//�رհ�ť
	CSkinButton						m_btManualLoad;						//���ذ�ť
	CSkinButton						m_btManualSave;						//���水ť
	CSkinButton						m_btManualNote;						//��˵��ť

	//���ư�ť
protected:
	CSkinButton						m_btManualHead;						//���װ�ť
	CSkinButton						m_btManualLast;						//���װ�ť
	CSkinButton						m_btManualPlay;						//���װ�ť
	CSkinButton						m_btManualNext;						//���װ�ť
	CSkinButton						m_btManualTail;						//���װ�ť

	//��Դ����
protected:
	CSkinImage						m_ImageViewBack;					//������Դ

	//��������
public:
	//���캯��
	CDlgChessManual();
	//��������
	virtual ~CDlgChessManual();

	//���غ���
protected:
	//�ؼ���
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ����Ϣ
	virtual VOID OnOK();
	//ȡ����Ϣ
	virtual VOID OnCancel();

	//���ܺ���
public:
	//��������
	bool SetChessManual(CChessManualArray & ChessManualArray, CChessManualFile & ChessManualFile);

	//�ڲ�����
private:
	//���¿���
	VOID UpdateManualControl();
	//���׶�λ
	VOID OrientationManual(WORD wStepCount);
	//�����ؼ�
	VOID RectifyGameView(INT nWidth, INT nHeight);

	//���װ�ť
protected:
	//���װ�ť
	VOID OnBnClickedHead();
	//���װ�ť
	VOID OnBnClickedLast();
	//���װ�ť
	VOID OnBnClickedPlay();
	//���װ�ť
	VOID OnBnClickedNext();
	//���װ�ť
	VOID OnBnClickedTail();

	//���ư�ť
protected:
	//���ذ�ť
	VOID OnBnClickedHide();
	//�򿪰�ť
	VOID OnBnClickedLoad();
	//���水ť
	VOID OnBnClickedSave();
	//��˵��ť
	VOID OnBnClickedNote();

	//��Ϣӳ��
protected:
	//�ػ�����
	VOID OnPaint();
	//ʱ����Ϣ
	VOID OnTimer(UINT nIDEvent);
	//�滭����
	BOOL OnEraseBkgnd(CDC * pDC);
	//ѡ��ı�
	VOID OnLbnSelchangeManualList();
	//�����Ϣ
	VOID OnLButtonDown(UINT nFlags, CPoint Point);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif
