<!--#include file="Inc/CommonFun.asp" -->
<!--#include file="Inc/Conn.asp" -->
<!--#include file="Inc/md5.asp" -->

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" >
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <meta http-equiv="X-UA-Compatible" content="IE=EmulateIE7" />
    <meta name="Description" content="<%=CxGame.WebTitle %>" />
    <meta name="Keywords" content="<%=CxGame.WebTitle %>" />
    <meta name="rating" content="general" />
    <meta name="robots" content="all" />
    <title><%=CxGame.WebTitle %></title>
    <link rel="stylesheet" type="text/css" href="css/layout.css" />
    <link rel="stylesheet" type="text/css" href="css/global.css" />
    <link rel="stylesheet" type="text/css" href="Css/reg_layout.css" />
    <script src="JS/jquery_last.js" type="text/javascript"></script>
    <script src="JS/formValidator.js" type="text/javascript"></script>
    <script src="JS/formValidatorRegex.js" type="text/javascript"></script>
    <script src="JS/passwordStrength.js" type="text/javascript"></script>
    
    <link type="text/css" href="Js/lhgdialog/lhgdialog.css" rel="stylesheet"/>
    <script src="JS/lhgdialog/lhgcore.min.js" type="text/javascript"></script>
    <script src="JS/lhgdialog/lhgdialog.js" type="text/javascript"></script>
    
    <script type="text/javascript">
        $(document).ready(function(){
            //页面验证
            $.formValidator.initConfig({formid:"form1",alertmessage:false});
            $("#txtAccounts").formValidator({onshow:" ",onfocus:"用户名至少6~16个英文或中文字符",oncorrect:"该用户名可以注册"})
                .inputValidator({min:6,max:16,onerror:"你输入的用户名非法,请确认"})
                .regexValidator({regexp:"usernamecn",datatype:"enum",onerror:"用户名格式不正确"})
                .ajaxValidator({
                    isvalid:true,
                    type : "get",
                    url : "Inc/Ajax.asp?t=CheckUser",
                    datatype : "json",
                    success : function(data){	
                        if(data == 0)
			            {
                            return true;
			            }
                        else
			            {
                            return false;
			            }                        
		            },
		            error: function(){alert("服务器没有返回数据，可能服务器忙，请重试");},
		            onerror : "该用户名不可用，请更换用户名",
		            onwait : "正在对用户名进行合法性校验，请稍候..."
                });
            $("#txtNickname").formValidator({onshow:" ",onfocus:"昵称至少2~16个英文或中文字符"})
                .inputValidator({min:2,max:16,onerror:"你输入的昵称非法,请确认"})
                .regexValidator({regexp:"usernamecn",datatype:"enum",onerror:"昵称格式不正确"});
            $("#txtLogonPass").formValidator({onshow:" ",onfocus:"请输入密码，至少需要6位！"})
                .inputValidator({min:6,onerror:"你输入的密码非法,请确认"});
            $("#txtLogonPass2").formValidator({onshow:" ",onfocus:"请输入密码，至少需要6位！"})
                .compareValidator({desid:"txtLogonPass",operateor:"=",onerror:"两次密码不一致,请确认"}); 
            $("#txtInsurePass1").formValidator({onshow:" ",onfocus:"请输入银行密码，至少需要6位！"})
                .inputValidator({min:6,onerror:"你输入的银行密码非法,请确认"});
            $("#txtInsurePass2").formValidator({onshow:" ",onfocus:"请输入银行密码，至少需要6位！"})
                .compareValidator({ desid: "txtInsurePass1", operateor: "=", onerror: "两次密码不一致,请确认" });
            $("#txtPassPortID").formValidator({empty:true,onempty:" ",onshow:" ",onfocus:" "})
                .regexValidator({regexp:"idcard",datatype:"enum",onerror:"身份证号码不正确"});           
            $("#txtCode").formValidator({onshow:"请输入相同验证码",onfocus:"请输入相同验证码"}).inputValidator({min:4,max:4,onerror:"验证码输入错误,请确认"});
            $(":checkbox[id='chkAgree']").formValidator({tipid:"chkAgreeTip",onshow:"必须勾选",onfocus:"必须勾选"}).inputValidator({min:1,onerror:"请勾选《用户服务条款》"});
            //密码强度
            strongRankBind($("#txtLogonPass"), $("#lblLogonPassSafeRank"));
            strongRankBind($("#txtInsurePass"), $("#lblInsurePassSafeRank"));
            //弹出页面
            J('#btnSwitchFace').dialog({ id: 'winUserfaceList', title: '更换头像', width:540,height:385, page: 'Inc/UserfacesList.asp',rang: true,cover: true });
            //验证码
            $("#ImageCheck").bind("click",function(){$("#ImageCheck").attr("src","Inc/Code_Img.asp?"+ Math.random())})
        })
    </script>
</head>
<body>
<%CxGame.Channel = "Member" %>
<!--#include file="Themes/Common_Header.asp" -->

<!--页面主体开始-->
<form id="form1" action="Register.asp" method="post">
<div class="main">
	<div class="regBody">
	  <div class="regTitle"></div>
	  <div class="regBg">
			<!--创建帐号开始-->
			<div class="title lan f14 bold">创建帐号</div>
			<div >
			<table width="100%" border="0" cellpadding="0" cellspacing="0" class="mtop13">
			  <tr>
				<td width="230" align="right" class="f14">用&nbsp;&nbsp;户&nbsp;名：</td>
				<td width="206"><input id="txtAccounts" name="txtAccounts" type="text" class="text width200" value="<%=CxGame.GetInfo(0,"txtAccounts") %>" /><span class="hong f14">&nbsp;*</span></td>
			    <td width="304"><span id="txtAccountsTip"></span></td>
			  </tr>
			  <tr>
				<td width="230" align="right" class="f14">昵&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;称：</td>
				<td width="206"><input id="txtNickname" name="txtNickname" type="text" class="text width200" value="<%=CxGame.GetInfo(0,"txtNickname") %>" /><span class="hong f14">&nbsp;*</span></td>
			    <td width="304"><span id="txtNicknameTip" class="tipsRight"></span></td>
			  </tr>
			  <tr>
				<td width="230" align="right" class="f14">形&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;象：</td>
				<td width="206">
				    <input id="inFaceID" name="inFaceID" type="hidden" value="<%=CxGame.GetInfo(1,"inFaceID") %>" />
				    <img id="picFace" src="/gamepic/face<%=CxGame.GetInfo(1,"inFaceID") %>.gif" alt="" />&nbsp;&nbsp;
				    <input id="btnSwitchFace" type="button" value="更换头像" class="bnt lan" />
				</td>
				<td width="304">&nbsp;</td>
			  </tr>
			  <tr>
				<td width="230" align="right" class="f14">性&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;别：</td>
				<td width="206">
                  	<select name="ddlGender" class="width206" tabindex="4">
                    <option value="0"<%If CxGame.GetInfo(1,"ddlGender")=0 Then %> selected="selected" <%End If %>>女性</option>
                    <option value="1"<%If CxGame.GetInfo(1,"ddlGender")=1 Then %> selected="selected" <%End If %>>男性</option>
                	</select>
				</td>
			    <td></td>
			  </tr>			  
			</table>
			</div>
			<!--创建帐号结束-->
			
			<!--登录密码开始-->
			<div class="title lan f14 bold mtop13">登录密码</div>
			<div >
			<table width="100%" border="0" cellpadding="0" cellspacing="0" class="mtop13">	
			  <tr>
				<td width="230" align="right" class="f14">安全等级：</td>
				<td width="206"><span class="safe0" id="lblLogonPassSafeRank"></span></td>
			    <td width="304"></td>
			  </tr>
			  <tr>
				<td align="right" class="f14">密&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;码：</td>
				<td><input id="txtLogonPass" name="txtLogonPass" type="password" class="text width200" value="<%=CxGame.GetInfo(0,"txtLogonPass") %>" /><span class="hong f14">&nbsp;*</span></td>
			    <td><span id="txtLogonPassTip"></span></td>
			  </tr>
			  <tr>
				<td align="right" class="f14">确认密码：</td>
				<td><input id="txtLogonPass2" type="password" class="text width200" value="<%=CxGame.GetInfo(0,"txtLogonPass") %>" /><span class="hong f14">&nbsp;*</span></td>
			    <td><span id="txtLogonPass2Tip"></span></td>
			  </tr>
			  <tr>
				<td width="230" align="right" class="f14">安全等级：</td>
				<td width="206"><span class="safe0" id="lblInsurePassSafeRank"></span></td>
			    <td width="304"></td>
			  </tr> 
			  <tr>
				<td align="right" class="f14">保险柜密码：</td>
				<td><input id="txtInsurePass1" name="txtInsurePass" type="password" class="text width200" value="<%=CxGame.GetInfo(0,"txtInsurePass1") %>" /><span class="hong f14">&nbsp;*</span></td>
			    <td><span id="txtInsurePass1Tip"></span></td>
			  </tr>
			  <tr>
				<td align="right" class="f14">确认密码：</td>
				<td><input id="txtInsurePass2" type="password" class="text width200" value="<%=CxGame.GetInfo(0,"txtInsurePass1") %>" /><span class="hong f14">&nbsp;*</span></td>
			    <td><span id="txtInsurePass2Tip"></span></td>
			  </tr> 			  
			</table>
			</div>
			<!--登录密码结束-->	
			
			<!--推荐人开始-->
			<div class="title lan f14 bold mtop13">推荐人</div>
			<div >
			<table width="100%" border="0" cellpadding="0" cellspacing="0" class="mtop13">	
			  <tr>
			    <td width="220" align="right" class="f14">推&nbsp;&nbsp;荐&nbsp;人：</td>
			    <td width="206"><input name="txtSpreader" type="text" class="text width200" value="<%=CxGame.GetInfo(0,"txtSpreader") %>" /></td>
			    <td width="304"></td>
		      </tr>	  	
		      <tr>
			    <td width="220" align="right" class="f14">真实姓名：</td>
			    <td width="206"><input name="txtCompellation" type="text" class="text width200" value="<%=CxGame.GetInfo(0,"txtCompellation") %>" /></td>
			    <td width="304"></td>
		      </tr>	
		      <tr>
			    <td width="220" align="right" class="f14">身份证号：</td>
			    <td width="206"><input id="txtPassPortID" name="txtPassPortID" type="text" class="text width200" value="<%=CxGame.GetInfo(0,"txtPassPortID") %>" /></td>
			    <td><span id="txtPassPortIDTip"></span></td>
		      </tr>	  
			  <tr>
			    <td align="right" class="f14">验&nbsp;&nbsp;证&nbsp;码：</td>
			    <td colspan="2">
			        <input id="txtCode" name="txtCode" type="text" class="text width110" style="width:134px;" />
			        <img src="Inc/Code_Img.asp" id="ImageCheck" style="cursor:pointer;border: 1px solid #333333;vertical-align:text-bottom;" title="点击更换验证码图片!"/>
			        <span id="txtCodeTip"></span>
			    </td>
		      </tr>
			</table>
			</div>
			<!--推荐人结束-->
			
			<!--服务条款开始-->
			<div class="title lan f14 bold mtop13">服务条款</div>
			<div >
			<table width="100%" border="0" cellpadding="0" cellspacing="0" class="mtop13 mbottom10">
			  <tr>
				<td width="230" align="right" class="f14">&nbsp;</td>
				<td><input id="chkAgree" checked="checked" type="checkbox" name="chkAgree" />
                我已阅读并同意 <a href="/Agreement.asp" target="_blank">用户服务条款</a>
                <span id="chkAgreeTip"></span>
                </td>
			  </tr>
			  <tr>
				<td align="right" class="f14">&nbsp;</td>
				<td class="padding15">
				    <input type="submit" value="注 册" class="btn1" />&nbsp;&nbsp;
				    <input type="reset" value="重 置" class="btn1" />
				    <input name="reg" type="hidden" id="reg" value="true" />
				    <%CxGame.UserReg()%>
				</td>
		      </tr>
			</table>
			</div>
			<!--服务条款结束-->			
			<div class="clear"></div>
		</div>
		<div class="regBottom"><div class="clear"></div></div>
		<div class="clear"></div>
	</div>
	<div class="clear"></div>
</div>
</form>
<!--页面主体结束-->
<!--#include file="Themes/Common_Footer.asp" -->
</body>
</html>
