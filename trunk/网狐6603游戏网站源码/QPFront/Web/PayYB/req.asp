<!-- #include file="yeepayCommon.asp" -->
<%
'**************************************************
'* @Description �ױ�֧����Ʒͨ��֧���ӿڷ��� 
'* @V3.0
'* @Author rui.xin
'**************************************************

	Dim p2_Order
	Dim p3_Amt
	Dim p4_Cur
	Dim p5_Pid
	Dim p6_Pcat
	Dim p7_Pdesc
	Dim p8_Url
	Dim pa_MP
	Dim pd_FrpId
	Dim pr_NeedResponse
	Dim sNewString

	'	�̼������û�������Ʒ��֧����Ϣ.
	''�ױ�֧��ƽ̨ͳһʹ��GBK/GB2312���뷽ʽ,�������õ����ģ���ע��ת��
	
	'	�̻�������,ѡ��.
	''����Ϊ""���ύ�Ķ����ű����������˻�������Ψһ;Ϊ""ʱ���ױ�֧�����Զ�����������̻�������.
	p2_Order				=	request("p2_Order")
	
	'	֧�����,����.
	''��λ:Ԫ����ȷ����.
	p3_Amt					=	request("p3_Amt")
	
	'	���ױ���,�̶�ֵ"CNY".
	p4_Cur					=	"CNY"			
	
	'	��Ʒ����
	''����֧��ʱ��ʾ���ױ�֧���������Ķ�����Ʒ��Ϣ.
	p5_Pid					= request("p5_Pid")	
	
	'	��Ʒ����
	p6_Pcat					= request("p6_Pcat")
	
	'	��Ʒ����
	p7_Pdesc				=	request("p7_Pdesc")				
	
	'	�̻�����֧���ɹ����ݵĵ�ַ,֧���ɹ����ױ�֧������õ�ַ�������γɹ�֪ͨ.
	p8_Url					= request("p8_Url")		
	
	'	�̻���չ��Ϣ
	''�̻�����������д1K ���ַ���,֧���ɹ�ʱ��ԭ������.												
	pa_MP						= request("pa_MP")
	
	'	���չ����
	''Ĭ��Ϊ""�����ױ�֧������.��������ʾ�ױ�֧����ҳ�棬ֱ����ת�������չ�������֧��������һ��ͨ��֧��ҳ�棬���ֶο����ո�¼:���չ��б����ò���ֵ.			
	pd_FrpId				=	request("pd_FrpId")
	
	'	Ӧ�����
	''Ĭ��Ϊ"1": ��ҪӦ�����;
	pr_NeedResponse	= "1"				
	
	'����ǩ����������ǩ����
	hmac = HTMLcommom(p1_MerId,p2_Order,p3_Amt,p4_Cur,p5_Pid,p6_Pcat,p7_Pdesc,p8_Url,pa_MP,pd_FrpId,pr_NeedResponse)
	
%>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html>
<head>
<title>To YeePay Page</title>
</head>
<body onLoad="document.yeepay.submit()">
	<form name="yeepay" action="<%=reqURL_onLine%>" method="post">
		<input type="hidden" name="p0_Cmd"					value="<%=p0_Cmd%>">
		<input type="hidden" name="p1_MerId"				value="<%=p1_MerId%>">
		<input type="hidden" name="p2_Order"				value="<%=p2_Order%>">
		<input type="hidden" name="p3_Amt"					value="<%=p3_Amt%>">
		<input type="hidden" name="p4_Cur"					value="<%=p4_Cur%>">
		<input type="hidden" name="p5_Pid"					value="<%=p5_Pid%>">
		<input type="hidden" name="p6_Pcat"					value="<%=p6_Pcat%>">
		<input type="hidden" name="p7_Pdesc"				value="<%=p7_Pdesc%>">
		<input type="hidden" name="p8_Url"					value="<%=p8_Url%>">
		<input type="hidden" name="p9_SAF"					value="<%=p9_SAF%>">
		<input type="hidden" name="pa_MP"						value="<%=pa_MP%>">
		<input type="hidden" name="pd_FrpId"				value="<%=pd_FrpId%>">		
		<input type="hidden" name="pr_NeedResponse"	value="<%=pr_NeedResponse%>">
		<input type="hidden" name="hmac"						value="<%=hmac%>">
	</form>
</body>
</html>