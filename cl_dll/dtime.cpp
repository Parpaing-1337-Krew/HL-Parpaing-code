#include "hud.h"
#include "cl_util.h"
#include "parsemsg.h"
#include "vgui_TeamFortressViewport.h"
#include "vgui_MOTD.h"
#include <string.h>
#include <stdio.h>

DECLARE_MESSAGE ( m_Time, Time);
int CHudTime::Init ( void )
{

// Ne pas oublier le HOOK_MESSAGE pour intercepter les
// messages envoyé par le serveur
HOOK_MESSAGE ( Time );
m_iFlags |= HUD_ACTIVE;
gHUD.AddHudElem ( this );
return 1;

};
void CHudTime::Reset ( void )
{
};

int CHudTime::VidInit ( void )
{

int HUD_chiffre_0 = gHUD.GetSpriteIndex ( "number_0" );
m_prc_0 = &gHUD.GetSpriteRect ( HUD_chiffre_0 );
m_hSprite = SPR_Load ( "sprites/2points.spr" );
m_prcSprite.top = 16;
m_prcSprite.left = 0;
m_prcSprite.bottom = 0;
m_prcSprite.right = 16;
return 1;

};

int CHudTime::MsgFunc_Time ( const char *pszName, int iSize, void *pbuf )
{

BEGIN_READ ( pbuf, iSize );
	int buff;
	buff = READ_SHORT();

	if (buff==1)
	gHUD.m_lTime = READ_COORD();

	if (buff==2)
	{
	gHUD.m_i51		= READ_BYTE();
	gHUD.m_iRicard = READ_BYTE();
	}

	if (buff==3)
	gHUD.m_iTeam = READ_BYTE();
	
	if (buff==4)
	{
	gHUD.nbmacon = READ_BYTE();
	gHUD.nbblame = READ_BYTE();
	}

	if (buff==5)
	{
		gHUD.Vguimsg = READ_STRING();
	}


	gViewPort->m_pMapMenu->Update();
return 1;

}

#define MOTD_HBAR_X						XRES(545)
#define MOTD_HBAR_W						XRES(95)

#define MOTD_WINDOW_X				XRES(500)
#define MOTD_WINDOW_WI				XRES(140)
#define MOTD_WINDOW_Y				YRES(0)
#define MOTD_WINDOW_SIZE_X			XRES(640)
#define MOTD_WINDOW_SIZE_Y			YRES(50)
#define MOTD_TAB_DIFF					YRES(6)


int CHudTime::Draw(float flTime)
{

	//FillRGBA(MOTD_WINDOW_X, 0, MOTD_WINDOW_WI ,MOTD_WINDOW_SIZE_Y, 0,255, 0, 230);
if (gHUD.m_iTeam == 1 || gHUD.m_iTeam==2)
{
	int Av;
	Av = (MOTD_HBAR_W /100) * gHUD.m_i51;

	FillRGBA(MOTD_HBAR_X, MOTD_51_SIZE_Y + MOTD_TAB_DIFF, Av ,MOTD_TAB_SIZE, 0, 0, 255, 255);

	Av = (MOTD_HBAR_W /100) * gHUD.m_iRicard;

	FillRGBA(MOTD_HBAR_X, MOTD_RICARD_SIZE_Y + MOTD_TAB_DIFF, Av ,MOTD_TAB_SIZE, 255, 0, 0, 255);
} else if (gHUD.m_iTeam==3) {

/*	char blame[30];
	sprintf(blame, "Blame : %i/%i", gHUD.nbblame,gHUD.nbmacon  );*/


}
	/*int Ahealth;
	Ahealth = (MOTD_HBAR_W /100) * m_iHealth;
	if (m_iHealth<=20)
	FillRGBA(MOTD_HBAR_X, 43, Ahealth , 12, 254, 84, 44, 255);
	else
	FillRGBA(MOTD_HBAR_X, 43, Ahealth , 12, 255, 255, 255, 100);*/

return 1;


/*gHUD.m_lTime = m_iTimeRemaining;
int r, g, b, x, y, tx,tt=0, a = 100;
int m_iMin , m_iSec;

m_iMin = m_iTimeRemaining / 60;
m_iSec = m_iTimeRemaining % 60;

if (m_iMin==0 && m_iSec<31)
	
	{
		r = 96; //couleur plus sympa (douanier)
		g = 0;
		b = 0;
		a = 200;
} else {
UnpackRGB(r,g,b, RGB_YELLOWISH);
}
ScaleColors(r, g, b, a);
x = ScreenWidth/5;
y = ScreenHeight - gHUD.m_iFontHeight - gHUD.m_iFontHeight / 2;


//hlp
	if ( (gHUD.m_iHideHUDDisplay & HIDEHUD_HEALTH) || gEngfuncs.IsSpectateOnly() )
	{
	int poum = ScreenHeight - ( gHUD.m_iFontHeight +20);
	FillRGBA(0, poum,ScreenWidth, ScreenHeight - poum, r, g, b, 80);
	FillRGBA(0, poum-1,ScreenWidth, 1, r, g, b, 255);
	gHUD.DrawHudString( (ScreenWidth - 20*5)/2, y, 600, "Mode Spectateur", 255, 255, 255 );

	x=10; // on colle le tps à droite en mode specateur !

	} 



if (m_iTimeRemaining==-1)
{
gHUD.DrawHudString( x, y, 600, "ROUND FINI", 255, 255, 255 );
} else {
//min
if (m_iMin<10) {
gHUD.DrawHudNumber ( x, y, DHN_DRAWZERO, 0, r, g, b );
tx=m_prc_0->right - m_prc_0->left;
}
gHUD.DrawHudNumber ( x+tx, y, DHN_DRAWZERO, m_iMin, r, g, b );

// :
SPR_Set ( m_hSprite, r, g, b);
SPR_DrawAdditive (0, x + 34, y, &m_prcSprite );
//sec
if (m_iSec<10) {
gHUD.DrawHudNumber ( x+tx*2+10, y, DHN_DRAWZERO, 0, r, g, b );
tt =m_prc_0->right - m_prc_0->left;
}
gHUD.DrawHudNumber ( x+tx*2+tt+10, y, DHN_DRAWZERO, m_iSec, r, g, b );


}
	

return 1;
*/
}

