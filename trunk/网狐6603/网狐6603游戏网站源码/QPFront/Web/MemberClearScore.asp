<!--#include file="Inc/CommonFun.asp" -->
<!--#include file="Inc/AutoCheck.asp" -->
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
    <link rel="stylesheet" type="text/css" href="Css/user_layout.css" />
    <link rel="stylesheet" type="text/css" href="Css/game_layout.css" />    
</head>
<body>
<%
    CxGame.Channel = "Member" 
    CxGame.IsCheckLogin
%>
<!--#include file="Themes/Common_Header.asp" -->

<!--页面主体开始-->
<div class="userBody" style="margin-top:8px;">
	<div class="userTop"></div>
	<div class="userBg">
	    <!--左边页面开始-->
	    <div class="LeftSide">
			<!--会员登录开始-->
			<!--#include file="Themes/Common_Logon.asp" -->
			<!--会员登录结束-->
			<!--#include file="Themes/Member_Sidebar.asp" -->					
		</div>
		<!--左边页面结束-->
		
		<!--右边页面开始-->		
		<div class="mainRight2">
			<div class="userRigthBody">
				<div class="userRigthTitle">会员中心 - 会员服务 - 负分清零</div>
				<div class="userRigthBg">
                <!--负分清零开始-->
                <% 
                    Select case lcase(Request("action"))
                        case "clean"
				        Call ClearScore()				        
				        case else
				        Call Main()
                    End Select 
                    
                    '负分清零
                    Sub ClearScore()
                        Dim kindID,dname,rs,IP
                        kindID = CxGame.GetInfo(1,"KindID")
                        dname="QPGameScoreDB"
                        IF kindID<=0 Then
                            dname="QPGameScoreDB"
                        Else
                            dname = CxGame.GetKindDBNameByKindID(kindID)
                            If IsNull(dname) Or dname="" Then
                                dname="QPGameScoreDB"
                            End If
                        End If  
                        
                        IP = CxGame.GetIP
                        Call ConnectGame(dname)
                            Set rs = GameConn.Execute("WEB_PW_ResetGameScore "&Session("UserID")&",'"&IP&"'")
                            CxGame.MessageBoxReload rs(1),"MemberClearScore.asp"
                        Call CloseGame()            
                    End Sub
                    
                    Sub Main()
                        Dim ArrayList,i,rs
                        Dim str,j,htmlT
                        ArrayList = CxGame.GetScoreKindList
                        For i=0 To Ubound(ArrayList) 
                            If Not IsEmpty(ArrayList(i,0)) Then
                                ConnectGame(ArrayList(i,3))
                                    Set rs = GameConn.Execute("Select * from GameScoreInfo(nolock) where UserID = "&Session("UserID")&" And Score<0")
                                    If not rs.eof Then
                                        j=j+1
                                        str = str&"<tr align=""center"" class=""trBg1""><td>"&ArrayList(i,1)&"</td><td>"&rs("Score")&"</td>"
                                        str = str&"<td><a class=""ll"" href=""?action=clean&KindID="&ArrayList(i,0)&""">清零</a></td></tr>"
                                    End If
                                CloseGame()
                            End If
                        Next  
                        Set ArrayList = nothing
                        
                        If IsEmpty(str) Then
                            htmlT = "<div class=""loginHint""><div class=""Uright"">恭喜！您的游戏成绩保持的非常好，没有负分需要清零。</div></div>"
                        Else
                            htmlT = "<table width=""100%"" border=""0"" cellspacing=""0"" cellpadding=""0"">"&vbCrLf
                            htmlT = htmlT&"<tr><td><span class=""bold"" >"&Session("UserName")&"，</span>你有以下<span class=""bold"" >"&j&"</span>款游戏需要进行负分清零</td></tr>"&vbCrLf
                            htmlT = htmlT&"<tr><td>"&vbCrLf
                            htmlT = htmlT&"<table width=""100%"" border=""0"" cellpadding=""0"" cellspacing=""1"" bgcolor=""#BAD7E5"" class=""leftTop"">"&vbCrLf
                            htmlT = htmlT&"<tr align=""center"" class=""bold"">"&vbCrLf
                            htmlT = htmlT&"<td class=""leftTop trBg"">游戏名称</td><td class=""leftTop trBg"">积分</td><td class=""leftTop trBg"">&nbsp;</td>"&vbCrLf
                            htmlT = htmlT&"</tr>"&vbCrLf
                            htmlT = htmlT&str&vbCrLf
                            htmlT = htmlT&"</table>"&vbCrLf
                            htmlT = htmlT&"</td></tr>"&vbCrLf
                            htmlT = htmlT&"</table>"
                        End If
                %>
                <%
                    response.Write htmlT
                    End Sub
                %>
                <!--负分清零结束-->
                <div class="clear"></div>
		 	    </div>
				<div class="userRightBottom"><div class="clear"></div></div>
				<div class="clear"></div>
			</div>
			<div class="clear"></div>
		</div>
		<!--右边页面结束-->
		<div class="clear"></div>
	</div>
	<div class="userBottom"><div class="clear"></div></div>
<div class="clear"></div>
</div>
<!--页面主体结束-->
<!--#include file="Themes/Common_Footer.asp" -->
</body>
</html>
