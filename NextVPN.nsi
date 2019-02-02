############################################################################################
#      NSIS Installation Script created by NSIS Quick Setup Script Generator v1.09.18
#               Entirely Edited with NullSoft Scriptable Installation System                
#              by Vlasis K. Barkas aka Red Wine red_wine@freemail.gr Sep 2006               
############################################################################################

!define APP_NAME "NextVPN"
!define COMP_NAME "VPN Makers"
!define VERSION "2.70.00.00"
!define COPYRIGHT "VPNMakers  © 2017"
!define DESCRIPTION "NextVPN"
!define INSTALLER_NAME "C:\Users\Oplus\Desktop\setup.exe"
!define MAIN_APP_EXE "NextVPN.exe"
!define INSTALL_TYPE "SetShellVarContext all"
!define REG_ROOT "HKLM"
!define REG_APP_PATH "Software\Microsoft\Windows\CurrentVersion\App Paths\${MAIN_APP_EXE}"
!define UNINSTALL_PATH "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}"

######################################################################

VIProductVersion  "${VERSION}"
VIAddVersionKey "ProductName"  "${APP_NAME}"
VIAddVersionKey "CompanyName"  "${COMP_NAME}"
VIAddVersionKey "LegalCopyright"  "${COPYRIGHT}"
VIAddVersionKey "FileDescription"  "${DESCRIPTION}"
VIAddVersionKey "FileVersion"  "${VERSION}"

######################################################################

SetCompressor /SOLID Lzma
Name "${APP_NAME}"
Caption "${APP_NAME}"
OutFile "${INSTALLER_NAME}"
BrandingText "${APP_NAME}"
XPStyle on
InstallDirRegKey "${REG_ROOT}" "${REG_APP_PATH}" ""
InstallDir "$LOCALAPPDATA\NextVPN"

######################################################################

!include "MUI2.nsh"
 
!define MUI_ICON "C:\Users\Oplus\Documents\QProjects\NextVPN\SetupIcon.ico"
!define MUI_ABORTWARNING
!define MUI_UNABORTWARNING


!insertmacro MUI_PAGE_WELCOME

!ifdef LICENSE_TXT
!insertmacro MUI_PAGE_LICENSE "${LICENSE_TXT}"
!endif

!ifdef REG_START_MENU
!define MUI_STARTMENUPAGE_NODISABLE
!define MUI_STARTMENUPAGE_DEFAULTFOLDER "NextVPN"
!define MUI_STARTMENUPAGE_REGISTRY_ROOT "${REG_ROOT}"
!define MUI_STARTMENUPAGE_REGISTRY_KEY "${UNINSTALL_PATH}"
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "${REG_START_MENU}"
!insertmacro MUI_PAGE_STARTMENU Application $SM_Folder
!endif

!insertmacro MUI_PAGE_INSTFILES

!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_CONFIRM

!insertmacro MUI_UNPAGE_INSTFILES

!insertmacro MUI_UNPAGE_FINISH

!insertmacro MUI_LANGUAGE "English"

######################################################################

Section -MainProgram
${INSTALL_TYPE}
SetOverwrite ifnewer
SetOutPath "$INSTDIR"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\cert.crt"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\D3Dcompiler_47.dll"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\key.key"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\libEGL.dll"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\libgcc_s_dw2-1.dll"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\libGLESV2.dll"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\libstdc++-6.dll"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\libwinpthread-1.dll"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\NextVPN.exe"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\opengl32sw.dll"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\pptp.pbk"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\Qt5Core.dll"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\Qt5Gui.dll"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\Qt5Network.dll"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\Qt5Svg.dll"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\Qt5Widgets.dll"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\Qt5Xml.dll"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\servers"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\sstp.pbk"
SetOutPath "$INSTDIR\TunnelPlus"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\TunnelPlus\zebedee.exe"
SetOutPath "$INSTDIR\translations"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\translations\qt_ca.qm"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\translations\qt_cs.qm"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\translations\qt_de.qm"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\translations\qt_en.qm"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\translations\qt_fi.qm"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\translations\qt_fr.qm"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\translations\qt_he.qm"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\translations\qt_hu.qm"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\translations\qt_it.qm"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\translations\qt_ja.qm"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\translations\qt_ko.qm"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\translations\qt_lv.qm"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\translations\qt_pl.qm"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\translations\qt_ru.qm"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\translations\qt_sk.qm"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\translations\qt_uk.qm"
SetOutPath "$INSTDIR\STunnel"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\STunnel\4758cca.dll"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\STunnel\4758cca.dll.manifest"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\STunnel\aep.dll"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\STunnel\aep.dll.manifest"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\STunnel\atalla.dll"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\STunnel\atalla.dll.manifest"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\STunnel\capi.dll"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\STunnel\capi.dll.manifest"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\STunnel\chil.dll"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\STunnel\chil.dll.manifest"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\STunnel\cswift.dll"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\STunnel\cswift.dll.manifest"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\STunnel\customdns.conf.txt"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\STunnel\gmp.dll"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\STunnel\gmp.dll.manifest"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\STunnel\gost.dll"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\STunnel\gost.dll.manifest"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\STunnel\libeay32.dll"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\STunnel\libeay32.dll.manifest"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\STunnel\Microsoft.VC90.CRT.Manifest"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\STunnel\msvcr90.dll"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\STunnel\nuron.dll"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\STunnel\nuron.dll.manifest"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\STunnel\openssl.exe"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\STunnel\openssl.exe.manifest"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\STunnel\padlock.dll"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\STunnel\padlock.dll.manifest"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\STunnel\peer-Filter&amp;cl.pem"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\STunnel\peer-Filter&cl.pem"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\STunnel\ssleay32.dll"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\STunnel\ssleay32.dll.manifest"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\STunnel\stunnel.cnf"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\STunnel\stunnel.conf"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\STunnel\stunnel.exe"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\STunnel\stunnel.exe.manifest"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\STunnel\stunnel.pem"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\STunnel\sureware.dll"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\STunnel\sureware.dll.manifest"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\STunnel\tstunnel.exe"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\STunnel\tstunnel.exe.manifest"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\STunnel\ubsec.dll"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\STunnel\ubsec.dll.manifest"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\STunnel\zlib1.dll"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\STunnel\zlib1.dll.manifest"
SetOutPath "$INSTDIR\Proxifier"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\Proxifier\Helper64.exe"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\Proxifier\Proxifier.exe"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\Proxifier\ProxyChecker.exe"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\Proxifier\PrxDrvPE.dll"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\Proxifier\PrxDrvPE64.dll"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\Proxifier\Settings.ini"
SetOutPath "$INSTDIR\Proxifier\Profiles"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\Proxifier\Profiles\Default.ppx"
SetOutPath "$INSTDIR\platforms"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\platforms\qwindows.dll"
SetOutPath "$INSTDIR\OpenVPN\x86"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\OpenVPN\x86\libeay32.dll"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\OpenVPN\x86\liblzo2-2.dll"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\OpenVPN\x86\libpkcs11-helper-1.dll"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\OpenVPN\x86\openvpn.exe"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\OpenVPN\x86\ssleay32.dll"
SetOutPath "$INSTDIR\OpenVPN\x64"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\OpenVPN\x64\libeay32.dll"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\OpenVPN\x64\liblzo2-2.dll"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\OpenVPN\x64\libpkcs11-helper-1.dll"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\OpenVPN\x64\openvpn.exe"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\OpenVPN\x64\ssleay32.dll"
SetOutPath "$INSTDIR\OpenVPN\Drivers\x86"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\OpenVPN\Drivers\x86\devcon86.exe"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\OpenVPN\Drivers\x86\driver.inf"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\OpenVPN\Drivers\x86\tap0901.cat"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\OpenVPN\Drivers\x86\tap0901.sys"
SetOutPath "$INSTDIR\OpenVPN\Drivers\x64"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\OpenVPN\Drivers\x64\devcon64.exe"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\OpenVPN\Drivers\x64\driver.inf"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\OpenVPN\Drivers\x64\tap0901.cat"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\OpenVPN\Drivers\x64\tap0901.sys"
SetOutPath "$INSTDIR\imageformats"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\imageformats\qdds.dll"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\imageformats\qgif.dll"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\imageformats\qicns.dll"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\imageformats\qico.dll"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\imageformats\qjpeg.dll"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\imageformats\qsvg.dll"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\imageformats\qtga.dll"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\imageformats\qtiff.dll"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\imageformats\qwbmp.dll"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\imageformats\qwebp.dll"
SetOutPath "$INSTDIR\iconengines"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\iconengines\qsvgicon.dll"
SetOutPath "$INSTDIR\bearer"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\bearer\qgenericbearer.dll"
File "C:\Users\Oplus\Documents\QProjects\build-NextVPN-Desktop_Qt_5_7_1_MinGW_32bit-Release\release\bearer\qnativewifibearer.dll"
SectionEnd

######################################################################

Section -Icons_Reg
SetOutPath "$INSTDIR"
WriteUninstaller "$INSTDIR\uninstall.exe"

!ifdef REG_START_MENU
!insertmacro MUI_STARTMENU_WRITE_BEGIN Application
CreateDirectory "$SMPROGRAMS\$SM_Folder"
CreateShortCut "$SMPROGRAMS\$SM_Folder\${APP_NAME}.lnk" "$INSTDIR\${MAIN_APP_EXE}"
CreateShortCut "$DESKTOP\${APP_NAME}.lnk" "$INSTDIR\${MAIN_APP_EXE}"
!ifdef WEB_SITE
WriteIniStr "$INSTDIR\${APP_NAME} website.url" "InternetShortcut" "URL" "${WEB_SITE}"
CreateShortCut "$SMPROGRAMS\$SM_Folder\${APP_NAME} Website.lnk" "$INSTDIR\${APP_NAME} website.url"
!endif
!insertmacro MUI_STARTMENU_WRITE_END
!endif

!ifndef REG_START_MENU
CreateDirectory "$SMPROGRAMS\NextVPN"
CreateShortCut "$SMPROGRAMS\NextVPN\${APP_NAME}.lnk" "$INSTDIR\${MAIN_APP_EXE}"
CreateShortCut "$DESKTOP\${APP_NAME}.lnk" "$INSTDIR\${MAIN_APP_EXE}"
!ifdef WEB_SITE
WriteIniStr "$INSTDIR\${APP_NAME} website.url" "InternetShortcut" "URL" "${WEB_SITE}"
CreateShortCut "$SMPROGRAMS\NextVPN\${APP_NAME} Website.lnk" "$INSTDIR\${APP_NAME} website.url"
!endif
!endif

WriteRegStr ${REG_ROOT} "${REG_APP_PATH}" "" "$INSTDIR\${MAIN_APP_EXE}"
WriteRegStr ${REG_ROOT} "${UNINSTALL_PATH}"  "DisplayName" "${APP_NAME}"
WriteRegStr ${REG_ROOT} "${UNINSTALL_PATH}"  "UninstallString" "$INSTDIR\uninstall.exe"
WriteRegStr ${REG_ROOT} "${UNINSTALL_PATH}"  "DisplayIcon" "$INSTDIR\${MAIN_APP_EXE}"
WriteRegStr ${REG_ROOT} "${UNINSTALL_PATH}"  "DisplayVersion" "${VERSION}"
WriteRegStr ${REG_ROOT} "${UNINSTALL_PATH}"  "Publisher" "${COMP_NAME}"

!ifdef WEB_SITE
WriteRegStr ${REG_ROOT} "${UNINSTALL_PATH}"  "URLInfoAbout" "${WEB_SITE}"
!endif

MessageBox MB_YESNO "Do you want to run NextVPN now?" IDYES true IDNO false
true:
  Exec "$INSTDIR\${APP_NAME}.exe"
false:
  DetailPrint "it's false"
SectionEnd

######################################################################

Section Uninstall
${INSTALL_TYPE}
Delete "$INSTDIR\cert.crt"
Delete "$INSTDIR\D3Dcompiler_47.dll"
Delete "$INSTDIR\key.key"
Delete "$INSTDIR\libEGL.dll"
Delete "$INSTDIR\libgcc_s_dw2-1.dll"
Delete "$INSTDIR\libGLESV2.dll"
Delete "$INSTDIR\libstdc++-6.dll"
Delete "$INSTDIR\libwinpthread-1.dll"
Delete "$INSTDIR\NextVPN.exe"
Delete "$INSTDIR\opengl32sw.dll"
Delete "$INSTDIR\pptp.pbk"
Delete "$INSTDIR\Qt5Core.dll"
Delete "$INSTDIR\Qt5Gui.dll"
Delete "$INSTDIR\Qt5Network.dll"
Delete "$INSTDIR\Qt5Svg.dll"
Delete "$INSTDIR\Qt5Widgets.dll"
Delete "$INSTDIR\Qt5Xml.dll"
Delete "$INSTDIR\servers"
Delete "$INSTDIR\sstp.pbk"
Delete "$INSTDIR\TunnelPlus\zebedee.exe"
Delete "$INSTDIR\translations\qt_ca.qm"
Delete "$INSTDIR\translations\qt_cs.qm"
Delete "$INSTDIR\translations\qt_de.qm"
Delete "$INSTDIR\translations\qt_en.qm"
Delete "$INSTDIR\translations\qt_fi.qm"
Delete "$INSTDIR\translations\qt_fr.qm"
Delete "$INSTDIR\translations\qt_he.qm"
Delete "$INSTDIR\translations\qt_hu.qm"
Delete "$INSTDIR\translations\qt_it.qm"
Delete "$INSTDIR\translations\qt_ja.qm"
Delete "$INSTDIR\translations\qt_ko.qm"
Delete "$INSTDIR\translations\qt_lv.qm"
Delete "$INSTDIR\translations\qt_pl.qm"
Delete "$INSTDIR\translations\qt_ru.qm"
Delete "$INSTDIR\translations\qt_sk.qm"
Delete "$INSTDIR\translations\qt_uk.qm"
Delete "$INSTDIR\STunnel\4758cca.dll"
Delete "$INSTDIR\STunnel\4758cca.dll.manifest"
Delete "$INSTDIR\STunnel\aep.dll"
Delete "$INSTDIR\STunnel\aep.dll.manifest"
Delete "$INSTDIR\STunnel\atalla.dll"
Delete "$INSTDIR\STunnel\atalla.dll.manifest"
Delete "$INSTDIR\STunnel\capi.dll"
Delete "$INSTDIR\STunnel\capi.dll.manifest"
Delete "$INSTDIR\STunnel\chil.dll"
Delete "$INSTDIR\STunnel\chil.dll.manifest"
Delete "$INSTDIR\STunnel\cswift.dll"
Delete "$INSTDIR\STunnel\cswift.dll.manifest"
Delete "$INSTDIR\STunnel\customdns.conf.txt"
Delete "$INSTDIR\STunnel\gmp.dll"
Delete "$INSTDIR\STunnel\gmp.dll.manifest"
Delete "$INSTDIR\STunnel\gost.dll"
Delete "$INSTDIR\STunnel\gost.dll.manifest"
Delete "$INSTDIR\STunnel\libeay32.dll"
Delete "$INSTDIR\STunnel\libeay32.dll.manifest"
Delete "$INSTDIR\STunnel\Microsoft.VC90.CRT.Manifest"
Delete "$INSTDIR\STunnel\msvcr90.dll"
Delete "$INSTDIR\STunnel\nuron.dll"
Delete "$INSTDIR\STunnel\nuron.dll.manifest"
Delete "$INSTDIR\STunnel\openssl.exe"
Delete "$INSTDIR\STunnel\openssl.exe.manifest"
Delete "$INSTDIR\STunnel\padlock.dll"
Delete "$INSTDIR\STunnel\padlock.dll.manifest"
Delete "$INSTDIR\STunnel\peer-Filter&amp;cl.pem"
Delete "$INSTDIR\STunnel\peer-Filter&cl.pem"
Delete "$INSTDIR\STunnel\ssleay32.dll"
Delete "$INSTDIR\STunnel\ssleay32.dll.manifest"
Delete "$INSTDIR\STunnel\stunnel.cnf"
Delete "$INSTDIR\STunnel\stunnel.conf"
Delete "$INSTDIR\STunnel\stunnel.exe"
Delete "$INSTDIR\STunnel\stunnel.exe.manifest"
Delete "$INSTDIR\STunnel\stunnel.pem"
Delete "$INSTDIR\STunnel\sureware.dll"
Delete "$INSTDIR\STunnel\sureware.dll.manifest"
Delete "$INSTDIR\STunnel\tstunnel.exe"
Delete "$INSTDIR\STunnel\tstunnel.exe.manifest"
Delete "$INSTDIR\STunnel\ubsec.dll"
Delete "$INSTDIR\STunnel\ubsec.dll.manifest"
Delete "$INSTDIR\STunnel\zlib1.dll"
Delete "$INSTDIR\STunnel\zlib1.dll.manifest"
Delete "$INSTDIR\Proxifier\Helper64.exe"
Delete "$INSTDIR\Proxifier\Proxifier.exe"
Delete "$INSTDIR\Proxifier\ProxyChecker.exe"
Delete "$INSTDIR\Proxifier\PrxDrvPE.dll"
Delete "$INSTDIR\Proxifier\PrxDrvPE64.dll"
Delete "$INSTDIR\Proxifier\Settings.ini"
Delete "$INSTDIR\Proxifier\Profiles\Default.ppx"
Delete "$INSTDIR\platforms\qwindows.dll"
Delete "$INSTDIR\OpenVPN\x86\libeay32.dll"
Delete "$INSTDIR\OpenVPN\x86\liblzo2-2.dll"
Delete "$INSTDIR\OpenVPN\x86\libpkcs11-helper-1.dll"
Delete "$INSTDIR\OpenVPN\x86\openvpn.exe"
Delete "$INSTDIR\OpenVPN\x86\ssleay32.dll"
Delete "$INSTDIR\OpenVPN\x64\libeay32.dll"
Delete "$INSTDIR\OpenVPN\x64\liblzo2-2.dll"
Delete "$INSTDIR\OpenVPN\x64\libpkcs11-helper-1.dll"
Delete "$INSTDIR\OpenVPN\x64\openvpn.exe"
Delete "$INSTDIR\OpenVPN\x64\ssleay32.dll"
Delete "$INSTDIR\OpenVPN\Drivers\x86\devcon86.exe"
Delete "$INSTDIR\OpenVPN\Drivers\x86\driver.inf"
Delete "$INSTDIR\OpenVPN\Drivers\x86\tap0901.cat"
Delete "$INSTDIR\OpenVPN\Drivers\x86\tap0901.sys"
Delete "$INSTDIR\OpenVPN\Drivers\x64\devcon64.exe"
Delete "$INSTDIR\OpenVPN\Drivers\x64\driver.inf"
Delete "$INSTDIR\OpenVPN\Drivers\x64\tap0901.cat"
Delete "$INSTDIR\OpenVPN\Drivers\x64\tap0901.sys"
Delete "$INSTDIR\imageformats\qdds.dll"
Delete "$INSTDIR\imageformats\qgif.dll"
Delete "$INSTDIR\imageformats\qicns.dll"
Delete "$INSTDIR\imageformats\qico.dll"
Delete "$INSTDIR\imageformats\qjpeg.dll"
Delete "$INSTDIR\imageformats\qsvg.dll"
Delete "$INSTDIR\imageformats\qtga.dll"
Delete "$INSTDIR\imageformats\qtiff.dll"
Delete "$INSTDIR\imageformats\qwbmp.dll"
Delete "$INSTDIR\imageformats\qwebp.dll"
Delete "$INSTDIR\iconengines\qsvgicon.dll"
Delete "$INSTDIR\bearer\qgenericbearer.dll"
Delete "$INSTDIR\bearer\qnativewifibearer.dll"
 
RmDir "$INSTDIR\bearer"
RmDir "$INSTDIR\iconengines"
RmDir "$INSTDIR\imageformats"
RmDir "$INSTDIR\OpenVPN\Drivers\x64"
RmDir "$INSTDIR\OpenVPN\Drivers\x86"
RmDir "$INSTDIR\OpenVPN\x64"
RmDir "$INSTDIR\OpenVPN\x86"
RmDir "$INSTDIR\platforms"
RmDir "$INSTDIR\Proxifier\Profiles"
RmDir "$INSTDIR\Proxifier"
RmDir "$INSTDIR\STunnel"
RmDir "$INSTDIR\translations"
RmDir "$INSTDIR\TunnelPlus"
 
Delete "$INSTDIR\uninstall.exe"
!ifdef WEB_SITE
Delete "$INSTDIR\${APP_NAME} website.url"
!endif

RmDir "$INSTDIR"

!ifdef REG_START_MENU
!insertmacro MUI_STARTMENU_GETFOLDER "Application" $SM_Folder
Delete "$SMPROGRAMS\$SM_Folder\${APP_NAME}.lnk"
!ifdef WEB_SITE
Delete "$SMPROGRAMS\$SM_Folder\${APP_NAME} Website.lnk"
!endif
Delete "$DESKTOP\${APP_NAME}.lnk"

RmDir "$SMPROGRAMS\$SM_Folder"
!endif

!ifndef REG_START_MENU
Delete "$SMPROGRAMS\NextVPN\${APP_NAME}.lnk"
!ifdef WEB_SITE
Delete "$SMPROGRAMS\NextVPN\${APP_NAME} Website.lnk"
!endif
Delete "$DESKTOP\${APP_NAME}.lnk"

RmDir "$SMPROGRAMS\NextVPN"
!endif

DeleteRegKey ${REG_ROOT} "${REG_APP_PATH}"
DeleteRegKey ${REG_ROOT} "${UNINSTALL_PATH}"
SectionEnd

######################################################################

