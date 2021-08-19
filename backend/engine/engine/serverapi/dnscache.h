#ifndef DNSCACHE_H
#define DNSCACHE_H

#include <QMap>
#include <QObject>
#include <QHostInfo>

// used by NetworkManagerCustomDns for custom DNS resolver
class DnsCache : public QObject
{
    Q_OBJECT
public:
    explicit DnsCache(QObject *parent = 0);
    virtual ~DnsCache();

    void resolve(const QString &hostname, void *userData);
    void resolve(const QString &hostname, int cacheTimeout, void *userData);

signals:
    void resolved(bool success, void *userData, const QStringList &ips);
    void ipsInCachChanged(const QStringList &ips);

private slots:
    void onDnsResolverFinished(const QString &hostname, const QHostInfo &hostInfo, void *userPointer);

private:

    struct ResolvedHostInfo
    {
        qint64 time;
        QStringList ips;
    };

    static constexpr int CACHE_TIMEOUT = 60000;        // 1 min
    QMap<QString, ResolvedHostInfo> resolvedHosts_;
    QSet<QString> resolvedIps_;

    struct PendingResolvingHosts
    {
        QString hostname;
        void *userData;
    };

    QSet<QString> resolvingHostsInProgress_;
    QVector<PendingResolvingHosts> pendingHosts_;

    void checkForNewIps(const QHostInfo &hostInfo);
    void checkForNewIps(const QString &ip);
};

#endif // DNSCACHE_H