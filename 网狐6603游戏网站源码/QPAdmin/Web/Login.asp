
<!--#include file="GameConn.asp"-->
<!--#include file="md5.asp"-->

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<%
    dim Action,FoundErr,ErrMsg
    Action = Trim(Request("Action"))
    if Action="Login" then
        call ChkLogin()
    elseif Action="Logout" then
        call Logout()
    else
        call main()
    end if
    if FoundErr=True then
        call WriteErrMsg()
    end if
    
    sub main()
%>
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8">
    <title>管理员登录</title>
    <link href="Css/layout.css" rel="stylesheet" type="text/css" />
    <style type="text/css">
    <!--
    body {
	    background-image: url(images/loginBg.jpg);
	    font-family:"宋体";
    }
    -->
    </style>
    <script type="text/javascript">
        function ChangeCodeimg()
        {
            document.Login.ImageCheck.src = "Code_Img2.asp?"+ Math.random();
        }
        
        function SetFocus()
        {
            if (document.Login.txtLoginName.value=="")
	            document.Login.txtLoginName.focus();
            else
	            document.Login.txtLoginName.select();
        }
        
        function CheckForm()
        {
            if(document.Login.txtLoginName.value=="")
            {
                alert("请输入用户名！");
                document.Login.txtLoginName.focus();
                return false;
            }
            if(document.Login.txtLoginPass.value == "")
	        {
		        alert("请输入密码！");
		        document.Login.txtLoginPass.focus();
		        return false;
	        }
	        if(document.Login.txtCodeOp.value=="")
	        {
	            alert("请输入验证码！");
		        document.Login.txtCodeOp.focus();
		        return false;
	        }
        }
        function CheckBrowser() 
        {
            var app=navigator.appName;
            var verStr=navigator.appVersion;
            if (app.indexOf('Netscape') != -1) {
                alert("<%=SiteName%>提示你,你使用的是Netscape浏览器，可能会导致无法使用后台的部分功能。建议您使用 IE6.0 或以上版本。");
            } 
            else if (app.indexOf('Microsoft') != -1) {
              if (verStr.indexOf("MSIE 3.0")!=-1 || verStr.indexOf("MSIE 4.0") != -1 || verStr.indexOf("MSIE 5.0") != -1 || verStr.indexOf("MSIE 5.1") != -1)
                alert("<%=SiteName%>提示你,您的浏览器版本太低，可能会导致无法使用后台的部分功能。建议您使用 IE6.0 或以上版本。");
            }
        }
    </script>
</head>
<body>
<br />
<br />
<br />
<br />
<br />
<br />
<br />
<br />
<br />
<form name="Login" action="Login.asp" method="post" target="_parent" onsubmit="return CheckForm()">
<table width="705" border="0" align="center" cellpadding="0" cellspacing="0">
  <tr>
    <td><table width="705" border="0" cellspacing="0" cellpadding="0">
      <tr>
        <td width="125">&nbsp;</td>
        <td width="581"><img src="images/loginLogo.png" width="360" height="75" /></td>
      </tr>
    </table></td>
  </tr>
  <tr>
    <td><img src="images/login01.png" width="705" height="50" /></td>
  </tr>
  <tr>
    <td background="images/login02.png" width="705" height="235" ><table width="100%" border="0" cellspacing="0" cellpadding="0">
      <tr>
        <td width="301" height="33">&nbsp;</td>
        <td width="73" height="33" class="f14" align="right">帐　号：</td>
        <td width="331" height="33"><input name="txtLoginName" type="text" class="text" /></td>
      </tr>
      <tr>
        <td width="301" height="33">&nbsp;</td>
        <td height="33" class="f14" align="right">密　码：</td>
        <td height="33"><input name="txtLoginPass" type="password" class="text" /></td>
      </tr>
      <tr>
        <td width="301" height="33">&nbsp;</td>
        <td height="33" class="f14" align="right">验证码：</td>
        <td height="33"><input name="txtCodeOp" type="text" class="text" /></td>
      </tr>
      <tr>
        <td width="301" height="30">&nbsp;</td>
        <td height="30">&nbsp;</td>
        <td height="30" class="hui6">验证码请按下图中的数字填写</td>
      </tr>
      <tr>
        <td width="301" height="30">&nbsp;</td>
        <td height="30">&nbsp;</td>
        <td height="30"><img src="Code_Img2.asp" name="ImageCheck" style="cursor:pointer;border: 1px solid #333333;" title="点击更换验证码图片!" onclick="ChangeCodeimg()"/></td>
      </tr>
      <tr>
        <td width="301" height="30">&nbsp;</td>
        <td height="30">&nbsp;</td>
        <td height="30" class="lan"><a href="javascript:void(0)" class="l" onclick="ChangeCodeimg()">看不清楚？ 换一个</a></td>
      </tr>
      <tr>
        <td width="301" height="30">&nbsp;</td>
        <td height="30">&nbsp;</td>
        <td height="30"><input type="image" src="images/loginBtn.jpg" width="86" height="34" /><input type="hidden" name="Action" value="Login" /></td>
      </tr>
    </table></td>
  </tr>
  <tr>
    <td width="705" height="54" align="center" background="images/login03.png"  style="color:#115287;"><%=GetCopyRightName %></td>
  </tr>
  <tr>
    <td><img src="images/login04.png" width="705" height="46" /></td>
  </tr>
  <tr>
    <td><img src="images/login05.png" width="705" height="53" /></td>
  </tr>
</table>
</form>
<script type="text/javascript">
    CheckBrowser();
    SetFocus();    
</script>
</body>
</html>
<% 
end sub
    
sub ChkLogin()
    dim sql,rs
    dim userName,password,checkCode
    if Session("GetCode")<>Trim(Request("txtCodeOp")) then
        FoundErr = True
        Session("GetCode") = "scsc"
        ErrMsg=ErrMsg & "验证码不对，请正确输入验证码！"
    end if
    
    Session("GetCode") = "scsc"
    userName = Replace(Trim(Request("txtLoginName")),"'","")
    password = Replace(Trim(Request("txtLoginPass")),"'","")
    if userName="" then
        FoundErr = True
        ErrMsg=ErrMsg&"用户名不能为空！"
    end if
    if password="" then
        FoundErr = True
        ErrMsg=ErrMsg&"密码不能为空！"
    end if
    
    if FoundErr=True then
		exit sub
	end if
	
	password = md5(password,32)
	Call ConnectGame(QPPlatformManagerDB)
	set rs = server.createobject("adodb.recordset")
	sql = "select * from Base_Users where Username='"&userName&"' and password='"&password&"'"
	rs.Open sql,GameConn,1,3	
	if rs.BOF and rs.EOF then
	    FoundErr = True
        ErrMsg=ErrMsg&"用户名或密码错误！"
	else
	    if password<>rs("Password") then
	        FoundErr = True
	        ErrMsg=ErrMsg&"用户名或密码错误！"
	    elseif Request.ServerVariables("REMOTE_ADDR")<>rs("BandIP") and rs("IsBand")="0" Then
	        FoundErr = True
	        ErrMsg=ErrMsg&"此帐号已被绑定IP，无法登陆！"
	    end if
	end if    
	if FoundErr=True then
		session("AdminName")=""
		session("AdminPassword")=""
		rs.close
		set rs=nothing
		exit sub
	end if	
	rs("PreLogintime") = rs("LastLoginTime")
	rs("PreLoginIP") = rs("LastLoginIP")
	rs("LastLoginIP")=Request.ServerVariables("REMOTE_ADDR")
	rs("LastLoginTime")=now()
	rs("LoginTimes")=rs("LoginTimes")+1
	rs.Update
	session("AdminID") = rs("UserID")
	session("AdminName")=rs("username")
	session("AdminPassword")=rs("Password")
	
	Response.Cookies("QPAdminUserID")= rs("username")
    Response.Cookies("QPAdminUserPasswd")=rs("Password")
    rs.close
    set rs = nothing
    Call InsertSystemSecurity("登录")
    response.Redirect "Index.asp"
end sub

sub Logout()
     Call InsertSystemSecurity("登出")
    session("AdminID")=""
	session("AdminName")=""
	session("AdminPassword")=""
	Response.Cookies("QPAdminUserID")= ""
    Response.Cookies("QPAdminUserPasswd")=""
    Response.Cookies("QPAdminUserID").Expires = DateAdd("m",-1,now())
    Response.Cookies("QPAdminUserPasswd").Expires = DateAdd("m",-1,now())
	Response.Redirect "../Index.asp"
end sub
Sub InsertSystemSecurity(OperatingName)
        Call ConnectGame(QPPlatformManagerDB)
        GameConn.execute "Insert into SystemSecurity (OperatingTime,OperatingName,OperatingIP,OperatingAccounts) values ('"&Now()&"','"&OperatingName&"','"&Request.ServerVariables("REMOTE_ADDR")&"','"&session("AdminName")&"')"
End Sub   
 
sub WriteErrMsg()
    Session("ErrMsg") = ErrMsg
	Response.Redirect "Error.asp"
end sub
%>