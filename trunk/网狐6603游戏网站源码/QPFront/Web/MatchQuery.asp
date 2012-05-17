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
    <script src="JS/comm.js" type="text/javascript"></script>   
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
				<li><a href="MatchOrder.asp?id=<%=CxGame.GetInfo(1,"id") %>">比赛排名</a></li>
				<li class="current"><a href="MatchQuery.asp?id=<%=CxGame.GetInfo(1,"id") %>">得奖查询</a></li>
				</ul>
		  	</div>
			<div class="right"></div>
		</div>
	</div>
	
    <div class="contMain">
	<div class="top3"></div>
    <div class="mainBox">
        <script type="text/javascript">
            function CheckForm()
            {
                if(document.form1.txtAccounts.value.trim()=="")
                {
                    alert("内容不能为空！");
                    return false;
                }
            }
        </script>
	    <div class="mainBox1 f14 lineHeight25">
	      <form id="form1" name="form1" method="Post" action="" onsubmit="return CheckForm()">
	      <table width="68%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
              <td height="40" valign="top">请选择查询类型：
                <select name="sType">
                  <option value="0"<%If CxGame.GetInfo(1,"sType")=0 Then %> selected="selected" <%End If %>>按用户名查询</option>
                  <option value="1"<%If CxGame.GetInfo(1,"sType")=1 Then %> selected="selected" <%End If %>>按ID查询</option>
                </select> 
                <input name="txtAccounts" type="text" class="width200 text" value="<%=CxGame.GetInfo(0,"txtAccounts") %>" />
                <input type="submit" class="bnt lan bold" value="查询" />
                <input type="hidden" name="Query" value="true" />
              </td>
            </tr>
          </table>
          <% 
            If CxGame.GetInfo(0,"Query") = "true" Then
                Call ConnectGame(QPGameMatchDB)
                Dim rs,sql
                Set rs=Server.CreateObject("Adodb.RecordSet")
                sql = "select Top 20 ROW_NUMBER() OVER (ORDER BY Score DESC) AS RankedID,* from MatchScore where MatchID="&Request("id")
                If CxGame.GetInfo(1,"sType")=0 Then
                    sql = "select * from ("&sql&") a Where UserID= (select UserID from QPAccountsDBLink.QPAccountsDB.dbo.AccountsInfo where Accounts='"&CxGame.GetInfo(0,"txtAccounts")&"')"
                Else
                    sql = "select * from ("&sql&") a Where UserID= (select UserID from QPAccountsDBLink.QPAccountsDB.dbo.AccountsInfo where GameID="&CxGame.GetInfo(0,"txtAccounts")&")"
                End If
                rs.Open sql,GameConn,1,3
                If rs.EOF Then
                    response.Write "抱歉！您没有取得排名。"
                Else
                    response.Write "您的成绩是第 "&rs("RankedID")&" 名！"
                End If
            End If
          %>
          </form>
          <br />
              <br />
              <br />
              <br />
              <br />
              <br />
              <br />
              <br />
              <br />
              <br />
              <br />
              <br />
              <br />
              <br />
              <br />
              <br />
              <br />
              <br />
              <br />
              <br />  
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
