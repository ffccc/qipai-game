<!--#include file="../../CommonFun.asp"-->
<!--#include file="../../GameConn.asp"-->
<!--#include file="../../Cls_Page.asp"-->
<!--#include file="../../Conn.asp"-->
<!--#include file="../../md5.asp"-->
<!--#include FILE="../../Inc/upload_5xsoft.inc"-->
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" >
<head runat="server">
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <title></title>
    <link href="../../Css/layout.css" rel="stylesheet" type="text/css" />
    <script type="text/javascript" src="../../Js/common.js"></script>
    <script type="text/javascript" src="../../Js/Sort.js"></script>
    
</head>

	
<body>
   
     <!-- 头部菜单 Start -->
     
    <table width="100%" border="0" cellpadding="0" cellspacing="0" class="title">
        <tr>
            <td width="19" height="25" valign="top"  class="Lpd10"><div class="arr"></div></td>
            <td width="1232" height="25" valign="top" align="left">你当前位置：管理中心 - 规则管理</td>
        </tr>
    </table>
    <!-- 头部菜单 End -->  
     <%
     
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
            GameConn.execute "Delete From GameRulesInfo Where KindID="&ID
	        fileurl="../../Upload/gamehelp"&"/"&"gameimg_"&Request("id")&"."&Request("imgtype")
	        Set deleteFileObject=Server.CreateObject("Scripting.FileSystemObject")
            delefilepath=Server.MapPath(fileurl)
            If deleteFileObject.FileExists(delefilepath) then
                Set delefilepath=deleteFileObject.GetFile(delefilepath)
                delefilepath.Delete
            End If
	        Call CxGame.ShowInfo2("删除成功！","RoleList.asp",1200)
        End Sub
        
        '新增,更新
	    Sub Save()
	        Dim fileurl,rs,sql,rs1,rql1,rs2,rql2,n,kindName,sql_name,rs_name
	        Set rs1=Server.CreateObject("Adodb.RecordSet")
	        Set rs_name=Server.CreateObject("Adodb.RecordSet")
	        If Request.TotalBytes<204800 Then
	            Set upload = New upload_5xsoft 
                Set file = upload.file("file1")  
                n = right(file.FileName,4)
                
	            If upload.form("in_KindID")="" Then
	                sql1="select * from  GameRulesInfo where KindID="&upload.form("KindID")
	            Else
	                sql1="select * from  GameRulesInfo where KindID="&upload.form("in_KindID")&" AND KindID<>"&upload.form("KindID")
	               
	            End IF
	            sql_name="select KindName from QPPlatformDBLink.QPPlatformDB.dbo.GameKindItem where KindID ='"&upload.form("KindID")&"'"
                rs_name.Open sql_name,GameConn,1,3
                kindName=rs_name("KindName")
	            rs1.Open sql1,GameConn,1,3
	            If rs1.BOF and rs1.EOF Then
	                If upload.form("in_KindID")="" Then
                        f= Server.MapPath("../../Upload/gamehelp")&"\\"
                        fileurl="/Upload/gamehelp/"&"gameimg_"&upload.form("KindID")&n
                        Set rs=Server.CreateObject("Adodb.RecordSet")
                        sql="Insert into GameRulesInfo(KindID,KindName,ImgRuleUrl,HelpIntro,HelpRule,HelpGrade,CollectDate) values ('"&upload.form("KindID")&"','"&kindName&"','"&fileurl&"','"&upload.form("HelpIntro")&"','"&upload.form("content1")&"','"&upload.form("content2")&"','"&Now()&"')"
                        rs.Open sql,GameConn,1,3
                        Call CxGame.ShowInfo2("新增成功！","RoleList.asp",1200)
                        file.SaveAs f&"gameimg_"&upload.form("KindID")&n  
                        session("image")="gameimg_"&upload.form("KindID")&n
                        Set rs = nothing
                    Else 
                        fileurl="/Upload/gamehelp"&"/"&"gameimg_"&upload.form("KindID")&"."&upload.form("in_imgtype")
	                    Set deleteFileObject=Server.CreateObject("Scripting.FileSystemObject")
                        delefilepath=Server.MapPath(fileurl)
                        If deleteFileObject.FileExists(delefilepath) then
                            Set delefilepath=deleteFileObject.GetFile(delefilepath)
                            delefilepath.Delete
                        End If
                        f= Server.MapPath("../../Upload/gamehelp")&"\\"
                        fileurl="/Upload/gamehelp/"&"gameimg_"&upload.form("KindID")&n
                        file.SaveAs f&"gameimg_"&upload.form("KindID")&n  
                        session("image")="gameimg_"&upload.form("KindID")&n
                        Set rs2=Server.CreateObject("Adodb.RecordSet")
                        sql2="update  GameRulesInfo set KindID='"&upload.form("KindID")&"',KindName='"&kindName&"',ImgRuleUrl='"&fileurl&"',HelpIntro='"&upload.form("HelpIntro")&"',HelpRule='"&upload.form("content1")&"',HelpGrade='"&upload.form("content2")&"',ModifyDate='"&Now()&"' where KindID="&upload.form("in_KindID")
                        rs2.Open sql2,GameConn,1,3
                        Call CxGame.ShowInfo2("更新成功！","RoleList.asp",1200)
                        Set rs2 = nothing
                    End If
                Else
                Call CxGame.ShowInfo2("游戏标识重复！","RoleList.asp",1200)
                End IF
                Set rs1 = nothing
            Else
                Call CxGame.MessageBoxReload("上传图片不得大于200KB!","RoleList.asp")
            End If
            
        End Sub
        
        Sub Main()
            Dim cidArray, lLoop
            cidArray = Split(Request("cid"),",")
            For lLoop=0 To UBound(cidArray)
                Call DeleteInfo(cidArray(lLoop))
            Next  
            If Request("orders")<>""And Request("OrderType")<>"" Then
                If Request("OrderType")<>"-102" Then
                    OrderStr=Replace(Request("orders")," ","+")&" Asc "
                End If
                If Request.QueryString("OrderType")<>"-101" Then
                    OrderStr=Replace(Request("orders")," ","+")&" Desc "
                End If
            Else 
                OrderStr = "CollectDate Asc"
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
	            .Pkey="roledi"					'主键
	            .Field="*"	'字段
	            .Table="GameRulesInfo"			'表名
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
        function CheckFormList()
        {
            if(!confirm("确定要执行选定的操作吗？"))
            {
                return false;
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
                    return false;
                }
            }
        }
    </script>
    
    <form id="form1" method="post" action="?action=orderinfo">
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" >
            <tr>
                <td height="39" class="titleOpBg">
                    <input type="button" value="新增" class="btn wd1" onclick="Redirect('RoleList.asp?action=newinfo')" />
                    <input class="btnLine" type="button" />
                    <input type="submit" value="删除" class="btn wd1" />     
                    <input type="hidden" name="Action" value="DeleteAll" />            
                </td>
            </tr>
        </table> 
        <div>        
            <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="box">
                <tr align="center" class="bold">
                    <td class="listTitle1" style="width:38px;"><input type="checkbox" name="chkAll" onclick="SelectAll(this.checked);" /></td>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('KindID',this)">游戏标识</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('KindName',this)">游戏名称</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('CollectDate',this)">新增日期</a></th>
                    <th class="listTitle"><a class="l1"  href="" onclick="GetOrder('ModifyDate',this)">修改日期</a></th>
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
                    <td onclick="Redirect('RoleList.asp?action=newinfo&KindID=<%=rs(0,i) %>')"><%=rs(0,i) %></td>    
                    <td onclick="Redirect('RoleList.asp?action=newinfo&KindID=<%=rs(0,i) %>')"><%=rs(1,i) %></td>    
                    <td onclick="Redirect('RoleList.asp?action=newinfo&KindID=<%=rs(0,i) %>')"><%=rs(8,i) %></td> 
                    <td onclick="Redirect('RoleList.asp?action=newinfo&KindID=<%=rs(0,i) %>')"><%=rs(9,i) %></td> 
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
         <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
                <td height="39" class="titleOpBg">
                   <input type="button" value="新增" class="btn wd1" onclick="Redirect('RoleList.asp?action=newinfo')" />
                   <input class="btnLine" type="button" />
                   <input type="submit" value="删除" class="btn wd1" />      
                </td>
            </tr>
        </table>        
    </form>
    <%
        End Sub
        
        Sub NewInfo()
            Dim rs,sql,KindID,KindName,HelpIntro,HelpRule,HelpGrade,RulesID,imgtype
            If(Request("KindID")<>"") Then
                Set rs=server.createobject("adodb.recordset")
                sql="select * from GameRulesInfo where KindID="&Request("KindID")
                rs.open sql,GameConn,1,3
                KindID=rs("KindID")
                KindName=rs("KindName")
                HelpIntro=rs("HelpIntro")
                HelpRule=rs("HelpRule")
                HelpGrade=rs("HelpGrade")
                RulesID=Request("RulesID")
                imgtype=right(rs("ImgRuleUrl"),3)
            Else
                KindID=""
                KindName=""
                HelpIntro=""
                HelpRule=""
                HelpGrade=""
                RulesID=""
                imgtype=""
            End If
    %> 
    <script type="text/javascript" src="../../kindeditor/kindeditor.js"></script>
    <script type="text/javascript">
		KE.show({
			id : 'content1',
			urlType : 'absolute',
			imageUploadJson : '../../asp/upload_json.asp',
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
		
	    KE.show({
			id : 'content2',
			urlType : 'absolute',
			imageUploadJson : '../../asp/upload_json.asp',
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
		    var KindID = document.form1.KindID.value.trim()
		    var content1 = document.form1.content1.value.trim();
		    var KindName = document.form1.KindName.value.trim()
		    var fileurl = document.form1.file1.value.trim()
		    var HelpIntro = document.form1.HelpIntro.value.trim()
		    var content2 = document.form1.content2.value.trim()
		    if(KindID=="")
		    {
		        alert("游戏标识不能为空！");
		        return false;
		    }
		  
		     if(KindName=="")
		    {
		        alert("游戏名称不能为空！");
		        return false;
		    }
		    
		    if(fileurl=="")
		    {
		        alert("未选择上传图片！");
		        return false;
		    }
		    
		     if(HelpIntro=="")
		    {
		        alert("游戏介绍不能为空！");
		        return false;
		    }
		      if(content1=="")
		    {
		        alert("规则介绍不能为空！");
		        return false;
		    }
		    if(content2=="")
		    {
		        alert("等级介绍不能为空！");
		        return false;
		    }
		}
	</script>
    <form name="form1" method="post" action="?action=save&KindID=<%=KindID %>" enctype="multipart/form-data"  onsubmit="return CheckFormInfo();">
         <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" >
             <tr>
                 <td height="39" class="titleOpBg">
                     <input type="submit" value="保存" class="btn wd1" /><input type="hidden" name="in_KindID" value="<%=KindID %>"<input type="hidden" name="in_imgtype" value="<%=imgtype %>"
                     <input class="btnLine" type="button" />
                     <input type="button" value="返回" class="btn wd1" onclick="Redirect('RoleList.asp')" />
                 </td>
             </tr>
         </table>  
         <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="listBg2">
            <tr>
                <td height="35" colspan="2" class="f14 bold Lpd10 Rpd10"><div class="hg3  pd7">规则管理</div></td>
            </tr>
            <tr>
                <td class="listTdLeft">游戏名称：</td>
                <td>
                    <select name="KindID" style="width:159px;">
			            <% 
			                Dim ArrayDB,i
                            ArrayDB = CxGame.GetKindList()
                            For i=0 To Ubound(ArrayDB)    
                            If KindID=ArrayDB(i,0) Then   
			            %>
			            <option value="<%=ArrayDB(i,0) %>" selected="selected"><%=ArrayDB(i,1) %></option>
			            <% Else %>
			            <option value="<%=ArrayDB(i,0) %>"><%=ArrayDB(i,1) %></option>
			            <% 
                            End If                        
                            Next  
                            Set ArrayDB = nothing
                        %>
			        </select>    
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">游戏截图：</td>
                <td>
                    <input name="file1" id="file1" type="file" class="text wd4" style="width:300px;height:20px" />
                </td>
            </tr>
             <tr>
                <td class="listTdLeft">游戏介绍：</td>
                <td>
                    <textarea name="HelpIntro" id="HelpIntro" wrap="virtual" style="height:60px;width:694px" rows="3" class="text wd4"><%=HelpIntro %></textarea>
                </td>
            </tr>
              <tr>
                <td colspan="2" style="height:20px">
                    &nbsp;
                </td>
            </tr>
             <tr>
                <td class="listTdLeft">规则介绍：</td>
                <td>
                    <textarea id="content1"  name="content1" cols="100" rows="8" style="width:700px;height:350px;visibility:hidden;"><%=HelpRule %>
                    </textarea> 
                </td>
            </tr>
            <tr>
                <td colspan="2" style="height:20px">
                    &nbsp;
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">等级介绍：</td>
                <td>
                   <textarea id="content2" name="content2" cols="100" rows="8" style="width:700px;height:350px;visibility:hidden;"><%=HelpGrade %>
                   </textarea> 
                </td>
            </tr>
         </table>
         
         <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
             <tr>
                 <td height="39" class="titleOpBg">
                     <input type="submit" value="保存" class="btn wd1" />
                     <input class="btnLine" type="button" />
                     <input type="button" value="返回" class="btn wd1" onclick="Redirect('RoleList.asp')" />
                 </td>
             </tr>
         </table>
     </form> 
     <%
        End Sub
     %>
</body>
</html>
