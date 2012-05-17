<!--#include file="Inc/CommonFun.asp" -->
<!--#include file="Inc/Conn.asp" -->
<!--#include file="Inc/Tool.asp" -->

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
<%CxGame.Channel = "News" %>
<!--#include file="Themes/Common_Header.asp" -->

<!--页面主体开始-->
<div class="main mtop10">
	<!--#include file="Themes/Common_Sidebar_A.asp" -->
	
	<!--右边部分开始-->
	<div class="mainRight1">
		<!--新闻公告开始-->
		<div class="newsBody">
			<div class="newsTitle bold hui3">新闻公告</div>
			<div class="newsBg2">
			    <%=CxGame.NewsList(20) %>            
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
