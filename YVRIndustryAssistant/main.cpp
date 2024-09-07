#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QFontDatabase>
#include <QQmlContext>
#include <QSystemSemaphore>
#include <QSharedMemory>
#include <QStandardPaths>
#include <QTextCodec>
#include <QTranslator>
#include "plugins/FramelessQuickWindow.h"
#include "YVRManager.h"

void LoadDefualtFont(const QGuiApplication& app)
{
    QString fontPath = QString(":/res/SourceHanSansCN-Medium.ttf");
    int loadedFontID = QFontDatabase::addApplicationFont(fontPath);
    QStringList loadedFontFamilies = QFontDatabase::applicationFontFamilies(loadedFontID);
    if (!loadedFontFamilies.empty())
    {
        QString sansCNFamily = loadedFontFamilies.at(0);
        QFont defaultFont = app.font();
        defaultFont.setFamily(sansCNFamily);
        defaultFont.setPixelSize(20);
        app.setFont(defaultFont);
    }
}

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    static QMutex mutex;
    mutex.lock();

    QString text;
    switch((int)type)
    {
    case QtDebugMsg:
       text = QString("Debug:");
       break;

    case QtWarningMsg:
       text = QString("Warning:");
       break;

    case QtCriticalMsg:
       text = QString("Critical:");
       break;

    case QtFatalMsg:
       text = QString("Fatal:");
        break;
    case QtInfoMsg:
        text = QString("Info:");
    }

    QString contextInfo = QString("\"%1(%2)\"").arg(QString(context.file)).arg(context.line);
    QString currentTime = QTime::currentTime().toString("hh:mm:ss.zzz");
    QString message = QString("[%1%2  %3]:%4").arg(text).arg(currentTime).arg(contextInfo).arg(msg);
    auto path = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    QFile file(QString(path + "/%1.txt").arg(QDate::currentDate().toString("yyyy-MM-dd")));
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream text_stream(&file);
    text_stream << message << "\r\n";
    file.flush();
    file.close();
    mutex.unlock();
}

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

#ifdef QT_NO_DEBUG
    qInstallMessageHandler(myMessageOutput);
#endif

//    QLocale locale;
//    if(locale.language() == QLocale::Language::English)
//    {
//        QTranslator *translator = new QTranslator(&app);
//        translator->load(QString("chinese.qm"));
//        app.installTranslator(translator);
//    }

    QGuiApplication::setOrganizationName("PFDM");

    LoadDefualtFont(app);
    app.setWindowIcon(QIcon(":/res/image/yvr.ico"));
    QQmlApplicationEngine engine;
    qmlRegisterType<FramelessQuickWindow>("QtShark.Window", 1, 0, "FramelessWindow");

    QString strKey = "PFDMIndustryAssistant";
    LPCWSTR wstrKey = strKey.toStdWString().c_str();
    HANDLE hMetex = CreateMutex(NULL,FALSE,wstrKey);
    if(GetLastError() == ERROR_ALREADY_EXISTS)
    {
    //如果互斥体存在，说明程序已经有实例在运行了,释放资源然后关闭本实例
        if(hMetex){
        CloseHandle(hMetex);
        hMetex = NULL;
        }
        const QUrl url(QStringLiteral("qrc:/qml/windows/SingleAppTip.qml"));
        QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                         &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QGuiApplication::exit(-1);
        }, Qt::QueuedConnection);
        engine.load(url);

        return app.exec();
    }

    qmlRegisterType<YVRManager>("YVRManager", 1, 0, "YVRManagerCLS");

    YVRManager* yvr = new YVRManager(&app);
    engine.rootContext()->setContextProperty("yvr", yvr);

    const QUrl url(QStringLiteral("qrc:/qml/mainWindow.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QGuiApplication::exit(-1);
    }, Qt::QueuedConnection);

    engine.load(url);



    return app.exec();
}
