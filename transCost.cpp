#include "transCost.h"
#include "QMessageBox"
#include "QExcel.h"
#include "QFileDialog"
#include "QDesktopServices"
transCost::transCost(QWidget* parent /*= Q_NULLPTR*/)
{
	ui.setupUi(this);
	InitTransCost();
	// ��ť��Ӧ
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(SetDefaultSea()));
	connect(ui.pushButton_4, SIGNAL(clicked()), this, SLOT(SetDefaultAir()));
	connect(ui.pushButton_6, SIGNAL(clicked()), this, SLOT(ExportExcel()));
	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(CalcFullRadio()));

}



// ��ʼ�����棨Ĭ��ֵ��
void transCost::InitTransCost()
{
	// �����װ����ߴ缰����
	ui.plainTextEdit_2->setPlainText(QString("800"));
	ui.plainTextEdit_3->setPlainText(QString("600"));
	ui.plainTextEdit_4->setPlainText(QString("1620"));
	ui.plainTextEdit_5->setPlainText(QString("100"));
	//ѡ�񷢻���ַgroup
	ui.comboBox->addItem(QString(u8"ȫ��"));
	ui.comboBox->addItem(QString(u8"����"));
	ui.comboBox->addItem(QString(u8"�й�"));
	ui.comboBox_2->addItem(QString(u8"UnKnown..."));
	ui.comboBox_3->addItem(QString(u8"UnKnown..."));

	// ����ʱ�����װ��
	ui.comboBox_4->addItems(QStringList("40HQ"));
	ui.comboBox_4->addItems(QStringList("20GP"));
	ui.comboBox_4->addItems(QStringList("40GP"));
	ui.plainTextEdit_8->setPlainText(QString(u8"80"));

	// ָ�����˱���
	ui.plainTextEdit_6->setPlainText(QString(u8"7.5"));
	ui.plainTextEdit_7->setPlainText(QString(u8"92.5"));

	// �鿴������
	ui.plainTextEdit_9->setPlainText(QString(u8"690.18"));
	ui.plainTextEdit_10->setPlainText(QString(u8"37.15"));

	// ר�ҵ���
	ui.plainTextEdit->setPlainText(QString(u8"�� �ð�װ���ܶ�Ϊ 128.6Kg/m3��DFX��װ���ܶȵȼ�ΪI��(С��130Kg/m3)�������Ż���ƣ���С����� \
		\n�� ��װ���߶ȳ���1.6�ף�����ʱ��������ƿ��ܻ�൹���䣬�뿼�ǵ�������ɿ��Ի��ֹ���ˡ�"));
}

// ��ʼ�����棨����ֵ��
void transCost::InitTransCost(QString transInfo)
{

}

// ����ʱ�����װ��Ĭ��
void transCost::SetDefaultSea()
{
	//ѡ�񷢻���ַgroup
	// ����ʱ�����װ��
	ui.comboBox_4->clear();
	ui.comboBox_4->addItems(QStringList("40HQ"));
	ui.comboBox_4->addItems(QStringList("20GP"));
	ui.comboBox_4->addItems(QStringList("40GP"));
	ui.plainTextEdit_8->setPlainText(QString(u8"80"));
}

// ����ʱ�����װ��Ĭ��
void transCost::SetDefaultAir()
{
	// ָ�����˱���
	ui.plainTextEdit_6->setPlainText(QString(u8"7.5"));
	ui.plainTextEdit_7->setPlainText(QString(u8"92.5"));
	//QMessageBox::information(NULL, "Tips", "2", QMessageBox::Ok);
}

// ������
void transCost::CalcResult()
{

}

// ����ʵװ��
void transCost::CalcFullRadio()
{

	BPContainerData container;
	if (!GetContainerData(strContainerType, container))
	{
		strErrMsg.AppendFormat(L"�޷���ȡ��װ�������\r\n");
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
		strErrMsg.AppendFormat(L"������װ���߼�װ��ߴ�Ϊ0��ֵ");
		return ;
	}
	NumWMax = int(ContainerWidth / PackageLength);
	NumLMax = int(ContainerWidth / PackageWidth);
	NumHMax = int(ContainerHeight / PackageHeight);
	NumWContainerL = int(ContainerLength / PackageLength);
	NumLContainerL = int(ContainerLength / PackageWidth);

	if (NumHMax <= 0 || (NumWMax <= 0 && NumWContainerL <= 0) || (NumLMax <= 0 && NumLContainerL <= 0))
	{
		strErrMsg.AppendFormat(L"������װ��Ԫ�����˼�װ��ߴ�");
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
				// ����ʣ��ռ��Ƿ���Լ����ڷ�
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

// ������ť
void transCost::ExportExcel()
{
	// ��ȡ����
	info_struct trans_cost;
	trans_cost = GetTotalInfo();


	// ��
	int row = 1;

	// ��
	int column = 14;
	QString dirpath = QFileDialog::getExistingDirectory(this,
		QString::fromLocal8Bit("ѡ��Ŀ¼"), "./", QFileDialog::ShowDirsOnly);
	dirpath += u8"/����ɱ�����.xlsx";
	QExcel qexcel(dirpath);
	qexcel.selectSheet(1, QString::fromLocal8Bit("����ɱ�����"));
	qexcel.selectSheet(1);
	// �����б���
	qexcel.setCellString(1, 1, QString(u8"����/����"));
	qexcel.setCellString(1, 2, QString(u8"������"));
	qexcel.setCellString(1, 3, QString(u8"����"));
	qexcel.setCellString(1, 4, QString(u8"����(mm)"));
	qexcel.setCellString(1, 5, QString(u8"���(mm)"));
	qexcel.setCellString(1, 6, QString(u8"�߶�(mm)"));
	qexcel.setCellString(1, 7, QString(u8"����(kg)"));
	qexcel.setCellString(1, 8, QString(u8"��������"));
	qexcel.setCellString(1, 9, QString(u8"��һ��װ���ڻ����е�ʵװ��"));
	qexcel.setCellString(1, 10, QString(u8"���˱���"));
	qexcel.setCellString(1, 11, QString(u8"�ǿ��˱���"));
	qexcel.setCellString(1, 12, QString(u8"�ð�װ����һ�������"));
	qexcel.setCellString(1, 13, QString(u8"�ð�װ����������˵ļ�ֵƽ���"));
	qexcel.setCellString(1, 14, QString(u8"ר�ҵ���"));
	for (int i = 1; i < column; i++)
	{
		// ����һ�еĿ�� 
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
	qexcel.setCellString(2, 13, trans_cost.trans_balance + QString(u8"		��Ԫ"));
	qexcel.setCellString(2, 14, trans_cost.comment);
	
	// �����иߣ��п�
	qexcel.setRowHeight(2, 100);
	qexcel.setColumnWidth(column, 100);
	
	
	
	QString strPath = "D:\\MYCODE\\QtPackaging\\QtPackaging\\prt.png";
	QString strCell = "A1:N1";
	qexcel.setCellTextCenter(strCell);
	// ���沢�ر�
	qexcel.save();
	qexcel.saveFile();
	qexcel.close();


	QMessageBox::StandardButton result = QMessageBox::information(this, QString::fromLocal8Bit("����"),
		QString::fromLocal8Bit("�ļ��Ѿ�����ɹ�,�Ƿ��?"), QMessageBox::Yes | QMessageBox::No);
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
	// ��ȡ��ǰ������������
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



