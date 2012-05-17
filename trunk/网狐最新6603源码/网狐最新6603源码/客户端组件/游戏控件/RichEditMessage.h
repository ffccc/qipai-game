#ifndef RICH_EDIT_MESSAGE_HEAD_FILE
#define RICH_EDIT_MESSAGE_HEAD_FILE

#pragma once

#include "ShareControlHead.h"

//////////////////////////////////////////////////////////////////////////////////

//��ɫ����
#define COLOR_DEBUG					RGB(0,128,128)						//������ɫ
#define COLOR_EXCEPTION				RGB(200,0,0)						//�쳣��ɫ

//��ɫ����
#define COLOR_WARN					RGB(255,128,0)						//������ɫ
#define COLOR_EVENT					RGB(125,125,125)					//�¼���Ϣ
#define COLOR_NORMAL				RGB(0,0,0)							//��ͨ��Ϣ
#define COLOR_SYSTEM_STRING			RGB(10,10,10)						//ϵͳ��Ϣ

//˽����ɫ
#define COLOR_USER					RGB(0,0,255)						//�Է���ɫ
#define COLOR_MYSELF				RGB(0,128,64)						//�Լ���ɫ

//λ�ö���
#define STRING_OFFSET				90									//����λ��

//////////////////////////////////////////////////////////////////////////////////

//��˵��
class CExpressionManager;

//�ַ���Ϣ
class SHARE_CONTROL_CLASS CRichEditMessage : public CSkinRichEdit, public IStringMessage
{
	//ͼƬ��Դ
protected:
	COLORREF						m_crFillColor;						//�����ɫ
	CDataObject						m_ImageDataSystem;					//��Դ����
	CDataObject						m_ImageDataPrompt;					//��Դ����
	CDataObject						m_ImageDataAffiche;					//��Դ����

	//�������
protected:
	CSkinScrollBar					m_SkinScrollBar;					//��������

	//�������
protected:
	CExpressionManager *			m_pExpressionManager;				//�������
	IRichEditMessageSink *			m_pIRichEditMessageSink;			//�ӿڱ���

	//��������
public:
	//���캯��
	CRichEditMessage();
	//��������
	virtual ~CRichEditMessage();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { return; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//�¼���Ϣ
public:
	//�����¼�
	virtual bool InsertUserEnter(LPCTSTR pszUserName);
	//�뿪�¼�
	virtual bool InsertUserLeave(LPCTSTR pszUserName);
	//�����¼�
	virtual bool InsertUserOffLine(LPCTSTR pszUserName);

	//�ַ���Ϣ
public:
	//��ͨ��Ϣ
	virtual bool InsertNormalString(LPCTSTR pszString);
	//ϵͳ��Ϣ
	virtual bool InsertSystemString(LPCTSTR pszString);
	//��ʾ��Ϣ
	virtual bool InsertPromptString(LPCTSTR pszString);
	//������Ϣ
	virtual bool InsertAfficheString(LPCTSTR pszString);

	//������Ϣ
public:
	//������Ϣ
	virtual bool InsertCustomString(LPCTSTR pszString, COLORREF crColor);
	//������Ϣ
	virtual bool InsertCustomString(LPCTSTR pszString, COLORREF crColor, COLORREF crBackColor);

	//������Ϣ
public:
	//�û�����
	virtual bool InsertExpression(LPCTSTR pszSendUser, LPCTSTR pszImagePath);
	//�û�����
	virtual bool InsertExpression(LPCTSTR pszSendUser, LPCTSTR pszImagePath, bool bMyselfString);
	//�û�����
	virtual bool InsertExpression(LPCTSTR pszSendUser, LPCTSTR pszRecvUser, LPCTSTR pszImagePath);

	//������Ϣ
public:
	//�û�����
	virtual bool InsertUserChat(LPCTSTR pszSendUser, LPCTSTR pszString, COLORREF crColor);
	//�û�����
	virtual bool InsertUserChat(LPCTSTR pszSendUser, LPCTSTR pszRecvUser, LPCTSTR pszString, COLORREF crColor);
	//�û�˽��
	virtual bool InsertWhisperChat(LPCTSTR pszSendUser, LPCTSTR pszString, COLORREF crColor, bool bMyselfString);

	//���غ���
protected:
	//�ؼ���
	virtual VOID PreSubclassWindow();

	//���ú���
public:
	//���ýӿ�
	bool SetRichEditMessageSink(IUnknownEx * pIUnknownEx);
	//�������
	bool SetExpressionManager(CExpressionManager * pExpressionManager, COLORREF crFillColor);

	//���ܺ���
public:
	//����ʱ��
	bool InsertSystemTime(COLORREF crColor);
	//�û�����
	bool InsertUserAccounts(LPCTSTR pszAccounts);
	//�����ַ�
	bool TranslateInsertString(LPCTSTR pszString, COLORREF crColor);

	//�ڲ�����
protected:
	//����ͼƬ
	bool LoadRichEditImage(HINSTANCE hInstance, UINT uResourceID, CDataObject&ImageDataObject);

	//��Ϣӳ��
protected:
	//������Ϣ
	VOID OnEventLink(NMHDR * pNMHDR, LRESULT * pResult);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif