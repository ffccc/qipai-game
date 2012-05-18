<!--#include file="../Inc/CommonFun.asp" -->
<!--#include file="../Inc/Conn.asp" -->

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
    <link rel="stylesheet" type="text/css" href="../css/layout.css" />
    <link rel="stylesheet" type="text/css" href="../css/global.css" />
    <link rel="stylesheet" type="text/css" href="../Css/cz_layout.css" />
</head>
<body>
<br /><br />
<div class="rechargeBg1">
    <ul>
	<li><div class="sk"></div><span><input type="button" value="充值说明" class="bnt lan bold mtop10" hidefocus="true"/><input type="button" value="立即充值"  class="bnt lan bold mtop10" onclick= "location.href='../PayCardFill.asp';" hidefocus="true"/></span> </li>
	<li><div class="kq"></div><span><input type="button" value="充值说明" class="bnt lan bold mtop10" hidefocus="true"/><input type="button" value="立即充值"  class="bnt lan bold mtop10" onclick= "location.href='../PayOnlineFill.asp';"  hidefocus="true"/></span> </li>
    </ul>
  <div class="clear"></div>
</div>

</body>
</html>