<!--#include file="../../CommonFun.asp"-->
<!--#include file="../../function.asp"-->
<!--#include file="../../GameConn.asp"-->
<!--#include file="../../conn.asp"-->
<!--#include file="../../md5.asp"-->
<!--#include file="../../Cls_Page.asp"-->
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" >
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <title></title>
    <link href="../../Css/layout.css" rel="stylesheet" type="text/css" />
    <script type="text/javascript" src="../../Js/common.js"></script>
    <script type="text/javascript" src="../../Js/comm.js"></script>
    <script type="text/javascript" src="../../Js/Calendar.js"></script>
    <script type="text/javascript" src="../../Js/Check.js"></script>
    <script type="text/javascript" src="../../Js/Sort.js"></script>
    <script type="text/javascript">
        function ShowDiv()
        {
            document.getElementById('divQuery').style.display="block";
        }
        function HideDiv()
        {
            document.getElementById('divQuery').style.display="none";
        }
        function CheckForm()
        {
            var startReDate = document.form2.ReStartDate.value;
            var endReDate = document.form2.ReEndDate.value;
            var startLoDate = document.form2.LoStartDate.value;
            var endLoDate = document.form2.LoEndDate.value;
            if(!isDate(startReDate))
            {
                if(startReDate!="")
                {
                    alert("注册开始时间非法！");
                    return false;
                }
            }
            if(!isDate(endReDate))
            {
                if(endReDate!="")
                {
                    alert("注册结束时间非法！");
                    return false;
                }
            }
            if(!isDate(startLoDate))
            {
                if(startLoDate!="")
                {
                    alert("登录开始时间非法！");
                    return false;
                }
            }
            if(!isDate(endLoDate))
            {
                if(endLoDate!="")
                {
                    alert("登录结束时间非法！");
                    return false;
                }
            }
        }
    </script>
    <style type="text/css">
        .querybox {width:500px; background:#caebfc;font-size:12px;line-height: 18px; text-align:left;
        border: 1px solid #066ba4;z-index:999; display:none;position: absolute; top:150px; left:200px;padding:5px;
        filter:progid:DXImageTransform.Microsoft.DropShadow(color=#9a8559,offX=1,offY=1,positives=true); }
    </style>
</head>
<%
    If CxGame.GetRoleValue("100",trim(session("AdminName")))<"1" Then
    response.redirect "/Index.asp"   
    End If
%>
<body>
    <!-- 头部菜单 Start -->
    <table width="100%" border="0" cellpadding="0" cellspacing="0" class="title">
        <tr>
            <td width="19" height="25" valign="top"  class="Lpd10"><div class="arr"></div></td>
            <td width="1232" height="25" valign="top" align="left">你当前位置：游戏用户 - 用户管理</td>
        </tr>
    </table>
    <!-- 头部菜单 End -->
    <form name="form1" action="">
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="titleQueBg">
            <tr>
                <td class="listTdLeft" style="width: 80px">
                    普通查询：
                </td>
                <td>
                    <input name="search" type="text" class="text" value="<%=CxGame.GetInfo(0,"search") %>" />
                    <select name="swhat" class="button">
                        <option value="1" selected="selected">按用户帐号</option>   
                        <option value="2" <% If CxGame.GetInfo(1,"swhat") = 2 Then %> selected="selected" <%End If %>>按游戏ID</option> 
                        <option value="3"<% If CxGame.GetInfo(1,"swhat") = 3 Then %> selected="selected" <%End If %>>按注册地址</option>
                        <option value="4"<% If CxGame.GetInfo(1,"swhat") = 4 Then %> selected="selected" <%End If %>>按登录地址</option> 
                        <option value="5"<% If CxGame.GetInfo(1,"swhat") = 5 Then %> selected="selected" <%End If %>>按注册机器</option> 
                        <option value="6"<% If CxGame.GetInfo(1,"swhat") = 6 Then %> selected="selected" <%End If %>>按登录机器</option> 
                    </select>
                    <input type="submit" value="查询" class="btn wd1" />
                    <input type="button" value="高级查询" class="btn wd2" onclick="ShowDiv()" /> 
                    <input type="button" value="所有用户" class="btn wd2" onclick="GetAllUsers()" />                       
                    
                </td>
            </tr>
        </table>
    </form>
    <div id="divQuery" class="querybox">
        <form name="form2" action="" onsubmit="return CheckForm()">
            <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="listBg2">
                <tr>
                    <td height="35" colspan="2" class="f14 bold Lpd10 Rpd10"><div class="hg3  pd7">基本查询</div></td>
                </tr>
                <tr>
	                <td class="listTdLeft" style="width:100px;">用户昵称：</td>
	                <td>
	                    <input name="in_Nickname" type="text" class="text wd4" />  
	                    <input name="chkNickName" id="chkNickName" type="checkbox" value="1"/><label for="chkNickName">模糊查询</label>                
	                </td>
	            </tr>
	            <tr>
	                <td class="listTdLeft" style="width:100px;">用户帐号：</td>
	                <td>
	                    <input name="in_Accounts" type="text" class="text wd4" />       
	                    <input name="chkAccounts" id="chkAccounts" type="checkbox" value="1" /><label for="chkAccounts">模糊查询</label>                           
	                </td>
	            </tr>
	            <tr>
                    <td height="35" colspan="2" class="f14 bold Lpd10 Rpd10"><div class="hg3  pd7">时间查询</div></td>
                </tr>
                <tr>
	                <td class="listTdLeft" style="width:100px;">注册日期：</td>
	                <td>
	                    <input name="ReStartDate" type="text" class="text" style="width:100px;" maxlength="10" value="<%=Year(Date())&"-"&Month(Date())&"-1" %>" /><img src="../../Images/btn_calendar.gif" onclick="calendar(ReStartDate)" style="cursor:pointer;vertical-align:middle"/> 
	                    至
	                    <input name="ReEndDate" type="text" class="text" style="width:100px;" maxlength="10" value="<%=Date() %>" /><img src="../../Images/btn_calendar.gif" onclick="calendar(ReEndDate)" style="cursor:pointer;vertical-align:middle"/>                                     
	                </td>
	            </tr>
	            <tr>
	                <td class="listTdLeft" style="width:100px;">登录日期：</td>
	                <td>
	                    <input name="LoStartDate" type="text" class="text" style="width:100px;" maxlength="10" value="<%=Year(Date())&"-"&Month(Date())&"-1" %>" /><img src="../../Images/btn_calendar.gif" onclick="calendar(LoStartDate)" style="cursor:pointer;vertical-align:middle"/> 
	                    至
	                    <input name="LoEndDate" type="text" class="text" style="width:100px;" maxlength="10" value="<%=Date() %>" /><img src="../../Images/btn_calendar.gif" onclick="calendar(LoEndDate)" style="cursor:pointer;vertical-align:middle"/>                                     
	                </td>
	            </tr>
	            <tr>
	                <td colspan="2" align="right">
    	                <input type="submit" value="查询"  class="btn wd1" />
    	                <input type="button" value="取消"  class="btn wd1" onclick="HideDiv()" />
    	                <input type="hidden" name="QueryType" value="1" />
	                </td>
	            </tr>
            </table>
        </form>
    </div>
    <% 
        Call ConnectGame(QPAccountsDB)
        Select case lcase(Request("action"))
            case "save"
            Call SaveUserInfo()
            case else
            Call Main()
        End Select
        Call CloseGame()       
        
        '删除
        Sub Delete(lID)
            Dim ID
            ID = lID
            GameConn.execute "Delete From AccountsInfo Where UserID="&lID
        End Sub
        
        '禁用解冻
        Sub Nullity(lID,typeValue)
            Dim ID
            ID = lID
            GameConn.execute "Update AccountsInfo Set Nullity="&typeValue&" Where UserID="&lID
        End Sub
        
        Sub Main()            
            Dim cidArray, lLoop,acType
            cidArray = Split(Request("cid"),",")
            acType = Request("action")
            For lLoop=0 To UBound(cidArray)
                Select case acType
                    case "del"
                        Call Delete(cidArray(lLoop))
                    case "jinyong"
                        Call Nullity(cidArray(lLoop),1)
                    case "huanyuan"
                        Call Nullity(cidArray(lLoop),0)
                End Select
            Next            
            
            Dim rs,nav,Page,i
            Dim lCount, queryCondition, OrderStr
            If Request.QueryString("orders")<>""And Request.QueryString("OrderType")<>"" Then
                If Request.QueryString("OrderType")<>"-102" Then
                    OrderStr=Request.QueryString("orders")&" Asc "
                End If
                If Request.QueryString("OrderType")<>"-101" Then
                    OrderStr=Request.QueryString("orders")&" Desc "
                End If
            Else 
                OrderStr = "UserID Asc"
            End If
            '查询条件
            If CxGame.GetInfo(0,"search")<>"" Then
                Select case Request("swhat")
                    case 1
                        queryCondition = " Accounts='"&CxGame.GetInfo(0,"search")& "'"	
                    case 2
                        queryCondition = " GameID='"&CxGame.GetInfo(1,"search")& "'"
                    case 3
                        queryCondition = " RegisterIP='"&CxGame.GetInfo(0,"search")& "'"
                    case 4
                        queryCondition = " LastLogonIP='"&CxGame.GetInfo(0,"search")& "'"
                    case 5
                        queryCondition = " RegisterMachine='"&CxGame.GetInfo(0,"search")& "'"
                    case 6
                        queryCondition = " LastLogonMachine='"&CxGame.GetInfo(0,"search")& "'"
                End Select
            Else
                queryCondition = " UserID=0"
            End If   
            
            If CxGame.GetInfo(0,"QueryType")<>"" Then
                queryCondition = " 1=1 "
                If CxGame.GetInfo(0,"in_Nickname")<>"" Then
                    If CxGame.GetInfo(0,"chkNickName")<>"" Then
                        queryCondition = queryCondition& " And Nickname like '%"&CxGame.GetInfo(0,"in_Nickname")&"%'"
                    Else
                        queryCondition = queryCondition&" And Nickname = '"&CxGame.GetInfo(0,"in_Nickname")&"'"
                    End If
                End If
                
                If CxGame.GetInfo(0,"in_Accounts")<>"" Then
                    If CxGame.GetInfo(0,"chkAccounts")<>"" Then
                        queryCondition = queryCondition& " And  Accounts like '%"&CxGame.GetInfo(0,"in_Accounts")&"%'"
                    Else
                        queryCondition = queryCondition&" And  Accounts = '"&CxGame.GetInfo(0,"in_Accounts")&"'"
                    End If
                End If 
                
                If CxGame.GetInfo(0,"ReStartDate")<>"" Then
                    queryCondition = queryCondition&" And RegisterDate >= '"&CxGame.GetInfo(0,"ReStartDate")&"' "
                End If 
                
                If CxGame.GetInfo(0,"ReEndDate")<>"" Then
                    queryCondition = queryCondition&" And RegisterDate <= '"&CxGame.GetInfo(0,"ReEndDate")&"' "
                End If 
                
                If CxGame.GetInfo(0,"LoStartDate")<>"" Then
                    queryCondition = queryCondition&" And LastLogonDate >= '"&CxGame.GetInfo(0,"LoStartDate")&"' "
                End If
                
                If CxGame.GetInfo(0,"LoEndDate")<>"" Then
                    queryCondition = queryCondition&" And LastLogonDate <= '"&CxGame.GetInfo(0,"LoEndDate")&"' "
                End If
                
                If Trim(queryCondition) = "1=1" Then
                    queryCondition = " UserID=0"
                End If
            End If
            
            If Request("action")="alluser" Then
                queryCondition = " 1=1"
            End If
            Dim fields        
            fields = "UserID,GameID,Accounts,Gender,Nickname,FaceID,Experience,LoveLiness,Present,MemberOrder,MasterOrder,RegisterDate,RegisterIP,GameLogonTimes,LastLogonDate,LastLogonIP,"
            fields = fields&"Nullity"         
            
            '==============================================================================================================
            '执行存储过程
                    
            Set Page = new Cls_Page      '创建对象
            Set Page.Conn = GameConn     '得到数据库连接对象        
            With Page
                .PageSize = GetPageSize				'每页记录条数
                .PageParm = "page"				'页参数
                '.PageIndex = 10				'当前页，可选参数，一般是生成静态时需要
	            .Database = "mssql"				'数据库类型,AC为access,MSSQL为sqlserver2000存储过程版,MYSQL为mysql,PGSQL为PostGreSql
	            .Pkey="UserID"					'主键
	            .Field=fields	'字段
	            .Table="AccountsInfo"			'表名
	            .Condition=queryCondition		'条件,不需要where
	            .OrderBy=OrderStr				'排序,不需要order by,需要asc或者desc
	            .RecordCount = 0				'总记录数，可以外部赋值，0不保存（适合搜索），-1存为session，-2存为cookies，-3存为applacation

	            .NumericJump = 5                '数字上下页个数，可选参数，默认为3，负数为跳转个数，0为显示所有
	            .Template = "总记录数：{$RecordCount} 总页数：{$PageCount} 每页记录数：{$PageSize} 当前页数：{$PageIndex} {$FirstPage} {$PreviousPage} {$NumericPage} {$NextPage} {$LastPage} {$InputPage} {$SelectPage}" '整体模板，可选参数，有默认值
	            .FirstPage = "首页"             '可选参数，有默认值
	            .PreviousPage = "上一页"        '可选参数，有默认值
	            .NextPage = "下一页"            '可选参数，有默认值
	            .LastPage = "尾页"              '可选参数，有默认值
	            .NumericPage = " {$PageNum} "   '数字分页部分模板，可选参数，有默认值
            End With
            
            rs = Page.ResultSet()       '记录集
            lCount = Page.RowCount()    '可选，输出总记录数
            nav = Page.Nav()            '分页样式
            
            Page = Null
            Set Page = Nothing
            '==============================================================================================================
    %>
    <script type="text/javascript">        
        function Operate(opType)
        {
            var opVal = document.myformList.in_optype
            if(!confirm("确定要执行选定的操作吗？"))
            {
                return;
            }
            else
            {
                var cid="";
                var cids = GelTags("input");                
                for(var i=0;i<cids.length;i++)
                {
                    if(cids[i].checked)
                    {
                        if(cids[i].name == "cid")
                            cid+=cids[i].value+",";
                    }
                }       
                if(cid=="")
                {
                    showError("未选中任何数据");
                    return;
                }
                //操作处理
                if(opType=="del")
                    opVal.value = "del";
                if(opType=="jinyong")
                    opVal.value = "jinyong";
                if(opType=="huanyuan")
                    opVal.value = "huanyuan";
            }
            document.myformList.action = "?action="+opVal.value;
            document.myformList.submit();
        }
        
        function GetAllUsers()
        {
            document.myformList.action = "?action=alluser"
            document.myformList.submit();
        }
        
        function GrantManager(opType)
        {
            var cid="";
            var cids = GelTags("input"); 
            for(var i=0;i<cids.length;i++)
            {
                if(cids[i].checked)
                {
                    if(cids[i].name == "cid")
                        cid+=cids[i].value+",";
                }
            } 
            if(cid=="")
            {
                showError("未选中任何数据");
                return;
            }    
            //操作处理
            cid = cid.substring(0,cid.length-1);
            switch(opType)
            {
                case "GrantMember":
                    openWindowOwn('RGrantMember.asp?UserID='+cid,'_RGrantMember',600,240);
                    break;
                case "GrantTreasure":
                    openWindowOwn('RGrantTreasure.asp?UserID='+cid,'_GrantTreasure',600,240);
                    break;
                case "GrantScore":
                    openWindowOwn('RGrantScore.asp?UserID='+cid,'_GrantScore',600,275);
                    break;
                case "GrantGameID":
                    openWindowOwn('RGrantGameID.asp?UserID='+cid,'_GrantGameID',600,240);
                    break;
                case "GrantExperience":
                    openWindowOwn('RGrantExperience.asp?UserID='+cid,'_GrantExperience',600,240);
                    break;
                case "GrantClearScore":
                    openWindowOwn('RGrantClearScore.asp?UserID='+cid,'_GrantClearScore',600,240);
                    break;
                case "GrantFlee":
                    openWindowOwn('RGrantFlee.asp?UserID='+cid,'_GrantFlee',600,240);
                    break;
                case "dongjie":
                    openWindowOwn('AccountsNullity.asp?UserID='+cid,'_AccountsNullity',605,240);
                    break;
            }            
        }
       
    </script>
    <form name="myformList" method="post" action=''>
        <% If Request("action") = "del" Then %>
        <script type="text/javascript">
            showInfo("删除成功")
        </script>
        <% End If %>
        <% If Request("action") = "jinyong" Then %>
        <script type="text/javascript">
            showInfo("冻结成功")
        </script>
        <% End If %>
        <% If Request("action") = "huanyuan" Then %>
        <script type="text/javascript">
            showInfo("解冻成功")
        </script>
        <% 
            End If 
             If CxGame.GetRoleValue("100",trim(session("AdminName")))>"1" Then
        %>
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="Tmg7">
            <tr>
                <td height="39" class="titleOpBg">
                
	                <%
                        If CInt(CxGame.GetRoleValue("100",trim(session("AdminName"))))>7 or trim(session("AdminName"))="admin" Then
                    %>
	                <input type="button" value="删除" class="btn wd1" onclick="Operate('del')" />
                    <input type="button" value="冻结" class="btn wd1" onclick="GrantManager('dongjie')"/>
                    <input type="button" value="解冻" class="btn wd1" onclick="Operate('huanyuan')"/>
                    <input class="btnLine" type="button" /> 
                    <input type="button" value="赠送会员" class="btn wd2" onclick="GrantManager('GrantMember')" />     
                    <%
                        End If
                        If trim(session("AdminName"))="admin" or CxGame.GetRoleValue("100",trim(session("AdminName")))="2" or CxGame.GetRoleValue("100",trim(session("AdminName")))="3" or CxGame.GetRoleValue("100",trim(session("AdminName")))="6" or CxGame.GetRoleValue("100",trim(session("AdminName")))="10" or CxGame.GetRoleValue("100",trim(session("AdminName")))="7" or CxGame.GetRoleValue("100",trim(session("AdminName")))="11" or CxGame.GetRoleValue("100",trim(session("AdminName")))="14" or CxGame.GetRoleValue("100",trim(session("AdminName")))="15" Then
                    %>  
                    <input type="button" value="赠送金币" class="btn wd2" onclick="GrantManager('GrantTreasure')" />  
                    <%
                        End If
                        If trim(session("AdminName"))="admin" or CxGame.GetRoleValue("100",trim(session("AdminName")))="4" or CxGame.GetRoleValue("100",trim(session("AdminName")))="5" or CxGame.GetRoleValue("100",trim(session("AdminName")))="6" or CxGame.GetRoleValue("100",trim(session("AdminName")))="12" or CxGame.GetRoleValue("100",trim(session("AdminName")))="7" or CxGame.GetRoleValue("100",trim(session("AdminName")))="14" or CxGame.GetRoleValue("100",trim(session("AdminName")))="13" or CxGame.GetRoleValue("100",trim(session("AdminName")))="15" Then
                    %> 
                    <input type="button" value="赠送积分" class="btn wd2" onclick="GrantManager('GrantScore')" />     
                    <%
                        End If
                        If trim(session("AdminName"))="admin" or CInt(CxGame.GetRoleValue("100",trim(session("AdminName"))))>7 Then
                    %> 
                    <input type="button" value="赠送经验" class="btn wd2" onclick="GrantManager('GrantExperience')" /> 
                    <input type="button" value="清零积分" class="btn wd2" onclick="GrantManager('GrantClearScore')" />     
                    <input type="button" value="清零逃率" class="btn wd2" onclick="GrantManager('GrantFlee')" />    
                    <%  
                        End If
                    %>  
                    <input name="in_optype" type="hidden" />      
                </td>
            </tr>
        </table> 
        <%
            End If
        %>       
        <div id="content">   
            <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="box">
                <tr align="center" class="bold">
                    <td class="listTitle1"><input type="checkbox" name="chkAll" onclick="SelectAll(this.checked);" /></td>
                    <td class="listTitle2"><a class="l1"  href="" onclick="GetOrder('UserID',this)">用户标识</a></td>
                    <td class="listTitle2"><a class="l1"  href="" onclick="GetOrder('GameID',this)">游戏ID</a></td>
                    <td class="listTitle2"><a class="l1"  href="" onclick="GetOrder('Accounts',this)">用户帐号</a></td>
                    <td class="listTitle2"><a class="l1"  href="" onclick="GetOrder('Gender',this)">用户性别</a></td>
                    <td class="listTitle2"><a class="l1"  href="" onclick="GetOrder('Nickname',this)">用户昵称</a></td>
                    <td class="listTitle2"><a class="l1"  href="" onclick="GetOrder('FaceID',this)">头像</a></td>
                    <td class="listTitle2"><a class="l1"  href="" onclick="GetOrder('Experience',this)">经验值</a></td>
                    <td class="listTitle2"><a class="l1"  href="" onclick="GetOrder('LoveLiness',this)">魅力值</a></td>
                    <td class="listTitle2"><a class="l1"  href="" onclick="GetOrder('Present',this)">礼物</a></td>
                    <td class="listTitle2"><a class="l1"  href="" onclick="GetOrder('MemberOrder',this)">会员级别</a></td>
                    <td class="listTitle2"><a class="l1"  href="" onclick="GetOrder('MasterOrder',this)">管理级别</a></td>
                    <td class="listTitle2"><a  class="l1" href="" onclick="GetOrder('RegisterDate',this)">注册时间</a></td>
                    <td class="listTitle2"><a class="l1"  href="" onclick="GetOrder('RegisterIP',this)">注册地址</a></td>
                    <td class="listTitle2"><a class="l1"  href="" onclick="GetOrder('GameLogonTimes',this)">登录次数</a></td>                    
                    <td class="listTitle2"><a class="l1"  href="" onclick="GetOrder('LastLogonDate',this)">最后登录时间</a></td>
                    <td class="listTitle2"><a class="l1"  href="" onclick="GetOrder('LastLogonIP',this)">最后登录地址</a></td>
                    <td class="listTitle2"><a class="l1"  href="" onclick="GetOrder('Nullity',this)">帐号状态</a></td>
                    <td class="listTitle">管理</td>
                </tr>
                <% 
                    Dim UserID,className
                    If IsNull(rs) Then
                        Response.Write("<tr class='tdbg'><td colspan='100' align='center'><br>没有任何信息!<br><br></td></tr>")
                    Else
                    
                    For i=0 To Ubound(rs,2)
                    UserID = rs(0,i)     
                    If i Mod 2 = 0 Then
                        className="list"        
                    Else
                        className="listBg"      
                    End If 
                %>
                <tr align="center" class="<%=className %>" onmouseover="currentcolor=this.style.backgroundColor;this.style.backgroundColor='#caebfc';this.style.cursor='default';"
                    onmouseout="this.style.backgroundColor=currentcolor">
                    <td><input name='cid' type='checkbox' value='<%=UserID%>'/></td>                  
                    <td><%=rs(0,i) %></td> 
                    <td><%=rs(1,i) %></td> 
                    <td>
                        <%
                            If trim(session("AdminName"))="admin" or CxGame.GetRoleValue("100",trim(session("AdminName")))>"1" Then
                        %>
                        <a class="l" href="javascript:void(0)" onclick="javascript:openWindowOwn('AccountsInfo.asp?id=<%=UserID %>',<%=rs(0,i) %>,850,600)"><%=rs(2,i) %></a>
                        <%
                            Else
                        %>
                        <%=rs(2,i) %>
                        <%
                            End If
                        %>
                    </td>
                    <td>
                        <% If rs(3,i)=0 Then %>
                        保密
                        <% Elseif rs(3,i)=1 Then %>
                        男性
                        <% Else %>
                        女性
                        <% End If %>
                    </td> 
                    <td><%=rs(4,i) %></td> 
                    <td>
                        <img width="25" height="25" src="/gamepic/face<%=Rs(5,i) %>.gif" />
                    </td>
                    <td><%=rs(6,i) %></td> 
                    <td><%=rs(7,i) %></td> 
                    <td><%=rs(8,i) %></td> 
                    <td><%=CxGame.GetMemberOrderName(rs(9,i)) %></td>                    
                    <td>
                        <% If rs(10,i)=0 Then %>
                        普通会员
                        <% Elseif rs(10,i)=1 Then %>
                        <span style="color:#105399;font-weight:bold;">内部网管</span> 
                        <% Elseif rs(10,i)=2 Then %>
                        <span style="color:#cd6a00;font-weight:bold;">外部网管</span> 
                        <% End If %>
                    </td>
                    <td><%=rs(11,i) %></td>
                    <td><%=rs(12,i) %></td>
                    <td><%=rs(13,i) %></td>
                    <td><%=rs(14,i) %></td>
                    <td><%=rs(15,i) %></td>
                    <td>
                        <% If rs(16,i)=0 Then%>
                        正常                        
                        <% Else %>
                        <span style="color:red">冻结</span>
                        <% End If %>
                    </td>                    
                    <td>
                    <%
                        If trim(session("AdminName"))="admin" or CxGame.GetRoleValue("100",trim(session("AdminName")))>"1" Then
                    %>
                    <a class="l" href="javascript:void(0)" onclick="javascript:openWindowOwn('RGrantGameID.asp?UserID=<%=UserID %>','_GrantGameID',600,260);">赠送靓号</a></td>
                    <%
                        End If
                    %>
                </tr>
                <% 
                    Next
                    End If
                %>
            </table>    
        </div>
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
                <td class="listTitleBg"><span>选择：</span>&nbsp;<a class="l1" href="javascript:SelectAll(true);">全部</a>&nbsp;-&nbsp;<a class="l1" href="javascript:SelectAll(false);">无</a></td>
                <td class="page" align="right"><%Response.Write nav%></td>
            </tr>
        </table>  
        <%        
            If CxGame.GetRoleValue("100",trim(session("AdminName")))>"1" Then
        %>
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" id="OpList">
            <tr>
                <td height="39" class="titleOpBg">
                
	                <%
                        If trim(session("AdminName"))="admin" or CInt(CxGame.GetRoleValue("100",trim(session("AdminName"))))>7 Then
                    %>
	                <input type="button" value="删除" class="btn wd1" onclick="Operate('del')" />
                     <input type="button" value="冻结" class="btn wd1" onclick="GrantManager('dongjie')"/>
                    <input type="button" value="解冻" class="btn wd1" onclick="Operate('huanyuan')"/>
                    <input class="btnLine" type="button" /> 
                    <input type="button" value="赠送会员" class="btn wd2" onclick="GrantManager('GrantMember')" />     
                    <%
                        End If
                        If trim(session("AdminName"))="admin" or CxGame.GetRoleValue("100",trim(session("AdminName")))="2" or CxGame.GetRoleValue("100",trim(session("AdminName")))="3" or CxGame.GetRoleValue("100",trim(session("AdminName")))="6" or CxGame.GetRoleValue("100",trim(session("AdminName")))="10" or CxGame.GetRoleValue("100",trim(session("AdminName")))="7" or CxGame.GetRoleValue("100",trim(session("AdminName")))="11" or CxGame.GetRoleValue("100",trim(session("AdminName")))="14" or CxGame.GetRoleValue("100",trim(session("AdminName")))="15" Then
                    %>  
                    <input type="button" value="赠送金币" class="btn wd2" onclick="GrantManager('GrantTreasure')" />  
                    <%
                        End If
                        If trim(session("AdminName"))="admin" or CxGame.GetRoleValue("100",trim(session("AdminName")))="4" or CxGame.GetRoleValue("100",trim(session("AdminName")))="5" or CxGame.GetRoleValue("100",trim(session("AdminName")))="6" or CxGame.GetRoleValue("100",trim(session("AdminName")))="12" or CxGame.GetRoleValue("100",trim(session("AdminName")))="7" or CxGame.GetRoleValue("100",trim(session("AdminName")))="14" or CxGame.GetRoleValue("100",trim(session("AdminName")))="13" or CxGame.GetRoleValue("100",trim(session("AdminName")))="15" Then
                    %> 
                    <input type="button" value="赠送积分" class="btn wd2" onclick="GrantManager('GrantScore')" />     
                    <%
                        End If
                        If trim(session("AdminName"))="admin" or CInt(CxGame.GetRoleValue("100",trim(session("AdminName"))))>7 Then
                    %> 
                    <input type="button" value="赠送经验" class="btn wd2" onclick="GrantManager('GrantExperience')" /> 
                    <input type="button" value="清零积分" class="btn wd2" onclick="GrantManager('GrantClearScore')" />     
                    <input type="button" value="清零逃率" class="btn wd2" onclick="GrantManager('GrantFlee')" />    
                    <%  
                        End If
                    %>     
                    <input name="in_optype" type="hidden" />      
                </td>
            </tr>
        </table> 
        <%
            End If
        %>          
    </form>
    <% 
        End Sub         
    %>    
</body>
</html>
