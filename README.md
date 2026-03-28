這個程式主要是用來計算MOS和BJT的特性曲線和小訊號分析<br/>
目的是為了解悶.<br/>
不然市面上那麼多免費的SPICE不用，我幹麻要寫這個？<br/>

目前只做好繪製MOS的輸出和轉換曲線，以及存檔的功能<br/>
MOS的公式<br/>
Id =Kn (2*(Vgs-Vth)Vds_eff-Vds_eff^2)(1+λVds)<br/>
 * Kn= 1/2*μn*Cox*W/L<br/>
 * μn: 電子遷移率 (n-channel)<br/>
 * Cox:單位面積閘氧化層電容<br/>
 * W:通道寬度<br/>
 * L:通道長度<br/>
 * vds_eff：<br />
<p>
if (vds < v_sat) {<br/>
     vds_eff = vds;   // 線性區<br/>
  } else {<br/>
     vds_eff = v_sat; // 飽和區 (卡死在上限)<br/>
  }</p>
