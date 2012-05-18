<!--#include file="../../CommonFun.asp"-->
<!--#include file="../../GameConn.asp"-->
<!--#include file="../../conn.asp"-->
<!--#include file="../../md5.asp"-->

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" >
<head>
    <title></title>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <link href="../../Css/layout.css" rel="stylesheet" type="text/css" />
    <link href="../../Css/setting.css" rel="stylesheet" type="text/css" /> 
</head>
<body>
    <!-- 头部菜单 Start -->
    <table width="100%" border="0" cellpadding="0" cellspacing="0" class="title">
        <tr>
            <td width="19" height="25" valign="top"  class="Lpd10"><div class="arr"></div></td>
            <td width="1232" height="25" valign="top" align="left">前操作功能：确认密码</td>
        </tr>
    </table>
    <!-- 头部菜单 End -->
    <% 
        Call ConnectGame(QPRecordDB)
        Select case Request("action")
            case "save"
                Call Confirm()
            case else
                Call Main()
        End Select 
        Call CloseGame()
        
        '确认密码
        Sub Confirm()
            Dim rs,sql
            Set rs=Server.CreateObject("Adodb.RecordSet")
            If Request("Type") = 1 Then
                sql = "select ReLogonPasswd from RecordPasswdExpend where RecordID = "&Request("id")
            Else
                sql = "select ReInsurePasswd from RecordPasswdExpend where RecordID = "&Request("id")
            End If
            rs.Open sql,GameConn,1,3
            If md5(CxGame.GetInfo(0,"in_Pwd"),32) = rs(0) Then
                CxGame.MessageBoxClose("确认成功！")
            Else
                CxGame.MessageBoxClose("确认失败！")
            End If
            Set rs= Nothing
        End Sub
        
        Sub Main()
    %>
    <form name="form1" method="post" action='?action=save&id=<%=Request("id") %>&UserID=<%=Request("UserID") %>&Type=<%=Request("Type") %>' onsubmit="return CheckForm()">
        <table width="99%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
                <td class="titleOpBg Lpd10">
                    <input type="button" value="关闭" class="btn wd1" onclick="window.close()" />                
                    <input type="submit" value="确认" class="btn wd1" />   
                </td>
            </tr>
        </table>
        <table width="99%" border="0" align="center" cellpadding="0" cellspacing="0" class="listBg2">
            <tr>
	            <td class="listTdLeft"> 用户名：</td>
	            <td>
	                <span style="color:Blue;"><%=CxGame.GetUserNameByUserID(Request("UserID")) %></span>     
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft"> 密码：</td>
	            <td>
	                <input name="in_Pwd" type="password" class="text wd4"/>            
	            </td>
	        </tr>
        </table>
        <table width="99%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
                <td class="titleOpBg Lpd10">
                    <input type="button" value="关闭" class="btn wd1" onclick="window.close()" />                
                    <input type="submit" value="确认" class="btn wd1" />   
                </td>
            </tr>
        </table>
    </form>
    <% End Sub %>
</body>
</html>
