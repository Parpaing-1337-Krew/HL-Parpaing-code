		  
			
			  
				
				  
#include	"extdll.h"
#include	"util.h"
#include	"cbase.h"
#include	"monsters.h"
#include	"weapons.h"
#include	"nodes.h"
#include	"player.h"
#include	"gamerules.h"
#include	"decals.h"

#include	"special_parpaing.h"

LINK_ENTITY_TO_CLASS( weapon_iceparpaing, CIceParpaing );

void CIceParpaing::Spawn()
{
	CParpaing::Spawn();
	pev->renderfx = kRenderFxGlowShell;
	pev->renderamt = 20;
	pev->rendercolor = Vector( 0, 0, 255 );	// RGB
}

void CIceParpaing::SecondaryAttack()
{
#ifndef CLIENT_DLL

	CParpaing::CreateSpecialParpaing(ICE);
	m_pPlayer->m_iHasParpaing = 0;
	m_flNextSecondaryAttack = 3;
	m_pPlayer->pev->weapons &= ~(1<<16);
	DestroyItem();
	g_pGameRules->GetNextBestWeapon( m_pPlayer, this );

#endif
}
