#include "hud.h" 
#include "cl_util.h"
#include <string.h>
#include <stdio.h>
#include "parsemsg.h" 








DECLARE_MESSAGE(m_Defuse, Defuse )
int CHudDefuse::Init(void)
{
HOOK_MESSAGE( Defuse );
m_iFlags = 0;
gHUD.AddHudElem(this);
return 1;
};
int CHudDefuse::VidInit(void) 
{
return 1;
};

int CHudDefuse::MsgFunc_Defuse(const char *pszName, int iSize, void *pbuf)
{ 
BEGIN_READ( pbuf, iSize );
Defuse = READ_BYTE();
m_iFlags |= HUD_ACTIVE;

gHUD.Defuse = Defuse;

Time=gHUD.m_flTime;

return 1;
}

int CHudDefuse::Draw(float flTime)
{
if (Defuse ==0)
return 1;

int r, g, b, a=256,longueur;
UnpackRGB(r,g,b, 0x00FFA000);
int i = ScreenWidth*0.153;//0.15 en théorie
longueur = (gHUD.m_flTime-Time)*i*0.1;


	FillRGBA(
		ScreenWidth*0.5-ScreenWidth*0.08,
		ScreenHeight*0.5-ScreenHeight*0.01,
		ScreenWidth*0.16,
		ScreenHeight*0.02,
		r, g, b, a*0.3);
UnpackRGB(r,g,b, 0x00FFA000);
	FillRGBA(
		ScreenWidth*0.5-ScreenWidth*0.075,
		ScreenHeight*0.5-ScreenHeight*0.005,
		longueur,
		ScreenHeight*0.01,
		r, g, b, a*0.2);

if (gHUD.m_flTime-Time>=10)
{Defuse = 0;}


return 1;
}