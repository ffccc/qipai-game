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
    <link rel="stylesheet" type="text/css" href="css/contest_layout.css" />
</head>
<body>
<%CxGame.Channel = "Match"  %>
<!--#include file="Themes/Common_Header.asp" -->

<!--页面主体开始-->
<div class="main" style="margin-top:8px;">
	<div class="contTop">
		<div class="nav">
			<div class="left"></div>
			<div class="center">
				<ul>
				<li class="current"><a href="MatchView.asp?id=<%=CxGame.GetInfo(1,"id") %>">赛事公告</a></li>
				<li><a href="MatchApply.asp?id=<%=CxGame.GetInfo(1,"id") %>">比赛报名</a></li>
				<li><a href="MatchOrder.asp?id=<%=CxGame.GetInfo(1,"id") %>">比赛排名</a></li>
				<li><a href="MatchQuery.asp?id=<%=CxGame.GetInfo(1,"id") %>">得奖查询</a></li>
				</ul>
		  	</div>
			<div class="right"></div>
		</div>
	</div>
	
	<% 
	    Call ConnectGame(QPNativeWebDB)
	    Dim rs,i
	    Set rs=GameConn.Execute("Select MatchID,MatchContent from GameMatchInfo(nolock) Where MatchID="&CxGame.GetInfo(1,"id")&" And Nullity=0")
	    If rs.Eof Then
	        CxGame.MessageBoxReload "比赛信息不存在！","MatchIndex.asp"
	    Else
	%>
    <div class="contMain">
	<div class="top"></div>
    <div class="mainBox">
	    <div class="mainBox1 f14 lineHeight25">
	    <%=rs("MatchContent") %>
		<div align="center" class="mtop25"><a href="MatchApply.asp?id=<%=CxGame.GetInfo(1,"id") %>" hidefocus="true"><img src="images/bm.gif" border=0 /></a></div>
	    </div>
    </div>
	<div class="bottom"></div>
    </div>
    <% 
        End If
        rs.close:set rs=nothing
        CloseGame()	
    %>
<div class="clear"></div>
</div>
<!--页面主体结束-->
<!--#include file="Themes/Common_Footer.asp" -->
</body>
</html>
