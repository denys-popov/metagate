#ifndef TRANSACTIONS_H
#define TRANSACTIONS_H

#include <QObject>
#include <QString>

#include <functional>
#include <vector>
#include <map>

#include "client.h"
#include "TimerClass.h"

class NsLookup;

namespace transactions {

class Transactions : public TimerClass {
    Q_OBJECT
public:

    using RegisterAddressCallback = std::function<void()>;

public:

    explicit Transactions(NsLookup &nsLookup, QObject *parent = nullptr);

signals:

    void registerAddress(const QString &currency, const QString &address, const QString &type, const RegisterAddressCallback &callback);

public slots:

    void onRegisterAddress(const QString &currency, const QString &address, const QString &type, const RegisterAddressCallback &callback);

private slots:

    void onCallbackCall(std::function<void()> callback);

    void onRun();

    void onTimerEvent();

private:

    void processAddressMth(const QString &address, const std::vector<QString> &servers);

private:

    NsLookup &nsLookup;

    SimpleClient client;

    std::map<QString, bool> getFullTxs;
};

}

#endif // TRANSACTIONS_H