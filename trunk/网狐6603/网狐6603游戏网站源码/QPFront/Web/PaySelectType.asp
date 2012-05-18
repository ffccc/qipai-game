<!--#include file="Inc/CommonFun.asp" -->
<!--#include file="Inc/AutoCheck.asp" -->
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
    <link rel="stylesheet" type="text/css" href="css/cz_layout.css" />
    <script src="JS/jquery_last.js" type="text/javascript"></script>
    <script src="JS/formValidator.js" type="text/javascript"></script>
    <script src="JS/formValidatorRegex.js" type="text/javascript"></script>
    
    <script type="text/javascript">
        $(document).ready(function(){
            //页面验证
            $.formValidator.initConfig({formid:"form1",alertmessage:true,onerror:function(msg){alert(msg)}});
            $("#txtAccounts").formValidator({onshow:" ",onfocus:"用户名至少2~16个英文或中文字符",oncorrect:"该用户名可以充值"})
                .inputValidator({min:1,onerror:"充值用户名不能为空"})
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
		<!--网银充值开始-->
		<% 
		    Dim cardType
		    cardType = CxGame.GetInfo(0,"cardType")
		    If cardType="" Then
		        cardType=1
		    ElseIf cardType>3 Or cardType<=0 Then
		        cardType=1
		    End If
		%>
		<div class="recharge">
			<div class="recharge1 bold hui3">网银充值</div>
			<div class="skczBg">
				<div class="step2"></div>
				<div class="clear"></div><br />
				<!--充值卡开始-->
				<form name="form1" id="form1" method="post" action="PaySubmit.asp">
				<div class="crad">
					<ul>
					<li <% If cardType=1 Then %> class="tab2" <%Else %> class="tab1" <% End If %> onclick="location.href='?cardType=1'">水晶月卡</li>
					<li <% If cardType=2 Then %> class="tab2" <%Else %> class="tab1" <% End If %> onclick="location.href='?cardType=2'">黄金半年卡</li>
					<li <% If cardType=3 Then %> class="tab2" <%Else %> class="tab1" <% End If %> onclick="location.href='?cardType=3'">白金年卡</li>
					</ul>
				<div class="clear"></div>
				</div>
				<div class="card11" >
					<div class="err mtop13">请填写要充值的用户名：</div><br /><br />
						<div class="userBox left">
							<p><span class="left">充值用户名：</span><input type="text" id="txtAccounts" name="txtAccounts" class="text" /></p>
							<p><span class="left">确认用户名：</span><input type="text" id="txtAccounts2" name="txtAccounts2" class="text"  /></p>
							<input name="CardType" type="hidden" value="<%=cardType %>" />
						</div>	
						<%If cardType =1 Then %>					
						<div class="userBox1 right">
							<div class="cardBox1">
								<span><img src="images/card1.gif" /></span>
								<label class="zhuan f14 bold">水晶月卡</label>
								<label>购买价格：10元&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;会员时间：30天<br/>赠送金币：100000&nbsp;&nbsp;&nbsp;赠送会员：黄钻</label>
							<div class="clear"></div>
							</div>
						</div>
						<% End If %>
						<%If cardType =2 Then %>					
						<div class="userBox1 right">
							<div class="cardBox1">
								<span><img src="images/card2.gif" /></span>
								<label class="zhuan f14 bold">黄金半年卡</label>
								<label>购买价格：60元&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;会员时间：180天<br/>赠送金币：600000&nbsp;&nbsp;&nbsp;赠送会员：蓝钻</label>
							<div class="clear"></div>
							</div>
						</div>
						<% End If %>
						<%If cardType =3 Then %>					
						<div class="userBox1 right">
							<div class="cardBox1">
								<span><img src="images/card3.gif" /></span>
								<label class="zhuan f14 bold">白金年卡</label>
								<label>购买价格：120元&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;会员时间：360天<br/>赠送金币：1200000&nbsp;&nbsp;&nbsp;赠送会员：红钻</label>
							<div class="clear"></div>
							</div>
						</div>
						<% End If %>
						<div class="clear"></div>
                     <br />
					<div class="err mtop13">请选择支付平台：</div><br />
						<ul>
						<li><img src="images/kq.gif" /><span><input type="radio" name="PayType" value="2" checked="checked" /></span></li>
						<li><img src="images/yb.gif" /><span><input type="radio" name="PayType" value="3" /></span></li>
				        </ul>
					<div class="clear"></div><br /><br />
					<div class="center"><input type="submit" class="bntBuy bold f14" value="下一步" hidefocus="true" /></div><br />
					<div class="clear"></div><br /><br /><br /><br /><br /><br /><br /><br />
	  		  </div>
	  		  </form>
			  <!--充值卡结束-->
			<div class="clear"></div><br />
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
