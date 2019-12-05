/* 
HL-parpaing files...
Keep da ParpAing R00ts 'n nique le front national
	//BLP Kessafoula ce message ? ;>
*/








#include "hud.h" 
#include "cl_util.h"
#include <string.h>
#include <stdio.h>
#include "parsemsg.h" 

DECLARE_MESSAGE(m_BarArea, Bar )

int CHudBarArea::Init(void)
{

HOOK_MESSAGE( Bar );
m_iFlags = 0;
gHUD.AddHudElem(this);
return 1;

};

int CHudBarArea::VidInit(void) 
{

m_hSprite = LoadSprite("sprites/%bar.spr");//Le nom du sprite
return 1;

};


int CHudBarArea::Draw(float fTime)
{ 

if (m_iPos) 
{

int r, g, b, x, y; 
UnpackRGB(r,g,b, RGB_YELLOWISH);
SPR_Set(m_hSprite, r, g, b );
y = ScreenHeight/3 - SPR_Height(m_hSprite,0) - gHUD.m_iFontHeight;
x = SPR_Width(m_hSprite,0)/25; //Ici on choisit a quel endroit de l'ecran on affiche le sprite, x représente l'abscisse et y l'ordonnée.
SPR_DrawAdditive( m_iPos - 1, x, y, NULL); 

}
return 1;

}

int CHudBarArea::MsgFunc_Bar(const char *pszName, int iSize, void *pbuf)
{ 

BEGIN_READ( pbuf, iSize );
m_iPos = READ_BYTE();
if (m_iPos) 
m_iFlags |= HUD_ACTIVE; //On affiche la partie Bar du HUD 
else 
m_iFlags &= ~HUD_ACTIVE; //On masque la partie Bar du hud 
return 1;

}

