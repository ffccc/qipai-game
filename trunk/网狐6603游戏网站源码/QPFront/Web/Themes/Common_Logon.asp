<!--会员登录开始-->		
<% 
    If CxGame.IsLogin Then
%>	
    <div class="serve">
		<div class="serve1 bold hui3">个人信息</div>
	  	<div class="loginBg">
			<div class="login">
				<div class="logined">
					<dl><img src="gamepic/face<%=Session("FaceID") %>.gif" /></dl>
					<dl class="padding15">帐号：<%=Session("UserName") %><br />ＩＤ：<%=Session("GameID") %><br /></dl>
				</div>
				<div class="logined1">会员类型：</div><div class="logined2"><%=CxGame.GetMemberOrderName(Session("MemberOrder")) %></div>
				<div class="logined1">魅 力 值：</div><div class="logined2 bold hong"><%=Session("Loveliness") %></div>
				<div class="logined1">[<a href="MemberModifyLogonPass.asp" class="lh">修改密码</a>]&nbsp;&nbsp;</div>
				<div class="logined2">&nbsp;&nbsp;[<a href="?exit=true" class="lh">退出</a>]</div>
			<div class="clear"></div>
		  	</div>
		  <div class="clear"></div>	
	  </div>
		<div class="serve4"><div class="clear"></div></div>
	</div>
	<% CxGame.UserLogin() %>
<% 
    Else
%>
<div class="serve">
    <form name="form1" action="" method="post">
	    <div class="serve1 bold hui3">会员登录</div>
  	    <div class="loginBg">
		    <div class="login">
		    <ul>
		    <li>游戏帐号：<input type="text" name="txtAccounts" /></li>
		    <li>密　　码：<input type="password" name="txtLogonPass" /></li>
		    <li>
		    <span><img src="Inc/Code_Img.asp" id="ImageCheck" style="cursor:pointer;border: 1px solid #333333;width:55px;" title="点击更换验证码图片!"/></span>
		    验&nbsp;&nbsp;证&nbsp;码：<input type="text" name="txtCode" class="wd" /><input name="login" type="hidden" id="login" value="true">
		    </li>
		    <li><label><a href="MemberReLogonPass.asp" class="lh">忘记密码？</a></label><input type="image" src="images/login.gif" style="width:50px;height:24px; border:0; padding:0;" class="mleft60" /></li>
		    <li><a href="Register.asp"><img src="images/reg.gif" width="111" height="31" border="0" class="mleft45 " /></a></li>
		    </ul>
		    <div class="clear"></div>
	        </div>
	        <div class="clear"></div>	
        </div>
	    <div class="serve4"><div class="clear"></div></div>
	    <% CxGame.UserLogin() %>	
	</form>
</div>
<% End If %>
<!--会员登录结束-->