// Copyright (c) 2009-2016 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Developers
// Copyright (c) 2015-2016 Silk Network Developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef SILK_QT_SILKADDRESSVALIDATOR_H
#define SILK_QT_SILKADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class SilkAddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit SilkAddressEntryValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

/** Silk address widget validator, checks for a valid Silk address.
 */
class SilkAddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit SilkAddressCheckValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

#endif // SILK_QT_SILKADDRESSVALIDATOR_H
