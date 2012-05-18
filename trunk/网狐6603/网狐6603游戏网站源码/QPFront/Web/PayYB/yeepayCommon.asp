<!-- #include file="merchantProperties.asp" -->
<%
'**************************************************
'* @Description �ױ�֧����Ʒͨ��֧���ӿڷ��� 
'* @V3.0
'* @Author rui.xin
'**************************************************

	Dim reqURL_onLine
	Dim reqURL_QryOrd
	Dim reqURL_RefOrd
	Dim p0_Cmd
	Dim p9_SAF
	Dim sbOld

	'Asp������������ע�����Hmac-Md5-API.dll
	Set mctSDK = Server.CreateObject("HmacMd5API.HmacMd5Com")

	
Function HTMLcommom(p1_MerId,p2_Order,p3_Amt,p4_Cur,p5_Pid,p6_Pcat,p7_Pdesc,p8_Url,pa_MP,pd_FrpId,pr_NeedResponse)
	
	'	����֧�������ַ,�������
	'reqURL_onLine = "https://www.yeepay.com/app-merchant-proxy/node"    '	��ʽ��ַ
	reqURL_onLine = "http://tech.yeepay.com:8080/robot/debug.action"    '	����ʹ��
	'����ʱ����ʹ�÷������ṩ�Ĳ����̻���ź���Կ�������̻���ż���Կ���������򡣣�
	'1.��֧��/��ѯ/�˿������͵��õ�ַ��
	'2.��Ӧ�ûὫ���������һ�����������������⣬��ᱨ����ϸ��Ϣ��
	'3.�����������ȷ���󣬸�Ӧ�ý�����һ��ģ���֧���ɹ����ݣ�������ʹ�ø����ݲ���֧������Ľ��ճ���

	' ҵ������
	''����֧�����󣬹̶�ֵ ��Buy�� .	
	p0_Cmd = "Buy"
	
	'	�ͻ���ַ
	''Ϊ��1��: ��Ҫ�û����ͻ���ַ�����ױ�֧��ϵͳ;Ϊ��0��: ����Ҫ��Ĭ��Ϊ ��0��.
	p9_SAF = "0"				'��Ҫ��д�ͻ���Ϣ 0������Ҫ  1:��Ҫ
	
	sbOld  = ""

	'	����ǩ������һ�������ĵ��б�����ǩ��˳�����
	sbOld = sbOld + p0_Cmd
	sbOld = sbOld + p1_MerId
	sbOld = sbOld + p2_Order
	sbOld = sbOld + CStr(p3_Amt)		
	sbOld = sbOld + p4_Cur					
	sbOld = sbOld + p5_Pid
	sbOld = sbOld + p6_Pcat
	sbOld = sbOld + p7_Pdesc
	sbOld = sbOld + p8_Url
	sbOld = sbOld + p9_SAF
	sbOld = sbOld + pa_MP
	sbOld = sbOld + pd_FrpId
	sbOld = sbOld + pr_NeedResponse
	'Call logStr(logName,p2_Order, sbOld)
	HTMLcommom = mctSDK.HmacMd5(sbOld,merchantKey) 
	
End Function


Function getCallbackHmacString(r0_Cmd,r1_Code,r2_TrxId,r3_Amt,r4_Cur,r5_Pid,r6_Order,r7_Uid,r8_MP,r9_BType)

	'ȡ��ǩ������ǰ���ַ�����һ�������ĵ��б�����ǩ��˳�����
	sbOld = sbOld + CStr(p1_MerId)
	sbOld = sbOld + r0_Cmd
	sbOld = sbOld + r1_Code
	sbOld = sbOld + r2_TrxId
	sbOld = sbOld + r3_Amt
	sbOld = sbOld + r4_Cur
	sbOld = sbOld + r5_Pid
	sbOld = sbOld + r6_Order
	sbOld = sbOld + r7_Uid
	sbOld = sbOld + r8_MP
	sbOld = sbOld + r9_BType
	'Call logStr(logName,r6_Order, sbOld)
	getCallbackHmacString = mctSDK.HmacMd5(sbOld,merchantKey)
	
End Function


Function CheckHmac(r0_Cmd,r1_Code,r2_TrxId,r3_Amt,r4_Cur,r5_Pid,r6_Order,r7_Uid,r8_MP,r9_BType,hmac)
	if(hmac=getCallbackHmacString(r0_Cmd,r1_Code,r2_TrxId,r3_Amt,r4_Cur,r5_Pid,r6_Order,r7_Uid,r8_MP,r9_BType)) Then
		CheckHmac = True
	ELSE
		CheckHmac = False
	END IF
End Function


'ȡ������֧�����������еĲ���
Sub getCallBackValue(ByRef r0_Cmd,ByRef r1_Code,ByRef r2_TrxId,ByRef r3_Amt,ByRef r4_Cur,ByRef r5_Pid,ByRef r6_Order,ByRef r7_Uid,ByRef r8_MP,ByRef r9_BType,ByRef hmac,ByRef rb_BankId,ByRef ro_BankOrderId,ByRef rp_PayDate,ByRef rq_CardNo,ByRef ru_Trxtime)
	r0_Cmd		=	Request.QueryString("r0_Cmd")
	r1_Code		=	Request.QueryString("r1_Code")
	r2_TrxId	=	Request.QueryString("r2_TrxId")
	r3_Amt		=	Request.QueryString("r3_Amt")
	r4_Cur		=	Request.QueryString("r4_Cur")
	r5_Pid		=	Request.QueryString("r5_Pid")
	r6_Order	=	Request.QueryString("r6_Order")
	r7_Uid		=	Request.QueryString("r7_Uid")
	r8_MP			=	Request.QueryString("r8_MP")
	r9_BType	=	Request.QueryString("r9_BType") 	
	hmac			=	Request.QueryString("hmac")
	rb_BankId		=	Request.QueryString("rb_BankId")
	ro_BankOrderId	=	Request.QueryString("ro_BankOrderId")
	rp_PayDate		=	Request.QueryString("rp_PayDate")
	rq_CardNo		=	Request.QueryString("rq_CardNo")
	ru_Trxtime		=	Request.QueryString("ru_Trxtime")
End Sub


'ȡ�ö�����ѯ�Ľ��
Function queryOrdDetail(p2_Order)

	'������ѯ�����ַ,�������
	'reqURL_QryOrd	= "https://www.yeepay.com/app-merchant-proxy/command"    '	��ʽ��ַ			
	reqURL_QryOrd = "http://tech.yeepay.com:8080/robot/debug.action"    '	����ʹ��
	'����ʱ����ʹ�÷������ṩ�Ĳ����̻���ź���Կ�������̻���ż���Կ���������򡣣�
	'1.��֧��/��ѯ/�˿������͵��õ�ַ��
	'2.��Ӧ�ûὫ���������һ�����������������⣬��ᱨ����ϸ��Ϣ��
	'3.�����������ȷ���󣬸�Ӧ�ý�����һ��ģ���֧���ɹ����ݣ�������ʹ�ø����ݲ���֧������Ľ��ճ���
	
	'������ѯ���󣬹̶�ֵ"QueryOrdDetail"					
	p0_Cmd					= "QueryOrdDetail"																									

	'���м��ܴ�����һ����������˳�����
	sbOld =	""
	sbOld = sbOld + p0_Cmd					'����ҵ������
	sbOld = sbOld + p1_MerId				'�����̻����
	sbOld = sbOld + p2_Order				'�����̻�������
	
	hmac	= ""
	Call logStr(logName,p2_Order, sbOld)
	hmac	= mctSDK.HmacMd5(sbOld,merchantKey)
	
	'���м��ܴ�����һ����������˳�����
	sFormString = ""
	sFormString = sFormString + "p0_Cmd="		 + p0_Cmd			'����ҵ������
	sFormString = sFormString + "&p1_MerId=" + p1_MerId		'�����̻����
	sFormString = sFormString + "&p2_Order=" + Server.URLEncode(p2_Order)		'�����̻�������
	sFormString = sFormString + "&hmac="		 + hmac				'����ǩ������
	
  set objHttp = Server.CreateObject("MSXML2.ServerXMLHTTP")
	objHttp.open "POST", reqURL_QryOrd , false
	objHttp.setRequestHeader "Content-type", "application/x-www-form-urlencoded"
	objHttp.Send sFormString
   
  if (objHttp.status <> 200 ) then
	'HTTP ������
		response.Write("Status="&objHttp.status)
		response.end
	else
		
		strCallBack = objHttp.responseText
		aryCallBack = Split(strCallBack,vbLf)
		
		For i = LBound(aryCallBack) To UBound(aryCallBack) -1
			aryReturn = Split(aryCallBack(i),"=")
			If UBound(aryReturn) < 1 then
				returnMsg = aryCallBack(i)
			Else	
				sKey = aryReturn(0)
				sValue = aryReturn(1)
			
				If sKey = "r0_Cmd" Then								'ȡ��ҵ������
					r0_Cmd = sValue
				ElseIf sKey = "r1_Code" Then					'ȡ�ò�ѯ���
					r1_Code = sValue  	      	
				ElseIf sKey = "r2_TrxId" Then					'ȡ���ױ�֧��������ˮ��
					r2_TrxId = sValue  	      	
				ElseIf sKey = "r3_Amt" Then						'ȡ��֧�����
					r3_Amt = sValue
				ElseIf sKey = "r4_Cur" Then						'ȡ�ý��ױ���
					r4_Cur = sValue
				ElseIf sKey = "r5_Pid" Then						'ȡ����Ʒ����
					r5_Pid = URLDecode(sValue)
				ElseIf sKey = "r6_Order" Then					'ȡ���̻�������
					r6_Order = sValue
				ElseIf sKey = "r8_MP" Then						'ȡ���̻���չ��Ϣ
					r8_MP = URLDecode(sValue)
				ElseIf sKey = "rb_PayStatus" Then			'ȡ��֧��״̬
					rb_PayStatus = sValue
				ElseIf sKey = "rc_RefundCount" Then		'ȡ�����˿����
					rc_RefundCount = sValue
				ElseIf sKey = "rd_RefundAmt" Then			'ȡ�����˿���
					rd_RefundAmt = sValue
				ElseIf sKey = "hmac" Then							'ȡ��ǩ������
					hmac = sValue
				Else
					returnMsg = returnMsg + aryCallBack(i)   
				End If	
			End If	
		Next
		
		If trim(returnMsg)<> "" then
			queryOrdDetail = returnMsg
			exit Function
		End If
	
		'����У������ ȡ�ü���ǰ���ַ���
		sbOld	=	""
		sbOld = sbOld + r0_Cmd									'������Ϣ����
		sbOld = sbOld + r1_Code    							'���뷵��״̬    
		sbOld = sbOld + r2_TrxId								'����ƽ̨������ˮ��
		sbOld = sbOld + r3_Amt									'���뽻�׽��
		sbOld = sbOld + r4_Cur									'���뽻�ױ���
		sbOld = sbOld + r5_Pid									'������ƷID
		sbOld = sbOld + r6_Order								'����ȡ�ö�����
		sbOld = sbOld + r8_MP										'�����̼�����
		sbOld = sbOld + rb_PayStatus						'���뱣�չ��׶�����
		sbOld = sbOld + rc_RefundCount					'���뽻��״̬
		sbOld = sbOld + rd_RefundAmt						'���뽻��״̬
	
		sNewString = ""
	Call logStr(logName,r6_Order, sbOld)
		sNewString = mctSDK.HmacMd5(sbOld,merchantKey)

		'У������ȷ
		If sNewString = hmac Then   	
			If r1_Code = "1" Then
				'returnMsg	= returnMsg	& "������ѯ�ɹ�" 
				'returnMsg	= returnMsg	& "<BR><BR>������ "& r6_Order &" " 
				If rb_PayStatus = "SUCCESS" Then
					returnMsg	= returnMsg	&	"�ö�����֧��" 
				ElseIf rb_PayStatus = "CANCELED" Then
					returnMsg	= returnMsg	& "�ö�����ȡ��" 
				ElseIf rb_PayStatus = "AUTHORIZED" Then
					returnMsg	= returnMsg	& "�ö���δ֧��" 
				ElseIf rb_PayStatus = "INIT" Then
					returnMsg	= returnMsg	& "�ö���δ֧��" 
		  	End If
		  ElseIf r1_Code = "50" Then
				returnMsg	= returnMsg	& "�ö���������" 
			End If
		Else
			returnMsg	= returnMsg	& "������Ϣ���۸�!<BR>"
			returnMsg	= returnMsg	& "<BR>localhost: " & sNewString
			returnMsg	= returnMsg	& "<BR>YeePay: " & hmac
		End If
	End If
	
	queryOrdDetail = returnMsg

End Function


'ȡ�ö����˿�������������.
Function refundOrd(p2_Order,pb_TrxId,p3_Amt,p4_Cur,p5_Desc)

	'�����˿������ַ,�������
	'reqURL_RefOrd	= "https://www.yeepay.com/app-merchant-proxy/command"    '	��ʽ��ַ			
	reqURL_RefOrd = "http://tech.yeepay.com:8080/robot/debug.action"    '	����ʹ��
	'����ʱ����ʹ�÷������ṩ�Ĳ����̻���ź���Կ�������̻���ż���Կ���������򡣣�
	'1.��֧��/��ѯ/�˿������͵��õ�ַ��
	'2.��Ӧ�ûὫ���������һ�����������������⣬��ᱨ����ϸ��Ϣ��
	'3.�����������ȷ���󣬸�Ӧ�ý�����һ��ģ���֧���ɹ����ݣ�������ʹ�ø����ݲ���֧������Ľ��ճ���
	
	'�����˿����󣬹̶�ֵ"RefundOrd"					
	p0_Cmd				= "RefundOrd"																									

	'���м��ܴ�����һ����������˳�����
	sbOld =	""
	sbOld = sbOld + p0_Cmd					'����ҵ������
	sbOld = sbOld + p1_MerId				'�����̻����
	sbOld = sbOld + pb_TrxId				'�����ױ�֧��������ˮ��
	sbOld = sbOld + p3_Amt					'�����˿���
	sbOld = sbOld + p4_Cur					'���뽻�ױ���
	sbOld = sbOld + p5_Desc					'�����˿�˵��
	
	hmac	= ""
	Call logStr(logName,pb_TrxId, sbOld)
	hmac	= mctSDK.HmacMd5(sbOld,merchantKey)
	
	'���м��ܴ�����һ����������˳�����
	sFormString = ""
	sFormString = sFormString + "p0_Cmd="		 + p0_Cmd			'����ҵ������
	sFormString = sFormString + "&p1_MerId=" + p1_MerId		'�����̻����
	sFormString = sFormString + "&pb_TrxId=" + pb_TrxId		'�����ױ�֧��������ˮ��
	sFormString = sFormString + "&p3_Amt="	 + p3_Amt			'�����˿���
	sFormString = sFormString + "&p4_Cur="   + p4_Cur			'���뽻�ױ���
	sFormString = sFormString + "&p5_Desc="  + server.urlencode(p5_Desc)		'�����˿�˵��
	sFormString = sFormString + "&hmac="		 + hmac				'����ǩ������
	
  set objHttp = Server.CreateObject("MSXML2.ServerXMLHTTP")
	objHttp.open "POST", reqURL_RefOrd , false
	objHttp.setRequestHeader "Content-type", "application/x-www-form-urlencoded"
	objHttp.Send sFormString
   
  if (objHttp.status <> 200 ) then
	'HTTP ������
		response.Write("Status="&objHttp.status)
		response.end
	else
		
		strCallBack = objHttp.responseText
		aryCallBack = Split(strCallBack,vbLf)
		returnMsg = ""
		
		For i = LBound(aryCallBack) To UBound(aryCallBack) -1
			aryReturn = Split(aryCallBack(i),"=")
			If UBound(aryReturn) < 1 then
				returnMsg = aryCallBack(i)
			Else	
				sKey = aryReturn(0)
				sValue = aryReturn(1)
		
				If sKey = "r0_Cmd" Then								'ȡ��ҵ������
					r0_Cmd = sValue
				ElseIf sKey = "r1_Code" Then					'ȡ���˿�������
					r1_Code = sValue  	      	
				ElseIf sKey = "r2_TrxId" Then					'ȡ���ױ�֧��������ˮ��
					r2_TrxId = sValue  	      	
				ElseIf sKey = "r3_Amt" Then						'ȡ���˿���
					r3_Amt = sValue
				ElseIf sKey = "r4_Cur" Then						'ȡ�ý��ױ���
					r4_Cur = sValue
				ElseIf sKey = "hmac" Then							'ȡ��ǩ������
					hmac = sValue
				Else
					returnMsg = returnMsg + aryCallBack(i)   
				End If
			End If
		Next

		If trim(returnMsg)<> "" then
			refundOrd = returnMsg
			exit Function
		End If
		

		'����У������ ȡ�ü���ǰ���ַ���
		sbOld	=	""
		sbOld = sbOld + r0_Cmd									'������Ϣ����
		sbOld = sbOld + r1_Code    							'���뷵��״̬    
		sbOld = sbOld + r2_TrxId								'�����ױ�֧��������ˮ��
		sbOld = sbOld + r3_Amt									'�����˿���
		sbOld = sbOld + r4_Cur									'���뽻�ױ���
	
		sNewString = ""
	Call logStr(logName,r2_TrxId, sbOld)
		sNewString = mctSDK.HmacMd5(sbOld,merchantKey)

		'У������ȷ
		returnMsg	= ""
		If sNewString = hmac Then   	
			If CStr(r1_Code) = "1" Then
				returnMsg	= returnMsg	&	"�����˿������ύ�ɹ�!" 
		  Else
				returnMsg	= returnMsg	& "�����˿������ύʧ��!" 
			End If
		Else
			returnMsg	= returnMsg	& "������Ϣ���۸�!<BR>"
			returnMsg	= returnMsg	& "<BR>localhost: " & sNewString
			returnMsg	= returnMsg	& "<BR>YeePay: " & hmac
		End If
	End If
	
	refundOrd = returnMsg

End Function

'callback����֧�����������أ���������Ե�ͨѶ
'д�� onLine.log �����������Խӿ�
Sub createLog(ByRef returnMsg)
    filename = "./" & returnMsg & ".log"
    content = now()		&	","							&	request.ServerVariables("REMOTE_ADDR")
    content = content &	","							&	returnMsg
    content = content &	",�̻�������:["	& r6_Order & "]"
    content = content &	",֧�����:["		& r3_Amt & "]"
    content = content &	",ǩ������:["		& hmac & "]"
    
    Set FSO = Server.CreateObject("Scripting.FileSystemObject")   
    Set TS = FSO.OpenTextFile(Server.MapPath(filename),8,true) 
    TS.write content   
    TS.Writeline ""
    TS.Writeline ""
    Set TS = Nothing   
    Set FSO = Nothing   
End Sub
Sub logStr(ByRef returnMsg,Order, str)
    filename = "./" & returnMsg & ".log"
    content = now()		&	","							&	request.ServerVariables("REMOTE_ADDR")
    content = content &	",�̻�������:["	& Order & "]"
    content = content &	",str:[" & str & "]"
    Set FSO = Server.CreateObject("Scripting.FileSystemObject")   
    Set TS = FSO.OpenTextFile(Server.MapPath(filename),8,true) 
    TS.write content   
    TS.Writeline ""
    TS.Writeline ""
    Set TS = Nothing   
    Set FSO = Nothing   
End Sub

Function URLDecode(enStr)
	dim deStr,strSpecial
	dim c,i,v
	deStr=""
	strSpecial="!""#$%&'()*+,.-_/:;<=>?@[\]^`{|}~%"
	for i=1 to len(enStr)
    c=Mid(enStr,i,1)
    if c="%" then
      v=eval("&h"+Mid(enStr,i+1,2))
      if inStr(strSpecial,chr(v))>0 then
        deStr=deStr&chr(v)
        i=i+2
      else
        v=eval("&h"+ Mid(enStr,i+1,2) + Mid(enStr,i+4,2))
        deStr=deStr & chr(v)
        i=i+5
      end if
    else
      if c="+" then
        deStr=deStr&" "
      else
        deStr=deStr&c
      end if
    end if
  next
  URLDecode=deStr
End Function
%>