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
    <link rel="stylesheet" type="text/css" href="css/game_layout.css" />
    <link rel="stylesheet" type="text/css" href="css/cz_layout.css" />
</head>
<body>
<%CxGame.Channel = "Index"  %>
<!--#include file="Themes/Common_Header.asp" -->
<!--页面主体开始-->
<div class="main" style="margin-top:8px;">
	<!--左边部分开始-->
	<div class="mainLeft1">
	<!--#include file="Themes/Game_Sidebar.asp" -->
	</div>
	<!--左边部分结束-->
	
	<!--右边部分开始-->	
	<div class="mainRight1">	
	<% 
        Call ConnectGame(QPNativeWebDB)
        Dim rs
        Set rs=GameConn.Execute("Select * from GameRulesInfo Where KindID="&Request("KindID"))
        If rs.eof Then
            Response.Write("抱歉，该游戏帮助还没有添加。")
        Else
            Dim KindName,ImgRuleUrl,HelpIntro,HelpRule,HelpGrade
            KindName=rs("KindName")
            ImgRuleUrl=rs("ImgRuleUrl")
            HelpIntro=rs("HelpIntro")
            HelpRule=rs("HelpRule")
            HelpGrade=rs("HelpGrade") 	           	           
    %>	
		<!--游戏介绍内容开始-->		
		<div class="recharge">		
		    <div class="gameTitle hui3">您的位置：首页 - 游戏介绍 - <%=Trim(KindName) %>游戏规则</div>
			<div class="gameInfoBody">			
				<!--游戏截图开始-->
				<div class="gameInfo">					
					<div class="err mtop13">游戏截图</div>
					<div class="gameImg"><img src="<%=Replace(ImgRuleUrl,"/Upload",CxGame.ManagerDomain&"/Upload") %>" width="431" height="295" /></div>
					<!--游戏积分排行开始-->
					<!--<div class="gameTop">
						<div class="title"></div>
						<div class="topList">
							<ul>
							<li><span>10分</span><label class="ordinal1">1</label>测试帐号</li>
							<li><span>10分</span><label class="ordinal1">2</label>测试帐号</li>
							<li><span>10分</span><label class="ordinal1">3</label>测试帐号</li>
							<li><span>10分</span><label class="ordinal2">4</label>测试帐号</li>
							<li><span>10分</span><label class="ordinal2">5</label>测试帐号</li>
							<li><span>10分</span><label class="ordinal2">6</label>测试帐号</li>
							<li><span>10分</span><label class="ordinal2">7</label>测试帐号</li>
							<li><span>10分</span><label class="ordinal2">8</label>测试帐号</li>
							<li><span>10分</span><label class="ordinal2">9</label>测试帐号</li>
							<li><span>10分</span><label class="ordinal2">10</label>测试帐号</li>
							</ul>
						<div class="clear"></div>
						</div>
					<div class="clear"></div>
					</div>-->
					<!--游戏积分排行开始-->
				<div class="clear"></div>
				</div>
				<!--游戏截图开始-->
				
				<!--游戏介绍开始-->
				<div class="gameInfo">					
					<div class="err mtop13">游戏介绍</div>
					<div class="gameInfo mtop8 lineHeight22"><%=HelpIntro %></div>
				<div class="clear"></div>
				</div>
				<!--游戏介绍开始-->
				
				<!--规则介绍开始-->
				<div class="gameInfo">					
					<div class="err mtop13">规则介绍</div>
					<div class="gameInfo mtop8 lineHeight22">
					  <%=HelpRule %>
					</div>
				<div class="clear"></div>
				</div>
				<!--规则介绍开始-->
				
				<!--积分等级开始-->
				<div class="gameInfo">					
				<div class="err mtop13">积分等级</div>
				  <div class="gameInfo mtop8 lineHeight22">
				    <%=HelpGrade %>
				  </div>
				<div class="clear"></div>
				</div>
				<!--积分等级开始-->
                <br />
			<div class="clear"></div>
	 	</div>	 	
		<div class="clear"></div>
		<div class="recharge2"><div class="clear"></div></div>
		<div class="clear"></div>
		</div>			
		<!--游戏介绍内容结束-->	
		<% 
 	        End If
            rs.close:set rs=nothing
            CloseGame()	
 	    %>
	<div class="clear"></div>
	</div>
	<!--右边部分结束-->		
<div class="clear"></div>
</div>
<!--页面主体结束-->
<!--#include file="Themes/Common_Footer.asp" -->
</body>
</html>
