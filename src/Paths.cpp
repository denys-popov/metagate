#include "Paths.h"

#include <QStandardPaths>
#include <QApplication>
#include <QSettings>

#include <mutex>

#include "utils.h"
#include "check.h"
#include "Log.h"

constexpr auto *metahashWalletPagesPathEnv = "METAHASH_WALLET_PAGES_PATH";

const static QString WALLET_PATH_DEFAULT = ".metahash_wallets/";

const static QString WALLET_COMMON_PATH = ".metagate/";

const static QString LOG_PATH = "logs/";

const static QString PAGES_PATH = "pages/";

const static QString SETTINGS_NAME = "settings.ini";

const static QString DB_PATH = "database/";

const static QString AUTOUPDATER_PATH = "autoupdater/";

const static QString NS_LOOKUP_PATH = "./";

QString getWalletPath() {
    const QString res = makePath(QStandardPaths::writableLocation(QStandardPaths::HomeLocation), WALLET_PATH_DEFAULT);
    createFolder(res);
    return res;
}

QString getLogPath() {
    const QString res = makePath(QStandardPaths::writableLocation(QStandardPaths::HomeLocation), WALLET_COMMON_PATH, LOG_PATH);
    createFolder(res);
    return res;
}

QString getDbPath() {
    removeFolder(makePath(QStandardPaths::writableLocation(QStandardPaths::HomeLocation), WALLET_COMMON_PATH, "bd"));
    const QString res = makePath(QStandardPaths::writableLocation(QStandardPaths::HomeLocation), WALLET_COMMON_PATH, DB_PATH);
    createFolder(res);
    return res;
}

QString getNsLookupPath() {
    const QString res = makePath(QStandardPaths::writableLocation(QStandardPaths::HomeLocation), WALLET_COMMON_PATH, NS_LOOKUP_PATH);
    createFolder(res);
    return res;
}

static QString getOldPagesPath() {
    const auto path = qgetenv(metahashWalletPagesPathEnv);
    if (!path.isEmpty())
        return QString(path);

    const QString path1(makePath(QApplication::applicationDirPath(), "startSettings/"));
    const QString path2(makePath(QApplication::applicationDirPath(), "../WalletMetahash/startSettings/"));
    const QString path3(makePath(QApplication::applicationDirPath(), "../../WalletMetahash/startSettings/"));
    QString currentBeginPath;
    QDir dirTmp;
    if (dirTmp.exists(path1)) {
        currentBeginPath = path1;
    } else if (dirTmp.exists(path2)){
        currentBeginPath = path2;
    } else {
        currentBeginPath = path3;
    }

    return currentBeginPath;
}

QString getPagesPath() {
    static std::mutex mut;

    const QString oldPagesPath = getOldPagesPath();

    const QString newPagesPath = makePath(QStandardPaths::writableLocation(QStandardPaths::HomeLocation), WALLET_COMMON_PATH, PAGES_PATH);

    std::lock_guard<std::mutex> lock(mut);
    if (!isExistFolder(newPagesPath)) {
        LOG << "Create pages folder: " << newPagesPath << " " << oldPagesPath;
        CHECK(copyRecursively(oldPagesPath, newPagesPath, true, false), "not copy pages");
    }

    return newPagesPath;
}

QString getSettingsPath() {
    static std::mutex mut;

    const QString oldPagesPath = getOldPagesPath();
    const QString oldSettingsPath = makePath(oldPagesPath, SETTINGS_NAME);

    const QString res = makePath(QStandardPaths::writableLocation(QStandardPaths::HomeLocation), WALLET_COMMON_PATH);
    createFolder(res);
    const QString settings = makePath(res, SETTINGS_NAME);

    const QString pagesPath = getPagesPath();

    const auto replaceSettings = [&] {
        copyFile(oldSettingsPath, settings, true);
        removeFile(makePath(pagesPath, "nodes.txt"));
        removeFile(makePath(pagesPath, "servers.txt"));
        removeFile(makePath(pagesPath, "web_socket.txt"));
        removeFile(makePath(pagesPath, "version.txt"));
    };

    std::lock_guard<std::mutex> lock(mut);
    if (!isExistFile(settings)) {
        LOG << "Create settings: " << oldSettingsPath << " " << settings;
        replaceSettings();
    }

    const auto compareVersion = [&]() {// Чтобы в деструкторе закрылось
        const QSettings settingsFile(settings, QSettings::IniFormat);
        const QSettings oldSettingsFile(oldSettingsPath, QSettings::IniFormat);
        return settingsFile.value("version") == oldSettingsFile.value("version");
    };
    if (!compareVersion()) {
        LOG << "Replace settings: " << oldSettingsPath << " " << settings;
        replaceSettings();
    }

    return settings;
}

QString getAutoupdaterPath() {
    const QString path1(makePath(QStandardPaths::writableLocation(QStandardPaths::HomeLocation), WALLET_COMMON_PATH, AUTOUPDATER_PATH));
    QDir dirTmp(path1);
    if (!dirTmp.exists()) {
        CHECK(dirTmp.mkpath(path1), "dont create autoupdater path");
    }
    return path1;
}

QString getTmpAutoupdaterPath() {
    return makePath(getAutoupdaterPath(), "folder/");
}

void clearAutoupdatersPath() {
    auto remove = [](const QString &dirPath) {
        QDir dir(dirPath);
        if (dir.exists()) {
            CHECK(dir.removeRecursively(), "dont remove autoupdater path");
            CHECK(dir.mkpath(dirPath), "dont create autoupdater path");
        }
    };

    remove(getAutoupdaterPath());
    remove(getTmpAutoupdaterPath());
}
