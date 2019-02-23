// Copyright (c) 2009-2019 Satoshi Nakamoto
// Copyright (c) 2009-2019 The Bitcoin Developers
// Copyright (c) 2016-2019 Duality Blockchain Solutions Developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef SEQUENCE_QT_SEQUENCEADDRESSVALIDATOR_H
#define SEQUENCE_QT_SEQUENCEADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class SequenceAddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit SequenceAddressEntryValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

/** Sequence address widget validator, checks for a valid Sequence address.
 */
class SequenceAddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit SequenceAddressCheckValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

#endif // SEQUENCE_QT_SEQUENCEADDRESSVALIDATOR_H
