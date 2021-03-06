#ifndef TRANSACTIONSDBSTORAGE_H
#define TRANSACTIONSDBSTORAGE_H

#include "dbstorage.h"
#include "Transaction.h"
#include "BigNumber.h"
#include <vector>

namespace transactions {

class TransactionsDBStorage : public DBStorage
{
public:
    TransactionsDBStorage(const QString &path = QString());

    virtual void init(bool force = false) override;


    void addPayment(const QString &currency, const QString &txid, const QString &address, bool isInput,
                    const QString &ufrom, const QString &uto, const QString &value,
                    quint64 ts, const QString &data, const QString &fee, qint64 nonce,
                    bool isSetDelegate, bool isDelegate, QString delegateValue);
    void addPayment(const Transaction &trans);

    std::vector<Transaction> getPaymentsForAddress(const QString &address, const QString &currency,
                                              qint64 offset, qint64 count, bool asc) const;
    std::vector<Transaction> getPaymentsForCurrency(const QString &currency,
                                                  qint64 offset, qint64 count, bool asc) const;

    void removePaymentsForDest(const QString &address, const QString &currency);

    qint64 getPaymentsCountForAddress(const QString &address, const QString &currency, bool input);

    BigNumber calcInValueForAddress(const QString &address, const QString &currency);
    BigNumber calcOutValueForAddress(const QString &address, const QString &currency);

    qint64 getIsSetDelegatePaymentsCountForAddress(const QString &address, const QString &currency);
    BigNumber calcIsSetDelegateValueForAddress(const QString &address, const QString &currency, bool isDelegate, bool isInput);

    void addTracked(const QString &currency, const QString &address, const QString &name, const QString &type, const QString &tgroup);
    void addTracked(const AddressInfo &info);

    std::vector<AddressInfo> getTrackedForGroup(const QString &tgroup);

private:
    void createPaymentsList(QSqlQuery &query, std::vector<Transaction> &payments) const;
};

}

#endif // TRANSACTIONSDBSTORAGE_H
