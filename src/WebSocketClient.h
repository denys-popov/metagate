#ifndef WEBSOCKETCLIENT_H
#define WEBSOCKETCLIENT_H

#include <QObject>
#include <QThread>
#include <QtWebSockets/QWebSocket>

class WebSocketClient : public QObject
{
    Q_OBJECT
public:
    explicit WebSocketClient(const QString &url, QObject *parent = nullptr);

    ~WebSocketClient();

    void start();

signals:

    void closed();

    void sendMessage(QString message);

    void sendMessages(const std::vector<QString> &messages);

    void setHelloString(QString message);

    void setHelloString(const std::vector<QString> &messages);

    void addHelloString(QString message);

signals:

    void messageReceived(QString message);

public slots:

    void onStarted();

    void onConnected();
    void onTextMessageReceived(QString message);

    void onSendMessage(QString message);

    void onSendMessages(const std::vector<QString> &messages);

    void onSetHelloString(QString message);

    void onSetHelloString(const std::vector<QString> &messages);

    void onAddHelloString(QString message);

private slots:

    void socketError(QAbstractSocket::SocketError error);

private:

    void sendMessagesInternal();

private:

    QWebSocket m_webSocket;
    QUrl m_url;

    bool isStopped = false;

    bool isConnected = false;

    std::vector<QString> messageQueue;

    QThread thread1;

    std::vector<QString> helloStrings;
};

#endif // WEBSOCKETCLIENT_H
