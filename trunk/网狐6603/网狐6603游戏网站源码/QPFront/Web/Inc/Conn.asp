<%

'游戏数据库
Const RLGameIsSql = 1
Dim GameConn
Dim Ver

Ver="6.6.0.3"

Dim QPPlatformDB,QPAccountsDB,QPTreasureDB,QPRecordDB,QPNativeWebDB,QPGameMatchDB
QPPlatformDB="QPPlatformDB"
QPAccountsDB="QPAccountsDB"
QPTreasureDB="QPTreasureDB"
QPRecordDB = "QPRecordDB"
QPNativeWebDB = "QPNativeWebDB"
QPGameMatchDB="QPGameMatchDB"

Sub ConnectGame(lDataBase)
Dim ConnStr
	If RLGameIsSql = 1 Then
		'sql数据库连接参数：数据库名、用户密码、用户名、连接名（本地用local，外地用IP）
		Dim SqlDatabaseName,SqlPassword,SqlUsername,SqlLocalName
		SqlDatabaseName = lDataBase
		SqlPassword = "Welcome123"
		SqlUsername = "sa"
		SqlLocalName = "127.0.0.1,1433"  
		ConnStr = "Provider = Sqloledb; User ID = " & SqlUsername & "; Password = " & SqlPassword & "; Initial Catalog = " & SqlDatabaseName & "; Data Source = " & SqlLocalName & ";"		
	End If
	On Error Resume Next
	Set GameConn = Server.CreateObject("ADODB.Connection")
	GameConn.open ConnStr
	If Err Then		
	    err.Clear
		Set GameConn = Nothing		
		Response.Write "数据库连接出错，请检查连接字串。"
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

%>