<!--#include file="../../CommonFun.asp"-->
<!--#include file="../../function.asp"-->
<!--#include file="../../GameConn.asp"-->
<!--#include file="../../Cls_Page.asp"-->
<!--#include file="../../conn.asp"-->

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" >
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <title></title>
    <link href="../../Css/layout.css" rel="stylesheet" type="text/css" />
    <script type="text/javascript" src="../../Js/common.js"></script>
    <script type="text/javascript" src="../../Js/comm.js"></script>
    <script type="text/javascript" src="../../Js/Check.js"></script>
    <script type="text/javascript" src="../../Js/Calendar.js"></script>
    <script type="text/javascript" src="../../Js/Sort.js"></script>
    <script type="text/javascript" src="../../Js/My97DatePicker/WdatePicker.js"></script>
    
</head>

<%
    If CxGame.GetRoleValue("107",trim(session("AdminName")))<"1" Then
    response.redirect "/Index.asp"   
    End If
%>
<body>
    <!-- 头部菜单 Start -->
    <table width="100%" border="0" cellpadding="0" cellspacing="0" class="title">
        <tr>
            <td width="19" height="25" valign="top"  class="Lpd10"><div class="arr"></div></td>
            <td width="1232" height="25" valign="top" align="left">你当前位置：游戏用户 - 机器人管理</td>
        </tr>
    </table>
    <!-- 头部菜单 End -->
    <script type="text/javascript">        
        function QueryInfo(opType)
        {
            var startDate = document.form1.StartDate.value;
            var endDate = document.form1.EndDate.value;
            if(opType==1)
            {
                if(!isDate(startDate)||!isDate(endDate))
                {
                    alert("日期格式不正确！");
                    return
                }
            }
            if(compareDate(startDate,endDate)==false)
		    {
		        
		        alert("开始时间不可大于结束时间！")
		         return false;
		    }
            document.form1.QueryType.value = opType
            document.form1.submit();
        }
    </script>
    <form name="form1" action="">
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="titleQueBg">
            <tr>
                <td class="listTdLeft" style="width: 80px">
                    日期查询：
                </td>
                <td>
                    <% If CxGame.GetInfo(0,"StartDate")<>"" Then %>
                    <input name="StartDate" type="text" class="text" style="width:70px;" value="<%=CxGame.GetInfo(0,"StartDate") %>" maxlength="10" /><img src="../../Images/btn_calendar.gif" onclick="calendar(StartDate)" style="cursor:pointer;vertical-align:middle"/>                    
                    <% Else %>
                    <input name="StartDate" type="text" class="text" style="width:70px;" value="<%=Year(Date())&"-"&Month(Date())&"-1" %>" maxlength="10" /><img src="../../Images/btn_calendar.gif" onclick="calendar(StartDate)" style="cursor:pointer;vertical-align:middle"/>  
                    <% End If %>
                    至
                    <% If CxGame.GetInfo(0,"EndDate")<>"" Then %>
                    <input name="EndDate" type="text" class="text" style="width:70px;" value="<%=CxGame.GetInfo(0,"EndDate") %>" maxlength="10" /><img src="../../Images/btn_calendar.gif" onclick="calendar(EndDate)" style="cursor:pointer;vertical-align:middle"/>                    
                    <% Else%>
                    <input name="EndDate" type="text" class="text" style="width:70px;" value="<%=Date() %>"  maxlength="10"/><img src="../../Images/btn_calendar.gif" onclick="calendar(EndDate)" style="cursor:pointer;vertical-align:middle"/>                    
                    <% End If %>
                    <select name="in_Dname"  style="width:120px;">
                    <% 
                        Dim ArrayKind,i
                        ArrayKind = CxGame.GetRoomList()
                        For i=0 To Ubound(ArrayKind)                                                         
                    %>
                    <option value="<%=ArrayKind(i,2) %>,<%=ArrayKind(i,0) %>"<% If CxGame.GetInfo(0,"in_Dname")= ArrayKind(i,2)&","&ArrayKind(i,0) Then %> selected="selected" <% End If %> ><%=ArrayKind(i,1) %></option>
                    <%                      
                        Next  
                        Set ArrayKind = nothing
                    %>
                    </select>               
                    <select name="nullity" style="width:80px;"> 
                    <option value="0">全部状态</option>
                    <option value="1"<% If Request("nullity")=1 Then %> selected="selected" <%End If %>>启用</option>
                    <option value="2"<% If Request("nullity")=2 Then %> selected="selected" <%End If %>>禁止</option>
                    </select>  
                    <input type="button" value="查询" class="btn wd1" onclick="QueryInfo('1')" />
                    <input type="button" value="今天" class="btn wd1" onclick="QueryInfo('2')"/>
                    <input type="button" value="昨天" class="btn wd1" onclick="QueryInfo('3')"/>
                    <input type="button" value="本星期" class="btn wd6" onclick="QueryInfo('4')"/>
                    <input type="button" value="上星期" class="btn wd6" onclick="QueryInfo('5')"/>   
                    <input type="hidden" name="QueryType" />                 
                </td>
            </tr>
        </table>        
    </form> 
    <form name="form2" action="">
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="Tmg7 titleQueBg">
            <tr>
                <td class="listTdLeft" style="width: 80px">
                    用户查询：
                </td>
                <td>
                    <input name="search" type="text" class="text"  value="<%=CxGame.GetInfo(0,"search") %>"/>
                    <select name="swhat">
                    <option value="1" selected="selected">按用户帐号</option>   
                    <option value="2"<% If CxGame.GetInfo(1,"swhat")=2 Then %> selected="selected" <%End If %>>按游戏ID</option> 
                    </select>
                    <select name="in_Dname"  style="width:120px;">
                    <% 
                        Dim ArrayKindNew,k
                        ArrayKindNew = CxGame.GetRoomList()
                        For k=0 To Ubound(ArrayKindNew)                                                         
                    %>
                    <option value="<%=ArrayKindNew(k,2) %>,<%=ArrayKindNew(k,0) %>"<% If CxGame.GetInfo(0,"in_Dname")= ArrayKindNew(k,2)&","&ArrayKindNew(k,0) Then %> selected="selected" <% End If %> ><%=ArrayKindNew(k,1) %></option>
                    <%                      
                        Next  
                        Set ArrayKind = nothing
                    %>
                    </select>          
                    <input type="submit" value="查询" class="btn wd1" />    
                </td>
            </tr>
        </table>      
    </form>
    <% 
        Select case Request("action")
            case "androidInfo"
                Call GetAndroidInfo()
            case "saveandroidinfo"
                Call SaveAndroidInfo()
            case else
                Call Main()
        End Select 
        
        '保存
        Sub SaveAndroidInfo()
            '连接数据库
            Dim dname
            dname=Request("dbName")
           
            IF dname="" Then
                dname="QPGameScoreDB"
            End If  
                        
            Call ConnectGame(dname)        

            Dim rs,sql
            Set rs=Server.CreateObject("Adodb.RecordSet")
            sql = "select * from AndroidManager where UserID='"&Request("UserID")&"' AND ServerID='"&Request("ServerID")&"'"
            rs.Open sql,GameConn,1,3            
            rs("UserID") = Request("UserID")             
            rs("ServerID") = CxGame.GetInfo(0,"in_ServerID")
            rs("MinPlayDraw") = CxGame.GetInfo(1,"MinPlayDraw")
            rs("MaxPlayDraw") = CxGame.GetInfo(1,"MaxPlayDraw")
            rs("MinTakeScore") = CxGame.GetInfo(1,"MinTakeScore")
            rs("MaxTakeScore") = CxGame.GetInfo(1,"MaxTakeScore")
            rs("MinReposeTime") = CxGame.GetInfo(1,"MinReposeTime")
            rs("MaxReposeTime") = CxGame.GetInfo(1,"MaxReposeTime")
            rs("StartServiceTime") = CxGame.GetInfo(0,"StartServiceTime")
            rs("CloseServiceTime") = CxGame.GetInfo(0,"CloseServiceTime")
            rs("ServiceGender") = CxGame.GetInfo(1,"ServiceGender")
            rs("AndroidNote") = CxGame.GetInfo(0,"AndroidNote")
            rs("Nullity") = CxGame.GetInfo(1,"Nullity")                
            rs.update
            Call CxGame.ShowInfo2("修改成功！","AndroidUserInfo.asp?page="&Request("page")&"",1200)
            rs.close()
            Set rs = Nothing      
            Call CloseGame()     
        End Sub
        
        '删除
        Sub Delete(lID)
            Dim ID,KindID,dname
            ID = lID
            
            GameConn.execute "Delete From AndroidManager Where UserID="&lID
        End Sub
        
        '冻结解冻
        Sub Nullity(lID,typeValue)
            Dim ID,KindID,dname
            ID = lID
            GameConn.execute "Update AndroidManager Set Nullity="&typeValue&" Where UserID="&lID
        End Sub
        
        Sub Main()
            '连接数据库
            Dim dnameArray,dname,serverID
            dnameArray=Request("in_Dname")

            IF dnameArray="" Then
                dname="QPGameScoreDB"
                serverID=0
            Else
                dname = Split(dnameArray,",")(1)
                serverID = Split(dnameArray,",")(0)
            End If  
            
            Call ConnectGame(dname)
            '操作
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
                    OrderStr=Replace(Request.QueryString("orders")," ","+")&" Asc "
                End If
                If Request.QueryString("OrderType")<>"-101" Then
                    OrderStr=Replace(Request.QueryString("orders")," ","+")&" Desc "
                End If
            Else 
                OrderStr = "CreateDate DESC"
            End If
          
            queryCondition = " ServerID="&serverID
            Dim field
            field = "CreateDate,UserID,ServerID,AndroidNote,Nullity"
            '查询条件            
            If CxGame.GetInfo(0,"search")<>"" Then
                Select case Request("swhat")
                    case 1
                        queryCondition = queryCondition&" AND UserID = (Select UserID From QPAccountsDBLink.QPAccountsDB.dbo.AccountsInfo Where Accounts='"&Trim(Request("search"))&"')"
                    case 2
                        queryCondition = queryCondition&" AND UserID = (Select Top 1 UserID From QPAccountsDBLink.QPAccountsDB.dbo.AccountsInfo Where GameID='"&CxGame.SqlCheckNum(Trim(Request("search")))&"')"
                End Select
            End If    
            
            Dim startDate,endDate
            If CxGame.GetInfo(0,"QueryType")<>"" Then
                Select case CxGame.GetInfo(0,"QueryType")
                    case 1
                        startDate = CxGame.GetInfo(0,"StartDate")&" 00:00:00"
                        endDate = CxGame.GetInfo(0,"EndDate")&" 23:59:59"
                    case 2
                        startDate = Date()&" 00:00:00"
                        endDate = Date()&" 23:59:59"
                    case 3
                        startDate = DateAdd("d",-1,Date())&" 00:00:00"
                        endDate = DateAdd("d",-1,Date())&" 23:59:59"
                    case 4
                        startDate = DateAdd("d",1-Weekday(Date()),Date())&" 00:00:00"
                        endDate = DateAdd("d",6,startDate)&" 23:59:59"
                    case 5
                        startDate = DateAdd("d",1-7-Weekday(Date()),Date())&" 00:00:00"
                        endDate = DateAdd("d",6,startDate)&" 23:59:59"
                End Select
                queryCondition = queryCondition&" AND (CreateDate between '"&startDate&"' And '"&endDate&"')"
            End If
            
            If Request("nullity") <>"" Then
                Select Case Request("nullity")      
                    case 0
                        queryCondition = queryCondition&""
                    case 1
                        queryCondition = queryCondition&" And Nullity=0"
                    case 2
                        queryCondition = queryCondition&" And Nullity=1"
                End Select   
            End If         
           
            '==============================================================================================================
            '执行存储过程
                    
            Set Page = new Cls_Page      '创建对象
            Set Page.Conn = GameConn     '得到数据库连接对象        
            With Page
                .PageSize = GetPageSize					'每页记录条数
                .PageParm = "page"				'页参数
                '.PageIndex = 10				'当前页，可选参数，一般是生成静态时需要
	            .Database = "mssql"				'数据库类型,AC为access,MSSQL为sqlserver2000存储过程版,MYSQL为mysql,PGSQL为PostGreSql
	            .Pkey="UserID"					'主键
	            .Field=field	'字段
	            .Table="AndroidManager"			'表名
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
        <% End If %>
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="Tmg7">
            <tr>
                <td height="39" class="titleOpBg">
                    <%
                        If trim(session("AdminName"))="admin" or CxGame.GetRoleValue("107",trim(session("AdminName")))="3" or CxGame.GetRoleValue("107",trim(session("AdminName")))="1" or CxGame.GetRoleValue("107",trim(session("AdminName")))="7" Then
                    %>
                    <input type="button" class="btn wd1" onclick="Redirect('AndroidUserAdd.asp')" value="新增" />  
                    <%
                        End If
                        If trim(session("AdminName"))="admin" or CxGame.GetRoleValue("107",trim(session("AdminName")))>"3" Then
                    %>       
	                <input type="button" value="删除" class="btn wd1" onclick="Operate('del')" />
                    <input type="button" value="冻结" class="btn wd1" onclick="Operate('jinyong')"/>
                    <input type="button" value="解冻" class="btn wd1" onclick="Operate('huanyuan')"/>
                    <input class="btnLine" type="button" /> 
                    <input type="button" value="赠送会员" class="btn wd2" onclick="GrantManager('GrantMember')" />     
                    <%
                        End If
                        If trim(session("AdminName"))="admin" or CxGame.GetRoleValue("107",trim(session("AdminName")))="3" or CxGame.GetRoleValue("107",trim(session("AdminName")))="2" or CxGame.GetRoleValue("107",trim(session("AdminName")))="7" or CxGame.GetRoleValue("107",trim(session("AdminName")))="6" Then
                    %> 
                    <input type="button" value="赠送金币" class="btn wd2" onclick="GrantManager('GrantTreasure')" />     
                    <%
                        End If
                        If trim(session("AdminName"))="admin" or CxGame.GetRoleValue("107",trim(session("AdminName")))>"3" Then
                    %>
                    <input type="button" value="赠送积分" class="btn wd2" onclick="GrantManager('GrantScore')" />   
                    <input type="button" value="赠送经验" class="btn wd2" onclick="GrantManager('GrantExperience')" /> 
                    <input type="button" value="清零积分" class="btn wd2" onclick="GrantManager('GrantClearScore')" />     
                    <input type="button" value="清零逃率" class="btn wd2" onclick="GrantManager('GrantFlee')" />    
                    <%
                        End If
                    %>  
                    <input name="in_optype" type="hidden" />  
                    <input name="in_Dname1" type="hidden" value="<%=CxGame.GetInfo(0,"in_Dname") %>" />    
                </td>
            </tr>
        </table>  
        <div id="content">
            <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="box">
                <tr align="center" class="bold">
                    <th class="listTitle1" width='38' align='center'><input type="checkbox" name="chkAll" onclick="SelectAll(this.checked);" /></th>                    
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('CreateDate',this)">创建时间</a></th>                    
                    <th class="listTitle2"><a  class="l1" href="" onclick="GetOrder('UserID',this)">用户帐号</a></th>
                    <% If Request("in_Dname")="QPTreasureDB" Then %>
                    <th class="listTitle2"><a  class="l1" href="" onclick="GetOrder('UserID',this)">金币+银行</a></th>
                    <% End If %>   
                    <th class="listTitle2"><a  class="l1" href="" onclick="GetOrder('UserID',this)">赢局</a></th>   
                    <th class="listTitle2"><a  class="l1" href="" onclick="GetOrder('UserID',this)">输局</a></th>                       
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('ServerID',this)">房间</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('AndroidNote',this)">备注</a></th>
                    <th class="listTitle"><a class="l1"  href="" onclick="GetOrder('Nullity',this)">冻结状态</a></th>                    
                </tr>
                <% 
                    Dim className
                    If IsNull(rs) Then
                        Response.Write("<tr class='tdbg'><td colspan='100' align='center'><br>没有任何信息!<br><br></td></tr>")
                    Else
                    
                    For i=0 To Ubound(rs,2)
                    If i Mod 2 = 0 Then
                        className="list"        
                    Else
                        className="listBg"      
                    End If 
                %>
                <tr class="<%=className %>" onmouseover="currentcolor=this.style.backgroundColor;this.style.backgroundColor='#caebfc';this.style.cursor='pointer';"
                    onmouseout="this.style.backgroundColor=currentcolor">
                    <td><input name='cid' type='checkbox' value='<%=rs(1,i)%>'/></td>                  
                    <td onclick="Redirect('AndroidUserInfo.asp?action=androidInfo&UserID=<%=rs(1,i) %>&ServerID=<%=rs(2,i) %>&dbName=<%=Request("in_Dname") %>&page=<%=Request("page") %>')"><%=rs(0,i) %></td>   
                    <td>
                        <a class="l" href="javascript:void(0)" onclick="javascript:openWindowOwn('AccountsInfo.asp?id=<%=rs(1,i) %>',<%=rs(1,i) %>,850,600)"><%=CxGame.GetUserNameByUserID(rs(1,i)) %></a>
                    </td>
                    <% If trim(Request("in_Dname"))="QPTreasureDB" Then %>
                    <td onclick="Redirect('AndroidUserInfo.asp?action=androidInfo&UserID=<%=rs(1,i) %>&ServerID=<%=rs(2,i) %>&dbName=<%=Request("in_Dname") %>&page=<%=Request("page") %>')"><%=CxGame.GetScoreInfoByUserID(rs(1,i),dname)(1,0) %></td>  
                    <% End If %>
                    <td onclick="Redirect('AndroidUserInfo.asp?action=androidInfo&UserID=<%=rs(1,i) %>&ServerID=<%=rs(2,i) %>&dbName=<%=Request("in_Dname") %>&page=<%=Request("page") %>')"><%=CxGame.GetScoreInfoByUserID(rs(1,i),dname)(1,1) %></td>  
                    <td onclick="Redirect('AndroidUserInfo.asp?action=androidInfo&UserID=<%=rs(1,i) %>&ServerID=<%=rs(2,i) %>&dbName=<%=Request("in_Dname") %>&page=<%=Request("page") %>')"><%=CxGame.GetScoreInfoByUserID(rs(1,i),dname)(1,2) %></td>  
                    <td onclick="Redirect('AndroidUserInfo.asp?action=androidInfo&UserID=<%=rs(1,i) %>&ServerID=<%=rs(2,i) %>&dbName=<%=Request("in_Dname") %>&page=<%=Request("page") %>')"><%=CxGame.GetRoomNameByRoomID(rs(2,i)) %></td>  
                    <td onclick="Redirect('AndroidUserInfo.asp?action=androidInfo&UserID=<%=rs(1,i) %>&ServerID=<%=rs(2,i) %>&dbName=<%=Request("in_Dname") %>&page=<%=Request("page") %>')"><%=rs(3,i) %></td>  
                    <td onclick="Redirect('AndroidUserInfo.asp?action=androidInfo&UserID=<%=rs(1,i) %>&ServerID=<%=rs(2,i) %>&dbName=<%=Request("in_Dname") %>&page=<%=Request("page") %>')"><%=CxGame.GetNullityInfo(rs(4,i)) %></td>                      
                </tr>
                <% 
                    Next  
                    End If    
                %>                
            </table>            
        </div>
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
                <td class="listTitleBg" ><span>选择：</span>&nbsp;<a class="l1" href="javascript:SelectAll(true);">全部</a>&nbsp;-&nbsp;<a class="l1" href="javascript:SelectAll(false);">无</a></td>
                <td class="page" align="right" style="height: 19px"><%Response.Write nav%></td>
            </tr>
        </table>  
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" id="OpList">
            <tr>
                <td height="39" class="titleOpBg">
	                <%
                        If trim(session("AdminName"))="admin" or CxGame.GetRoleValue("107",trim(session("AdminName")))="3" or CxGame.GetRoleValue("107",trim(session("AdminName")))="1" or CxGame.GetRoleValue("107",trim(session("AdminName")))="7" Then
                    %>
                    <input type="button" class="btn wd1" onclick="Redirect('AndroidUserAdd.asp')" value="新增" />  
                    <%
                        End If
                        If trim(session("AdminName"))="admin" or CxGame.GetRoleValue("107",trim(session("AdminName")))>"3" Then
                    %>       
	                <input type="button" value="删除" class="btn wd1" onclick="Operate('del')" />
                    <input type="button" value="冻结" class="btn wd1" onclick="Operate('jinyong')"/>
                    <input type="button" value="解冻" class="btn wd1" onclick="Operate('huanyuan')"/>
                    <input class="btnLine" type="button" /> 
                    <input type="button" value="赠送会员" class="btn wd2" onclick="GrantManager('GrantMember')" />     
                    <%
                        End If
                        If trim(session("AdminName"))="admin" or CxGame.GetRoleValue("107",trim(session("AdminName")))="3" or CxGame.GetRoleValue("107",trim(session("AdminName")))="2" or CxGame.GetRoleValue("107",trim(session("AdminName")))="7" Then
                    %> 
                    <input type="button" value="赠送金币" class="btn wd2" onclick="GrantManager('GrantTreasure')" />     
                    <%
                        End If
                        If trim(session("AdminName"))="admin" or CxGame.GetRoleValue("107",trim(session("AdminName")))>"3" Then
                    %>
                    <input type="button" value="赠送积分" class="btn wd2" onclick="GrantManager('GrantScore')" />   
                    <input type="button" value="赠送经验" class="btn wd2" onclick="GrantManager('GrantExperience')" /> 
                    <input type="button" value="清零积分" class="btn wd2" onclick="GrantManager('GrantClearScore')" />     
                    <input type="button" value="清零逃率" class="btn wd2" onclick="GrantManager('GrantFlee')" />    
                    <%
                        End If
                    %>  
                    <input name="in_Dname" type="hidden" value="<%=CxGame.GetInfo(0,"in_Dname") %>" />    
                </td>
            </tr>
        </table>  
    </form>
    <% 
        Call CloseGame()
        End Sub
        
        rem 机器人信息
        Sub GetAndroidInfo
            '连接数据库
            Dim dname
            dname=Request("dbName")
           
            IF dname="" Then
                dname="QPGameScoreDB"
            End If              
            Call ConnectGame(dname)
            
            Dim rs,sql
            Dim UserID,ServerID,MinPlayDraw,MaxPlayDraw,MinTakeScore,MaxTakeScore,MinReposeTime,MaxReposeTime,StartServiceTime,CloseServiceTime,ServiceGender,AndroidNote,Nullity,CreateDate                
            Dim xorObj            
            Set rs=Server.CreateObject("Adodb.RecordSet")
            sql = "select * from AndroidManager(nolock) where UserID='"&Request("UserID")&"' AND ServerID='"&Request("ServerID")&"'"
            rs.Open sql,GameConn,1,3
            If not rs.Bof And not rs.Eof Then
                UserID = rs("UserID")                
                ServerID = rs("ServerID")
                MinPlayDraw = rs("MinPlayDraw")
                MaxPlayDraw = rs("MaxPlayDraw")
                MinTakeScore = rs("MinTakeScore")
                MaxTakeScore = rs("MaxTakeScore")
                MinReposeTime = rs("MinReposeTime")
                MaxReposeTime = rs("MaxReposeTime")
                StartServiceTime = rs("StartServiceTime")
                CloseServiceTime = rs("CloseServiceTime")
                ServiceGender = rs("ServiceGender")
                AndroidNote = rs("AndroidNote")
                Nullity = rs("Nullity")
                CreateDate = rs("CreateDate")
            End If
            Call CloseGame()
    %>
    <form name="myFormInfo" method="post" action='?action=saveandroidinfo&UserID=<%=Request("UserID") %>&ServerID=<%=Request("ServerID") %>&dbName=<%=Request("dbName") %>&page=<%=Request("page") %>' onsubmit="return CheckFormInfo()">
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="Tmg7">
            <tr>
                <td class="titleOpBg Lpd10">
                    <input type="button" value="返回" class="btn wd1" onclick="Redirect('AndroidUserInfo.asp')" />                           
                    <input class="btnLine" type="button" />  
                    <input type="submit" value="保存" class="btn wd1" />     
                </td>
            </tr>
        </table>
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="listBg2">
            <tr>
                <td height="35" colspan="2" class="f14 bold Lpd10 Rpd10"><div class="hg3  pd7">修改机器人信息</div></td>
            </tr>
            <tr>
                <td class="listTdLeft">机器人名：</td>
                <td>
                    <span class="lan"><%=CxGame.GetUserNameByUserID(UserID) %></span>
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">房间名称：</td>
                <td>       
                    <select name="in_ServerID" style="width:157px;">
                    <% 
                        Dim ArrayKindN,j
                        ArrayKindN = CxGame.GetRoomList1()
                        For j=0 To Ubound(ArrayKindN)    
                        If ServerID=ArrayKindN(j,0) Then    
                    %>
                    <option value="<%=ArrayKindN(j,0) %>" selected="selected"><%=ArrayKindN(j,1) %></option>
                    <% Else%>
                    <option value="<%=ArrayKindN(j,0) %>"><%=ArrayKindN(j,1) %></option>
                    <% 
                        End If
                        Next 
                        Set ArrayKindN = nothing
                    %>
                    </select>                
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">最少局数：</td>
                <td>
                    <input name="MinPlayDraw" type="text" class="text" value="<%=MinPlayDraw %>"/>    
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">最大局数：</td>
                <td>
                    <input name="MaxPlayDraw" type="text" class="text" value="<%=MaxPlayDraw %>"/>           
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">最少分数：</td>
                <td>
                    <input name="MinTakeScore" type="text" class="text" value="<%=MinTakeScore %>"/>              
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">最高分数：</td>
                <td>
                    <input name="MaxTakeScore" type="text" class="text" value="<%=MaxTakeScore %>"/>              
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">最少休息：</td>
                <td>
                    <input name="MinReposeTime" type="text" class="text" value="<%=MinReposeTime %>"/>              
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">最大休息：</td>
                <td>
                    <input name="MaxReposeTime" type="text" class="text" value="<%=MaxReposeTime %>"/>              
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">开始时间：</td>
                <td>
                    <input name="StartServiceTime" type="text" class="text" value="<%=StartServiceTime %>"/><img src="../../Images/btn_calendar.gif" onclick="WdatePicker({el:'StartServiceTime',skin:'whyGreen',dateFmt:'yyyy-MM-dd HH:mm:ss'})" style="cursor:pointer;vertical-align:middle"/>                   
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">结束时间：</td>
                <td>
                    <input name="CloseServiceTime" type="text" class="text" value="<%=CloseServiceTime %>"/><img src="../../Images/btn_calendar.gif" onclick="WdatePicker({el:'CloseServiceTime',skin:'whyGreen',dateFmt:'yyyy-MM-dd HH:mm:ss'})" style="cursor:pointer;vertical-align:middle"/>                  
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">服务类型：</td>
                <td>
                    <input name="ServiceGender" type="text" class="text" value="<%=ServiceGender %>"/>              
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">备注信息：</td>
                <td>
                    <input name="AndroidNote" type="text" class="text" value="<%=AndroidNote %>"/>              
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">禁用状态：</td>
                <td>
                    <input name="in_Nullity" id="Checkbox1" type="radio" value="0"<% If Nullity = 0 Then %> checked="checked"<% End If  %> /><label for="Checkbox1">启用</label> 
                    <input name="in_Nullity" id="Checkbox2" type="radio" value="1"<% If Nullity = 1 Then %> checked="checked"<% End If  %> /><label for="Checkbox2">冻结</label> 
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">创建时间：</td>
                <td>
                    <span><%=CreateDate %></span>
                </td>
            </tr>
        </table>
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
                <td class="titleOpBg Lpd10">
                    <input type="button" value="返回" class="btn wd1" onclick="Redirect('AndroidUserInfo.asp')" />                           
                    <input class="btnLine" type="button" />  
                    <input type="submit" value="保存" class="btn wd1" />     
                </td>
            </tr>
        </table>
    </form>
    <%
        rs.Close()
        Set rs=nothing
        End Sub
    %>
</body>
</html>
