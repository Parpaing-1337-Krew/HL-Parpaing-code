#include "hud.h" 
#include "cl_util.h"
#include <string.h>
#include <stdio.h>
#include "parsemsg.h" 


//hop met le macon chaos . niark



DECLARE_MESSAGE(m_Chaos, Chaos )

int CHudChaos::Init(void)
{

HOOK_MESSAGE( Chaos );

return 1;

};

int CHudChaos::VidInit(void) 
{
return 1;

};




int CHudChaos::MsgFunc_Chaos(const char *pszName, int iSize, void *pbuf)
{ 

BEGIN_READ( pbuf, iSize );
//m_iChaos = READ_BYTE();

			gEngfuncs.Con_DPrintf( "CHAOS\n");
/*if (m_iChaos==1)
{

}*/
//gHUD.Alcool = m_iChaos;
return 1;

}