#include    "extdll.h" 
#include    "util.h" 
#include    "cbase.h" 
#include    "weapons.h" 
#include    "player.h"

LINK_ENTITY_TO_CLASS( weapon_carnet, CCarnet );

enum carnet_e {
	CARNET_IDLE = 0,
	CARNET_IDLE2,
	CARNET_ATTACK,
	CARNET_HOLSTER,
	CARNET_DRAW,
};

// -------------------------------------------- 
// UseDecrement() - 
// -------------------------------------------- 

BOOL CCarnet::UseDecrement( void ) 
{ 
#if defined( CLIENT_WEAPONS ) 
    return TRUE; 
#else 
    return FALSE; 
#endif 
} 




// -------------------------------------------- 
// SendWeaponAnim() - jou l'animation iAnim de 
// du modèle de l'arme. 
// -------------------------------------------- 

void CCarnet::SendWeaponAnim( int iAnim, int skiplocal, int body ) 
{ 
	#ifndef CLIENT_DLL 
    MESSAGE_BEGIN( MSG_ONE, SVC_WEAPONANIM, NULL, m_pPlayer->pev ); 
        WRITE_BYTE( iAnim ); 
        WRITE_BYTE( body ); 
    MESSAGE_END(); 
	#endif 
}

// -------------------------------------------- 
// Spawn() - Apparition de l'arme sur la map. 
// -------------------------------------------- 

void CCarnet::Spawn( void ) 
{ 
    pev->classname  = MAKE_STRING( "weapon_carnet" ); // nom de l'entité 
    m_iId          = WEAPON_CARNET;                  // ID de l'arme 
    //m_iDefaultAmmo  = MONARME_DEFAULT_GIVE;            // munitions que donne l'arme quand on la ramasse 

    Precache();                                        // Précache des ressources necessaires

    SET_MODEL( ENT(pev), "models/w_carnet.mdl" );    // modèle "world" de l'arme 

    FallInit();                                        // prète à tomber au sol 
}

void CCarnet::Precache( void ) 
{ 
    // modèles de l'arme 
	PRECACHE_MODEL("models/v_carnet.mdl");
	PRECACHE_MODEL("models/w_carnet.mdl");
	PRECACHE_MODEL("models/p_carnet.mdl");

	PRECACHE_SOUND("weapons/carnet_ecrit.wav");
/*	PRECACHE_SOUND("weapons/carnet1.wav");
	PRECACHE_SOUND("weapons/carnet2.wav");
	PRECACHE_SOUND("weapons/carnet3.wav");*/



}

int CCarnet::GetItemInfo( ItemInfo *p ) 
{ 
    p->pszName      = STRING( pev->classname ); // nom de l'entité 
    p->pszAmmo1    = NULL;          // type de munitions pour le premier mode de tire 
    p->iMaxAmmo1    = -1;        // nombre maximum de munition type #1 
    p->pszAmmo2    = NULL;                    // type de munitions pour le second mode de tire 
    p->iMaxAmmo2    = -1;                      // nombre maximum de munition type #2 
    p->iMaxClip    = WEAPON_NOCLIP;        // capacité maximale du chargeur 
    p->iSlot        = 0;                        // slot dans le HUD 
    p->iPosition    = 2;                        // position dans le slot 
    p->iFlags      = 0;                        // drapeau d'état 
    p->iId = m_iId  = WEAPON_CARNET;          // ID de l'arme 
    p->iWeight      = CARNET_WEIGHT;          // priorité dans le choix automatique 

    return 1;                                  // tout s'est bien passé, on retourne 1 
}

void CCarnet::PrimaryAttack(void)
{
		
	#ifndef CLIENT_DLL 

	SendWeaponAnim( CARNET_ATTACK );
	m_pPlayer->SetAnimation( PLAYER_ATTACK1 );

	/*	switch( RANDOM_LONG(0,2) )
				{
				case 0:
					EMIT_SOUND(ENT(m_pPlayer->pev), CHAN_WEAPON, "weapons/carnet1.wav", 1, ATTN_NORM); break;
				case 1:
					EMIT_SOUND(ENT(m_pPlayer->pev), CHAN_WEAPON, "weapons/carnet2.wav", 1, ATTN_NORM); break;
				case 2:
					EMIT_SOUND(ENT(m_pPlayer->pev), CHAN_WEAPON, "weapons/carnet3.wav", 1, ATTN_NORM); break;
				}*/
	EMIT_SOUND(ENT(m_pPlayer->pev), CHAN_WEAPON, "weapons/carnet_ecrit.wav", 1, ATTN_NORM);

		TraceResult tr;

	UTIL_MakeVectors(m_pPlayer->pev->v_angle);
	UTIL_TraceLine(m_pPlayer->pev->origin + m_pPlayer->pev->view_ofs,m_pPlayer->pev->origin + m_pPlayer->pev->view_ofs + gpGlobals->v_forward * 8192,dont_ignore_monsters, m_pPlayer->edict(), &tr );
	if ( tr.flFraction != 1.0 && !FNullEnt( tr.pHit) )
	{
		CBaseEntity *pHit = CBaseEntity::Instance( tr.pHit );
		if (pHit->IsPlayer())
		{
		CBasePlayer *pMauvais;
		pMauvais = GetClassPtr((CBasePlayer *)pHit->pev);
		if (pMauvais->m_iTeam == MACON1 || pMauvais->m_iTeam == MACON2)
		{
			if (pMauvais->IsCheck) {
			pMauvais->BlameMacon(m_pPlayer,pMauvais); // hop un petit sprite ki lui rappele son dur labeur !
			m_pPlayer->AddPoints (1,1);

			}
			}

		}
	}





///////////// pour mes tests solo

/*	CBaseEntity *pEntity = NULL;
	while ((pEntity = UTIL_FindEntityInSphere( pEntity, m_pPlayer->pev->origin, 200 )) != NULL)
	{
		if (pEntity->IsPlayer() && !pEntity->IsMoving()) // si l'obj est bien 1 player et kil ne bouge pas.
		{
			CBasePlayer *pMauvais;
			pMauvais = GetClassPtr((CBasePlayer *)pEntity->pev);
			if (pMauvais->m_iTeam == MACON1 || pMauvais->m_iTeam == MACON2 || pMauvais->m_iTeam == 3)
			{
			if (pMauvais->IsCheck) {
			pMauvais->BlameMacon(m_pPlayer,pMauvais); // hop un petit sprite ki lui rappele son dur labeur !
			m_pPlayer->AddPoints (1,1);

			}

				}
		}
	}*/


	#endif
		



		int flags;
#ifdef CLIENT_WEAPONS
	flags = FEV_NOTHOST;
#else
	flags = 0;
#endif

		//PLAYBACK_EVENT_FULL( flags, m_pPlayer->edict(), m_usCarnetFire, 0.0, (float *)&g_vecZero, (float *)&g_vecZero, 0.0, 0.0, 0, 0, 0, 0 );
		m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + 7.0;

}

void CCarnet::SecondaryAttack(void)
{

	m_pPlayer->SelectItem ("weapon_sifflet");
}

int CCarnet::AddToPlayer( CBasePlayer *pPlayer ) 
{ 
    if( CBasePlayerWeapon::AddToPlayer( pPlayer ) ) 
    { 
        MESSAGE_BEGIN( MSG_ONE, gmsgWeapPickup, NULL, pPlayer->pev ); 
            WRITE_BYTE( m_iId ); 
        MESSAGE_END(); 

        return TRUE; 
    } 

    return FALSE; 
}

BOOL CCarnet::Deploy( void ) 
{ 
    return DefaultDeploy( "models/v_carnet.mdl", "models/p_carnet.mdl", CARNET_IDLE, "hive" ); 
} 


// -------------------------------------------- 
// Holster() - 
// -------------------------------------------- 

void CCarnet::Holster( int skiplocal /* = 0 */ ) 
{ 
    SendWeaponAnim( CARNET_HOLSTER ); 
}

// -------------------------------------------- 
// WeaponIdle() - 
// -------------------------------------------- 

void CCarnet::WeaponIdle( void ) 
{ 
    ResetEmptySound(); 

    m_pPlayer->GetAutoaimVector( AUTOAIM_5DEGREES ); 

    if( m_flTimeWeaponIdle > UTIL_WeaponTimeBase() ) 
        return; 


    switch( RANDOM_LONG( 0, 1 ) ) 
    { 
        // on joue une animation "idle" au hasard 
        default: 
        case 0: SendWeaponAnim( CARNET_IDLE ); break; 
        case 1: SendWeaponAnim( CARNET_IDLE2 ); break; 
    } 

    // temps aléatoire avant de rappler cette fonction 
    m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + UTIL_SharedRandomFloat( m_pPlayer->random_seed, 10, 15 ); 
}
