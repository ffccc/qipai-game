<!--#include file="CommonFun.asp"-->
<!--#include file="conn.asp"-->
<!--#include file="GameConn.asp"-->

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<%
    Call ConnectGame(QPPlatformManagerDB)
    dim Action
    Action = Trim(Request("Action"))
    
    If Action="band" then
        call Band()
    Else
        call UnBand()
    End If
    Sub Band()
        GameConn.execute "Update Base_Users Set BandIP='"&CxGame.GetUserip()&"',IsBand=0 where userid="&session("AdminID")
        CxGame.InsertSystemSecurity("绑定IP")
        Response.Redirect "../Index.asp"
    End Sub
    
    Sub UnBand()
        GameConn.execute "Update Base_Users Set IsBand=1 where userid="&session("AdminID")
        CxGame.InsertSystemSecurity("解除IP绑定")
        Response.Redirect "../Index.asp"
    End Sub
%>