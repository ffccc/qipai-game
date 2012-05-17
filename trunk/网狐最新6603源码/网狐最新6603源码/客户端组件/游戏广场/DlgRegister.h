#ifndef DLG_REGISTER_HEAD_FILE
#define DLG_REGISTER_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"
#include "WndValidateCode.h"

//////////////////////////////////////////////////////////////////////////////////

//ע�ᴰ��
class CDlgRegister : public CDialog
{
	//��������
public:
	WORD							m_wFaceID;							//ͷ���ʶ
	BYTE							m_cbGender;							//�û��Ա�
	TCHAR							m_szAccounts[LEN_ACCOUNTS];			//ע���ʺ�
	TCHAR							m_szNickName[LEN_NICKNAME];			//ע���ǳ�
	TCHAR							m_szSpreader[LEN_ACCOUNTS];			//�Ƽ��ʺ�
	TCHAR							m_szLogonPass[LEN_PASSWORD];		//��¼����
	TCHAR							m_szInsurePass[LEN_PASSWORD];		//��������
	TCHAR							m_szPassPortID[LEN_PASS_PORT_ID];	//֤������
	TCHAR							m_szCompellation[LEN_COMPELLATION];	//��ʵ����

	//���ñ���
protected:
	BYTE							m_cbRemPassword;					//��ס����
	BYTE							m_cbLogonPassLevel;					//����ǿ��
	BYTE							m_cbInsurePassLevel;				//����ǿ��

	//�ؼ�����
public:
	CSkinEdit						m_edAccounts;						//ע���ʺ�
	CSkinEdit						m_edNickName;						//ע���ʺ�
	CSkinEdit						m_edSpreader;						//�Ƽ��û�
	CSkinEdit						m_edPassPortID;						//���֤��
	CSkinEdit						m_edLogonPass1;						//��¼����
	CSkinEdit						m_edLogonPass2;						//��¼����
	CSkinEdit						m_edInsurePass1;					//��������
	CSkinEdit						m_edInsurePass2;					//��������
	CSkinEdit						m_edCompellation;					//��ʵ����
	CSkinEdit						m_edValidateCode;					//��֤����

	//�ؼ�����
protected:
	CSkinButton						m_btQuit;							//ȡ����ť
	CSkinButton						m_btEnter;							//ע�ᰴť
	CSkinButton						m_btCancel;							//�رհ�ť
	CSkinButton						m_btSelectFace;						//ѡ��ť

	//�ؼ�����
protected:
	CBrush							m_brBrush;							//������ˢ
	CFaceItemView					m_FaceItemView;						//ͷ����ͼ
	CSkinHyperLink					m_ChangeValidate;					//������֤

	//�������
protected:
	CSkinLayered					m_SkinLayered;						//�ֲ㴰��
	CWndValidateCode				m_WndValidateCode;					//��֤�ؼ�
	CFaceSelectControlHelper		m_FaceSelectControl;				//ͷ��ѡ��

	//��������
public:
	//���캯��
	CDlgRegister();
	//��������
	virtual ~CDlgRegister();

	//���غ���
public:
	//�ؼ���
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//���ú���
	virtual BOOL OnInitDialog();
	//ȷ������
	virtual VOID OnOK();
	//ȡ������
	virtual VOID OnCancel();

	//���ܺ���
public:
	//��������
	WORD ConstructRegisterPacket(BYTE cbBuffer[], WORD wBufferSize);

	//��������
private:
	//��ȡ��Ϣ
	bool GetInformation();
	//��ȡ�ַ�
	VOID GetControlString(UINT uControlID, TCHAR szString[], WORD wMaxCount);

	//��Ϣӳ��
public:
	//�رհ�ť
	VOID OnBnClickedQuit();
	//��������
	VOID OnEnChangeLogonPass();
	//��������
	VOID OnEnChangeInsurePass();
	//ͷ��ѡ��
	VOID OnBnClickedSelectFace();
	//������֤
	VOID OnBnClickedChangeValidate();

	//ϵͳ��Ϣ
protected:
	//�滭����
	BOOL OnEraseBkgnd(CDC * pDC);
	//��ʾ��Ϣ
	VOID OnShowWindow(BOOL bShow, UINT nStatus);
	//�����Ϣ
	VOID OnLButtonDown(UINT nFlags, CPoint Point);
	//λ�øı�
	VOID OnWindowPosChanged(WINDOWPOS * lpWndPos);
	//�ؼ���ɫ
	HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif