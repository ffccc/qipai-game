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
    <link rel="stylesheet" type="text/css" href="Css/user_layout.css" /> 
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
				<li><a href="MatchView.asp?id=<%=CxGame.GetInfo(1,"id") %>">赛事公告</a></li>
				<li><a href="MatchApply.asp?id=<%=CxGame.GetInfo(1,"id") %>">比赛报名</a></li>
				<li class="current"><a href="MatchOrder.asp?id=<%=CxGame.GetInfo(1,"id") %>">比赛排名</a></li>
				<li><a href="MatchQuery.asp?id=<%=CxGame.GetInfo(1,"id") %>">得奖查询</a></li>
				</ul>
		  	</div>
			<div class="right"></div>
		</div>
	</div>
	
    <div class="contMain">
	<div class="top2"></div>
    <div class="mainBox">
      <div class="mainBox1 f14">
        <% 
            Call ConnectGame(QPGameMatchDB)
            Dim rs,i,className
            Set rs=GameConn.Execute("Select Top 20 ROW_NUMBER() OVER (ORDER BY Score DESC) AS RankedID,* from MatchScore Where MatchID="&Request("id"))
            If rs.Eof Then
                Response.Write("没有排名信息！")
            Else
        %>
            <table width="100%" border="0" cellpadding="0" cellspacing="1" bgcolor="#C5E7FD">
                <tr align="center" class="bold">
                    <td height="35" class="leftTop trBg">排名</td>
                    <td class="leftTop trBg">帐号</td>
                    <td class="leftTop trBg">GameID</td>
                    <td class="leftTop trBg">积分</td>
                    <td class="leftTop trBg">胜局</td>
                    <td class="leftTop trBg">败局</td>     
                    <td class="leftTop trBg">平局</td>     
                    <td class="leftTop trBg">逃局</td>                       
                </tr>   
                <% 
                    Do While Not rs.Eof    
                    i=i+1
                    If i Mod 2 = 0 Then
                        className="trBg1"        
                    Else
                        className="trBg2"      
                    End If         
                %>
                <tr align="center" class="<%=className %>">
                    <td height="35"><%=rs("RankedID") %></td>
                    <td><%=CxGame.GetUserNameByUserID(rs("UserID")) %></td>
                    <td><%=CxGame.GetUserGameIDByUserID(rs("UserID")) %></td>
                    <td><%=rs("Score") %></td>
                    <td><%=rs("WinCount") %></td>
                    <td><%=rs("LostCount") %></td>
                    <td><%=rs("DrawCount") %></td>
                    <td><%=rs("FleeCount") %></td>
                </tr>
                <% 
                    rs.MoveNext
		            Loop
                %>
            </table>
        <% 
            End If
            rs.close:set rs=nothing
            CloseGame()	
        %>        
      </div>
  	</div>
	<div class="bottom"></div>
</div>
<div class="clear"></div>
</div>
<!--页面主体结束-->
<!--#include file="Themes/Common_Footer.asp" -->
</body>
</html>
