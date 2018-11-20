#ifndef STAKEREPORTDIALOG_H
#define STAKEREPORTDIALOG_H

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
    void updateStakeReportbalanceChanged(const int64_t& balance, const int64_t& total, const int64_t& stake, const int64_t& unconfirmedBalance, const int64_t& immatureBalance,
               const int64_t& watchOnlyBalance, const int64_t& watchOnlyStake, const int64_t& watchUnconfBalance, const int64_t& watchImmatureBalance);
    void updateStakeReportNow();
    void updateDisplayUnit(int);
    void CopyAllToClipboard();
};

#endif // STAKEREPORTDIALOG_H