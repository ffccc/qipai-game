<!-- #include file="yeepayCommon.asp" -->
<%
'**************************************************
'* @Description �ױ�֧��������ѯ�ӿڷ��� 
'* @V3.0
'* @Author rui.xin
'**************************************************

	Dim p2_Order
	Dim returnMsg

	'�̻������ţ��˲�������.
	p2_Order	= request("p2_Order")																									

	'ȡ�ö�����ѯ��������.
	returnMsg = queryOrdDetail(p2_Order)
	
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
		  	<td colspan="2" bgcolor="#CEE7BD">�ױ�֧��������ѯ�ӿ���ʾ��</td>
		  </tr>
		  <tr>
		  	<td align="left" width="30%">&nbsp;&nbsp;�̻�������</td>
		  	<td align="left">&nbsp;&nbsp;<input type="text" name="p2_Order" id="p2_Order" value="<%= p2_Order %>" /></td>
      </tr>
		  <tr>
		  	<td align="left" height="20">&nbsp;&nbsp;��ѯ���</td>
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