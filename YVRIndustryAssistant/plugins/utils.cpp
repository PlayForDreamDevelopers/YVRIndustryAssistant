#include "utils.h"
#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include "tlhelp32.h"
#include <QDebug>
#include <QProcess>
#include <Psapi.h>
#include <QFile>

using namespace std;
#pragma comment (lib, "advapi32.lib")

bool IsRemovableDrive(const std::wstring& driveLetter) {
    // Get the drive type
    UINT driveType = GetDriveType(driveLetter.c_str());

    return driveType == DRIVE_REMOVABLE;
}

int terminateProcess(QStringList ProcessName)
 {
    int result = 0;
     QString processName;

     HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0) ;
     PROCESSENTRY32 pInfo;
     pInfo.dwSize = sizeof(pInfo);
     Process32First(hSnapShot, &pInfo);
     do
     {
         //遍历任务的所有进程
         processName = (QString::fromUtf16(reinterpret_cast<const unsigned short *>(pInfo.szExeFile)));

         for(auto & procName : ProcessName)
         {

             if (processName == procName)
             {
                 //找到该进程 取出pid
                 int nPID = pInfo.th32ProcessID;
                 QString cmd = QString("taskkill /PID %1 /F").arg(nPID);
                 QProcess p(0);
                 p.start(cmd);
                 p.waitForStarted();
                 p.waitForFinished();
                 result ++;
             }
     }
     } while(Process32Next(hSnapShot, &pInfo));
     CloseHandle(hSnapShot);

     return result;
}

QStringList terminateProcess(QList<int> pids)
 {
     QStringList list;
     QString processName;

     HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0) ;
     PROCESSENTRY32 pInfo;
     pInfo.dwSize = sizeof(pInfo);
     Process32First(hSnapShot, &pInfo);
     do
     {
         //遍历任务的所有进程
         processName = (QString::fromUtf16(reinterpret_cast<const unsigned short *>(pInfo.szExeFile)));

         int pid = pInfo.th32ProcessID;
         for(auto & procName : pids)
         {
             if (pid == procName)
             {
                list.append(processName);
             }
     }
     } while(Process32Next(hSnapShot, &pInfo));
     CloseHandle(hSnapShot);

     return list;
}

bool isExistProcess(QString process)
{
    PROCESSENTRY32 processEntry32;
    HANDLE toolHelp32Snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (((int)toolHelp32Snapshot) != -1)
    {
        processEntry32.dwSize = sizeof(processEntry32);
        if (Process32First(toolHelp32Snapshot, &processEntry32))
        {
            do
            {
                int iLen = 2 * wcslen(processEntry32.szExeFile);
                char* chRtn = new char[iLen + 1];
                //转换成功返回为非负值
                wcstombs(chRtn, processEntry32.szExeFile, iLen + 1);
                if (strcmp(process.toUtf8(), chRtn) == 0)
                {
//                   auto path = GetPathByProcessID(processEntry32.th32ProcessID);
//                   if(QFile::exists(path.replace("steam.exe", "steamapps\\common\\SteamVR\\bin")))
//                   {
//                       m_isIntallSteamVR = true;
//                   }else {
//                       m_isIntallSteamVR = false;
//                   }

                   return true;
                }
            } while (Process32Next(toolHelp32Snapshot, &processEntry32));
        }
        CloseHandle(toolHelp32Snapshot);
    }
    return false;
}

QString getPathByProcessID(DWORD pid)
{
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (!hProcess)
    {
        //QMessageBox::warning(NULL,"GetPathByProcessID","无权访问该进程");
        return "";
    }
    WCHAR filePath[MAX_PATH];
    DWORD ret= GetModuleFileNameEx(hProcess, NULL, filePath, MAX_PATH) ;
    QString file = QString::fromStdWString( filePath );
    //QMessageBox::warning(NULL,"GetPathByProcessID ret=", QString::number(ret)+":"+file);
    CloseHandle(hProcess);
    return ret==0?"":file;
}

QString getPathByProcess(QString process)
{
    PROCESSENTRY32 processEntry32;
    HANDLE toolHelp32Snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (((int)toolHelp32Snapshot) != -1)
    {
        processEntry32.dwSize = sizeof(processEntry32);
        if (Process32First(toolHelp32Snapshot, &processEntry32))
        {
            do
            {
                int iLen = 2 * wcslen(processEntry32.szExeFile);
                char* chRtn = new char[iLen + 1];
                //转换成功返回为非负值
                wcstombs(chRtn, processEntry32.szExeFile, iLen + 1);
                if (strcmp(process.toUtf8(), chRtn) == 0)
                {
                  auto path = getPathByProcessID(processEntry32.th32ProcessID);
//                   if(QFile::exists(path.replace("adb.exe", "steamapps\\common\\SteamVR\\bin")))
//                   {

//                   }
                   return path;
                }
            } while (Process32Next(toolHelp32Snapshot, &processEntry32));
        }
        CloseHandle(toolHelp32Snapshot);
    }
    return "";
}


wstring Str2Wstr(string str)
{
    unsigned len = str.size() * 2;// 预留字节数
    setlocale(LC_CTYPE, "");     //必须调用此函数
    wchar_t *p = new wchar_t[len];// 申请一段内存存放转换后的字符串
    mbstowcs(p, str.c_str(), len);// 转换
    std::wstring str1(p);
    delete[] p;// 释放申请的内存
    return str1;
}
