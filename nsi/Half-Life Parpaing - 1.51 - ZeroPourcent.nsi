; Half-Life Parpaing : NSIS2 Script
; Version 0.51.1
; Par Xander135 ( Xander135@free.fr ) pour Half-Life Parpaing ( http://hlp.pmad.net )
;
; Editeur de texte recommandé pour le NSISage : Crimson Editor ( http://www.crimsoneditor.com/ )
; Script pour NSIS2.07

!define VERSION "1.51" ; Version de l'insanité
Name "Half-Life Parpaing ${VERSION}" ; Et son nom

!define VER_MAJOR 1
!define VER_MINOR 51_ZeroPourcent

!include "MUI.nsh" ; MUI : Modern UI

OutFile "Half-Life_Parpaing_${VER_MAJOR}.${VER_MINOR}.exe"

;--------------------------------
;Configuration

  ; LE chemin d'installation
  InstallDir "C:\SIERRA\Half-Life"

  InstallDirRegKey HKCU "Software\Valve\Steam" "ModInstallPath" ; Si Steam installé, install sur Steam, sinon, chemin HL par défaut

  ; Vérification de la validité du répertoire choisi.
  Function .onVerifyInstDir
  IfFileExists $INSTDIR\hl.exe Good ; Vérification : erreur si hl.exe n'existe pas dans le répertoire choisi
      Abort
    Good:
  FunctionEnd

  Function SetMyOutPath
    SetOutPath $INSTDIR
  FunctionEnd

  SetCompressor /SOLID lzma

  CRCCheck on ; Vaut mieux CRCer la chose vu la taille qu'elle a
  XPStyle on ; Ajoute un .manifest au header

  ; Utilisation d'un packer ( ici UPX : http://upx.sourceforge.net/ )
  ; Ligne 46 : Nom du fichier temporaire
  ; Ligne 47 : Chemin du packer et niveau de compression (ici UPX au max) avec nom du fichier temporaire au bout
  !packhdr \
  "temp.dat" \
  "C:\UPX\upx.exe -9 -t -V -L -v --best temp.dat" ; --all-methods --all-filters (uniquement avec UPX V1.90+)

;--------------------------------
;Modern UI Configuration
  ReserveFile "${NSISDIR}\Contrib\Graphics\Checks\parpchecks.bmp"
  ReserveFile "${NSISDIR}\Contrib\Graphics\Header\modern-header_prp.bmp"

  !define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\new_hlp_main.ico"
  !define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\hlp-uninst.ico"

  !define MUI_HEADERIMAGE
  !define MUI_HEADERIMAGE_BITMAP "${NSISDIR}\Contrib\Graphics\Header\modern-header_prp.bmp"
  !define MUI_COMPONENTSPAGE_CHECKBITMAP "${NSISDIR}\Contrib\Graphics\Checks\parpchecks.bmp"

  !define MUI_ABORTWARNING
  
  !insertmacro MUI_PAGE_LICENSE "${NSISDIR}\gpl.txt"
  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES

  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES

  !define MUI_LICENSEPAGE_CHECKBOX

  ; !define MUI_COMPONENTSPAGE_SMALLDESC ; Bof... Je préfère l'autre mise en page.

  !define MUI_FINISHPAGE_NOAUTOCLOSE
  !define MUI_FINISHPAGE_NOREBOOTSUPPORT

  !define MUI_FINISHPAGE_RUN "$INSTDIR\hl.exe"
  !define MUI_FINISHPAGE_RUN_PARAMETERS "-console -game hl-parpaing"
  !define MUI_FINISHPAGE_RUN_NOTCHECKED

  !define MUI_FINISHPAGE_SHOWREADME "$INSTDIR\hl-parpaing\readme.txt"
  !define MUI_FINISHPAGE_SHOWREADME_NOTCHECKED

  !define MUI_FINISHPAGE_LINK "Site Internet d'HLP"
  !define MUI_FINISHPAGE_LINK_LOCATION "http://hlp.pmad.net/"

  !insertmacro MUI_PAGE_FINISH

;--------------------------------
;Languages

  ; !insertmacro MUI_LANGUAGE "English" <- n'alourdissons pas l'installeur avec un language inutile
  !insertmacro MUI_LANGUAGE "French-HLP"

;--------------------------------
; Langues et descriptions

  ; Descriptions FR
  LangString DESC_InstallMod ${LANG_FRENCH} "Le mod (la base requise pour jouer)"
  LangString DESC_InstallShortcuts ${LANG_FRENCH} "Raccourcis dans le Menu Démarrer et sur le Bureau"
  LangString DESC_InstallPubsImgs ${LANG_FRENCH} "Publicités, images diverses, ...                                      (\extras\images\) (\extras\media\) (\extras\pubs\)"
  LangString DESC_InstallNsi ${LANG_FRENCH} "Fichier .nsi d'HLP pour l'installeur NSIS2 + icônes        (\extras\nsi\)"

;--------------------------------
; Sections Installeur

; ShowInstDetails show ; Ca ralentit tout, laissons donc les détails en option

InstType "Standard"

; V'là le mod !
Section "Mod" InstallMod
  SetDetailsPrint textonly
  DetailPrint "Installation du mod..."
  SetDetailsPrint listonly
  SectionIn 1 RO

  Call SetMyOutPath

; DLLS
  SetOutPath "$INSTDIR"
  File "${NSISDIR}\HLP\hl-parpaing\fmod.dll"
  SetOutPath "$INSTDIR\hl-parpaing\cl_dlls"
  File "${NSISDIR}\HLP\hl-parpaing\cl_dlls\client.dll"
  SetOutPath "$INSTDIR\hl-parpaing\dlls"
  File "${NSISDIR}\HLP\hl-parpaing\dlls\hl.dll"
  File "${NSISDIR}\HLP\hl-parpaing\dlls\libmySQL.dll"
  File "${NSISDIR}\HLP\hl-parpaing\dlls\mp_i386.so"
  File "${NSISDIR}\HLP\hl-parpaing\dlls\sql.exe"

; AUTRES
  SetOutPath "$INSTDIR\hl-parpaing\events"
  File /r "${NSISDIR}\HLP\hl-parpaing\events\*.sc"
  SetOutPath "$INSTDIR\hl-parpaing\gfx"
  File /r "${NSISDIR}\HLP\hl-parpaing\gfx\*.*"
  SetOutPath "$INSTDIR\hl-parpaing\maps"
  File /r "${NSISDIR}\HLP\hl-parpaing\maps\*.*"
  SetOutPath "$INSTDIR\hl-parpaing\media"
  File /r "${NSISDIR}\HLP\hl-parpaing\media\*.*"
  SetOutPath "$INSTDIR\hl-parpaing\models"
  File /r "${NSISDIR}\HLP\hl-parpaing\models\*.*"
  SetOutPath "$INSTDIR\hl-parpaing\resource" ; Partie "Interface Steam" (HL sans Steam est toujours supporté)
  File /r "${NSISDIR}\HLP\hl-parpaing\resource\*.*" ; Vive HL sans Steam
  SetOutPath "$INSTDIR\hl-parpaing\sound"
  File /r "${NSISDIR}\HLP\hl-parpaing\sound\*.*"
  SetOutPath "$INSTDIR\hl-parpaing\sprites"
  File /r "${NSISDIR}\HLP\hl-parpaing\sprites\*.*"

; WADS
  SetOutPath "$INSTDIR\hl-parpaing"
  File "${NSISDIR}\HLP\hl-parpaing\buildingc4.wad"
  File "${NSISDIR}\HLP\hl-parpaing\country.wad"
  File "${NSISDIR}\HLP\hl-parpaing\fortboyard.wad"
  File "${NSISDIR}\HLP\hl-parpaing\hlp_cimetiere.wad"
  File "${NSISDIR}\HLP\hl-parpaing\hlp_moutmoutland.wad"
  File "${NSISDIR}\HLP\hl-parpaing\hlp_musee.wad"
  File "${NSISDIR}\HLP\hl-parpaing\hlp_silograin.wad"
  File "${NSISDIR}\HLP\hl-parpaing\mout.wad"
  File "${NSISDIR}\HLP\hl-parpaing\mywads.wad"
  File "${NSISDIR}\HLP\hl-parpaing\piscines.wad"
  File "${NSISDIR}\HLP\hl-parpaing\ricard.wad"
  File "${NSISDIR}\HLP\hl-parpaing\ruinaga.wad"
  File "${NSISDIR}\HLP\hl-parpaing\sti_hlp_restoroute.wad"
  File "${NSISDIR}\HLP\hl-parpaing\trouperdu.wad"

; ICONES
  File "${NSISDIR}\HLP\hl-parpaing\hlp.ico"
  File "${NSISDIR}\HLP\hl-parpaing\hlp2.ico"

; README
  File "${NSISDIR}\HLP\hl-parpaing\readme.txt"

; CONFIG & DIVERS
  File "${NSISDIR}\HLP\hl-parpaing\1024_textscheme.txt"
  File "${NSISDIR}\HLP\hl-parpaing\1152_textscheme.txt"
  File "${NSISDIR}\HLP\hl-parpaing\1280_textscheme.txt"
  File "${NSISDIR}\HLP\hl-parpaing\1600_textscheme.txt"
  File "${NSISDIR}\HLP\hl-parpaing\320_textscheme.txt"
  File "${NSISDIR}\HLP\hl-parpaing\400_textscheme.txt"
  File "${NSISDIR}\HLP\hl-parpaing\512_textscheme.txt"
  File "${NSISDIR}\HLP\hl-parpaing\640_textscheme.txt"
  File "${NSISDIR}\HLP\hl-parpaing\800_textscheme.txt"
  File "${NSISDIR}\HLP\hl-parpaing\cached.wad"
  File "${NSISDIR}\HLP\hl-parpaing\config.cfg"
  File "${NSISDIR}\HLP\hl-parpaing\game.cfg"
  File "${NSISDIR}\HLP\hl-parpaing\gpl.txt" ; GPL pwnz
  File "${NSISDIR}\HLP\hl-parpaing\liblist.gam"
  File "${NSISDIR}\HLP\hl-parpaing\mapcycle.txt"
  File "${NSISDIR}\HLP\HL-Parpaing\readme-linux"
  File "${NSISDIR}\HLP\hl-parpaing\server.cfg"
  File "${NSISDIR}\HLP\hl-parpaing\Serveur Dedie.bat"
  File "${NSISDIR}\HLP\hl-parpaing\settings.scr" ; Accents virés pour compatibilité Steam
  File "${NSISDIR}\HLP\hl-parpaing\user.scr" ; Idem

; Mode d'emploi
  SetOutPath "$INSTDIR\hl-parpaing\howto"
  File /r "${NSISDIR}\HLP\hl-parpaing\howto\*.*"

; FGD
  SetOutPath "$INSTDIR\hl-parpaing\extras\fgd"
  File "${NSISDIR}\HLP\hl-parpaing\extras\fgd\hl-parpaing.fgd"

SectionEnd

; Là, c'est les raccourcis
Section "Raccourcis" InstallShortcuts
  SetDetailsPrint textonly
  DetailPrint "Installation des raccourcis..."
  SetDetailsPrint listonly
  SectionIn 1

; CreateShortCut shortcut_name.lnk shortcut_target [parameters [icon_file [icon index [showmode [hotkey]]]]]

SetOutPath "$SMPROGRAMS\Half-Life Parpaing"
  CreateShortCut "$SMPROGRAMS\Half-Life Parpaing\Désinstaller HLP.lnk" \
                 "$INSTDIR\hl-parpaing\hlp-uninst.exe"

  SetOutPath "$INSTDIR"
  CreateShortCut "$SMPROGRAMS\Half-Life Parpaing\Jouer à HLP.lnk" \
                 "$INSTDIR\hl.exe" "-console -game hl-parpaing" \
                 "$INSTDIR\hl-parpaing\hlp.ico" "0" "SW_SHOWNORMAL" ""

  WriteINIStr "$SMPROGRAMS\Half-Life Parpaing\Site Internet d'HLP.url" \
              "InternetShortcut" "URL" "http://hlp.pmad.net/"

  WriteINIStr "$SMPROGRAMS\Half-Life Parpaing\Mode d'emploi d'HLP.url" \
              "InternetShortcut" "URL" "$INSTDIR\hl-parpaing\howto\index.html"

  CreateShortCut "$DESKTOP\Half-Life Parpaing.lnk" \
                 "$INSTDIR\hl.exe" "-console -game hl-parpaing" \
                 "$INSTDIR\hl-parpaing\hlp2.ico" "0" "SW_SHOWNORMAL" ""
SectionEnd

; Pubs JPG
Section "Pubs HLP" InstallPubsImgs
  SetDetailsPrint textonly
  DetailPrint "Installation des extras graphiques et sonores..."
  SetDetailsPrint listonly
  SectionIn 1

  SetOutPath "$INSTDIR\hl-parpaing\extras\images"
  File /r "${NSISDIR}\HLP\hl-parpaing\extras\images\*.*"
  SetOutPath "$INSTDIR\hl-parpaing\extras\media"
  File /r "${NSISDIR}\HLP\hl-parpaing\extras\media\*.*"
  SetOutPath "$INSTDIR\hl-parpaing\extras\pubs"
  File /r "${NSISDIR}\HLP\hl-parpaing\extras\pubs\*.*"
SectionEnd

Section "-Désinstallation" InstallUninstall
  SetDetailsPrint textonly
  DetailPrint "Configuration de la désinstallation..."
  SetDetailsPrint listonly
  SectionIn 1

  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Half-Life Parpaing" "DisplayName" "Half-Life Parpaing 1.51 (désinstallation)"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Half-Life Parpaing" "UninstallString" "$INSTDIR\hl-parpaing\hlp-uninst.exe"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Half-Life Parpaing" "InstallLocation" "$INSTDIR\"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Half-Life Parpaing" "Publisher" "Parpignists, Inc."
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Half-Life Parpaing" "URLInfoAbout" "http://hlp.pmad.net"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Half-Life Parpaing" "NoModify" "-1"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Half-Life Parpaing" "NoRepair" "-1"

  SetOutPath $INSTDIR
  Delete "$INSTDIR\hl-parpaing\hlp-uninst.exe"
  WriteUninstaller "$INSTDIR\hl-parpaing\hlp-uninst.exe"
SectionEnd

Section "-Fin" End
  SetDetailsPrint textonly
  DetailPrint "Terminé. Y'a plus qu'à jouer."
  SetDetailsPrint listonly
  SectionIn 1 2 3
SectionEnd

; Apparition de la fenêtre

Section -post
   ExecShell open '$SMPROGRAMS\Half-Life Parpaing'
   Sleep 500
   BringToFront
SectionEnd

;--------------------------------
; Descriptions

!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${InstallMod} $(DESC_InstallMod)
  !insertmacro MUI_DESCRIPTION_TEXT ${InstallShortcuts} $(DESC_InstallShortcuts)
  !insertmacro MUI_DESCRIPTION_TEXT ${InstallPubsImgs} $(DESC_InstallPubsImgs)
!insertmacro MUI_FUNCTION_DESCRIPTION_END

;--------------------------------
; Désinstallation, partie 2

; ShowUninstDetails show ; Ca ralentit tout, laissons donc les détails en option

UninstallCaption "Désinstallation d'Half-Life Parpaing"

Section Uninstall
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Half-Life Parpaing"
  Delete "$SMPROGRAMS\Half-Life Parpaing\*.*"
  RMDir /r "$SMPROGRAMS\Half-Life Parpaing"
  Delete "$DESKTOP\Half-Life Parpaing.lnk"
  Delete "$INSTDIR\cl_dlls\*.*"
  Delete "$INSTDIR\dlls\*.*"
  Delete "$INSTDIR\events\*.*"
  Delete "$INSTDIR\extras\fgd\*.*"
  Delete "$INSTDIR\extras\images\*.*"
  Delete "$INSTDIR\extras\images\gifs\*.*"
  Delete "$INSTDIR\extras\media\*.*"
  Delete "$INSTDIR\extras\pubs\*.*"
  Delete "$INSTDIR\extras\screens\*.*"
  Delete "$INSTDIR\gfx\env\*.*"
  Delete "$INSTDIR\gfx\shell\*.*"
  Delete "$INSTDIR\howto\*.*"
  Delete "$INSTDIR\howto\images\*.*"
  Delete "$INSTDIR\howto\images\team\*.*"
  Delete "$INSTDIR\maps\*.*"
  Delete "$INSTDIR\media\*.*"
  Delete "$INSTDIR\models\*.*"
  Delete "$INSTDIR\models\player\Inspecteur\*.*"
  Delete "$INSTDIR\models\player\Macon[51]\*.*"
  Delete "$INSTDIR\models\player\Macon[Ricard]\*.*"
  Delete "$INSTDIR\resource\background\*.*"
  Delete "$INSTDIR\sound\*.*"
  Delete "$INSTDIR\sound\alcool\*.*"
  Delete "$INSTDIR\sound\building\*.*"
  Delete "$INSTDIR\sound\feedback\*.*"
  Delete "$INSTDIR\sound\flotagogo\*.*"
  Delete "$INSTDIR\sound\fortboyard\*.*"
  Delete "$INSTDIR\sound\items\*.*"
  Delete "$INSTDIR\sound\lepenthouse\*.*"
  Delete "$INSTDIR\sound\megret\*.*"
  Delete "$INSTDIR\sound\moutmoutland\*.*"
  Delete "$INSTDIR\sound\player\*.*"
  Delete "$INSTDIR\sound\restoroute\*.*"
  Delete "$INSTDIR\sound\restoroute\ambiance\*.*"
  Delete "$INSTDIR\sound\restoroute\zik\*.*"
  Delete "$INSTDIR\sound\ruinaga\*.*"
  Delete "$INSTDIR\sound\trouperdu\*.*"
  RMDir /r "$INSTDIR\sound\voix\Commentaires\Commentaires_persos"
  Delete "$INSTDIR\sound\voix\Commentaires\Inspecteurs_gagnent\*.*"
  Delete "$INSTDIR\sound\voix\Commentaires\Maçons_gagnent\*.*"
  Delete "$INSTDIR\sound\voix\Commentaires\macons_gagnent\*.*"
  RMDir  "$INSTDIR\sound\voix\Inspecteurs\Aie"
  Delete "$INSTDIR\sound\voix\Inspecteurs\Blame\*.*"
  Delete "$INSTDIR\sound\voix\Inspecteurs\CroiseMacon\*.*"
  Delete "$INSTDIR\sound\voix\Inspecteurs\BlameMaconBourre\*.*"
  Delete "$INSTDIR\sound\voix\Macons\BoisRicard\*.*"
  Delete "$INSTDIR\sound\voix\Macons\PerdVie\*.*"
  Delete "$INSTDIR\sound\voix\Macons\PoseParpaing\*.*"
  Delete "$INSTDIR\sound\voix\Macons\PrendParpaing\*.*"
  Delete "$INSTDIR\sound\voix\Macons\Blame\*.*"
  Delete "$INSTDIR\sound\weapons\*.*"
  Delete "$INSTDIR\sound\partpaing\*.*"
  Delete "$INSTDIR\sound\sncf\*.*"
  Delete "$INSTDIR\sprites\*.*"
  Delete "$INSTDIR\*.*"
  RMDir /r "$INSTDIR"
SectionEnd