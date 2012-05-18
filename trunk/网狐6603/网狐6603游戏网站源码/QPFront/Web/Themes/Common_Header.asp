<!--头部开始-->
<div class="main">
	<div class="top">
		<div class="left"><img src="images/logo.gif"/></div>
		<div class="right mtop13 lan">
			<ul>
		    <li class="top1"><a id="A1" href="javascript:void(0);" onclick="setHomepage('http://qp2.foxuc.net')" class="ll" >设为首页</a></li>
		    <li class="top2"><a href="Sitemap.asp" class="ll">网站地图</a></li>
		    <li class="top3"><a href="javascript:void(0);" onclick="addFavorites('http://qp2.foxuc.net','网狐游戏棋牌演示平台 - 游戏融入生活，快乐无处不在')" class="ll">加入收藏</a></li>
		    </ul>
			<div class="clear"></div>
		</div>
	</div>
	<div class="nav">
		<div class="navLeft"></div>
		<div class="navCenter">
		<ul>
	    <li <%If CxGame.Channel= "Index" Then %> class="current" <% End If %>><a href="Index.asp" hidefocus="true">首页</a></li>
	    <li <%If CxGame.Channel= "News" Then %> class="current" <% End If %>><a href="NewsList.asp" hidefocus="true">新闻公告</a></li>
	    <li <%If CxGame.Channel= "Member" Then %> class="current" <% End If %>><a href="MemberIndex.asp" hidefocus="true">会员中心</a></li>
	    <li <%If CxGame.Channel= "Pay" Then %> class="current" <% End If %>><a href="PayIndex.asp" hidefocus="true">充值中心</a></li>
	    <li <%If CxGame.Channel= "Match" Then %> class="current" <% End If %>><a href="MatchIndex.asp" hidefocus="true">比赛中心</a></li>
	    <li <%If CxGame.Channel= "Shop" Then %> class="current" <% End If %>><a href="ShopIndex.asp" hidefocus="true">游戏商场</a></li>
	    <li <%If CxGame.Channel= "Spread" Then %> class="current" <% End If %>><a href="SpreadIndex.asp" hidefocus="true">推广系统</a></li>
	    <li <%If CxGame.Channel= "Service" Then %> class="current" <% End If %>><a href="ServiceIndex.asp" hidefocus="true">客服中心</a></li>
	    </ul>	
		</div>
		<div class="navRight"></div>
  </div>
</div>
<div class="clear"></div>
<!--头部结束-->
<script src="Js/header.js" type="text/javascript"></script>