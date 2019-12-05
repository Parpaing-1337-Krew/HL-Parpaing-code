// code pour le sifflet
// commencer le 05/06/02 par madfab

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "monsters.h"
#include "weapons.h"
#include "nodes.h"
#include "player.h"
#include "gamerules.h"


//LINK_ENTITY_TO_CLASS( weapon_sifflet, CSifflet );



enum gauss_e {
	SI_IDLE = 0,
	SI_ATTACK,
};


void CSifflet::Spawn( )
{
	Precache( );
	m_iId = WEAPON_SIFFLET;
	SET_MODEL(ENT(pev), "models/w_sifflet.mdl");
	m_iClip = -1;

	FallInit();// get ready to fall down.
}


void CSifflet::Precache( void )
{
	PRECACHE_MODEL("models/v_sifflet.mdl");
	PRECACHE_MODEL("models/w_sifflet.mdl");
	PRECACHE_MODEL("models/p_sifflet.mdl");

	m_usCrowbar = PRECACHE_EVENT ( 1, "events/sifflet.sc" );
}

int CSifflet::GetItemInfo(ItemInfo *p)
{
	p->pszName = STRING(pev->classname);
	p->pszAmmo1 = NULL;
	p->iMaxAmmo1 = -1;
	p->pszAmmo2 = NULL;
	p->iMaxAmmo2 = -1;
	p->iMaxClip = WEAPON_NOCLIP;
	p->iSlot = 0;
	p->iPosition = 3;
	p->iId = WEAPON_SIFFLET;
	p->iWeight = SIFFLET_WEIGHT;
	return 1;
}



BOOL CSifflet::Deploy( )
{
	return DefaultDeploy( "models/v_sifflet.mdl", "models/p_sifflet.mdl", SI_IDLE, "crowbar" );
}

void CSifflet::Holster( int skiplocal /* = 0 */ )
{
	m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + 0.5;
	//SendWeaponAnim( CROWBAR_HOLSTER );
}




void CSifflet::PrimaryAttack()
{
		SendWeaponAnim( SI_ATTACK );
		m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + 1.0;

}


