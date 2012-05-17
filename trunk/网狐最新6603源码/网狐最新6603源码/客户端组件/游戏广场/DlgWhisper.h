#ifndef DLG_WHISPER_HEAD_FILE
#define DLG_WHISPER_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////////////

//��˵��
class CDlgWhisper;
class CServerViewItem;

//���鶨��
typedef CWHArray<CDlgWhisper *> CDlgWhisperItemArray;					//˽������
typedef CWHArray<IClientUserItem *> CClientUserItemArray;				//�û�����

//////////////////////////////////////////////////////////////////////////////////

//���ýṹ
struct tagWhisper
{
	//���ݱ���
	DWORD							dwServerRule;						//�������
	TCHAR							szServerName[LEN_SERVER];			//��������

	//�ӿڱ���
	ITCPSocket *					pITCPSocket;						//�������
	IClientUserItem *				pIMySelfUserItem;					//�Լ�ָ��
	IUserOrderParser *				pIUserOrderParser;					//�ȼ��ӿ�
	IGameLevelParser *				pIGameLevelParser;					//�ȼ��ӿ�
};

//////////////////////////////////////////////////////////////////////////////////

//˽�Ĵ���
class CDlgWhisper : public CSkinDialog, public IExpressionSink
{
	//״̬����
protected:
	bool							m_bCreateFlag;						//������־

	//���ݱ���
protected:
	DWORD							dwServerRule;						//�������
	TCHAR							szServerName[LEN_SERVER];			//��������

	//�ӿڱ���
protected:
	ITCPSocket *					m_pITCPSocket;						//�������
	IClientUserItem *				m_pIMySelfUserItem;					//�Լ�ָ��
	IUserOrderParser *				m_pIUserOrderParser;				//�ȼ����
	IGameLevelParser *				m_pIGameLevelParser;				//�ȼ��ӿ�

	//��������
protected:
	DWORD							m_dwConversationID;					//�Ự��ʶ
	CClientUserItemArray			m_ClientUserItemArray;				//�û�����

	//���ư�ť
protected:
	CSkinButton						m_btCancel;							//�رհ�ť
	CSkinButton						m_btSendChat;						//��������
	CSkinButton						m_btUserMenu;						//�û��˵�
	CSkinButton						m_btSendControl;					//���Ϳ���

	//���찴ť
protected:
	CSkinButton						m_btChatHistory;					//������ʷ
	CSkinButton						m_btChatButton1;					//��ɫ����
	CSkinButton						m_btChatButton2;					//���鰴ť
	CSkinButton						m_btChatButton3;					//������ť

	//����ؼ�
protected:
	CEdit							m_ChatInput;						//��������
	CRichEditMessage				m_ChatMessage;						//������Ϣ

	//�������
protected:
	CServerViewItem *				m_pServerViewItem;					//����ӿ�
	CExpressionControl *			m_pExpressionControl;				//���鴰��

	//��Դ����
protected:
	CEncircleBMP					m_EncircleChatInput;				//���뻷��
	CEncircleBMP					m_EncircleChatMessage;				//���컷��

	//��������
public:
	//���캯��
	CDlgWhisper();
	//��������
	virtual ~CDlgWhisper();

	//���غ���
protected:
	//�ؼ���
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//���ú���
	virtual BOOL OnInitDialog();
	//ȷ������
	virtual VOID OnOK();
	//ȡ����Ϣ
	virtual VOID OnCancel();
	//�����
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	//���ú���
protected:
	//�滭��Ϣ
	virtual VOID OnDrawClientArea(CDC * pDC, INT nWidth, INT nHeight);
	//�����¼�
	virtual VOID OnExpressionSelect(CExpression * pExpression, tagExpressionInfo * pExpressionInfo);

	//���ú���
public:
	//���ýӿ�
	VOID SetTCPSocket(ITCPSocket * pITCPSocket) { m_pITCPSocket=pITCPSocket; }
	//���ýӿ�
	VOID SetServerViewItem(CServerViewItem * pServerViewItem) { m_pServerViewItem=pServerViewItem; }
	//���ýӿ�
	VOID SetMySelfUserItem(IClientUserItem * pIMySelfUserItem) { m_pIMySelfUserItem=pIMySelfUserItem; }
	//���ýӿ�
	VOID SetUserOrderParser(IUserOrderParser * pIUserOrderParser) { m_pIUserOrderParser=pIUserOrderParser; }
	//���ýӿ�
	VOID SetGameLevelParser(IGameLevelParser * pIGameLevelParser) { m_pIGameLevelParser=pIGameLevelParser; }

	//���ܺ���
public:
	//�����ж�
	bool DisuseEstimate();
	//�û��Ƚ�
	bool CompareUserItem(DWORD dwUserID[], WORD wUserCount);
	//����Ч��
	bool EfficacyUserChat(LPCTSTR pszChatString, WORD wExpressionIndex);
	//��������
	bool CreateWhisperWnd(DWORD dwConversationID, IClientUserItem * pIClientUserItem[], WORD wUserCount, CWnd * pParentWnd);

	//�¼�����
public:
	//�û�����
	VOID OnEventUserEnter(IClientUserItem * pIClientUserItem);
	//�û��뿪
	VOID OnEventUserLeave(IClientUserItem * pIClientUserItem);
	//�û�״̬
	VOID OnEventUserStatus(IClientUserItem * pIClientUserItem);
	//�û�����
	VOID OnEventExpression(IClientUserItem * pIClientUserItem, WORD wItemIndex);
	//�û�˽��
	VOID OnEventWisperChat(IClientUserItem * pIClientUserItem, LPCTSTR pszString, COLORREF rcColor);

	//�ڲ�����
protected:
	//���±���
	VOID RectifyTitle();
	//�����ؼ�
	VOID RectifyControl(INT nWidth, INT nHeight);
	//������
	VOID SortUserIDData(DWORD dwUserID[], WORD wItemCount);
	//�滭����
	VOID DrawFunctionArea(CDC * pDC, INT nXPos, INT nYPos, INT nWidth, INT nHeight);
	//�滭�û�
	VOID DrawUserInfoArea(CDC * pDC, INT nXPos, INT nYPos, INT nWidth, INT nHeight);

	//��������
protected:
	//���ͱ���
	bool SendExpressionPacket(IClientUserItem * pIClientUserItem, WORD wItemIndex);
	//��������
	bool SendWhisperChatPacket(IClientUserItem * pIClientUserItem, LPCTSTR pszChatString);
	//���ͻỰ
	bool SendColloquyChatPacket(IClientUserItem * pIClientUserItem[], WORD wUserCount, LPCTSTR pszChatString);

	//��ť��Ϣ
protected:
	//�û��˵�
	VOID OnBnClickedUserMenu();
	//������Ϣ
	VOID OnBnClickedSendChat();
	//���Ϳ���
	VOID OnBnClickedSendControl();
	//���찴ť
	VOID OnBnClickedChatButton1();
	//���찴ť
	VOID OnBnClickedChatButton2();
	//���찴ť
	VOID OnBnClickedChatButton3();

	//�˵�����
protected:
	//������ɫ
	VOID OnSelectMoreColor();
	//ѡ����ɫ
	VOID OnSelectChatColor(UINT nCommandID);

	//��Ϣ����
protected:
	//������Ϣ
	VOID OnNcDestroy();
	//λ����Ϣ
	VOID OnSize(UINT nType, INT cx, INT cy);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif