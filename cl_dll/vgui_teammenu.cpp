//=========== (C) Copyright 1999 Valve, L.L.C. All rights reserved. ===========
//
// The copyright to the contents herein is the property of Valve, L.L.C.
// The contents may be used and/or copied only with the written permission of
// Valve, L.L.C., or in accordance with the terms and conditions stipulated in
// the agreement/contract under which the contents have been supplied.
//
// Purpose: TFC Team Menu
//
// $Workfile:     $
// $Date: 2003/04/28 20:46:41 $
//
//-----------------------------------------------------------------------------
// $Log: vgui_teammenu.cpp,v $
// Revision 1.1  2003/04/28 20:46:41  madfab
// *** empty log message ***
//
//
// $NoKeywords: $
//=============================================================================


/*-----------------------------------------------------------------------------

	Déclaration de la classe à changer dans vgui_teamfortressviewport.h
	(ligne 1160 si ta pas trop changer de truc ds ce fichier

-----------------------------------------------------------------------------*/

#include "vgui_int.h"
#include "VGUI_Font.h"
#include "VGUI_ScrollPanel.h"
#include "VGUI_TextImage.h"

#include "hud.h"
#include "cl_util.h"
#include "vgui_TeamFortressViewport.h"

// Team Menu Dimensions
#define TEAMMENU_TITLE_X				XRES(40)
#define TEAMMENU_TITLE_Y				YRES(32)
#define TEAMMENU_TOPLEFT_BUTTON_X		XRES(40)
#define TEAMMENU_TOPLEFT_BUTTON_Y		YRES(80)
#define TEAMMENU_BUTTON_SIZE_X			XRES(124)
#define TEAMMENU_BUTTON_SIZE_Y			YRES(24)
#define TEAMMENU_BUTTON_SPACER_Y		YRES(8)
#define TEAMMENU_WINDOW_X				XRES(176)
#define TEAMMENU_WINDOW_Y				YRES(80)
#define TEAMMENU_WINDOW_SIZE_X			XRES(424)
#define TEAMMENU_WINDOW_SIZE_Y			YRES(312)
#define TEAMMENU_WINDOW_TITLE_X			XRES(16)
#define TEAMMENU_WINDOW_TITLE_Y			YRES(16)
#define TEAMMENU_WINDOW_TEXT_X			XRES(16)
#define TEAMMENU_WINDOW_TEXT_Y			YRES(48)
#define TEAMMENU_WINDOW_TEXT_SIZE_Y		YRES(178)
#define TEAMMENU_WINDOW_INFO_X			XRES(16)
#define TEAMMENU_WINDOW_INFO_Y			YRES(234)

char* TeamName_Button[] = // By MiK : on mets les noms des teams/models dans un tableau de chaine de caractere
{
	"",				// Les boutons st numerotés a partir de un, donc rien pour le n°0
	"Macon[51]",
	"Macon[Ricard]",
	"Inspecteur",
	"Spectateur"
};

char* TeamName_Caption[] = // By MiK : et la c le nom tel kil sera affiché a l'écran.
{
	"",				// Pareil, on compte pas le premier
	"Macon 51",	// nom affiché sur le bouton
	"Macon Ricard",
	"Inspecteur",
	"Spectateur"
};

// Creation
CTeamMenuPanel::CTeamMenuPanel(int iTrans, int iRemoveMe, int x,int y,int wide,int tall) : CMenuPanel(iTrans, iRemoveMe, x,y,wide,tall)
{
	// Get the scheme used for the Titles
	CSchemeManager *pSchemes = gViewPort->GetSchemeManager();

	// schemes
	SchemeHandle_t hTitleScheme = pSchemes->getSchemeHandle( "Title Font" );
	SchemeHandle_t hTeamWindowText = pSchemes->getSchemeHandle( "Briefing Text" );
	SchemeHandle_t hTeamInfoText = pSchemes->getSchemeHandle( "Team Info Text" );

	// get the Font used for the Titles
	Font *pTitleFont = pSchemes->getFont( hTitleScheme );
	int r, g, b, a;




	// Fait les belles ligne de demarcation entre les classes


	// Create the title
	Label *pLabel = new Label( "", TEAMMENU_TITLE_X, TEAMMENU_TITLE_Y );
	pLabel->setParent( this );
	pLabel->setFont( pTitleFont );
	pSchemes->getFgColor( hTitleScheme, r, g, b, a );
	pLabel->setFgColor( r, g, b, a );
	pSchemes->getBgColor( hTitleScheme, r, g, b, a );
	pLabel->setBgColor( r, g, b, a );
	pLabel->setContentAlignment( vgui::Label::a_west );
	pLabel->setText(gHUD.m_TextMessage.BufferedLocaliseTextString("Half-Life Parpaing : Choisissez votre équipe :"));

	// Create the Info Window
	m_pTeamWindow  = new CTransparentPanel( 255, TEAMMENU_WINDOW_X, TEAMMENU_WINDOW_Y, TEAMMENU_WINDOW_SIZE_X, TEAMMENU_WINDOW_SIZE_Y );
	m_pTeamWindow->setParent( this );
	m_pTeamWindow->setBorder( new LineBorder( Color(255*0.7,170*0.7,0,0)) );

	// Create the Map Name Label
	m_pMapTitle = new Label( "", TEAMMENU_WINDOW_TITLE_X, TEAMMENU_WINDOW_TITLE_Y );
	m_pMapTitle->setFont( pTitleFont ); 
	m_pMapTitle->setParent( m_pTeamWindow );
	pSchemes->getFgColor( hTitleScheme, r, g, b, a );
	m_pMapTitle->setFgColor( r, g, b, a );
	pSchemes->getBgColor( hTitleScheme, r, g, b, a );
	m_pMapTitle->setBgColor( r, g, b, a );
	m_pMapTitle->setContentAlignment( vgui::Label::a_west );

	// Create the Scroll panel
	m_pScrollPanel = new CTFScrollPanel( TEAMMENU_WINDOW_TEXT_X, TEAMMENU_WINDOW_TEXT_Y, TEAMMENU_WINDOW_SIZE_X - (TEAMMENU_WINDOW_TEXT_X * 2), TEAMMENU_WINDOW_TEXT_SIZE_Y );
	m_pScrollPanel->setParent(m_pTeamWindow);
	m_pScrollPanel->setScrollBarVisible(false, false);

	// test
	m_pImage = new CImageLabel("mac10.tga",0,0,TEAMMENU_WINDOW_TITLE_X,TEAMMENU_WINDOW_TITLE_Y);

	// Create the Map Briefing panel
	m_pBriefing = new TextPanel("", 0,0, TEAMMENU_WINDOW_SIZE_X - TEAMMENU_WINDOW_TEXT_X, TEAMMENU_WINDOW_TEXT_SIZE_Y );
	m_pBriefing->setParent( m_pScrollPanel->getClient() );
	m_pBriefing->setFont( pSchemes->getFont(hTeamWindowText) );
	pSchemes->getFgColor( hTeamWindowText, r, g, b, a );
	m_pBriefing->setFgColor( r, g, b, a );
	pSchemes->getBgColor( hTeamWindowText, r, g, b, a );
	m_pBriefing->setBgColor( r, g, b, a );

	m_pBriefing->setText("Map Description not available.");
	
	// Team Menu buttons
	for (int i = 1; i <=4; i++)
	{
		char sz[256]; 

		int iYPos = TEAMMENU_TOPLEFT_BUTTON_Y + ( (TEAMMENU_BUTTON_SIZE_Y + TEAMMENU_BUTTON_SPACER_Y) * i );

		// By MiK : on change ds la premiere ligne le nom du bouton avec la variable crée en debut de fichier
		m_pButtons[i] = new CommandButton( TeamName_Caption[i], TEAMMENU_TOPLEFT_BUTTON_X, iYPos, TEAMMENU_BUTTON_SIZE_X, TEAMMENU_BUTTON_SIZE_Y, true);
		m_pButtons[i]->setParent( this );
		m_pButtons[i]->setContentAlignment( vgui::Label::a_west );
		m_pButtons[i]->setVisible( false );

		
		// Create the Signals
		sprintf(sz, "model %s", TeamName_Button[i] ); // on créer la chaine de caractere "model nom_du_model
		m_pButtons[i]->addInputSignal( new CHandler_MenuButtonOver(this, i) );
		m_pButtons[i]->addActionSignal( new CMenuHandler_StringCommand( TeamName_Button[i], true ) );
		m_pButtons[i]->addActionSignal( new CMenuHandler_StringCommand( sz, true ) ); // et on l'utilise ici
		
		sprintf( sz, "%d", i ); // By MiK : pour choper le nom de la touche on transforme le chiffe de i en caractere
		m_pButtons[i]->setBoundKey( sz[0] ); // par exemple si i = 1 la touche du clavier sera la touche 1.
		m_pButtons[i]->setVisible( true ); // Le bouton est visible


		// Create the Team Info panel
		m_pTeamInfoPanel[i] = new TextPanel("", TEAMMENU_WINDOW_INFO_X, TEAMMENU_WINDOW_INFO_Y, TEAMMENU_WINDOW_SIZE_X - TEAMMENU_WINDOW_INFO_X, TEAMMENU_WINDOW_SIZE_X - TEAMMENU_WINDOW_INFO_Y );
		m_pTeamInfoPanel[i]->setParent( m_pTeamWindow );
		m_pTeamInfoPanel[i]->setFont( pSchemes->getFont(hTeamInfoText) );

		if (i==1) {
		m_pTeamInfoPanel[i]->setFgColor( iTeamColors[1][0], iTeamColors[1][1], iTeamColors[1][2], 0 );
		} 
		if (i==2) {
		m_pTeamInfoPanel[i]->setFgColor(  iTeamColors[2][0], iTeamColors[2][1], iTeamColors[2][2], 0 );
		} 
		if (i==3) {
		m_pTeamInfoPanel[i]->setFgColor(  iTeamColors[4][0], iTeamColors[4][1], iTeamColors[4][2], 0 );
		} 
		if (i==4) {
		m_pTeamInfoPanel[i]->setFgColor(  iTeamColors[3][0], iTeamColors[3][1], iTeamColors[3][2], 0 );
		}

		m_pTeamInfoPanel[i]->setBgColor( 0,0,0, 255 );
	}	

	int YPos = TEAMMENU_TOPLEFT_BUTTON_Y + ( (TEAMMENU_BUTTON_SIZE_Y + TEAMMENU_BUTTON_SPACER_Y) * 6 );
	// La position vertical du bouton annuler equivaut a la position du 6ème bouton
	// (le 5ème n'existe pas ca laisse un espace)

	// Create the Cancel button
	m_pCancelButton = new CommandButton( CHudTextMessage::BufferedLocaliseTextString( "Cancel" ), TEAMMENU_TOPLEFT_BUTTON_X, YPos, TEAMMENU_BUTTON_SIZE_X, TEAMMENU_BUTTON_SIZE_Y);
	m_pCancelButton->setParent( this );
	m_pCancelButton->addActionSignal( new CMenuHandler_TextWindow(HIDE_TEXTWINDOW) );
	m_pCancelButton->setVisible( true );
	m_pCancelButton->setBoundKey( '5' ); // par exemple si i = 1 la touche du clavier sera la touche 1.


// info player à gauche pour savoir dans kel team on va !

	m_pp = new Label("", 0,400);
	m_pp->setParent( this);
	m_pp->setBgColor( 50,50,50, 50 );
	m_pp->setVisible( false );

	Initialize();
}

//-----------------------------------------------------------------------------
// Purpose: Called each time a new level is started.
//-----------------------------------------------------------------------------
void CTeamMenuPanel::Initialize( void )
{
	m_bUpdatedMapName = false;
	m_iCurrentInfo = 0;
	m_pScrollPanel->setScrollValue( 0, 0 );
}

//-----------------------------------------------------------------------------
// Purpose: Called everytime the Team Menu is displayed
//-----------------------------------------------------------------------------
void CTeamMenuPanel::Update( void )
{


	int	 iYPos = TEAMMENU_TOPLEFT_BUTTON_Y;

	// Set the team buttons
	for (int i = 1; i <= 4; i++)
	{
		if (m_pButtons[i])
		{
				m_pButtons[i]->setVisible( true );

				m_pButtons[i]->setPos( TEAMMENU_TOPLEFT_BUTTON_X, iYPos ); // on replace les boutons pr etre sur
				iYPos += TEAMMENU_BUTTON_SIZE_Y + TEAMMENU_BUTTON_SPACER_Y;

				// Start with the first option up
				if (!m_iCurrentInfo)
					SetActiveInfo( i );

				char szPlayerList[ (MAX_PLAYER_NAME_LENGTH + 3) * 31 ];  // name + ", "
				strcpy(szPlayerList, "\n");
				// Update the Team Info
				// Now count the number of teammembers of this class
				int iTotal = 0;
				for ( int j = 1; j < MAX_PLAYERS; j++ )
				{
					if ( g_PlayerInfoList[j].name == NULL )
						continue; // empty player slot, skip
					if ( g_PlayerInfoList[j].thisplayer )
						continue; // skip this player
					
				//	if ( g_PlayerExtraInfo[j].teamnumber != i )
					//	continue; // skip over players in other teams

					if (i == 1) {
					if ( !strcmp( g_PlayerExtraInfo[j].teamname, "Macon[Ricard]" ) )
					continue;
					if ( !strcmp( g_PlayerExtraInfo[j].teamname, "Spectacteur" ) )
					continue;
					if ( !strcmp( g_PlayerExtraInfo[j].teamname, "Inspecteur" ) )
					continue;


					}
					if (i == 2) {
					if ( !strcmp( g_PlayerExtraInfo[j].teamname, "Macon[51]" ) )
					continue;
					if ( !strcmp( g_PlayerExtraInfo[j].teamname, "Spectacteur" ) )
					continue;
					if ( !strcmp( g_PlayerExtraInfo[j].teamname, "Inspecteur" ) )
					continue;
					}
					if (i == 3) {
					if ( !strcmp( g_PlayerExtraInfo[j].teamname, "Macon[51]" ) )
					continue;
					if ( !strcmp( g_PlayerExtraInfo[j].teamname, "Macon[Ricard]" ) )
					continue;
					if ( !strcmp( g_PlayerExtraInfo[j].teamname, "Spectacteur" ) )
					continue;
					
					}
					if (i == 4) {
					if ( !strcmp( g_PlayerExtraInfo[j].teamname, "Macon[51]" ) )
					continue;
					if ( !strcmp( g_PlayerExtraInfo[j].teamname, "Macon[Ricard]" ) )
					continue;
					if ( !strcmp( g_PlayerExtraInfo[j].teamname, "Inspecteur" ) )
					continue;
					}

int test;
test = 0;
					if ( !strcmp( g_PlayerExtraInfo[j].teamname, "Macon[51]" ) )
					test = 1;
					if ( !strcmp( g_PlayerExtraInfo[j].teamname, "Macon[Ricard]" ) )
					test = 1;
					if ( !strcmp( g_PlayerExtraInfo[j].teamname, "Inspecteur" ) )
					test = 1;
					if ( !strcmp( g_PlayerExtraInfo[j].teamname, "Spectacteur" ) )
					test = 1;


					if (test == 0)
					{
						strncpy( g_PlayerExtraInfo[j].teamname, "Spectacteur", MAX_TEAM_NAME );
					}


					iTotal++;
					if (iTotal > 1)
						strncat( szPlayerList, ", ", sizeof(szPlayerList) - strlen(szPlayerList) );
					strncat( szPlayerList, g_PlayerInfoList[j].name, sizeof(szPlayerList) - strlen(szPlayerList) );
					szPlayerList[ sizeof(szPlayerList) - 1 ] = '\0';
					test = 0;
				}

				if (iTotal > 0)
				{
					// Set the text of the info Panel
					char szText[ ((MAX_PLAYER_NAME_LENGTH + 3) * 31) + 256 ]; 
					if (iTotal == 1)
					{
						//sprintf(szText, "%s: %d Player (%d points)", gViewPort->GetTeamName(i), iTotal, g_TeamInfo[i].frags );
						if (i == 1) {
							sprintf(szText, "Macon 51: %d Player (%d points)",  iTotal, g_TeamInfo[i].frags );
						} if (i == 2) {
							sprintf(szText, "Macon Ricard: %d Player (%d points)",  iTotal, g_TeamInfo[i].frags );
						} if (i == 3) {
							sprintf(szText, "Inspecteur: %d Player (%d points)",  iTotal, g_TeamInfo[i].frags );
						} if (i == 4) {
							sprintf(szText, "Spectacteur: %d Player (%d points)",  iTotal, g_TeamInfo[i].frags );
						}
					}
					else
					{
						if (i == 1) {
							sprintf(szText, "Macon 51: %d Players (%d points)",  iTotal, g_TeamInfo[i].frags );
						} if (i == 2) {
							sprintf(szText, "Macon Ricard: %d Players (%d points)",  iTotal, g_TeamInfo[i].frags );
						} if (i == 3) {
							sprintf(szText, "Inspecteur: %d Players (%d points)",  iTotal, g_TeamInfo[i].frags );
						} if (i == 4) {
							sprintf(szText, "Spectacteur: %d Players (%d points)",  iTotal, g_TeamInfo[i].frags );
						}
					}
					strncat( szText, szPlayerList, sizeof(szText) - strlen(szText) );
					szText[ sizeof(szText) - 1 ] = '\0';

					m_pTeamInfoPanel[i]->setText( szText );
				}
				else
				{
					m_pTeamInfoPanel[i]->setText( "" );
				}
		}
	}

	// If the player is already in a team, make the cancel button visible
	if ( gHUD.m_iTeam )
	{
		m_pCancelButton->setPos( TEAMMENU_TOPLEFT_BUTTON_X, iYPos );
		iYPos += TEAMMENU_BUTTON_SIZE_Y + TEAMMENU_BUTTON_SPACER_Y;
		m_pCancelButton->setVisible( true );

	// planque le bouton de l'equipe ou on est pour eviter un respawn
	//gEngfuncs.Con_DPrintf( "team : %s - %i\n",gViewPort->GetTeamName(gHUD.m_iTeam) ,g_iTeamNumber); // equivalent de ALERT mais coté client

		
		
		
//007 : bonne idée malheureusement des fois ca plante ... j'ai donc supprimé



/*	if ( !strcmp( gViewPort->GetTeamName(g_iTeamNumber+1), "Inspecteur" ) )
	{
	m_pButtons[1]->setVisible( true ); // 51
	m_pButtons[2]->setVisible( true ); // ricard
	m_pButtons[3]->setVisible( false ); // inspecteur
	m_pButtons[4]->setVisible( true ); // spectateur
	}

	if ( !strcmp( gViewPort->GetTeamName(g_iTeamNumber+1), "Macon[51]" ) )
	{
	m_pButtons[1]->setVisible( false ); // 51
	m_pButtons[2]->setVisible( true ); // ricard
	m_pButtons[3]->setVisible( true ); // inspecteur
	m_pButtons[4]->setVisible( true ); // spectateur
	}

		if ( !strcmp( gViewPort->GetTeamName(g_iTeamNumber+1), "Macon[Ricard]" ) )
	{
	m_pButtons[1]->setVisible( true ); // 51
	m_pButtons[2]->setVisible( false ); // ricard
	m_pButtons[3]->setVisible( true ); // inspecteur
	m_pButtons[4]->setVisible( true ); // spectateur
	}

	}
	else
	{
		m_pCancelButton->setVisible( false );
	}

*/




if ( !strcmp( gViewPort->GetTeamName(g_iTeamNumber+1), "Inspecteur" ) ||  !strcmp( gViewPort->GetTeamName(g_iTeamNumber+1), "Macon[51]" ) || !strcmp( gViewPort->GetTeamName(g_iTeamNumber+1), "Macon[Ricard]" ))
	{
	m_pButtons[1]->setVisible( true ); // 51
	m_pButtons[2]->setVisible( true ); // ricard
	m_pButtons[3]->setVisible( true ); // inspecteur
	m_pButtons[4]->setVisible( true ); // spectateur
	m_pCancelButton->setVisible( true ); // cancel
	}



	}











	// Set the Map Title
	if (!m_bUpdatedMapName)
	{
		const char *level = gEngfuncs.pfnGetLevelName();
		if (level && level[0])
		{
			char sz[256]; 
			char szTitle[256]; 
			char *ch;

			// Update the level name
			strcpy( sz, level );
			ch = strchr( sz, '/' );
			if (!ch)
				ch = strchr( sz, '\\' );
			strcpy( szTitle, ch+1 );
			ch = strchr( szTitle, '.' );
			*ch = '\0';
			m_pMapTitle->setText( szTitle );
			*ch = '.';

			// Update the map briefing
			strcpy( sz, level );
			ch = strchr( sz, '.' );
			*ch = '\0';
			strcat( sz, ".txt" );
			char *pfile = (char*)gEngfuncs.COM_LoadFile( sz, 5, NULL );
			if (pfile)
			{
				m_pBriefing->setText( pfile );

				// Get the total size of the Briefing text and resize the text panel
				int iXSize, iYSize;
				m_pBriefing->getTextImage()->getTextSize( iXSize, iYSize );
				m_pBriefing->setSize( iXSize, iYSize );
			}

			m_bUpdatedMapName = true;
		}
	}

// update info team & player



	m_pScrollPanel->validate();
}

//=====================================
// Key inputs
bool CTeamMenuPanel::SlotInput( int iSlot )
{


	if ( iSlot == 1)
	{
		m_pButtons[1]->fireActionSignal();
	return true;
	}

	if ( iSlot == 2)
	{
	m_pButtons[2]->fireActionSignal();
	return true;
	}

	if ( iSlot == 3)
	{
	m_pButtons[3]->fireActionSignal();
	return true;
	}

	if ( iSlot == 4)
	{
	m_pButtons[4]->fireActionSignal();
	return true;
	}

	// Annuler
	if ( iSlot == 5)
	{
		m_pCancelButton->fireActionSignal();
		return true;
	}

	// Otherwise, see if a particular team is selectable
	if ( (iSlot < 1) || (iSlot > gViewPort->GetNumberOfTeams()) )
		return false;
	if ( !m_pButtons[ iSlot ] )
		return false;

	// Is the button pushable?
	if ( m_pButtons[ iSlot ]->isVisible() )
	{
		m_pButtons[ iSlot ]->fireActionSignal();
		return true;
	}

	return false;
}

//======================================
// Update the Team menu before opening it
void CTeamMenuPanel::Open( void )
{
	Update();
	CMenuPanel::Open();
}

void CTeamMenuPanel::paintBackground()
{
	// make sure we get the map briefing up
	if ( !m_bUpdatedMapName )
		Update();

	CMenuPanel::paintBackground();
}

//======================================
// Mouse is over a team button, bring up the class info
void CTeamMenuPanel::SetActiveInfo( int iInput )
{
	// Remove all the Info panels and bring up the specified one
	m_pCancelButton->setArmed( false );
	for (int i = 1; i <= 4; i++)
	{
		m_pButtons[i]->setArmed( false );
		m_pTeamInfoPanel[i]->setVisible( false );
	}

	// 6 is Spectate
	if (iInput == 5)
	{
		m_pCancelButton->setArmed( true );
	}
	else
	{
		m_pButtons[iInput]->setArmed( true );
		m_pTeamInfoPanel[iInput]->setVisible( true );
	}

	m_iCurrentInfo = iInput;

	m_pScrollPanel->validate();
}


