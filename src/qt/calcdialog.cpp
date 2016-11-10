// Copyright (c) 2015-2016 Silk Network Developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "calcdialog.h"
#include "ui_calcdialog.h"
#include <QString>

calcDialog::calcDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::calcDialog)
{
    ui->setupUi(this);
	
	connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(pushButtonClicked()));
}

calcDialog::~calcDialog()
{
    delete ui;
}

void calcDialog::setModel(ClientModel *model)
{

}


void calcDialog::pushButtonClicked()
{
	QString strUserSize = ui->blockSizeEdit->text();
	double dUserBlock = strUserSize.toDouble();
	double dMax = 1000;
	
}

void calcDialog::on_buttonBox_accepted()
{
	close();
}