// Copyright (c) 2009-2017 Satoshi Nakamoto
// Copyright (c) 2009-2017 The Bitcoin Developers
// Copyright (c) 2016-2017 Duality Blockchain Solutions Developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef SEQUENCE_QT_NETWORKSTYLE_H
#define SEQUENCE_QT_NETWORKSTYLE_H

#include <QIcon>
#include <QPixmap>
#include <QString>

/* Coin network-specific GUI style information */
class NetworkStyle
{
public:
    /** Get style associated with provided BIP70 network id, or 0 if not known */
    static const NetworkStyle *instantiate(const QString &networkId);

    const QString &getAppName() const { return appName; }
    const QIcon &getAppIcon() const { return appIcon; }
    const QString &getTitleAddText() const { return titleAddText; }
    const QPixmap &getSplashImage() const { return splashImage; }

private:
    NetworkStyle(const QString &appName, const QString &appIcon, const char *titleAddText, const QString &splashImage);

    QString appName;
    QIcon appIcon;
    QString titleAddText;
    QPixmap splashImage;
};

#endif // SEQUENCE_QT_NETWORKSTYLE_H
