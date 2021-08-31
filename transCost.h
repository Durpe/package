#pragma once
#include <qwidget.h>
#include "ui_transCost.h"

//����ɱ�����
class transCost : public QWidget
{

	Q_OBJECT

public:
	transCost(QWidget* parent = Q_NULLPTR);
	
	void InitTransCost(); // ��ʼ�����棨Ĭ��ֵ��
	void InitTransCost(QString transInfo);	//��ʼ�����棨����ֵ��
	
public slots:
	void SetDefaultSea(); // ����ʱ�����װ��Ĭ��
	void CalcFullRadio();// ����ʵװ��
	void SetDefaultAir(); // ����ʱ�����װ��Ĭ��
	void CalcResult(); // ������
	void ExportExcel(); // ������ť


private:
	Ui::transCost ui;
	struct info_struct 
	{
		QString o_iner;		// ����/����
		QString certain;	// ������
		QString country;	// ����
		QString length;		// ����(mm)
		QString width;		// ���(mm)
		QString height;		// �߶�(mm)
		QString weight;		// ����(kg)
		QString container_type;		// ��������
		QString actual_pack_rate;	// ��һ��װ���ڻ����е�ʵװ��
		QString	air_trans_rate;		// ���˱���
		QString not_air_trans_rate;	// �ǿ��˱���
		QString trans_cost;			// �ð�װ����һ�������
		QString trans_balance;		// �ð�װ����������˵ļ�ֵƽ���
		QString comment;			// ר�ҵ���
	};
	info_struct GetTotalInfo();
};

