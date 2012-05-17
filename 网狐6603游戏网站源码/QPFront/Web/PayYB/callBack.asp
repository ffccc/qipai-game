<!-- #include file="../Inc/Conn.asp" -->
<!-- #include file="yeepayCommon.asp" -->
<%
'**************************************************
'* @Description 易宝支付产品通用支付接口范例  
'* @V3.0
'* @Author rui.xin
'**************************************************

	'	只有支付成功时易宝支付才会通知商户.
	''支付成功回调有两次，都会通知到在线支付请求参数中的p8_Url 上：浏览器重定向;服务器点对点通讯.
	
	Dim r0_Cmd
	Dim r1_Code
	Dim r2_TrxId
	Dim r3_Amt
	Dim r4_Cur
	Dim r5_Pid
	Dim r6_Order
	Dim r7_Uid
	Dim r8_MP
	Dim r9_BType
	Dim hmac
	'不用验证参数
	Dim rb_BankId,ro_BankOrderId,rp_PayDate,rq_CardNo,ru_Trxtime
	
	Dim bRet
	Dim returnMsg
	
	'解析返回参数
	Call getCallBackValue(r0_Cmd,r1_Code,r2_TrxId,r3_Amt,r4_Cur,r5_Pid,r6_Order,r7_Uid,r8_MP,r9_BType,hmac,rb_BankId,ro_BankOrderId,rp_PayDate,rq_CardNo,ru_Trxtime)
	'Call createLog(logName)
	'判断返回签名是否正确（True/False）
	bRet = CheckHmac(r0_Cmd,r1_Code,r2_TrxId,r3_Amt,r4_Cur,r5_Pid,r6_Order,r7_Uid,r8_MP,r9_BType,hmac)
	'以上代码和变量不需要修改.
	
	
	'校验码正确
	returnMsg	= ""
	If bRet = True Then
	  If(r1_Code="1") Then
	  	
		'需要比较返回的金额与商家数据库中订单的金额是否相等，只有相等的情况下才认为是交易成功.
		'并且需要对返回的处理进行事务控制，进行记录的排它性处理，防止对同一条交易重复发货的情况发生.
		
		'易宝支付记录
		Rem 易宝支付记录
		rp_PayDate = Mid(rp_PayDate,1,4)&"-"&Mid(rp_PayDate,5,2)&"-"&Mid(rp_PayDate,7,2)&" "&Mid(rp_PayDate,9,2)&":"&Mid(rp_PayDate,11,2)&":"&Mid(rp_PayDate,13,2)
		ru_Trxtime = Mid(ru_Trxtime,1,4)&"-"&Mid(ru_Trxtime,5,2)&"-"&Mid(ru_Trxtime,7,2)&" "&Mid(ru_Trxtime,9,2)&":"&Mid(ru_Trxtime,11,2)&":"&Mid(ru_Trxtime,13,2)
        Call ConnectGame(QPTreasureDB)
        GameConn.Execute("WSP_PW_AddReturnYPInfo '"&p1_MerId&"','"&r0_Cmd&"','"&r1_Code&"','"&r2_TrxId&"','"&r3_Amt&"','"&r4_Cur&"','"&r5_Pid&"','"&r6_Order&"','"&r7_Uid&"','"&r8_MP&"','"&r9_BType&"','"&rb_BankId&"','"&ro_BankOrderId&"','"&rp_PayDate&"','"&rq_CardNo&"','"&ru_Trxtime&"','"&hmac&"'")
        Dim IP
        IP = Request.ServerVariables("REMOTE_ADDR")
        GameConn.Execute("WSP_PW_FilledOnLine '"&r6_Order&"','"&IP&"'")
        CloseGame()
		
			If(r9_BType="1") Then
				'	在线支付页面返回	
				returnMsg	= returnMsg	&	r3_Amt & "RMB Success！"				
			ElseIf(r9_BType="2") Then				
	  		'	如果需要应答机制则必须回写以"success"开头的stream,大小写不敏感.
	  		''易宝支付收到该stream，便认为商户已收到；否则将继续发送通知，直到商户收到为止。
		 		Response.write("success")
				'Call createLog(logName)
				returnMsg	= returnMsg	& "在线支付服务器返回"
			End IF  
	  End IF
	Else
		returnMsg	= returnMsg	&	"交易信息被篡改"
	End If


%>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<title>易宝支付 - 网上开店 | 电子商务首选服务商，支付用易宝，方便又可靠！</title>
<meta name="description" content="易宝支付作为第三方金融服务提供商，与工商保险柜、招商保险柜、建设保险柜、农业保险柜、民生保险柜等多家国 内保险柜及VISA、MasterCard外卡组织紧密合作，为个人客户提供在线支付、电话支付、虚拟账户理财服务， 为企业商户提供保险柜网关支付、代收代付、委托结算以及B2B转款等多项增值业务。">
<meta http-equiv="Content-Type" content="text/html; charset=gb2312" />
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
		  	<td colspan="2" bgcolor="#CEE7BD">易宝支付通用接口演示：</td>
		  </tr>
		  <tr>
		  	<td align="left" width="30%">&nbsp;&nbsp;商户订单号</td>
		  	<td align="left">&nbsp;&nbsp;<%= r6_Order %></td>
      </tr>
		  <tr>
		  	<td align="left">&nbsp;&nbsp;支付结果</td>
		  	<td align="left">&nbsp;&nbsp;<%= returnMsg %></td>
      </tr>
      <tr>
      	<td height="5" bgcolor="#6BBE18" colspan="2"></td>
      </tr>
      </table></td>
        </tr>
      </table>
	</body>
</html>