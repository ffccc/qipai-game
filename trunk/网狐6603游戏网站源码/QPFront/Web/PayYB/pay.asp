<%@LANGUAGE="VBSCRIPT" CODEPAGE="936"%>
<%Session.CodePage=936 %>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=gb2312" />
<title>易宝支付 - 网上开店 | 电子商务首选服务商，支付用易宝，方便又可靠！</title>
<meta name="description" content="易宝支付作为第三方金融服务提供商，与工商保险柜、招商保险柜、建设保险柜、农业保险柜、民生保险柜等多家国 内保险柜及VISA、MasterCard外卡组织紧密合作，为个人客户提供在线支付、电话支付、虚拟账户理财服务， 为企业商户提供保险柜网关支付、代收代付、委托结算以及B2B转款等多项增值业务。">
		<link href="css/yeepaytest.css" type="text/css" rel="stylesheet" />
	</head>
	<body>
		<table width="50%" border="0" align="center" cellpadding="0" cellspacing="0" style="border:solid 1px #107929">
		  <tr>
		    <td><table width="100%" border="0" align="center" cellpadding="5" cellspacing="1">
		  </tr>
		  <tr>
		    <td height="30" align="left"><a href="http://www.yeepay.com/"><img src="http://www.yeepay.com/new-indeximages/yeepay-indexlogo.gif" alt="YeePay易宝支付 创新的多元化支付平台 安全 可靠 便捷 自助接入" width="141" height="47" border="0" /></a></td>
		  </tr>
		  <tr>
		  	<td height="30" colspan="2" bgcolor="#6BBE18"><span style="color: #FFFFFF">感谢您使用易宝支付平台</span></td>
		  </tr>
		  <tr>
		  	<td colspan="2" bgcolor="#CEE7BD">易宝支付产品通用支付接口演示：</td>
		  </tr>
			<form method="get" action="req.asp" targe="_blank">		
		  <tr>
		  	<td align="left" width="30%">&nbsp;&nbsp;商户订单号</td>
		  	<td align="left">&nbsp;&nbsp;<input size="50" type="text" name="p2_Order" id="p2_Order" value="<%=Request("OrderNo") %>" readonly /></td>
      </tr>
		  <tr>
		  	<td align="left">&nbsp;&nbsp;支付金额</td>
		  	<td align="left">&nbsp;&nbsp;<input size="50" type="text" name="p3_Amt" id="p3_Amt" value="<%=Request("CardPrice") %>" readonly/>&nbsp;<span style="color:#FF0000;font-weight:100;">*</span></td>
      </tr>
		  <tr style="display:none">
		  	<td align="left">&nbsp;&nbsp;商品名称</td>
		  	<td align="left">&nbsp;&nbsp;<input size="50" type="text" name="p5_Pid" id="p5_Pid" value=""/></td>
      </tr>
		  <tr style="display:none">
		  	<td align="left">&nbsp;&nbsp;商品种类</td>
		  	<td align="left">&nbsp;&nbsp;<input size="50" type="text" name="p6_Pcat" id="p6_Pcat" value="producttype"/></td>
      </tr>
		  <tr style="display:none">
		  	<td align="left">&nbsp;&nbsp;商品描述</td>
		  	<td align="left">&nbsp;&nbsp;<input size="50" type="text" name="p7_Pdesc" id="p7_Pdesc"  value="productdesc"/></td>
      </tr>
		  <tr style="display:none">
		  	<td align="left">&nbsp;&nbsp;接收支付成功数据的地址</td>
		  	<td align="left">&nbsp;&nbsp;<input size="50" type="text" name="p8_Url" id="p8_Url" value="http://localhost:6061/PayYB/callBack.asp" />&nbsp;<span style="color:#FF0000;font-weight:100;">*</span></td>
      </tr>
		  <tr>
		  	<td align="left">&nbsp;&nbsp;用户名</td>
		  	<td align="left">&nbsp;&nbsp;<input size="50" type="text" name="pa_MP" id="pa_MP"  value="<%=Request("Accounts") %>" readonly/></td>
      </tr>
		  
		  <tr style="display:none">
		  	<td align="left">&nbsp;&nbsp;支付通道编码</td>
		  	<td align="left">&nbsp;&nbsp;<input type="text" name="pd_FrpId" /><!--支付通道编码在易宝支付产品(HTML版)通用接口使用说明中-->
		  	</td>
      </tr>
		  <tr>
		  	<td align="left">&nbsp;</td>
		  	<td align="left">&nbsp;&nbsp;<input type="submit" value="马上支付" /></td>
      </tr>
    </form>
      <tr>
      	<td height="5" bgcolor="#6BBE18" colspan="2"></td>
      </tr>
      </table></td>
        </tr>
      </table>
	</body>
</html>
<% 
Function utf2gb(Body) 
    Dim Objstream 
    Set Objstream = Server.CreateObject("adodb.stream") 
    objstream.Charset = "gb2312" 
    objstream.Type = 2 
    objstream.Mode =3 
    objstream.Open 
    objstream.WriteText body 
    objstream.Position = 0 
    objstream.Charset = "utf-8" 
    objstream.Type = 2 
    utf2gb = objstream.ReadText 
    objstream.Close 
    set objstream = nothing 
End Function 
%>