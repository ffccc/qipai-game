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
<% 
    '获取新闻来源
    Function GetLinkUrl(linkUrl)
        Dim rValue
        If linkUrl<>"" Then
            rValue = linkUrl
        Else
            rValue = CxGame.WebTitle
        End If
        GetLinkUrl = rValue
    End Function
    
    '获取上一篇新闻ID
    Function GetPreNewsID()
        Dim rs,rValue
        Set rs=GameConn.Execute("Select Top 1 NewsID,Subject from News Where NewsID<"& Request("id") &" order by NewsID desc")
        If Not rs.Eof Then
            rValue = "<a title=""上一篇新闻："&rs("Subject")&""" href=""NewsView.asp?id="&rs("NewsID")&""">上一篇</a>"
        Else
            rValue = "<a disabled=""disabled"" title=""这是最后一篇新闻"">上一篇</a>"
        End If
        GetPreNewsID = rValue
    End Function
    
    '获取下一篇新闻ID
    Function GetNextNewsID()
        Dim rs,rValue
        Set rs=GameConn.Execute("Select Top 1 NewsID,Subject from News Where NewsID>"& Request("id") &" order by NewsID asc")
        If Not rs.Eof Then
            rValue = "<a title=""下一篇新闻："&rs("Subject")&""" href=""NewsView.asp?id="&rs("NewsID")&""">下一篇</a>"
        Else
            rValue = "<a disabled=""disabled"" title=""这是最新一篇新闻"">下一篇</a>"
        End If
        GetNextNewsID = rValue
    End Function
    
    ConnectGame(QPNativeWebDB)
    Dim rs
    Set rs=GameConn.Execute("Select NewsID,Subject,LinkUrl,Body,ClassID,IssueDate from News Where NewsID="& Request("id") &"")
    If Not rs.Eof Then
%>
<!--新闻内容开始-->
<div class="main">
    <div class="viewTop">您的位置：首页>>新闻动态>>正文</div>
	<div class="page"><%=GetPreNewsID() %><%=GetNextNewsID() %><a href="NewsList.asp" title="返回新闻列表">返回新闻列表</a></div>
	<div class="viewBody">		
		<div class="viewTitle f16 bold" style="border-bottom:dashed 1px #ccc;"><%=rs("Subject") %></div>
		<div class="viewTitle hui6">类别：<%=CxGame.GetNewsTypeName(rs("ClassID")) %> 来源：<%=GetLinkUrl(rs("LinkUrl")) %> 发布于：<%=rs("IssueDate") %></div>
		<div class="ViewContent">		　　
		    <%=Replace(rs("Body"),"/Upload/News",CxGame.ManagerDomain&"/Upload/News") %>
		    <div style="font: 0px/0px sans-serif;clear: both;display: block;"> </div>
		<div class="clear"></div>
	    </div>
    </div>
    <div class="page"><%=GetPreNewsID() %><%=GetNextNewsID() %><a href="NewsList.asp" title="返回新闻列表">返回新闻列表</a></div>
    <div class="clear"></div>    
</div>
<!--新闻内容结束-->

<% 
    End If
    rs.Close:Set rs=Nothing
    CloseGame()
%>
<!--#include file="Themes/Common_Footer.asp" -->
</body>
</html>
