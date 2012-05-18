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
    <link rel="stylesheet" type="text/css" href="css/news_layout.css" />
</head>
<body>
<%CxGame.Channel = "Spread"  %>
<!--#include file="Themes/Common_Header.asp" -->
<!--页面主体开始-->
<div class="main" style="margin-top:8px;">
	<!--左边部分开始-->
	<!--#include file="Themes/Common_Sidebar_A.asp" -->		
	<!--左边部分结束-->
	
	<!--右边部分开始-->
	<% 
	    ConnectGame(QPTreasureDB)
	    Dim RegisterGrantScore,PlayTimeCount,PlayTimeGrantScore,FillGrantRate,BalanceRate,MinBalanceScore
	    Set rs = GameConn.Execute("Select * from GlobalSpreadInfo(nolock)")
	    If rs.eof Then
	        RegisterGrantScore=0
	        PlayTimeCount=0
	        PlayTimeGrantScore=0
	        FillGrantRate=0
	        BalanceRate=0
	        MinBalanceScore=0
	    Else
	        RegisterGrantScore=rs(1)
	        PlayTimeCount=rs(2)
	        PlayTimeGrantScore=rs(3)
	        FillGrantRate=rs(4)
	        BalanceRate=rs(5)
	        MinBalanceScore=rs(6)
	    End If
	    rs.Close:Set rs=Nothing:CloseGame()
	%>
	<div class="mainRight1">
		<!--新闻公告开始-->
		<div class="newsBody">
		<div class="tgTitle"></div>
		  <div class="tgBg">
				<div class="tgTitle1 lan f14 bold">1、游戏推广员制度</div>
           		<div class="content">　　经推荐的玩家游戏时间达到<%=PlayTimeCount/3600 %>个小时，推荐人将获得<%=PlayTimeGrantScore %>金币的一次性奖励。
把推荐人填为您的游戏帐号，您将立即获得<%=RegisterGrantScore %>金币
</div>
				
				<div class="tgTitle1 lan f14 bold">2、推荐方式</div>
<div class="content"><span class="bold">方式一</span>：<br />
           		  （1）介绍新用户进http://qp6603.foxuc.net/ 下载游戏并安装。<br />
           		  <span class="bold">方式二</span>：<br />
           		  （1）在公共上网场所（如网吧），进http://qp6603.foxuc.net/下载游戏并安装。<br />
           		  （2）在安装游戏的目录下建立文件custom\introduce.txt，文件内容设置为：[config] introducer = 您的游戏帐号<br />
           		  在网吧上网的用户。
                  <br />
          举例：您把面对面视频游戏安装在D:\Program Files\ttmdm下，你的游戏帐号为aaaa 。 那么就先在ttmdm目录下建立custom
          <br />
          </div>
				
				<div class="tgTitle1 lan f14 bold">3、推广业绩</div>
<div class="content">（1）被推荐玩家游戏时间达到<%=PlayTimeCount/3600 %>个小时，您将获得<%=PlayTimeGrantScore %>金币的一次性奖励；
  <br />
  游戏时间：是指在各个游戏中,进行游戏的总时间,不包含挂机时间。游戏时间一旦达到<%=PlayTimeCount/3600 %>个小时，该玩家则成为有效用户。
<br />
           		  （2）被推荐玩家充值,每充值1元钱,您将获得被推荐玩家充值金币<%=CCur(FillGrantRate)*100 %>%；<br />
           		  （3）推荐玩家推广业绩的<%=CCur(BalanceRate)*100 %>%。当您所推荐的玩家，也推荐了其他人时,他（她）每次结算推广业绩时（将推广获得的金币转入游戏帐户中）,系统同时赠送您<%=CCur(BalanceRate)*100 %>%的金额，他的提成收入保持不变。这样,您间接推荐的玩家都有可能为您带来业绩。<br />
	        <span class="bold">例如：您推荐了玩家A，A又推荐了好多其他的玩家。当A结算业绩时，获得了2000万金币提成，您的业绩同时也增加<%=CCur(BalanceRate)*2000 %>万金币</span></div>
				
				<div class="tgTitle1 lan f14 bold">4、推广员业绩查询</div>
       		    <div class="content">
       		    推广员业绩查询和结算请登录会员中心
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
