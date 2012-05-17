<!--#include file="Conn.asp" -->
<% 
    If Trim(Request("t")) = "CheckUser" Then
        Call CheckUser()
    End If    
    
    Rem 检查用户
    Sub CheckUser()
        Dim username
        username = Trim(Request("txtAccounts"))
        Dim rs,sql
        Call ConnectGame(QPAccountsDB)
        Set rs=Server.CreateObject("Adodb.RecordSet")
        sql = "SELECT UserID FROM AccountsInfo(nolock) WHERE Accounts='"&username&"'"
        rs.Open sql,GameConn,1,3
        If Not rs.eof Then   
            rs.close
            set rs=nothing
            CloseGame()  
            response.Write 1
            Exit Sub
        End If 
        rs.close
        set rs=nothing
        CloseGame() 
        response.Write 0
    End Sub
%>