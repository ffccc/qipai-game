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
<%CxGame.Channel = "Match"  %>
<!--#include file="Themes/Common_Header.asp" -->

<!--页面主体开始-->
<div class="main" style="margin-top:8px;">
	<!--左边部分开始-->
	<!--#include file="Themes/Common_Sidebar_A.asp" -->
	<!--左边部分结束-->
	
	<!--右边部分开始-->
	<div class="mainRight1">
		<!--赛事列表开始-->		
		<div class="newsBody">
		<div class="left"><img src="images/match.gif" /></div>
		<% 
		    Call ConnectGame(QPNativeWebDB)
		    Dim rs,i
		    Set rs=GameConn.Execute("Select Top 20 MatchID,MatchTitle,MatchSummary from GameMatchInfo Where Nullity=0 Order By MatchID Desc")
		    If rs.Eof Then
		        Response.Write("<div class=""tgBg"">抱歉，该游戏帮助还没有添加。</div>")
	        Else
		%>
	    <div class="tgBg">
            <% 
                Do While Not rs.Eof	        
                    Response.Write("<div class=""tgTitle1"" ><a href=""MatchView.asp?id="&rs("MatchID")&""" class=""lh lan f14 bold"">"&rs("MatchTitle")&"</a></div>"&vbCrLf)
                    Response.Write("<div class=""content hui6 mbottom10"">"&rs("MatchSummary")&"</div>"&vbCrLf)
                rs.MoveNext
		        Loop
            %>		    
	    </div>
	    <% 
		    End If
		    rs.close:set rs=nothing
	        CloseGame()		     
		%>
		<div class="clear"></div>
		<div class="newsBottom"><div class="clear"></div></div>
		<div class="clear"></div>
		</div>		
		<!--赛事列表结束-->
	<div class="clear"></div>
 	</div>
	<!--右边部分结束-->
<div class="clear"></div>
</div>
<!--页面主体结束-->
<!--#include file="Themes/Common_Footer.asp" -->
</body>
</html>
