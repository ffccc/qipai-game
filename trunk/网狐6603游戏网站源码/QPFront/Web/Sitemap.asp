<!--#include file="Inc/CommonFun.asp" -->
<!--#include file="Inc/Conn.asp" -->
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
    <link rel="stylesheet" type="text/css" href="Css/news_layout.css" />
</head>
<body>
<%
    CxGame.Channel = "Member" 
%>
<!--#include file="Themes/Common_Header.asp" -->

<!--页面主体开始-->
<div class="main" style="margin-top:8px;">
	<!--左边部分开始-->
	<!--#include file="Themes/Common_Sidebar_A.asp" -->	
	<!--左边部分结束-->
	
	<!--右边部分开始-->
	<div class="mainRight1">
		<!--新闻公告开始-->
		<div class="newsBody">
			<div class="mapTitle"></div>
		  <div class="tgBg">
				<div class="tgTitle1 bold">网站导航</div>
           		<div class="content">
					<ul>
					<li><a href="Index.asp" class="lw">首页</a></li>
					<li><a href="NewsList.asp" class="lw">新闻公告</a></li>
					<li><a href="MemberIndex.asp" class="lw">会员中心</a></li>
					<li><a href="PayIndex.asp" class="lw">充值中心</a></li>
					<li><a href="MatchIndex.asp" class="lw">比赛中心</a></li>
					<li><a href="ShopIndex.asp" class="lw">游戏商场</a></li>
					<li><a href="SpreadIndex.asp" class="lw">推广系统</a></li>
					<li><a href="ServiceIndex.asp" class="lw">客服中心</a></li>
					</ul>
				<div class="clear"></div>
				</div>
				
				<div class="tgTitle1 bold">会员中心</div>
				<div class="content">
					<ul>
					<li><a href="MemberModifyNikeName.asp" class="lw">修改昵称</a></li>
					<li><a href="MemberModifyUserInfo.asp" class="lw">修改资料</a></li>
					<li><a href="MemberModifyFace.asp" class="lw">修改头像</a></li>
					<li><a href="MemberModifyLogonPass.asp" class="lw">修改登录密码</a></li>
					<li><a href="MemberModifyInsurePass.asp" class="lw">修改保险柜密码</a></li>
					<li><a href="MemberApplyProtect.asp" class="lw">申请密码保护</a></li>
					<li><a href="MemberModifyProtect.asp" class="lw">修改密码保护</a></li>
					<li><a href="MemberReLogonPass.asp" class="lw">找回登录密码</a></li>
					<li><a href="MemberReInsurePass.asp" class="lw">找回保险柜密码</a></li>
					<li><a href="MemberInsureIn.asp" class="lw">存款</a></li>
					<li><a href="MemberInsureOut.asp" class="lw">取款</a></li>
					<li><a href="MemberInsureTransfer.asp" class="lw">转帐</a></li>
					<li><a href="MemberInsureRecord.asp" class="lw">交易明细</a></li>
					<li><a href="MemberConvertPresent.asp" class="lw">魅力兑换</a></li>
					<li><a href="MemberConvertRecord.asp" class="lw">魅力兑换记录</a></li>
					<li><a href="MemberClearScore.asp" class="lw">负分清零</a></li>
					<li><a href="MemberClearFlee.asp" class="lw">逃跑清零</a></li>
					<li><a href="MemberMoorMachine.asp" class="lw">固定机器</a></li>
					<li><a href="MemberSpreadIn.asp" class="lw">业绩查询</a></li>
					<li><a href="MemberSpreadIn.asp" class="lw">结算查询</a></li>
					<li><a href="MemberSpreadBalance.asp" class="lw">业绩结算</a></li>
					<li><a href="MemberSpreadInfo.asp" class="lw">推广明细</a></li>
					</ul>
				<div class="clear"></div>
				</div>
				
				<div class="tgTitle1 bold">充值中心</div>
   		    	<div class="content">
					<ul>
					<li><a href="PayCardFill.asp" class="lw">实卡充值</a></li>
					<li><a href="PayOnlineFill.asp" class="lw">快钱充值</a></li>
					<li><a href="PayOnlineFill.asp" class="lw">易宝充值</a></li>
					</ul>
				<div  class="clear"></div>
				</div>
				
				<div class="tgTitle1 bold">客服中心</div>
   		    	<div class="content">
					<ul>
					<li><a href="ServiceIndex.asp" class="lw">新手帮助</a></li>
					<li><a href="ServiceIssue.asp" class="lw">常见问题</a></li>
					<li><a href="ServiceFeedback.asp" class="lw">问题反馈</a></li>
					<li><a href="ServiceCustomer.asp" class="lw">客服电话</a></li>
					</ul>
				<div  class="clear"></div>
				</div>
				
				<div class="tgTitle1 bold">下载中心</div>
   		    	<div class="content">
					<ul>
					<li><a href="http://download.foxuc.com/loader/whgame/unicode/WHGamePlaza.exe" class="lw">精简版下载</a></li>
					<li><a href="http://download.foxuc.com/loader/whgame/unicode/WHGameFull.exe" class="lw">完整版下载</a></li>
					</ul>
				<div  class="clear"></div>
				</div>
				
		  </div>
		  <div class="clear"></div>
			<div class="newsBottom"><div class="clear"></div></div>
		<div class="clear"></div>
		</div>
		<!--新闻公告结束-->
	<div class="clear"></div>
 	</div>
	<!--右边部分结束-->
<div class="clear"></div>
</div>
<!--页面主体结束-->

<!--#include file="Themes/Common_Footer.asp" -->
</body>
</html>
