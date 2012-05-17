<!--#include file="Inc/CommonFun.asp" -->
<!--#include file="Inc/Conn.asp" -->
<!--#include file="Inc/md5.asp" -->

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" >
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <meta http-equiv="X-UA-Compatible" content="IE=EmulateIE7" />
    <meta name="Description" content="<%=CxGame.WebTitle %>" />
    <meta name="Keywords" content="<%=CxGame.WebTitle %>" />
    <meta name="rating" content="general" />
    <meta name="robots" content="all" />
    <title><%=CxGame.WebTitle %></title>
    <link rel="stylesheet" type="text/css" href="css/layout.css" />
    <link rel="stylesheet" type="text/css" href="css/global.css" />
    <link rel="stylesheet" type="text/css" href="Css/user_layout.css" />
    <link rel="stylesheet" type="text/css" href="Css/game_layout.css" />    
</head>
<body>
<%
    CxGame.Channel = "Member" 
    CxGame.IsCheckLogin
%>
<!--#include file="Themes/Common_Header.asp" -->

<!--页面主体开始-->
<div class="userBody" style="margin-top:8px;">
	<div class="userTop"></div>
	<div class="userBg">
	    <!--左边页面开始-->
	    <div class="LeftSide">
			<!--会员登录开始-->
			<!--#include file="Themes/Common_Logon.asp" -->
			<!--会员登录结束-->
			<!--#include file="Themes/Member_Sidebar.asp" -->					
		</div>
		<!--左边页面结束-->
		
		<!--右边页面开始-->
		<% 
		    ConnectGame(QPAccountsDB)
		    Set rs = GameConn.Execute("Select * from AccountsInfo(nolock) where UserID="&Session("UserID"))
		    If rs.eof Then
		        Session("UserID")=empty
			    Session("GameID")=empty
			    Session("UserName")=empty
			    Session("NikeName")=empty
			    Session("FaceID") = empty
			    Session("MemberOrder") = empty
			    Session("Loveliness") = empty
			    CxGame.MessageBoxReload "帐号冻结，请联系管理员！","Index.asp"
		    Else
		    
		%>
		<div class="mainRight2">
			<div class="userRigthBody">
				<div class="userRigthTitle">首页 - 帐号服务中心 - 会员服务</div>
				<div class="userRigthBg">
					<div class="title f14 bold lan">基本信息</div>
				  	<table width="96%" border="0" align="center" cellpadding="0" cellspacing="0">
                      <tr>
                        <td colspan="4">尊敬的 <span class="hong bold"><%=rs("Accounts") %></span> ,欢迎您来到网狐棋牌用户个人中心。</td>
                      </tr>
                      <tr>
                        <td width="15%" align="right" bgcolor="#f7f9fd">昵 　 称：</td>
                        <td width="29%" bgcolor="#f7f9fd"><%=rs("Nickname") %></td>
                        <td width="19%" align="right" bgcolor="#f7f9fd">性　　别：</td>
                        <td width="37%" bgcolor="#f7f9fd">
                        <% If rs("Gender") = 0 Then %>
                        女
                        <% Else %>
                        男
                        <%End If %>                        
                        </td>
                      </tr>
                      <tr>
                        <td align="right">会员等级：</td>
                        <td><%=CxGame.GetMemberOrderName(rs("MemberOrder")) %></td>
                        <td align="right">经 验 值：</td>
                        <td><%=rs("Experience") %></td>
                      </tr>
                      <tr>
                        <td align="right" bgcolor="#F7F9FD">个性签名：</td>
                        <td colspan="3" bgcolor="#F7F9FD"><%=rs("UnderWrite") %></td>
                      </tr>
                    </table>
					<br /><br />
					<div class="title f14 bold lan">快捷通道</div>
						<div class="userTD">
							<div><a href="ServiceIndex.asp" class="help" hidefocus="true"></a></div>
							<div><a href="#" class="game" hidefocus="true"></a></div>
							<div><a href="PayIndex.asp" class="user" hidefocus="true"></a></div>
							<div><a href="MemberApplyProtect.asp" class="protect" hidefocus="true"></a></div>
							<div><a href="MemberModifyLogonPass.asp" class="pass" hidefocus="true"></a></div>
							<div><a href="MemberInsureTransfer.asp" class="bank" hidefocus="true"></a></div>
							<div class="clear"></div>
						</div>
					
				  	<div class="clear"></div>
	 	  	    </div>
				<div class="userRightBottom"><div class="clear"></div></div>
				<div class="clear"></div>
			</div>
			<div class="clear"></div>
		</div>
		<%
            rs.Close:Set rs=Nothing:CloseGame()
            End If
        %>     
		<!--右边页面结束-->
		<div class="clear"></div>
	</div>
	<div class="userBottom"><div class="clear"></div></div>
<div class="clear"></div>
</div>
<!--页面主体结束-->
<!--#include file="Themes/Common_Footer.asp" -->
</body>
</html>
