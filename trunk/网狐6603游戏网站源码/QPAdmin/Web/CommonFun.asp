<%@LANGUAGE="VBSCRIPT" CODEPAGE="65001"%>
<% 
Response.Buffer=True
Response.Charset="utf-8" 
Session.CodePage=65001

'网站数据库
Dim AdminName,AdminPassword
AdminName=replace(trim(session("AdminName")),"'","")
AdminPassword=replace(trim(session("AdminPassword")),"'","")
if AdminName="" or adminpassword="" then
    response.redirect "/Login.asp"
end if
  
Rem 游戏网页操作类    
Class GameObj
    Private Sub Class_Initialize

    End Sub 

    Private Sub Class_Terminate()

	End Sub 
	
	Rem 检查数字
	Public Function SqlCheckNum(lInStr)
	    If lInStr<>"" And Not IsNull(lInStr) And IsNumeric(lInStr) Then
	        SqlCheckNum = lInStr
	    Else 
	        SqlCheckNum = 0
	    End If
	End Function 
	
	Rem 检查字符串
	Public Function SqlCheckStr(lInStr)
	    If lInStr<>"" And Not IsNull(lInStr) Then
	        SqlCheckStr = Replace(lInStr, "'", "''")
	    Else
	        SqlCheckStr = ""
	    End If
	End Function
	
	Rem 获取页面信息
	Function GetInfo(Typ,FormName)
	    GetInfo=Trim(Request(FormName))
	    If Typ=1 Then
		    IF Not IsNumeric(GetInfo) Then
			    GetInfo=0
		    Else
			    GetInfo=CLng(GetInfo)
		    End IF
	    End IF	
	End Function
	

	
	Rem 跳转到错误页面
	Sub WriteErrMsg(ErrMsg)
        Session("ErrMsg") = ErrMsg
	    Response.Redirect "../../Error.asp"
    End Sub
    
    Rem 操作信息提示
    Sub ShowInfo(info)
        Response.Write "<script>showInfo('"&info&"')</script>"
    End Sub
    
    Rem 操作信息提示
    Sub ShowInfo2(info,url,timeout)
        Dim rValue
        rValue = "<script>showInfo('"&info&"');"
        rValue = rValue&"setTimeout(""Redirect('"&url&"'),"&timeout&""")"
        rValue = rValue&"</script>"
        Response.Write rValue
    End Sub
    
    Rem 弹出信息提示
    Sub MessageBox(info)
        Dim rValue
        rValue = "<script>alert('"&info&"')</script>"
        Response.Write rValue
    End Sub
    
    Rem 弹出信息提示关闭窗口
    Sub MessageBoxClose(info)
        Dim rValue
        rValue = "<script>alert('"&info&"');"
        rValue = rValue&"self.opener.location = self.opener.location;; self.focus();self.opener=null;self.close()"
        rValue = rValue&"</script>"
        Response.Write rValue
    End Sub
    
    Rem 弹出信息提示刷新页面
    Sub MessageBoxReload(info,url)
        Dim rValue
        rValue = "<script>alert('"&info&"');"
        rValue = rValue&"window.location.href='"&url&"'"
        rValue = rValue&"</script>"
        Response.Write rValue
    End Sub
    
    Rem 冻结状态显示
    Function GetNullityInfo(nullity)
        Dim rInfo
        rInfo=""
        If nullity=0 Then
            rInfo = "<span>否</span>"
        Else
            rInfo = "<span style='color:red'>是</span>"
        End If
        GetNullityInfo = rInfo
    End Function
    
    Rem 状态显示
    Function GetStatusInfo(status)
        Dim rInfo
        rInfo=""
        If status=0 Then
            rInfo = "<span style='color:red'>否</span>"
        Else
            rInfo = "<span>是</span>"
        End If
        GetStatusInfo = rInfo
    End Function
    
    Rem 获取会员等级名称
    Function GetMemberOrderName(memberOrder)
        Dim rInfo
        Select case memberOrder
            case 1
                rInfo = "<span style='color:#15599f;font-weight:bold;'>蓝钻</span>"
            case 2
                rInfo = "<span style='color:#d16213;font-weight:bold;'>黄钻</span>"
            case 3
                rInfo = "<span style='color:#777777; font-weight:bold;'>白钻</span>"
            case 4
                rInfo = "<span style='color:#b70000; font-weight:bold;'>红钻</span>"
            case else
                rInfo = "<span>普通会员</span>"
        End Select 
        GetMemberOrderName = rInfo
    End Function 
    
    Rem 获取会员卡使用范围
    Function GetCardUseRange(useRange)
        Dim rInfo
        Select case useRange
            case 0
                rInfo="全部用户"
            case 1
                rInfo="新注册用户"
            case 2
                rInfo="第一次充值用户"
        End Select
        GetCardUseRange = rInfo
    End Function
    
    Rem 获取道具发行范围
    Function GetIssueAreae(issueAreae)
        Dim rInfo
        Select case issueAreae
            case 0
                 rInfo="无"
            case 1
                rInfo="商城道具"
            case 2
                rInfo="游戏道具"
            case 4
                rInfo="房间道具"
            case 3
                rInfo="商城道具,游戏道具"
            case 5
                rInfo="商城道具,房间道具"
            case 6
                rInfo="游戏道具,房间道具"
            case 7
                rInfo="商城道具,游戏道具,房间道具"
        End Select
        GetIssueAreae = rInfo
    End Function
    
    Rem 获取道具发行范围
    Function GetServiceArea(serviceArea)
        Dim rInfo
        Select case serviceArea
            case 0
                 rInfo="无"
            case 1
                rInfo="自己范围"
            case 2
                rInfo="玩家范围"
            case 4
                rInfo="旁观范围"
            case 3
                rInfo="自己范围,玩家范围"
            case 5
                rInfo="自己范围,旁观范围"
            case 6
                rInfo="玩家范围,旁观范围"
            case 7
                rInfo="自己范围,玩家范围,旁观范围"
        End Select
        GetServiceArea = rInfo
    End Function
    
    Rem 机器列表
    Function GetDataBaseList()
        Dim rs,sql,n,j
        Call ConnectGame(QPPlatformDB)
        Set rs=Server.CreateObject("Adodb.RecordSet")
        sql="SELECT DBAddr,Information FROM DataBaseInfo(nolock)"
        rs.Open sql,GameConn,1,3
        n= rs.RecordCount-1
        redim ArrayList(n,1)
        j=0
        do while not rs.eof 
            ArrayList(j,0)=rs("DBAddr")
            ArrayList(j,1)=rs("Information")
            j=j+1
            rs.movenext 
        loop
        rs.close
        set rs=nothing
        CloseGame()
        GetDataBaseList = ArrayList
    End Function
    
    Rem 获取模块名称
    Function GetGameNameByGameID(gameID)
        Dim rs,sql,gameName
        gameName=""
        Call ConnectGame(QPPlatformDB)
        Set rs=Server.CreateObject("Adodb.RecordSet")
        sql="SELECT GameName FROM GameGameItem(nolock)  WHERE GameID="&gameID
        rs.Open sql,GameConn,1,3
        If Not rs.eof Then    
            gameName=rs("GameName")
        End If 
        rs.close
        set rs=nothing
        CloseGame()
        GetGameNameByGameID = gameName
    End Function 
    
    Rem 最大模块标识
    Function GetMaxGameID()
        Dim rs,sql,rValue
        Call ConnectGame(QPPlatformDB)
        Set rs=Server.CreateObject("Adodb.RecordSet")
        sql="SELECT MAX(GameID) AS MaxGameID FROM GameGameItem(nolock)"
        rs.Open sql,GameConn,1,3        
        rValue=rs("MaxGameID")
        If IsNull(rValue) Then
            rValue=0
        End If         
        rs.close
        set rs=nothing        
        CloseGame()
        GetMaxGameID = rValue
    End Function
    
    Rem 游戏模块列表
    Function GetGameList()
        Dim rs,sql,n,j
        Call ConnectGame(QPPlatformDB)
        Set rs=Server.CreateObject("Adodb.RecordSet")
        sql="SELECT GameID,GameName FROM GameGameItem(nolock)"
        rs.Open sql,GameConn,1,3
        n= rs.RecordCount-1
        redim ArrayList(n,1)
        j=0
        do while not rs.eof 
            ArrayList(j,0)=rs("GameID")
            ArrayList(j,1)=rs("GameName")
            j=j+1
            rs.movenext 
        loop
        rs.close
        set rs=nothing
        CloseGame()
        GetGameList = ArrayList
    End Function
    
    Rem 游戏类别名称
    Function GetTypeNameByTypeID(typeID)
        Dim rs,sql,typeName
        typeName=""
        Call ConnectGame(QPPlatformDB)
        Set rs=Server.CreateObject("Adodb.RecordSet")
        sql="SELECT TypeName FROM GameTypeItem(nolock)  WHERE TypeID="&typeID
        rs.Open sql,GameConn,1,3
        If Not rs.eof Then    
            typeName=rs("TypeName")
        End If 
        rs.close
        set rs=nothing
        CloseGame()
        GetTypeNameByTypeID = typeName
    End Function
    
    Rem 游戏金币数，输赢数
    Function GetScoreInfoByUserID(UserID,dname)
        Dim rs,sql,Score
        Score=""
        Call ConnectGame(dname)
        Set rs=Server.CreateObject("Adodb.RecordSet")
        If dname="QPTreasureDB" Then
        sql="SELECT (Score+InsureScore) AS Score,Wincount,LostCount FROM GameScoreInfo(nolock)  WHERE UserID="&UserID
        Else
        sql="SELECT 0 as Score, Wincount,LostCount FROM GameScoreInfo(nolock)  WHERE UserID="&UserID
        End if
        rs.Open sql,GameConn,1,3
        redim ArrayList(1,2)
        If Not rs.eof Then    
            ArrayList(1,0)=rs("Score")
            ArrayList(1,1)=rs("Wincount")
            ArrayList(1,2)=rs("LostCount")
        End If 
        rs.close
        set rs=nothing
        CloseGame()
        GetScoreInfoByUserID = ArrayList
    End Function
    
    
    Rem 游戏类别列表
    Function GetTypeList()
        Dim rs,sql,n,j
        Call ConnectGame(QPPlatformDB)
        Set rs=Server.CreateObject("Adodb.RecordSet")
        sql="SELECT TypeID,TypeName FROM GameTypeItem(nolock) ORDER BY SortID"
        rs.Open sql,GameConn,1,3
        n= rs.RecordCount-1
        redim ArrayList(n,1)
        j=0
        do while not rs.eof 
            ArrayList(j,0)=rs("TypeID")
            ArrayList(j,1)=rs("TypeName")
            j=j+1
            rs.movenext 
        loop 
        rs.close
        set rs=nothing
        CloseGame()
        GetTypeList = ArrayList
    End Function
    
    Rem 最大类型标识
    Function GetMaxTypeID()
        Dim rs,sql,rValue
        Call ConnectGame(QPPlatformDB)
        Set rs=Server.CreateObject("Adodb.RecordSet")
        sql="SELECT MAX(TypeID) AS MaxTypeID FROM GameTypeItem(nolock)"
        rs.Open sql,GameConn,1,3
        rValue=rs("MaxTypeID")
        If IsNull(rValue) Then
            rValue=0
        End If     
        rs.close
        set rs=nothing
        CloseGame()
        GetMaxTypeID = rValue
    End Function
    
    Rem 游戏名称
    Function GetKindNameByKindID(kindID)
        Dim rs,sql,kindName
        kindName=""
        Call ConnectGame(QPPlatformDB)
        Set rs=Server.CreateObject("Adodb.RecordSet")
        sql="SELECT KindName FROM GameKindItem(nolock)  WHERE KindID="&kindID
        rs.Open sql,GameConn,1,3
        If Not rs.eof Then    
            kindName=rs("KindName")
        End If 
        rs.close
        set rs=nothing
        CloseGame()
        GetKindNameByKindID = kindName
    End Function
    
    Rem 获取游戏数据库名称
    Function GetKindDBNameByKindID(kindID)
        Dim rs,sql,rValue
        rValue=""
        Call ConnectGame(QPPlatformDB)
        Set rs=Server.CreateObject("Adodb.RecordSet")
        sql = "SELECT a.KindID,a.KindName,b.DataBaseAddr,b.DataBaseName,a.TypeID FROM GameKindItem a,GameGameItem b WHERE a.GameID = b.GameID AND KindID="&kindID
        rs.Open sql,GameConn,1,3
        If Not rs.eof Then    
            rValue=Trim(rs("DataBaseName"))
        End If 
        rs.close
        set rs=nothing
        CloseGame()
        GetKindDBNameByKindID = rValue
    End Function 
    
    Rem 游戏列表
    Function GetKindList()
        Dim rs,sql,n,j
        Call ConnectGame(QPPlatformDB)
        Set rs=Server.CreateObject("Adodb.RecordSet")
        sql="SELECT KindID,KindName FROM GameKindItem(nolock) ORDER BY SortID"
        rs.Open sql,GameConn,1,3
        n= rs.RecordCount-1
        redim ArrayList(n,1)
        j=0
        do while not rs.eof 
            ArrayList(j,0)=rs("KindID")
            ArrayList(j,1)=rs("KindName")
            j=j+1
            rs.movenext 
        loop 
        rs.close
        set rs=nothing
        CloseGame()
        GetKindList = ArrayList
    End Function
    
    Rem 游戏列表
    Function GetRoomList()
        Dim rs,sql,n,j
        Call ConnectGame(QPPlatformDB)
        Set rs=Server.CreateObject("Adodb.RecordSet")
        sql="SELECT DataBaseName,ServerName,ServerID FROM GameRoomInfo(nolock) ORDER BY SortID"
        rs.Open sql,GameConn,1,3
        n= rs.RecordCount-1
        redim ArrayList(n,2)
        j=0
        do while not rs.eof 
            ArrayList(j,0)=rs("DataBaseName")
            ArrayList(j,1)=rs("ServerName")
            ArrayList(j,2)=rs("ServerID")
            j=j+1
            rs.movenext 
        loop 
        rs.close
        set rs=nothing
        CloseGame()
        GetRoomList = ArrayList
    End Function
    
    Rem 游戏列表
    Function GetRoomList1()
        Dim rs,sql,n,j
        Call ConnectGame(QPPlatformDB)
        Set rs=Server.CreateObject("Adodb.RecordSet")
        sql="SELECT ServerID,ServerName FROM GameRoomInfo(nolock) ORDER BY SortID"
        rs.Open sql,GameConn,1,3
        n= rs.RecordCount-1
        redim ArrayList(n,1)
        j=0
        do while not rs.eof 
            ArrayList(j,0)=rs("ServerID")
            ArrayList(j,1)=rs("ServerName")
            j=j+1
            rs.movenext 
        loop 
        rs.close
        set rs=nothing
        CloseGame()
        GetRoomList1 = ArrayList
    End Function
    
    Rem 获取所有积分游戏列表
    Function GetScoreKindList()
        Dim rs,sql,n,j
        Call ConnectGame(QPPlatformDB)
        Set rs=Server.CreateObject("Adodb.RecordSet")
        sql = "SELECT a.KindID,a.KindName,b.DataBaseAddr,b.DataBaseName,a.TypeID FROM GameKindItem a,GameGameItem b WHERE a.GameID = b.GameID ORDER BY SortID"
        rs.Open sql,GameConn,1,3
        n= rs.RecordCount-1
        redim ArrayList(n,4)
        j=0
        do while not rs.eof 
            If rs("DataBaseName")<> "QPTreasureDB" Then
                ArrayList(j,0)=rs("KindID")
                ArrayList(j,1)=rs("KindName")
                ArrayList(j,2)=rs("DataBaseAddr")
                ArrayList(j,3)=rs("DataBaseName")
                ArrayList(j,4)=rs("TypeID")
                j=j+1
            End If
            rs.movenext 
        loop 
        rs.close
        set rs=nothing
        CloseGame()
        GetScoreKindList = ArrayList
    End Function
    
    Rem 获取所有游戏列表
    Function GetAllKindList()
        Dim rs,sql,n,j
        Call ConnectGame(QPPlatformDB)
        Set rs=Server.CreateObject("Adodb.RecordSet")
        sql = "SELECT a.KindID,a.KindName,b.DataBaseAddr,b.DataBaseName,a.TypeID FROM GameKindItem a,GameGameItem b WHERE a.GameID = b.GameID ORDER BY SortID"
        rs.Open sql,GameConn,1,3
        n= rs.RecordCount-1
        redim ArrayList(n,4)
        j=0
        do while not rs.eof 
            ArrayList(j,0)=rs("KindID")
            ArrayList(j,1)=rs("KindName")
            ArrayList(j,2)=rs("DataBaseAddr")
            ArrayList(j,3)=rs("DataBaseName")
            ArrayList(j,4)=rs("TypeID")
            j=j+1
            rs.movenext 
        loop 
        rs.close
        set rs=nothing
        CloseGame()
        GetAllKindList = ArrayList
    End Function
    
    Rem 最大游戏标识
    Function GetMaxKindID()
        Dim rs,sql,rValue
        Call ConnectGame(QPPlatformDB)
        Set rs=Server.CreateObject("Adodb.RecordSet")
        sql="SELECT MAX(KindID) AS MaxKindID FROM GameKindItem(nolock)"
        rs.Open sql,GameConn,1,3
        rValue=rs("MaxKindID")
        If IsNull(rValue) Then
            rValue=0
        End If     
        rs.close
        set rs=nothing
        CloseGame()
        GetMaxKindID = rValue
    End Function
    
    Rem 节点名称
    Function GetNodeNameByNodeID(nodeID)
        Dim rs,sql,nodeName
        nodeName=""
        Call ConnectGame(QPPlatformDB)
        Set rs=Server.CreateObject("Adodb.RecordSet")
        sql="SELECT NodeName FROM GameNodeItem(nolock)  WHERE NodeID="&nodeID
        rs.Open sql,GameConn,1,3
        If Not rs.eof Then    
            nodeName=rs("NodeName")
        End If 
        rs.close
        set rs=nothing
        CloseGame()
        GetNodeNameByNodeID = nodeName
    End Function
    
    Rem 节点列表
    Function GetNodeList()
        Dim rs,sql,n,j
        Call ConnectGame(QPPlatformDB)
        Set rs=Server.CreateObject("Adodb.RecordSet")
        sql="SELECT NodeID,NodeName FROM GameNodeItem(nolock) ORDER BY SortID"
        rs.Open sql,GameConn,1,3
        n= rs.RecordCount-1
        redim ArrayList(n,1)
        j=0
        do while not rs.eof 
            ArrayList(j,0)=rs("NodeID")
            ArrayList(j,1)=rs("NodeName")
            j=j+1
            rs.movenext 
        loop 
        rs.close
        set rs=nothing
        CloseGame()
        GetNodeList = ArrayList
    End Function
    
    Rem 最大节点标识
    Function GetMaxNodeID()
        Dim rs,sql,rValue
        Call ConnectGame(QPPlatformDB)
        Set rs=Server.CreateObject("Adodb.RecordSet")
        sql="SELECT MAX(NodeID) AS MaxNodeID FROM GameNodeItem(nolock)"
        rs.Open sql,GameConn,1,3
        rValue=rs("MaxNodeID")
        If IsNull(rValue) Then
            rValue=0
        End If  
        rs.close
        set rs=nothing
        CloseGame()
        GetMaxNodeID = rValue
    End Function
    
    Rem 最大页面标识
    Function GetMaxPageID()
        Dim rs,sql,rValue
        Call ConnectGame(QPPlatformDB)
        Set rs=Server.CreateObject("Adodb.RecordSet")
        sql="SELECT MAX(PageID) AS MaxPageID FROM GamePageItem(nolock)"
        rs.Open sql,GameConn,1,3
        rValue=rs("MaxPageID")
        If IsNull(rValue) Then
            rValue=0
        End If  
        rs.close
        set rs=nothing
        CloseGame()
        GetMaxPageID = rValue
    End Function
    
    Rem 根据房间标识获取房间名称
    Function GetRoomNameByRoomID(serverID)
    
        Dim rs,sql,serverName
        serverName=""
        Call ConnectGame(QPPlatformDB)
        Set rs=Server.CreateObject("Adodb.RecordSet")
        sql="SELECT ServerName FROM GameRoomInfo(nolock)  WHERE ServerID="&serverID
        rs.Open sql,GameConn,1,3
        If Not rs.eof Then    
            serverName=rs("ServerName")
        End If 
        rs.close
        set rs=nothing
        CloseGame()
        GetRoomNameByRoomID = serverName
    End Function
    
    Rem 限制内容是否存在
    Function IsExistConfineContent(contect)
        Dim rs,sql,hasConfine
        hasConfine=False
        Call ConnectGame(QPAccountsDB)
        Set rs=Server.CreateObject("Adodb.RecordSet")
        sql = "SELECT String FROM ConfineContent(nolock) WHERE String='"&Trim(contect)&"'"
        rs.Open sql,GameConn,1,3
        If Not rs.eof Then    
            hasConfine=True
        End If 
        rs.close
        set rs=nothing
        CloseGame()
        IsExistConfineContent = hasConfine
    End Function 
    
    Rem 限制内容是否存在
    Function IsExistConfineNickName(contect)
        Dim rs,sql,hasConfine
        hasConfine=False
        Call ConnectGame(QPAccountsDB)
        Set rs=Server.CreateObject("Adodb.RecordSet")
        sql = "SELECT String FROM ConfineNickName(nolock) WHERE String='"&Trim(contect)&"'"
        rs.Open sql,GameConn,1,3
        If Not rs.eof Then    
            hasConfine=True
        End If 
        rs.close
        set rs=nothing
        CloseGame()
        IsExistConfineNickName = hasConfine
    End Function 
    
    Rem 限制地址是否存在    
    Function IsExistConfineAddress(address)
        Dim rs,sql,hasConfine
        hasConfine=False
        Call ConnectGame(QPAccountsDB)
        Set rs=Server.CreateObject("Adodb.RecordSet")
        sql = "SELECT AddrString FROM ConfineAddress(nolock) WHERE AddrString='"&Trim(address)&"'"
        rs.Open sql,GameConn,1,3
        If Not rs.eof Then    
            hasConfine=True
        End If 
        rs.close
        set rs=nothing
        CloseGame()
        IsExistConfineAddress = hasConfine
    End Function
    
    Rem 限制机器是否存在
    Function IsExistConfineMachine(machine)
        Dim rs,sql,hasConfine
        hasConfine=False
        Call ConnectGame(QPAccountsDB)
        Set rs=Server.CreateObject("Adodb.RecordSet")
        sql = "SELECT MachineSerial FROM ConfineMachine(nolock) WHERE MachineSerial='"&Trim(machine)&"'"
        rs.Open sql,GameConn,1,3
        If Not rs.eof Then    
            hasConfine=True
        End If 
        rs.close
        set rs=nothing
        CloseGame()
        IsExistConfineMachine = hasConfine
    End Function 
    
    Rem 根据用户ID获取用户名称
    Function GetUserNameByUserID(userID)
        Dim rs,sql,userName
        userName=""
        Call ConnectGame(QPAccountsDB)
        Set rs=Server.CreateObject("Adodb.RecordSet")
        sql="SELECT Accounts FROM AccountsInfo(nolock)  WHERE UserID="&userID
        rs.Open sql,GameConn,1,3
        If Not rs.eof Then    
            userName=rs("Accounts")
        End If 
        rs.close
        set rs=nothing
        CloseGame()
        GetUserNameByUserID = userName
    End Function 
    
    Rem 根据用户ID获取游戏ID
    Function GetGameIDByUserID(userID)
        Dim rs,sql,GameID
        userName=""
        Call ConnectGame(QPAccountsDB)
        Set rs=Server.CreateObject("Adodb.RecordSet")
        sql="SELECT GameID FROM AccountsInfo(nolock)  WHERE UserID="&userID
        rs.Open sql,GameConn,1,3
        If Not rs.eof Then    
            GameID=rs("GameID")
        End If 
        rs.close
        set rs=nothing
        CloseGame()
        GetGameIDByUserID = GameID
    End Function 
    
    Rem 会员卡类型列表
    Function GetCardTypeList()
        Dim rs,sql,n,j
        Call ConnectGame(QPTreasureDB)
        Set rs=Server.CreateObject("Adodb.RecordSet")
        sql="SELECT CardTypeID,CardName,CardGold,MemberDays,CardPrice FROM GlobalLivcard(nolock) ORDER BY CardTypeID"
        rs.Open sql,GameConn,1,3
        n= rs.RecordCount-1
        redim ArrayList(n,4)
        j=0
        do while not rs.eof 
            ArrayList(j,0)=rs("CardTypeID")
            ArrayList(j,1)=rs("CardName")
            ArrayList(j,2)=rs("CardGold")
            ArrayList(j,3)=rs("MemberDays")
            ArrayList(j,4)=rs("CardPrice")
            j=j+1
            rs.movenext 
        loop 
        rs.close
        set rs=nothing
        CloseGame()
        GetCardTypeList = ArrayList
    End Function
    
    Rem 获取银行金币
    Function GetInsureByUserID(UserID)
        Dim rs,sql,InsureScore
        cardName=""
        Call ConnectGame(QPTreasureDB)
        Set rs=Server.CreateObject("Adodb.RecordSet")
        sql="SELECT InsureScore FROM GameScoreInfo(nolock)  WHERE UserID="&UserID
        rs.Open sql,GameConn,1,3
        If Not rs.eof Then    
            InsureScore=rs("InsureScore")
        End If 
        rs.close
        set rs=nothing
        CloseGame()
        GetInsureByUserID = InsureScore
    End Function
    
    Rem 根据UserID获取贡献金币
    Function GetRecordSpreadByUserID(UserID)
        Dim rs,sql,score
        cardName=""
        Call ConnectGame(QPTreasureDB)
        Set rs=Server.CreateObject("Adodb.RecordSet")
        sql="select sum(score) as score from  RecordSpreadInfo where childrenid="&UserID
        rs.Open sql,GameConn,1,3
        If Not rs.eof Then  
            If rs("score")<>"" Then
                score=rs("score")
            else
                score=0
            End If
        End If 
        rs.close
        set rs=nothing
        CloseGame()
        GetRecordSpreadByUserID = score
    End Function
    
    Rem 获取会员卡名称
    Function GetCardNameByCardID(cardTypeID)
        Dim rs,sql,cardName
        cardName=""
        Call ConnectGame(QPTreasureDB)
        Set rs=Server.CreateObject("Adodb.RecordSet")
        sql="SELECT CardName FROM GlobalLivcard(nolock)  WHERE CardTypeID="&cardTypeID
        rs.Open sql,GameConn,1,3
        If Not rs.eof Then    
            cardName=rs("CardName")
        End If 
        rs.close
        set rs=nothing
        CloseGame()
        GetCardNameByCardID = cardName
    End Function 
    
    Rem 获取销售商名称
    Function GetSalesPersonBySerialID(SerialID)
        Dim rs,sql,SalesPerson
        SalesPerson=""
        Call ConnectGame(QPTreasureDB)
        Set rs=Server.CreateObject("Adodb.RecordSet")
        sql="SELECT SalesPerson FROM LivcardAssociator(nolock)  WHERE SerialID='"&SerialID&"'"
        rs.Open sql,GameConn,1,3
        If Not rs.eof Then    
            SalesPerson=rs("SalesPerson")
        End If 
        rs.close
        set rs=nothing
        CloseGame()
        GetSalesPersonBySerialID = SalesPerson
    End Function 
    
    Rem 获取销售商名称
    Function GetSalesPersonByBuildID(BuildID)
        Dim rs,sql,SalesPerson
        SalesPerson=""
        Call ConnectGame(QPTreasureDB)
        Set rs=Server.CreateObject("Adodb.RecordSet")
        sql="SELECT SalesPerson FROM LivcardAssociator(nolock)  WHERE BuildID='"&BuildID&"'"
        rs.Open sql,GameConn,1,3
        If Not rs.eof Then    
            SalesPerson=rs("SalesPerson")
        End If 
        rs.close
        set rs=nothing
        CloseGame()
        GetSalesPersonByBuildID = SalesPerson
    End Function 

    Rem 获取充值服务列表
    Function GetShareInfoList()
        Dim rs,sql,n,j
        Call ConnectGame(QPTreasureDB)
        Set rs=Server.CreateObject("Adodb.RecordSet")
        sql="SELECT ShareID,ShareName FROM GlobalShareInfo(nolock) ORDER BY ShareID"
        rs.Open sql,GameConn,1,3
        n= rs.RecordCount-1
        redim ArrayList(n,1)
        j=0
        do while not rs.eof 
            ArrayList(j,0)=rs("ShareID")
            ArrayList(j,1)=rs("ShareName")            
            j=j+1
            rs.movenext 
        loop 
        rs.close
        set rs=nothing
        CloseGame()
        GetShareInfoList = ArrayList
    End Function
    
    Rem 获取服务名称
    Function GetShareNameByShareID(shareID)
        Dim rs,sql,rValue
        rValue=""
        Call ConnectGame(QPTreasureDB)
        Set rs=Server.CreateObject("Adodb.RecordSet")
        sql="SELECT ShareName FROM GlobalShareInfo(nolock)  WHERE ShareID="&shareID
        rs.Open sql,GameConn,1,3
        If Not rs.eof Then    
            rValue=rs("ShareName")
        End If 
        rs.close
        set rs=nothing
        CloseGame()
        GetShareNameByShareID = rValue
    End Function
    
    Rem 获取用户积分
    Function GetUserScore(userID,dbName)
        Dim rs,sql,rValue
        rValue=0
        Call ConnectGame(dbName)
        Set rs=Server.CreateObject("Adodb.RecordSet")
        sql="SELECT Score FROM GameScoreInfo(nolock)  WHERE UserID="&userID
        rs.Open sql,GameConn,1,3
        If Not rs.eof Then    
            rValue=rs("Score")
        End If 
        rs.close
        set rs=nothing
        CloseGame()
        GetUserScore = rValue
    End Function 
    
    Rem 获取用户在线信息
    Function GetUserLocker(userID)
        Dim rs,sql
        Call ConnectGame(QPTreasureDB)
        Set rs=Server.CreateObject("Adodb.RecordSet")
        sql="SELECT KindID,ServerID FROM GameScoreLocker(nolock) Where UserID="&userID
        rs.Open sql,GameConn,1,3
        Dim ArrayList(1,1)
        If Not rs.eof Then    
            ArrayList(0,0)=GetKindNameByKindID(rs("KindID"))
            ArrayList(0,1)=GetRoomNameByRoomID(rs("ServerID"))
        Else
            ArrayList(0,0)=""
            ArrayList(0,1)=""
        End If
        rs.close
        set rs=nothing
        CloseGame()
        GetUserLocker = ArrayList
    End Function
    
    Rem 获取用户基本信息
    Function GetUserInfo(userID)
        Dim rs,sql
        Call ConnectGame(QPAccountsDB)
        Set rs=Server.CreateObject("Adodb.RecordSet")
        sql="SELECT GameID,Experience,Present,LoveLiness FROM AccountsInfo(nolock) Where UserID="&userID
        rs.Open sql,GameConn,1,3
        Dim ArrayList(1,4)
        If Not rs.eof Then    
            ArrayList(0,0)=rs("GameID")
            ArrayList(0,1)=rs("Experience")       
            ArrayList(0,2)=rs("Present")
            ArrayList(0,3)=rs("LoveLiness")  
        End If
        rs.close
        set rs=nothing
        CloseGame()
        GetUserInfo = ArrayList
    End Function
    
    Rem 获取基本权限信息
    Function GetRoleInfo()
        Dim rs,sql,n
        Call ConnectGame(QPPlatformManagerDB)
        Set rs=Server.CreateObject("Adodb.RecordSet")
        sql="SELECT RoleID,RoleName FROM Base_Roles(nolock) "
        rs.Open sql,GameConn,1,3
        n= rs.RecordCount-1
        redim ArrayList(n,1)
        n=0
        do while not rs.eof 
            ArrayList(n,0)=rs("RoleID")
            ArrayList(n,1)=rs("RoleName")            
            n=n+1
            rs.movenext 
        loop 
        rs.close
        set rs=nothing
        CloseGame()
        GetRoleInfo = ArrayList
    End Function
    
    Rem 获取用户权限类型
    Function GetRoleName()
        Dim rs,sql,n
        Call ConnectGame(QPPlatformManagerDB)
        Set rs=Server.CreateObject("Adodb.RecordSet")
        sql="SELECT br.RoleName as RoleName FROM Base_Roles br,Base_Users bu where br.roleid=bu.roleid and bu.UserName='"&trim(session("AdminName"))&"'"
        rs.Open sql,GameConn,1,3
        
        do while not rs.eof 
            roleValue=rs("RoleName")          
            rs.movenext 
        loop 
        rs.close
        set rs=nothing
        CloseGame()
        GetRoleName = roleValue
    End Function
    
    Rem 根据组别ID获取用户权限类型
    Function GetRoleNameByRoleID(RoleID)
        Dim rs,sql,n
        Call ConnectGame(QPPlatformManagerDB)
        Set rs=Server.CreateObject("Adodb.RecordSet")
        sql="SELECT RoleName FROM Base_Roles where RoleID="&RoleID
        rs.Open sql,GameConn,1,3
        
        do while not rs.eof 
            roleValue=rs("RoleName")          
            rs.movenext 
        loop 
        rs.close
        set rs=nothing
        CloseGame()
        GetRoleNameByRoleID = roleValue
    End Function
    
    Rem 获取此用户该模块的权限值
    Function GetRoleValue(ModuleID,UserName)
        Dim rs,sql,n,roleValue
        Call ConnectGame(QPPlatformManagerDB)
        Set rs=Server.CreateObject("Adodb.RecordSet")
        sql="select br.OperationPermission from Base_RolePermission br,Base_Users bu where br.roleid=bu.roleid and bu.username='"&UserName&"' and br.moduleid ="&ModuleID
        rs.Open sql,GameConn,1,3
        do while not rs.eof 
            roleValue=rs("OperationPermission")          
            rs.movenext 
        loop 
        If IsNull(roleValue) Then
            rValue=0
        End If
        rs.close
        set rs=nothing
        CloseGame()
        GetRoleValue = roleValue
    End Function
    
    Rem 根据管理员ID获取管理员帐号
    Function GetAdmin(UserID)
        Dim rs,sql,n,roleValue
        Call ConnectGame(QPPlatformManagerDB)
        Set rs=Server.CreateObject("Adodb.RecordSet")
        sql="select username from Base_Users where  UserID ="&UserID
        rs.Open sql,GameConn,1,3
        do while not rs.eof 
            roleValue=rs("username")          
            rs.movenext 
        loop 
        If IsNull(roleValue) Then
            rValue=0
        End If
        rs.close
        set rs=nothing
        CloseGame()
        GetAdmin = roleValue
    End Function
    
    Rem 获取赛事列表
    Function GetMatchList()
        Dim rs,sql,n,j
        Call ConnectGame("QPNativeWebDB")
        Set rs=Server.CreateObject("Adodb.RecordSet")
        sql="SELECT MatchID,MatchTitle FROM GameMatchInfo(nolock) "
        rs.Open sql,GameConn,1,3
        n= rs.RecordCount-1
        redim ArrayList(n,1)
        j=0
        do while not rs.eof 
            ArrayList(j,0)=rs("MatchID")
            ArrayList(j,1)=rs("MatchTitle")            
            j=j+1
            rs.movenext 
        loop 
        rs.close
        set rs=nothing
        CloseGame()
        GetMatchList = ArrayList
    End Function
    
    Rem 根据赛事ID获取赛事名
    Function GetMatchTitle(MatchID)
        Dim rs,sql,n,j,roleValue
        Call ConnectGame("QPNativeWebDB")
        Set rs=Server.CreateObject("Adodb.RecordSet")
        sql="SELECT MatchTitle FROM GameMatchInfo(nolock) where MatchID="&MatchID
         rs.Open sql,GameConn,1,3
        do while not rs.eof 
            roleValue=rs("MatchTitle")          
            rs.movenext 
        loop 
        If IsNull(roleValue) Then
            rValue=0
        End If
        rs.close
        set rs=nothing
        CloseGame()
        GetMatchTitle = roleValue
    End Function
    
    Rem 获取用户IP
    Function GetUserip()
        Dim GetClientIP
        '如果客户端用了代理服务器，则应该用ServerVariables("HTTP_X_FORWARDED_FOR")方法
        GetClientIP = Request.ServerVariables("HTTP_X_FORWARDED_FOR")
        If GetClientIP = "" or isnull(GetClientIP) or isempty(GetClientIP) Then
        '如果客户端没用代理，应该用Request.ServerVariables("REMOTE_ADDR")方法
        GetClientIP = Request.ServerVariables("REMOTE_ADDR")
        end if
        GetUserip = GetClientIP
    End function
    
    Rem 获取IP绑定值
    Function GetIPBandValue()
        Dim rs,sql,n,j,roleValue
        Call ConnectGame(QPPlatformManagerDB)
        Set rs=Server.CreateObject("Adodb.RecordSet")
        sql="SELECT IsBand FROM Base_Users(nolock) where userid="&session("AdminID")
         rs.Open sql,GameConn,1,3
        do while not rs.eof 
            roleValue=rs("IsBand")          
            rs.movenext 
        loop 
        If IsNull(roleValue) Then
            rValue=0
        End If
        rs.close
        set rs=nothing
        CloseGame()
        GetIPBandValue = roleValue
    End function
    
    Rem 安全日志
    Sub InsertSystemSecurity(OperatingName)
        Call ConnectGame(QPPlatformManagerDB)
        GameConn.execute "Insert into SystemSecurity (OperatingTime,OperatingName,OperatingIP,OperatingAccounts) values ('"&Now()&"','"&OperatingName&"','"&Request.ServerVariables("REMOTE_ADDR")&"','"&session("AdminName")&"')"
    End Sub
    
End Class    

Rem 建立类对象
Dim CxGame
Set CxGame=New GameObj
%>