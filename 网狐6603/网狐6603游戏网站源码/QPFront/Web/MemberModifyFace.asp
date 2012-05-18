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
    
    <link type="text/css" href="Js/lhgdialog/lhgdialog.css" rel="stylesheet"/>
    <script src="JS/lhgdialog/lhgcore.min.js" type="text/javascript"></script>
    <script src="JS/lhgdialog/lhgdialog.js" type="text/javascript"></script>
    <script type="text/javascript">
	    $(document).ready(function(){
            //弹出页面
            J('#btnSwitchFace').dialog({ id: 'winUserfaceList', title: '更换头像', width:540,height:385, page: 'Inc/UserfacesList.asp',rang: true,cover: true });
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
				<div class="userRigthTitle">会员中心 - 帐号服务 - 修改头像</div>
				<div class="userRigthBg">
				  <!--修改头像开始-->
				  <form id="form1" name="form1" method="post" action="">
                  <table width="100%" border="0" cellspacing="0" cellpadding="0">
                    <tr>
                      <td colspan="2">请您选择游戏中的头像代表，以下是您正在使用的头像：</td>
                      </tr>
                    <tr>
                      <td width="36%" align="right">
                        <input id="inFaceID" name="inFaceID" type="hidden" value="<%=Session("FaceID") %>" />
				        <img id="picFace" src="/gamepic/face<%=Session("FaceID") %>.gif" alt="" />&nbsp;&nbsp;
                      </td>
                      <td width="64%">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="javascript:void(0)" id="btnSwitchFace">查看更多头像</a></td>
                    </tr>
                    <tr>
                      <td>&nbsp;</td>
                      <td>&nbsp;</td>
                    </tr>
                    <tr>
                      <td>&nbsp;</td>
                      <td>
                          <input name="button4" type="submit" class="bnt lan bold" value="确定" hidefocus="true"/>
                          <input name="Modify" type="hidden" value="true" />
                      </td>
                    </tr>
                  </table>
                  <%CxGame.ModifyFace() %>
                  </form>
	              <!--修改头像结束-->
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
