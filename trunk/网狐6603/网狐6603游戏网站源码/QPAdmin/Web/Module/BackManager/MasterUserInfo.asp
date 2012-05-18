<!--#include file="../../CommonFun.asp"-->
<!--#include file="../../GameConn.asp"-->
<!--#include file="../../conn.asp"-->
<!--#include file="../../md5.asp"-->

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" >
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <title><%=GetQPAdminSiteName() %></title>
    <link href="../../Css/layout.css" rel="stylesheet" type="text/css" />
</head>
<body>
    <!-- 头部菜单 Start -->
    <table width="100%" border="0" cellpadding="0" cellspacing="0" class="title">
        <tr>
            <td width="19" height="25" valign="top"  class="Lpd10"><div class="arr"></div></td>
            <td width="1232" height="25" valign="top" align="left">目前操作功能：管理员密码修改</td>
        </tr>
    </table>
    <!-- 头部菜单 End -->
    <% 
        Select case lcase(Request("action"))
            case "save"
            Call SaveUserInfo()
            Call Main()
            case else
            Call Main()
        End Select
        
        Sub SaveUserInfo()
            Dim rs,sql
            Dim oldPwd,oldPwd2,newPwd,newPwd2
            oldPwd = md5(CxGame.GetInfo(0,"in_OldPwd"),32)
            oldPwd2 = CxGame.GetInfo(0,"in_OldPwdhdf")
            newPwd = CxGame.GetInfo(0,"in_NewPwd")
            newPwd2 = CxGame.GetInfo(0,"in_ConfirmNewPwd")
            If oldPwd<>oldPwd2 Then
                CxGame.MessageBox("原始密码输入错误！")                
                Exit Sub
            End If
            If newPwd="" Then
                CxGame.MessageBox("新输入的密码不能为空！")                
                Exit Sub
            End If
            If newPwd<>newPwd2 Then
                CxGame.MessageBox("二次输入的密码不相同，请重新输入！")
                Exit Sub
            End If
            Call ConnectGame("QPPlatformManagerDB")
            Set rs=Server.CreateObject("Adodb.RecordSet")
            sql = "select * from Base_Users where Username='"&Request("UserName")&"'"
            rs.Open sql,GameConn,1,3
            rs("Password") = md5(newPwd,32)
            rs("RoleID") = Request("in_Role")
            rs.update
            CxGame.InsertSystemSecurity("修改密码")
            Response.Write "<script>window.close()</script>"
            rs.close()
            Set rs = Nothing   
        End Sub
        
        Sub Main()
            Dim rs,sql
            Call ConnectGame(QPPlatformManagerDB)
            Set rs=Server.CreateObject("Adodb.RecordSet")
            sql = "select * from Base_Users where Username='"&Request("UserName")&"'"
            rs.Open sql,GameConn,1,3    
    %>
    <form id="form1" method="post" action="?action=save&UserName=<%=Request("UserName") %>">
        <table width="99%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
                <td class="titleOpBg Lpd10">
                    <input type="button" value="关闭" class="btn wd1" onclick="window.close()" />                
                    <input type="submit" value="保存" class="btn wd1" />   
                </td>
            </tr>
        </table>
        <table width="99%" border="0" align="center" cellpadding="0" cellspacing="0" class="listBg2">
            <tr>
	            <td class="listTdLeft">用户名：</td>
	            <td>
	                <span style="color:Blue;"><%=rs("UserName") %></span>                   
	            </td>
	        </tr>
	        <tr>
                <td class="listTdLeft">
                    权限组别：
                </td>
                <td>                
                    <span><%=CxGame.GetRoleNameByRoleID(rs("RoleID")) %></span>
                </td>
            </tr>
	        <tr>
	            <td class="listTdLeft">原始密码：</td>
	            <td>
	                <input name="in_OldPwd" type="password" class="text" /> 
                    <input name="in_OldPwdhdf" type="hidden" value="<%=rs("Password") %>" />    
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">新密码：</td>
	            <td>
	                <input name="in_NewPwd" type="password" class="text" />              
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">确认新密码：</td>
	            <td>
	                <input name="in_ConfirmNewPwd" type="password" class="text"/>               
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">登录次数：</td>
	            <td>
	                <span><%=rs("LoginTimes") %></span> 
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">最后登录地址：</td>
	            <td>
	                <span><%=rs("LastLoginIP") %>&nbsp;&nbsp;</span><%=GetCityFromIP(rs("LastLoginIP")) %>        
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">最后登录时间：</td>
	            <td>
	                <span><%=rs("LastLogintime") %></span>
	            </td>
	        </tr>
        </table>         
    </form>
    <% 
        rs.Close()
        Set rs=nothing
        End Sub
    %>
</body>
</html>
