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
    <link rel="stylesheet" type="text/css" href="css/cz_layout.css" />
</head>
<body>
<%CxGame.Channel = "Pay"  %>
<!--#include file="Themes/Common_Header.asp" -->
<!--页面主体开始-->
<div class="main" style="margin-top:8px;">
	<!--左边部分开始-->
	<!--#include file="Themes/Common_Sidebar_A.asp" -->
	<!--左边部分结束-->
	
	<!--右边部分开始-->
	<div class="mainRight1">
		<!--网银充值开始-->
		<% 
		    ConnectGame(QPTreasureDB)
		    Dim orderID,operUserID,shareID,accounts,cardTypeID,cardTotal,IP,orderPre,orderLen,rs
		    operUserID = Session("UserID")
		    If IsEmpty(operUserID) Then
                operUserID=0
            End If
            shareID = CxGame.GetInfo(1,"PayType")
            accounts = CxGame.GetInfo(0,"txtAccounts")
            cardTypeID = CxGame.GetInfo(1,"CardType")
            cardTotal = 1
            IP = CxGame.GetIP()
            If shareID=2 Then
                orderPre = "KQ"
            Elseif shareID=3 Then
                orderPre = "YB"
            Else
                orderPre=""
            End If
            '订单长度
            orderLen = 20
            orderID = Tool.CreateOrderNum(orderPre,orderLen)
            Set rs= GameConn.Execute("WSP_PW_ApplyOnLineOrder '"&orderID&"',"&operUserID&","&shareID&",'"&accounts&"',"&cardTypeID&",1,'"&IP&"'")
            If rs(0)<>0 Then
                CxGame.MessageBoxReload rs(1),"PaySelectType.asp?cardType=1"
			    rs.Close:Set rs=Nothing:CloseGame()
		    Else	
		        Dim actUrl,actButton,CardPrice
		        If shareID=2 Then
		            CardPrice = CCur(rs("CardPrice"))*100
		            actUrl = "/PayKQ/send.asp"
		            actButton = "<input type=""submit"" class=""bntBuy1 bold f14"" value=""去快钱支付平台在线支付"" hidefocus=""true"" />"
		        Else
		            CardPrice=rs("CardPrice")
		            actUrl = "/PayYB/pay.asp"
		            actButton = "<input type=""submit"" class=""bntBuy1 bold f14"" value=""去易宝支付平台在线支付"" hidefocus=""true"" />"
		        End If
		%>
		<div class="recharge">
			<div class="recharge1 bold hui3">网银充值</div>
			<div class="skczBg">
			    <form name="form1" id="form1" method="get" action="<%=actUrl %>">
				<div class="step3"></div>
				<div class="err mtop25">您购买的是[<span class="hong"><%=CxGame.GetCardTypeName(rs("CardTypeID")) %></span>]，请确认以下订单信息：</div>
			    <!--订单信息开始-->
			  	<div class="tipsInfo1">
			  		<div class="tipsSu hong f14 bold">您的订单下达成功！</div>
					<div class="tipsInfo">
						<ul>
						<li>订 单 号：<%=rs("OrderID") %></li>
						<li>家园帐号：<%=rs("Accounts") %></li>
						<li>帐号  ID：<%=rs("GameID") %></li>
						<li>购卡类别：<%=CxGame.GetCardTypeName(rs("CardTypeID")) %></li>
						<li>订单金额：<%=rs("CardPrice") %>元</li>
						<li>支付平台：<%If shareID=2 Then  %>快钱支付平台<%Else %>易宝支付平台<%End If %></li> 
 						</ul>
					<div class="clear"></div>
					</div><br /><br />
					<div class="center">
					<%=actButton %>
					<input name="OrderNo" type="hidden" value="<%=rs("OrderID") %>" />
					<input name="Accounts" type="hidden" value="<%=rs("Accounts") %>" />
					<input name="CardPrice" type="hidden" value="<%=CardPrice %>" />
					<input name="CardTypeID" type="hidden" value="<%=rs("CardTypeID") %>" />
					<input name="CardTypeName" type="hidden" value="<%=CxGame.GetCardTypeName(rs("CardTypeID")) %>" />					
					</div><br />
			  	<div class="clear"></div><br /><br /><br /><br /><br /><br />
		  	    </div>
		  	    </form>
				<!--订单信息结束-->				
			<div class="clear"></div><br />
		    </div>
			<div class="recharge2"><div class="clear"></div></div>
		<div class="clear"></div>
		</div>
		<% 
		    rs.Close:Set rs=Nothing:CloseGame()
		    End If
		%>
		<!--网银充值结束-->
	<div class="clear"></div>
	</div>
	<!--右边部分结束-->
<div class="clear"></div>
</div>
<!--页面主体结束-->
<!--#include file="Themes/Common_Footer.asp" -->
</body>
</html>
