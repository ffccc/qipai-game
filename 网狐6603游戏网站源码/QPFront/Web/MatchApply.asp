<!--#include file="Inc/CommonFun.asp" -->
<!--#include file="Inc/Conn.asp" -->
<!--#include file="Inc/md5.asp" -->

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
    <link rel="stylesheet" type="text/css" href="css/news_layout.css" />
    <script src="JS/jquery_last.js" type="text/javascript"></script>
    <script src="JS/formValidator.js" type="text/javascript"></script>
    <script src="JS/formValidatorRegex.js" type="text/javascript"></script>
    
    <script type="text/javascript">
        $(document).ready(function(){
            //页面验证
            $.formValidator.initConfig({formid:"form1",alertmessage:true,onerror:function(msg){alert(msg)}});
            $("#txtAccounts").formValidator().inputValidator({min:1,empty:{leftempty:false,rightempty:false,emptyerror:"两边不能有空符号！"},onerror:"游戏帐号不能为空！"})
            $("#txtPwd").formValidator().inputValidator({min:1,empty:{leftempty:false,rightempty:false,emptyerror:"两边不能有空符号！"},onerror:"游戏密码不能为空！"})
            $("#txtCompellation").formValidator().inputValidator({min:1,empty:{leftempty:false,rightempty:false,emptyerror:"两边不能有空符号！"},onerror:"真实姓名不能为空！"})
            $("#txtPassportID").formValidator().inputValidator({min:1,empty:{leftempty:false,rightempty:false,emptyerror:"两边不能有空符号！"},onerror:"身份证号不能为空！"})
            $("#txtMobilePhone").formValidator().inputValidator({min:1,empty:{leftempty:false,rightempty:false,emptyerror:"两边不能有空符号！"},onerror:"联系电话不能为空！"})
            $("#txtEMail").formValidator().inputValidator({min:1,empty:{leftempty:false,rightempty:false,emptyerror:"两边不能有空符号！"},onerror:"电子邮件不能为空！"})
            $("#txtQQ").formValidator().inputValidator({min:1,empty:{leftempty:false,rightempty:false,emptyerror:"两边不能有空符号！"},onerror:"QQ不能为空！"})
            $("#txtDwellingPlace").formValidator().inputValidator({min:1,empty:{leftempty:false,rightempty:false,emptyerror:"两边不能有空符号！"},onerror:"详细地址不能为空！"})
            $("#txtPostalCode").formValidator().inputValidator({min:1,empty:{leftempty:false,rightempty:false,emptyerror:"两边不能有空符号！"},onerror:"邮编不能为空！"})
        })
    </script>
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
				<li class="current"><a href="MatchApply.asp?id=<%=CxGame.GetInfo(1,"id") %>">比赛报名</a></li>
				<li><a href="MatchOrder.asp?id=<%=CxGame.GetInfo(1,"id") %>">比赛排名</a></li>
				<li><a href="MatchQuery.asp?id=<%=CxGame.GetInfo(1,"id") %>">得奖查询</a></li>
				</ul>
		  	</div>
			<div class="right"></div>
		</div>
	</div>
	
	<% 
	    Call ConnectGame(QPNativeWebDB)
	    Dim rs,info
	    Set rs=GameConn.Execute("Select * from GameMatchInfo(nolock) Where MatchID="&CxGame.GetInfo(1,"id")&" And Nullity=0")
	    If rs.Eof Then
	        CxGame.MessageBoxReload "比赛信息不存在！","MatchIndex.asp"
	    Else
	        If Now()<rs("ApplyBeginDate") Then
	            info="报名未开始！"
	        End If
	        If Now()>rs("ApplyEndDate") Then
	            info="报名已结束！"
	        End If	
	%>
    <div class="contMain">
	<div class="top1"></div>
    <div class="mainBox">
	    <div class="mainBox1 f14 lineHeight25">
	        <% 
	            If info<>"" Then
	                Response.Write info
	            Else
	        %>
	        <form id="form1" name="form1" method="post" action="">
	   		<table width="59%" border="0" align="center" cellpadding="0" cellspacing="0">
              <tr>
                <td height="35" align="right">游戏帐号： </td>
                <td height="35"><input id="txtAccounts" name="txtAccounts" type="text" class="text width200" /><span class="hong f14">&nbsp;*</span></td>
              </tr>
              <tr>
                <td height="35" align="right">游戏密码： </td>
                <td height="35"><input id="txtPwd" name="txtPwd" type="password" class="text width200" /><span class="hong f14">&nbsp;*</span></td>
              </tr>
              <tr>
                <td height="35" align="right">真实姓名： </td>
                <td height="35"><input id="txtCompellation" name="txtCompellation" type="text" class="text width200" /><span class="hong f14">&nbsp;*</span></td>
              </tr>
              <tr>
                <td height="35" align="right">性别： </td>
                <td height="35">
                    <input type="radio" name="radGender" value="1" checked="checked" />男                    
                    <input type="radio" name="radGender" value="2"/>女                    
                </td>
              </tr>              
              <tr>
                <td height="35" align="right">身份证号：</td>
                <td height="35"><input type="text" id="txtPassportID" name="txtPassportID"  class="text width200" /><span class="hong f14">&nbsp;*</span></td>
              </tr>              
              <tr>
                <td height="35" align="right">联系电话：</td>
                <td height="35"><input type="text" id="txtMobilePhone" name="txtMobilePhone" class="text width200" /><span class="hong f14">&nbsp;*</span></td>
              </tr>
              <tr>
                <td height="35" align="right">电子邮件：</td>
                <td height="35"><input type="text" id="txtEMail" name="txtEMail" class="text width200" /><span class="hong f14">&nbsp;*</span></td>
              </tr>   
              <tr>
                <td height="35" align="right">QQ：</td>
                <td height="35"><input type="text" id="txtQQ" name="txtQQ"  class="text width200" /><span class="hong f14">&nbsp;*</span></td>
              </tr>           
              <tr>
                <td height="35" align="right">详细地址：</td>
                <td height="35"><input type="text" id="txtDwellingPlace" name="txtDwellingPlace" class="text width200" /><span class="hong f14">&nbsp;*</span></td>
              </tr>
              <tr>
                <td height="35" align="right">邮编：</td>
                <td height="35"><input type="text" id="txtPostalCode" name="txtPostalCode" class="text width200" /><span class="hong f14">&nbsp;*</span></td>
              </tr>              
              <tr>
                <td height="15"></td>
                <td height="15"></td>
              </tr>
              <tr>
                <td height="35">&nbsp;</td>
                <td height="35">
                    <input type="submit" name="Submit" value="提 交" class="btn1" />&nbsp;&nbsp;
                    <input type="reset" name="Submit2" value="重 置" class="btn1" />
                    <input type="hidden" name="Apply" value="true" />
                </td>
              </tr>
            </table>
            <% CxGame.GameMatchApply %>
            </form>
            <% 
                End If
            %>
  	    </div>  	    
  	</div>
	<div class="bottom"></div>
    </div>
    <% 
        End If
        rs.close:set rs=nothing
        CloseGame()	
    %>
<div class="clear"></div>
</div>
<!--页面主体结束-->
<!--#include file="Themes/Common_Footer.asp" -->
</body>
</html>
