// Copyright (c) 2016-2017 Duality Blockchain Solutions Developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef CALCDIALOG_H
#define CALCDIALOG_H

#include <QDialog>

namespace Ui {
class calcDialog;
}
class ClientModel;

class calcDialog : public QDialog
{
    Q_OBJECT

public:
    explicit calcDialog(QWidget *parent = 0);
    ~calcDialog();

    void setModel(ClientModel *model);
private Q_SLOTS:
    void on_buttonBox_accepted();
	void pushButtonClicked();

private:
    Ui::calcDialog *ui;
};

#endif // CALCDIALOG_H