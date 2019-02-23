#ifndef STAKEREPORTDIALOG_H
#define STAKEREPORTDIALOG_H

#include "amount.h"

#include <QDialog>

namespace Ui {
    class StakeReportDialog;
}

class WalletModel;

class StakeReportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StakeReportDialog(QWidget *parent = 0);
    ~StakeReportDialog();

    void setModel(WalletModel *model);
    void showEvent(QShowEvent* event);

private:
    Ui::StakeReportDialog *ui;
    WalletModel *ex_model;

    qint64 nLastReportUpdate;
    bool disablereportupdate;
    bool alreadyConnected;

    void updateStakeReport(bool fImmediate);

private slots:
    void updateStakeReportTimer();

public slots:
    void updateStakeReportbalanceChanged(const CAmount& balance, const CAmount& total, const CAmount& stake, const CAmount& unconfirmedBalance, const CAmount& immatureBalance,
               const CAmount& watchOnlyBalance, const CAmount& watchOnlyStake, const CAmount& watchUnconfBalance, const CAmount& watchImmatureBalance);
    void updateStakeReportNow();
    void updateDisplayUnit(int);
    void CopyAllToClipboard();
};

#endif // STAKEREPORTDIALOG_H