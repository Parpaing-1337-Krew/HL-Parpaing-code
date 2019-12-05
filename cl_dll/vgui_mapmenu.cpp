#include "VGUI_Font.h"
#include "VGUI_ScrollPanel.h"
#include "VGUI_TextImage.h"

#include<VGUI_StackLayout.h>

#include "hud.h"
#include "cl_util.h"
#include "camera.h"
#include "kbutton.h"
#include "const.h"

#include "vgui_int.h"
#include "vgui_TeamFortressViewport.h"
#include "vgui_ServerBrowser.h"
#include "vgui_MOTD.h"


CMapMenuPanel::CMapMenuPanel(int iTrans, int iRemoveMe,int x, int y, int wide, int tall) : CMenuPanel(iTrans, iRemoveMe, x,y,wide,tall)
{
	// Get the scheme used for the Titles
	CSchemeManager *pSchemes = gViewPort->GetSchemeManager();

	// schemes
	SchemeHandle_t hTitleScheme = pSchemes->getSchemeHandle( "Infopanel" );
	SchemeHandle_t hMOTDText = pSchemes->getSchemeHandle( "CommandMenu Text" );

	int r ,g , b , a;

	//////////////////////////////////////////////////////////////////////////////////////////
	// infos jeu
	//////////////////////////////////////////////////////////////////////////////////////////


	m_pBackgroundPanel = new CTransparentPanel( 0,MOTD_WINDOW_X, MOTD_WINDOW_Y, MOTD_WINDOW_SIZE_X, MOTD_WINDOW_SIZE_Y );
	m_pBackgroundPanel->setParent( this );
	m_pBackgroundPanel->setBorder( new LineBorder( Color(255 * 0.7,170 * 0.7,0,0)) );
	pSchemes->getBgColor( hTitleScheme, r, g, b, a );
	m_pBackgroundPanel->setBgColor( r, g, b, a );
	m_pBackgroundPanel->setBgColor(100, 100, 100 ,100);
	m_pBackgroundPanel->setVisible( true );

	/*char img[256];
	sprintf(img, "%d_.tga", m_xRes, m_pSchemeList[i].schemeName);*/


	/*m_pImage =  new CImageLabel("infosfond",MOTD_WINDOW_X,0,MOTD_WINDOW_SIZE_X,MOTD_WINDOW_SIZE_Y);
	m_pImage->setParent( this );
	m_pImage->setContentFitted(false);
	m_pImage->setSize(0,MOTD_WINDOW_SIZE_Y);
	m_pImage->setVisible( true );*/

	pFond = new Label( "",MOTD_WINDOW_X, 0 );
	pFond->setParent( this );
	pFond->setSize(MOTD_WINDOW_SIZE_X,MOTD_WINDOW_SIZE_Y);
	//pTimeLabel->setFont( pSchemes->getFont(hTitleScheme) );
	pFond->setBgColor( 50,50,50,100 );
	pFond->setVisible( true );

	pTimeLabel = new Label( "",MOTD_WINDOW_X, MOTD_TPS_SIZE_Y );
	pTimeLabel->setParent( this );
	//pTimeLabel->setFont( pSchemes->getFont(hTitleScheme) );
	pTimeLabel->setBgColor( 0,0,0, 255 );
	pTimeLabel->setVisible( true );

	/*pHLabel = new Label( "",MOTD_WINDOW_X, 18 );
	pHLabel->setParent( this );
	pHLabel->setFont( pSchemes->getFont(hMOTDText) );
	pHLabel->setVisible( true );*/

	p51 = new Label( "",MOTD_WINDOW_X, MOTD_51_SIZE_Y);
	p51->setParent( this );
	p51->setBgColor( 0,0,0, 255 );
	p51->setText("51 :");
	p51->setFont( pSchemes->getFont(hTitleScheme) );
	p51->setVisible( false );

	pRicard = new Label( "",MOTD_WINDOW_X, MOTD_RICARD_SIZE_Y );
	pRicard->setParent( this );
	pRicard->setBgColor( 0,0,0, 255 );
	pRicard->setText("Ricard :");
	pRicard->setFont( pSchemes->getFont(hTitleScheme) );
	pRicard->setVisible( false );


	pInsLabel = new Label( "",MOTD_WINDOW_X, MOTD_51_SIZE_Y);
	pInsLabel->setParent( this );
	pInsLabel->setBgColor( 0,0,0, 255 );
	pInsLabel->setText("Blame :");
	pInsLabel->setFont( pSchemes->getFont(hTitleScheme) );
	pInsLabel->setVisible( false );


	//////////////////////////////////////////////////////////////////////////////////////////
	// infos diverses
	//////////////////////////////////////////////////////////////////////////////////////////


	m_pInfos = new CTransparentPanel( 0,0, ScreenHeight -20, ScreenWidth, 20 );
	m_pInfos->setParent( this );
	m_pInfos->setBorder( new LineBorder( Color(255 * 0.7,170 * 0.7,0,0)) );
	pSchemes->getBgColor( hTitleScheme, r, g, b, a );
	m_pInfos->setBgColor( r, g, b, a );
	m_pInfos->setBgColor(100, 100, 100 ,100);
	m_pInfos->setVisible( false );

	pFond2 = new Label( "",0, ScreenHeight -20 );
	pFond2->setParent( this );
	pFond2->setSize(ScreenWidth,20);
	pFond2->setBgColor( 255,255,255,0 );
	pFond2->setVisible( false );

	pInfos = new Label( "",0, ScreenHeight -20);
	pInfos->setParent( this );
	pInfos->setBgColor( 0,0,0, 255 );
	pInfos->setFont( pSchemes->getFont(hTitleScheme) );
	pInfos->setVisible( false );




Initialize();

}
//-----------------------------------------------------------------------------
// Purpose: Called each time a new level is started.
//-----------------------------------------------------------------------------
void CMapMenuPanel::Initialize( void )
{
	pTimeLabel->setText("");
	//pHLabel->setText("");

// TODO: Clear out all the values here

}
void CMapMenuPanel::setSize(int wide,int tall)
{
Panel::setSize(wide,tall);
}
void CMapMenuPanel::Update()
{

	int m_iMin = gHUD.m_lTime / 60;
	int m_iSec = gHUD.m_lTime;
	m_iSec = m_iSec %60;

	char time[10];
	if (m_iSec<10)
	sprintf(time, "%i:0%i", m_iMin,m_iSec );
		else
	sprintf(time, "%i:%i", m_iMin,m_iSec );

	pTimeLabel->setText(time);

	if (gHUD.Vguimsg)
	{
	char msg[200];
	sprintf(msg, "%s", gHUD.Vguimsg  );
	m_pInfos->setVisible (true);
	pInfos->setVisible (true);
	pFond2->setVisible (true);
	pInfos->setText(msg);
	gHUD.m_flLastmsg = gHUD.m_flTime + 1.0;
	} else if (gHUD.m_flTime>gHUD.m_flLastmsg)
	{
	m_pInfos->setVisible (false);
	pInfos->setVisible (false);
	pFond2->setVisible (false);	
	}

	char blame[30];
	sprintf(blame, "Blame : %i/%i", gHUD.nbblame,gHUD.nbmacon  );



if (gHUD.m_iTeam == 1 || gHUD.m_iTeam==2)
{	
	p51->setVisible( true );
	pRicard->setVisible( true );
	pInsLabel->setVisible( false );

} else if (gHUD.m_iTeam == 3){

	p51->setVisible( false );
	pRicard->setVisible( false );
	pInsLabel->setVisible( true );
	pInsLabel->setText(blame);


} else {

	p51->setVisible( false );
	pRicard->setVisible( false );
	pInsLabel->setVisible( false );

}


	/*char health[10];
	sprintf(health, "%i %%", gHUD.m_Health.m_iHealth );
	pHLabel->setText(health);*/

	/*char m51[20];
	if (!gHUD.m_i51)
	gHUD.m_i51=0;
	sprintf(m51, "51 : %i %%", gHUD.m_i51 );
	p51->setText(m51);

	char mRicard[20];
	if (!gHUD.m_iRicard)
	gHUD.m_iRicard=0;

	sprintf(mRicard, "Ricard : %i %%", gHUD.m_iRicard );
	pRicard->setText(mRicard);*/



}
void CMapMenuPanel::RebuildData()
{
Update();
}
