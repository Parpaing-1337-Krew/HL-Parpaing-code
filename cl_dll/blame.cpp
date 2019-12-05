#include "hud.h"
#include "cl_util.h"
#include "parsemsg.h"
#include <string.h>
#include <stdio.h>

DECLARE_MESSAGE ( m_Blame, Blame);
DECLARE_MESSAGE ( m_Carnet, Carnet);

int CHudBlame::Init ( void )
{

// Ne pas oublier le HOOK_MESSAGE pour intercepter les
// messages envoyé par le serveur
HOOK_MESSAGE ( Blame );
m_iFlags |= HUD_ACTIVE;
gHUD.AddHudElem ( this );
return 1;

};
void CHudBlame::Reset ( void )
{
};

int CHudBlame::VidInit ( void )
{

int HUD_chiffre_0 = gHUD.GetSpriteIndex ( "number_0" );
//m_prc_0 = &gHUD.GetSpriteRect ( HUD_chiffre_0 );

m_Spr = SPR_Load ( "sprites/blame.spr" );

m_prcSprite.top = 16;
m_prcSprite.left = 0;
m_prcSprite.bottom = 0;
m_prcSprite.right = 16;
return 1;

};

int CHudBlame::MsgFunc_Blame ( const char *pszName, int iSize, void *pbuf )
{

BEGIN_READ ( pbuf, iSize );
m_iAction = READ_BYTE();

	if (m_iAction==1)
	{
		m_iFlags |= HUD_ACTIVE;
		m_iFade=255;
	}

return 1;

}

int CHudBlame::Draw ( float flTime )
{
/*	int r, g, b;
	UnpackRGB(r,g,b, RGB_YELLOWISH);
	ScaleColors( r, g, b, m_iFade );

	SPR_Set ( m_Spr, r, g, b);
	SPR_DrawAdditive (0, (ScreenWidth - 128) /2, (ScreenHeight - 128)/2, &m_prcSprite );

	m_iFade--;

	if (m_iFade==0)
	m_iFlags &= ~HUD_ACTIVE;

*/
return 1;

}



int CHudCarnet::Init ( void )
{

// Ne pas oublier le HOOK_MESSAGE pour intercepter les
// messages envoyé par le serveur
HOOK_MESSAGE ( Carnet );
m_iFlags |= HUD_ACTIVE;
gHUD.AddHudElem ( this );
return 1;

};
void CHudCarnet::Reset ( void )
{
};

int CHudCarnet::VidInit ( void )
{

int HUD_chiffre_0 = gHUD.GetSpriteIndex ( "number_0" );
//m_prc_0 = &gHUD.GetSpriteRect ( HUD_chiffre_0 );

m_Spr = SPR_Load ( "sprites/qblame.spr" );

m_prcSprite.top = 16;
m_prcSprite.left = 0;
m_prcSprite.bottom = 0;
m_prcSprite.right = 16;
return 1;

};

int CHudCarnet::MsgFunc_Carnet ( const char *pszName, int iSize, void *pbuf )
{

BEGIN_READ ( pbuf, iSize );
m_iAction = READ_BYTE();

	if (m_iAction==1)
	{
		m_iFlags |= HUD_ACTIVE;
		m_iStart=-1;
	}

return 1;

}

int CHudCarnet::Draw ( float flTime )
{
	if (m_iStart==-1)
	m_iStart=flTime;

	int r, g, b;
	UnpackRGB(r,g,b, RGB_YELLOWISH);
	ScaleColors( r, g, b, 200 );

	int m_iStatus = (128 * (flTime-m_iStart) )/3;
	FillRGBA((ScreenWidth - 128) /2, (ScreenHeight - 128)/2 +128,m_iStatus, 10, r, g, b, 200); // barre réel moins transparente

	SPR_Set ( m_Spr, r, g, b);
	SPR_DrawAdditive (0, (ScreenWidth - 128) /2, (ScreenHeight - 128)/2, &m_prcSprite );


	if (flTime> m_iStart+3)
	m_iFlags &= ~HUD_ACTIVE;


return 1;

}