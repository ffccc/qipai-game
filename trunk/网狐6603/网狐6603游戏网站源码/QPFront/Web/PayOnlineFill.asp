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
		<div class="recharge">
			<div class="recharge1 bold hui3">网银充值</div>
			<div class="skczBg">
				<div class="step1"></div>
				<div class="err mtop25">请选择以下充值卡类型：</div>
			    <!--充值卡开始-->
                <div class="content1">
				<ul>
				<li><span class="card1"></span><span class="hui3"><strong class="lan  f14">水晶月卡</strong><br/>购买价格：10元<br/>赠送金币：100000<br/>赠送会员：黄钻<br/>会员时间：30天
				</span><strong>
				<input type="button" class="bntBuy bold f14" value="点击购买" onclick= "location.href='PaySelectType.asp?cardType=1';"  hidefocus="true"/></strong></li>
				<li><span class="card2"></span><span class="hui3"><strong class="huang f14">黄金半年卡</strong><br/>购买价格：60元<br/>赠送金币：600000<br/>赠送会员：蓝钻<br/>会员时间：180天
				</span><strong>
				<input type="button" class="bntBuy bold f14" value="点击购买" onclick= "location.href='PaySelectType.asp?cardType=2';" hidefocus="true"/></strong></li>
				<li><span class="card3"></span><span class="hui3"><strong class="hui6 f14">白金年卡</strong><br/>购买价格：120元<br/>赠送金币：1200000<br/>赠送会员：红钻<br/>会员时间：360天</span><strong>
				<input type="button" class="bntBuy bold f14" onclick= "location.href='PaySelectType.asp?cardType=3';"  value="点击购买" hidefocus="true"/></strong></li>
				</ul>
				<div class="clear"></div>
			    </div>
				<!--充值卡结束-->				
			<div class="clear"></div>
		    </div>
			<div class="recharge2"><div class="clear"></div></div>
		<div class="clear"></div>
		</div>
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
