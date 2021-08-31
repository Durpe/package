#include "transCost.h"
#include "QMessageBox"
#include "QExcel.h"
#include "QFileDialog"
#include "QDesktopServices"
transCost::transCost(QWidget* parent /*= Q_NULLPTR*/)
{
	ui.setupUi(this);
	InitTransCost();
	// 按钮响应
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(SetDefaultSea()));
	connect(ui.pushButton_4, SIGNAL(clicked()), this, SLOT(SetDefaultAir()));
	connect(ui.pushButton_6, SIGNAL(clicked()), this, SLOT(ExportExcel()));
	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(CalcFullRadio()));

}



// 初始化界面（默认值）
void transCost::InitTransCost()
{
	// 运输包装件外尺寸及重量
	ui.plainTextEdit_2->setPlainText(QString("800"));
	ui.plainTextEdit_3->setPlainText(QString("600"));
	ui.plainTextEdit_4->setPlainText(QString("1620"));
	ui.plainTextEdit_5->setPlainText(QString("100"));
	//选择发货地址group
	ui.comboBox->addItem(QString(u8"全球"));
	ui.comboBox->addItem(QString(u8"海外"));
	ui.comboBox->addItem(QString(u8"中国"));
	ui.comboBox_2->addItem(QString(u8"UnKnown..."));
	ui.comboBox_3->addItem(QString(u8"UnKnown..."));

	// 海运时运输包装件
	ui.comboBox_4->addItems(QStringList("40HQ"));
	ui.comboBox_4->addItems(QStringList("20GP"));
	ui.comboBox_4->addItems(QStringList("40GP"));
	ui.plainTextEdit_8->setPlainText(QString(u8"80"));

	// 指定空运比例
	ui.plainTextEdit_6->setPlainText(QString(u8"7.5"));
	ui.plainTextEdit_7->setPlainText(QString(u8"92.5"));

	// 查看核算结果
	ui.plainTextEdit_9->setPlainText(QString(u8"690.18"));
	ui.plainTextEdit_10->setPlainText(QString(u8"37.15"));

	// 专家点评
	ui.plainTextEdit->setPlainText(QString(u8"◆ 该包装件密度为 128.6Kg/m3，DFX包装件密度等级为I级(小于130Kg/m3)，建议优化设计，减小体积。 \
		\n◆ 包装件高度超过1.6米，空运时因舱门限制可能会侧倒运输，请考虑倒放运输可靠性或禁止空运。"));
}

// 初始化界面（传入值）
void transCost::InitTransCost(QString transInfo)
{

}

// 海运时运输包装件默认
void transCost::SetDefaultSea()
{
	//选择发货地址group
	// 海运时运输包装件
	ui.comboBox_4->clear();
	ui.comboBox_4->addItems(QStringList("40HQ"));
	ui.comboBox_4->addItems(QStringList("20GP"));
	ui.comboBox_4->addItems(QStringList("40GP"));
	ui.plainTextEdit_8->setPlainText(QString(u8"80"));
}

// 空运时运输包装件默认
void transCost::SetDefaultAir()
{
	// 指定空运比例
	ui.plainTextEdit_6->setPlainText(QString(u8"7.5"));
	ui.plainTextEdit_7->setPlainText(QString(u8"92.5"));
	//QMessageBox::information(NULL, "Tips", "2", QMessageBox::Ok);
}

// 核算结果
void transCost::CalcResult()
{

}

// 计算实装率
void transCost::CalcFullRadio()
{

	BPContainerData container;
	if (!GetContainerData(strContainerType, container))
	{
		strErrMsg.AppendFormat(L"无法获取集装箱的数据\r\n");
		return;
	}

	double ContainerWidth, ContainerLength, ContainerHeight;
	int NumWMax, NumLMax, NumHMax;
	int NumWContainerW[100] = { 0 }, NumLContainerW[100] = { 0 }, NumWContainerL, NumLContainerL;
	double ResidualSpareW, ResidualSpareL;
	int NumLMaxResidualSpareL[100] = { 0 }, NumWMaxResidualSpareW[100] = { 0 }, NumWMaxResidualSpareL[100] = { 0 }, NumLMaxResidualSpareW[100] = { 0 };
	double /*FreightWidth[100], */FreightProjection[100] = { 0 };
	double FreightVolume[100] = { 0 };
	double FreightFullRatio[100] = { 0 };// 'FreightFullRatioMax
	int i, j, k, m, n;

	ContainerLength = container.length;
	ContainerWidth = container.width;
	ContainerHeight = container.height;

	if (LESS_EQUAL(PackageWidth, 0.0) || LESS_EQUAL(PackageLength, 0.0) || LESS_EQUAL(PackageHeight, 0.0) ||
		LESS_EQUAL(ContainerLength, 0.0) || LESS_EQUAL(ContainerWidth, 0.0) || LESS_EQUAL(ContainerHeight, 0.0))
	{
		strErrMsg.AppendFormat(L"单个包装或者集装箱尺寸为0或负值");
		return ;
	}
	NumWMax = int(ContainerWidth / PackageLength);
	NumLMax = int(ContainerWidth / PackageWidth);
	NumHMax = int(ContainerHeight / PackageHeight);
	NumWContainerL = int(ContainerLength / PackageLength);
	NumLContainerL = int(ContainerLength / PackageWidth);

	if (NumHMax <= 0 || (NumWMax <= 0 && NumWContainerL <= 0) || (NumLMax <= 0 && NumLContainerL <= 0))
	{
		strErrMsg.AppendFormat(L"单个包装单元超过了集装箱尺寸");
		return;
	}

	n = 0;
	for (i = NumWMax; i >= 0; i--)
	{
		for (j = NumLMax; j >= 0; j--)
		{
			if (LESS_EQUAL(PackageLength * i + PackageWidth * j, ContainerWidth))
			{
				NumWContainerW[n] = i;
				NumLContainerW[n] = j;
				// 计算剩余空间是否可以继续摆放
				if (PackageLength > PackageWidth)
				{
					ResidualSpareL = ContainerLength - NumWContainerL * PackageLength;
					ResidualSpareW = PackageWidth * NumLContainerW[n];
					NumLMaxResidualSpareL[n] = int(ResidualSpareL / PackageWidth);
					NumWMaxResidualSpareW[n] = int(ResidualSpareW / PackageLength);
				}
				else
				{
					ResidualSpareL = ContainerLength - NumLContainerL * PackageWidth;
					ResidualSpareW = PackageLength * NumWContainerW[n];
					NumWMaxResidualSpareL[n] = int(ResidualSpareL / PackageLength);
					NumLMaxResidualSpareW[n] = int(ResidualSpareW / PackageWidth);
				}
				FreightProjection[n] = (NumWContainerL * NumLContainerW[n] + NumLContainerL * NumWContainerW[n] + NumLMaxResidualSpareL[n] * NumWMaxResidualSpareW[n] + NumWMaxResidualSpareL[n] * NumLMaxResidualSpareW[n]) * (PackageLength * PackageWidth);
				if (nAllowedLayers == 0)
				{
					NumHMax = NumHMax;
				}
				else
				{
					NumHMax = min(nAllowedLayers, NumHMax);
				}
				FreightVolume[n] = FreightProjection[n] * PackageHeight * NumHMax;
				FreightFullRatio[n] = FreightVolume[n] / (container.volume * 1000.0 * 1000.0 * 1000.0);
				n = n + 1;
				goto loopend;
			}
		}
	loopend:
		;
	}

	packData.dFullRatio = FreightFullRatio[0];
	m = 0;
	for (k = 0; k < 100; k++)
	{
		for (n = k + 1; n < 100; n++)
		{
			if (GREATER_EQUAL(FreightFullRatio[n], packData.dFullRatio))
			{
				packData.dFullRatio = FreightFullRatio[n];
				m = n;
			}
		}
	}

	packData.nHorColNum = NumWContainerW[m];
	if (packData.nHorColNum == 0)
		packData.nHorRowNum = 0;
	else
		packData.nHorRowNum = NumLContainerL;
	packData.nVerColNum = NumLContainerW[m];
	if (packData.nVerColNum == 0)
		packData.nVerRowNum = 0;
	else
		packData.nVerRowNum = NumWContainerL;
	packData.nLayerNum = NumHMax;
	packData.nBoxNum = (NumWContainerL * NumLContainerW[m] + NumLContainerL * NumWContainerW[m] + NumLMaxResidualSpareL[m] * NumWMaxResidualSpareW[m] + NumWMaxResidualSpareL[m] * NumLMaxResidualSpareW[m]) * NumHMax;
	if (PackageLength > PackageWidth)
	{
		if (NumLMaxResidualSpareL[m] == 0)
			NumWMaxResidualSpareW[m] = 0;
		if (NumWMaxResidualSpareW[m] == 0)
			NumLMaxResidualSpareL[m] = 0;
		packData.nEndHorRow = NumLMaxResidualSpareL[m];
		packData.nEndHorCol = NumWMaxResidualSpareW[m];
	}
	else
	{
		if (NumWMaxResidualSpareL[m] == 0)
			NumLMaxResidualSpareW[m] = 0;
		if (NumLMaxResidualSpareW[m] == 0)
			NumWMaxResidualSpareL[m] = 0;
		packData.nEndHorRow = NumWMaxResidualSpareL[m];
		packData.nEndHorCol = NumLMaxResidualSpareW[m];
	}

	packData.container = container;
	packData.dPackLength = PackageLength;
	packData.dPackWidth = PackageWidth;
	packData.dPackHeight = PackageHeight;
	packData.dPackWeight = PackageWeight;

	return;
	ui.plainTextEdit_8->setPlainText("111");
}

// 导出按钮
void transCost::ExportExcel()
{
	// 获取数据
	info_struct trans_cost;
	trans_cost = GetTotalInfo();


	// 行
	int row = 1;

	// 列
	int column = 14;
	QString dirpath = QFileDialog::getExistingDirectory(this,
		QString::fromLocal8Bit("选择目录"), "./", QFileDialog::ShowDirsOnly);
	dirpath += u8"/运输成本核算.xlsx";
	QExcel qexcel(dirpath);
	qexcel.selectSheet(1, QString::fromLocal8Bit("运输成本核算"));
	qexcel.selectSheet(1);
	// 设置列标题
	qexcel.setCellString(1, 1, QString(u8"国内/国外"));
	qexcel.setCellString(1, 2, QString(u8"地区部"));
	qexcel.setCellString(1, 3, QString(u8"国家"));
	qexcel.setCellString(1, 4, QString(u8"长度(mm)"));
	qexcel.setCellString(1, 5, QString(u8"宽度(mm)"));
	qexcel.setCellString(1, 6, QString(u8"高度(mm)"));
	qexcel.setCellString(1, 7, QString(u8"重量(kg)"));
	qexcel.setCellString(1, 8, QString(u8"货柜类型"));
	qexcel.setCellString(1, 9, QString(u8"单一包装见在货柜中的实装率"));
	qexcel.setCellString(1, 10, QString(u8"空运比例"));
	qexcel.setCellString(1, 11, QString(u8"非空运比率"));
	qexcel.setCellString(1, 12, QString(u8"该包装件的一次运输费"));
	qexcel.setCellString(1, 13, QString(u8"该包装件海运与空运的价值平衡点"));
	qexcel.setCellString(1, 14, QString(u8"专家点评"));
	for (int i = 1; i < column; i++)
	{
		// 设置一列的宽度 
		qexcel.setColumnWidth(column, 15);
		/*QVariant str = m_model->headerData(i, Qt::Horizontal);
		QString qs = str.toString();
		qexcel.setCellString(i + 2, 1, qs);*/
	}
	qexcel.setCellString(2, 1, trans_cost.o_iner);
	qexcel.setCellString(2, 2, trans_cost.certain);
	qexcel.setCellString(2, 3, trans_cost.country);
	qexcel.setCellString(2, 4, trans_cost.length);
	qexcel.setCellString(2, 5, trans_cost.width);
	qexcel.setCellString(2, 6, trans_cost.height);
	qexcel.setCellString(2, 7, trans_cost.weight);
	qexcel.setCellString(2, 8, trans_cost.container_type);
	qexcel.setCellString(2, 9, trans_cost.actual_pack_rate + QString(u8"	%"));
	qexcel.setCellString(2, 10, trans_cost.air_trans_rate + QString(u8"		%"));
	qexcel.setCellString(2, 11, trans_cost.not_air_trans_rate + QString(u8"		%"));
	qexcel.setCellString(2, 12, trans_cost.trans_cost + QString(u8"		RMB"));
	qexcel.setCellString(2, 13, trans_cost.trans_balance + QString(u8"		万元"));
	qexcel.setCellString(2, 14, trans_cost.comment);
	
	// 设置行高，列宽
	qexcel.setRowHeight(2, 100);
	qexcel.setColumnWidth(column, 100);
	
	
	
	QString strPath = "D:\\MYCODE\\QtPackaging\\QtPackaging\\prt.png";
	QString strCell = "A1:N1";
	qexcel.setCellTextCenter(strCell);
	// 保存并关闭
	qexcel.save();
	qexcel.saveFile();
	qexcel.close();


	QMessageBox::StandardButton result = QMessageBox::information(this, QString::fromLocal8Bit("警告"),
		QString::fromLocal8Bit("文件已经保存成功,是否打开?"), QMessageBox::Yes | QMessageBox::No);
	switch (result)
	{
	case QMessageBox::Yes:
		QDesktopServices::openUrl(QUrl::fromLocalFile(dirpath));
		break;
	case QMessageBox::No:
		break;
	default:
		break;
	}

}

transCost::info_struct transCost::GetTotalInfo()
{
	// 获取当前界面所有数据
	info_struct trans_cost;
	trans_cost.o_iner = ui.comboBox->currentText();
	trans_cost.certain = ui.comboBox_2->currentText();
	trans_cost.country = ui.comboBox_3->currentText();
	trans_cost.container_type = ui.comboBox_4->currentText();
	trans_cost.length = ui.plainTextEdit_2->toPlainText();
	trans_cost.width = ui.plainTextEdit_3->toPlainText();
	trans_cost.height = ui.plainTextEdit_4->toPlainText();
	trans_cost.weight = ui.plainTextEdit_5->toPlainText();
	trans_cost.actual_pack_rate = ui.plainTextEdit_8->toPlainText();
	trans_cost.air_trans_rate = ui.plainTextEdit_6->toPlainText();
	trans_cost.not_air_trans_rate = ui.plainTextEdit_7->toPlainText();
	trans_cost.trans_cost = ui.plainTextEdit_9->toPlainText();
	trans_cost.trans_balance = ui.plainTextEdit_10->toPlainText();
	trans_cost.comment = ui.plainTextEdit->toPlainText();
	return trans_cost;
}



