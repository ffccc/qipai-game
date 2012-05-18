<!--#include file="Inc/CommonFun.asp" -->
<!--#include file="Inc/AutoCheck.asp" -->
<!--#include file="Inc/Conn.asp" -->
<!--#include file="Inc/Cls_Page.asp" -->

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
    <script src="JS/jquery_last.js" type="text/javascript"></script>   
    <script src="JS/formValidator.js" type="text/javascript"></script>
    <script src="JS/formValidatorRegex.js" type="text/javascript"></script>
    
    <script type="text/javascript">
        $(document).ready(function(){
            //页面验证
           $.formValidator.initConfig({formid:"form1",alertmessage:true,onerror:function(msg){alert(msg)}});
            $("#txtBalance").formValidator()
                .regexValidator({regexp:"intege1",datatype:"enum",onerror:"结算金额必须为整数！"});            
        })
    </script>
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
				<div class="userRigthTitle">会员中心 - 推广服务 - 业绩结算</div>
				<div class="userRigthBg">
				<!--兑换开始-->
				<% 
				    ConnectGame(QPTreasureDB)
                    Select case lcase(Request("action"))
                        case "convert"
				        Call Convert()				        
				        case else
				        Call Main()
                    End Select 
                    Call CloseGame()  
                    
                    '魅力兑换
                    Sub Convert()
                        Dim UserID,balance,IP,rs
                        UserID = Session("UserID")
                        balance = CxGame.GetInfo(0,"txtBalance")
                        IP = CxGame.GetIP
                        Set rs = GameConn.Execute("WSP_PW_SpreadBalance "&UserID&","&balance&",'"&IP&"'")
                        CxGame.MessageBoxReload rs(1),"MemberSpreadBalance.asp"
                    End Sub
				    
				    Sub Main()
				        Dim rs
		                Set rs = GameConn.Execute("WSP_PW_GetUserSpreadInfo "&Session("UserID")&"")
		                If rs(0) <> 0 Then
		                    CxGame.MessageBoxReload rs(1),"MemberIndex.asp"
		                Else
				%>
	            <div class="title f14 bold lan">业绩结算</div>
	            <br />
	            <table width="100%" border="0" align="center" cellpadding="0" cellspacing="1" bgcolor="#BAD7E5">
	                <tr>
	                <td height="139" background="images/contestBg1.jpg">
	                    <table width="96%" border="0" align="center" cellpadding="0" cellspacing="0" >
                          <tr>
                            <td width="25%" align="right" class="lineBottom">ID：</td>
                            <td width="27%" class="lineBottom"><%=rs("GameID") %></td>
                            <td width="17%" align="right" class="lineBottom">帐　号：</td>
                            <td width="31%" class="lineBottom"><%=rs("Accounts") %></td>
                          </tr>
                          <tr>
                            <td align="right" class="lineBottom">提成总收入：</td>
                            <td  class="lineBottom"><%=rs("SpreadIn") %></td>
                            <td align="right"  class="lineBottom">结算总支出：</td>
                            <td  class="lineBottom"><%=rs("SpreadOut") %></td>
                          </tr>
                          <tr>
                            <td align="right">当前推广余额：</td>
                            <td class="hong bold"><%=rs("SpreadBalance") %></td>
                            <td align="right">当前保险柜存款：</td>
                            <td class="hong bold"><%=rs("InsureScore") %></td>
                          </tr>
                        </table>
	                </td>
	                </tr>
	            </table>
	            <br />
	            <form id="form1" name="form1" method="post" action="?action=convert">
	            <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
                  <tr>
                    <td>
                    <table width="100%" border="0" cellspacing="0" cellpadding="0" class="loginHint">
                      <tr class="bold hui3">
                        <td width="28%" align="right">将</td>
                        <td width="26%" align="center"><input id="txtBalance" name="txtBalance" type="text" class="text" value="<%=rs("SpreadBalance") %>" /></td>
                        <td width="46%" align="left">推广金币结算到游戏账户</td>
                      </tr>
                    </table>
                    </td>
                  </tr>
                  <tr>
                    <td align="center"><input name="Submit" type="submit" class="bnt lan bold" value="结算" /></td>
                  </tr>
                </table>
                </form>
                <br />
                <div class="title f14 bold lan">温馨提示</div>
                <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
                   <tr>
                     <td width="10%" align="right"></td>
                     <td width="90%" class="hui6">
                        1、结算金币直接转入保险柜。<br>
                        2、有任何问题，请您联系客服中心。<br>
                     </td>
                   </tr>
                </table>     
                <% 
                    rs.Close:Set rs=Nothing
                    End If
                    End Sub
                %>
	            <!--兑换结束-->
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
