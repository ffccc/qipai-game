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
    <link rel="stylesheet" type="text/css" href="css/customer_layout.css" />
</head>
<body>
<%CxGame.Channel = "Service"  %>
<!--#include file="Themes/Common_Header.asp" -->
<!--页面主体开始-->
<div class="main">
	<div class="customerBody">
		<div class="customerTitle"></div>
		<div class="customerBg">
			<div class="cLeft">
				<div class="cLeftTop"></div>
				<div><a href="ServiceIndex.asp" class="xsbz" hidefocus="true"></a></div>
				<div class="cjwt"></div>
				<div><a href="ServiceFeedback.asp" class="wtfk" hidefocus="true"></a></div>
				<div><a href="ServiceCustomer.asp" class="kfdh" hidefocus="true"></a></div>
				<div class="cLeftBottom"></div>
				<div class="clear"></div>
			</div>
			
			<div class="cRight">
				<%=CxGame.IssueList(20) %>
            	<div class="clear"></div>
		  	</div>
			<div class="clear"></div>
		</div>
		<div class="customerBottom"></div>
		<div class="clear"></div>
	</div>
	<div class="clear"></div>
</div>
<!--页面主体结束-->
<!--#include file="Themes/Common_Footer.asp" -->
</body>
</html>
