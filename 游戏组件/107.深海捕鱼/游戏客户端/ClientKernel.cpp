#include "xlbe.h"
#include "ClientKernel.h"


//////////////////////////////////////////////////////////////////////////
CClientKernel *CClientKernel::m_ClientKernel = 0;

//////////////////////////////////////////////////////////////////////////

//构造函数
CClientKernel::CClientKernel()
{
    m_dwUserID=0L;
    m_wTableID=INVALID_TABLE;
    m_wChairID=INVALID_CHAIR;

    //房间信息
    memset(&m_ServerAttribute,0,sizeof(m_ServerAttribute));

    //游戏信息
    m_pMeUserItem=NULL;
    m_bGameStatus=GS_FREE;
    for (int i=0; i<MAX_CHAIR; i++)
        m_pUserItem[i] = NULL;

    //通讯变量
    m_hShareMemory=NULL;
    m_pShareMemory=NULL;

    m_hWndGameFrame = NULL;
    m_pIClientKernelSink = NULL;

    m_hWndChannel = NULL;
    m_hWndGameServer = NULL;

    m_ClientKernel = this;

    return;
}

//析构函数
CClientKernel::~CClientKernel(void)
{
    DestroyClientKernel();

    return;
}

//设置接口
bool  CClientKernel::DestroyClientKernel()
{
    SendData(IPC_MIAN_IPC_KERNEL,IPC_SUB_IPC_CLIENT_CLOSE);

    //关闭窗口
    if (m_hWndChannel!=NULL)
    {
        DestroyWindow(m_hWndChannel);
        m_hWndChannel = NULL;
    }

    //设置接口
    m_ClientKernel = NULL;
    m_pIClientKernelSink=NULL;

    std::deque<IPC_Buffer *>::iterator i=m_IPCBuffers.begin();
    for (; i!=m_IPCBuffers.end(); ++i)
    {
        delete (*i);
    }
    m_IPCBuffers.clear();

    for (int i=0; i<MAX_CHAIR; i++)
    {
        if (m_pUserItem[i] != NULL)
        {
            delete m_pUserItem[i];
            m_pUserItem[i] = NULL;
        }
    }

    //共享内存
    if (m_pShareMemory!=NULL)
    {
        UnmapViewOfFile(m_pShareMemory);
        m_pShareMemory=NULL;
    }
    if (m_hShareMemory!=NULL)
    {
        CloseHandle(m_hShareMemory);
        m_hShareMemory=NULL;
    }

    return true;
}

//内核配置
bool  CClientKernel::InitClientKernel(HWND hWnd, const std::string &CmdLine, CClientKernelSink* pIUnknownEx)
{

    if (CmdLine.empty()) return false;

    //获取框架
    m_hWndGameFrame=hWnd;
    m_pIClientKernelSink = pIUnknownEx;

    //创建窗口
    if (m_hWndChannel==NULL) 
    {
        WNDCLASS wc = { CS_HREDRAW | CS_VREDRAW, 
            CClientKernel::ChannelWndProc, 0, 0, GetModuleHandle(0),
            LoadIcon(0, IDI_APPLICATION), LoadCursor(NULL, IDC_ARROW),
            (HBRUSH)GetStockObject(BLACK_BRUSH), 0, "ChannelWND"};
        RegisterClass(&wc);

        //创建窗口
        m_hWndChannel = CreateWindowEx(0, "ChannelWND","ChannelWND",WS_CHILD,0,0,0,0,GetDesktopWindow(),0,GetModuleHandle(0),0);
    }

    //命令行处理
    if (CmdLine.empty()!=true)
    {
        std::string::size_type pos1;
        std::string::size_type pos2;
        std::string szRoomToken;
        std::string szComLine(CmdLine);

        pos1 = szComLine.find("/RoomToken:");
        if (pos1 != std::string::npos)
        {
            pos2 = szComLine.find_first_of("/", pos1+10);
            szRoomToken = szComLine.substr(pos1+11, pos2-pos1-12);
        }

        //共享内存
        if (!szRoomToken.empty())
        {
            m_hShareMemory=OpenFileMapping(FILE_MAP_ALL_ACCESS,FALSE,szRoomToken.c_str());
            if (m_hShareMemory==NULL) return false;

            m_pShareMemory=(tagShareMemory *)MapViewOfFile(m_hShareMemory,FILE_MAP_ALL_ACCESS,0,0,0);
            if (m_pShareMemory==NULL) return false;

            if (m_pShareMemory->wDataSize<sizeof(tagShareMemory)) return false;
            m_pShareMemory->hWndGameFrame=m_hWndGameFrame;

            m_hWndGameServer = m_pShareMemory->hWndGameServer;

            SendData(IPC_MIAN_IPC_KERNEL,IPC_SUB_IPC_CLIENT_CONNECT);
        }
    }


    //更新标题
    UpdateGameTitle();

    return true;
}

//获取玩家
const tagUserData *  CClientKernel::GetUserInfo(WORD wChairID)
{
    if (wChairID>=MAX_CHAIR) return NULL;
    if (wChairID>=m_ServerAttribute.wChairCount) return NULL;
    return m_pUserItem[wChairID];
}

bool CClientKernel::SendData(WORD wMainCmdID, WORD wSubCmdID)
{
    if (!IsWindow(m_hWndGameServer)) return false;

    //构造数据
    IPC_Head IPCHead;
    IPCHead.wVersion=IPC_VER;
    IPCHead.wDataSize=sizeof(IPC_Head);
    IPCHead.wMainCmdID=wMainCmdID;
    IPCHead.wSubCmdID=wSubCmdID;

    //发送数据
    COPYDATASTRUCT CopyDataStruct;
    CopyDataStruct.lpData=&IPCHead;
    CopyDataStruct.cbData=IPCHead.wDataSize;
    CopyDataStruct.dwData=IPC_IDENTIFIER;
    SendMessage(m_hWndGameServer,WM_COPYDATA,(LPARAM)m_hWndChannel,(WPARAM)&CopyDataStruct);

    return true;
}

//发送数据
bool CClientKernel::SendData(WORD wMainCmdID, WORD wSubCmdID, void * pBuffer, WORD wDataSize)
{
    //效验数据
    if (wDataSize>IPC_PACKAGE) return false;
    if (!IsWindow(m_hWndGameServer)) return false;

    //构造数据
    BYTE cbSendBuffer[IPC_PACKAGE+sizeof(IPC_Head)];
    IPC_Head * pIPCHead=(IPC_Head *)cbSendBuffer;
    pIPCHead->wVersion=IPC_VER;
    pIPCHead->wDataSize=sizeof(IPC_Head);
    pIPCHead->wMainCmdID=wMainCmdID;
    pIPCHead->wSubCmdID=wSubCmdID;
    if (pBuffer!=NULL)
    {
        pIPCHead->wDataSize+=wDataSize;
        CopyMemory(cbSendBuffer+sizeof(IPC_Head),pBuffer,wDataSize);
    }

    //发送数据
    COPYDATASTRUCT CopyDataStruct;
    CopyDataStruct.lpData=cbSendBuffer;
    CopyDataStruct.cbData=pIPCHead->wDataSize;
    CopyDataStruct.dwData=IPC_IDENTIFIER;
    SendMessage(m_hWndGameServer,WM_COPYDATA,(LPARAM)m_hWndChannel,(WPARAM)&CopyDataStruct);

    return true;
}

//发送函数
bool  CClientKernel::SendSocketData(WORD wMainCmdID, WORD wSubCmdID)
{
    //构造数据
    IPC_SocketPackage SocketPackage;
    memset(&SocketPackage,0,sizeof(SocketPackage));
    SocketPackage.Command.wMainCmdID=wMainCmdID;
    SocketPackage.Command.wSubCmdID=wSubCmdID;

    //发送数据
    WORD wSendSize=sizeof(SocketPackage.Command);

    return SendData(IPC_MAIN_SOCKET, IPC_SUB_SOCKET_SEND, &SocketPackage, wSendSize);
}

//发送函数
bool  CClientKernel::SendSocketData(WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize)
{
    //构造数据
    IPC_SocketPackage SocketPackage;
    memset(&SocketPackage,0,sizeof(SocketPackage));
    SocketPackage.Command.wMainCmdID=wMainCmdID;
    SocketPackage.Command.wSubCmdID=wSubCmdID;
    if (wDataSize>0)
    {
        CopyMemory(SocketPackage.cbBuffer,pData,wDataSize);
    }

    //发送数据
    WORD wSendSize=sizeof(SocketPackage.Command)+wDataSize;

    return SendData(IPC_MAIN_SOCKET, IPC_SUB_SOCKET_SEND, &SocketPackage, wSendSize);
}

//发送准备
bool  CClientKernel::SendUserReady(void * pBuffer, WORD wDataSize)
{
    return SendSocketData(MDM_GF_FRAME,SUB_GF_USER_READY,pBuffer,wDataSize);
}

////发送聊天
bool  CClientKernel::SendChatMessage(DWORD dwTargetUserID, LPCTSTR pszChatMessage, COLORREF crFontColor)
{
    //效验状态
    if (m_pMeUserItem==NULL) return false;

    //构造数据
    CMD_GF_UserChat UserChat;
    UserChat.crFontColor=crFontColor;
    UserChat.dwTargetUserID=dwTargetUserID;
    UserChat.dwSendUserID=m_pMeUserItem->dwUserID;
    lstrcpyn(UserChat.szChatMessage,pszChatMessage,MAX_CHAT_LEN);
    UserChat.wChatLength=lstrlen(UserChat.szChatMessage);

    //发送命令
    WORD wSendSize=sizeof(UserChat)-sizeof(UserChat.szChatMessage)+UserChat.wChatLength;
    SendSocketData(MDM_GF_FRAME,SUB_GF_USER_CHAT,&UserChat,wSendSize);

    return true;
}

//信道数据处理
bool  CClientKernel::OnChannelMessage(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend)
{
    //特殊处理
    if (m_pIClientKernelSink==NULL) return true;

    //默认处理
    switch (pHead->wMainCmdID)
    {
    case IPC_MAIN_SOCKET:	//网络数据
        {
            bool bSuccess=OnIPCSocket(pHead,pIPCBuffer,wDataSize,hWndSend);
            if (bSuccess==false)
            {
                if (m_pIClientKernelSink)
                    m_pIClientKernelSink->CloseGameFrame();
            }
            return true;
        }
    case IPC_MAIN_CONFIG:	//配置信息
        {
            return OnIPCConfig(pHead,pIPCBuffer,wDataSize,hWndSend);
        }
    case IPC_MAIN_USER:		//用户信息
        {
            return OnIPCUser(pHead,pIPCBuffer,wDataSize,hWndSend);
        }
    case IPC_MAIN_CONCTROL:	//控制消息
        {
            return OnIPCControl(pHead,pIPCBuffer,wDataSize,hWndSend);
        }
    }

    return false;
}

//网络消息
bool CClientKernel::OnIPCSocket(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend)
{

    switch (pHead->wSubCmdID)
    {
    case IPC_SUB_SOCKET_RECV:	//数据接收
        {
            //效验数据
            if (wDataSize<sizeof(CMD_Command)) return false;

            //提取数据
            WORD wPacketSize=wDataSize-sizeof(CMD_Command);
            IPC_SocketPackage * pSocketPackage=(IPC_SocketPackage *)pIPCBuffer;

            //特殊处理
            bool bHandle=false;
            if (m_pIClientKernelSink==NULL) return true;

            if (wPacketSize==0)	
                bHandle=m_pIClientKernelSink->OnEventSocket(pSocketPackage->Command,NULL,0);
            else
                bHandle=m_pIClientKernelSink->OnEventSocket(pSocketPackage->Command,pSocketPackage->cbBuffer,wPacketSize);

            //框架消息
            if ((bHandle==false)&&(pSocketPackage->Command.wMainCmdID==MDM_GF_FRAME))
            {
                switch (pSocketPackage->Command.wSubCmdID)
                {
                case SUB_GF_OPTION:			//游戏配置
                    {
                        //效验参数
                        if (wPacketSize!=sizeof(CMD_GF_Option)) return false;

                        //消息处理
                        CMD_GF_Option * pOption=(CMD_GF_Option *)pSocketPackage->cbBuffer;
                        m_bGameStatus=pOption->bGameStatus;

                        return true;
                    }
                case SUB_GF_SCENE:			//游戏场景
                    {
                        return m_pIClientKernelSink->OnEventGameScene(m_bGameStatus,false,pSocketPackage->cbBuffer,wPacketSize);
                    }
                case SUB_GF_LOOKON_CONTROL:	//旁观控制
                    {
                        return true;
                    }

                case SUB_GF_USER_CHAT:		//聊天信息
                    {
                        return true;
                    }
                case SUB_GF_MESSAGE:		//系统消息
                    {
                        //效验参数
                        CMD_GF_Message * pMessage=(CMD_GF_Message *)pSocketPackage->cbBuffer;
                        if (wDataSize<=(sizeof(CMD_GF_Message)-sizeof(pMessage->szContent))) return false;

                        //中断判断
                        if (pMessage->wMessageType&SMT_CLOSE_GAME) 
                        {
                        }

                        //关闭房间
                        if (pMessage->wMessageType&SMT_CLOSE_GAME)
                        {
                            m_pIClientKernelSink->CloseGameFrame();
                        }

                        return true;
                    }
                }
            }

            return true;
        }     
    }

    return true;
}

//配置信息
bool CClientKernel::OnIPCConfig(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend)
{

    switch (pHead->wSubCmdID)
    {
    case IPC_SUB_SERVER_INFO:	//房间信息
        {
            //效验参数
            if (wDataSize<sizeof(IPC_GF_ServerInfo)) return false;

            //处理数据
            IPC_GF_ServerInfo * pServerInfo=(IPC_GF_ServerInfo *)pIPCBuffer;
            m_wTableID=pServerInfo->wTableID;
            m_wChairID=pServerInfo->wChairID;
            m_dwUserID=pServerInfo->dwUserID;
            m_ServerAttribute.wKindID=pServerInfo->wKindID;
            m_ServerAttribute.wServerID=pServerInfo->wServerID;
            m_ServerAttribute.wGameGenre=pServerInfo->wGameGenre;
            m_ServerAttribute.wChairCount=pServerInfo->wChairCount;
            m_ServerAttribute.dwVideoAddr=pServerInfo->dwVideoAddr;
            m_ServerAttribute.cbHideUserInfo=pServerInfo->cbHideUserInfo;
            lstrcpyn(m_ServerAttribute.szKindName,pServerInfo->szKindName,KIND_LEN);
            lstrcpyn(m_ServerAttribute.szServerName,pServerInfo->szServerName,SERVER_LEN);

            //更新标题
            UpdateGameTitle();

            return true;
        }
    }
    return false;
}

//用户通知
bool CClientKernel::OnIPCUser(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend)
{
    switch (pHead->wSubCmdID)
    {
    case IPC_SUB_USER_COME:		//用户消息
        {
            //效验参数
            if (wDataSize<sizeof(tagUserInfoHead)) return false;


            //读取基本信息
            tagUserData *pUserData = new tagUserData;

            tagUserInfoHead * pUserInfoHead=(tagUserInfoHead *)pIPCBuffer;

            pUserData->wFaceID=pUserInfoHead->wFaceID;
            //pUserData->dwCustomFaceVer=pUserInfoHead->dwCustomFaceVer;
            pUserData->wTableID=pUserInfoHead->wTableID;
            pUserData->wChairID=pUserInfoHead->wChairID;
            pUserData->cbGender=pUserInfoHead->cbGender;
            pUserData->cbUserStatus=pUserInfoHead->cbUserStatus;
            pUserData->cbMemberOrder=pUserInfoHead->cbMemberOrder;
            pUserData->cbMasterOrder=pUserInfoHead->cbMasterOrder;
            pUserData->dwUserID=pUserInfoHead->dwUserID;
            pUserData->dwGameID=pUserInfoHead->dwGameID;
            pUserData->dwGroupID=pUserInfoHead->dwGroupID;
            pUserData->dwUserRight=pUserInfoHead->dwUserRight;
            //pUserData->dwLoveliness=pUserInfoHead->dwLoveliness;
            pUserData->dwMasterRight=pUserInfoHead->dwMasterRight;
            pUserData->lScore=pUserInfoHead->UserScoreInfo.lScore;
            //pUserData->lGameGold=pUserInfoHead->UserScoreInfo.lGameGold;
            //pUserData->lInsureScore=pUserInfoHead->UserScoreInfo.lInsureScore;
            pUserData->lWinCount=pUserInfoHead->UserScoreInfo.lWinCount;
            pUserData->lLostCount=pUserInfoHead->UserScoreInfo.lLostCount;
            pUserData->lDrawCount=pUserInfoHead->UserScoreInfo.lDrawCount;
            pUserData->lFleeCount=pUserInfoHead->UserScoreInfo.lFleeCount;
            pUserData->lExperience=pUserInfoHead->UserScoreInfo.lExperience;

            //for ( WORD wPropID = 0; wPropID < PROPERTY_COUNT; ++wPropID )
            //{
                //pUserData->dwPropResidualTime[wPropID] = pUserInfoHead->dwPropResidualTime[wPropID];
            //}

            //读取扩展信息
            void * pDataBuffer=NULL;
            tagDataDescribe DataDescribe;
            CRecvPacketHelper RecvPacket(pUserInfoHead+1,wDataSize-sizeof(tagUserInfoHead));
            while (true)
            {
                pDataBuffer=RecvPacket.GetData(DataDescribe);
                if (DataDescribe.wDataDescribe==DTP_NULL) break;
                switch (DataDescribe.wDataDescribe)
                {
                case DTP_USER_ACCOUNTS:		//用户帐户
                    {
                        if (DataDescribe.wDataSize<=sizeof(pUserData->szName))
                        {
                            CopyMemory(&pUserData->szName,pDataBuffer,DataDescribe.wDataSize);
                            //pUserData->szName[sizeof(pUserData->czNickName)-1]=0;
                        }
                        break;
                    }
                case DTP_UNDER_WRITE:		//个性签名
                    {
                        if (DataDescribe.wDataSize<=sizeof(pUserData->szUnderWrite))
                        {
                            CopyMemory(&pUserData->szUnderWrite,pDataBuffer,DataDescribe.wDataSize);
                            //pUserData->szUnderWrite[CountArray(UserData.szUnderWrite)-1]=0;
                        }
                        break;
                    }
                case DTP_USER_GROUP_NAME:	//用户社团
                    {
                        if (DataDescribe.wDataSize<=sizeof(pUserData->szGroupName))
                        {
                            CopyMemory(&pUserData->szGroupName,pDataBuffer,DataDescribe.wDataSize);
                            //pUserData->szGroupName[CountArray(UserData.szGroupName)-1]=0;
                        }
                        break;
                    }
                case DTP_USER_COMPANION:	//用户关系
                    {
                        if (DataDescribe.wDataSize<=sizeof(pUserData->cbCompanion))
                        {
                            CopyMemory(&pUserData->cbCompanion,pDataBuffer,DataDescribe.wDataSize);
                        }
                        break;
                    }
                //case DTP_USER_NICKNAME:		//用户昵称
                //    {
                //        if (DataDescribe.wDataSize<=sizeof(pUserData->czNickName))
                //        {
                //            CopyMemory(&pUserData->szNickName,pDataBuffer,DataDescribe.wDataSize);
                //        }
                //        break;
                //    }
                }
            }

            //增加用户
            ActiveUserItem(pUserData);

            return true;
        }
    case IPC_SUB_USER_STATUS:	//用户状态
        {
            //效验参数


            if (wDataSize<sizeof(IPC_UserStatus)) return false;

            //消息处理
            IPC_UserStatus * pUserStatus=(IPC_UserStatus *)pIPCBuffer;
            if (pUserStatus->cbUserStatus<US_SIT) 
            {
                if (pUserStatus->dwUserID==m_dwUserID)
                {
                    ResetClientKernel();
                    return true;
                }
                else
                           
                DeleteUserItem(pUserStatus->dwUserID);
            }
            else 
                UpdateUserItem(pUserStatus->dwUserID,pUserStatus->cbUserStatus,pUserStatus->wNetDelay);

            return true;
        }
    case IPC_SUB_USER_SCORE:	//用户积分
        {
            //效验参数

            if (wDataSize<sizeof(IPC_UserScore)) return false;

            //消息处理
            IPC_UserScore * pUserScore=(IPC_UserScore *)pIPCBuffer;

            //tagUserData *pUserData = SearchUserItem(pUserScore->dwUserID);
            //pUserData->dwLoveliness = pUserScore->UserScore->dwLoveliness;

            UpdateUserItem(pUserScore->dwUserID,&pUserScore->UserScore);

            return true;
        }
    //case IPC_SUB_MEMBERORDER:	//会员等级
    //    {
    //        //效验参数
    //        if (wDataSize<sizeof(IPC_MemberOrder)) return false;

    //        //消息处理
    //        IPC_MemberOrder * pMemberOrder=(IPC_MemberOrder *)pIPCBuffer;
    //        tagUserData *pUserData = SearchUserItem(pMemberOrder->dwUserID);
    //        pUserData->cbMemberOrder = pMemberOrder->cbMember;

    //        //更新界面
    //        m_pIClientKernelSink->OnEventUserMemberOrder(pUserData,pUserData->wChairID,false);

    //        return true;
    //    }
    case IPC_SUB_GAME_START:	//游戏开始
        {
            //设置用户
            for (WORD i=0;i<m_ServerAttribute.wChairCount;i++)
            {
                if (m_pUserItem[i]!=NULL) 
                {
                    m_pUserItem[i]->cbUserStatus=US_PLAY;
                    m_pIClientKernelSink->OnEventUserStatus(m_pUserItem[i],m_pUserItem[i]->wChairID,false);
                }
            }

            return true;
        }
    case IPC_SUB_GAME_FINISH:	//游戏结束
        {
            //设置用户
            for (WORD i=0;i<m_ServerAttribute.wChairCount;i++)
            {
                if (m_pUserItem[i]!=NULL)
                {
                    m_pUserItem[i]->cbUserStatus=US_SIT;
                    m_pIClientKernelSink->OnEventUserStatus(m_pUserItem[i],m_pUserItem[i]->wChairID,false);
                }
            }

            return true;
        }
    //case IPC_SUB_UPDATE_FACE:	//更新头像
    //    {
    //        return true;
    //    }
    }

    return false;
}

//控制通知
bool CClientKernel::OnIPCControl(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend)
{
    switch (pHead->wSubCmdID)
    {
    case IPC_SUB_START_FINISH:		//启动完成
        {
            //获取场景
            CMD_GF_Info Info;
            ZeroMemory(&Info,sizeof(Info));
            Info.bAllowLookon=FALSE;

            //发送消息
            SendSocketData(MDM_GF_FRAME,SUB_GF_INFO,&Info,sizeof(Info));

            return true;
        }
    case IPC_SUB_CLOSE_FRAME:		//关闭框架
        {
            if (m_pIClientKernelSink!=NULL)
                m_pIClientKernelSink->CloseGameFrame();

            return true;
        }
    }

    return false;
}

tagUserData *  CClientKernel::SearchUserItem(DWORD dwUserID)
{
    for (int i=0; i<MAX_CHAIR; i++)
    {

        if (m_pUserItem[i] && m_pUserItem[i]->dwUserID == dwUserID)
            return m_pUserItem[i];
    }

    return NULL;
}

//增加用户
bool CClientKernel::ActiveUserItem(tagUserData * pUserData)
{
    //通知改变
    m_pUserItem[pUserData->wChairID] = pUserData;

    if (m_dwUserID==pUserData->dwUserID)
    {
        m_pMeUserItem=m_pUserItem[pUserData->wChairID];
        m_wTableID=m_pMeUserItem->wTableID;
        m_wChairID=m_pMeUserItem->wChairID;
    }	

    if (m_pIClientKernelSink!=NULL)
        m_pIClientKernelSink->OnEventUserEnter(m_pUserItem[pUserData->wChairID], pUserData->wChairID, false);

    return true;
}

//删除用户
bool CClientKernel::DeleteUserItem(DWORD dwUserID)
{
    //游戏用户
    tagUserData * pUserData=SearchUserItem(dwUserID);

    if (m_pIClientKernelSink!=NULL) 
        m_pIClientKernelSink->OnEventUserLeave(pUserData,pUserData->wChairID,false);


    int wChairID = pUserData->wChairID ;
    delete m_pUserItem[wChairID];
    m_pUserItem[wChairID] = NULL;

    return true;
}

//更新用户
bool CClientKernel::UpdateUserItem(DWORD dwUserID, const tagUserScore * pUserScore)
{
    //寻找用户
    tagUserData * pUserData=SearchUserItem(dwUserID);
    if (pUserData==NULL) return false;

    //设置数据
    pUserData->lScore=pUserScore->lScore;
    //pUserData->lGameGold=pUserScore->lGameGold;
    //pUserData->lInsureScore=pUserScore->lInsureScore;
    pUserData->lWinCount=pUserScore->lWinCount;
    pUserData->lLostCount=pUserScore->lLostCount;
    pUserData->lDrawCount=pUserScore->lDrawCount;
    pUserData->lFleeCount=pUserScore->lFleeCount;
    pUserData->lExperience=pUserScore->lExperience;

    //通知改变

    if (m_pIClientKernelSink!=NULL)
        m_pIClientKernelSink->OnEventUserScore(pUserData,pUserData->wChairID,false);

    return true;
}

//更新用户
bool CClientKernel::UpdateUserItem(DWORD dwUserID, BYTE cbUserStatus, WORD wNetDelay)
{
    //寻找用户
    tagUserData * pUserData=SearchUserItem(dwUserID);
    if (pUserData==NULL) return false;

    //设置数据
    pUserData->cbUserStatus=cbUserStatus;

    //通知改变
    bool bLookonUser=(cbUserStatus==US_LOOKON);

    if (m_pIClientKernelSink!=NULL) 
        m_pIClientKernelSink->OnEventUserStatus(pUserData,pUserData->wChairID,bLookonUser);

    return false;
}

//更新标题
void CClientKernel::UpdateGameTitle()
{
    //变量定义
    TCHAR szTitle[256]=TEXT("");

    //获取文件
    TCHAR szProcessName[MAX_PATH]=TEXT("猎鱼高手");

    //构造标题
    if ((m_ServerAttribute.szServerName[0]!=0)&&(m_wTableID!=INVALID_TABLE)&&(m_ServerAttribute.cbHideUserInfo==FALSE))
    {
        sprintf(szTitle,TEXT("%s ［ %s - 第 %d 桌 ］"),szProcessName,m_ServerAttribute.szServerName,m_wTableID+1);
    }
    else
    {
        sprintf(szTitle,TEXT("%s"),szProcessName);
    }


    SetWindowText(Root::instance()->render_window()->window_handle(), szTitle);

    return;
}

//内核重置
void CClientKernel::ResetClientKernel()
{
    //设置变量
    m_pMeUserItem=NULL;
    m_bGameStatus=US_FREE;
    m_wTableID=INVALID_TABLE;
    m_wChairID=INVALID_CHAIR;

    //清理界面
    if (m_pIClientKernelSink!=NULL)
        m_pIClientKernelSink->ResetGameFrame();

    return;
}

LRESULT CClientKernel::ChannelWndProc(HWND hWnd, UINT uMsg,WPARAM wParam, LPARAM lParam)
{
    switch( uMsg )
    {
    case WM_COPYDATA:
        if (CClientKernel::m_ClientKernel)
            CClientKernel::m_ClientKernel->OnCopyData(HWND(wParam),(COPYDATASTRUCT *) lParam);
        return 0;
    case WM_IPC_MESSAGE:
        if (CClientKernel::m_ClientKernel)
            CClientKernel::m_ClientKernel->OnIPCMessage(wParam, lParam);
        return 0;
    }

    return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

bool CClientKernel::OnCopyData(HWND hWnd, COPYDATASTRUCT * pCopyDataStruct)
{
    //效验数据
    if (pCopyDataStruct->dwData!=IPC_IDENTIFIER) return false;
    if (pCopyDataStruct->cbData<sizeof(IPC_Head)) return false;

    //提取数据
    IPC_Buffer *pIPCBuffer = new IPC_Buffer;
    CopyMemory(pIPCBuffer,pCopyDataStruct->lpData,pCopyDataStruct->cbData);
    if (pIPCBuffer->Head.wVersion!=IPC_VER) return false;
    if (pIPCBuffer->Head.wDataSize!=pCopyDataStruct->cbData) return false;

    pIPCBuffer->Head.wDataSize-=sizeof(IPC_Head);

    m_IPCBuffers.push_front(pIPCBuffer);

    PostMessage(m_hWndChannel, WM_IPC_MESSAGE,(WPARAM)pIPCBuffer->Head.wDataSize,(LPARAM)pIPCBuffer->Head.wDataSize);

    return true;
}

bool CClientKernel::OnIPCMessage(WPARAM wParam, LPARAM lParam)
{
    IPC_Buffer *pIPCBuffer = m_IPCBuffers.back();

    if (pIPCBuffer->Head.wMainCmdID==IPC_MIAN_IPC_KERNEL)	
    {
        //内核消息
        switch (pIPCBuffer->Head.wSubCmdID)
        {
        case IPC_SUB_IPC_CLIENT_CONNECT:	//连接消息
            {
                SendData(IPC_MIAN_IPC_KERNEL, IPC_SUB_IPC_SERVER_ACCEPT);

                break;
            }
        case IPC_SUB_IPC_SERVER_ACCEPT:		//应答消息
            {

                break;
            }
        case IPC_SUB_IPC_CLIENT_CLOSE:		//关闭消息
            {
                //关闭连接
                SendData(IPC_MIAN_IPC_KERNEL,IPC_SUB_IPC_CLIENT_CLOSE);
                if (m_hWndChannel!=NULL)
                {
                    DestroyWindow(m_hWndChannel);
                    m_hWndChannel = NULL;
                }

                break;
            }

        }
    }
    else	//外围消息
    {
        WORD wDataSize=pIPCBuffer->Head.wDataSize;
        OnChannelMessage(&pIPCBuffer->Head,pIPCBuffer->cbBuffer,wDataSize,NULL);
    }

    delete pIPCBuffer;
    m_IPCBuffers.pop_back();

    return true;
}

//////////////////////////////////////////////////////////////////////////
