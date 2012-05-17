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
            $("#lblTitle").attr("innerText","转入帐号：");
            $("#radType1").bind("click",function(){$("#lblTitle").attr("innerText","转入帐号：");})
            $("#radType2").bind("click",function(){$("#lblTitle").attr("innerText","转入ＩＤ：");})
            
            //页面验证
            $.formValidator.initConfig({formid:"form1",alertmessage:true,onerror:function(msg){alert(msg)}});
            $("#txtUser").formValidator().inputValidator({min:1,empty:{leftempty:false,rightempty:false,emptyerror:"两边不能有空符号！"},onerror:"转入用户不能为空！"})
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
				<div class="userRigthTitle">会员中心 - 保险柜服务 - 转账</div>
				<div class="userRigthBg">
				<!--转帐开始-->
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
			    <div class="title f14 bold lan">金币转账</div>
	            <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
	              <tr>
                    <td width="400px" colspan="2" align="center" style="padding-left:10px;">
                        <input type="radio" id="radType1" name="radType" value="0" checked="checked" /><label for="radType1" hidefocus="true">按用户名</label>
                        <input type="radio" id="radType2" name="radType" value="1" /><label for="radType2" hidefocus="true">按ＩＤ号码回</label>
                    </td>
                  </tr>
                  <tr>
                    <td width="20%" align="right"><label id="lblTitle"></label></td>
                    <td width="80%"><input type="text" id="txtUser" name="txtUser" class="text" /></td>
                  </tr>
                  <tr>
                      <td width="20%" align="right">转账金额：</td>
                      <td width="80%"><input type="text" id="txtScore" name="txtScore" class="text" /></td>
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
                        <input name="button" type="submit" class="bnt lan bold" value="确定转账" hidefocus="true"/>
                        <input name="InsureTransfer" type="hidden" value="true" />
                    </td>
                  </tr>
                </table> 
                <br />
                  <div class="title f14 bold lan">温馨提示</div>
                  <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="10%" align="right"></td>
                      <td width="90%" class="hui6">
                        1、您可以填写接收人的“用户名”或“游戏ID”进行赠送。<br>
                        2、您赠送给接收人的金币会在您携带的金币上面扣除。<br>
                        3、每次赠送，系统收取一定比例的服务费。<br>
                      </td>
                    </tr>
                  </table>       
                </form>
	            <!--转帐结束-->
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
