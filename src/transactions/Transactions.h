#ifndef TRANSACTIONS_H
#define TRANSACTIONS_H

#include <QObject>
#include <QString>

#include <functional>
#include <vector>
#include <map>
#include <set>

#include "client.h"
#include "HttpClient.h"
#include "TimerClass.h"

class NsLookup;
struct TypedException;

class JavascriptWrapper;

namespace transactions {

class TransactionsJavascript;
class TransactionsDBStorage;
struct BalanceInfo;
struct Transaction;
struct AddressInfo;

class Transactions : public TimerClass {
    Q_OBJECT
public:

    struct SendParameters {
        size_t countServersSend;
        size_t countServersGet;
        QString typeSend;
        QString typeGet;
        seconds timeout;
    };

private:

    using TransactionHash = std::string;

    class SendedTransactionWatcher {
    public:

        SendedTransactionWatcher(const SendedTransactionWatcher &) = delete;
        SendedTransactionWatcher(SendedTransactionWatcher &&) = delete;
        SendedTransactionWatcher& operator=(const SendedTransactionWatcher &) = delete;
        SendedTransactionWatcher& operator=(SendedTransactionWatcher &&) = delete;

        SendedTransactionWatcher(Transactions &txManager, const TransactionHash &hash, const time_point &startTime, const std::vector<QString> &servers, const seconds &timeout)
            : startTime(startTime)
            , timeout(timeout)
            , txManager(txManager)
            , hash(hash)
            , servers(servers.begin(), servers.end())
            , allServers(servers.begin(), servers.end())
        {}

        ~SendedTransactionWatcher();

        std::set<QString> getServersCopy() const {
            return servers;
        }

        bool isEmpty() const {
            return allServers.empty();
        }

        bool isTimeout(const time_point &now) const {
            return now - startTime >= timeout;
        }

        void removeServer(const QString &server) {
            servers.erase(server);
        }

        void returnServer(const QString &server) {
            if (allServers.find(server) != allServers.end()) {
                servers.insert(server);
            }
        }

        void okServer(const QString &server) {
            if (allServers.find(server) != allServers.end()) {
                servers.erase(server);
                allServers.erase(server);
            }
        }

        void setError(const QString &server, const QString &error) {
            errors[server] = error;
        }

    private:
        const time_point startTime;
        const seconds timeout;

        Transactions &txManager;

        TransactionHash hash;

        std::set<QString> servers;
        std::set<QString> allServers;
        std::map<QString, QString> errors;
    };

    struct ServersStruct {
        int countRequests = 0;
        QString currency;

        ServersStruct(const QString &currency)
            : currency(currency)
        {}
    };

public:

    using RegisterAddressCallback = std::function<void(const TypedException &exception)>;

    using GetTxsCallback = std::function<void(const std::vector<Transaction> &txs, const TypedException &exception)>;

    using CalcBalanceCallback = std::function<void(const BalanceInfo &txs, const TypedException &exception)>;

    using SetCurrentGroupCallback = std::function<void(const TypedException &exception)>;

    using GetAddressesCallback = std::function<void(const std::vector<AddressInfo> &result, const TypedException &exception)>;

    using GetTxCallback = std::function<void(const Transaction &txs, const TypedException &exception)>;

    using GetLastUpdateCallback = std::function<void(const system_time_point &lastUpdate, const system_time_point &now)>;

    using GetNonceCallback = std::function<void(size_t nonce, const QString &server, const TypedException &exception)>;

    using Callback = std::function<void()>;

public:

    explicit Transactions(NsLookup &nsLookup, TransactionsJavascript &javascriptWrapper, TransactionsDBStorage &db, QObject *parent = nullptr);

    void setJavascriptWrapper(JavascriptWrapper &wrapper) {
        javascriptWrapperCannonical = &wrapper;
    }

signals:

    void registerAddresses(const std::vector<AddressInfo> &addresses, const RegisterAddressCallback &callback);

    void getAddresses(const QString &group, const GetAddressesCallback &callback);

    void setCurrentGroup(const QString &group, const SetCurrentGroupCallback &callback);

    void getTxs(const QString &address, const QString &currency, const QString &fromTx, int count, bool asc, const GetTxsCallback &callback);

    void getTxs2(const QString &address, const QString &currency, int from, int count, bool asc, const GetTxsCallback &callback);

    void getTxsAll(const QString &currency, const QString &fromTx, int count, bool asc, const GetTxsCallback &callback);

    void getTxsAll2(const QString &currency, int from, int count, bool asc, const GetTxsCallback &callback);

    void calcBalance(const QString &address, const QString &currency, const CalcBalanceCallback &callback);

    void getNonce(const QString &requestId, const QString &from, const SendParameters &sendParams, const GetNonceCallback &callback);

    void sendTransaction(const QString &requestId, const QString &to, const QString &value, size_t nonce, const QString &data, const QString &fee, const QString &pubkey, const QString &sign, const SendParameters &sendParams);

    void getTxFromServer(const QString &txHash, const QString &type, const GetTxCallback &callback);

    void getLastUpdateBalance(const QString &currency, const GetLastUpdateCallback &callback);

public slots:

    void onRegisterAddresses(const std::vector<AddressInfo> &addresses, const RegisterAddressCallback &callback);

    void onGetAddresses(const QString &group, const GetAddressesCallback &callback);

    void onSetCurrentGroup(const QString &group, const SetCurrentGroupCallback &callback);

    void onGetTxs(const QString &address, const QString &currency, const QString &fromTx, int count, bool asc, const GetTxsCallback &callback);

    void onGetTxs2(const QString &address, const QString &currency, int from, int count, bool asc, const GetTxsCallback &callback);

    void onGetTxsAll(const QString &currency, const QString &fromTx, int count, bool asc, const GetTxsCallback &callback);

    void onGetTxsAll2(const QString &currency, int from, int count, bool asc, const GetTxsCallback &callback);

    void onCalcBalance(const QString &address, const QString &currency, const CalcBalanceCallback &callback);

    void onGetNonce(const QString &requestId, const QString &from, const SendParameters &sendParams, const GetNonceCallback &callback);

    void onSendTransaction(const QString &requestId, const QString &to, const QString &value, size_t nonce, const QString &data, const QString &fee, const QString &pubkey, const QString &sign, const SendParameters &sendParams);

    void onGetTxFromServer(const QString &txHash, const QString &type, const GetTxCallback &callback);

    void onGetLastUpdateBalance(const QString &currency, const GetLastUpdateCallback &callback);

private slots:

    void onCallbackCall(Callback callback);

    void onRun();

    void onTimerEvent();

    void onSendTxEvent();

private:

    void processAddressMth(const QString &address, const QString &currency, const std::vector<QString> &servers, const std::shared_ptr<ServersStruct> &servStruct);

    void newBalance(const QString &address, const QString &currency, const BalanceInfo &balance, const std::vector<Transaction> &txs, const std::shared_ptr<ServersStruct> &servStruct);

    void updateBalanceTime(const QString &currency, const std::shared_ptr<ServersStruct> &servStruct);

    template<typename Func>
    void runCallback(const Func &callback);

    template<typename Func>
    void runCallbackJsWrap(const Func &callback);

    std::vector<AddressInfo> getAddressesInfos(const QString &group);

    BalanceInfo getBalance(const QString &address, const QString &currency);

    void addToSendTxWatcher(const TransactionHash &hash, size_t countServers, const QString &group, const seconds &timeout);

    void sendErrorGetTx(const TransactionHash &hash, const QString &server);

private:

    NsLookup &nsLookup;

    TransactionsJavascript &javascriptWrapper;

    JavascriptWrapper *javascriptWrapperCannonical = nullptr;

    TransactionsDBStorage &db;

    SimpleClient client;

    HttpSimpleClient tcpClient;

    QString currentGroup;

    QTimer timerSendTx;

    std::map<TransactionHash, SendedTransactionWatcher> sendTxWathcers;

    std::map<QString, system_time_point> lastSuccessUpdateTimestamps;
};

}

#endif // TRANSACTIONS_H
