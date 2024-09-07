'''同步、修改、编译、拷贝 qml 项目'''

import os
import shutil
import sys
import re


config = {
    # 选项开关 #######################
    'yvr_version': '1.0.0',

    # 打包前是否需要编译
    #'build' : True,

    # 是否需要重新编译
    #'rebuild': False,


    # QML 界面目录
    'gui_path': './',

    # 打包目录和生成目录 #################
    # 安装文件输出目录
    'output_dir': './../bin/Release',

    # 依赖的外部程序路径 ###################
    # git可执行文件所在目录
    'git_bin': 'C:\\Program Files\\Git\\bin',

    # NSIS打包程序makensis.exe所在目录
    'nsis_bin': 'D:\\NSIS',

    # VS2019 devenv.exe 所在目录
    'devenv_bin': 'C:\\Program Files (x86)\\Microsoft Visual Studio\\2017\\Community\\Common7\\IDE',

    # qt 所在目录
    'qmake_bin': 'C:\\Qt\\Qt5.14.2\\5.14.2\\msvc2017_64\\bin\\qmake.exe',
    
    # qt 所在目录
    'dep_bin': 'C:\\Qt\\Qt5.14.2\\5.14.2\\msvc2017_64\\bin\\windeployqt.exe',
    
    # Qt 编译工具 所在目录
    'qml_bin': 'C:\\Qt\\Qt5.14.2\\5.14.2\\msvc2017_64\\qml',

    # Qt 编译工具 所在目录
    'jom_bin': 'C:\\Qt\\Qt5.14.2\\Tools\\QtCreator\\bin\\jom.exe'
}

def SetupEnvironment():
   os.environ['PATH'] = \
        config.get('git_bin') + ';' + \
        config.get('nsis_bin') + ';' + \
        config.get('devenv_bin') + ';' + \
        config.get('qmake_bin') + ';' + \
        os.environ['PATH']

def VersionInit(version,branch,buildNumber):
    buildVersion = ''
    with open("version.h", "r", encoding="utf8") as file, open("version.h", "r+", encoding="utf8") as newFile:
        for line in file.readlines():
            prdVersion = re.findall('\d.\d.\d.\d+', line)
            devVersion = re.findall('Dev_\d.\d.\d_\d+', line)
            if "".join(prdVersion) != "" and branch in ("release","rtc"):
                buildVersion = version + '.' + buildNumber
                line = line.replace("".join(prdVersion), buildVersion)
            if "".join(devVersion) != "" and branch in ("master","dev"):
                if "".join(devVersion).find(version) > -1:
                    suffixNum = int("".join(devVersion)[-3:])+1
                    suffix = ''
                    if suffixNum > 0 and suffixNum < 10:
                        suffix = '00' + str(suffixNum)
                    if suffixNum >= 10 and suffixNum < 100:
                        suffix = '0' + str(suffixNum)
                    buildVersion = 'Dev_' + version + '_' + suffix
                    # line = line.replace("".join(devVersion), 'Dev_'+version+'_'+suffix)
                else:
                    buildVersion = 'Dev_' + version + '_001'
                line = line.replace("".join(devVersion), buildVersion)
            newFile.write(line)
    newFile.close()
    file.close()
    # 将版本号写入version.txt
    with open("version.txt", "w", encoding="utf8") as versionFile:
        versionFile.write(buildVersion)
    versionFile.close()

def Pull(repo_dir, checkout=False, origin='origin', branch='master'):
    original_dir = os.getcwd()
    os.chdir(repo_dir)

    if (checkout):
        os.system('git checkout .')
    os.system('git pull ' + origin + ' ' + branch)

    os.chdir(original_dir)


def UpdateCode():
    print("更新代码...")
    Pull(config.get('gui_path'))
    print("项目代码更新完成！")


def BuildGui():
    proj = config.get('gui_path') + '/YVRIndustryAssistant.pro'

    print('开始 qml 编译...')
    out_exe = config.get('output_dir') + '/PFDMIndustryAssistant.exe'
    if os.path.exists(out_exe):
        os.remove(out_exe)

    print('正在生成项目配置...')
    os.system(config.get('qmake_bin') + ' ' + proj)
    print('项目配置生成完成，开始编译...')
    os.system(config.get('jom_bin') + ' /f ' + config.get('gui_path')+'\\Makefile.Release')
    if not os.path.exists(out_exe):
        print('PFDMIndustryAssistant 编译失败！')
        exit(1)
    os.system(config.get('dep_bin') + ' ' + out_exe + ' -qmldir ' + config.get('qml_bin'))

def MovePluginsFiles():
    print('正在移动文件...')
    
    dstdir = config.get('output_dir') + "/plugins/"
    if os.path.exists(dstdir):
        shutil.rmtree(dstdir)
    else:
        os.makedirs(dstdir)
        
    shutil.move(config.get('output_dir') + '/bearer', dstdir)
    shutil.move(config.get('output_dir') + '/iconengines', dstdir)
    shutil.move(config.get('output_dir') + '/platforms', dstdir)
    shutil.move(config.get('output_dir') + '/playlistformats', dstdir)
    shutil.move(config.get('output_dir') + '/qmltooling', dstdir)
    shutil.move(config.get('output_dir') + '/scenegraph', dstdir)
    shutil.move(config.get('output_dir') + '/styles', dstdir)
    shutil.move(config.get('output_dir') + '/translations', dstdir)

    print('文件移动完成')
    
def ComposeNSISCmdline(nsi_file, version):
    print('packing...')
    return config.get('nsis_bin')+'/makensis ' + \
        '/DPRODUCT_VERSION="' + version + '" ' +\
        '/DPRODUCT_OUT_FILE_NAME="PFDMIndustryAssistant_setup_' + version + '.exe" ' +\
        nsi_file


def DoWork():
    #SetupEnvironment()
    #UpdateCode()
    BuildGui()
    MovePluginsFiles()
    version = ''
    with open("version.txt", "r", encoding="utf8") as file:
        version = file.readlines()[0]
    file.close()
    os.system(ComposeNSISCmdline(config.get('output_dir')+'/yvr-installer.nsi', version))

if __name__ == '__main__':
    #version = config.get('yvr_version')
    version = sys.argv[1]
    branch = sys.argv[2]
    buildNumber = sys.argv[3]
    VersionInit(version, branch, buildNumber)
    DoWork()
