#ifndef UTILS_H
#define UTILS_H
#include <QStringList>
#include "windows.h"

bool IsRemovableDrive(const std::wstring& driveLetter);
QStringList terminateProcess(QList<int> pids);
int terminateProcess(QStringList ProcessName);
bool isExistProcess(QString process);
QString getPathByProcessID(DWORD pid);
QString getPathByProcess(QString process);

#endif // UTILS_H
