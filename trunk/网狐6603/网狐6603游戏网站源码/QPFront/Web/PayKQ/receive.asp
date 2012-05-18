<!--#include file="../Inc/CommonFun.asp"-->
<!--#include file="../Inc/Conn.asp"-->
<!--#include file="md5.asp"-->
<%
'*
'* @Description: 快钱人民币支付网关接口范例
'* @Copyright (c) 上海快钱信息服务有限公司
'* @version 2.0
'*

'获取人民币网关账户号
merchantAcctId=trim(request("merchantAcctId"))

'设置人民币网关密钥
''区分大小写
key="ZUZNJB8MF63GA83J"

'获取网关版本.固定值
''快钱会根据版本号来调用对应的接口处理程序。
''本代码版本号固定为v2.0
version=trim(request("version"))

'获取语言种类.固定选择值。
''只能选择1、2、3
''1代表中文；2代表英文
''默认值为1
language=trim(request("language"))

'签名类型.固定值
''1代表MD5签名
''当前版本固定为1
signType=trim(request("signType"))

'获取支付方式
''值为：10、11、12、13、14
''00：组合支付（网关支付页面显示快钱支持的各种支付方式，推荐使用）10：保险柜卡支付（网关支付页面只显示保险柜卡支付）.11：电话保险柜支付（网关支付页面只显示电话支付）.12：快钱账户支付（网关支付页面只显示快钱账户支付）.13：线下支付（网关支付页面只显示线下支付方式）.14：B2B支付（网关支付页面只显示B2B支付，但需要向快钱申请开通才能使用）
payType=trim(request("payType"))

'获取保险柜代码
''参见保险柜代码列表
bankId=trim(request("bankId"))

'获取商户订单号
orderId=trim(request("orderId"))

'获取订单提交时间
''获取商户提交订单时的时间.14位数字。年[4位]月[2位]日[2位]时[2位]分[2位]秒[2位]
''如：20080101010101
orderTime=trim(request("orderTime"))

'获取原始订单金额
''订单提交到快钱时的金额，单位为分。
''比方2 ，代表0.02元
orderAmount=trim(request("orderAmount"))

'获取快钱交易号
''获取该交易在快钱的交易号
dealId=trim(request("dealId"))

'获取保险柜交易号
''如果使用保险柜卡支付时，在保险柜的交易号。如不是通过保险柜支付，则为空
bankDealId=trim(request("bankDealId"))

'获取在快钱交易时间
''14位数字。年[4位]月[2位]日[2位]时[2位]分[2位]秒[2位]
''如；20080101010101
dealTime=trim(request("dealTime"))

'获取实际支付金额
''单位为分
''比方 2 ，代表0.02元
payAmount=trim(request("payAmount"))

'获取交易手续费
''单位为分
''比方 2 ，代表0.02元
fee=trim(request("fee"))

'获取扩展字段1
ext1=trim(request("ext1"))

'获取扩展字段2
ext2=trim(request("ext2"))

'获取处理结果
''10代表 成功11代表 失败
''00代表 下订单成功（仅对电话保险柜支付订单返回）;01代表 下订单失败（仅对电话保险柜支付订单返回）
payResult=trim(request("payResult"))

'获取错误代码
''详细见文档错误代码列表
errCode=trim(request("errCode"))

'获取加密签名串
signMsg=trim(request("signMsg"))



'生成加密串。必须保持如下顺序。
	merchantSignMsgVal=appendParam(merchantSignMsgVal,"merchantAcctId",merchantAcctId)
	merchantSignMsgVal=appendParam(merchantSignMsgVal,"version",version)
	merchantSignMsgVal=appendParam(merchantSignMsgVal,"language",language)
	merchantSignMsgVal=appendParam(merchantSignMsgVal,"signType",signType)
	merchantSignMsgVal=appendParam(merchantSignMsgVal,"payType",payType)
	merchantSignMsgVal=appendParam(merchantSignMsgVal,"bankId",bankId)
	merchantSignMsgVal=appendParam(merchantSignMsgVal,"orderId",orderId)
	merchantSignMsgVal=appendParam(merchantSignMsgVal,"orderTime",orderTime)
	merchantSignMsgVal=appendParam(merchantSignMsgVal,"orderAmount",orderAmount)
	merchantSignMsgVal=appendParam(merchantSignMsgVal,"dealId",dealId)
	merchantSignMsgVal=appendParam(merchantSignMsgVal,"bankDealId",bankDealId)
	merchantSignMsgVal=appendParam(merchantSignMsgVal,"dealTime",dealTime)
	merchantSignMsgVal=appendParam(merchantSignMsgVal,"payAmount",payAmount)
	merchantSignMsgVal=appendParam(merchantSignMsgVal,"fee",fee)
	merchantSignMsgVal=appendParam(merchantSignMsgVal,"ext1",ext1)
	merchantSignMsgVal=appendParam(merchantSignMsgVal,"ext2",ext2)
	merchantSignMsgVal=appendParam(merchantSignMsgVal,"payResult",payResult)
	merchantSignMsgVal=appendParam(merchantSignMsgVal,"errCode",errCode)
	merchantSignMsgVal=appendParam(merchantSignMsgVal,"key",key)
merchantSignMsg= md5(merchantSignMsgVal)


'初始化结果及地址
rtnOk=0
rtnUrl=""

'商家进行数据处理，并跳转会商家显示支付结果的页面
''首先进行签名字符串验证
If UCase(signMsg)=UCase(merchantSignMsg) then
    '新增快钱记录
    payAmount=payAmount/100
    orderAmount=orderAmount/100
    fee=fee/100
    orderTime = Mid(orderTime,1,4)&"-"&Mid(orderTime,5,2)&"-"&Mid(orderTime,7,2)&" "&Mid(orderTime,9,2)&":"&Mid(orderTime,11,2)&":"&Mid(orderTime,13,2)
    dealTime = Mid(dealTime,1,4)&"-"&Mid(dealTime,5,2)&"-"&Mid(dealTime,7,2)&" "&Mid(dealTime,9,2)&":"&Mid(dealTime,11,2)&":"&Mid(dealTime,13,2)
	Call CxGame.CreateKQReturnInfo(merchantAcctId,version,language,signType,payType,bankId,orderId,orderTime,orderAmount,dealId,bankDealId,dealTime,payAmount,fee,payResult,errCode,signMsg,ext1,ext2)
	
	''接着进行支付结果判断
	Select Case payResult	      
		  case "10"
			
			'*  
			' 商户网站逻辑处理，比方更新订单支付状态为成功
			' 特别注意：只有UCase(signMsg)=UCase(merchantSignMsgVal)，且payResult=10，才表示支付成功！
			'*
			
			'报告给快钱处理结果，并提供将要重定向的地址。
			rtnOk=1
			rtnUrl="http://qp2.foxuc.net/PayKQ/show.asp?msg=success!"
			'在线充值
			Call CxGame.OnlinePay(orderId,ext1)
		 Case else

			rtnOk=1
			rtnUrl="http://qp2.foxuc.net/PayKQ/show.asp?msg=false!"

	End select

else

	rtnOk=1
	rtnUrl="http://qp2.foxuc.net/PayKQ/show.asp?msg=error!"

End if





	'功能函数。将变量值不为空的参数组成字符串
	Function appendParam(returnStr,paramId,paramValue)

		If returnStr <> "" Then
			If paramValue <> "" then
				returnStr=returnStr&"&"&paramId&"="&paramValue
			End if
		Else 
			If paramValue <> "" then
				returnStr=paramId&"="&paramValue
			End if
		End if
		
		appendParam=ReturnStr

	End Function
	'功能函数。将变量值不为空的参数组成字符串。结束


'以下报告给快钱处理结果，并提供将要重定向的地址
%>
<result><%=rtnOk %></result><redirecturl><%=rtnUrl%></redirecturl>