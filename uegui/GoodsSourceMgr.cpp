#ifndef _UEGUI_GOODSSOURCEMGR_H
#include "GoodsSourceMgr.h"
#endif
#include <string.h>
using namespace UeGui;

CmakeDataFiles::CmakeDataFiles():m_goodSourceMgr(CFileController<GoodsSource>::getGoodsSourceCtrl()),
m_LogisticsTrackMgr(CFileController<LogisticsTrackInfo>::getLogisticsTrackCtrl()),
m_GSDetailMgr(CFileController<GoodsSourceDetail>::getGoodsSourceDetailCtrl())
{

}
void CmakeDataFiles::makeGoodsSourceDetail()
{
	m_GSDetailMgr.ConnectToFile();
	if (m_GSDetailMgr.GetCount()<1)
		//if( 1==2 )
	{
		GoodsSourceDetail tmp;
		memset(&tmp,0,sizeof(GoodsSourceDetail));

		tmp.Idx_GoodsSource=0;
		::sprintf( (char *)tmp.goods_type, "食品" );
		::sprintf( (char *)tmp.srcPlace, "顺德区陈村镇石州工业区1号" );
		::sprintf( (char *)tmp.srcQcode, "xjsatj8b1" );
		::sprintf( (char *)tmp.dstPlace, "云南省昆明市县师家村路" );
		::sprintf( (char *)tmp.dstQcode, "03088jcyx" );
		::sprintf( (char *)tmp.sendOutTime, "" );
		tmp.transPrice=0;
		::sprintf( (char *)tmp.limitTime, "" );
		::sprintf( (char *)tmp.payWay, "" );
		::sprintf( (char *)tmp.publishTime, "2012-10-22 13:45" );
		::sprintf( (char *)tmp.publisher, "朱小姐" );
		::sprintf( (char *)tmp.telphone, "15526837898" );
		::sprintf( (char *)tmp.notes, "" );
		m_GSDetailMgr.AddOneItem(tmp);

		tmp.Idx_GoodsSource=1;
		::sprintf( (char *)tmp.goods_type, "建材" );
		::sprintf( (char *)tmp.srcPlace, "顺德区大良环市北路38号" );
		::sprintf( (char *)tmp.srcQcode, "xqnkw97fg" );
		::sprintf( (char *)tmp.dstPlace, "湖南省长沙市冯家冲" );
		::sprintf( (char *)tmp.dstQcode, "33y6k1qma" );
		::sprintf( (char *)tmp.sendOutTime, "" );
		tmp.transPrice=0;
		::sprintf( (char *)tmp.limitTime, "" );
		::sprintf( (char *)tmp.payWay, "" );
		::sprintf( (char *)tmp.publishTime, "2012-10-22 13:45" );
		::sprintf( (char *)tmp.publisher, "" );
		::sprintf( (char *)tmp.telphone, "18688487719" );
		::sprintf( (char *)tmp.notes, "" );
		m_GSDetailMgr.AddOneItem(tmp);

		tmp.Idx_GoodsSource=2;
		::sprintf( (char *)tmp.goods_type, "" );
		::sprintf( (char *)tmp.srcPlace, "广东省佛山市" );
		::sprintf( (char *)tmp.srcQcode, "ddi4vs3c4" );
		::sprintf( (char *)tmp.dstPlace, "云南省楚雄市青龙路223号" );
		::sprintf( (char *)tmp.dstQcode, "0jb112ii9" );
		::sprintf( (char *)tmp.sendOutTime, "" );
		tmp.transPrice=0;
		::sprintf( (char *)tmp.limitTime, "" );
		::sprintf( (char *)tmp.payWay, "" );
		::sprintf( (char *)tmp.publishTime, "" );
		::sprintf( (char *)tmp.publisher, "" );
		::sprintf( (char *)tmp.telphone, "13041996990" );
		::sprintf( (char *)tmp.notes, "求9.6米单桥车或高栏车" );
		m_GSDetailMgr.AddOneItem(tmp);

		tmp.Idx_GoodsSource=3;
		::sprintf( (char *)tmp.goods_type, "机械" );
		::sprintf( (char *)tmp.srcPlace, "顺德容桂建业中路13号" );
		::sprintf( (char *)tmp.srcQcode, "x4m6vfi0n" );
		::sprintf( (char *)tmp.dstPlace, "云南省文山壮族苗族自治州山县金石路" );
		::sprintf( (char *)tmp.dstQcode, "dt0eguhpy" );
		::sprintf( (char *)tmp.sendOutTime, "" );
		tmp.transPrice=0;
		::sprintf( (char *)tmp.limitTime, "" );
		::sprintf( (char *)tmp.payWay, "" );
		::sprintf( (char *)tmp.publishTime, "2012-10-22 13:45" );
		::sprintf( (char *)tmp.publisher, "" );
		::sprintf( (char *)tmp.telphone, "13825099145" );
		::sprintf( (char *)tmp.notes, "" );
		m_GSDetailMgr.AddOneItem(tmp);

		tmp.Idx_GoodsSource=4;
		::sprintf( (char *)tmp.goods_type, "建材" );
		::sprintf( (char *)tmp.srcPlace, "顺德区乐从镇世海钢铁物流3号地H东11号" );
		::sprintf( (char *)tmp.srcQcode, "x1qdk3m9v" );
		::sprintf( (char *)tmp.dstPlace, "江苏省苏州市相城区12栋5号" );
		::sprintf( (char *)tmp.dstQcode, "6k2190at2" );
		::sprintf( (char *)tmp.sendOutTime, "" );
		tmp.transPrice=0;
		::sprintf( (char *)tmp.limitTime, "" );
		::sprintf( (char *)tmp.payWay, "" );
		::sprintf( (char *)tmp.publishTime, "2012-10-22 13:45" );
		::sprintf( (char *)tmp.publisher, "" );
		::sprintf( (char *)tmp.telphone, "13710068629" );
		::sprintf( (char *)tmp.notes, "" );
		m_GSDetailMgr.AddOneItem(tmp);

		tmp.Idx_GoodsSource=5;
		::sprintf( (char *)tmp.goods_type, "" );
		::sprintf( (char *)tmp.srcPlace, "顺德大良康城花园" );
		::sprintf( (char *)tmp.srcQcode, "x4mkcxbnd" );
		::sprintf( (char *)tmp.dstPlace, "河南省新郑市神州路" );
		::sprintf( (char *)tmp.dstQcode, "8skuakw73" );
		::sprintf( (char *)tmp.sendOutTime, "" );
		tmp.transPrice=0;
		::sprintf( (char *)tmp.limitTime, "" );
		::sprintf( (char *)tmp.payWay, "" );
		::sprintf( (char *)tmp.publishTime, "2012-10-22 13:45" );
		::sprintf( (char *)tmp.publisher, "" );
		::sprintf( (char *)tmp.telphone, "13725223815" );
		::sprintf( (char *)tmp.notes, "" );
		m_GSDetailMgr.AddOneItem(tmp);

		tmp.Idx_GoodsSource=6;
		::sprintf( (char *)tmp.goods_type, "" );
		::sprintf( (char *)tmp.srcPlace, "顺德区容桂镇文海中路23号" );
		::sprintf( (char *)tmp.srcQcode, "x7thnai5p" );
		::sprintf( (char *)tmp.dstPlace, "湖北省武汉市江岸区兴业路" );
		::sprintf( (char *)tmp.dstQcode, "5up6w30f2" );
		::sprintf( (char *)tmp.sendOutTime, "" );
		tmp.transPrice=0;
		::sprintf( (char *)tmp.limitTime, "" );
		::sprintf( (char *)tmp.payWay, "" );
		::sprintf( (char *)tmp.publishTime, "2012-10-22 13:45" );
		::sprintf( (char *)tmp.publisher, "" );
		::sprintf( (char *)tmp.telphone, "13376148369" );
		::sprintf( (char *)tmp.notes, "" );
		m_GSDetailMgr.AddOneItem(tmp);

		tmp.Idx_GoodsSource=7;
		::sprintf( (char *)tmp.goods_type, "" );
		::sprintf( (char *)tmp.srcPlace, "乐从镇新隆工业区" );
		::sprintf( (char *)tmp.srcQcode, "xksedav57" );
		::sprintf( (char *)tmp.dstPlace, "北京市东城区民旺北胡同" );
		::sprintf( (char *)tmp.dstQcode, "ceyxw8ttq" );
		::sprintf( (char *)tmp.sendOutTime, "" );
		tmp.transPrice=0;
		::sprintf( (char *)tmp.limitTime, "" );
		::sprintf( (char *)tmp.payWay, "" );
		::sprintf( (char *)tmp.publishTime, "2012-10-22 13:45" );
		::sprintf( (char *)tmp.publisher, "" );
		::sprintf( (char *)tmp.telphone, "13345672168" );
		::sprintf( (char *)tmp.notes, "" );
		m_GSDetailMgr.AddOneItem(tmp);

		tmp.Idx_GoodsSource=8;
		::sprintf( (char *)tmp.goods_type, "" );
		::sprintf( (char *)tmp.srcPlace, "顺德区伦教镇" );
		::sprintf( (char *)tmp.srcQcode, "xwp8pur1t" );
		::sprintf( (char *)tmp.dstPlace, "云南省曲靖市陆良县" );
		::sprintf( (char *)tmp.dstQcode, "0pahhfy80" );
		::sprintf( (char *)tmp.sendOutTime, "" );
		tmp.transPrice=0;
		::sprintf( (char *)tmp.limitTime, "" );
		::sprintf( (char *)tmp.payWay, "" );
		::sprintf( (char *)tmp.publishTime, "2012-10-22 13:45" );
		::sprintf( (char *)tmp.publisher, "" );
		::sprintf( (char *)tmp.telphone, "15346146819" );
		::sprintf( (char *)tmp.notes, "" );
		m_GSDetailMgr.AddOneItem(tmp);

		tmp.Idx_GoodsSource=9;
		::sprintf( (char *)tmp.goods_type, "" );
		::sprintf( (char *)tmp.srcPlace, "顺德乐从" );
		::sprintf( (char *)tmp.srcQcode, "x6t07ujug" );
		::sprintf( (char *)tmp.dstPlace, "辽宁省沈阳市苏家屯区永兴街" );
		::sprintf( (char *)tmp.dstQcode, "fs46h07ya" );
		::sprintf( (char *)tmp.sendOutTime, "" );
		tmp.transPrice=0;
		::sprintf( (char *)tmp.limitTime, "" );
		::sprintf( (char *)tmp.payWay, "" );
		::sprintf( (char *)tmp.publishTime, "2012-10-22 13:45" );
		::sprintf( (char *)tmp.publisher, "" );
		::sprintf( (char *)tmp.telphone, "15302240018" );
		::sprintf( (char *)tmp.notes, "" );
		m_GSDetailMgr.AddOneItem(tmp);

		tmp.Idx_GoodsSource=10;
		::sprintf( (char *)tmp.goods_type, "" );
		::sprintf( (char *)tmp.srcPlace, "顺德区乐从镇" );
		::sprintf( (char *)tmp.srcQcode, "x9rfmjkyb" );
		::sprintf( (char *)tmp.dstPlace, "四川省绵阳市涪城区临园路西段" );
		::sprintf( (char *)tmp.dstQcode, "6a9ivb1gk" );
		::sprintf( (char *)tmp.sendOutTime, "" );
		tmp.transPrice=0;
		::sprintf( (char *)tmp.limitTime, "" );
		::sprintf( (char *)tmp.payWay, "" );
		::sprintf( (char *)tmp.publishTime, "2012-10-22 13:45" );
		::sprintf( (char *)tmp.publisher, "" );
		::sprintf( (char *)tmp.telphone, "13708465724" );
		::sprintf( (char *)tmp.notes, "" );
		m_GSDetailMgr.AddOneItem(tmp);

		tmp.Idx_GoodsSource=11;
		::sprintf( (char *)tmp.goods_type, "" );
		::sprintf( (char *)tmp.srcPlace, "顺德区大良镇绿田路52号 " );
		::sprintf( (char *)tmp.srcQcode, "x5n9shu19" );
		::sprintf( (char *)tmp.dstPlace, "浙江省金华市永康市溪中东路258号、260号" );
		::sprintf( (char *)tmp.dstQcode, "49av2hvss" );
		::sprintf( (char *)tmp.sendOutTime, "" );
		tmp.transPrice=0;
		::sprintf( (char *)tmp.limitTime, "" );
		::sprintf( (char *)tmp.payWay, "" );
		::sprintf( (char *)tmp.publishTime, "2012-10-22 13:45" );
		::sprintf( (char *)tmp.publisher, "" );
		::sprintf( (char *)tmp.telphone, "13708465724" );
		::sprintf( (char *)tmp.notes, "" );
		m_GSDetailMgr.AddOneItem(tmp);

		tmp.Idx_GoodsSource=12;
		::sprintf( (char *)tmp.goods_type, "" );
		::sprintf( (char *)tmp.srcPlace, "顺德区陈村镇" );
		::sprintf( (char *)tmp.srcQcode, "xht1xn85s" );
		::sprintf( (char *)tmp.dstPlace, "内蒙古包头白云鄂博矿区广场西道" );
		::sprintf( (char *)tmp.dstQcode, "fj9945daq" );
		::sprintf( (char *)tmp.sendOutTime, "" );
		tmp.transPrice=0;
		::sprintf( (char *)tmp.limitTime, "" );
		::sprintf( (char *)tmp.payWay, "" );
		::sprintf( (char *)tmp.publishTime, "2012-10-22 13:45" );
		::sprintf( (char *)tmp.publisher, "" );
		::sprintf( (char *)tmp.telphone, "13978691611" );
		::sprintf( (char *)tmp.notes, "" );
		m_GSDetailMgr.AddOneItem(tmp);

		tmp.Idx_GoodsSource=13;
		::sprintf( (char *)tmp.goods_type, "" );
		::sprintf( (char *)tmp.srcPlace, "顺德伦教永丰成德巷66号" );
		::sprintf( (char *)tmp.srcQcode, "xcpsct6jh" );
		::sprintf( (char *)tmp.dstPlace, "广东省湛江市赤坎区" );
		::sprintf( (char *)tmp.dstQcode, "u0e7mvhn1" );
		::sprintf( (char *)tmp.sendOutTime, "" );
		tmp.transPrice=0;
		::sprintf( (char *)tmp.limitTime, "" );
		::sprintf( (char *)tmp.payWay, "" );
		::sprintf( (char *)tmp.publishTime, "2012-10-22 13:45" );
		::sprintf( (char *)tmp.publisher, "" );
		::sprintf( (char *)tmp.telphone, "18902279093" );
		::sprintf( (char *)tmp.notes, "" );
		m_GSDetailMgr.AddOneItem(tmp);

		tmp.Idx_GoodsSource=14;
		::sprintf( (char *)tmp.goods_type, "" );
		::sprintf( (char *)tmp.srcPlace, "顺德区碧江镇" );
		::sprintf( (char *)tmp.srcQcode, "xerndehdb" );
		::sprintf( (char *)tmp.dstPlace, "甘肃省兰州市七里河区西津西路68号" );
		::sprintf( (char *)tmp.dstQcode, "9nymkcpm3" );
		::sprintf( (char *)tmp.sendOutTime, "" );
		tmp.transPrice=0;
		::sprintf( (char *)tmp.limitTime, "" );
		::sprintf( (char *)tmp.payWay, "" );
		::sprintf( (char *)tmp.publishTime, "2012-10-22 13:45" );
		::sprintf( (char *)tmp.publisher, "" );
		::sprintf( (char *)tmp.telphone, "18633569097" );
		::sprintf( (char *)tmp.notes, "" );
		m_GSDetailMgr.AddOneItem(tmp);

		tmp.Idx_GoodsSource=15;
		::sprintf( (char *)tmp.goods_type, "" );
		::sprintf( (char *)tmp.srcPlace, "北滘镇三洪奇西岸3街103号" );
		::sprintf( (char *)tmp.srcQcode, "xwqg4f75h" );
		::sprintf( (char *)tmp.dstPlace, "陕西省咸阳市渭城区民生西路44号" );
		::sprintf( (char *)tmp.dstQcode, "8aj7fs2ne" );
		::sprintf( (char *)tmp.sendOutTime, "" );
		tmp.transPrice=0;
		::sprintf( (char *)tmp.limitTime, "" );
		::sprintf( (char *)tmp.payWay, "" );
		::sprintf( (char *)tmp.publishTime, "2012-10-22 13:45" );
		::sprintf( (char *)tmp.publisher, "" );
		::sprintf( (char *)tmp.telphone, "13500569456" );
		::sprintf( (char *)tmp.notes, "" );
		m_GSDetailMgr.AddOneItem(tmp);

		tmp.Idx_GoodsSource=16;
		::sprintf( (char *)tmp.goods_type, "" );
		::sprintf( (char *)tmp.srcPlace, "顺德区大良环市北路38号" );
		::sprintf( (char *)tmp.srcQcode, "x1nswx5n9" );
		::sprintf( (char *)tmp.dstPlace, "湖北省荆州市荆州区荆州大道" );
		::sprintf( (char *)tmp.dstQcode, "55fmn6hvq" );
		::sprintf( (char *)tmp.sendOutTime, "" );
		tmp.transPrice=0;
		::sprintf( (char *)tmp.limitTime, "" );
		::sprintf( (char *)tmp.payWay, "" );
		::sprintf( (char *)tmp.publishTime, "2012-10-22 13:45" );
		::sprintf( (char *)tmp.publisher, "" );
		::sprintf( (char *)tmp.telphone, "13135337569" );
		::sprintf( (char *)tmp.notes, "" );
		m_GSDetailMgr.AddOneItem(tmp);

		tmp.Idx_GoodsSource=17;
		::sprintf( (char *)tmp.goods_type, "" );
		::sprintf( (char *)tmp.srcPlace, "广东省佛山市" );
		::sprintf( (char *)tmp.srcQcode, "xxks6h7q8" );
		::sprintf( (char *)tmp.dstPlace, "江苏省盐城市响水县204国道" );
		::sprintf( (char *)tmp.dstQcode, "8tfpujkgv" );
		::sprintf( (char *)tmp.sendOutTime, "" );
		tmp.transPrice=0;
		::sprintf( (char *)tmp.limitTime, "" );
		::sprintf( (char *)tmp.payWay, "" );
		::sprintf( (char *)tmp.publishTime, "2012-10-22 13:45" );
		::sprintf( (char *)tmp.publisher, "" );
		::sprintf( (char *)tmp.telphone, "18935699183" );
		::sprintf( (char *)tmp.notes, "" );
		m_GSDetailMgr.AddOneItem(tmp);

		tmp.Idx_GoodsSource=18;
		::sprintf( (char *)tmp.goods_type, "" );
		::sprintf( (char *)tmp.srcPlace, "广东省佛山市" );
		::sprintf( (char *)tmp.srcQcode, "xbk3u7rbd" );
		::sprintf( (char *)tmp.dstPlace, "上海市宝山区真大路352弄400号" );
		::sprintf( (char *)tmp.dstQcode, "6h5ih3emf" );
		::sprintf( (char *)tmp.sendOutTime, "" );
		tmp.transPrice=0;
		::sprintf( (char *)tmp.limitTime, "" );
		::sprintf( (char *)tmp.payWay, "" );
		::sprintf( (char *)tmp.publishTime, "2012-10-22 13:48" );
		::sprintf( (char *)tmp.publisher, "" );
		::sprintf( (char *)tmp.telphone, "13233118690" );
		::sprintf( (char *)tmp.notes, "" );
		m_GSDetailMgr.AddOneItem(tmp);

		tmp.Idx_GoodsSource=19;
		::sprintf( (char *)tmp.goods_type, "" );
		::sprintf( (char *)tmp.srcPlace, "广东省佛山市" );
		::sprintf( (char *)tmp.srcQcode, "xtpjep6e6" );
		::sprintf( (char *)tmp.dstPlace, "四川省成都市青羊区科联街" );
		::sprintf( (char *)tmp.dstQcode, "5eqb7fi9v" );
		::sprintf( (char *)tmp.sendOutTime, "" );
		tmp.transPrice=0;
		::sprintf( (char *)tmp.limitTime, "" );
		::sprintf( (char *)tmp.payWay, "" );
		::sprintf( (char *)tmp.publishTime, "2012-10-22 13:48" );
		::sprintf( (char *)tmp.publisher, "" );
		::sprintf( (char *)tmp.telphone, "18644585689" );
		::sprintf( (char *)tmp.notes, "" );
		m_GSDetailMgr.AddOneItem(tmp);

		tmp.Idx_GoodsSource=20;
		::sprintf( (char *)tmp.goods_type, "" );
		::sprintf( (char *)tmp.srcPlace, "广东省佛山市" );
		::sprintf( (char *)tmp.srcQcode, "x8pgreh26" );
		::sprintf( (char *)tmp.dstPlace, "福建省厦门市同安区环城南路74号之11" );
		::sprintf( (char *)tmp.dstQcode, "032hfjb7b" );
		::sprintf( (char *)tmp.sendOutTime, "" );
		tmp.transPrice=0;
		::sprintf( (char *)tmp.limitTime, "" );
		::sprintf( (char *)tmp.payWay, "" );
		::sprintf( (char *)tmp.publishTime, "2012-10-22 13:48" );
		::sprintf( (char *)tmp.publisher, "" );
		::sprintf( (char *)tmp.telphone, "15333192297" );
		::sprintf( (char *)tmp.notes, "" );
		m_GSDetailMgr.AddOneItem(tmp);

		tmp.Idx_GoodsSource=21;
		::sprintf( (char *)tmp.goods_type, "" );
		::sprintf( (char *)tmp.srcPlace, "广东省佛山市" );
		::sprintf( (char *)tmp.srcQcode, "xbpewiumm" );
		::sprintf( (char *)tmp.dstPlace, "黑龙江省齐齐哈尔市富裕县" );
		::sprintf( (char *)tmp.dstQcode, "mydn5bbnn" );
		::sprintf( (char *)tmp.sendOutTime, "" );
		tmp.transPrice=0;
		::sprintf( (char *)tmp.limitTime, "" );
		::sprintf( (char *)tmp.payWay, "" );
		::sprintf( (char *)tmp.publishTime, "2012-10-22 13:48" );
		::sprintf( (char *)tmp.publisher, "" );
		::sprintf( (char *)tmp.telphone, "13208979986" );
		::sprintf( (char *)tmp.notes, "" );
		m_GSDetailMgr.AddOneItem(tmp);

		tmp.Idx_GoodsSource=22;
		::sprintf( (char *)tmp.goods_type, "" );
		::sprintf( (char *)tmp.srcPlace, "广东省佛山市" );
		::sprintf( (char *)tmp.srcQcode, "xtn9mm00f" );
		::sprintf( (char *)tmp.dstPlace, "浙江省杭州市萧山区通惠中路" );
		::sprintf( (char *)tmp.dstQcode, "53ahxncs8" );
		::sprintf( (char *)tmp.sendOutTime, "" );
		tmp.transPrice=0;
		::sprintf( (char *)tmp.limitTime, "" );
		::sprintf( (char *)tmp.payWay, "" );
		::sprintf( (char *)tmp.publishTime, "2012-10-22 13:48" );
		::sprintf( (char *)tmp.publisher, "" );
		::sprintf( (char *)tmp.telphone, "18633966490" );
		::sprintf( (char *)tmp.notes, "" );
		m_GSDetailMgr.AddOneItem(tmp);

		tmp.Idx_GoodsSource=23;
		::sprintf( (char *)tmp.goods_type, "" );
		::sprintf( (char *)tmp.srcPlace, "广东省佛山市" );
		::sprintf( (char *)tmp.srcQcode, "xgsnfiva4" );
		::sprintf( (char *)tmp.dstPlace, "新疆省阿克苏" );
		::sprintf( (char *)tmp.dstQcode, "eyv0bf94c" );
		::sprintf( (char *)tmp.sendOutTime, "" );
		tmp.transPrice=0;
		::sprintf( (char *)tmp.limitTime, "" );
		::sprintf( (char *)tmp.payWay, "" );
		::sprintf( (char *)tmp.publishTime, "2012-10-22 13:48" );
		::sprintf( (char *)tmp.publisher, "" );
		::sprintf( (char *)tmp.telphone, "15900787984" );
		::sprintf( (char *)tmp.notes, "" );
		m_GSDetailMgr.AddOneItem(tmp);

		tmp.Idx_GoodsSource=24;
		::sprintf( (char *)tmp.goods_type, "" );
		::sprintf( (char *)tmp.srcPlace, "广东省佛山市" );
		::sprintf( (char *)tmp.srcQcode, "x4s6k7ft0" );
		::sprintf( (char *)tmp.dstPlace, "广东省梅州市梅江区东升二路" );
		::sprintf( (char *)tmp.dstQcode, "d7snb903b" );
		::sprintf( (char *)tmp.sendOutTime, "" );
		tmp.transPrice=0;
		::sprintf( (char *)tmp.limitTime, "" );
		::sprintf( (char *)tmp.payWay, "" );
		::sprintf( (char *)tmp.publishTime, "2012-10-22 13:48" );
		::sprintf( (char *)tmp.publisher, "" );
		::sprintf( (char *)tmp.telphone, "13735339999" );
		::sprintf( (char *)tmp.notes, "" );
		m_GSDetailMgr.AddOneItem(tmp);

		tmp.Idx_GoodsSource=25;
		::sprintf( (char *)tmp.goods_type, "" );
		::sprintf( (char *)tmp.srcPlace, "广东省佛山市" );
		::sprintf( (char *)tmp.srcQcode, "xxsjucrbd" );
		::sprintf( (char *)tmp.dstPlace, "甘肃省酒泉市瓜州县渊泉街" );
		::sprintf( (char *)tmp.dstQcode, "e9av6mpja" );
		::sprintf( (char *)tmp.sendOutTime, "" );
		tmp.transPrice=0;
		::sprintf( (char *)tmp.limitTime, "" );
		::sprintf( (char *)tmp.payWay, "" );
		::sprintf( (char *)tmp.publishTime, "2012-10-22 13:48" );
		::sprintf( (char *)tmp.publisher, "" );
		::sprintf( (char *)tmp.telphone, "15900997981" );
		::sprintf( (char *)tmp.notes, "" );
		m_GSDetailMgr.AddOneItem(tmp);

		tmp.Idx_GoodsSource=26;
		::sprintf( (char *)tmp.goods_type, "" );
		::sprintf( (char *)tmp.srcPlace, "广东省佛山市" );
		::sprintf( (char *)tmp.srcQcode, "xysgcqiq8" );
		::sprintf( (char *)tmp.dstPlace, "黑龙江省哈尔滨市西大直街副71号" );
		::sprintf( (char *)tmp.dstQcode, "j01q1ajyb" );
		::sprintf( (char *)tmp.sendOutTime, "" );
		tmp.transPrice=0;
		::sprintf( (char *)tmp.limitTime, "" );
		::sprintf( (char *)tmp.payWay, "" );
		::sprintf( (char *)tmp.publishTime, "2012-10-22 13:48" );
		::sprintf( (char *)tmp.publisher, "" );
		::sprintf( (char *)tmp.telphone, "13244689987" );
		::sprintf( (char *)tmp.notes, "" );
		m_GSDetailMgr.AddOneItem(tmp);

		tmp.Idx_GoodsSource=27;
		::sprintf( (char *)tmp.goods_type, "" );
		::sprintf( (char *)tmp.srcPlace, "广东省佛山市" );
		::sprintf( (char *)tmp.srcQcode, "xcr699fm9" );
		::sprintf( (char *)tmp.dstPlace, "广东省晋宁市流沙大道西" );
		::sprintf( (char *)tmp.dstQcode, "dexr8n29r" );
		::sprintf( (char *)tmp.sendOutTime, "" );
		tmp.transPrice=0;
		::sprintf( (char *)tmp.limitTime, "" );
		::sprintf( (char *)tmp.payWay, "" );
		::sprintf( (char *)tmp.publishTime, "2012-10-22 13:48" );
		::sprintf( (char *)tmp.publisher, "" );
		::sprintf( (char *)tmp.telphone, "13302571512" );
		::sprintf( (char *)tmp.notes, "" );
		m_GSDetailMgr.AddOneItem(tmp);

		tmp.Idx_GoodsSource=28;
		::sprintf( (char *)tmp.goods_type, "" );
		::sprintf( (char *)tmp.srcPlace, "广东省佛山市" );
		::sprintf( (char *)tmp.srcQcode, "xhsv4ei20" );
		::sprintf( (char *)tmp.dstPlace, "辽宁省沈阳市铁西区南十中路24号" );
		::sprintf( (char *)tmp.dstQcode, "ff92qaw44" );
		::sprintf( (char *)tmp.sendOutTime, "" );
		tmp.transPrice=0;
		::sprintf( (char *)tmp.limitTime, "" );
		::sprintf( (char *)tmp.payWay, "" );
		::sprintf( (char *)tmp.publishTime, "" );
		::sprintf( (char *)tmp.publisher, "" );
		::sprintf( (char *)tmp.telphone, "13244686172" );
		::sprintf( (char *)tmp.notes, "" );
		m_GSDetailMgr.AddOneItem(tmp);

		tmp.Idx_GoodsSource=29;
		::sprintf( (char *)tmp.goods_type, "" );
		::sprintf( (char *)tmp.srcPlace, "广东省佛山市" );
		::sprintf( (char *)tmp.srcQcode, "xprxriqpy" );
		::sprintf( (char *)tmp.dstPlace, "四川省郫县" );
		::sprintf( (char *)tmp.dstQcode, "5yv7emnt4" );
		::sprintf( (char *)tmp.sendOutTime, "" );
		tmp.transPrice=0;
		::sprintf( (char *)tmp.limitTime, "" );
		::sprintf( (char *)tmp.payWay, "" );
		::sprintf( (char *)tmp.publishTime, "" );
		::sprintf( (char *)tmp.publisher, "" );
		::sprintf( (char *)tmp.telphone, "15866791181" );
		::sprintf( (char *)tmp.notes, "" );
		m_GSDetailMgr.AddOneItem(tmp);

		tmp.Idx_GoodsSource=30;
		::sprintf( (char *)tmp.goods_type, "" );
		::sprintf( (char *)tmp.srcPlace, "广东省佛山市" );
		::sprintf( (char *)tmp.srcQcode, "x9sj1a8im" );
		::sprintf( (char *)tmp.dstPlace, "湖北省随州市曾都区文峰巷" );
		::sprintf( (char *)tmp.dstQcode, "6th8i3ihm" );
		::sprintf( (char *)tmp.sendOutTime, "" );
		tmp.transPrice=0;
		::sprintf( (char *)tmp.limitTime, "" );
		::sprintf( (char *)tmp.payWay, "" );
		::sprintf( (char *)tmp.publishTime, "" );
		::sprintf( (char *)tmp.publisher, "" );
		::sprintf( (char *)tmp.telphone, "15300983086" );
		::sprintf( (char *)tmp.notes, "" );
		m_GSDetailMgr.AddOneItem(tmp);

		tmp.Idx_GoodsSource=31;
		::sprintf( (char *)tmp.goods_type, "" );
		::sprintf( (char *)tmp.srcPlace, "广东省佛山市" );
		::sprintf( (char *)tmp.srcQcode, "xtr4adf0b" );
		::sprintf( (char *)tmp.dstPlace, "河北省天津市" );
		::sprintf( (char *)tmp.dstQcode, "" );
		::sprintf( (char *)tmp.sendOutTime, "" );
		tmp.transPrice=0;
		::sprintf( (char *)tmp.limitTime, "" );
		::sprintf( (char *)tmp.payWay, "" );
		::sprintf( (char *)tmp.publishTime, "" );
		::sprintf( (char *)tmp.publisher, "" );
		::sprintf( (char *)tmp.telphone, "13966668607" );
		::sprintf( (char *)tmp.notes, "" );
		m_GSDetailMgr.AddOneItem(tmp);

		tmp.Idx_GoodsSource=32;
		::sprintf( (char *)tmp.goods_type, "" );
		::sprintf( (char *)tmp.srcPlace, "广东省佛山市" );
		::sprintf( (char *)tmp.srcQcode, "x6tdb5emh" );
		::sprintf( (char *)tmp.dstPlace, "河南省登封市中岳大街" );
		::sprintf( (char *)tmp.dstQcode, "8in0ehw62" );
		::sprintf( (char *)tmp.sendOutTime, "" );
		tmp.transPrice=0;
		::sprintf( (char *)tmp.limitTime, "" );
		::sprintf( (char *)tmp.payWay, "" );
		::sprintf( (char *)tmp.publishTime, "" );
		::sprintf( (char *)tmp.publisher, "" );
		::sprintf( (char *)tmp.telphone, "15322224489" );
		::sprintf( (char *)tmp.notes, "" );
		m_GSDetailMgr.AddOneItem(tmp);

		tmp.Idx_GoodsSource=33;
		::sprintf( (char *)tmp.goods_type, "" );
		::sprintf( (char *)tmp.srcPlace, "广东省佛山市" );
		::sprintf( (char *)tmp.srcQcode, "xssapm9cd" );
		::sprintf( (char *)tmp.dstPlace, "湖南省长沙市天心区南门口晏家塘162号" );
		::sprintf( (char *)tmp.dstQcode, "3nt43hedw" );
		::sprintf( (char *)tmp.sendOutTime, "" );
		tmp.transPrice=0;
		::sprintf( (char *)tmp.limitTime, "" );
		::sprintf( (char *)tmp.payWay, "" );
		::sprintf( (char *)tmp.publishTime, "" );
		::sprintf( (char *)tmp.publisher, "" );
		::sprintf( (char *)tmp.telphone, "18900797980" );
		::sprintf( (char *)tmp.notes, "" );
		m_GSDetailMgr.AddOneItem(tmp);

		tmp.Idx_GoodsSource=34;
		::sprintf( (char *)tmp.goods_type, "" );
		::sprintf( (char *)tmp.srcPlace, "广东省佛山市" );
		::sprintf( (char *)tmp.srcQcode, "xuvdn7efq" );
		::sprintf( (char *)tmp.dstPlace, "浙江省嘉兴市海盐县长安路" );
		::sprintf( (char *)tmp.dstQcode, "5dm24f8kv" );
		::sprintf( (char *)tmp.sendOutTime, "" );
		tmp.transPrice=0;
		::sprintf( (char *)tmp.limitTime, "" );
		::sprintf( (char *)tmp.payWay, "" );
		::sprintf( (char *)tmp.publishTime, "" );
		::sprintf( (char *)tmp.publisher, "" );
		::sprintf( (char *)tmp.telphone, "18600077589" );
		::sprintf( (char *)tmp.notes, "" );
		m_GSDetailMgr.AddOneItem(tmp);

		tmp.Idx_GoodsSource=35;
		::sprintf( (char *)tmp.goods_type, "" );
		::sprintf( (char *)tmp.srcPlace, "广东省佛山市" );
		::sprintf( (char *)tmp.srcQcode, "xvt4hs6um" );
		::sprintf( (char *)tmp.dstPlace, "湖南省邵阳市" );
		::sprintf( (char *)tmp.dstQcode, "3cd1bp2tk" );
		::sprintf( (char *)tmp.sendOutTime, "" );
		tmp.transPrice=0;
		::sprintf( (char *)tmp.limitTime, "" );
		::sprintf( (char *)tmp.payWay, "" );
		::sprintf( (char *)tmp.publishTime, "" );
		::sprintf( (char *)tmp.publisher, "" );
		::sprintf( (char *)tmp.telphone, "15366795867" );
		::sprintf( (char *)tmp.notes, "" );
		m_GSDetailMgr.AddOneItem(tmp);

		tmp.Idx_GoodsSource=36;
		::sprintf( (char *)tmp.goods_type, "" );
		::sprintf( (char *)tmp.srcPlace, "广东省佛山市" );
		::sprintf( (char *)tmp.srcQcode, "xmv716y9j" );
		::sprintf( (char *)tmp.dstPlace, "河南省商丘市永城市雪枫路" );
		::sprintf( (char *)tmp.dstQcode, "8j6qp9amb" );
		::sprintf( (char *)tmp.sendOutTime, "" );
		tmp.transPrice=0;
		::sprintf( (char *)tmp.limitTime, "" );
		::sprintf( (char *)tmp.payWay, "" );
		::sprintf( (char *)tmp.publishTime, "" );
		::sprintf( (char *)tmp.publisher, "" );
		::sprintf( (char *)tmp.telphone, "18673498934" );
		::sprintf( (char *)tmp.notes, "" );
		m_GSDetailMgr.AddOneItem(tmp);

		tmp.Idx_GoodsSource=37;
		::sprintf( (char *)tmp.goods_type, "" );
		::sprintf( (char *)tmp.srcPlace, "广东省佛山市" );
		::sprintf( (char *)tmp.srcQcode, "xrvsa9tdk" );
		::sprintf( (char *)tmp.dstPlace, "湖南省湘乡市" );
		::sprintf( (char *)tmp.dstQcode, "3jf7jd1y1" );
		::sprintf( (char *)tmp.sendOutTime, "" );
		tmp.transPrice=0;
		::sprintf( (char *)tmp.limitTime, "" );
		::sprintf( (char *)tmp.payWay, "" );
		::sprintf( (char *)tmp.publishTime, "" );
		::sprintf( (char *)tmp.publisher, "" );
		::sprintf( (char *)tmp.telphone, "13925228609" );
		::sprintf( (char *)tmp.notes, "" );
		m_GSDetailMgr.AddOneItem(tmp);

		tmp.Idx_GoodsSource=38;
		::sprintf( (char *)tmp.goods_type, "" );
		::sprintf( (char *)tmp.srcPlace, "广东省佛山市" );
		::sprintf( (char *)tmp.srcQcode, "xjtebwpdi" );
		::sprintf( (char *)tmp.dstPlace, "河南省新乡市卫滨区" );
		::sprintf( (char *)tmp.dstQcode, "9s9aue7ga" );
		::sprintf( (char *)tmp.sendOutTime, "" );
		tmp.transPrice=0;
		::sprintf( (char *)tmp.limitTime, "" );
		::sprintf( (char *)tmp.payWay, "" );
		::sprintf( (char *)tmp.publishTime, "" );
		::sprintf( (char *)tmp.publisher, "" );
		::sprintf( (char *)tmp.telphone, "18623249890" );
		::sprintf( (char *)tmp.notes, "" );
		m_GSDetailMgr.AddOneItem(tmp);

		tmp.Idx_GoodsSource=39;
		::sprintf( (char *)tmp.goods_type, "" );
		::sprintf( (char *)tmp.srcPlace, "广东省佛山市" );
		::sprintf( (char *)tmp.srcQcode, "xtwyr64rb" );
		::sprintf( (char *)tmp.dstPlace, "四川省成都市龙泉驿区和平路2、4号" );
		::sprintf( (char *)tmp.dstQcode, "59q32prd9" );
		::sprintf( (char *)tmp.sendOutTime, "" );
		tmp.transPrice=0;
		::sprintf( (char *)tmp.limitTime, "" );
		::sprintf( (char *)tmp.payWay, "" );
		::sprintf( (char *)tmp.publishTime, "" );
		::sprintf( (char *)tmp.publisher, "" );
		::sprintf( (char *)tmp.telphone, "13369695087" );
		::sprintf( (char *)tmp.notes, "" );
		m_GSDetailMgr.AddOneItem(tmp);

		tmp.Idx_GoodsSource=40;
		::sprintf( (char *)tmp.goods_type, "" );
		::sprintf( (char *)tmp.srcPlace, "广东省佛山市" );
		::sprintf( (char *)tmp.srcQcode, "xdw1bff41" );
		::sprintf( (char *)tmp.dstPlace, "上海市浦东新区恒大市场" );
		::sprintf( (char *)tmp.dstQcode, "602h9sqpj" );
		::sprintf( (char *)tmp.sendOutTime, "" );
		tmp.transPrice=0;
		::sprintf( (char *)tmp.limitTime, "" );
		::sprintf( (char *)tmp.payWay, "" );
		::sprintf( (char *)tmp.publishTime, "" );
		::sprintf( (char *)tmp.publisher, "" );
		::sprintf( (char *)tmp.telphone, "15600245382" );
		::sprintf( (char *)tmp.notes, "" );
		m_GSDetailMgr.AddOneItem(tmp);

		tmp.Idx_GoodsSource=41;
		::sprintf( (char *)tmp.goods_type, "" );
		::sprintf( (char *)tmp.srcPlace, "广东省佛山市" );
		::sprintf( (char *)tmp.srcQcode, "xeyhvmxpw" );
		::sprintf( (char *)tmp.dstPlace, "黑龙江省齐齐哈尔市富裕县" );
		::sprintf( (char *)tmp.dstQcode, "mydn5bbnn" );
		::sprintf( (char *)tmp.sendOutTime, "" );
		tmp.transPrice=0;
		::sprintf( (char *)tmp.limitTime, "" );
		::sprintf( (char *)tmp.payWay, "" );
		::sprintf( (char *)tmp.publishTime, "" );
		::sprintf( (char *)tmp.publisher, "" );
		::sprintf( (char *)tmp.telphone, "15668866791" );
		::sprintf( (char *)tmp.notes, "" );
		m_GSDetailMgr.AddOneItem(tmp);

		tmp.Idx_GoodsSource=42;
		::sprintf( (char *)tmp.goods_type, "" );
		::sprintf( (char *)tmp.srcPlace, "广东省佛山市" );
		::sprintf( (char *)tmp.srcQcode, "x7w5gp7jt" );
		::sprintf( (char *)tmp.dstPlace, "云南省昆明市县师家村路" );
		::sprintf( (char *)tmp.dstQcode, "03088jcyx" );
		::sprintf( (char *)tmp.sendOutTime, "" );
		tmp.transPrice=0;
		::sprintf( (char *)tmp.limitTime, "" );
		::sprintf( (char *)tmp.payWay, "" );
		::sprintf( (char *)tmp.publishTime, "" );
		::sprintf( (char *)tmp.publisher, "" );
		::sprintf( (char *)tmp.telphone, "15969799768" );
		::sprintf( (char *)tmp.notes, "" );
		m_GSDetailMgr.AddOneItem(tmp);

		tmp.Idx_GoodsSource=43;
		::sprintf( (char *)tmp.goods_type, "" );
		::sprintf( (char *)tmp.srcPlace, "广东省佛山市" );
		::sprintf( (char *)tmp.srcQcode, "xhy9mbs4k" );
		::sprintf( (char *)tmp.dstPlace, "广东省晋宁市流沙大道西" );
		::sprintf( (char *)tmp.dstQcode, "dexr8n29r" );
		::sprintf( (char *)tmp.sendOutTime, "" );
		tmp.transPrice=0;
		::sprintf( (char *)tmp.limitTime, "" );
		::sprintf( (char *)tmp.payWay, "" );
		::sprintf( (char *)tmp.publishTime, "" );
		::sprintf( (char *)tmp.publisher, "" );
		::sprintf( (char *)tmp.telphone, "13525226138" );
		::sprintf( (char *)tmp.notes, "" );
		m_GSDetailMgr.AddOneItem(tmp);

		tmp.Idx_GoodsSource=44;
		::sprintf( (char *)tmp.goods_type, "" );
		::sprintf( (char *)tmp.srcPlace, "广东省佛山市" );
		::sprintf( (char *)tmp.srcQcode, "x6y5abk2a" );
		::sprintf( (char *)tmp.dstPlace, "新疆省昌吉市绿洲 南路" );
		::sprintf( (char *)tmp.dstQcode, "hxd68v0i3" );
		::sprintf( (char *)tmp.sendOutTime, "" );
		tmp.transPrice=0;
		::sprintf( (char *)tmp.limitTime, "" );
		::sprintf( (char *)tmp.payWay, "" );
		::sprintf( (char *)tmp.publishTime, "" );
		::sprintf( (char *)tmp.publisher, "" );
		::sprintf( (char *)tmp.telphone, "15300899003" );
		::sprintf( (char *)tmp.notes, "" );
		m_GSDetailMgr.AddOneItem(tmp);

		tmp.Idx_GoodsSource=45;
		::sprintf( (char *)tmp.goods_type, "" );
		::sprintf( (char *)tmp.srcPlace, "广东省佛山市" );
		::sprintf( (char *)tmp.srcQcode, "xeyeki7ej" );
		::sprintf( (char *)tmp.dstPlace, "陕西省西安市碑林区更新街3号名爵大厦" );
		::sprintf( (char *)tmp.dstQcode, "83gqbs370" );
		::sprintf( (char *)tmp.sendOutTime, "" );
		tmp.transPrice=0;
		::sprintf( (char *)tmp.limitTime, "" );
		::sprintf( (char *)tmp.payWay, "" );
		::sprintf( (char *)tmp.publishTime, "" );
		::sprintf( (char *)tmp.publisher, "" );
		::sprintf( (char *)tmp.telphone, "13822215469" );
		::sprintf( (char *)tmp.notes, "" );
		m_GSDetailMgr.AddOneItem(tmp);

		tmp.Idx_GoodsSource=46;
		::sprintf( (char *)tmp.goods_type, "" );
		::sprintf( (char *)tmp.srcPlace, "广东省佛山市" );
		::sprintf( (char *)tmp.srcQcode, "xfyt959ft" );
		::sprintf( (char *)tmp.dstPlace, "黑龙江省黑河五大连池" );
		::sprintf( (char *)tmp.dstQcode, "m4m4q3qpg" );
		::sprintf( (char *)tmp.sendOutTime, "" );
		tmp.transPrice=0;
		::sprintf( (char *)tmp.limitTime, "" );
		::sprintf( (char *)tmp.payWay, "" );
		::sprintf( (char *)tmp.publishTime, "" );
		::sprintf( (char *)tmp.publisher, "" );
		::sprintf( (char *)tmp.telphone, "13288732181" );
		::sprintf( (char *)tmp.notes, "" );
		m_GSDetailMgr.AddOneItem(tmp);

		tmp.Idx_GoodsSource=47;
		::sprintf( (char *)tmp.goods_type, "" );
		::sprintf( (char *)tmp.srcPlace, "广东省佛山市" );
		::sprintf( (char *)tmp.srcQcode, "xjw3abw18" );
		::sprintf( (char *)tmp.dstPlace, "四川省自贡市富顺县育仁路" );
		::sprintf( (char *)tmp.dstQcode, "41jvhdm24" );
		::sprintf( (char *)tmp.sendOutTime, "" );
		tmp.transPrice=0;
		::sprintf( (char *)tmp.limitTime, "" );
		::sprintf( (char *)tmp.payWay, "" );
		::sprintf( (char *)tmp.publishTime, "" );
		::sprintf( (char *)tmp.publisher, "" );
		::sprintf( (char *)tmp.telphone, "13394382189" );
		::sprintf( (char *)tmp.notes, "可拼车" );
		m_GSDetailMgr.AddOneItem(tmp);

	}
	m_GSDetailMgr.DisconnectFile();
}
void CmakeDataFiles::makeGoodsSource()
{
	m_goodSourceMgr.ConnectToFile();
	if (m_goodSourceMgr.GetCount()<1)
		//if (1==2)
	{
		GoodsSource tmp;

		::sprintf( (char *)tmp.code, "GDFS000001" );
		::sprintf( (char *)tmp.goods_name, "饮料" );
		::sprintf( (char *)tmp.total_size, "500吨" );
		::sprintf( (char *)tmp.address, "佛山市顺德区土禾设备有限公司 配货信息部" );
		m_goodSourceMgr.AddOneItem(tmp);

		::sprintf( (char *)tmp.code, "GDFS000002" );
		::sprintf( (char *)tmp.goods_name, "地砖" );
		::sprintf( (char *)tmp.total_size, "40吨" );
		::sprintf( (char *)tmp.address, "福盈酒店 配货信息部" );
		m_goodSourceMgr.AddOneItem(tmp);

		::sprintf( (char *)tmp.code, "GDFS00003" );
		::sprintf( (char *)tmp.goods_name, "编织袋" );
		::sprintf( (char *)tmp.total_size, "10吨" );
		::sprintf( (char *)tmp.address, "广佛物流中心" );
		m_goodSourceMgr.AddOneItem(tmp);

		::sprintf( (char *)tmp.code, "GDFS000004" );
		::sprintf( (char *)tmp.goods_name, "设备" );
		::sprintf( (char *)tmp.total_size, "18吨" );
		::sprintf( (char *)tmp.address, "万和电气 配货信息部" );
		m_goodSourceMgr.AddOneItem(tmp);

		::sprintf( (char *)tmp.code, "GDFS000005" );
		::sprintf( (char *)tmp.goods_name, "铝材" );
		::sprintf( (char *)tmp.total_size, "35方" );
		::sprintf( (char *)tmp.address, "佛山市顺德区路骏钢材有限公司 配货信息部" );
		m_goodSourceMgr.AddOneItem(tmp);

		::sprintf( (char *)tmp.code, "GDFS000006" );
		::sprintf( (char *)tmp.goods_name, "塑料颗粒" );
		::sprintf( (char *)tmp.total_size, "38方" );
		::sprintf( (char *)tmp.address, "宇通公司 配货信息部" );
		m_goodSourceMgr.AddOneItem(tmp);

		::sprintf( (char *)tmp.code, "GDFS000007" );
		::sprintf( (char *)tmp.goods_name, "配件" );
		::sprintf( (char *)tmp.total_size, "15吨" );
		::sprintf( (char *)tmp.address, "佛山创意物流 配货信息部" );
		m_goodSourceMgr.AddOneItem(tmp);

		::sprintf( (char *)tmp.code, "GDFS000008" );
		::sprintf( (char *)tmp.goods_name, "百货" );
		::sprintf( (char *)tmp.total_size, "32方" );
		::sprintf( (char *)tmp.address, "佛山市鼎力物流有限公司 配货信息部" );
		m_goodSourceMgr.AddOneItem(tmp);

		::sprintf( (char *)tmp.code, "GDFS000009" );
		::sprintf( (char *)tmp.goods_name, "家具" );
		::sprintf( (char *)tmp.total_size, "20吨" );
		::sprintf( (char *)tmp.address, "顺德区立创电子有限公司 配货信息部" );
		m_goodSourceMgr.AddOneItem(tmp);

		::sprintf( (char *)tmp.code, "GDFS000010" );
		::sprintf( (char *)tmp.goods_name, "家具" );
		::sprintf( (char *)tmp.total_size, "4吨" );
		::sprintf( (char *)tmp.address, "光辉家具贸易公司 配货信息部" );
		m_goodSourceMgr.AddOneItem(tmp);

		::sprintf( (char *)tmp.code, "GDFS000011" );
		::sprintf( (char *)tmp.goods_name, "家具" );
		::sprintf( (char *)tmp.total_size, "24方" );
		::sprintf( (char *)tmp.address, "盟邦物流 配货信息部" );
		m_goodSourceMgr.AddOneItem(tmp);

		::sprintf( (char *)tmp.code, "GDFS000012" );
		::sprintf( (char *)tmp.goods_name, "设备" );
		::sprintf( (char *)tmp.total_size, "42吨" );
		::sprintf( (char *)tmp.address, "诚顺安和 配货信息部" );
		m_goodSourceMgr.AddOneItem(tmp);

		::sprintf( (char *)tmp.code, "GDFS000013" );
		::sprintf( (char *)tmp.goods_name, "风机" );
		::sprintf( (char *)tmp.total_size, "25吨" );
		::sprintf( (char *)tmp.address, "佛山市隆信钨钼合金有限公司 配货信息部" );
		m_goodSourceMgr.AddOneItem(tmp);

		::sprintf( (char *)tmp.code, "GDFS000014" );
		::sprintf( (char *)tmp.goods_name, "地砖" );
		::sprintf( (char *)tmp.total_size, "40吨" );
		::sprintf( (char *)tmp.address, "东科公司 配货信息部" );
		m_goodSourceMgr.AddOneItem(tmp);

		::sprintf( (char *)tmp.code, "GDFS000015" );
		::sprintf( (char *)tmp.goods_name, "家具" );
		::sprintf( (char *)tmp.total_size, "3吨" );
		::sprintf( (char *)tmp.address, "鸿顺地产 配货信息部" );
		m_goodSourceMgr.AddOneItem(tmp);

		::sprintf( (char *)tmp.code, "GDFS000016" );
		::sprintf( (char *)tmp.goods_name, "棉纱" );
		::sprintf( (char *)tmp.total_size, "35吨" );
		::sprintf( (char *)tmp.address, "生活电器 配货信息部" );
		m_goodSourceMgr.AddOneItem(tmp);

		::sprintf( (char *)tmp.code, "GDFS000017" );
		::sprintf( (char *)tmp.goods_name, "重货" );
		::sprintf( (char *)tmp.total_size, "50吨" );
		::sprintf( (char *)tmp.address, "福盈酒店 配货信息部" );
		m_goodSourceMgr.AddOneItem(tmp);

		::sprintf( (char *)tmp.code, "GDFS000018" );
		::sprintf( (char *)tmp.goods_name, "钢管" );
		::sprintf( (char *)tmp.total_size, "14吨" );
		::sprintf( (char *)tmp.address, "德丰物流中心" );
		m_goodSourceMgr.AddOneItem(tmp);

		::sprintf( (char *)tmp.code, "GDFS000019" );
		::sprintf( (char *)tmp.goods_name, "电器" );
		::sprintf( (char *)tmp.total_size, "25吨" );
		::sprintf( (char *)tmp.address, "细滘物流中心" );
		m_goodSourceMgr.AddOneItem(tmp);

		::sprintf( (char *)tmp.code, "GDFS000020" );
		::sprintf( (char *)tmp.goods_name, "鱼" );
		::sprintf( (char *)tmp.total_size, "19吨" );
		::sprintf( (char *)tmp.address, "安发物流中心" );
		m_goodSourceMgr.AddOneItem(tmp);

		::sprintf( (char *)tmp.code, "GDFS000021" );
		::sprintf( (char *)tmp.goods_name, "设备" );
		::sprintf( (char *)tmp.total_size, "24吨" );
		::sprintf( (char *)tmp.address, "航空物流中心" );
		m_goodSourceMgr.AddOneItem(tmp);

		::sprintf( (char *)tmp.code, "GDFS000022" );
		::sprintf( (char *)tmp.goods_name, "家具" );
		::sprintf( (char *)tmp.total_size, "15吨" );
		::sprintf( (char *)tmp.address, "亚洲国际物流中心" );
		m_goodSourceMgr.AddOneItem(tmp);

		::sprintf( (char *)tmp.code, "GDFS000023" );
		::sprintf( (char *)tmp.goods_name, "大理石" );
		::sprintf( (char *)tmp.total_size, "38吨" );
		::sprintf( (char *)tmp.address, "顺德企发物流中心" );
		m_goodSourceMgr.AddOneItem(tmp);

		::sprintf( (char *)tmp.code, "GDFS000024" );
		::sprintf( (char *)tmp.goods_name, "家具" );
		::sprintf( (char *)tmp.total_size, "35方" );
		::sprintf( (char *)tmp.address, "建朗物流中心" );
		m_goodSourceMgr.AddOneItem(tmp);

		::sprintf( (char *)tmp.code, "GDFS000025" );
		::sprintf( (char *)tmp.goods_name, "设备" );
		::sprintf( (char *)tmp.total_size, "14吨" );
		::sprintf( (char *)tmp.address, "沙岗陶瓷仓储物流中心" );
		m_goodSourceMgr.AddOneItem(tmp);

		::sprintf( (char *)tmp.code, "GDFS000026" );
		::sprintf( (char *)tmp.goods_name, "家具" );
		::sprintf( (char *)tmp.total_size, "33方" );
		::sprintf( (char *)tmp.address, "金地带瓷业物流中心" );
		m_goodSourceMgr.AddOneItem(tmp);

		::sprintf( (char *)tmp.code, "GDFS000027" );
		::sprintf( (char *)tmp.goods_name, "设备" );
		::sprintf( (char *)tmp.total_size, "35吨" );
		::sprintf( (char *)tmp.address, "兴龙陶瓷物流中心" );
		m_goodSourceMgr.AddOneItem(tmp);

		::sprintf( (char *)tmp.code, "GDFS000028" );
		::sprintf( (char *)tmp.goods_name, "地砖" );
		::sprintf( (char *)tmp.total_size, "不限" );
		::sprintf( (char *)tmp.address, "乐从世海钢材物流中心" );
		m_goodSourceMgr.AddOneItem(tmp);

		::sprintf( (char *)tmp.code, "GDFS000029" );
		::sprintf( (char *)tmp.goods_name, "铝材" );
		::sprintf( (char *)tmp.total_size, "35吨" );
		::sprintf( (char *)tmp.address, "恒发.奥丽斯物流中心" );
		m_goodSourceMgr.AddOneItem(tmp);

		::sprintf( (char *)tmp.code, "GDFS000032" );
		::sprintf( (char *)tmp.goods_name, "百货" );
		::sprintf( (char *)tmp.total_size, "33方" );
		::sprintf( (char *)tmp.address, "雄乐国际物流中心" );
		m_goodSourceMgr.AddOneItem(tmp);

		::sprintf( (char *)tmp.code, "GDFS000031" );
		::sprintf( (char *)tmp.goods_name, "地砖" );
		::sprintf( (char *)tmp.total_size, "45吨" );
		::sprintf( (char *)tmp.address, "金地带物流中心" );
		m_goodSourceMgr.AddOneItem(tmp);

		::sprintf( (char *)tmp.code, "GDFS000032" );
		::sprintf( (char *)tmp.goods_name, "设备" );
		::sprintf( (char *)tmp.total_size, "35吨" );
		::sprintf( (char *)tmp.address, "旭东钢铁物流中心" );
		m_goodSourceMgr.AddOneItem(tmp);

		::sprintf( (char *)tmp.code, "GDFS000033" );
		::sprintf( (char *)tmp.goods_name, "家具" );
		::sprintf( (char *)tmp.total_size, "35方" );
		::sprintf( (char *)tmp.address, "五洲通物流中心" );
		m_goodSourceMgr.AddOneItem(tmp);

		::sprintf( (char *)tmp.code, "GDFS000034" );
		::sprintf( (char *)tmp.goods_name, "联塑塑料管" );
		::sprintf( (char *)tmp.total_size, "14吨" );
		::sprintf( (char *)tmp.address, "番村仓储物流中心" );
		m_goodSourceMgr.AddOneItem(tmp);

		::sprintf( (char *)tmp.code, "GDFS000035" );
		::sprintf( (char *)tmp.goods_name, "设备" );
		::sprintf( (char *)tmp.total_size, "35吨" );
		::sprintf( (char *)tmp.address, "国美电器佛山物流中心" );
		m_goodSourceMgr.AddOneItem(tmp);

		::sprintf( (char *)tmp.code, "GDFS000036" );
		::sprintf( (char *)tmp.goods_name, "联塑塑料管" );
		::sprintf( (char *)tmp.total_size, "16吨" );
		::sprintf( (char *)tmp.address, "新华书店物流中心" );
		m_goodSourceMgr.AddOneItem(tmp);

		::sprintf( (char *)tmp.code, "GDFS000037" );
		::sprintf( (char *)tmp.goods_name, "地砖" );
		::sprintf( (char *)tmp.total_size, "40吨" );
		::sprintf( (char *)tmp.address, "佛山市城北物流中心" );
		m_goodSourceMgr.AddOneItem(tmp);

		::sprintf( (char *)tmp.code, "GDFS000038" );
		::sprintf( (char *)tmp.goods_name, "家具" );
		::sprintf( (char *)tmp.total_size, "8吨" );
		::sprintf( (char *)tmp.address, "德邦物流中心" );
		m_goodSourceMgr.AddOneItem(tmp);

		::sprintf( (char *)tmp.code, "GDFS000039" );
		::sprintf( (char *)tmp.goods_name, "钢管" );
		::sprintf( (char *)tmp.total_size, "37吨" );
		::sprintf( (char *)tmp.address, "明威陶瓷物流中心" );
		m_goodSourceMgr.AddOneItem(tmp);

		::sprintf( (char *)tmp.code, "GDFS000040" );
		::sprintf( (char *)tmp.goods_name, "家具" );
		::sprintf( (char *)tmp.total_size, "10吨" );
		::sprintf( (char *)tmp.address, "恒晖物流中心" );
		m_goodSourceMgr.AddOneItem(tmp);

		::sprintf( (char *)tmp.code, "GDFS000041" );
		::sprintf( (char *)tmp.goods_name, "铝板" );
		::sprintf( (char *)tmp.total_size, "8吨" );
		::sprintf( (char *)tmp.address, "大沥兴腾物流中心" );
		m_goodSourceMgr.AddOneItem(tmp);

		::sprintf( (char *)tmp.code, "GDFS000042" );
		::sprintf( (char *)tmp.goods_name, "家具" );
		::sprintf( (char *)tmp.total_size, "20吨" );
		::sprintf( (char *)tmp.address, "铁煌钢铁仓储物流中心" );
		m_goodSourceMgr.AddOneItem(tmp);

		::sprintf( (char *)tmp.code, "GDFS000043" );
		::sprintf( (char *)tmp.goods_name, "饮料" );
		::sprintf( (char *)tmp.total_size, "35方" );
		::sprintf( (char *)tmp.address, "南方特产物流中心" );
		m_goodSourceMgr.AddOneItem(tmp);

		::sprintf( (char *)tmp.code, "GDFS000044" );
		::sprintf( (char *)tmp.goods_name, "普宁水管" );
		::sprintf( (char *)tmp.total_size, "15方" );
		::sprintf( (char *)tmp.address, "遐志物流中心" );
		m_goodSourceMgr.AddOneItem(tmp);

		::sprintf( (char *)tmp.code, "GDFS000045" );
		::sprintf( (char *)tmp.goods_name, "设备" );
		::sprintf( (char *)tmp.total_size, "23吨" );
		::sprintf( (char *)tmp.address, "敦豪物流中心" );
		m_goodSourceMgr.AddOneItem(tmp);

		::sprintf( (char *)tmp.code, "GDFS000046" );
		::sprintf( (char *)tmp.goods_name, "棉花包" );
		::sprintf( (char *)tmp.total_size, "35吨" );
		::sprintf( (char *)tmp.address, "现通物流中心" );
		m_goodSourceMgr.AddOneItem(tmp);

		::sprintf( (char *)tmp.code, "GDFS000047" );
		::sprintf( (char *)tmp.goods_name, "电器" );
		::sprintf( (char *)tmp.total_size, "26吨" );
		::sprintf( (char *)tmp.address, "丰禾园现代物流中心" );
		m_goodSourceMgr.AddOneItem(tmp);

		::sprintf( (char *)tmp.code, "GDFS000048" );
		::sprintf( (char *)tmp.goods_name, "设备" );
		::sprintf( (char *)tmp.total_size, "2吨" );
		::sprintf( (char *)tmp.address, "亚昌货运物流中心" );
		m_goodSourceMgr.AddOneItem(tmp);
	}
	m_goodSourceMgr.DisconnectFile();
}

		