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
    <script src="JS/jquery_last.js" type="text/javascript"></script>
    <script type="text/javascript">
        $(document).ready(function(){
            //验证码
            $("#ImageCheck").bind("click",function(){$("#ImageCheck").attr("src","Inc/Code_Img2.asp?"+ Math.random())})  
            $("#ImageCheck2").bind("click",function(){$("#ImageCheck").attr("src","Inc/Code_Img2.asp?"+ Math.random())})  
            $("#txtAccounts").focus();
        })
    </script>
</head>
<body>
<%
    If Request("Type")="" Then
        CxGame.Channel = "Member"  
    Else
        CxGame.Channel = Request("Type")
    End If 
%>
<!--#include file="Themes/Common_Header.asp" -->
<!--页面主体开始-->
<div class="main">
    <form name="form1" action="" method="post">
	<div class="logonBody">
		<div class="left"><img src="images/login01.jpg" border="0" usemap="#Map" />
        <map name="Map" id="Map"><area shape="rect" coords="6,154,142,178" href="Register.asp" /></map>
        </div>
		<div class="left"><img src="images/login02.jpg" /></div>
		<div class="left"><img src="images/login03.jpg" /></div>
 	    <div class="logonBg">
			<div class="left"><img src="images/login04.jpg" /></div>
	      <div class="logonBg1">
	        <table width="277" border="0" cellspacing="0" cellpadding="0">
                <tr>
                  <td width="70" height="35" class="f14" align="right">帐　号：</td>
                  <td width="207" height="35"><input name="txtAccounts" id="txtLoginName" type="text" class="text" /></td>
                </tr>
                <tr>
                  <td class="f14" align="right">密　码：</td>
                  <td><input name="txtLogonPass" type="password" class="text" style="width:149px;" /></td>
                </tr>
                <tr>
                  <td class="f14" align="right">验证码：</td>
                  <td><input type="text" name="txtCode" class="text" /><input type="hidden" name="login" value="true" /></td>
                </tr>
                <tr>
                  <td></td>
                  <td class="hui6">验证码请按下图中的数字填写</td>
                </tr>
                <tr>
                  <td></td>
                  <td><img src="Inc/Code_Img2.asp" name="ImageCheck" id="ImageCheck" style="cursor:pointer;border: 1px solid #333333;" title="点击更换验证码图片!" /></td>
                </tr>
                <tr>
                  <td></td>
                  <td><a href="javascript:void(0)" class="ll" id="ImageCheck2">看不清楚？ 换一个</a></td>
                </tr>
                <tr>
                  <td></td>
                  <td><input type="image" src="images/login1.gif" width="86" height="33" />&nbsp;&nbsp;<a href="Register.asp"><img src="images/reg1.gif" width="86" height="33" border="0" /></a></td>
                </tr>
              </table>
	      </div>
			<div class="logonBg2"><label><a href="MemberReLogonPass.asp" class="lh">忘记密码？</a></label></div>
			<div class="left"><img src="images/login06.jpg" /></div>
			<div class="clear"></div>
	    </div>
		<div class="left"><img src="images/login07.jpg" /></div>
		<div class="clear"></div>
	</div>
	<%CxGame.UserLogin() %>
	</form>
	<div class="clear"></div>
</div>
<!--页面主体结束-->
<!--#include file="Themes/Common_Footer.asp" -->
</body>
</html>
