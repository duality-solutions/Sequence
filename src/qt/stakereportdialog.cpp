//*****************************************************
//
// Dialog which report the earning made with stake over time
// Original coding by Remy5
//

#include "stakereportdialog.h"
#include "ui_stakereportdialog.h"      

#include "guiconstants.h"
#include "walletmodel.h"
#include "sequenceunits.h"
#include "rpc/rpcserver.h"
#include "optionsmodel.h"
#include "main.h"
#include "chain.h"
#include "util.h"

#include "wallet/wallet.h"
#include "wallet/walletdb.h"

#include <QWidget>
#include <QDateTime>
#include <QObject>
#include <QTimer>
#include <QClipboard>

// using namespace json_spirit;
using namespace boost;
using namespace std;

struct StakePeriodRange_T {
    int64_t Start;
    int64_t End;
    int64_t Total;
    int Count;
    string Name;
};

typedef vector<StakePeriodRange_T> vStakePeriodRange_T;

extern vStakePeriodRange_T PrepareRangeForStakeReport();
extern int GetStakeSubTotal(vStakePeriodRange_T& aRange, const isminefilter filter);

StakeReportDialog::StakeReportDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StakeReportDialog)
{
    ui->setupUi(this);

    QTableWidget *TableW = ui->StakeReportTable;

    alreadyConnected = false;

    // fill the table with clone of row 0
    for(int y=TableW->rowCount(); --y >= 1;)
        for(int x=TableW->columnCount(); --x >= 0;)
            TableW->setItem(y, x,
                TableW->item(0, x)->clone());

    TableW->horizontalHeader()->resizeSection(1,160);

    QApplication::processEvents();

    updateStakeReportNow();  // 1st update
}

StakeReportDialog::~StakeReportDialog()
{
    delete ui;
}

void StakeReportDialog::setModel(WalletModel *model)
{
    this->ex_model = model;

    if(ex_model && ex_model->getOptionsModel() && !alreadyConnected)
    {
        alreadyConnected = true;

        connect(ex_model->getOptionsModel(), SIGNAL(displayUnitChanged(int)), this, SLOT(updateDisplayUnit(int)));
        connect(ui->button_Refresh, SIGNAL(clicked()), this, SLOT(updateStakeReportNow()));
        connect(ui->CopytoClipboard, SIGNAL(clicked()), this, SLOT(CopyAllToClipboard()));

        disablereportupdate = GetBoolArg("-disablereportupdate", false);

        if (!disablereportupdate)
        {
            QTimer *timer = new QTimer(this);
            connect(timer, SIGNAL(timeout()), this, SLOT(updateStakeReportTimer()));
         	connect(model, SIGNAL(balanceChanged(CAmount,CAmount,CAmount,CAmount,CAmount,CAmount,CAmount,CAmount,CAmount)), this, SLOT(updateStakeReportbalanceChanged(CAmount,CAmount,CAmount,CAmount,CAmount,CAmount,CAmount,CAmount,CAmount)));

            timer->start(MODEL_UPDATE_DELAY*5);
        }
    }
}

void StakeReportDialog::updateStakeReportbalanceChanged(const CAmount& balance, const CAmount& total, const CAmount& stake, const CAmount& unconfirmedBalance,
                              const CAmount& immatureBalance, const CAmount& watchOnlyBalance,const CAmount& watchOnlyStake, 
                              const CAmount& watchUnconfBalance, const CAmount& watchImmatureBalance)
{
    StakeReportDialog::updateStakeReportNow();
}

void StakeReportDialog::updateDisplayUnit(int)
{
    StakeReportDialog::updateStakeReportNow();
}

void StakeReportDialog::updateStakeReportTimer()
{
    static int lastBest = 0 ;
    if (lastBest != chainActive.Height())
    {
        lastBest = chainActive.Height();
        StakeReportDialog::updateStakeReport(false);
    }
}

void StakeReportDialog::showEvent( QShowEvent* event )
{
    QWidget::showEvent( event );
    StakeReportDialog::updateStakeReportNow();
}

// Extendable localtime format
QString HalfDate(int64_t nTime, QString TimeMode="")
{
    QDateTime OrigDate = QDateTime::fromTime_t((qint32)nTime);
    QString LocalDate = OrigDate.date().toString("yyyy-MM-dd");
    if (TimeMode != "")
        LocalDate += " " + OrigDate.toString(TimeMode);

    return LocalDate;
}

// format Sequencevalue with all trailing zero
QString Coin_0Pad(int nUnit, int64_t amount)
{
    QString result = SequenceUnits::format(nUnit, amount);

    int poin = result.indexOf(".") + 1;
    poin += SequenceUnits::decimals(nUnit);

    return result.leftJustified(poin, '0');
}

void StakeReportDialog::updateStakeReportNow()
{
    updateStakeReport(true);
}

void StakeReportDialog::updateStakeReport(bool fImmediate=false)
{
    static vStakePeriodRange_T aRange;
    int nItemCounted=0;

    if (fImmediate) nLastReportUpdate = 0;

    if (this->isHidden())
        return;

    int64_t nTook = GetTimeMillis();

    // Skip report recalc if initially downloading the blockchain
    if (IsInitialBlockDownload())
    {
        QApplication::processEvents();

        ui->TimeTook->setText(tr("Please wait downloading blocks..."));
        ui->TimeTook->repaint();
        QApplication::processEvents();

        return;
    }

    // Skip report recalc if not immediate or before 5 minutes from last
    if (GetTime() - nLastReportUpdate > 300)
    {
        QApplication::processEvents();
        ui->TimeTook->setText(tr("Please wait..."));
        ui->TimeTook->repaint();
        QApplication::processEvents();

        aRange = PrepareRangeForStakeReport();

        // get subtotal calc
        nItemCounted = GetStakeSubTotal(aRange, ISMINE_SPENDABLE);

        nLastReportUpdate = GetTime();

        nTook = GetTimeMillis() - nTook;

    }

    int64_t nTook2 = GetTimeMillis();

    // actually update labels
    int nDisplayUnit = SequenceUnits::SEQ;
    if (ex_model && ex_model->getOptionsModel())
         nDisplayUnit = ex_model->getOptionsModel()->getDisplayUnit();

    ui->L_Coin->setText(SequenceUnits::name(nDisplayUnit) + " " + tr("SubTotal"));

    QTableWidget *TableW = ui->StakeReportTable;

    TableW->horizontalHeaderItem(1)->setText(SequenceUnits::name(nDisplayUnit) + " " +tr("Amount"));

    int i=30;

    TableW->setSortingEnabled(false);
    for(int y=0; y<i; y++)
    {
       TableW->item(y,0)->setText(HalfDate(aRange[y].Start));
       TableW->item(y,1)->setText(Coin_0Pad(nDisplayUnit, aRange[y].Total));
       TableW->item(y,2)->setText(QString::number(aRange[y].Count));
    }
    TableW->setSortingEnabled(true);

    ui->Amount_24H->setText(Coin_0Pad(nDisplayUnit, aRange[i].Total) + tr(" SEQ"));
    ui->Stake_24H->setText(QString::number(aRange[i++].Count));
    ui->Amount_7D->setText(Coin_0Pad(nDisplayUnit, aRange[i].Total) + tr(" SEQ"));
    ui->Stake_7D->setText(QString::number(aRange[i++].Count));
    ui->Amount_30D->setText(Coin_0Pad(nDisplayUnit, aRange[i].Total) + tr(" SEQ"));
    ui->Stake_30D->setText(QString::number(aRange[i++].Count));
    ui->Amount_365D->setText(Coin_0Pad(nDisplayUnit, aRange[i].Total) + tr(" SEQ"));
    ui->Stake_365D->setText(QString::number(aRange[i++].Count));

    string sRefreshType = disablereportupdate ? "Manual refresh" : "Auto refresh";

    string strCurr_block_info = strprintf("%s  -  %s : %6d @ %s\nhash %s\n",
           sRefreshType.c_str(), "Current Block", chainActive.Height(),
           HalfDate((chainActive.Tip()->GetMedianTimePast()+1), "hh:mm:ss").toStdString().c_str(),
           chainActive.Tip()->GetBlockHash().ToString());

    if (IsInitialBlockDownload())
    {
        ui->L_CurrentBlock->setText(tr(""));
        ui->TimeTook_2->setText(tr(""));
        ui->Amount_Last->setText(tr(""));
        ui->L_LastStakeTime->setText(tr("Syncing Blockchain"));
    } else {
        ui->L_CurrentBlock->setText(strCurr_block_info.c_str() );
        ui->TimeTook_2->setText(tr("Refresh took ") + QString::number(GetTimeMillis() -nTook2) +  "ms");
        ui->Amount_Last->setText(tr("Amount: ") + Coin_0Pad(nDisplayUnit, aRange[i].Total) + tr(" SEQ"));
        ui->L_LastStakeTime->setText(tr("Latest stake date: ") + HalfDate(aRange[i].Start, "hh:mm"));
    }

    ui->Stake_Counted->setText(tr("Stakes analysed: ") + QString::number(nItemCounted));
    if (nItemCounted)
        ui->TimeTook->setText(tr("Last Recalc took ") + QString::number(nTook) +  "ms");
}

QString GridGetLabelTextAt(QGridLayout * Grid, int row, int column, QString Empty = "")
{
    if (Grid && Grid->itemAtPosition(row, column) &&
            Grid->itemAtPosition(row, column)->widget())
        return ((QLabel *) Grid->itemAtPosition(row, column)->widget())->text();
    else
        return Empty;
}

void StakeReportDialog::CopyAllToClipboard()
{
    QString Repo;

    Repo += "           Sequence Stake Mini Report\n";
    Repo += "           ----------------------\n";

    QString  RowForm = "%1 %2 %3\n";

    for(int y=0; y<ui->gridLayout->rowCount(); y++)
    {
        if (y == 5)
            Repo += "\n"; //  separator line
        else
            Repo += RowForm
                .arg(GridGetLabelTextAt(ui->gridLayout, y,0), -16)
                .arg(GridGetLabelTextAt(ui->gridLayout, y,1), 16)
                .arg(GridGetLabelTextAt(ui->gridLayout, y,2), 7);
    }

    Repo += "\n";

    QTableWidget *TableW = ui->StakeReportTable;
    RowForm = "%1, %2, %3\n";

    Repo += RowForm
        .arg(TableW->horizontalHeaderItem(0)->text(), -10)
        .arg(TableW->horizontalHeaderItem(1)->text(), 16)
        .arg(TableW->horizontalHeaderItem(2)->text(), 7);

    for(int y=0; y<30; y++)
    {
        Repo += RowForm
            .arg(TableW->item(y,0)->text(), -10)
            .arg(TableW->item(y,1)->text(), 16)
            .arg(TableW->item(y,2)->text(), 7);
    }

    Repo += "\n" + ui->L_CurrentBlock->text() + "\n";

    QApplication::clipboard()->setText(Repo);

}