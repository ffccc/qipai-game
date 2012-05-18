<!--#include file="Inc/CommonFun.asp" -->
<!--#include file="Inc/AutoCheck.asp" -->
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
    <link rel="stylesheet" type="text/css" href="css/customer_layout.css" />
    <script src="JS/comm.js" type="text/javascript"></script>
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
				<div><a href="ServiceIssue.asp" class="cjwt" hidefocus="true"></a></div>
				<div class="wtfk"></div>
				<div><a href="ServiceCustomer.asp" class="kfdh" hidefocus="true"></a></div>
				<div class="cLeftBottom"></div>
				<div class="clear"></div>
			</div>			
			<div class="cRight"><br />
			    <% 
			        Call ConnectGame(QPNativeWebDB)
			        Select case lcase(Request("action"))
			            case "getinfo"
			            Call Main()
			            Call GetFeedbackInfo()		
			            case "addinfo"
			            Call AddInfo()		        
			            case else
			            Call Main()
			        End Select 
			        CloseGame()		
			        
			        '获取回复状态
			        Function GetFeedbackStatus(info)
			            If IsNull(info) Then
			                GetFeedbackStatus="未解决"
			            Else
			                GetFeedbackStatus="已解决"
			            End If
			        End Function 
			        
			        '获取用户信息
			        Function GetAccountsInfo(info)
			            If info="" Then
			                GetAccountsInfo="匿名用户"
			            Else
			                GetAccountsInfo=info
			            End If
			        End Function 
			        
			        '新增问题反馈
			        Sub AddInfo()			            
			            Dim accounts,title,contect,IP,rs
			            accounts = CxGame.GetInfo(0,"txtAccounts")
			            title = CxGame.GetInfo(0,"txtTitle")
			            If title="" Then
			                CxGame.MessageBoxReload "主题不能为空","ServiceFeedback.asp"
			                Exit Sub
			            End If
			            contect = CxGame.GetInfo(0,"txtContent")
			            If contect="" Then
			                CxGame.MessageBoxReload "内容不能为空","ServiceFeedback.asp"
			                Exit Sub
			            End If
			            IP=CxGame.GetIP()
			            Set rs = GameConn.Execute("WSP_PW_AddGameFeedback '"&accounts&"','"&title&"','"&contect&"','"&IP&"'")
			            CxGame.MessageBoxReload rs(1),"ServiceFeedback.asp"
			        End Sub
    			    
			        Sub Main()
			            Dim rs,Result_n,pageSize
			            pageSize=10'每页显示数据
			            Set Rs=Server.Createobject("Adodb.Recordset")
			            rs.Open "Select FeedbackID,FeedbackTitle,Accounts,FeedbackDate,ViewCount,RevertDate from GameFeedbackInfo where Nullity=0 order by FeedbackID desc",GameConn,1,1
			            rs.PageSize = pageSize
                        Result_n = rs.RecordCount                        
                        Dim Maxpage,Page,i,classInfo
                        If Result_n>0 Then
                            Maxpage = rs.PageCount
                            Page = Request("Page")
                            If Not IsNumeric(Page) or Page="" then
				                Page=1
			                Else
				                Page=CInt(Page)
			                End if
			                If page<1 then
				                page=1
			                Elseif page>maxpage then
				                page=maxpage
			                End if	
			                rs.AbsolutePage=Page
	                    Else
			                rs.Close:Set rs=Nothing
			                Response.Write "没有任何反馈信息!"
			                Exit Sub
		                End If 
		                Response.Write("<table width=""100%"" border=""0"" cellpadding=""0"" cellspacing=""0"">"&vbCrLf)
			            Response.Write("<tr align=""center""><td class=""line"">提交者</td><td class=""line"">留言主题</td><td class=""line"">提交时间</td><td class=""line"">回复</td><td class=""line"">人气</td></tr>"&vbCrLf)
			            For i=0 To pageSize-1   
			                If i Mod 2 =0 Then
			                    classInfo = "align=""center"""  
			                Else
			                    classInfo = "class=""tdBg"" align=""center"""    
			                End If
			                response.Write("<tr "&classInfo&">")
			                response.Write("<td>"&GetAccountsInfo(rs("Accounts"))&"</td>"&vbCrLf)       
			                response.Write("<td><a href=""?action=getinfo&id="&rs("FeedbackID")&""" class=""lh"">"&rs("FeedbackTitle")&"</a></td>"&vbCrLf)             
			                response.Write("<td>"&Tool.Format_Time(rs("FeedbackDate"),2)&"</td>"&vbCrLf)            
			                response.Write("<td>"&GetFeedbackStatus(rs("RevertDate"))&"</td>"&vbCrLf)        
			                response.Write("<td>"&rs("ViewCount")&"</td>"&vbCrLf)  
			                response.Write("</tr>")                         
                            rs.MoveNext
                            If rs.Eof Then Exit For                            
                        Next
                        rs.Close:Set rs=Nothing
                        response.Write("</table>")
                        CxGame.GoPage maxpage,page
                    End Sub
			    %>	
			    <script type="text/javascript">
			        function CheckForm()
			        {
			            if(document.form1.txtTitle.value.trim()=="")
			            {
			                alert("主题不能为空！");
			                return false;
			            }
			            
			            if(document.form1.txtContent.value.trim()=="")
			            {
			                alert("内容不能为空！");
			                return false;
			            }
			        }
			    </script>	
			    <form id="form1" name="form1" method="post" action="?action=addinfo" onsubmit="return CheckForm()">			    
            	<table width="100%" border="0" cellpadding="0" cellspacing="0" bgcolor="#eef2f9" class="mtop13">
                  <tr class="height">
                    <td width="23%" height="65" align="right">用户名：</td>
                    <td width="77%">
                        <input type="text" name="txtAccounts" class="width206" value="<%=Session("UserName") %>" />&nbsp;
                        <font class="hui6">用户名为空为匿名用户</font>
                    </td>
                  </tr>
                  <tr style="padding-bottom:15px;">
                    <td align="right">主题：</td>
                    <td><input type="text" name="txtTitle" style="width:400px;"/></td>
                  </tr>
                  <tr>
                    <td align="right">内容：</td>
                    <td><textarea name="txtContent" rows="10" style="width:400px;"></textarea></td>
                  </tr>
                  <tr>
                    <td class="height">&nbsp;</td>
                    <td><input type="submit" name="Submit" value="发表留言" class="btn" /></td>
                  </tr>
                </table>
                </form>
                <%                    
                    Sub GetFeedbackInfo()
                        Dim rs
                        Set rs=GameConn.Execute("Select * from GameFeedbackInfo(nolock) Where FeedbackID="&Request("id"))
                        If not rs.EOF Then
                        GameConn.Execute("Update GameFeedbackInfo Set ViewCount=ViewCount+1 Where FeedbackID="&Request("id"))
                %>
            	<table width="100%" border="0" cellpadding="0" cellspacing="1" bgcolor="#C4CFD5" class="padding15">
                  <tr>
                    <td width="27%" align="right" bgcolor="#FFFFFF">留言者：</td>
                    <td width="73%" bgcolor="#FFFFFF"><%=GetAccountsInfo(rs("Accounts")) %></td>
                  </tr>
                  <tr>
                    <td align="right" bgcolor="#FFFFFF">留言内容：</td>
                    <td bgcolor="#FFFFFF"><%=rs("FeedbackContent") %></td>
                  </tr>
                  <% 
                       If Not IsNull(rs("RevertDate")) Then
                  %>
                  <tr>
                    <td align="right" bgcolor="#F7F9FD" class="">管理员回复：</td>
                    <td bgcolor="#F7F9FD">
                    <%=rs("RevertContent") %>
                    </td>
                  </tr>
                  <% 
                      End If
                  %>
                </table>
                <% 
                        End If
                    End Sub
                %>
            	<br />
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
