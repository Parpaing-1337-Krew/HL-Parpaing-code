#include "hud.h" 
#include "cl_util.h"
#include <string.h>
#include <stdio.h>
#include "parsemsg.h" 

DECLARE_MESSAGE(m_Skill, Skill )

int CHudSkill::Init(void)
{

HOOK_MESSAGE( Skill );
m_iFlags = 0;
gHUD.AddHudElem(this);
return 1;

};

int CHudSkill::VidInit(void) 
{

m_hSprite = LoadSprite("sprites/%bar.spr");
return 1;

};


int CHudSkill::Draw(float fTime)
{ 


if (Skill <= 0)
return 1;

int SpacerX = ScreenWidth/12;
int k = ScreenHeight/20;

int SpacerY[] = 
{
	1,
	0,			
	1,
	0,
	1,
	0,
	1,
	0,
	1,
	0,
	1,
	0,
	1,
	0,
	1,
	0,
	1,
	0
}; //c moyen comme code, mais ca marche -_-


int i;
for (i = 0 ; i < Skill ; i++)
{
int r, g, b, x, y; 
UnpackRGB(r,g,b, RGB_YELLOWISH);
SPR_Set(m_hSprite, r, g, b );

x = ScreenWidth /6 + i * SpacerX ;
y = ScreenHeight /1.3 + SpacerY[i] * k;

SPR_DrawAdditive(0, x, y, NULL); 
}




return 1;

}

int CHudSkill::MsgFunc_Skill(const char *pszName, int iSize, void *pbuf)
{ 

BEGIN_READ( pbuf, iSize );
Skill = READ_SHORT();


if (Skill !=0) 
m_iFlags |= HUD_ACTIVE; 
else 
m_iFlags &= ~HUD_ACTIVE; 
return 1;

}

