'''同步、修改、编译、拷贝 qml 项目'''

import os
import shutil


config = {
    # 选项开关 #######################
    'yvr_version': '1.1.1',

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
    'nsis_bin': 'C:\\NSIS',

    # VS2019 devenv.exe 所在目录
    'devenv_bin': 'C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\Common7\\IDE',

    # qt 所在目录
    'qmake_bin': 'C:\\Qt\\Qt5.14.2\\5.14.2\\msvc2017_64\\bin\\qmake.exe',
    
    # qt 所在目录
    'dep_bin': 'C:\\Qt\\Qt5.14.2\\5.14.2\\msvc2017_64\\bin\\windeployqt.exe',

    # Qt 编译工具 所在目录
    'jom_bin': 'C:\\Qt\\Qt5.14.2\\Tools\\QtCreator\\bin\\jom.exe',
    
    # Qt 编译工具 所在目录
    'qml_bin': 'C:\\Qt\\Qt5.14.2\\5.14.2\\msvc2017_64\\qml'
}

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
    print(config.get('jom_bin') + ' /f ' + config.get('gui_path')+'\\Makefile.Release')
    os.system(config.get('jom_bin') + ' /f ' + config.get('gui_path')+'\\Makefile.Release')
    if not os.path.exists(out_exe):
        print('PFDMIndustryAssistant 编译失败！')
        exit(1)

    os.system(config.get('dep_bin') + ' ' + out_exe + ' -qmldir ' + config.get('qml_bin'))

def MovePluginsFiles():
    print('正在移动文件...')
    dstdir = config.get('output_dir') + "/plugins/"
    if not os.path.exists(dstdir):
        os.makedirs(dstdir)
    else:
        shutil.rmtree(dstdir)
        
    shutil.move(config.get('output_dir') + '/bearer', dstdir)
    shutil.move(config.get('output_dir') + '/iconengines', dstdir)
    shutil.move(config.get('output_dir') + '/platforminputcontexts', dstdir)
    shutil.move(config.get('output_dir') + '/platforms', dstdir)
    shutil.move(config.get('output_dir') + '/playlistformats', dstdir)
    shutil.move(config.get('output_dir') + '/qmltooling', dstdir)
    shutil.move(config.get('output_dir') + '/scenegraph', dstdir)
    shutil.move(config.get('output_dir') + '/styles', dstdir)
    shutil.move(config.get('output_dir') + '/translations', dstdir)
    shutil.move(config.get('output_dir') + '/virtualkeyboard', dstdir)
    print('文件移动完成')

    
def ComposeNSISCmdline(nsi_file, version):
    print('packing...')
    return config.get('nsis_bin')+'/makensis ' + \
        '/DPRODUCT_VERSION="' + version + '" ' +\
        '/DPRODUCT_OUT_FILE_NAME="PFDMIndustryAssistant_setup_' + version + '.exe" ' +\
        nsi_file


def DoWork(version):
    #UpdateCode()
    BuildGui()
    MovePluginsFiles()
    os.system(ComposeNSISCmdline(config.get('output_dir')+'/yvr-installer.nsi', version))

if __name__ == '__main__':
    version = config.get('yvr_version')
    DoWork(version)
