#ifndef CLIENT_KERNEL_HEAD_FILE
#define CLIENT_KERNEL_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
#include <windows.h>

#include <string>
#include <deque>

#include <ASSERT.H>

#include "E:\�ɶ�\�����������а�Ȩ_��Ӫ\�����ļ�\GlobalDef.h"
#include "E:\�ɶ�\�����������а�Ȩ_��Ӫ\�����ļ�\GlobalFrame.h"
#include "E:\�ɶ�\�����������а�Ȩ_��Ӫ\�����ļ�\GlobalField.h"

//��Ч��ֵ
#define INVALID_BYTE				((BYTE)(0xFF))						//��Ч��ֵ
#define INVALID_WORD				((WORD)(0xFFFF))					//��Ч��ֵ
#define INVALID_DWORD				((DWORD)(0xFFFFFFFF))				//��Ч��ֵ

//��Ч��ֵ
#define INVALID_TABLE				INVALID_WORD						//��Ч����
#define INVALID_CHAIR				INVALID_WORD						//��Ч����

//�ں���������
#define IPC_MIAN_IPC_KERNEL					0							//�ں�����

//�ں���������
#define IPC_SUB_IPC_CLIENT_CONNECT			1							//����֪ͨ
#define IPC_SUB_IPC_SERVER_ACCEPT			2							//Ӧ��֪ͨ
#define IPC_SUB_IPC_CLIENT_CLOSE			3							//�ر�֪ͨ

#define WM_IPC_MESSAGE				(WM_USER+1511)						//IPC ��Ϣ

//��������
struct tagServerAttribute
{
	WORD							wKindID;							//���ͱ�ʶ
	WORD							wServerID;							//�����ʶ
	WORD							wGameGenre;							//��Ϸ����
	WORD							wChairCount;						//������Ŀ
	DWORD							dwVideoAddr;						//��Ƶ��ַ
	BYTE							cbHideUserInfo;						//������Ϣ
	TCHAR							szKindName[KIND_LEN];				//��������
	TCHAR							szServerName[SERVER_LEN];			//��������
};

//////////////////////////////////////////////////////////////////////////
class CClientKernelSink
{
public:
    virtual void ResetGameFrame()=0;
    virtual void CloseGameFrame()=0;
    virtual bool OnEventSocket(const CMD_Command & Command, const void * pBuffer, WORD wDataSize)=0;
    virtual bool OnEventGameScene(BYTE cbGameStation, bool bLookonOther, void * pBuffer, WORD wDataSize)=0;
    virtual void OnEventUserMemberOrder(tagUserData * pUserData, WORD wChairID, bool bLookonUser)=0;
    virtual void OnEventUserStatus(tagUserData * pUserData, WORD wChairID, bool bLookonUser)=0;
	  virtual bool OnEventUserLeave(tagUserData * pUserData, WORD wChairID, bool bLookonUser)=0;
	  virtual bool OnEventUserEnter(tagUserData * pUserData, WORD wChairID, bool bLookonUserr)=0;
    virtual void OnEventUserScore(tagUserData * pUserData, WORD wChairID, bool bLookonUser)=0;
};                                     

//////////////////////////////////////////////////////////////////////////
//�ͻ����ں�
class CClientKernel 
{
    //������Ϣ
protected:
    WORD								m_wTableID;						//���Ӻ���
    WORD								m_wChairID;						//���Ӻ���

    //������Ϣ
protected:
    DWORD								m_dwUserID;						//�û� I D
    tagServerAttribute	m_ServerAttribute;		//��������

    //��Ϸ��Ϣ
protected:
    BYTE								m_bGameStatus;					//��Ϸ״̬

    //�û���Ϣ
protected:
    tagUserData				  * m_pMeUserItem;				//�Լ���Ϣ
    tagUserData				  * m_pUserItem[MAX_CHAIR];		//�����Ϣ

    //ͨѶ����
protected:
    HANDLE					    m_hShareMemory;					//������
    tagShareMemory	    * m_pShareMemory;				//�����ڴ�

    //�ӿ�ָ��
protected:
    HWND                m_hWndGameFrame;
    CClientKernelSink   *m_pIClientKernelSink;

    //�ŵ�
protected:
    HWND                  m_hWndChannel;
    HWND                  m_hWndGameServer;
    std::deque<IPC_Buffer *> m_IPCBuffers;
    static CClientKernel *m_ClientKernel;


    //��������
public:
    //���캯��
    CClientKernel();
    //��������
    virtual ~CClientKernel();

    //�����ӿ�
public:
    //���ýӿ�
    virtual bool  DestroyClientKernel();
    //�ں�����
    virtual bool  InitClientKernel(HWND hWnd, const std::string &CmdLine, CClientKernelSink* pIUnknownEx);

    //��Ϣ�ӿ�
public:
    //��������
    virtual const tagServerAttribute *  GetServerAttribute() { return &m_ServerAttribute; }

    //״̬�ӿ�
public:
    //��Ϸ״̬
    virtual BYTE  GetGameStatus() { return m_bGameStatus; }
    //��Ϸ״̬
    virtual void  SetGameStatus(BYTE bGameStatus) { m_bGameStatus=bGameStatus; }

    //�û��ӿ�
public:
    //�Լ�λ��
    virtual WORD  GetMeChairID() { return m_wChairID; }
    //��ȡ�Լ�
    virtual const tagUserData *  GetMeUserInfo() { return m_pMeUserItem; }
    //��ȡ���
    virtual const tagUserData *  GetUserInfo(WORD wChairID);

    //���ͽӿ�
public:
    virtual bool  SendData(WORD wMainCmdID, WORD wSubCmdID);

    virtual bool  SendData(WORD wMainCmdID, WORD wSubCmdID, void * pBuffer, WORD wDataSize);
    //���ͺ���
    virtual bool  SendSocketData(WORD wMainCmdID, WORD wSubCmdID);
    //���ͺ���
    virtual bool  SendSocketData(WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize);

    //���ܽӿ�
public:
    //����׼��
    virtual bool  SendUserReady(void * pBuffer, WORD wDataSize);
    //��������
    virtual bool  SendChatMessage(DWORD dwTargetUserID, LPCTSTR pszChatMessage, COLORREF crFontColor);

    //�ŵ��ӿ�
public:
    //�ŵ����ݴ���
    virtual bool  OnChannelMessage(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend);

    //IPC ������
private:
    //������Ϣ
    bool OnIPCSocket(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend);
    //������Ϣ
    bool OnIPCConfig(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend);
    //�û�֪ͨ
    bool OnIPCUser(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend);
    //����֪ͨ
    bool OnIPCControl(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend);

    //�û�����
private:
    tagUserData *  SearchUserItem(DWORD dwUserID);

    bool ActiveUserItem(tagUserData * pUserData);
    //ɾ���û�
    bool DeleteUserItem(DWORD dwUserID);
    //�����û�
    bool UpdateUserItem(DWORD dwUerID, const tagUserScore * pUserScore);
    //�����û�
    bool UpdateUserItem(DWORD dwUserID, BYTE cbUserStatus, WORD wNetDelay);

    //�ڲ�����
private:
    ////���±���
    void UpdateGameTitle();
    //�ں�����
    void ResetClientKernel();

public:
    bool OnCopyData(HWND hWnd, COPYDATASTRUCT * pCopyDataStruct);
    bool OnIPCMessage(WPARAM wParam, LPARAM lParam);

private:
    static LRESULT CALLBACK ChannelWndProc(HWND hWnd, UINT uMsg,WPARAM wParam, LPARAM lParam);
};

//////////////////////////////////////////////////////////////////////////

#endif