// Copyright (c) 2009-2016 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Developers
// Copyright (c) 2015-2016 Silk Network Developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef SILK_QT_OVERVIEWPAGE_H
#define SILK_QT_OVERVIEWPAGE_H

#include "amount.h"

#include <QWidget>

class ClientModel;
class TransactionFilterProxy;
class TxViewDelegate;
class WalletModel;

namespace Ui {
    class OverviewPage;
}

QT_BEGIN_NAMESPACE
class QModelIndex;
QT_END_NAMESPACE

/** Overview ("home") page widget */
class OverviewPage : public QWidget
{
    Q_OBJECT

public:
    explicit OverviewPage(QWidget *parent = 0);
    ~OverviewPage();

    void setClientModel(ClientModel *clientModel);
    void setWalletModel(WalletModel *walletModel);
    void showOutOfSyncWarning(bool fShow);
    void showCamel();
    void hideCamel();
    void moveCamel();
    
public slots:
    void setBalance(const CAmount& balance, const CAmount& total, const CAmount& stake, const CAmount& unconfirmedBalance,
                    const CAmount& immatureBalance, const CAmount& watchOnlyBalance, const CAmount& watchOnlyStake, 
                    const CAmount& watchUnconfBalance, const CAmount& watchImmatureBalance);
signals:
    void transactionClicked(const QModelIndex &index);

private:
    Ui::OverviewPage *ui;
    ClientModel *clientModel;
    WalletModel *walletModel;
    CAmount currentBalance;
    CAmount currentTotal;
    CAmount currentStake;
    CAmount currentUnconfirmedBalance;
    CAmount currentImmatureBalance;
    CAmount currentWatchOnlyBalance;
    CAmount currentWatchUnconfBalance;
    CAmount currentWatchImmatureBalance;
    CAmount currentWatchOnlyStake;

    TxViewDelegate *txdelegate;
    TransactionFilterProxy *filter;

    QMovie *gifCamel;
    bool fCamelVisibile;
    unsigned int movePixs;
    int timerId;

private slots:
    void updateDisplayUnit();
    void handleTransactionClicked(const QModelIndex &index);
    void updateAlerts(const QString &warnings);
    void updateWatchOnlyLabels(bool showWatchOnly);

protected:
    void timerEvent(QTimerEvent *event);
};

#endif // SILK_QT_OVERVIEWPAGE_H
