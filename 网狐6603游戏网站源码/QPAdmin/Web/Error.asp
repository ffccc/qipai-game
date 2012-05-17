<html xmlns="http://www.w3.org/1999/xhtml">
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8">
    <title>登陆出错！</title>
    <link href="Css/layout.css" rel="stylesheet" type="text/css" />    
</head>
<body>
    <table width="100%" height="100%" border="0" cellpadding="0" cellspacing="0">
        <tr height="20%">
            <td>
                &nbsp;</td>
        </tr>
        <tr>
            <td align="center" valign="top">
                <table width="200" border="0" cellpadding="0" cellspacing="0">
                    <tr>
                        <td height="48" valign="middle" background="images/MessageIcon/MessageHead.gif">
                            <table width="100%" border="0" cellspacing="0" cellpadding="0">
                                <tr>
                                    <td width="7%" height="8">
                                    </td>
                                    <td width="93%" align="left">
                                    </td>
                                </tr>
                                <tr>
                                    <td>
                                        &nbsp;</td>
                                    <td align="left">
                                        <span style="font-weight: bold; font-size: 18px;">错误信息！</span></td>
                                </tr>
                            </table>
                        </td>
                    </tr>
                    <tr>
                        <td background="images/MessageIcon/MessageBody.gif">
                            <table width="100%" style="table-layout: fixed;" border="0" cellspacing="0" cellpadding="0">
                                <tr>
                                    <td width="6%">
                                        &nbsp;</td>
                                    <td width="11%" height="120">
                                        <img src="images/MessageIcon/MessageError.gif"></td>
                                    <td width="77%" align="left">
                                        <table width="100%" border="0" cellspacing="10" cellpadding="0">
                                            <tr>
                                                <td style="word-break: break-all;font-size:12px;">
                                                    <%=Session("ErrMsg") %></td>
                                            </tr>
                                        </table>
                                    </td>
                                    <td width="6%">
                                        &nbsp;</td>
                                </tr>
                                <tr>
                                    <td>
                                        &nbsp;</td>
                                    <td>
                                        &nbsp;</td>
                                    <td align="left">
                                        <input type='button' id='Message_Button0' class="btn wd1" onclick="JavaScript:top.window.location.href='Login.asp';"
                                            value='返回' title='点击按钮重新输入！' />
                                    </td>
                                    <td>
                                        &nbsp;</td>
                                </tr>
                            </table>
                        </td>
                    </tr>
                    <tr>
                        <td>
                            <img src="images/MessageIcon/MessageEnd.gif" width="459" height="29" /></td>
                    </tr>
                </table>
            </td>
        </tr>
    </table>
</body>
</html>
