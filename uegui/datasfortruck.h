#ifndef _UEGUI_DATASFORTRUCK_H
#define _UEGUI_DATASFORTRUCK_H

namespace UeGui
{
	//货源信息
	typedef struct GoodsSource
	{
		const static int MaxLength=128;
		const static int MinLength=32;

		//编号
		char code[MinLength];
		//货物名称
		char goods_name[MaxLength];	
		//货物重量/体积
		char total_size[MinLength];
		//配货站
		char address[MaxLength];

		GoodsSource()
		{
			memset(code,0,MinLength);
			memset(goods_name,0,MaxLength);
			memset(total_size,0,MinLength);
			memset(address,0,MaxLength);
		}
	} GoodsSource;

	//货物详情
	typedef struct GoodsSourceDetail
	{
		const static int MaxLength=128;
		const static int MinLength=32;

		//货源信息索引
		int Idx_GoodsSource;
		//货物类型
		char goods_type[MinLength];
		//包装方式
		int packingWay;
		//发货地点
		char srcPlace[MaxLength];
		//目的地点
		char dstPlace[MaxLength];
		//发货点Q码
		char srcQcode[10];
		//目的地Q码
		char dstQcode[10];
		//发货时间
		char sendOutTime[MinLength];
		//运输价格
		float transPrice;
		//运输时间期限
		char limitTime[MinLength];
		//结算方式
		char payWay[MinLength];
		//发布日期
		char publishTime[MinLength];
		//发布商家
		char publisher[MinLength];
		//联系电话
		char telphone[MinLength];
		//备注
		char notes[MinLength];
	} GoodsSourceDetail;

	//物流信息
	typedef struct LogisticsTrackInfo
	{
		const static int MaxLength=128;
		//const static int MinLength=32;

		//索引
		int idx_SourceDetail;
		//备注
		char notes[MaxLength];

		LogisticsTrackInfo():idx_SourceDetail(-1)
		{
			memset(notes,0,MaxLength);
		}
	} LogisticsTrackInfo;

}

#endif
