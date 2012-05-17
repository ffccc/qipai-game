#ifndef PASSWORD_CONTROL_HEAD_FILE
#define PASSWORD_CONTROL_HEAD_FILE

#pragma once

#include "ShareControlHead.h"

//////////////////////////////////////////////////////////////////////////////////

//�������
class SHARE_CONTROL_CLASS CPasswordKeyboard : public CDialog
{
	//���ñ���
protected:
	INT								m_nRandLine[4];						//�������
	INT								m_nItemCount[4];					//�ַ���Ŀ
	TCHAR							m_szKeyboradChar[4][2][14];			//�����ַ�

	//����״̬
protected:
	bool							m_bShiftStatus;						//����״̬
	bool							m_bCapsLockStatus;					//����״̬

	//״̬����
protected:
	WORD							m_wHoverRow;						//��������
	WORD							m_wHoverLine;						//��������
	bool							m_bMouseDown;						//����״̬

	//��Դ��С
protected:
	CSize							m_SizeImageItem1;					//��Դ��С
	CSize							m_SizeImageItem2;					//��Դ��С
	CSize							m_SizeImageItem3;					//��Դ��С
	CSize							m_SizeBackGround;					//������С
	CSize							m_SizeImageButton;					//��Դ��С

	//��������
public:
	//���캯��
	CPasswordKeyboard();
	//��������
	virtual ~CPasswordKeyboard();

	//���غ���
protected:
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//��������
	virtual BOOL OnInitDialog();
	//ȷ������
	virtual VOID OnOK() { return; }
	//ȡ����Ϣ
	virtual VOID OnCancel() { return; }

	//��������
protected:
	//����λ��
	VOID SetCurrentStation(CPoint MousePoint);
	//�������
	WORD GetVirualKeyCode(WORD wHoverLine, WORD wHoverRow);

	//��Ϣ����
protected:
	//����滭
	VOID OnNcPaint();
	//�滭����
	BOOL OnEraseBkgnd(CDC * pDC);
	//ʧȥ����
	VOID OnKillFocus(CWnd * pNewWnd);
	//�����Ϣ
	VOID OnLButtonUp(UINT nFlags, CPoint Point);
	//�����Ϣ
	VOID OnLButtonDown(UINT nFlags, CPoint Point);
	//�����Ϣ
	BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

//����༭
class SHARE_CONTROL_CLASS CPasswordControl : public CWnd
{
	//��Ԫ����
	friend class CPasswordKeyboard;

	//״̬����
protected:
	bool							m_bModify;							//�޸ı�־
	bool							m_bFalsity;							//��ٱ�־
	bool							m_bDrawBorad;						//�滭�߿�
	bool							m_bRenderImage;						//��Ⱦ��־
	TCHAR							m_szPassword[LEN_PASSWORD];			//�û�����

	//�ؼ�����
protected:
	CSkinEditEx						m_edPassword;						//����ؼ�
	CSkinButton						m_btKeyboard;						//���̰�ť
	CPasswordKeyboard				m_PasswordKeyboard;					//�������

	//��������
public:
	//���캯��
	CPasswordControl();
	//��������
	virtual ~CPasswordControl();

	//���غ���
protected:
	//�󶨺���
	virtual VOID PreSubclassWindow();

	//״̬����
public:
	//�޸�״̬
	bool IsModifyPassword() { return m_bModify; }
	//���״̬
	bool IsFalsityPassword() { return m_bFalsity; }

	//���ܺ���
public:
	//�߿�����
	VOID SetDrawBorad(bool bDrawBorad);
	//��������
	VOID SetUserPassword(LPCTSTR pszPassword);
	//��ȡ����
	LPCTSTR GetUserPassword(TCHAR szPassword[LEN_PASSWORD]);

	//��Ϣ����
protected:
	//�ػ���Ϣ
	VOID OnPaint();
	//�ػ���Ϣ
	VOID OnNcPaint();
	//�滭����
	BOOL OnEraseBkgnd(CDC * pDC);
	//��ȡ����
	VOID OnSetFocus(CWnd * pOldWnd);

	//�ؼ���Ϣ
protected:
	//����ı�
	VOID OnEnChangePassword();
	//���̰�ť
	VOID OnBnClickedKeyboard();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif