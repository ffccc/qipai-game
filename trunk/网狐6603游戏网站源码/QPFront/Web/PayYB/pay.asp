<%@LANGUAGE="VBSCRIPT" CODEPAGE="936"%>
<%Session.CodePage=936 %>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=gb2312" />
<title>�ױ�֧�� - ���Ͽ��� | ����������ѡ�����̣�֧�����ױ��������ֿɿ���</title>
<meta name="description" content="�ױ�֧����Ϊ���������ڷ����ṩ�̣��빤�̱��չ����̱��չ񡢽��豣�չ�ũҵ���չ��������չ�ȶ�ҹ� �ڱ��չ�VISA��MasterCard�⿨��֯���ܺ�����Ϊ���˿ͻ��ṩ����֧�����绰֧���������˻���Ʒ��� Ϊ��ҵ�̻��ṩ���չ�����֧�������մ�����ί�н����Լ�B2Bת��ȶ�����ֵҵ��">
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
		  	<td colspan="2" bgcolor="#CEE7BD">�ױ�֧����Ʒͨ��֧���ӿ���ʾ��</td>
		  </tr>
			<form method="get" action="req.asp" targe="_blank">		
		  <tr>
		  	<td align="left" width="30%">&nbsp;&nbsp;�̻�������</td>
		  	<td align="left">&nbsp;&nbsp;<input size="50" type="text" name="p2_Order" id="p2_Order" value="<%=Request("OrderNo") %>" readonly /></td>
      </tr>
		  <tr>
		  	<td align="left">&nbsp;&nbsp;֧�����</td>
		  	<td align="left">&nbsp;&nbsp;<input size="50" type="text" name="p3_Amt" id="p3_Amt" value="<%=Request("CardPrice") %>" readonly/>&nbsp;<span style="color:#FF0000;font-weight:100;">*</span></td>
      </tr>
		  <tr style="display:none">
		  	<td align="left">&nbsp;&nbsp;��Ʒ����</td>
		  	<td align="left">&nbsp;&nbsp;<input size="50" type="text" name="p5_Pid" id="p5_Pid" value=""/></td>
      </tr>
		  <tr style="display:none">
		  	<td align="left">&nbsp;&nbsp;��Ʒ����</td>
		  	<td align="left">&nbsp;&nbsp;<input size="50" type="text" name="p6_Pcat" id="p6_Pcat" value="producttype"/></td>
      </tr>
		  <tr style="display:none">
		  	<td align="left">&nbsp;&nbsp;��Ʒ����</td>
		  	<td align="left">&nbsp;&nbsp;<input size="50" type="text" name="p7_Pdesc" id="p7_Pdesc"  value="productdesc"/></td>
      </tr>
		  <tr style="display:none">
		  	<td align="left">&nbsp;&nbsp;����֧���ɹ����ݵĵ�ַ</td>
		  	<td align="left">&nbsp;&nbsp;<input size="50" type="text" name="p8_Url" id="p8_Url" value="http://localhost:6061/PayYB/callBack.asp" />&nbsp;<span style="color:#FF0000;font-weight:100;">*</span></td>
      </tr>
		  <tr>
		  	<td align="left">&nbsp;&nbsp;�û���</td>
		  	<td align="left">&nbsp;&nbsp;<input size="50" type="text" name="pa_MP" id="pa_MP"  value="<%=Request("Accounts") %>" readonly/></td>
      </tr>
		  
		  <tr style="display:none">
		  	<td align="left">&nbsp;&nbsp;֧��ͨ������</td>
		  	<td align="left">&nbsp;&nbsp;<input type="text" name="pd_FrpId" /><!--֧��ͨ���������ױ�֧����Ʒ(HTML��)ͨ�ýӿ�ʹ��˵����-->
		  	</td>
      </tr>
		  <tr>
		  	<td align="left">&nbsp;</td>
		  	<td align="left">&nbsp;&nbsp;<input type="submit" value="����֧��" /></td>
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