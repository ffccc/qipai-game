<!--#include file="CommonFun.asp"-->
<!--#include file="conn.asp"-->
<!--#include file="GameConn.asp"-->
<html xmlns="http://www.w3.org/1999/xhtml" >
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <title><%=GetQPAdminSiteName() %></title>
    <link href="Css/layout.css" rel="stylesheet" type="text/css" />
    <script type="text/javascript" src="Js/common.js"></script>       
    <script type="text/javascript" src="Js/comm.js"></script>    
</head>
<body class="warper">
<table border=0 cellspacing=0 cellPadding=0 width="100%" height="100%">
<tbody>
<tr><td class="top">
<div class="logo left"><img src="Images/logo.png" /></div>
<div class="left hui f12 Tmg20 lh Lmg10" >
<div>欢迎您,<a class="f" href="javascript:openWindow('Module/BackManager/MasterUserInfo.asp?UserName=<%=session("AdminName") %>',500, 320)" class="white12"><span class="cheng"><%=session("AdminName")%></span></a><<%=CxGame.GetRoleName() %>></div>
<div><a href="Index.asp" class="f">后台首页</a> | <a href="Login.asp?Action=Logout" class="f">安全退出</a>| <% If CxGame.GetIPBandValue()="0" Then %> <a href="BandIP.asp?Action=unband" class="f" >解除绑定</a><% Else %> <a href="BandIP.asp?Action=band" class="f" >绑定IP</a><% End If %></div>
</div>

<!--<div class="right hui f12 Tmg7 lh Rpd10" ><a href="#" class="f">家园首页</a> | <a href="#" class="f">家园论坛</a> | <a href="#" class="f">网狐棋牌</a></div>-->
</td>
</tr>
<tr>
<td>
<div class="sidebar_a">
<iframe src="Left.asp" frameborder="0" style="WIDTH: 173px; HEIGHT: 100%; visibility:inherit"></iframe>
</div>
<div class="sidebar_b">
<iframe name="frm_main_content" id="frm_main_content" height="100%" src="right.asp" frameBorder=no width="100%" ></iframe>
</div>
</td>
</tr>
<tr>
<td style="height:0px">
    <div id="msgBoxDIV" style="position: absolute; width: 100%; padding-top: 4px; height: 24px; top: 55px; text-align: center;">
        <span class="msg"></span>
    </div>
</td>
</tr>
</tbody>
</table>
</body>
</html>
