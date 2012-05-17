<%
'*
'* @Description: 快钱人民币支付网关接口范例
'* @Copyright (c) 上海快钱信息服务有限公司
'* @version 2.0
'*

'*
'在本文件中，商家应从数据库中，查询到订单的状态信息以及订单的处理结果。给出支付人响应的提示。

'本范例采用最简单的模式，直接从receive页面获取支付状态提示给用户。
'*

orderId=trim(request("orderId"))
orderAmount=request("orderAmount")/100
msg=trim(request("msg"))


%>
<!doctype html public "-//w3c//dtd html 4.0 transitional//en" >
<html>
	<head>
		<title>快钱支付结果</title>
		<meta http-equiv="content-type" content="text/html; charset=utf-8" >
	</head>
	
<BODY>
	
	<div align="center">
		<table width="259" border="0" cellpadding="1" cellspacing="1" bgcolor="#CCCCCC" >
			<tr bgcolor="#FFFFFF">
				<td width="80">支付方式:</td>
				<td >快钱[99bill]</td>
			</tr>
			<tr bgcolor="#FFFFFF">
				<td >订单编号:</td>
				<td ><%=orderId %></td>
			</tr>
			<tr bgcolor="#FFFFFF">
				<td>订单金额:</td>
				<td><%=(orderAmount)&"元" %></td>
			</tr>
			<tr bgcolor="#FFFFFF">
				<td>支付结果:</td>
				<td><%=msg %></td>
			</tr>
			<tr>
				<td></td>
				<td></td>
			</tr>
	  </table>
	  <input type=button value=' 关闭网页 ' onclick='javascript:window.close();'>
	</div>

</BODY>
</HTML>