#ifndef CLIENT_KERNEL_HEAD_FILE
#define CLIENT_KERNEL_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
#include <windows.h>

#include <string>
#include <deque>

#include <ASSERT.H>

#include "E:\成都\幸运棋牌所有版权_运营\公共文件\GlobalDef.h"
#include "E:\成都\幸运棋牌所有版权_运营\公共文件\GlobalFrame.h"
#include "E:\成都\幸运棋牌所有版权_运营\公共文件\GlobalField.h"

//无效数值
#define INVALID_BYTE				((BYTE)(0xFF))						//无效数值
#define INVALID_WORD				((WORD)(0xFFFF))					//无效数值
#define INVALID_DWORD				((DWORD)(0xFFFFFFFF))				//无效数值

//无效数值
#define INVALID_TABLE				INVALID_WORD						//无效桌子
#define INVALID_CHAIR				INVALID_WORD						//无效椅子

//内核主命令码
#define IPC_MIAN_IPC_KERNEL					0							//内核命令

//内核子命令码
#define IPC_SUB_IPC_CLIENT_CONNECT			1							//连接通知
#define IPC_SUB_IPC_SERVER_ACCEPT			2							//应答通知
#define IPC_SUB_IPC_CLIENT_CLOSE			3							//关闭通知

#define WM_IPC_MESSAGE				(WM_USER+1511)						//IPC 消息

//房间属性
struct tagServerAttribute
{
	WORD							wKindID;							//类型标识
	WORD							wServerID;							//房间标识
	WORD							wGameGenre;							//游戏类型
	WORD							wChairCount;						//椅子数目
	DWORD							dwVideoAddr;						//视频地址
	BYTE							cbHideUserInfo;						//隐藏信息
	TCHAR							szKindName[KIND_LEN];				//类型名字
	TCHAR							szServerName[SERVER_LEN];			//房间名称
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
//客户端内核
class CClientKernel 
{
    //房间信息
protected:
    WORD								m_wTableID;						//桌子号码
    WORD								m_wChairID;						//椅子号码

    //固有信息
protected:
    DWORD								m_dwUserID;						//用户 I D
    tagServerAttribute	m_ServerAttribute;		//房间属性

    //游戏信息
protected:
    BYTE								m_bGameStatus;					//游戏状态

    //用户信息
protected:
    tagUserData				  * m_pMeUserItem;				//自己信息
    tagUserData				  * m_pUserItem[MAX_CHAIR];		//玩家信息

    //通讯变量
protected:
    HANDLE					    m_hShareMemory;					//共享句柄
    tagShareMemory	    * m_pShareMemory;				//共享内存

    //接口指针
protected:
    HWND                m_hWndGameFrame;
    CClientKernelSink   *m_pIClientKernelSink;

    //信道
protected:
    HWND                  m_hWndChannel;
    HWND                  m_hWndGameServer;
    std::deque<IPC_Buffer *> m_IPCBuffers;
    static CClientKernel *m_ClientKernel;


    //函数定义
public:
    //构造函数
    CClientKernel();
    //析构函数
    virtual ~CClientKernel();

    //驱动接口
public:
    //设置接口
    virtual bool  DestroyClientKernel();
    //内核配置
    virtual bool  InitClientKernel(HWND hWnd, const std::string &CmdLine, CClientKernelSink* pIUnknownEx);

    //信息接口
public:
    //房间属性
    virtual const tagServerAttribute *  GetServerAttribute() { return &m_ServerAttribute; }

    //状态接口
public:
    //游戏状态
    virtual BYTE  GetGameStatus() { return m_bGameStatus; }
    //游戏状态
    virtual void  SetGameStatus(BYTE bGameStatus) { m_bGameStatus=bGameStatus; }

    //用户接口
public:
    //自己位置
    virtual WORD  GetMeChairID() { return m_wChairID; }
    //获取自己
    virtual const tagUserData *  GetMeUserInfo() { return m_pMeUserItem; }
    //获取玩家
    virtual const tagUserData *  GetUserInfo(WORD wChairID);

    //发送接口
public:
    virtual bool  SendData(WORD wMainCmdID, WORD wSubCmdID);

    virtual bool  SendData(WORD wMainCmdID, WORD wSubCmdID, void * pBuffer, WORD wDataSize);
    //发送函数
    virtual bool  SendSocketData(WORD wMainCmdID, WORD wSubCmdID);
    //发送函数
    virtual bool  SendSocketData(WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize);

    //功能接口
public:
    //发送准备
    virtual bool  SendUserReady(void * pBuffer, WORD wDataSize);
    //发送聊天
    virtual bool  SendChatMessage(DWORD dwTargetUserID, LPCTSTR pszChatMessage, COLORREF crFontColor);

    //信道接口
public:
    //信道数据处理
    virtual bool  OnChannelMessage(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend);

    //IPC 子命令
private:
    //网络消息
    bool OnIPCSocket(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend);
    //配置信息
    bool OnIPCConfig(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend);
    //用户通知
    bool OnIPCUser(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend);
    //控制通知
    bool OnIPCControl(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend);

    //用户管理
private:
    tagUserData *  SearchUserItem(DWORD dwUserID);

    bool ActiveUserItem(tagUserData * pUserData);
    //删除用户
    bool DeleteUserItem(DWORD dwUserID);
    //更新用户
    bool UpdateUserItem(DWORD dwUerID, const tagUserScore * pUserScore);
    //更新用户
    bool UpdateUserItem(DWORD dwUserID, BYTE cbUserStatus, WORD wNetDelay);

    //内部函数
private:
    ////更新标题
    void UpdateGameTitle();
    //内核重置
    void ResetClientKernel();

public:
    bool OnCopyData(HWND hWnd, COPYDATASTRUCT * pCopyDataStruct);
    bool OnIPCMessage(WPARAM wParam, LPARAM lParam);

private:
    static LRESULT CALLBACK ChannelWndProc(HWND hWnd, UINT uMsg,WPARAM wParam, LPARAM lParam);
};

//////////////////////////////////////////////////////////////////////////

#endif