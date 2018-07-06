#ifndef TST_WALLET_H
#define TST_WALLET_H

#include <QObject>
#include <QTest>


class tst_Wallet : public QObject
{
    Q_OBJECT
public:
    explicit tst_Wallet(QObject *parent = nullptr);

private slots:

    void testEncryptBtc_data();
    void testEncryptBtc();

    void testCreateBinTransaction_data();
    void testCreateBinTransaction();

    void testSsl_data();
    void testSsl();

    void testCreateMth_data();
    void testCreateMth();

    void testCreateEth_data();
    void testCreateEth();
    
    void testCreateBtc_data();
    void testCreateBtc();

    void testEthWallet();

    void testBitcoinTransaction_data();
    void testBitcoinTransaction();
};

#endif // TST_WALLET_H