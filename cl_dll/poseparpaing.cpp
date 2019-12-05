#include "hud.h"
#include "cl_util.h"
#include "parsemsg.h"
#include <string.h>
#include <stdio.h>

DECLARE_MESSAGE ( m_Parpaing, Parpaing);
int CHudParpaing::Init ( void )
{

// Ne pas oublier le HOOK_MESSAGE pour intercepter les
// messages envoyé par le serveur
HOOK_MESSAGE ( Parpaing );
m_iFlags |= HUD_ACTIVE;
gHUD.AddHudElem ( this );
return 1;

};
void CHudParpaing::Reset ( void )
{
};

int CHudParpaing::VidInit ( void )
{

int HUD_chiffre_0 = gHUD.GetSpriteIndex ( "number_0" );
m_prc_0 = &gHUD.GetSpriteRect ( HUD_chiffre_0 );

m_SprP1 = SPR_Load ( "sprites/pose_parpaing.spr" );

m_prcSprite.top = 16;
m_prcSprite.left = 0;
m_prcSprite.bottom = 0;
m_prcSprite.right = 16;
return 1;

};

int CHudParpaing::MsgFunc_Parpaing ( const char *pszName, int iSize, void *pbuf )
{

BEGIN_READ ( pbuf, iSize );
m_iAction = READ_SHORT();

	if (m_iAction==1)
	{
		m_iFlags |= HUD_ACTIVE;
		m_iStatus=0;
		m_iStart=0;

	}
	if (m_iAction==3)
	{
		m_iFlags &= ~HUD_ACTIVE;
		m_iStatus=0;
		m_iStart=0;

	}

return 1;

}

int CHudParpaing::Draw ( float flTime )
{

		if (!m_iStart || m_iStart==0)
	m_iStart = flTime;

	int tps = (int)flTime - m_iStart;

if (m_iAction==1)
{
	int r, g, b,anim/*, x, y*/;
	//gEngfuncs.Con_DPrintf( "%i\n",tps); // equivalent de ALERT mais coté client
	UnpackRGB(r,g,b, RGB_YELLOWISH);


	ScaleColors( r, g, b, 255 );
	//m_iStatus++;
	//FillRGBA(10, 10,270, 13, r, g, b, 110); // barre max plus transparente
	FillRGBA(10, 10,m_iStatus, 13, r, g, b, 200); // barre réel moins transparente

	if (tps==0)
		anim=0;

	if (tps==1)
		anim=1;
	
	if (tps==2)
		anim=2;

	SPR_Set ( m_SprP1, r, g, b);
	SPR_DrawAdditive (anim, ScreenWidth - 256 -100, -50, &m_prcSprite );

} else {

	if (tps!=0 && tps<7)
	{
	int r, g, b/*, x, y*/;
	UnpackRGB(r,g,b, RGB_YELLOWISH);
	ScaleColors( r, g, b, 255 );
	SPR_Set ( m_SprP1, r, g, b);
	SPR_DrawAdditive (3, ScreenWidth - 256 -100, -50, &m_prcSprite );
	} else {
	m_iFlags &= ~HUD_ACTIVE;
	m_iStatus=0;
	m_iStart=0;
	}

}
return 1;

}