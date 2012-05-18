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
    <link rel="stylesheet" type="text/css" href="css/game_layout.css" />
</head>
<body>
<%CxGame.Channel = "Shop"  %>
<!--#include file="Themes/Common_Header.asp" -->
<!--页面主体开始-->
<div class="main"  style="margin-top:8px;">
	<!--左边部分开始-->
	<!--#include file="Themes/Common_Sidebar_A.asp" -->	
	<!--左边部分结束-->
	
	<!--右边部分开始-->
	<div class="mainRight1">
		<!--道具商城开始-->
		<div class="gameBanner">
			<div class="shopTitle bold bai f14">道具商城</div>
			<div class="shopBg2">
				<ul>
				<li><span><img src="img/prop_01.gif" /></span><label><a href="#" class="lh">双倍积分卡</a></label><label class="Fline">原价格：1000</label><label class="cheng">会员价：900</label>
				  <label><img src="images/buy.gif" /> <img src="images/present.gif" width="33" height="18" /></label></li>
				<li><span><img src="img/prop_02.gif" width="90" height="90" /></span>
				  <label><a href="#" class="lh">四倍积分卡</a></label><label class="Fline">原价格：1000</label><label class="cheng">会员价：900</label>
				  <label><img src="images/buy.gif" /> <img src="images/present.gif" width="33" height="18" /></label></li>
				<li><span><img src="img/prop_03.gif" width="90" height="90" /></span>
				  <label><a href="#" class="lh">负分清零</a></label><label class="Fline">原价格：1000</label><label class="cheng">会员价：900</label>
				  <label><img src="images/buy.gif" /> <img src="images/present.gif" width="33" height="18" /></label></li>
				<li><span><img src="img/prop_04.gif" /></span><label><a href="#" class="lh">清逃跑率</a></label><label class="Fline">原价格：1000</label><label class="cheng">会员价：900</label>
				  <label><img src="images/buy.gif" /> <img src="images/present.gif" width="33" height="18" /></label></li>
				<li><span><img src="img/prop_05.gif" /></span><label><a href="#" class="lh">小喇叭</a></label><label class="Fline">原价格：1000</label><label class="cheng">会员价：900</label>
				  <label><img src="images/buy.gif" /> <img src="images/present.gif" width="33" height="18" /></label></li>
				<li><span><img src="img/prop_06.gif" /></span><label><a href="#" class="lh">防踢卡</a></label><label class="Fline">原价格：1000</label><label class="cheng">会员价：900</label>
				  <label><img src="images/buy.gif" /> <img src="images/present.gif" width="33" height="18" /></label></li>
				<li><span><img src="img/prop_07.gif" /></span><label><a href="#" class="lh">护身符</a></label><label class="Fline">原价格：1000</label><label class="cheng">会员价：900</label>
				  <label><img src="images/buy.gif" /> <img src="images/present.gif" width="33" height="18" /></label></li>
				<li><span><img src="img/prop_08.gif" /></span><label><a href="#" class="lh">红钻会员卡</a></label><label class="Fline">原价格：1000</label><label class="cheng">会员价：900</label>
				  <label><img src="images/buy.gif" /> <img src="images/present.gif" width="33" height="18" /></label></li>
				<li><span><img src="img/prop_09.gif" /></span><label><a href="#" class="lh">蓝钻会员卡</a></label><label class="Fline">原价格：1000</label><label class="cheng">会员价：900</label>
				  <label><img src="images/buy.gif" /> <img src="images/present.gif" width="33" height="18" /></label></li>
				<li><span><img src="img/prop_10.gif" /></span><label><a href="#" class="lh">黄钻会员卡</a></label><label class="Fline">原价格：1000</label><label class="cheng">会员价：900</label>
				  <label><img src="images/buy.gif" /> <img src="images/present.gif" width="33" height="18" /></label></li>
				<li><span><img src="img/prop_11.gif" /></span><label><a href="#" class="lh">紫钻会员卡</a></label><label class="Fline">原价格：1000</label><label class="cheng">会员价：900</label>
				  <label><img src="images/buy.gif" /> <img src="images/present.gif" width="33" height="18" /></label></li>
				</ul>	
			<div class="clear"></div>
		  	</div>

			<div class="shopBottom"><div class="clear"></div></div>
		<div class="clear"></div>
		</div>
		<!--道具商城结束-->
		
		<!--礼物商城开始-->
		<div class="gameBanner mtop10">
			<div class="shopTitle bold bai f14">礼物商城</div>
			<div class="shopBg2">
				<ul>
				<li><span><img src="img/xw.gif"/></span><label><a href="#" class="lh">香吻</a></label><label class="Fline">原价格：1000</label><label class="cheng">会员价：900</label><label>魅力点：+1</label><label>魅力值：+1</label><label><img src="images/viewInfo.gif" border="0"/></label>
				</li>
				<li><span><img src="img/xh.gif"/></span>
				  <label><a href="#" class="lh">鲜花</a></label>
				  <label class="Fline">原价格：1000</label><label class="cheng">会员价：900</label><label>魅力点：+1</label><label>魅力值：+1</label><label><img src="images/viewInfo.gif" border="0"/></label>
				</li>
				<li><span><img src="img/qc.gif"/></span>
				  <label><a href="#" class="lh">汽车</a></label>
				  <label class="Fline">原价格：1000</label><label class="cheng">会员价：900</label><label>魅力点：+1</label><label>魅力值：+1</label><label><img src="images/viewInfo.gif" border="0"/></label>
				</li>
				<li><span><img src="img/gz.gif" width="90" height="90" /></span>
				  <label><a href="#" class="lh">鼓掌</a></label>
				  <label class="Fline">原价格：1000</label><label class="cheng">会员价：900</label><label>魅力点：+1</label><label>魅力值：+1</label><label><img src="images/viewInfo.gif" border="0"/></label>
				</li>
				<li><span><img src="img/pj.gif" width="90" height="90" /></span>
				  <label><a href="#" class="lh">啤酒</a></label>
				  <label class="Fline">原价格：1000</label><label class="cheng">会员价：900</label><label>魅力点：+1</label><label>魅力值：+1</label><label><img src="images/viewInfo.gif" border="0"/></label>
				</li>
				<li><span><img src="img/xy.gif" /></span>
				<label><a href="#" class="lh">香烟</a></label>
				<label class="Fline">原价格：1000</label><label class="cheng">会员价：900</label><label>魅力点：+1</label><label>魅力值：+1</label><label><img src="images/viewInfo.gif" border="0"/></label>
				</li>
				<li>
				  <span><img src="img/zj.gif" /></span>
				  <label><a href="#" class="lh">钻戒</a></label><label class="Fline">原价格：1000</label><label class="cheng">会员价：900</label><label>魅力点：+1</label>
<label>魅力值：+1</label><label><img src="images/viewInfo.gif" border="0"/></label>
				</li>
				<li><span><img src="img/bs.gif" /></span><label><a href="#" class="lh">别墅</a></label><label class="Fline">原价格：1000</label><label class="cheng">会员价：900</label><label>魅力点：+1</label><label>魅力值：+1</label><label><img src="images/viewInfo.gif" border="0"/></label>
				</li>
				<li><span><img src="img/cd.gif" /></span><label><a href="#" class="lh">臭蛋</a></label><label class="Fline">原价格：1000</label><label class="cheng">会员价：900</label>
				<label>魅力点：-1</label>
				<label>魅力值：+1</label><label><img src="images/viewInfo.gif" border="0"/></label>
				</li>
				<li><span><img src="img/zt.gif" /></span><label><a href="#" class="lh">砖头</a></label><label class="Fline">原价格：1000</label><label class="cheng">会员价：900</label>
				<label>魅力点：-1</label>
				<label>魅力值：+1</label><label><img src="images/viewInfo.gif" border="0"/></label>
				</li>
				<li><span><img src="img/zd.gif" width="90" height="90" /></span>
				  <label><a href="#" class="lh">炸弹</a></label>
				  <label class="Fline">原价格：1000</label><label class="cheng">会员价：900</label>
				  <label>魅力点：-1</label>
				  <label>魅力值：+1</label><label><img src="images/viewInfo.gif" border="0"/></label>
				</li>
				</ul>	
			<div class="clear"></div>
	  	  </div>

			<div class="shopBottom"><div class="clear"></div></div>
		<div class="clear"></div>
		</div>
		<!--礼物商城结束-->
		
		
	<div class="clear"></div>
	</div>
	<!--右边部分结束-->
<div class="clear"></div>
</div>
<!--页面主体结束-->
<!--#include file="Themes/Common_Footer.asp" -->
</body>
</html>
