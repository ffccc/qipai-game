
 <script type="text/javascript" src="../../Js/common.js"></script>
<tr>
    <td class="listTdLeft">全选<input type="checkbox" onclick="SelectAllTable(this.checked,'userOrder');" />    用户权限：</td>
    <td>
        <table border="0" style="padding:5px 5px 5px 0;" cellpadding="0" cellspacing="0" id="userOrder">
        <tr>
            <td><input name="in_UserRight" id="in_UserRight1" type="checkbox" value="1"<% If rs("UserRight") And 1 Then %> checked='checked'<% End If %> /><label for="in_UserRight1">不能进行游戏</label></td>           
            <td><input name="in_UserRight" id="in_UserRight2" type="checkbox" value="2"<% If rs("UserRight") And 2 Then %> checked='checked'<% End If %>/><label for="in_UserRight2">不能旁观游戏</label></td>
            <td><input name="in_UserRight" id="in_UserRight3" type="checkbox" value="4"<% If rs("UserRight") And 4 Then %> checked='checked'<% End If %>/><label for="in_UserRight3">不能发送私聊</label></td>
            <td><input name="in_UserRight" id="in_UserRight4" type="checkbox" value="8"<% If rs("UserRight") And 8 Then %> checked='checked'<% End If %>/><label for="in_UserRight4">不能大厅聊天</label></td>
            <td><input name="in_UserRight" id="in_UserRight5" type="checkbox" value="16"<% If rs("UserRight") And 16 Then %> checked='checked'<% End If %>/><label for="in_UserRight5">不能游戏聊天</label></td>
            <td><input name="in_UserRight" id="in_UserRight6" type="checkbox" value="256"<% If rs("UserRight") And 256 Then %> checked='checked'<% End If %>/><label for="in_UserRight6">游戏双倍积分</label></td>
        </tr> 
        <tr>
            <td><input name="in_UserRight" id="in_UserRight7" type="checkbox" value="512"<% If rs("UserRight") And 512 Then %> checked='checked'<% End If %>/><label for="in_UserRight7">游戏踢出用户</label></td>
            <td><input name="in_UserRight" id="in_UserRight8" type="checkbox" value="268435456"<% If rs("UserRight") And 268435456 Then %> checked='checked'<% End If %>/><label for="in_UserRight8">游戏比赛用户</label></td>
        </tr>
        </table>
    </td>
</tr>
<tr>
    <td class="listTdLeft">管理级别：</td>
    <td>
        <input name="in_MasterOrder" id="in_MasterOrder1" type="radio" value="0"<% If rs("MasterOrder")=0 Then %>checked='checked'<% End If %> /><label for="in_MasterOrder1">普通玩家</label>
        <input name="in_MasterOrder" id="in_MasterOrder2" type="radio" value="1"<% If rs("MasterOrder")=1 Then %> checked='checked'<% End If %> /><span style="color:#105399;font-weight:bold;"><label for="in_MasterOrder2">管理员</label></span>
    </td>
</tr>
<tr>
    <td class="listTdLeft">全选<input type="checkbox"  onclick="SelectAllTable(this.checked,'masterOrder');"/>    管理权限：</td>
    <td>
        <table border="0" style="padding:5px 5px 5px 0;" cellpadding="0" cellspacing="0" id="masterOrder">
        <tr>
            <td><input name="in_MasterRight" id="in_MasterRight1" type="checkbox" value="1"<% If rs("MasterRight") And 1 Then %> checked='checked'<% End If %>/><label for="in_MasterRight1">允许禁止游戏</label></td>           
            <td><input name="in_MasterRight" id="in_MasterRight2" type="checkbox" value="2"<% If rs("MasterRight") And 2 Then %> checked='checked'<% End If %>/><label for="in_MasterRight2">允许禁止旁观</label></td>
            <td><input name="in_MasterRight" id="in_MasterRight3" type="checkbox" value="4"<% If rs("MasterRight") And 4 Then %> checked='checked'<% End If %>/><label for="in_MasterRight3">允许禁止私聊</label></td>
            <td><input name="in_MasterRight" id="in_MasterRight4" type="checkbox" value="8"<% If rs("MasterRight") And 8 Then %> checked='checked'<% End If %>/><label for="in_MasterRight4">允许禁止聊天</label></td>
            <td><input name="in_MasterRight" id="in_MasterRight5" type="checkbox" value="16"<% If rs("MasterRight") And 16 Then %> checked='checked'<% End If %>/><label for="in_MasterRight5">允许禁止聊天</label></td>
            <td><input name="in_MasterRight" id="in_MasterRight6" type="checkbox" value="256"<% If rs("MasterRight") And 256 Then %> checked='checked'<% End If %>/><label for="in_MasterRight6">允许踢出用户</label></td>
        </tr> 
        <tr>
            <td><input name="in_MasterRight" id="in_MasterRight7" type="checkbox" value="512"<% If rs("MasterRight") And 512 Then %> checked='checked'<% End If %>/><label for="in_MasterRight7">允许查看地址</label></td>           
            <td><input name="in_MasterRight" id="in_MasterRight8" type="checkbox" value="1024"<% If rs("MasterRight") And 1024 Then %> checked='checked'<% End If %>/><label for="in_MasterRight8">允许解散游戏</label></td>
            <td><input name="in_MasterRight" id="in_MasterRight9" type="checkbox" value="4096"<% If rs("MasterRight") And 4096 Then %> checked='checked'<% End If %>/><label for="in_MasterRight9">允许禁止地址</label></td>
            <td><input name="in_MasterRight" id="in_MasterRight10" type="checkbox" value="8192"<% If rs("MasterRight") And 8192 Then %> checked='checked'<% End If %>/><label for="in_MasterRight10">允许禁止机器</label></td>
            <td><input name="in_MasterRight" id="in_MasterRight11" type="checkbox" value="16384"<% If rs("MasterRight") And 16384 Then %> checked='checked'<% End If %>/><label for="in_MasterRight11">允许发送警告</label></td>
            <td><input name="in_MasterRight" id="in_MasterRight12" type="checkbox" value="32768"<% If rs("MasterRight") And 32768 Then %> checked='checked'<% End If %>/><label for="in_MasterRight12">允许修改积分</label></td>
        </tr> 
        <tr>
            <td><input name="in_MasterRight" id="in_MasterRight13" type="checkbox" value="65536"<% If rs("MasterRight") And 65536 Then %> checked='checked'<% End If %>/><label for="in_MasterRight13">允许封锁帐号</label></td>           
            <td><input name="in_MasterRight" id="in_MasterRight14" type="checkbox" value="1048576"<% If rs("MasterRight") And 1048576 Then %> checked='checked'<% End If %>/><label for="in_MasterRight14">允许游戏绑定</label></td>
            <td><input name="in_MasterRight" id="in_MasterRight15" type="checkbox" value="2097152"<% If rs("MasterRight") And 2097152 Then %> checked='checked'<% End If %>/><label for="in_MasterRight15">允许全局绑定</label></td>            
            <td><input name="in_MasterRight" id="in_MasterRight16" type="checkbox" value="16777216"<% If rs("MasterRight") And 16777216 Then %> checked='checked'<% End If %>/><label for="in_MasterRight16">允许发布消息</label></td>
            <td><input name="in_MasterRight" id="in_MasterRight17" type="checkbox" value="33554432"<% If rs("MasterRight") And 33554432 Then %> checked='checked'<% End If %>/><label for="in_MasterRight17">允许管理房间</label></td>            
            <td><input name="in_MasterRight" id="in_MasterRight18" type="checkbox" value="67108864"<% If rs("MasterRight") And 67108864 Then %> checked='checked'<% End If %>/><label for="in_MasterRight18">允许管理配置</label></td>            
        </tr> 
        <tr>
            <td><input name="in_MasterRight" id="in_MasterRight19" type="checkbox" value="134217728"<% If rs("MasterRight") And 134217728 Then %> checked='checked'<% End If %>/><label for="in_MasterRight19">允许管理机器</label></td>           
        </tr>
        </table>
    </td>
</tr> 
