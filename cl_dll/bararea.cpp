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
m_iSens=1;
m_iFade=0;
return 1;

};


int CHudBarArea::Draw(float fTime)
{ 

int r, g, b, x, y; 
x=50;
UnpackRGB(r,g,b, RGB_YELLOWISH);

if (m_iSens==1)
m_iFade+=3;
if (m_iSens==2)
m_iFade-=3;

if (m_iFade>=255)
m_iSens=2;
if (m_iFade<=0)
m_iSens=1;

ScaleColors( r, g, b, m_iFade );
SPR_Set(m_hSprite, r, g, b );
y = ScreenHeight/2 - SPR_Height(m_hSprite,0) - gHUD.m_iFontHeight;
SPR_DrawAdditive(0, x, y, NULL); 

if (test!=1 && m_iFade<=20) 
m_iFlags &= ~HUD_ACTIVE; //On masque la partie reload du hud 
//gEngfuncs.Con_DPrintf( "%i-%i\n",m_iSens,m_iFade); // equivalent de ALERT mais coté client

return 1;

}

int CHudBarArea::MsgFunc_Bar(const char *pszName, int iSize, void *pbuf)
{ 

BEGIN_READ( pbuf, iSize );
test = READ_BYTE();
if (test) 
m_iFlags |= HUD_ACTIVE; //On affiche la partie reload du HUD 

return 1;

}

