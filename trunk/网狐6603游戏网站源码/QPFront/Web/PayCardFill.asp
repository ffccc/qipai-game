<!--#include file="Inc/CommonFun.asp" -->
<!--#include file="Inc/AutoCheck.asp" -->
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
    <script src="JS/jquery_last.js" type="text/javascript"></script>
    <script src="JS/formValidator.js" type="text/javascript"></script>
    <script src="JS/formValidatorRegex.js" type="text/javascript"></script>
    
    <script type="text/javascript">
        $(document).ready(function(){
            //页面验证
            $.formValidator.initConfig({formid:"form1",alertmessage:false});
            $("#txtAccounts").formValidator({onshow:" ",onfocus:"用户名至少2~16个英文或中文字符",oncorrect:"该用户名可以充值"})
                .inputValidator({min:2,max:16,onerror:"你输入的用户名非法,请确认"})
                .regexValidator({regexp:"usernamecn",datatype:"enum",onerror:"用户名格式不正确"})
                .ajaxValidator({
                    isvalid:true,
                    type : "get",
                    url : "Inc/Ajax.asp?t=CheckUser",
                    datatype : "json",
                    success : function(data){	
                        if(data == 0)
			            {
                            return false;
			            }
                        else
			            {
                            return true;
			            }                        
		            },
		            error: function(){alert("服务器没有返回数据，可能服务器忙，请重试");},
		            onerror : "该用户名不存在，请更换用户名",
		            onwait : "正在对用户名进行合法性校验，请稍候..."
                });
            $("#txtAccounts2").formValidator({onshow:" ",onfocus:"用户名至少2~16个英文或中文字符"})
                .compareValidator({desid:"txtAccounts",operateor:"=",onerror:"两次输入不一致,请确认"});
            $("#txtSerialID").formValidator({onshow:" "}).inputValidator({min:1,empty:{leftempty:false,rightempty:false,emptyerror:"两边不能有空符号！"},onerror:"请输入卡号"})
            $("#txtPassword").formValidator({onshow:" "}).inputValidator({min:1,empty:{leftempty:false,rightempty:false,emptyerror:"两边不能有空符号！"},onerror:"请输入密码"})
        })
    </script>
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
		<!--实卡充值开始-->
		<div class="recharge">
			<div class="recharge1 bold hui3">实卡充值</div>
			<div class="skczBg">
			    <form id="form1" name="form1" method="post" action="">
				<div class="content" >
				<p>
				    <span class="f14">游戏账号：</span>
				    <span><input type="text" id="txtAccounts" name="txtAccounts" class="text width200" value="<%=Session("UserName") %>"/></span>
				    <label id="txtAccountsTip"></label>
				</p>
				<p>
				    <span class="f14">确定账号：</span>
				    <span><input type="text" id="txtAccounts2" name="txtAccounts2" class="text width200" value="<%=Session("UserName") %>" /></span>
				    <label id="txtAccounts2Tip"></label>
				</p>
				<p>
				    <span class="f14">充值卡号：</span>
				    <span><input type="text" id="txtSerialID" name="txtSerialID" class="text width200"/></span>
				    <label id="txtSerialIDTip"></label>
				</p>
				<p>
				    <span class="f14">充值密码：</span>
				    <span><input type="password" id="txtPassword" name="txtPassword" class="text width200"/></span>
				    <label id="txtPasswordTip"></label>
				</p>
				<span style="padding-left:100px;">
			    <input type="submit" value="充值"  class="bnt lan bold" hidefocus="true"/>
			    <input type="reset" value="重置" class="bnt lan bold" hidefocus="true"/>
			    <input name="Pay" type="hidden" value="true" />
				</span>
				</div>
				<% CxGame.CardPay() %>
				</form>
				<!--充值卡开始-->
				<div class="content1" style="border-top:1px dashed #ccc;">
				<ul>
				<li><span class="card1"></span><span class="hui3"><strong class="lan  f14">水晶月卡</strong><br/>购买价格：10元<br/>赠送金币：100000<br/>赠送会员：黄钻<br/>会员时间：30天</span></li>
				<li><span class="card2"></span><span class="hui3"><strong class="huang f14">黄金半年卡</strong><br/>购买价格：60元<br/>赠送金币：600000<br/>赠送会员：蓝钻<br/>会员时间：180天</span></li>
				<li><span class="card3"></span><span class="hui3"><strong class="hui6 f14">白金年卡</strong><br/>购买价格：120元<br/>赠送金币：1200000<br/>赠送会员：红钻<br/>会员时间：360天</span></li>		
				</ul>
				<div class="clear"></div>
			    </div>
				<!--充值卡结束-->				
			<div class="clear"></div>
			</div>
			<div class="recharge2"><div class="clear"></div></div>
		<div class="clear"></div>
		</div>
		<!--实卡充值结束-->
		
	<div class="clear"></div>
	</div>
	<!--右边部分结束-->
<div class="clear"></div>
</div>
<!--页面主体结束-->
<!--#include file="Themes/Common_Footer.asp" -->
</body>
</html>
