<!-- #include file="yeepayCommon.asp" -->
<%
'**************************************************
'* @Description �ױ�֧�������˿�����ӿڷ��� 
'* @V3.0
'* @Author rui.xin
'**************************************************

	Dim pb_TrxId
	Dim p3_Amt
	Dim p4_Cur
	Dim p5_Desc
	Dim returnMsg

	' �ױ�֧��������ˮ��																					
	pb_TrxId	= request("pb_TrxId")
	
	' �˿���
	''��λ:Ԫ����ȷ����.
	p3_Amt		= request("p3_Amt")
	
	'	���ױ���,�̶�ֵ"CNY".
	p4_Cur		=	"CNY"			

	' �˿�˵��
	''��ϸ�����˿�ԭ�����Ϣ.
	p5_Desc		= request("p5_Desc")
	
	'ȡ�ö����˿���������.
	returnMsg = refundOrd(p2_Order,pb_TrxId,p3_Amt,p4_Cur,p5_Desc)
	
%>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<title>�ױ�֧�� - ���Ͽ��� | ����������ѡ�����̣�֧�����ױ��������ֿɿ���</title>
<meta name="description" content="�ױ�֧����Ϊ���������ڷ����ṩ�̣��빤�̱��չ����̱��չ񡢽��豣�չ�ũҵ���չ��������չ�ȶ�ҹ� �ڱ��չ�VISA��MasterCard�⿨��֯���ܺ�����Ϊ���˿ͻ��ṩ����֧�����绰֧���������˻���Ʒ��� Ϊ��ҵ�̻��ṩ���չ�����֧�������մ�����ί�н����Լ�B2Bת��ȶ�����ֵҵ��">
<meta http-equiv="Content-Type" content="text/html; charset=gb2312" />
<link href="css/yeepaytest.css" type="text/css" rel="stylesheet" />
</head>
	<body>
		<table width="50%" border="0" align="center" cellpadding="0" cellspacing="0" style="border:solid 1px #107929">
		  <tr>
		    <td><table width="100%" border="0" align="center" cellpadding="5" cellspacing="1">
		  </tr>
		  <tr>
		    <td height="30" align="left"><a href="http://www.yeepay.com/"><img src="http://www.yeepay.com/new-indeximages/yeepay-indexlogo.gif" alt="YeePay�ױ�֧�� ���µĶ�Ԫ��֧��ƽ̨ ��ȫ �ɿ� ��� ��������" width="141" height="47" border="0" /></a></td>
		  </tr>
		  <tr>
		  	<td height="30" colspan="2" bgcolor="#6BBE18"><span style="color: #FFFFFF">��л��ʹ���ױ�֧��ƽ̨</span></td>
		  </tr>
		  <tr>
		  	<td colspan="2" bgcolor="#CEE7BD">�ױ�֧�������˿�����ӿ���ʾ��</td>
		  </tr>
		  <tr>
		  	<td align="left" width="30%">&nbsp;&nbsp;�ױ�֧��������ˮ��</td>
		  	<td align="left">&nbsp;&nbsp;<input type="text" name="pb_TrxId" id="pb_TrxId" value="<%= pb_TrxId %>" /></td>
      </tr>
		  <tr>
		  	<td align="left" width="30%">&nbsp;&nbsp;�˿���(Ԫ)</td>
		  	<td align="left">&nbsp;&nbsp;<input type="text" name="p3_Amt" id="p3_Amt" value="<%= p3_Amt %>" /></td>
      </tr>
		  <tr>
		  	<td align="left" height="20">&nbsp;&nbsp;�����˿���������</td>
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