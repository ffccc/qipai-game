<!--#include file="../../CommonFun.asp"-->
<!--#include file="../../function.asp"-->
<!--#include file="../../GameConn.asp"-->
<!--#include file="../../conn.asp"-->

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" >
<head runat="server">
    <title>无标题页</title>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <link href="../../Css/layout.css" rel="stylesheet" type="text/css" />
    <script type="text/javascript" src="../../Js/common.js"></script>
    <script type="text/javascript" src="../../Js/comm.js"></script>
    <script type="text/javascript" src="../../Js/Check.js"></script>
    <script type="text/javascript" src="../../Js/Calendar.js"></script>
     <script type="text/javascript">
        function CheckFormInfo2(siteid)
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
                else
                {
                    document.myFormInfo2.action="?siteid="+siteid
                    document.myFormInfo2.submit();
                }
            }
        }
    </script>
</head>

<%
    If CxGame.GetRoleValue("308",trim(session("AdminName")))<"1" Then
    response.redirect "/Index.asp"   
    End If
%>
<body>
     <!-- 头部菜单 Start -->
    <table width="100%" border="0" cellpadding="0" cellspacing="0" class="title">
        <tr>
            <td width="19" height="25" valign="top"  class="Lpd10"><div class="arr"></div></td>
            <td width="1232" height="25" valign="top" align="left">你当前位置：后台系统 - 泡点设置</td>
        </tr>
    </table>
    <% 
    
        Call ConnectGame(QPPlatformManagerDB)
        Select case lcase(Request("action"))
            case "save"
            Call Save()
            case "new"
            Call NewTimeCounts(Request("siteid"))
            case "savetimes"
            Call SaveTimes()
            case else
            Call Main()
        End Select
        
        '删除
        Sub DeleteUserInfo(lID)
            Dim rs,sql,ID
            ID = Trim(lID)
            GameConn.execute "delete from GrantTimeCountsInfo where GrantID="&ID    
            Call CxGame.ShowInfo2("删除成功！","GrantTimeCountsInfo.asp?siteid="&Request("siteid"),1200)         
        End Sub
        
        Sub Save()
            Dim IsSuccess
            IsSuccess=1
            Response.Write CInt(Request("in_PlayRCCounts"))
            For i=0 To CInt(Request("in_PlayRCCounts"))-1
                If Request("in_GrantCouts"&i)<>"" and Request("in_GrantGameScore"&i)<>"" and Request("in_GrantScore"&i)<>""  and Request("in_GrantLoveliness"&i)<>"" and Request("in_GrantExp"&i)<>"" Then
                    GameConn.execute "update GrantTimeCountsInfo set GrantCouts='"&Request("in_GrantCouts"&i)&"',GrantGameScore="&Request("in_GrantGameScore"&i)&",GrantScore="&Request("in_GrantScore"&i)&",GrantLoveliness="&Request("in_GrantLoveliness"&i)&" ,GrantExp="&Request("in_GrantExp"&i)&" where GrantCouts="&Request("in_GrantCouts"&i)&" and siteid="&Request("siteid")&" and GrantType="&Request("in_IsPaly"&i)  
                Else
                    IsSuccess=0
                    CxGame.MessageBox("资料不完整，请重新更新！")   
                    Call Main()  
                End If
            Next
            If IsSuccess=1 Then
                For i=0 To CInt(Request("in_OnlineRCCounts"))-1
                    If Request("in_PGrantCouts"&i)<>"" and Request("in_PGrantGameScore"&i)<>"" and Request("in_PGrantScore"&i)<>"" and Request("in_PGrantLoveliness"&i)<>"" and Request("in_PGrantExp"&i)<>"" Then
                        GameConn.execute "update GrantTimeCountsInfo set GrantCouts='"&Request("in_PGrantCouts"&i)&"',GrantGameScore="&Request("in_PGrantGameScore"&i)&",GrantScore="&Request("in_PGrantScore"&i)&",GrantLoveliness="&Request("in_PGrantLoveliness"&i)&",GrantExp="&Request("in_PGrantExp"&i)&" where GrantCouts="&Request("in_PGrantCouts"&i)&" and siteid="&Request("siteid")&" and GrantType="&Request("in_PIsPaly"&i)
                     Else
                        IsSuccess=0
                        CxGame.MessageBox("资料不完整，请重新更新！")   
                        Call Main() 
                    End If
                Next
            End If
             If IsSuccess=1 Then
                For i=0 To CInt(Request("in_AllRCCounts"))-1
                    If Request("in_AGrantCouts"&i)<>"" and Request("in_AGrantGameScore"&i)<>"" and Request("in_AGrantScore"&i)<>"" and Request("in_AGrantLoveliness"&i)<>"" and Request("in_AGrantExp"&i)<>"" Then
                        GameConn.execute "update GrantTimeCountsInfo set GrantCouts='"&Request("in_AGrantCouts"&i)&"',GrantGameScore="&Request("in_AGrantGameScore"&i)&",GrantScore="&Request("in_AGrantScore"&i)&",GrantLoveliness="&Request("in_AGrantLoveliness"&i)&",GrantExp="&Request("in_AGrantExp"&i)&" where GrantCouts="&Request("in_AGrantCouts"&i)&" and siteid="&Request("siteid")&" and GrantType="&Request("in_AIsPaly"&i)
                     Else
                        IsSuccess=0
                        CxGame.MessageBox("资料不完整，请重新更新！")   
                        Call Main() 
                    End If
                Next
            End If
            If IsSuccess=1 Then
                Call CxGame.ShowInfo2("更新成功！","GrantTimeCountsInfo.asp?siteid="&Request("siteid"),1200)  
            End If
            
        End Sub 
        
        Sub savetimes()
            Dim rs,sql
            Set rs=Server.CreateObject("Adodb.RecordSet")
            sql="select * from  GrantTimeCountsInfo where GrantCouts="&Request("in_GrantCouts")&" and GrantType="&Request("in_GrantType")&" and siteid="&Request("siteid")  
            rs.Open sql,GameConn,1,3
            dim GrantCouts,GrantScore,GrantGameScore,GrantLoveliness,GrantExp
            If Request("in_GrantCouts")<>"" Then
                GrantCouts=Request("in_GrantCouts")
            Else 
                GrantCouts=0
            End If
            
            If Request("in_GrantScore")<>"" Then
                GrantScore=Request("in_GrantScore")
            Else 
                GrantScore=0
            End If
            
            If Request("in_GrantGameScore")<>"" Then
                GrantGameScore=Request("in_GrantGameScore")
            Else 
                GrantGameScore=0
            End If
            
            If Request("in_GrantLoveliness")<>"" Then
                GrantLoveliness=Request("in_GrantLoveliness")
            Else 
                GrantLoveliness=0
            End If
            
            If Request("in_GrantExp")<>"" Then
                GrantExp=Request("in_GrantExp")
            Else 
                GrantExp=0
            End If
            
            If rs.BOF and rs.EOF Then
                Response.Write "insert into GrantTimeCountsInfo (GrantCouts,GrantScore,GrantGameScore,GrantLoveliness,GrantExp,GrantType,siteid) values ("&GrantCouts&","&GrantScore&","&GrantGameScore&","&GrantLoveliness&","&GrantExp&","&Request("in_GrantType")&","&Request("siteid")&")"
                GameConn.execute "insert into GrantTimeCountsInfo (GrantCouts,GrantScore,GrantGameScore,GrantLoveliness,GrantExp,GrantType,siteid) values ("&GrantCouts&","&GrantScore&","&GrantGameScore&","&GrantLoveliness&","&GrantExp&","&Request("in_GrantType")&","&Request("siteid")&")"
            Call CxGame.ShowInfo2("新增成功！","GrantTimeCountsInfo.asp?siteid="&Request("siteid"),1200)   
            Else
                Call CxGame.ShowInfo2("新增失败，时长重复！","GrantTimeCountsInfo.asp?action=new&siteid="&Request("siteid"),1200)
            End If
            Call Main() 
        End Sub 
        
        Sub Main()
            Dim cidArray, lLoop
            cidArray = Split(Request("cid"),",")
            For lLoop=0 To UBound(cidArray)
                Call DeleteUserInfo(cidArray(lLoop))
            Next 
            Dim rs,sql
            Dim GrantRoom,GrantStartDate,GrantEndDate,GrantObjet,MaxGrant
            Set rs=server.createobject("adodb.recordset")    
            sql="select *  from  GrantTimeCountsInfo Where GrantType=1 and siteid="&Request("siteid")&"  Order by GrantCouts ASC "     
            rs.open sql,GameConn,1,3
            
    %>
    <script type="text/javascript">
        function CheckFormInfo()
        {
            var m_list_table =  window.document.getElementById("tbTimeCounts");
            var m_list_text = GelTags("input", m_list_table);
            for (var i = m_list_text.length - 1; i >= 0; i--) {
                if(IsPositiveInt(m_list_text[i].value)==false)
                {
                    alert("填入数据非数字字符或者超过允许最大大小！");
                    return false;
                }
            }
        }
    </script>
    <form name="myFormInfo2" method="post" action='?action=save&siteid=<%=Request("siteid") %>' onsubmit="return CheckFormInfo()">
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
                <td class="titleOpBg Lpd10"> 
                    <input type="button" value="返回" class="btn wd1" onclick="Redirect('GrantList.asp')" />
                    <input type="submit" value="保存"  class="btn wd1" />  
                    <input type="button" value="新增" class="btn wd1" onclick="Redirect('GrantTimeCountsInfo.asp?action=new&siteid=<%=Request("siteid") %>')" />
                    <input type="button" value="删除" class="btn wd1" onclick="CheckFormInfo2('<%=Request("siteid") %>')" />  
                </td>
            </tr>
        </table>
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="listBg2" id="tbTimeCounts">
            <tr>
                <td height="35" colspan="4" class="f14 bold Lpd10 Rpd10"><div class="hg3  pd7">根据游戏时间设置</div><input type=hidden name="in_PlayRCCounts" value="<%=rs.RecordCount%>" /><input type=hidden name="siteid" value="<%=Request("siteid") %>" /></td>
            </tr>
            <%
                If rs.RecordCount=0 Then
                    Response.Write("<tr class='tdbg'><td colspan='100' align='center'><br>没有任何信息!<br><br></td></tr>")
                Else
                Dim i
                i=0
                do until rs.eof 
            %>
            <tr>
                <td class="listTdLeft"><input name='cid' type='checkbox' value='<%=rs("GrantID")%>'/>时段<%=i+1%>： </td>
                <td colspan="4">
                    <input name="in_GrantCouts<%=i%>" type="text" class="text" style="width:100px;" value="<%=rs("GrantCouts") %>"  />                
                </td>
            </tr>
            <tr>
                <td class="listTdLeft"><input type=hidden name="in_IsPaly<%=i%>" value="<%=rs("GrantType") %>" /></td>
                <td>
                    赠送金币：<input name="in_GrantGameScore<%=i%>" type="text" class="text" style="width:100px;" value="<%=rs("GrantGameScore") %>" />                
                </td>
                <td>
                    赠送积分：<input name="in_GrantScore<%=i%>" type="text" class="text" style="width:100px;" value="<%=rs("GrantScore") %>" />                
                </td>
                <td>
                    赠送魅力：<input name="in_GrantLoveliness<%=i%>" type="text" class="text" style="width:100px;" value="<%=rs("GrantLoveliness") %>" />                
                </td>
                 <td>
                    赠送经验：<input name="in_GrantExp<%=i%>" type="text" class="text" style="width:100px;" value="<%=rs("GrantExp") %>" />                
                </td>
            </tr>
            <%
                i=i+1
                rs.movenext
                loop
                rs.Close()
                Set rs=nothing
                End If
                Dim rs1
                Dim j
                j=0
                Set rs1=server.createobject("adodb.recordset")    
                sql="select *  from  GrantTimeCountsInfo Where GrantType=0 and siteid="&Request("siteid")&"  Order by GrantCouts ASC "     
                rs1.open sql,GameConn,1,3
            %>
            <tr>
                <td height="35" colspan="4" class="f14 bold Lpd10 Rpd10"><div class="hg3  pd7">根据在线时间设置</div><input type=hidden name="in_OnlineRCCounts" value="<%=rs1.RecordCount %>" /></td>
            </tr>
            <%
                If rs1.RecordCount=0 Then
                    Response.Write("<tr class='tdbg'><td colspan='100' align='center'><br>没有任何信息!<br><br></td></tr>")
                Else
                do until rs1.eof
            %>
            <tr>
                <td class="listTdLeft"><input name='cid' type='checkbox' value='<%=rs1("GrantID")%>'/>时段<%=j+1%>：</td>
                <td colspan="4">
                    <input name="in_PGrantCouts<%=j%>" type="text" class="text" style="width:100px;" value="<%=rs1("GrantCouts") %>" />                
                </td>
            </tr>
            <tr>
                <td class="listTdLeft"><input type="hidden" name="in_PIsPaly<%=j%>" value="<%=rs1("GrantType") %>" /> </td>
                <td>
                    赠送金币：<input name="in_PGrantGameScore<%=j%>" type="text" class="text" style="width:100px;" value="<%=rs1("GrantGameScore") %>" />                
                </td>
                <td>
                    赠送积分：<input name="in_PGrantScore<%=j%>" type="text" class="text" style="width:100px;" value="<%=rs1("GrantScore") %>" />                
                </td>
                <td>
                    赠送魅力：<input name="in_PGrantLoveliness<%=j%>" type="text" class="text" style="width:100px;" value="<%=rs1("GrantLoveliness") %>" />                
                </td>
                 <td>
                    赠送经验：<input name="in_PGrantExp<%=j%>" type="text" class="text" style="width:100px;" value="<%=rs1("GrantExp") %>" />                
                </td>
            </tr>
            <%
                j=j+1
                rs1.movenext
                loop
                rs1.Close()
                Set rs1=nothing
                End If
                Dim rs2
                Dim h
                h=0
                Set rs2=server.createobject("adodb.recordset")    
                sql="select *  from  GrantTimeCountsInfo Where GrantType=2 and siteid="&Request("siteid")&"  Order by GrantCouts ASC "     
                rs2.open sql,GameConn,1,3
            %>
              <tr>
                <td height="35" colspan="4" class="f14 bold Lpd10 Rpd10"><div class="hg3  pd7">根据整体时间设置</div><input type=hidden name="in_AllRCCounts" value="<%=rs2.RecordCount %>" /></td>
            </tr>
            <%
                If rs2.RecordCount=0 Then
                    Response.Write("<tr class='tdbg'><td colspan='100' align='center'><br>没有任何信息!<br><br></td></tr>")
                Else
                do until rs2.eof
            %>
            <tr>
                <td class="listTdLeft"><input name='cid' type='checkbox' value='<%=rs2("GrantID")%>'/>时段<%=h+1%>：</td>
                <td colspan="4">
                    <input name="in_AGrantCouts<%=h%>" type="text" class="text" style="width:100px;" value="<%=rs2("GrantCouts") %>" />                
                </td>
            </tr>
            <tr>
                <td class="listTdLeft"><input type="hidden" name="in_AIsPaly<%=h%>" value="<%=rs2("GrantType") %>" /> </td>
                <td>
                    赠送金币：<input name="in_AGrantGameScore<%=h%>" type="text" class="text" style="width:100px;" value="<%=rs2("GrantGameScore") %>" />                
                </td>
                <td>
                    赠送积分：<input name="in_AGrantScore<%=h%>" type="text" class="text" style="width:100px;" value="<%=rs2("GrantScore") %>" />                
                </td>
                <td>
                    赠送魅力：<input name="in_AGrantLoveliness<%=h%>" type="text" class="text" style="width:100px;" value="<%=rs2("GrantLoveliness") %>" />                
                </td>
                 <td>
                    赠送经验：<input name="in_AGrantExp<%=h%>" type="text" class="text" style="width:100px;" value="<%=rs2("GrantExp") %>" />                
                </td>
            </tr>
            <%
                h=h+1
                rs2.movenext
                loop
                rs2.Close()
                Set rs2=nothing
                End If
            %>
        </table>
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
                <td class="titleOpBg Lpd10"> 
                    <input type="button" value="返回" class="btn wd1" onclick="Redirect('GrantList.asp')" />
                    <input type="submit" value="保存"  class="btn wd1" />  
                    <input type="button" value="新增" class="btn wd1" onclick="Redirect('GrantTimeCountsInfo.asp?action=new&siteid=<%=Request("siteid") %>')" />
                    <input type="button" value="删除" class="btn wd1" />   
                </td>
            </tr>
        </table>
    </form>
    <% 
        End Sub
        
        Sub NewTimeCounts(siteid)
    %>
     
    <script type="text/javascript">
        function CheckFormInfo()
        {
            var grantCouts = document.myFormInfo1.in_GrantCouts.value.trim();
            var grantGameScore = document.myFormInfo1.in_GrantGameScore.value.trim();
            var grantScore = document.myFormInfo1.in_GrantScore.value.trim();
            var grantLoveliness = document.myFormInfo1.in_GrantLoveliness.value.trim();
            var grantExp = document.myFormInfo1.in_GrantExp.value.trim();
            if(grantCouts=="")
            {
                alert("时长不能为空！");
                return false;
            }
            if(grantCouts!="")
            {
                if(IsPositiveInt(grantCouts)==false)
                {
                    alert("时长非数值型字符！");
                    return false;
                }
            }
            if(grantGameScore!="")
            {
                if(IsPositiveInt(grantGameScore)==false)
                {
                    alert("赠送金币数目非数值型字符！");
                    return false;
                }
            }
            if(grantScore!="")
            {
                if(IsPositiveInt(grantScore)==false)
                {
                    alert("赠送积分数目非数值型字符！");
                    return false;
                }
            }
            if(grantLoveliness!="")
            {
                if(IsPositiveInt(grantLoveliness)==false)
                {
                    alert("赠送魅力数目非数值型字符！");
                    return false;
                }
            }
            if(grantExp!="")
            {
                if(IsPositiveInt(grantExp)==false)
                {
                    alert("赠送经验数目非数值型字符！");
                    return false;
                }
            }
        }
    </script>
    <form name="myFormInfo1" method="post" action='?action=savetimes&siteid=<%=siteid %>' onsubmit="return CheckFormInfo()">
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
                <td class="titleOpBg Lpd10"> 
                    <input type="submit" value="保存" class="btn wd1" />  
                    <input type="button" value="返回" class="btn wd1" onclick="Redirect('GrantTimeCountsInfo.asp?siteid=<%=siteid %>')" /><input type=hidden name="siteid" value="<%=siteid %>" />
                </td>
            </tr>
        </table>
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="listBg2">
             <tr>
                <td height="35" colspan="2" class="f14 bold Lpd10 Rpd10"><div class="hg3  pd7">赠送类型</div></td>
            </tr>
            <tr>
                <td class="listTdLeft">类型：</td>
                <td>
                   <input name="in_GrantType"  type="radio" checked="checked" value="0" />根据在线时间
                   <input name="in_GrantType" type="radio"  value="1"/>根据游戏时间
                   <input name="in_GrantType" type="radio"  value="2"/>根据整体时间
                </td>
            </tr>
            <tr>
                <td height="35" colspan="2" class="f14 bold Lpd10 Rpd10"><div class="hg3  pd7">赠送时长</div></td>
            </tr>
             <tr>
                <td class="listTdLeft">时长(单位秒)</td>
                <td>
                 <input name="in_GrantCouts" type="text" class="text" style="width:100px;" /> 
                </td>
            </tr>
            <tr>
                <td height="35" colspan="2" class="f14 bold Lpd10 Rpd10"><div class="hg3  pd7">赠送金币</div></td>
            </tr>
            <tr>
                <td class="listTdLeft">数目</td>
                <td>
                   <input name="in_GrantGameScore" type="text" class="text" style="width:100px;" /> 
                </td>
            </tr>
             <tr>
                <td height="35" colspan="2" class="f14 bold Lpd10 Rpd10"><div class="hg3  pd7">赠送积分</div></td>
            </tr>
            <tr>
                <td class="listTdLeft">数目</td>
                <td>
                   <input name="in_GrantScore" type="text" class="text" style="width:100px;" /> 
                </td>
            </tr>
             <tr>
                <td height="35" colspan="2" class="f14 bold Lpd10 Rpd10"><div class="hg3  pd7">赠送魅力</div></td>
            </tr>
            <tr>
                <td class="listTdLeft">数目</td>
                <td>
                   <input name="in_GrantLoveliness" type="text" class="text" style="width:100px;" /> 
                </td>
            </tr>
             <tr>
                <td height="35" colspan="2" class="f14 bold Lpd10 Rpd10"><div class="hg3  pd7">赠送经验</div></td>
            </tr>
            <tr>
                <td class="listTdLeft">数目</td>
                <td>
                   <input name="in_GrantExp" type="text" class="text" style="width:100px;" /> 
                </td>
            </tr>
        </table>
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
                <td class="titleOpBg Lpd10"> 
                    <input type="submit" value="保存" class="btn wd1" />  
                    <input type="button" value="返回" class="btn wd1" onclick="Redirect('GrantTimeCountsInfo.asp?siteid=<%=siteid %>')" />
                </td>
            </tr>
        </table>
    </form>
    <%
        End Sub
    %>
</body>
</html>