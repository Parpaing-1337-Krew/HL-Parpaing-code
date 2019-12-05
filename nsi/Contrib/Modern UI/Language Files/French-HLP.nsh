;NSIS Modern User Interface - Language File
;Compatible with Modern UI 1.68

;Language: French (1036)
;By S�bastien Delahaye <seb@delahaye.net>
;Adapted for HLP by Mathieu Gollain <xander135@free.fr>

;--------------------------------

!insertmacro MUI_LANGUAGEFILE_BEGIN "FRENCH"

  !define MUI_LANGNAME "Fran�ais"

  !define MUI_TEXT_WELCOME_INFO_TITLE "Bienvenue dans le programme d'installation d'$(^Name)"
  !define MUI_TEXT_WELCOME_INFO_TEXT "Vous �tes sur le point d'installer $(^Name) sur votre ordinateur.\r\n\r\n"

  !define MUI_TEXT_LICENSE_TITLE "Licence utilisateur"
  !define MUI_TEXT_LICENSE_SUBTITLE "Veuillez examiner les termes de la licence avant d'installer $(^Name)."
  !define MUI_INNERTEXT_LICENSE_TOP "Appuyez sur Page Down pour lire le reste de la licence utilisateur."
  !define MUI_INNERTEXT_LICENSE_BOTTOM "Si vous acceptez les termes de la licence utilisateur, cliquez sur J'accepte pour continuer. Vous devez accepter la licence utilisateur pour installer $(^Name)."
  !define MUI_INNERTEXT_LICENSE_BOTTOM_CHECKBOX "Si vous acceptez les termes de la licence utilisateur, cochez la case ci-dessous. Vous devez accepter la licence utilisateur pour installer $(^Name)."
  !define MUI_INNERTEXT_LICENSE_BOTTOM_RADIOBUTTONS "Si vous acceptez les termes de la licence utilisateur, s�lectionnez le premier choix ci-dessous. Vous devez accepter la licence utilisateur pour installer $(^Name)."

  !define MUI_TEXT_COMPONENTS_TITLE "Choisissez les composants"
  !define MUI_TEXT_COMPONENTS_SUBTITLE "Choisissez les composants d'$(^Name) que vous souhaitez installer."
  !define MUI_INNERTEXT_COMPONENTS_DESCRIPTION_TITLE "Description"
  !define MUI_INNERTEXT_COMPONENTS_DESCRIPTION_INFO "Passer le curseur de votre souris sur un composant pour voir sa description."

  !define MUI_TEXT_DIRECTORY_TITLE "Choisissez le dossier d'installation"
  !define MUI_TEXT_DIRECTORY_SUBTITLE "Choisissez le dossier dans lequel installer $(^Name). Normalement autod�tect�. Doit contenir hl.exe !"
  !define MUI_TEXT_INSTALLING_TITLE "Installation en cours"
  !define MUI_TEXT_INSTALLING_SUBTITLE "Veuillez patienter pendant qu'$(^Name) est en train d'�tre install�."

  !define MUI_TEXT_FINISH_TITLE "Installation termin�e"
  !define MUI_TEXT_FINISH_SUBTITLE "L'installation s'est termin�e avec succ�s."

  !define MUI_TEXT_ABORT_TITLE "Installation interrompue"
  !define MUI_TEXT_ABORT_SUBTITLE "L'installation n'a pas �t� termin�e."

  !define MUI_BUTTONTEXT_FINISH "&Fermer"
  !define MUI_TEXT_FINISH_INFO_TITLE "Fin de l'installation d'$(^Name)"
  !define MUI_TEXT_FINISH_INFO_TEXT "$(^Name) a �t� install� sur votre ordinateur.\r\n\r\nCliquez sur Fermer pour quitter le programme d'installation."
  !define MUI_TEXT_FINISH_INFO_REBOOT "Votre ordinateur doit �tre red�marr� afin de compl�ter l'installation d'$(^Name). Souhaitez-vous red�marrer maintenant ?"
  !define MUI_TEXT_FINISH_REBOOTNOW "Red�marrer maintenant"
  !define MUI_TEXT_FINISH_REBOOTLATER "Je souhaite red�marrer moi-m�me plus tard"
  !define MUI_TEXT_FINISH_RUN "Lancer $(^Name)"
  !define MUI_TEXT_FINISH_SHOWREADME "Afficher le fichier Readme"

  !define MUI_TEXT_STARTMENU_TITLE "Choisissez un dossier dans le menu D�marrer"
  !define MUI_TEXT_STARTMENU_SUBTITLE "Choisissez un dossier dans le menu D�marrer pour les raccourcis de l'application."
  !define MUI_INNERTEXT_STARTMENU_TOP "Choisissez le dossier du menu D�marrer dans lequel vous voulez placer les raccourcis du programme. Vous pouvez �galement entrer un nouveau nom pour cr�er un nouveau dossier."
  !define MUI_INNERTEXT_STARTMENU_CHECKBOX "Ne pas cr�er de raccourcis"

  !define MUI_TEXT_ABORTWARNING "�tes-vous s�r de vouloir quitter l'installation d'$(^Name) ?"


  !define MUI_UNTEXT_CONFIRM_TITLE "D�sinstaller $(^Name)"
  !define MUI_UNTEXT_CONFIRM_SUBTITLE "Supprimer $(^Name) de votre ordinateur."

  !define MUI_UNTEXT_LICENSE_TITLE "Licence utilisateur"
  !define MUI_UNTEXT_LICENSE_SUBTITLE "Veuillez examiner les termes de la licence avant de d�sinstaller $(^Name)."
  !define MUI_UNINNERTEXT_LICENSE_BOTTOM "Si vous acceptez les termes de la licence utilisateur, cliquez sur J'accepte pour continuer. Vous devez accepter la licence utilisateur pour d�sinstaller $(^Name)."
  !define MUI_UNINNERTEXT_LICENSE_BOTTOM_CHECKBOX "Si vous acceptez les termes de la licence utilisateur, cochez la case ci-dessous. Vous devez accepter la licence utilisateur pour d�sintaller $(^Name)."
  !define MUI_UNINNERTEXT_LICENSE_BOTTOM_RADIOBUTTONS "Si vous acceptez les termes de la licence utilisateur, s�lectionnez le premier choix ci-dessous. Vous devez accepter la licence utilisateur pour d�sinstaller $(^Name)."

  !define MUI_UNTEXT_UNINSTALLING_TITLE "D�sinstallation en cours"
  !define MUI_UNTEXT_UNINSTALLING_SUBTITLE "Veuillez patienter pendant qu'$(^Name) est en train d'�tre supprim� de votre ordinateur."

  !define MUI_UNTEXT_COMPONENTS_TITLE "Choisissez les composants"
  !define MUI_UNTEXT_COMPONENTS_SUBTITLE "Cochez les composants d'$(^Name) que vous souhaitez d�sinstaller."

  !define MUI_UNTEXT_DIRECTORY_TITLE "Choisissez le dossier d'installation"
  !define MUI_UNTEXT_DIRECTORY_SUBTITLE "Choisissez le dossier dans lequel vous voulez d�sinstaller $(^Name)."

  !define MUI_UNTEXT_FINISH_TITLE "D�sinstallation termin�e"
  !define MUI_UNTEXT_FINISH_SUBTITLE "La d�sinstallation s'est termin�e avec succ�s."

  !define MUI_UNTEXT_ABORT_TITLE "D�sinstallation interrompue"
  !define MUI_UNTEXT_ABORT_SUBTITLE "La d�sinstallation n'a pas �t� termin�e."

!insertmacro MUI_LANGUAGEFILE_END