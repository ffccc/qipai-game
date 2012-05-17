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
            $("#txtOldPass").formValidator({onshow:"请输入原始密码！",onfocus:"请输入密码，至少需要6位！"})
                .inputValidator({min:6,onerror:"你输入的密码非法,请确认"});
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
		<div class="mainRight2">
			<div class="userRigthBody">
				<div class="userRigthTitle">会员中心 - 帐号服务 - 修改保险柜密码</div>
				<div class="userRigthBg">
				  <!--修改登录密码开始-->
				  <form id="form1" name="form1" method="post" action="">
                  <table width="100%" border="0" cellspacing="0" cellpadding="0">
                  <tr>
                    <td width="200" align="right">原密码：</td>
                    <td>
                        <input type="password" id="txtOldPass" name="txtOldPass" class="text" />&nbsp;
                        <span id="txtOldPassTip"></span>
                    </td>                    
                  </tr>
                  <tr>
                    <td align="right">新密码：</td>
                    <td>
                        <input type="password" id="txtNewPass" name="txtNewPass" class="text" />&nbsp;
                        <span id="txtNewPassTip"></span>
                    </td>
                  </tr>
                  <tr>
                    <td align="right">确认新密码：</td>
                    <td>
                        <input type="password" id="txtNewPass2" name="txtNewPass2" class="text" />&nbsp;
                        <span id="txtNewPass2Tip"></span>
                    </td>
                  </tr>
                  <tr>
                    <td>&nbsp;</td>
                    <td>
                        <input name="button" type="submit" class="bnt lan bold" value="确定" hidefocus="true"/>
                        <input name="button" type="reset" class="bnt lan bold" value="取消" hidefocus="true"/>
                        <input name="Modify" type="hidden" value="true" />
                    </td>
                  </tr>      
                  </table>
                  <% CxGame.ModifyPassword("InsurePass")%>
                  </form>
	              <!--修改登录密码结束-->
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
