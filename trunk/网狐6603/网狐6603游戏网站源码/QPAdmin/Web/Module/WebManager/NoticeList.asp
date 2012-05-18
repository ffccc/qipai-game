<!--#include file="../../CommonFun.asp"-->
<!--#include file="../../GameConn.asp"-->
<!--#include file="../../Cls_Page.asp"-->
<!--#include file="../../Conn.asp"-->
<!--#include file="../../md5.asp"-->

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" >
<head runat="server">
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <title></title>
    <link href="../../Css/layout.css" rel="stylesheet" type="text/css" />
    <script type="text/javascript" src="../../Js/common.js"></script>
    <script type="text/javascript" src="../../Js/Sort.js"></script>
    <script type="text/javascript" src="../../Js/Check.js"></script>
    <script language="javascript" type="text/javascript" src="../../Js/My97DatePicker/WdatePicker.js"></script>

    
</head>

<body>
    <!-- 头部菜单 Start -->
     
    <table width="100%" border="0" cellpadding="0" cellspacing="0" class="title">
        <tr>
            <td width="19" height="25" valign="top"  class="Lpd10"><div class="arr"></div></td>
            <td width="1232" height="25" valign="top" align="left">你当前位置：管理中心 - 弹出页面</td>
        </tr>
    </table>
    <!-- 头部菜单 End -->  <%
     
	    Call ConnectGame("QPNativeWebDB")
        Select case lcase(Request("action"))
            case "save"
            Call Save()
            case "newinfo"
            Call NewInfo()
            case "delete"
            Call DeleteInfo()
            case else
            Call Main()
        End Select
        
        '删除
        Sub DeleteInfo(lID)
            Dim rs,sql,fileurl,ID
            ID = Trim(lID)
            GameConn.execute "Delete From Notice Where NoticeID="&ID
	        Call CxGame.ShowInfo2("删除成功！","NoticeList.asp",1200)
        End Sub
        
        '冻结解冻
        Sub Nullity(lID,typeValue)
            Dim ID
            ID = lID
            GameConn.execute "Update Notice Set Nullity="&typeValue&" Where NoticeID="&ID
        End Sub
        
        
        '新增,更新
	    Sub Save()
	        Dim fileurl,rs,sql,Location
	        Set rs=Server.CreateObject("Adodb.RecordSet")
	        Location=""
	           If(Request("ck1"))<>"" Then
                If Location<>"" Then
                    Location=Location&","&Request("ck1")
                Else
                    Location=Request("ck1")
                End If
            End If
            
            If(Request("ck2"))<>"" Then
                If Location<>"" Then
                    Location=Location&","&Request("ck2")
                Else
                    Location=Request("ck2")
                End If
            End If
            
             If(Request("ck3"))<>"" Then
                If Location<>"" Then
                    Location=Location&","&Request("ck3")
                Else
                    Location=Request("ck3")
                End If
            End If
            
             If(Request("ck4"))<>"" Then
                If Location<>"" Then
                    Location=Location&","&Request("ck4")
                Else
                    Location=Request("ck4")
                End If
            End If
            
             If(Request("ck5"))<>"" Then
                If Location<>"" Then
                    Location=Location&","&Request("ck5")
                Else
                    Location=Request("ck5")
                End If
            End If
            
             If(Request("ck6"))<>"" Then
                If Location<>"" Then
                    Location=Location&","&Request("ck6")
                Else
                    Location=Request("ck6")
                End If
            End If
            
             If(Request("ck7"))<>"" Then
                If Location<>"" Then
                    Location=Location&","&Request("ck7")
                Else
                    Location=Request("ck7")
                End If
            End If
            
             If(Request("ck8"))<>"" Then
                If Location<>"" Then
                    Location=Location&","&Request("ck8")
                Else
                    Location=Request("ck8")
                End If
            End If
            
	        If Request("in_NoticeID")="" Then
                sql="Insert into Notice(Subject,IsLink,LinkUrl,Body,Location,Width,Height,StartDate,OverDate,Nullity) values ('"&Request("Subject")&"','"&Request("IsLink")&"','"&Request("LinkUrl")&"','"&Request("content1")&"','"&Location&"','"&Request("Width")&"','"&Request("Height")&"','"&Request("StartDate")&"','"&Request("OverDate")&"','"&Request("in_Nullity")&"')"
                rs.Open sql,GameConn,1,3
                Call CxGame.ShowInfo2("新增成功！","NoticeList.asp",1200)
                Set rs = nothing
            Else 
                sql="update  Notice set Nullity='"&Request("in_Nullity")&"', Subject='"&Request("Subject")&"',IsLink='"&Request("IsLinks")&"',LinkUrl='"&Request("LinkUrl")&"',Body='"&Request("content1")&"',Location='"&Location&"',Width='"&Request("Width")&"',Height='"&Request("Height")&"',StartDate='"&Request("StartDate")&"',OverDate='"&Request("OverDate")&"' where NoticeID="&Request("in_NoticeID")
                rs.Open sql,GameConn,1,3
                Call CxGame.ShowInfo2("更新成功！","NoticeList.asp",1200)
                Set rs = nothing
            End If
        End Sub
        
        Sub Main()
            Dim cidArray, lLoop
            cidArray = Split(Request("cid"),",")
            acType = Request("action")
            For lLoop=0 To UBound(cidArray)
                Select case acType
                    case "del"
                        Call DeleteInfo(cidArray(lLoop))
                    case "jinyong"
                        Call Nullity(cidArray(lLoop),1)
                    case "huanyuan"
                        Call Nullity(cidArray(lLoop),0)
                End Select
            Next      
             
            If Request("orders")<>""And Request("OrderType")<>"" Then
                If Request("OrderType")<>"-102" Then
                    OrderStr=Replace(Request("orders")," ","+")&" Asc "
                End If
                If Request.QueryString("OrderType")<>"-101" Then
                    OrderStr=Replace(Request("orders")," ","+")&" Desc "
                End If
            Else 
                OrderStr = "OverDate desc"
            End If   
            '==============================================================================================================
            '执行存储过程
                    
            Set Page = new Cls_Page      '创建对象
            Set Page.Conn = GameConn     '得到数据库连接对象        
            With Page
                .PageSize = 20				'每页记录条数
                .PageParm = "page"				'页参数
                '.PageIndex = 10				'当前页，可选参数，一般是生成静态时需要
	            .Database = "mssql"				'数据库类型,AC为access,MSSQL为sqlserver2000存储过程版,MYSQL为mysql,PGSQL为PostGreSql
	            .Pkey="NoticeID"					'主键
	            .Field="*"	'字段
	            .Table="Notice"			'表名
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
            var opVal = document.form1.in_optype
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
            document.form1.action = "?action="+opVal.value;
            document.form1.submit();
        }
    </script>
    
    <form name="form1" id="form1" method="post" action="?action=orderinfo">
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" >
            <tr>
                <td height="39" class="titleOpBg">
                    <input type="button" value="新增" class="btn wd1" onclick="Redirect('NoticeList.asp?action=newinfo')" />
                    <input class="btnLine" type="button" />
                    <input type="submit" value="删除" class="btn wd1" />  
                     <input type="button" value="冻结" class="btn wd1" onclick="Operate('jinyong')"/>
                     <input type="button" value="解冻" class="btn wd1" onclick="Operate('huanyuan')"/>   
                    <input name="in_optype" type="hidden" />                 
                </td>
            </tr>
        </table> 
        <div id="content">        
            <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="box">
                <tr align="center" class="bold">
                    <td class="listTitle1" style="width:38px;"><input type="checkbox" name="chkAll" onclick="SelectAll(this.checked);" /></td>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('Subject',this)">标题</a></th>
                    <th class="listTitle2">窗口大小</th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('Nullity',this)">禁用</a></th>
                    <th class="listTitle"><a class="l1"  href="" onclick="GetOrder('StartDate',this)">开始</a></th>
                    <th class="listTitle"><a class="l1"  href="" onclick="GetOrder('OverDate',this)">结束</a></th>
                    <th class="listTitle"><a class="l1"  href="" onclick="GetOrder('Location',this)">公告范围</a></th>
                </tr>
                <% 
                    Dim rs,sql,id,className,rowNum,OrderStr
                   
                    If IsNull(rs) Then
                        Response.Write("<tr class='tdbg'><td colspan='100' align='center'><br>没有任何信息!<br><br></td></tr>")
                    Else                    
                    For i=0 To Ubound(rs,2)    
                    rowNum = rowNum+1
                    If rowNum Mod 2 = 0 Then
                        className="list"        
                    Else
                        className="listBg"      
                    End If        
                %>
                <tr class="<%=className %>" onmouseover="currentcolor=this.style.backgroundColor;this.style.backgroundColor='#caebfc';this.style.cursor='pointer';"
                    onmouseout="this.style.backgroundColor=currentcolor">      
                    <td><input name='cid' type='checkbox' value='<%=rs(0,i)%>'/></td>   
                    <td onclick="Redirect('NoticeList.asp?action=newinfo&NoticeID=<%=rs(0,i) %>')"><%=rs(1,i) %></td>    
                    <td onclick="Redirect('NoticeList.asp?action=newinfo&NoticeID=<%=rs(0,i) %>')"><%=rs(6,i) %>×<%=rs(7,i) %></td>    
                    <td onclick="Redirect('NoticeList.asp?action=newinfo&NoticeID=<%=rs(0,i) %>')">
                     <% If rs(10,i)=0 Then%>
                        否                        
                        <% Else%>
                        <span style="color:red">是</span>
                        <% End If %>
                    
                    </td> 
                    <td onclick="Redirect('NoticeList.asp?action=newinfo&NoticeID=<%=rs(0,i) %>')"><%=rs(8,i) %></td> 
                    <td onclick="Redirect('NoticeList.asp?action=newinfo&NoticeID=<%=rs(0,i) %>')"><%=rs(9,i) %></td> 
                    <td onclick="Redirect('NoticeList.asp?action=newinfo&NoticeID=<%=rs(0,i) %>')">
                    <%
                        Dim Location
                        Location=""
                        If InStr(rs(5,i),"0") Then
                            If Location<>"" Then
                                Location=Location&","&"首页"
                            Else
                                Location="首页"
                            End If
                        End If
                        If InStr(rs(5,i),"1") Then
                            If Location<>"" Then
                                Location=Location&","&"新闻公告"
                            Else
                                Location="新闻公告"
                            End If
                        End If
                        If InStr(rs(5,i),"2") Then
                            If Location<>"" Then
                                Location=Location&","&"会员中心"
                            Else
                                Location="会员中心"
                            End If
                        End If
                        If InStr(rs(5,i),"3") Then
                            If Location<>"" Then
                                Location=Location&","&"充值中心"
                            Else
                                Location="充值中心"
                            End If
                        End If
                        If InStr(rs(5,i),"4") Then
                            If Location<>"" Then
                                Location=Location&","&"比赛中心"
                            Else
                                Location="比赛中心"
                            End If
                        End If
                        If InStr(rs(5,i),"5") Then
                            If Location<>"" Then
                                Location=Location&","&"游戏商城"
                            Else
                                Location="游戏商城"
                            End If
                        End If
                        If InStr(rs(5,i),"6") Then
                            If Location<>"" Then
                                Location=Location&","&"推广系统"
                            Else
                                Location="推广系统"
                            End If
                        End If
                        If InStr(rs(5,i),"7") Then
                            If Location<>"" Then
                                Location=Location&","&"客服中心"
                            Else
                                Location="客服中心"
                            End If
                        End If
                    %>
                    <%=Location %>
                    </td> 
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
         <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" id="OpList">
            <tr>
                <td height="39" class="titleOpBg">
                    <input type="button" value="新增" class="btn wd1" onclick="Redirect('NoticeList.asp?action=newinfo')" />
                    <input class="btnLine" type="button" />
                    <input type="submit" value="删除" class="btn wd1" />  
                     <input type="button" value="冻结" class="btn wd1" onclick="Operate('jinyong')"/>
                     <input type="button" value="解冻" class="btn wd1" onclick="Operate('huanyuan')"/>   
                    <input name="in_optype" type="hidden" /> 
                </td>
            </tr>
        </table>        
    </form>
    <%
        End Sub
        
        Sub NewInfo()
            Dim rs,sql,Subject,LinkUrl,IsLink,Body,Location,Width,Height,StartDate,OverDate,NoticeID,Nullity
            NoticeID=Request("NoticeID")
            If(Request("NoticeID")<>"") Then
                Set rs=server.createobject("adodb.recordset")
                sql="select * from Notice where NoticeID="&Request("NoticeID")
                rs.open sql,GameConn,1,3
                Subject=rs("Subject")
                LinkUrl=rs("LinkUrl")
                IsLink=rs("IsLink")
                Body=rs("Body")
                Location=rs("Location")
                Width=rs("Width")
                Height=rs("Height")
                StartDate=rs("StartDate")
                OverDate=rs("OverDate")
                Nullity=rs("Nullity")
            Else
                Subject=""
                LinkUrl=""
                IsLink="0"
                Body=""
                Location=""
                Width=""
                Height=""
                StartDate=""
                OverDate=""
                Nullity=0
            End If
    %> 
    <script type="text/javascript" src="../../kindeditor/kindeditor.js"></script>
    <script type="text/javascript">
		KE.show({
			id : 'content1',
			urlType : 'absolute',
			imageUploadJson : '../../asp/upload_json.asp?type=Notice',
			fileManagerJson : '../../asp/file_manager_json.asp',
			allowFileManager : true,
			afterCreate : function(id) {
				KE.event.ctrl(document, 13, function() {
					KE.util.setData(id);
					document.forms['example'].submit();
				});
				KE.event.ctrl(KE.g[id].iframeDoc, 13, function() {
					KE.util.setData(id);
					document.forms['example'].submit();
				});
			}
		});
		
		function CheckFormInfo()
		{
		    var Subject = document.form1.Subject.value.trim()
		    var content1 = document.form1.content1.value.trim();
		    var LinkUrl = document.form1.LinkUrl.value.trim()
		    var StartDate = document.form1.StartDate.value.trim()
		    var OverDate = document.form1.OverDate.value.trim()
		    var Width = document.form1.Width.value.trim()
		    var Height = document.form1.Height.value.trim()
		    if(Subject=="")
		    {
		        alert("标题不能为空！");
		        return false;
		    }
		  
		     if(LinkUrl=="")
		    {
		        alert("地址不能为空！");
		        return false;
		    }
		    
		    if(content1=="")
		    {
		        alert("内容不能为空！");
		        return false;
		    }
		    
		    if(StartDate=="")
		    {
		        alert("开始时间不能为空！");
		        return false;
		    }
		    if(OverDate=="")
		    {
		        alert("结束时间不能为空！");
		        return false;
		    }
		    if(StartDate>OverDate)
		    {
		        alert("开始时间不能大于结束时间！");
		        return false;
		    }
		    if(Width=="")
		    {
		        alert("宽不能为空！");
		        return false;
		    }
		    if(IsPositiveInt(Width)==false)
		    {
		        alert("宽非数值型字符！");
                return false;
		    }
		    if(Height=="")
		    {
		        alert("高不能为空！");
		        return false;
		    }
		    if(IsPositiveInt(Height)==false)
		    {
		        alert("高非数值型字符！");
                return false;
		    }
		    if(compareDate(StartDate,OverDate)==false)
		    {
		        
		        alert("开始时间不可大于结束时间！")
		        return
		    }
		}
	</script>
    <form name="form1" method="post" action="?action=save&NoticeID=<%=NoticeID %>"   onsubmit="return CheckFormInfo();">
         <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" >
             <tr>
                 <td height="39" class="titleOpBg">
                     <input type="submit" value="保存" class="btn wd1" /><input type="hidden" name="in_NoticeID" value="<%=NoticeID%>"/>
                     <input class="btnLine" type="button" />
                     <input type="button" value="返回" class="btn wd1" onclick="Redirect('NoticeList.asp')" />
                 </td>
             </tr>
         </table>  
         <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="listBg2">
            <tr>
                <td height="35" colspan="2" class="f14 bold Lpd10 Rpd10"><div class="hg3  pd7">规则管理</div></td>
            </tr>
            <tr>
                <td class="listTdLeft">标题：</td>
                <td>
                    <input name="Subject" id="Subject" type="text" class="text wd4" style="width:585px;" value="<%=Subject %>" />
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">引用：</td>
                <td>                    
                    <input name="LinkUrl" type="text" class="text" style="width:585px;" <% If LinkUrl="" Then %> value="http://" <% Else %> value="<%=LinkUrl %>" <% End If %>/>     
                    <input name="IsLinks" id="IsLinks" type="checkbox" value="1" <%If IsLink=1 Then %> checked="checked" <% End If %> /><label for="IsLinks">链接地址</label>                              
                </td>
            </tr>
             <tr>
                <td class="listTdLeft">内容：</td>
                <td>
                    <textarea id="content1"  name="content1" cols="100" rows="8" style="width:700px;height:350px;visibility:hidden;"><%=Body %>
                    </textarea> 
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">冻结：</td>
                <td>
                    <input name="in_Nullity"  id="in_Nullity0" type="radio" value="0"<% If Nullity=0 Then %> checked="checked"<% End If %> onclick="ChangeOverDate(this)" /><label for="in_Nullity0">正常</label>
                    <input name="in_Nullity"  id="in_Nullity1" type="radio" value="1"<% If Nullity=1 Then %> checked="checked"<% End If %> onclick="ChangeOverDate(this)" /><label for="in_Nullity1">冻结</label> 
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">公告范围：</td>
                <td>
                  <input name="ck1" type=checkbox  value="0" <%If InStr(Location,"0")>0 Then %> checked="checked" <% End If %>  />首页 <input name="ck2" type=checkbox  value="1" <%If InStr(Location,"1")>0 Then %> checked="checked" <% End If %>  />新闻公告 <input name="ck3" type=checkbox  value="2" <%If InStr(Location,"2")>0 Then %> checked="checked" <% End If %>  />会员中心 <input name="ck4" type=checkbox  value="3" <%If InStr(Location,"3")>0 Then %> checked="checked" <% End If %>  />充值中心 <input type=checkbox name="ck5"  value="4" <%If InStr(Location,"4")>0 Then %> checked="checked" <% End If %>  />比赛中心 <input name="ck6" type=checkbox  value="5" <%If InStr(Location,"5")>0 Then %> checked="checked" <% End If %>  />游戏商城 <input type=checkbox name="ck7"  value="6" <%If InStr(Location,"6")>0 Then %> checked="checked" <% End If %>  />推广系统 <input type=checkbox name="ck8"  value="7" <%If InStr(Location,"7")>0 Then %> checked="checked" <% End If %>  />客服中心
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">公告日期：</td>
                <td>
                   <input class="Wdate" type="text" onClick="WdatePicker({skin:'whyGreen',dateFmt:'yyyy-MM-dd HH:mm:ss'})" name="StartDate"  value="<%=StartDate %>" >至 <input class="Wdate" type="text" name="OverDate" onClick="WdatePicker({skin:'whyGreen',dateFmt:'yyyy-MM-dd HH:mm:ss'})"  value="<%=OverDate %>" >
                </td>
            </tr>
            <tr>
                <td class="listTdLeft" >窗口大小：</td>
                <td>宽度：<input type=text name="Width" class="text wd1" value="<%=Width %>" />
                </td>
            </tr>
            <tr>
                <td class="listTdLeft"></td>
                <td>
                    高度：<input type=text name="Height" class="text wd1" value="<%=Height %>"  />
                </td>
            </tr>
         </table>
         <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
             <tr>
                 <td height="39" class="titleOpBg">
                     <input type="submit" value="保存" class="btn wd1" />
                     <input class="btnLine" type="button" />
                     <input type="button" value="返回" class="btn wd1" onclick="Redirect('NoticeList.asp')" />
                 </td>
             </tr>
         </table>
     </form> 
     <%
        End Sub
      %>
</body>
</html>
