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
    <script src="JS/comm.js" type="text/javascript"></script>
    <style type="text/css">
    .btn{ background:url(images/KFbtn.gif) no-repeat; width:129px; height:46px; border:none;}
    body{ margin:0; padding:0; font-size:12px;}
     p { text-indent:24px; color:#646464;line-height:20px;}
    .text1{width:399px; height:25px;line-height:25px; padding-left:4px;border: 1px solid #93c3f3;overflow-y:hidden;}
    .text2{margin-top:4px; width:400px; height:200px; padding:4px 0 0 4px;border: 1px solid #93c3f3;overflow-y:hidden;}
    </style>
</head>
<body>
<!--页面主体开始-->
<% 
    Call ConnectGame(QPNativeWebDB)
    Select case lcase(Request("action"))	
        case "addinfo"
        Call AddInfo()		        
        case else
        Call Main()
    End Select 
    CloseGame()		
    
    '新增问题反馈
    Sub AddInfo()			            
        Dim accounts,title,contect,IP,rs
        accounts = Session("UserName")
        title = CxGame.GetInfo(0,"txtTitle")
        If title="" Then
            CxGame.MessageBoxReload "主题不能为空","Feedback.asp"
            Exit Sub
        End If
        contect = CxGame.GetInfo(0,"txtContent")
        If contect="" Then
            CxGame.MessageBoxReload "内容不能为空","Feedback.asp"
            Exit Sub
        End If
        IP=CxGame.GetIP()
        Set rs = GameConn.Execute("WSP_PW_AddGameFeedback '"&accounts&"','"&title&"','"&contect&"','"&IP&"'")
        CxGame.MessageBoxReload rs(1),"Feedback.asp"
    End Sub
    
    Sub Main()
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
<table width="500" border="0" cellpadding="0" cellspacing="1" bgcolor="#FFFFFF">
  <tr>
    <td width="500" height="398" valign="top" bgcolor="#FFFFFF" ><br />
      <table width="96%" border="0" align="center" cellpadding="0" cellspacing="0">
      <tr>
	  	<td width="12%" height="45" align="right">&nbsp;</td>
        <td height="45"><p>
谢谢您对我们的游戏提出宝贵的意见和建议，我们将用心聆听您的建议，游戏全体人员将竭
尽全力地为您打造出一个开心快乐的游戏乐园，愿您游戏愉快。
</p></td>
        </tr>
      <tr>
        <td width="12%" height="45" align="right">主题：</td>
        <td width="88%" height="45"><input type="text" name="txtTitle" class="text1" /></td>
      </tr>
      <tr>
        <td align="right">内容：</td>
        <td><textarea name="txtContent" class="text2"></textarea></td>
      </tr>
      <tr>
        <td height="68">&nbsp;</td>
        <td align="center"><input type="submit" class="btn" value=""/></td>
      </tr>
    </table>
    </td>
  </tr>
</table>
</form>
<% 
    End Sub
%>
</body>
</html>
