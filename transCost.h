#pragma once
#include <qwidget.h>
#include "ui_transCost.h"

//运输成本核算
class transCost : public QWidget
{

	Q_OBJECT

public:
	transCost(QWidget* parent = Q_NULLPTR);
	
	void InitTransCost(); // 初始化界面（默认值）
	void InitTransCost(QString transInfo);	//初始化界面（传入值）
	
public slots:
	void SetDefaultSea(); // 海运时运输包装件默认
	void CalcFullRadio();// 计算实装率
	void SetDefaultAir(); // 空运时运输包装件默认
	void CalcResult(); // 核算结果
	void ExportExcel(); // 导出按钮


private:
	Ui::transCost ui;
	struct info_struct 
	{
		QString o_iner;		// 国内/国外
		QString certain;	// 地区部
		QString country;	// 国家
		QString length;		// 长度(mm)
		QString width;		// 宽度(mm)
		QString height;		// 高度(mm)
		QString weight;		// 重量(kg)
		QString container_type;		// 货柜类型
		QString actual_pack_rate;	// 单一包装见在货柜中的实装率
		QString	air_trans_rate;		// 空运比例
		QString not_air_trans_rate;	// 非空运比例
		QString trans_cost;			// 该包装件的一次运输费
		QString trans_balance;		// 该包装件海运与空运的价值平衡点
		QString comment;			// 专家点评
	};
	info_struct GetTotalInfo();
};

