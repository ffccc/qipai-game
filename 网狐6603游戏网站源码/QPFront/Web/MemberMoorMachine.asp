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
    <script src="JS/jquery_last.js" type="text/javascript"></script>   
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
		<div class="mainRight2">
			<div class="userRigthBody">
				<div class="userRigthTitle">会员中心 - 会员服务 - 固定机器</div>
				<div class="userRigthBg">
				<!--固定机器开始-->
				<% 
				    ConnectGame(QPAccountsDB)
				    Select case lcase(Request("action"))
				        case "apply"
				        Call ApplyMoorMachine()		
				        case "cancel"
				        Call CancelMoorMachine()				        
				        case else
				        Call Main()
				    End Select 
				    Call CloseGame() 
				    
				    '申请绑定
				    Sub ApplyMoorMachine()
				        Dim dwUserID,strResponse1,strResponse2,strResponse3,IP,rs
				        dwUserID = Session("UserID")
                        strResponse1 = CxGame.GetInfo(0,"txtResponse1")
                        strResponse2 = CxGame.GetInfo(0,"txtResponse2")
                        strResponse3 = CxGame.GetInfo(0,"txtResponse3")
                        IP = CxGame.GetIP()
                        Set rs = GameConn.Execute("WSP_PW_ApplyMoorMachine "&dwUserID&",'"&strResponse1&"','"&strResponse2&"','"&strResponse3&"','"&IP&"'")
                        CxGame.MessageBoxReload rs(1),"MemberMoorMachine.asp"
			            rs.Close:Set rs=Nothing
				    End Sub
				    
				    '取消绑定
				    Sub CancelMoorMachine()
				        Dim dwUserID,strResponse1,strResponse2,strResponse3,IP,rs
				        dwUserID = Session("UserID")
                        strResponse1 = CxGame.GetInfo(0,"txtResponse1")
                        strResponse2 = CxGame.GetInfo(0,"txtResponse2")
                        strResponse3 = CxGame.GetInfo(0,"txtResponse3")
                        IP = CxGame.GetIP()
                        Set rs = GameConn.Execute("WSP_PW_CancelMoorMachine "&dwUserID&",'"&strResponse1&"','"&strResponse2&"','"&strResponse3&"','"&IP&"'")
                        CxGame.MessageBoxReload rs(1),"MemberMoorMachine.asp"
			            rs.Close:Set rs=Nothing
				    End Sub
				    
				    Sub Main()
				        Dim rs,rs_1
				        Set rs = GameConn.Execute("WSP_PW_GetAccountProtectByUserID "&Session("UserID")&"")
				        If rs(0)<>0 Then
				            CxGame.MessageBoxReload rs(1),"MemberIndex.asp"
				        Else				        
				            Set rs_1 = GameConn.Execute("Select MoorMachine from AccountsInfo Where UserID="&Session("UserID"))
				            If rs_1(0) = 0 Then
				%>
                <form id="form1" name="form1" method="post" action="?action=apply">
                  <table width="100%" cellpadding="0" cellspacing="0">
                    <tr class="bold">
                      <td width="26%" align="right"></td>
                      <td colspan="2" align="left">请回答下面问题</td>
                    </tr>                    
                    <tr bgcolor="#f7f9fd">
                      <td align="right">问题：</td>
                      <td colspan="2" align="left"><%=rs("Question1") %></td>
                    </tr>                   
                    <tr>
                      <td height="40" align="right">答案：</td>
                      <td width="27%" align="left"><input type="text" id="txtResponse1" name="txtResponse1" class="text" /></td>
                      <td width="47%" align="left"><span class="tipsM"></span></td>
                    </tr>
                    <tr bgcolor="#f7f9fd">
                      <td align="right">问题：</td>
                      <td colspan="2" align="left"><%=rs("Question2") %></td>
                    </tr>                   
                    <tr>
                      <td height="40" align="right">答案：</td>
                      <td width="27%" align="left"><input type="text" id="txtResponse2" name="txtResponse2" class="text" /></td>
                      <td width="47%" align="left"><span class="tipsM"></span></td>
                    </tr>
                    <tr bgcolor="#f7f9fd">
                      <td align="right">问题：</td>
                      <td colspan="2" align="left"><%=rs("Question3") %></td>
                    </tr>                   
                    <tr>
                      <td height="40" align="right">答案：</td>
                      <td width="27%" align="left"><input type="text" id="txtResponse3" name="txtResponse3" class="text" /></td>
                      <td width="47%" align="left"><span class="tipsM"></span></td>
                    </tr>                    
                    <tr>
                      <td height="40" align="right">&nbsp;</td>
                      <td colspan="2" align="left">
                        <input name="button2242" type="submit" class="bnt lan bold" value="申请绑定" hidefocus="true"/>
                        <input name="button2432" type="reset" class="bnt lan bold" value="重置" hidefocus="true"/>
                      </td>
                    </tr>
                  </table>
                </form>
                <% 
                    Else
                %>
                <form id="form2" name="form1" method="post" action="?action=cancel">
                  <table width="100%" cellpadding="0" cellspacing="0">
                    <tr class="bold">
                      <td width="26%" align="right"></td>
                      <td colspan="2" align="left">请回答下面问题</td>
                    </tr>                    
                    <tr bgcolor="#f7f9fd">
                      <td align="right">问题：</td>
                      <td colspan="2" align="left"><%=rs("Question1") %></td>
                    </tr>                   
                    <tr>
                      <td height="40" align="right">答案：</td>
                      <td width="27%" align="left"><input type="text" id="Text1" name="txtResponse1" class="text" /></td>
                      <td width="47%" align="left"><span class="tipsM"></span></td>
                    </tr>
                    <tr bgcolor="#f7f9fd">
                      <td align="right">问题：</td>
                      <td colspan="2" align="left"><%=rs("Question2") %></td>
                    </tr>                   
                    <tr>
                      <td height="40" align="right">答案：</td>
                      <td width="27%" align="left"><input type="text" id="Text2" name="txtResponse2" class="text" /></td>
                      <td width="47%" align="left"><span class="tipsM"></span></td>
                    </tr>
                    <tr bgcolor="#f7f9fd">
                      <td align="right">问题：</td>
                      <td colspan="2" align="left"><%=rs("Question3") %></td>
                    </tr>                   
                    <tr>
                      <td height="40" align="right">答案：</td>
                      <td width="27%" align="left"><input type="text" id="Text3" name="txtResponse3" class="text" /></td>
                      <td width="47%" align="left"><span class="tipsM"></span></td>
                    </tr>                    
                    <tr>
                      <td height="40" align="right">&nbsp;</td>
                      <td colspan="2" align="left">
                        <input name="button2242" type="submit" class="bnt lan bold" value="取消绑定" hidefocus="true"/>
                        <input name="button2432" type="reset" class="bnt lan bold" value="重置" hidefocus="true"/>
                      </td>
                    </tr>
                  </table>
                </form>
                <% 
                        End If
                    End If
                    End Sub
                %>
	            <!--固定机器结束-->
                <div class="clear"></div>
		 	    </div>
				<div class="userRightBottom"><div class="clear"></div></div>
				<div class="clear"></div>
			</div>
			<div class="clear"></div>
		</div>
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
