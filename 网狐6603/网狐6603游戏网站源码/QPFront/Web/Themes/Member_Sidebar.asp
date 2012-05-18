<script type="text/javascript">    
    function setCookie2(sName, sValue) {
        var expires = new Date();
        expires.setTime(expires.getTime() + 16 * 60 * 1000);
        document.cookie = sName + "=" + escape(sValue) + "; expires=" + expires.toGMTString() + "; path=/";
    }
    
    function getCookie (sName) {
	    var aCookie = document.cookie.split("; ");
	    for (var i=0; i < aCookie.length; i++) {
		    var aCrumb = aCookie[i].split("=");
		    if (sName == aCrumb[0])
			    return unescape(aCrumb[1]);
	    }
	    return null;
    }
    
    function showHide(obj) {
        var oStyle = obj.style;
        var imgId = obj.id.replace("M","S");
//        oStyle.display == "none" ? oStyle.display = "block" : oStyle.display = "none";
//        oStyle.display == "none" ? document.getElementById(imgId).className = "gameListTitle2" : document.getElementById(imgId).className = "gameListTitle1";
        
        if(oStyle.display == "none")
        {
            oStyle.display = "block";
            document.getElementById(imgId).className = "gameListTitle1";
            setCookie2(obj.id,"on")
        }
        else
        {
            oStyle.display = "none";
            document.getElementById(imgId).className = "gameListTitle2";
            setCookie2(obj.id,"off")
        }
    }    
    window.onload = function() {
        for(var i=1;i<5;i++)
        {
            if (getCookie("M_"+i) == null || getCookie("M_"+i) == undefined || getCookie("M_"+i) == "on") 
            {
                document.getElementById("M_"+i).style.display = "block";
                document.getElementById("S_"+i).className = "gameListTitle1";
            }
            else
            {
                document.getElementById("M_"+i).style.display = "none";
                document.getElementById("S_"+i).className = "gameListTitle2";
            }
        }
    }
</script>
<div class="LeftSide mtop10">
<!--游戏列表开始-->
<div class="gameList"><div class="clear"></div></div>
<div class="gameListBg">
	<!--帐号服务开始-->
	<div class="gameListTitle1" id="S_1" onclick="JavaScript:showHide(document.getElementById('M_1'));"><label class="userType1">账号服务</label></div>			
	<div class="clear"></div>
	<ul id="M_1">
	<li><span class="userErr"><a href="MemberModifyNikeName.asp" class="lh">修改昵称</a></span></li>
	<li><span class="userErr"><a href="MemberModifyUserInfo.asp" class="lh">修改资料</a></span></li>
	<li><span class="userErr"><a href="MemberModifyFace.asp" class="lh">修改头像</a></span></li>
	<li><span class="userErr"><a href="MemberModifyLogonPass.asp" class="lh">修改登录密码</a></span></li>
	<li><span class="userErr"><a href="MemberModifyInsurePass.asp" class="lh">修改保险柜密码</a></span></li>
	</ul>
	<!--帐号服务结束-->
	
	<!--帐号安全开始-->
	<div class="gameListTitle1" id="S_2" onclick="JavaScript:showHide(document.getElementById('M_2'));"><label class="userType2">账号安全</label></div>				
	<div class="clear"></div>
	<ul id="M_2">
	<li><span class="userErr"><a href="MemberApplyProtect.asp" class="lh">申请密码保护</a></span></li>
	<li><span class="userErr"><a href="MemberModifyProtect.asp" class="lh">修改密码保护</a></span></li>
	<li><span class="userErr"><a href="MemberReLogonPass.asp" class="lh">找回登录密码</a></span></li>
	<li><span class="userErr"><a href="MemberReInsurePass.asp" class="lh">找回保险柜密码</a></span></li>
	</ul>
	<!--帐号安全结束-->
	
	<!--保险柜服务开始-->				
	<div class="gameListTitle1" id="S_3" onclick="JavaScript:showHide(document.getElementById('M_3'));"><label class="userType3">保险柜服务</label></div>
	<div class="clear"></div>
	<ul id="M_3">
	<li><span class="userErr"><a href="MemberInsureIn.asp" class="lh">存款</a></span></li>
	<li><span class="userErr"><a href="MemberInsureOut.asp" class="lh">取款</a></span></li>
	<li><span class="userErr"><a href="MemberInsureTransfer.asp" class="lh">转帐</a></span></li>
	<li><span class="userErr"><a href="MemberInsureRecord.asp" class="lh">交易明细</a></span></li>
	<li><span class="userErr"><a href="MemberConvertPresent.asp" class="lh">魅力兑换</a></span></li>
	<li><span class="userErr"><a href="MemberConvertRecord.asp" class="lh">魅力兑换记录</a></span></li>
	</ul>
	<!--保险柜服务结束-->
	
	<!--充值中心开始-->	
	<div class="gameListTitle1" id="S_4" onclick="JavaScript:showHide(document.getElementById('M_4'));"><label class="userType4">充值中心</label></div>
	<div class="clear"></div>
	<ul id="M_4">
	<li><span class="userErr"><a href="PayIndex.asp" class="lh">会员介绍</a></span></li>
	<li><span class="userErr"><a href="MemberPayRecord.asp" class="lh">充值记录</a></span></li>
	</ul>
	<!--充值中心结束-->	
		
	<!--会员服务开始-->	
	<div class="gameListTitle1" id="S_5" onclick="JavaScript:showHide(document.getElementById('M_5'));"><label class="userType4">会员服务</label></div>
	<div class="clear"></div>
	<ul id="M_5">
	<li><span class="userErr"><a href="MemberClearScore.asp" class="lh">负分清零</a></span></li>
	<li><span class="userErr"><a href="MemberClearFlee.asp" class="lh">逃跑清零</a></span></li>
	<li><span class="userErr"><a href="MemberGameRecord.asp" class="lh">游戏记录</a></span></li>
	<li><span class="userErr"><a href="MemberMoorMachine.asp" class="lh">固定机器</a></span></li>
	</ul>
	<!--会员服务结束-->	
	
	<!--推广中心开始-->	
	<div class="gameListTitle1" id="S_6" onclick="JavaScript:showHide(document.getElementById('M_6'));"><label class="userType4">推广服务</label></div>
	<div class="clear"></div>
	<ul id="M_6">
	<li><span class="userErr"><a href="MemberSpreadIn.asp" class="lh">业绩查询</a></span></li>
	<li><span class="userErr"><a href="MemberSpreadBalance.asp" class="lh">业绩结算</a></span></li>
	<li><span class="userErr"><a href="MemberSpreadInfo.asp" class="lh">推广明细查询</a></span></li>
	<li><span class="userErr"><a href="MemberSpreadOut.asp" class="lh">结算明细查询</a></span></li>
	</ul>
	<!--推广中心结束-->	
</div>
<div class="gameListEnd"></div>
<!--游戏列表结束-->
</div>		
<div class="clear"></div>