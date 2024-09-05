#ifndef YVRMANAGER_H
#define YVRMANAGER_H

#include <QObject>
#include <QVariant>
#include <QJsonArray>
#include <QMutex>
#include <QTime>

QT_FORWARD_DECLARE_CLASS(VRMonitor)
QT_FORWARD_DECLARE_CLASS(Settings)
QT_FORWARD_DECLARE_CLASS(ConnectManager)
QT_FORWARD_DECLARE_CLASS(ResourceListModel)
QT_FORWARD_DECLARE_CLASS(ResourceAddListModel)

class YVRManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString fileSavePath READ fileSavePath NOTIFY fileSavePathChange)
    Q_PROPERTY(bool control READ getControl WRITE setControl NOTIFY controlChange)

    Q_PROPERTY(QVariant resourcesListModel READ resourcesList NOTIFY resourcesListChange)
    Q_PROPERTY(QVariant resourcesListModelAdd READ resourcesListAdd NOTIFY resourcesListAddChange)
    Q_PROPERTY(QVariant resourcesListModelAuto READ resourcesListAuto NOTIFY resourcesListAutoChange)
    Q_PROPERTY(QVariant vrListModel READ vrList NOTIFY vrListChange)
    Q_PROPERTY(QVariant vrDicoverListModel READ vrDiscoverList NOTIFY vrListChange)

public:
    enum  PageType{
        HomePage = 0,
        PlayControl = 1,
        AddPlayControl = 2,
        Autonomous = 3,
        DeploymentPlan = 4,
        PlanManager,
        DeviceManager,
        ProgramSettings,
    };

    Q_ENUM(PageType)
public:

    explicit YVRManager(QObject *parent = nullptr);
    ~YVRManager();

    Q_INVOKABLE void mainWinsizeChange();
    Q_INVOKABLE void deploymentPlan();
    Q_INVOKABLE QString getVersion();

    Q_INVOKABLE void setRefreshState(bool refresh);

    Q_INVOKABLE void rebootN(bool selectAll);
    Q_INVOKABLE void rebootP(bool selectAll);
    Q_INVOKABLE void rebootN(QString sn);
    Q_INVOKABLE void rebootP(QString sn);

    Q_INVOKABLE void addDiscoverDeice(bool selectAll);
    Q_INVOKABLE void loadDevicesExcel(QString path);
    Q_INVOKABLE void showDeviceExampleExcel();
    Q_INVOKABLE bool verfityId(QString sn, QString id);

    Q_INVOKABLE void pause( int index);
    Q_INVOKABLE void play(int index, bool loopPlay, bool selectAll);
    Q_INVOKABLE void clearPlan();


    Q_INVOKABLE bool firstPlayControl() const;
    Q_INVOKABLE bool firstAddDevice() const;

    QVariant resourcesList();
    QVariant resourcesListAdd();
    QVariant resourcesListAuto();
    QVariant vrList() const;
    QVariant vrDiscoverList() const;
    QString fileSavePath() const;

    bool getControl() const;
    void setControl(bool is);
    Q_INVOKABLE void gotoFolder(QString path);

signals:
    void showToast(QString toast);
    void changeMainwinSize();
    void resourcesListChange();
    void resourcesListAddChange();
    void resourcesListAutoChange();
    void vrListChange();
    void vrListAddChange();
    void selectAllChange();
    void fileSavePathChange();
    void controlChange();

private:
    void init();
    void timerEvent(QTimerEvent *event) override;

private:

    VRMonitor* m_monitor = nullptr;
    ResourceListModel* m_resourcesList = nullptr;
    ResourceAddListModel* m_resourcesListAdd = nullptr;
    ResourceAddListModel* m_resourcesListAuto = nullptr;
    ConnectManager* m_UDPBroadCast = nullptr;
    bool m_needSetMainWinSize = false;

    bool m_realTimeFresh = false;
    PageType m_pageType = HomePage;
    QString m_fileSavePath;
};

#endif // YVRMANAGER_H
