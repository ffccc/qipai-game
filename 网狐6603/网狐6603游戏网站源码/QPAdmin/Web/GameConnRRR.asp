<%
'游戏数据库
Const RLGameIsSql = 1
Dim GameConn
Dim Ver

Ver="6.6.0.3"

Dim QPPlatformDB,QPAccountsDB,QPTreasureDB,QPRecordDB,QPPlatformManagerDB,QPNativeWebDB
QPPlatformDB="QPPlatformDB"
QPAccountsDB="QPAccountsDB"
QPTreasureDB="QPTreasureDB"
QPRecordDB = "QPRecordDB"
QPPlatformManagerDB="QPPlatformManagerDB"
QPNativeWebDB="QPNativeWebDB"
QPLandDB="QPLandDB"

Sub ConnectGame(lDataBase)
Dim ConnStr,RLWebSqlNow
	If RLGameIsSql = 1 Then
		'sql数据库连接参数：数据库名、用户密码、用户名、连接名（本地用local，外地用IP）
		Dim SqlDatabaseName,SqlPassword,SqlUsername,SqlLocalName
		SqlDatabaseName = lDataBase
		SqlPassword = "xx1433~!"
		SqlUsername = "sa"
		SqlLocalName = "98.126.218.4,1433"  
		ConnStr = "Provider = Sqloledb; User ID = " & SqlUsername & "; Password = " & SqlPassword & "; Initial Catalog = " & SqlDatabaseName & "; Data Source = " & SqlLocalName & ";"
		RLWebSqlNow = "GetDate()"		
	Else
		'Access用户
		ConnStr = "Provider = Microsoft.Jet.OLEDB.4.0;Data Source = " & Server.MapPath(RLDbPath & RLAccessDb)
		RLWebSqlNow = "Now()"
	End If
	On Error Resume Next
	Set GameConn = Server.CreateObject("ADODB.Connection")
	GameConn.open ConnStr
	If Err Then		
		Set GameConn = Nothing		
		Response.Write "数据库连接出错，请检查连接字串。"
		If Session("TcName")="admin" Then
			Response.Write Err.Description
		Else
			err.Clear
		End If
		Response.End
	End If
End Sub

Sub CloseGame()
IF Not IsEmpty(GameConn) Then
    Exit Sub
End IF
If IsObject(GameConn) Then
    IF GameConn.State<>0 Then
	    GameConn.Close
	    Set GameConn = Nothing
	End IF
End If
End Sub

Rem 获取棋牌管理后台署名
Function GetQPAdminSiteName()
    Dim qpSitenameRs,qpSitename,sql,rs
    Call ConnectGame(QPPlatformManagerDB)
    Set rs=Server.CreateObject("Adodb.RecordSet")
    sql="select top 1 SiteName from QPAdminSiteInfo"
    rs.Open sql,GameConn,1,3    
    If Not rs.Eof Then
        qpSitename=rs(0)
    Else
        qpSitename="网狐棋牌平台管理后台"
    End If
    rs.Close
    Set rs=Nothing
    GetQPAdminSiteName= qpSitename   
End Function

Rem 获取版权名称
Function GetCopyRightName()
    Dim rs,rValue,sql
    Call ConnectGame(QPPlatformManagerDB)
    Set rs=Server.CreateObject("Adodb.RecordSet")
    sql="select top 1 CopyRight from QPAdminSiteInfo"
    rs.Open sql,GameConn,1,3  
    If Not rs.Eof Then
        rValue = rs(0)
    Else
        rValue = "CopyRight 2010 深圳市网狐科技有限公司"
    End If
    Set rs=nothing
    GetCopyRightName = rValue
End Function

Rem 获取列表数据数
Function GetPageSize()
    Dim rs,rValue,sql
    Call ConnectGame(QPPlatformManagerDB)
    Set rs=Server.CreateObject("Adodb.RecordSet")
    sql="select top 1 PageSize from QPAdminSiteInfo"
    rs.Open sql,GameConn,1,3  
    If Not rs.Eof Then
        If Not IsNumeric(rs(0)) Then
            rValue = 30
        Else        
            rValue = rs(0)
        End If
    Else
        rValue = 30
    End If
    Set rs=nothing
    GetPageSize = rValue
End Function

Rem 获取棋牌管理员名称
Function GetQPAdminName(userID)
    Dim rs,rValue,sql
    rValue=""
    Call ConnectGame(QPPlatformManagerDB)
    Set rs=Server.CreateObject("Adodb.RecordSet")
    sql="select * from Base_Users where UserID="&userID
    rs.Open sql,GameConn,1,3  
    If Not rs.eof Then
        rValue = rs(1)
    End If
    rs.Close
    Set rs = Nothing
    GetQPAdminName = rValue
End Function
%>