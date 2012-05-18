<!--#include file="Inc/CommonFun.asp" -->
<!--#include file="Inc/AutoCheck.asp" -->
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
    <script src="JS/formValidator.js" type="text/javascript"></script>
    <script src="JS/formValidatorRegex.js" type="text/javascript"></script>
    
    <script type="text/javascript">
        $(document).ready(function(){
            //页面验证
            $.formValidator.initConfig({formid:"form1",alertmessage:false});
            $("#txtNewPass").formValidator({onshow:"请输入密码，至少需要6位！",onfocus:"请输入密码，至少需要6位！"})
                .inputValidator({min:6,onerror:"你输入的密码非法,请确认"});
            $("#txtNewPass2").formValidator({onshow:"确认密码必须和新密码完全一致！",onfocus:"请输入密码，至少需要6位！"})
                .compareValidator({desid:"txtNewPass",operateor:"=",onerror:"两次密码不一致,请确认"});
        })
    </script>
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
		    Dim rs
		    Set rs = GameConn.Execute("WSP_PW_GetAccountProtectByUserID "&Session("UserID")&"")
		    If rs(0)<>0 Then
	            CxGame.MessageBoxReload rs(1),"MemberIndex.asp"
	        Else
		%>
		<div class="mainRight2">
			<div class="userRigthBody">
				<div class="userRigthTitle">会员中心 - 帐号安全 - 找回保险柜密码</div>
				<div class="userRigthBg">
				  <!--找回密码开始-->     
				  <form id="form1" name="form1" method="post" action="">          
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
                      <td width="47%" align="left"><span class="txtResponse1Tip"></span></td>
                    </tr>
                    <tr bgcolor="#f7f9fd">
                      <td align="right">问题：</td>
                      <td colspan="2" align="left"><%=rs("Question2") %></td>
                    </tr>                   
                    <tr>
                      <td height="40" align="right">答案：</td>
                      <td width="27%" align="left"><input type="text" id="txtResponse2" name="txtResponse2" class="text" /></td>
                      <td width="47%" align="left"><span class="txtResponse1Tip"></span></td>
                    </tr>
                    <tr bgcolor="#f7f9fd">
                      <td align="right">问题：</td>
                      <td colspan="2" align="left"><%=rs("Question3") %></td>
                    </tr>                   
                    <tr>
                      <td height="40" align="right">答案：</td>
                      <td width="27%" align="left"><input type="text" id="txtResponse3" name="txtResponse3" class="text" /></td>
                      <td width="47%" align="left"><span class="txtResponse1Tip"></span></td>
                    </tr>     
                    <tr  bgcolor="#F7F9FD">
                      <td align="right">新密码：</td>
                      <td align="left"><input type="password" id="txtNewPass" name="txtNewPass" class="text" style="width:149px;" /></td>
                      <td align="left"><span id="txtNewPassTip"></span></td>
                    </tr>

                    <tr bgcolor="#F7F9FD">
                      <td align="right">确认密码：</td>
                      <td align="left"><input type="password" id="txtNewPass2" name="txtNewPass2" class="text" style="width:149px;" /></td>
                      <td align="left"><span id="txtNewPass2Tip"></span></td>
                    </tr>
                    <tr>
                      <td  align="right">&nbsp;</td>
                      <td colspan="2" align="left">
                        <input name="button223" type="submit" class="bnt lan bold" value="确定" hidefocus="true"/>
                        <input name="button242" type="reset" class="bnt lan bold" value="重置" hidefocus="true"/>
                        <input type="hidden" name="InsurePass" value="true" />
                      </td>
                      </tr>
                    </table>
                   </form>     
                <!--找回密码开始-->
                <% 
                    rs.Close:Set rs=Nothing:CloseGame()
                    End If
                    CxGame.ResetPassword() 
                %>
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
