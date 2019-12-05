#include "hud.h" 
#include "cl_util.h"
#include <string.h>
#include <stdio.h>
#include "parsemsg.h" 


//update la quantité d'alcool que le maçon a dans le sang en % de la dose maxi (10g/L pour un gros maçon a peu pres -_-)



DECLARE_MESSAGE(m_Alcool, Alcool )

int CHudAlcool::Init(void)
{

HOOK_MESSAGE( Alcool );

return 1;

};

int CHudAlcool::VidInit(void) 
{
return 1;

};




int CHudAlcool::MsgFunc_Alcool(const char *pszName, int iSize, void *pbuf)
{ 

BEGIN_READ( pbuf, iSize );
m_iAlcool = READ_SHORT();
gHUD.Alcool = m_iAlcool;
return 1;

}

