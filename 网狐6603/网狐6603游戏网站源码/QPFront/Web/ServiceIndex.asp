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
    <link rel="stylesheet" type="text/css" href="css/customer_layout.css" />
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
				<div class="xsbz"></div>
				<div><a href="ServiceIssue.asp" class="cjwt" hidefocus="true"></a></div>
				<div><a href="ServiceFeedback.asp" class="wtfk" hidefocus="true"></a></div>
				<div><a href="ServiceCustomer.asp" class="kfdh" hidefocus="true"></a></div>
				<div class="cLeftBottom"></div>
				<div class="clear"></div>
			</div>			
			<div class="cRight">				
              <div>
                <h4>一、下载游戏大厅：</h4>
                <p>在网站首页左上角点击“下载游戏大厅”，进入下载页面，选择简单安装包或是完整安装包，下载大厅安装程序；</p>
                <div>
                  <p><img alt="游戏大厅下载" src="images/xsrm/xsrm_down.jpg" /></p>
                  <p><img alt="游戏大厅下载" src="images/xsrm/xsrm_down2.jpg" /></p>
                </div>
              </div>
              <div>
                <h4>二、安装游戏大厅：</h4>
                <p>双击已下载的安装程序 ，根据提示，进行安装；安装成功后，会在您的桌面自动生成一个大厅的LOGO图标；</p>
                <div>
                  <p><img alt="双击安装程序" src="images/xsrm/xsrm_intall_ico.jpg" /></p>
                </div>
              </div>
              <div>
                <h4>三、注册及登录：</h4>
                <p>安装成功后，会自动弹出改下登陆框，点击右下角的“申请帐号”按钮，进行注册；注册成功后，就直接进入平台了；下次再进平台时，只是双击桌面游戏平台ICO图标，输入帐号及密码，点击登录即可进入平台游戏；</p>
                <p>注意：如果您是家用电脑，可在登录框记住密码前的框内点勾，方便下次登录。不过，在公共场合，请勿用此功能。</p>
                <div>
                  <p><img alt="您可以在这里登录游戏或进行帐号申请" src="images/xsrm/xsrm_logon.jpg" /></p>
                </div>
              </div>
              <div>
                <h4>四、寻找游戏：</h4>
                <p>进入游戏广场后，在左边的游戏列表中选择您想要玩的游戏，第一次玩时需要进行下载。下载OK后双击进入游戏大厅了。</p>
                <div>
                  <p><img alt="游戏列表" src="images/xsrm/xsrm_list.jpg" /></p>
                </div>
              </div>
              <div>
                <h4>五、寻找空桌子和椅子：</h4>
                <p>进入游戏大厅时，您就可以找空桌椅。如下图：</p>
                <div>
                  <p><img alt="选择空闲桌子和椅子坐下" src="images/xsrm/xsrm_table.jpg" /></p>
                </div>
                <p>里面稍大的是桌子，外围的是椅子。当有人坐下时，头像代替椅子。您可以直接选择空的椅子，也可以点击房间工具栏上的“加入”按钮自动加入到空椅子。</p>
                <div>
                  <p><img alt="也可以单击“加入”直接进入到空闲的游戏桌" src="images/xsrm/xsrm_toolbar.jpg" /></p>
                </div>
              </div>
              <div>
                <h4>六、开始游戏：</h4>
                <p>当您坐上椅子的时候，也就马上进入了游戏房间。点击“开始”按钮，进入游戏。</p>
                <div>
                  <p><img alt="点击“开始”按钮，进入游戏" src="images/xsrm/xsrm_gamestart.jpg" /></p>
                </div>
              </div>
              <div>
                <h4>七、离开游戏：</h4>
                <p>当您想离开游戏房间时，直接点右上角的关闭按钮。这时会出现下面对话框，“退出房间”按钮就退回到大厅，重新选择房间；如果点击“关闭大厅”，则退出游戏。</p>
                <div>
                  <p><img alt="点击“关闭大厅”，则退出游戏" src="images/xsrm/xsrm_logout.jpg" /></p>
                </div>
              </div>
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
