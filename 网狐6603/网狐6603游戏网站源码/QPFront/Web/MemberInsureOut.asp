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
           $.formValidator.initConfig({formid:"form1",alertmessage:true,onerror:function(msg){alert(msg)}});
            $("#txtScore").formValidator().inputValidator({min:1000,type:"number",onerror:"非常抱歉，您每笔取出数目最少 1000 金币！"})
                .regexValidator({regexp:"intege1",datatype:"enum",onerror:"取出的金币必须为整数，并且不能大于保险柜数目！"});            
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
		<% CxGame.OPInsure()%>
		<div class="mainRight2">
			<div class="userRigthBody">
				<div class="userRigthTitle">会员中心 - 保险柜服务 - 取款</div>
				<div class="userRigthBg">
				<!--取款开始-->
				  <form name="form1" id="form1" method="post" action="">
				  <div class="title f14 bold lan">财富信息</div>
				  <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
				    <tr>
				        <td width="20%"></td>	        
				        <td>保险柜存款：<%=Session("InsureScore") %> 金币  <a href="PayIndex.asp" class="ll">我要充值</a></td>
				    </tr>
				    <tr>
				        <td></td>	        
				        <td>现金余额：<%=Session("Score") %> 金币</td>
				    </tr>
				  </table>
				  <div class="title f14 bold lan">取出金币</div>
	              <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="20%" align="right">取出金币：</td>
                      <td width="80%"><input type="text" id="txtScore" name="txtScore" class="text" value="<%=Session("InsureScore") %>" /></td>
                    </tr>
                    <tr>
                      <td align="right">保险柜密码：</td>
                      <td><input type="password" id="txtInsurePass" name="txtInsurePass" class="text" style="width:149px;" /></td>
                    </tr>
                    <tr>
                      <td align="right">备注信息：</td>
                      <td><textarea id="txtNote" name="txtNote" rows="5" wrap="virtual" class="beizhu" ></textarea></td>
                    </tr>
                    <tr>
                      <td>&nbsp;</td>
                      <td>
                        <input name="button2" type="submit" class="bnt lan bold" value="取出金币" hidefocus="true"/>
                        <input name="InsureOut" type="hidden" value="true" />
                      </td>
                    </tr>
                  </table>
                  <br />
                  <div class="title f14 bold lan">温馨提示</div>
                  <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="10%" align="right"></td>
                      <td width="90%" class="hui6">
                        1、如果您在金币类游戏中，您将不能够取出金币。<br />
                        2、有任何问题，请您联系客服中心。<br />
                      </td>
                    </tr>
                  </table>        
                  </form>
	            <!--取款结束-->
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
