<!--玩家排行榜开始-->
<div class="clear"></div>
<div class="serve mtop10">
  <div class="serve1 bold hui3 ">玩家排行榜</div>
	<div class="tab">
		<div class="tab1" id="S_0" onclick="ChangeTab(0,2)">财富排行</div>
		<div class="tab2" id="S_1" onclick="ChangeTab(1,2)">魅力排行</div>
	</div>
	<div class="topBg" id="M_0">
		<%=CxGame.GoldTop(10,8) %>			
	    <div class="clear"></div>
	</div>
	<div class="topBg" id="M_1">
		<%=CxGame.PresentTop(10,12) %>		
	    <div class="clear"></div>
	</div>
	<div class="clear"></div>
</div>
<!--玩家排行榜结束-->
<script type="text/javascript">
    function ChangeTab(idx,count)
    {
        for (i_tr = 0; i_tr < count; i_tr++)
        {
            if (i_tr == idx)
            {
                var tab = document.getElementById('S_'+idx);
                var content = document.getElementById('M_'+idx);
                tab.className="tab1";
                content.style.display="block";
                continue;
            }
            var tab = document.getElementById('S_'+i_tr);
            var content = document.getElementById('M_'+i_tr);
            tab.className="tab2";
            content.style.display="none";
        }
    }
    ChangeTab(0,2);
</script>