#pragma once

#include "MyEdit.h"
#include "AInfo.h"

// CMyListView �Ի���

#include "..\..\..\������\Include\clientshare.h"


//��ť��Ϣ
#define			IDM_START				300
#define			IDM_EXERCISE			301
#define			IDM_QUIT				302
#define			IDM_CHATCOLOR			303
//������Ϣ
#define			IDM_GUIDE				401
#define			IDM_RESET				402
#define			IDM_BOM					403
#define			IDM_STOPIT				404
#define			IDM_CLOG				405

struct UserData_List
{
	LPCTSTR m_name;
	LONG	m_id;
	LPCTSTR m_grade;
	LONG	m_score;
};
struct UserChat_List
{
	UserChat_List(){m_type=0;}
	LPCTSTR m_name;
	CString m_message;
	COLORREF m_color;
	int    m_type;
};

//��������
struct SupperLink
{
	SupperLink(){m_LinkRect_1 = CRect(0,0,0,0);m_LinkRect_2 = CRect(0,0,0,0);m_LinkRect_3 = CRect(0,0,0,0);}
	CString m_URL;
	CRect m_LinkRect_1;
	CRect m_LinkRect_2;
	CRect m_LinkRect_3;
};

class CMyListView : public CDialog , public IMessageProxy
{
	DECLARE_DYNAMIC(CMyListView)
private:
	enum {  CHAT = 0, DATA};
	enum {  CHATMESSAGE=0, SYSMESSAGE, INOROUT};    //��Ϣ����
	int		m_CurrentType;							//��ǰ����
	bool	m_CanScroll;							//�Ƿ���Թ���
	int		m_XPos;									//��׼

	int     m_CurrentSelectUser;

	bool	m_bReDrawBk;
	bool	m_DisplayPropInfo;
	CString m_PropInfoString;

	CDC		memDC;

	HCURSOR					m_hand;
	CLogic					m_Logic;
	CExpression				m_Expression;					//���鴰��
	CBrush					m_Brush;
	IClientKernel			* m_pIClientKernel;				//�ں˽ӿ�

	//button
	CSkinButton				m_btUp;
	CSkinButton				m_btDown;
	CSkinButton				m_btChat;
	CSkinButton				m_btData;

	CSkinButton				m_btAdvise;
	CSkinButton				m_btOrder;
	CSkinButton				m_btQuit2;
	CSkinButton				m_btSet;


	CSkinButton				m_btSend;								//Send
	CSkinButton				m_btColor;								//color;
	CSkinButton				m_btFace;								//face
	CSkinButton				m_btClear;								//clear;

public:
	CSkinButton				m_btExercise;
private:
	CSkinButton				m_btQuit;

	CSkinButton				m_btTeamA;
	CSkinButton				m_btTeamB;
	CSkinButton				m_btTeamFree;

	//����
	CSkinButton				m_btGuide;
	CSkinButton				m_btReset;
	CSkinButton				m_btBom;
	//CSkinButton				m_btStopIt;
//	CSkinButton				m_btClog;


	CSkinImage				m_ImageListViewBg;
	CSkinImage				m_ImageFace;
	CSkinImage				m_ImageLogo;

	//Edit
	CMyEdit					m_messageEdit;                  //��Ϣ��

	//
	CAInfo					m_AInfo;						//������Ϣ
	int						m_AInfoPos;	

public:
	CMyListView(CWnd* pParent = NULL);					// ��׼���캯��
	virtual ~CMyListView();

	CSkinButton				m_btStart;

	//
	CArrayTemplate<UserChat_List> m_Chat;				//������Ϣ����
	CArrayTemplate<UserData_List> m_Data;	

	CArrayTemplate<SupperLink> m_SupperLink;			//����������
	int				m_ChatPos;							//������ʼλ��
	int				m_DataPos;							//�û�����

	int				m_ImplementCount[PROP_MAX_CNT];

	tagUserData*	m_pCurrentUserData;
	
// �Ի�������
	enum { IDD = IDD_LISTINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();

public:
	//ɾ���û�����
	void DeleteUserItem(tagUserData * pUserData);
	//��ȡ�û�����
	void InsertUserItem(const tagUserData * pUserData);
	//���������¼
	bool InsertChat(LPCTSTR pszSendName, LPCTSTR str, COLORREF color);
	//���»���
	void UpdateUserItem(tagUserData * pUserData);
	
	//�����ں�
	void SetClientKernel(IClientKernel * pIClientKernel);

	//��ɫ��ť
	void OnBnClickedColorSet();
	//���鰴ť
	void OnBnClickedExpression();
	//������Ϣ
	LRESULT OnHitExpression(WPARAM wParam, LPARAM lParam);
	//���Ͱ�ť
	void OnBnClickedSendChat();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void OnUp(void);
	void OnDown(void);
protected:
	virtual void OnOK();
	virtual void OnCancel();
public:
	void OnMakeChat(void);
	void OnMakeData(void);
public:
	CRect AutoDrawTextAndImage(CDC * pDC, LPRECT lpRect,CString szString);
	bool OutPutData(CDC * pDC, CString data, CPoint *cpos,LPRECT lpRect,bool textorimage);
	void ClearChat(void);
	// ��������
	void PropZero(void);

	//���þ��
	 bool __cdecl SetRichEditHwnd(CSkinRichEdit * pSkinRichEdit);
	//�����ӿ�
public:
	//�ͷŶ���
	virtual bool __cdecl Release() { if (IsValid()) delete this; return true; }
	//�Ƿ���Ч
	virtual bool __cdecl IsValid() { return AfxIsValidAddress(this,sizeof(CMyListView))?true:false; }
	//�ӿڲ�ѯ
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//������ɫ
	virtual bool __cdecl SetMessageOption(tagMessageOption & MessageOption);

	//���ƽӿ�
public:
	//�����Ļ
	virtual bool __cdecl CleanScreen();
	//���ù���
	virtual bool __cdecl SetAutoScroll(bool bScroll);
	//������Ϣ
	virtual bool __cdecl LoadMessage(LPCTSTR pszFileName);
	//������Ϣ
	virtual bool __cdecl SaveMessage(LPCTSTR pszFileName);
	// �����û���Ϣ
	void SetUserDataItem(tagUserData* pUserData);

	//��Ϣ�ӿ�
public:
	//����ͼƬ
	virtual bool __cdecl InsertImage(CBitmap * pBitmap);
	//����ʱ��
	virtual bool __cdecl InsertSystemTime(COLORREF crColor);
	//�û�����
	virtual bool __cdecl InsertUserName(LPCTSTR pszUserName);
	//ϵͳ��Ϣ
	virtual bool __cdecl InsertSystemString(LPCTSTR pszString, UINT uSize);
	//��ɫ��Ϣ
	virtual bool __cdecl InsertShtickString(LPCTSTR pszString, COLORREF crColor, UINT uSize, bool bNewLine);
	//��ͨ��Ϣ
	virtual bool __cdecl InsertGeneralString(LPCTSTR pszString, COLORREF crColor, UINT uSize, bool bNewLine);
	//�û�����
	virtual bool __cdecl InsertUserChat(LPCTSTR pszSendName, LPCTSTR pszString, COLORREF crColor, UINT uSize);
	//�û�����
	virtual bool __cdecl InsertUserChat(LPCTSTR pszSendName, LPCTSTR pszRecvName, LPCTSTR pszString, COLORREF crColor, UINT uSize);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	void OpenUrl(CString strBuffer);
	// ��Ϸ��ʼ
	void OnStart(void);
	// ��Ϸ����
	void OnQuit(void);
	// ָ����
	void OnGuide(void);
	// ����
	void OnReset(void);
	// ը��
	void OnBom(void);
	// ��ϰ
	void OnExecise(void);
	// ��ʼ����ѡ��
	void InitCtrl(void);
	// ���µ��߰�ť��ʾ״̬
	void UpdateImplementState(void);
	//void OnStopIt(void);
	// �ϰ�
	//void OnClog(void);
	void OnSet(void);
	void OnAdvise(void);
	void OnOrder(void);
	void OnQuit2(void);
};
