<!--#include file="../../CommonFun.asp"-->
<!--#include file="../../GameConn.asp"-->
<!--#include file="../../md5.asp"-->
<!--#include file="function.asp"-->

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" >
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <title></title>
    <link href="../../Css/layout.css" rel="stylesheet" type="text/css" />
    <script type="text/javascript" src="../../Js/common.js"></script>   
    <script type="text/javascript" src="../../Js/comm.js"></script>   
    <script type="text/javascript" src="../../Js/Check.js"></script>
</head>
<%
    If CxGame.GetRoleValue("200",trim(session("AdminName")))<"1" Then
    response.redirect "/Index.asp"   
    End If
%>
<body>
    <!-- 头部菜单 Start -->
    <table width="100%" border="0" cellpadding="0" cellspacing="0" class="title">
        <tr>
            <td width="19" height="25" valign="top"  class="Lpd10"><div class="arr"></div></td>
            <td width="1232" height="25" valign="top" align="left">你当前位置：充值系统 - 实卡管理</td>
        </tr>
    </table>
    <!-- 头部菜单 End -->
    <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="Tmg7">
        <tr>
            <td height="28">
                <ul>
                    <li class="tab2" onclick="openWindowSelf('LivcardList.asp')">会员卡管理</li>
	                <li class="tab1">会员卡生成</li>
	                <li class="tab2" onclick="openWindowSelf('LivcardStat.asp')">库存统计</li>
	                <li class="tab2" onclick="openWindowSelf('LivcardGlobal.asp')">类型管理</li>
                </ul>
            </td>
        </tr>
    </table> 
    <% 
        Call ConnectGame(QPTreasureDB)
        Select case lcase(Request("action"))
            case "create"
            Call CreateCard()
            case else
            Call Main()
        End Select
        Call CloseGame()       
        
        Sub Main()   
            Dim ArrayList
            ArrayList = CxGame.GetCardTypeList 
        
    %>
    <script type="text/jscript">
        function GetCardInfo(obj)
        {
            document.myFormInfo.in_CardGold.value = obj.split(',')[1];
            document.myFormInfo.in_MemberDays.value = obj.split(',')[2];
            document.myFormInfo.in_CardPrice.value = obj.split(',')[3]
        }
        
        function CheckFormInfo()
        {
            var count = document.myFormInfo.in_Count.value.trim();
            var cardGold = document.myFormInfo.in_CardGold.value.trim();
            var memberDays = document.myFormInfo.in_MemberDays.value.trim();
            var cardPrefix = document.myFormInfo.in_CardPrefix.value.trim();
            var cardLength = document.myFormInfo.in_CardLength.value.trim();
            var pwdLength= document.myFormInfo.in_PwdLength.value.trim();
            var validDate = document.myFormInfo.in_ValidDate.value.trim();
            if(count==""||IsPositiveInt(count)==false)
            {
                alert("生成数量不能为空或非数字字符或者超过允许大小！");
                return false;
            }   
            else
            {
                if(count==0)
                {
                    alert("生成数量必须大于0！");
                    return false;
                }
                else if(count>2000)
                {
                    alert("生成数量不可以超过2000！");
                    return false;
                }
            }         
            if(cardGold==""||IsPositiveInt(cardGold)==false)
            {
                alert("赠送金币不能为空或非数字字符或者超过允许大小！");
                return false;
            }
            if(memberDays==""||IsPositiveInt(memberDays)==false)
            {
                alert("会员天数不能为空或非数字字符或者超过允许大小！");
                return false;
            }  
            if(validDate!=""&&!IsPositiveInt(validDate))
            {
                alert("有效天数输入非法或者超过允许大小！");
                return false;
            }       
            if(cardLength==""||IsPositiveInt(cardLength)==false)
            {
                alert("卡号长度不能为空或非数字字符或者超过允许大小！");
                return false;
            }
            else
            {
                if(cardLength<15)
                {
                    alert("卡号长度长度必须大于等于15小于32！");
                    return false;
                }
                if(cardLength>31)
                {
                    alert("卡号长度长度必须大于等于15小于32！");
                    return false;
                }
            }
            if(pwdLength==""||IsPositiveInt(pwdLength)==false)
            {
                alert("密码长度不能为空或非数字字符！");
                return false;
            }
            else
            {
                if(pwdLength<8)
                {
                    alert("密码长度必须大于等于8！");
                    return false;
                }
                if(pwdLength>32)
                {
                    alert("密码长度必须大于等于8小于33！");
                    return false;
                }                
            }
        }
    </script>
    <form name="myFormInfo" method="post" action="?action=create" onsubmit="return CheckFormInfo()">
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
                <td class="titleOpBg Lpd10">
                    <input type="reset" value="重置" class="btn wd1" />                              
                    <input class="btnLine" type="button" />  
                    <input type="submit" value="生成会员卡" class="btn wd3" />      
                </td>
            </tr>
        </table>
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="listBg2">
            <tr>
                <td height="35" colspan="2" class="f14 bold Lpd10 Rpd10"><div class="hg3  pd7">基本信息</div></td>
            </tr>
            <tr>
	            <td class="listTdLeft">会员卡类型：</td>
	            <td>
	                <select name="in_CardTypeID" style="width:157px;" onchange="GetCardInfo(this.value)">
			        <% 
                        Dim i
                        ArrayList = CxGame.GetCardTypeList
                        For i=0 To Ubound(ArrayList)                                                     
                    %>
                    <option value="<%=ArrayList(i,0) %>,<%=ArrayList(i,2) %>,<%=ArrayList(i,3) %>,<%=ArrayList(i,4) %>"><%=ArrayList(i,1) %></option>
                    <%                      
                        Next  
                        'Set ArrayList = nothing
                    %>
			        </select>  
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">生成数量：</td>
	            <td>
	                <input name="in_Count" type="text" class="text" />                     
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">赠送金币：</td>
	            <td>
	                <input name="in_CardGold" type="text" class="text" value="<%=ArrayList(0,2) %>"/>
				    <input name="in_CardPrice" type="hidden" value="<%=ArrayList(0,4) %>" />                    
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">会员天数：</td>
	            <td>
	                <input name="in_MemberDays" type="text" class="text" value="<%=ArrayList(0,3) %>"/>                                 
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">使用范围：</td>
	            <td>
	                <select name="in_UseRange" style="width:157px;">
	                <option value="0">全部用户</option>
	                <option value="1">新注册用户</option>
	                <option value="2">第一次充值用户</option>
	                </select>
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">销售商：</td>
	            <td>
	                <input name="in_SalesPerson" type="text" class="text"/>
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">有效天数：</td>
	            <td>
	                <input name="in_ValidDate" type="text" class="text"/>
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">备注：</td>
	            <td>
	                <input name="in_NoteInfo" type="text" class="text" style="width:300px;" value="管理员【<%=AdminName %>】 于 <%=Date() %> 生成" />   
	            </td>
	        </tr>
	        <tr>
                <td height="35" colspan="2" class="f14 bold Lpd10 Rpd10"><div class="hg3  pd7">卡号规则</div></td>
            </tr>
            <tr>
	            <td class="listTdLeft">开始字符：</td>
	            <td>
	                <input name="in_CardPrefix" type="text" class="text" value="P" />&nbsp;&nbsp;<span class="hui6">开始字符可为空</span>     
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">卡号长度：</td>
	            <td>
	                <input name="in_CardLength" type="text" class="text" value="15" />&nbsp;&nbsp;<span class="hui6">卡号长度必须大于等于15小于32</span>  
	            </td>
	        </tr>
	        <tr>
                <td height="35" colspan="2" class="f14 bold Lpd10 Rpd10"><div class="hg3  pd7">卡密规则</div></td>
            </tr>
            <tr>
	            <td class="listTdLeft">密码字符类型：</td>
	            <td>
	                <input name="in_PasswdType" id="in_PasswdType1" type="checkbox" value="1" checked="checked" /><label for="in_PasswdType1">数字</label> 
                    <input name="in_PasswdType" id="in_PasswdType2" type="checkbox" value="2" /><label for="in_PasswdType2">含小写字母</label> 
                    <input name="in_PasswdType" id="in_PasswdType3" type="checkbox" value="4" /><label for="in_PasswdType3">含大写字母</label>  
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">密码长度：</td>
	            <td>
	                <input name="in_PwdLength" type="text" class="text" value="8" />&nbsp;&nbsp;<span class="hui6">密码长度必须大于等于8小于33</span> 
	            </td>
	        </tr>
        </table>
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
                <td class="titleOpBg Lpd10">
                    <input type="reset" value="重置" class="btn wd1" />                              
                    <input class="btnLine" type="button" />  
                    <input type="submit" value="生成会员卡" class="btn wd3" />      
                </td>
            </tr>
        </table>
    </form>
    <% 
        End Sub
        
        '生成会员卡
        Sub CreateCard()
            Dim cardTypeID,cardPrice,count,cardGold,memberDays,noteInfo,cardPrefix,cardLength,pwdType,pwdTypeArray,RL_i,pwdLength,useRange,salesPerson,validDate
            cardTypeID = Split(Request("in_CardTypeID"),",")(0)
            cardPrice = Split(Request("in_CardTypeID"),",")(3)
            count = CxGame.GetInfo(1,"in_Count")
            cardGold = CxGame.GetInfo(1,"in_CardGold")
            memberDays = CxGame.GetInfo(1,"in_MemberDays")
            noteInfo = CxGame.GetInfo(0,"in_NoteInfo")
            cardPrefix = CxGame.GetInfo(0,"in_CardPrefix")
            cardLength = CxGame.GetInfo(0,"in_CardLength")
            pwdTypeArray = Split(Request("in_PasswdType"),",")
            pwdType=0
            For RL_i=0 To UBound(pwdTypeArray)
                pwdType = pwdType Or pwdTypeArray(RL_i)
            Next
            pwdLength = CxGame.GetInfo(0,"in_PwdLength")
            '扩展字段
            useRange = CxGame.GetInfo(0,"in_UseRange")
            salesPerson = CxGame.GetInfo(0,"in_SalesPerson")
            validDate = CxGame.GetInfo(0,"in_ValidDate")
            Call LivcardCreate(cardTypeID,cardPrice,count,cardGold,memberDays,noteInfo,cardPrefix,cardLength,pwdType,pwdLength,useRange,salesPerson,validDate)
            response.Redirect("LivcardList.asp")
        End Sub        
    %>  
</body>
</html>
