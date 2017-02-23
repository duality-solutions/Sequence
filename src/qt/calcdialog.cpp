// Copyright (c) 2015-2017 Silk Network Developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "calcdialog.h"
#include "ui_calcdialog.h"

#include "silkunits.h"
#include "transactiontablemodel.h"
#include "transactiondesc.h"
#include "transactionrecord.h"
#include "walletmodel.h"

#include "main.h"
#include "wallet/wallet.h"

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

// Calculates the percent profit within the user defined timeframe (in days)
void calcDialog::pushButtonClicked()
{
	float rate = 0;
	CWalletTx tx;
	CWalletTx ptx;
	const TransactionRecord *wtx = nullptr;
	uint256 hash;

	QString strRewardSize = ui->stakeDaysEdit->text(); // strUserSize, blockSizeEdit
	float days = strRewardSize.toFloat();

	//if (wtx->decomposeTransaction(wallet->hash, &tx)) 
	{
		if (tx.vin.size() == 1) 
		{
			rate = 100.0f * (wtx->credit + wtx->debit) / -wtx->debit;

			//if (wallet->GetTransaction(tx->vin[0].prevout.hash, ptx)) 
			{
				days = (tx.nTime - ptx.nTime) / 86400.0f;

				strRewardSize += "\n" + tr("%1% SLK staked in %2 days").arg(rate).arg(days);
			}
		}
	}
	
}

void calcDialog::on_buttonBox_accepted()
{
	close();
}
