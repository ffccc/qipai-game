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
    <link rel="stylesheet" type="text/css" href="css/news_layout.css" />
</head>
<body>
<%CxGame.Channel = "Service"  %>
<!--#include file="Themes/Common_Header.asp" -->
<!--新闻内容开始-->
<% 
    '获取上一篇ID
    Function GetPreIssueID()
        Dim rs,rValue
        Set rs=GameConn.Execute("Select Top 1 IssueID,IssueTitle from GameIssueInfo Where IssueID<"& Request("id") &" order by IssueID desc")
        If Not rs.Eof Then
            rValue = "<a title=""上一篇问题："&rs("IssueTitle")&""" href=""ServiceIssueView.asp?id="&rs("IssueID")&""">上一篇</a>"
        Else
            rValue = "<a disabled=""disabled"" title=""这是最后一篇问题"">上一篇</a>"
        End If
        GetPreIssueID = rValue
    End Function
    
    '获取下一篇ID
    Function GetNextIssueID()
        Dim rs,rValue
        Set rs=GameConn.Execute("Select Top 1 IssueID,IssueTitle from GameIssueInfo Where IssueID>"& Request("id") &" order by IssueID asc")
        If Not rs.Eof Then
            rValue = "<a title=""下一篇问题："&rs("IssueTitle")&""" href=""ServiceIssueView.asp?id="&rs("IssueID")&""">下一篇</a>"
        Else
            rValue = "<a disabled=""disabled"" title=""这是最新一篇问题"">下一篇</a>"
        End If
        GetNextIssueID = rValue
    End Function
    
    ConnectGame(QPNativeWebDB)
    Dim rs
    Set rs=GameConn.Execute("Select * from GameIssueInfo(nolock) Where IssueID="&Request("id"))
    If Not rs.Eof Then
%>
<div class="main">
    <div class="viewTop">您的位置：首页>>客服中心>>常见问题</div>
    <div class="page"><%=GetPreIssueID() %><%=GetNextIssueID() %><a href="ServiceIssueView.asp" title="返回列表">返回列表</a></div>
	<div class="viewBody">		
		<div class="viewTitle f16 bold" style="border-bottom:dashed 1px #ccc;"><%=rs("IssueTitle") %></div>
		<div class="ViewContent">		　　
		    <%=rs("IssueContent") %>
		<div class="clear"></div>
	    </div>
    </div>
    <div class="page"><%=GetPreIssueID() %><%=GetNextIssueID() %><a href="ServiceIssueView.asp" title="返回列表">返回列表</a></div>
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
