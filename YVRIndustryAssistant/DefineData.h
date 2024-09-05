#ifndef DEFINEDATA_H
#define DEFINEDATA_H
#include <QString>
#include <QStringList>

enum YVRResourceType{
    Image2D = 0,
    ImageCubeMap = 1,
    ImageVR360 = 2,
    ImageVR180,
    Video2DNormal,
    Video2D180,
    Video2D360,
    Video3DTBNormal,
    Video3DTB180,
    Video3DTB360,
    Video3DLRNormal,
    Video3DLR180,
    Video3DLR360,
    UndeifneFile = 99
};

struct FileInfo
{
  QString path;
  QString file;
  QString poster;

  QString realFile;
  QString realPoster;

  QString showName;
  int index = -1;
  QString describe;
  YVRResourceType type;
  qint64 size;
  QString md5;
  QStringList groups = {"默认"};
};

struct ShowFileInfo
{
    int id;
    QString poster;
    QString showName;
    QString realFile;
    YVRResourceType type;
    QString prePlayTime = "";
};

struct YVRPlayDevice{
    QString sn;     // 设备SN
    QString id;     // 设备号
    QString status;
    bool canSelect;
    bool hasSelect = false; // 是否连接
};

enum YVRState{
    Offline = 0,   // 离线
    Unnse,    // 未佩戴
    Onuse,     // 已佩戴
    Onplay,     // 播放
    Pause ,      // 暂停
    PlayError,  // 播放异常
    Control,      // 受控中
    Uncontrol,    // 未受控
    HeadSetPower,       // 电量
    LeftController,     // 左电量
    RightController,    // 右电量
    StateUndefine = 99  // 未定义
};

enum DeviceType{
    YVR_D1 = 0,
    YVR_D2,
};





#endif // DEFINEDATA_H
