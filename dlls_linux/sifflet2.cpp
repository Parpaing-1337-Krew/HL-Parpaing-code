#include    "extdll.h" 
#include    "util.h" 
#include    "cbase.h" 
#include    "weapons.h" 
#include    "player.h"

LINK_ENTITY_TO_CLASS( weapon_sifflet, CSifflet2 );

#ifndef CLIENT_WEAPONS
extern int gmsgCarnet;
#endif

enum sifflet2_e {
	SIFFLET2_IDLE = 0,
	SIFFLET2_IDLE2,
	SIFFLET2_ATTACK,
};

// -------------------------------------------- 
// UseDecrement() - 
// -------------------------------------------- 

BOOL CSifflet2::UseDecrement( void ) 
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

void CSifflet2::SendWeaponAnim( int iAnim, int skiplocal, int body ) 
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

void CSifflet2::Spawn( void ) 
{   
	
	Precache(); 
    pev->classname  = MAKE_STRING( "weapon_sifflet" ); // nom de l'entité 
    m_iId          = WEAPON_SIFFLET2;					// ID de l'arme 


    //m_iDefaultAmmo  = MONARME_DEFAULT_GIVE;            // munitions que donne l'arme quand on la ramasse 

                                       // Précache des ressources necessaires 
//	pev->classname = MAKE_STRING("sifflet");			// La taquetiquetique du gendarmeu


    SET_MODEL( ENT(pev), "models/w_sifflet.mdl" );    // modèle "world" de l'arme 

    FallInit();                                        // prète à tomber au sol 
}

void CSifflet2::Precache( void ) 
{ 
    // modèles de l'arme 
	PRECACHE_MODEL("models/v_sifflet.mdl");
	PRECACHE_MODEL("models/w_sifflet.mdl");
	PRECACHE_MODEL("models/p_sifflet.mdl");

	PRECACHE_SOUND("weapons/sifflet1.wav");
	PRECACHE_SOUND("weapons/sifflet2.wav");
	PRECACHE_SOUND("weapons/sifflet3.wav");


//	m_usSifflet2Fire = PRECACHE_EVENT ( 1, "events/snarkfire.sc" );


}

int CSifflet2::GetItemInfo( ItemInfo *p ) 
{ 
    p->pszName      = STRING( pev->classname ); // nom de l'entité 
    p->pszAmmo1    = NULL;          // type de munitions pour le premier mode de tire 
    p->iMaxAmmo1    = -1;        // nombre maximum de munition type #1 
    p->pszAmmo2    = NULL;                    // type de munitions pour le second mode de tire 
    p->iMaxAmmo2    = -1;                      // nombre maximum de munition type #2 
    p->iMaxClip    = WEAPON_NOCLIP;        // capacité maximale du chargeur 
    p->iSlot        = 1;                        // slot dans le HUD 
    p->iPosition    = 1;                        // position dans le slot 
    p->iFlags      = 0;                        // drapeau d'état 
    p->iId = m_iId  = WEAPON_SIFFLET2;          // ID de l'arme 
    p->iWeight      = SIFFLET2_WEIGHT;          // priorité dans le choix automatique 

    return 1;                                  // tout s'est bien passé, on retourne 1 
}

void CSifflet2::PrimaryAttack(void)
{
		SendWeaponAnim( SIFFLET2_ATTACK );
		m_pPlayer->SetAnimation( PLAYER_ATTACK1 );
		
		// son
		switch( RANDOM_LONG(0,2) )
				{
				case 0:
					EMIT_SOUND(ENT(m_pPlayer->pev), CHAN_WEAPON, "weapons/sifflet1.wav", 1, ATTN_NORM); break;
				case 1:
					EMIT_SOUND(ENT(m_pPlayer->pev), CHAN_WEAPON, "weapons/sifflet2.wav", 1, ATTN_NORM); break;
				case 2:
					EMIT_SOUND(ENT(m_pPlayer->pev), CHAN_WEAPON, "weapons/sifflet3.wav", 1, ATTN_NORM); break;
				}

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
		if (pMauvais->m_iTeam == MACON1 || pMauvais->m_iTeam == MACON2/* pour les tests || pMauvais->m_iTeam == INSPECTEUR*/)
		{
			pMauvais->IsCheck = 1;
			pMauvais->StopByIns = gpGlobals->time + 3.0;

			#ifndef CLIENT_WEAPONS

			MESSAGE_BEGIN( MSG_ONE, gmsgCarnet , NULL ,m_pPlayer->edict());
			WRITE_BYTE(1);  // dit a l'inspecteur de se magnier assez vite pour pas que ca proie l'echappe !
			MESSAGE_END();
			#endif

			pMauvais->EnableControl (FALSE);
		}

		}
	}

	///////////////////////// pour mes tests solos !!!!!!!!!!!!!

	/*CBaseEntity *pEntity = NULL;
	while ((pEntity = UTIL_FindEntityInSphere( pEntity, m_pPlayer->pev->origin, 200 )) != NULL)
	{
		if (pEntity->IsPlayer() && !pEntity->IsMoving()) // si l'obj est bien 1 player et kil ne bouge pas.
		{
			CBasePlayer *pMauvais;
			pMauvais = GetClassPtr((CBasePlayer *)pEntity->pev);
			if (pMauvais->m_iTeam == MACON1 || pMauvais->m_iTeam == MACON2 || pMauvais->m_iTeam == 3)
			{
			pMauvais->IsCheck = 1;
			pMauvais->StopByIns = gpGlobals->time + 3.0;

			#ifndef CLIENT_WEAPONS

			MESSAGE_BEGIN( MSG_ONE, gmsgCarnet , NULL ,m_pPlayer->edict());
			WRITE_BYTE(1);  // dit a l'inspecteur de se magnier assez vite pour pas que ca proie l'echappe !
			MESSAGE_END();
			#endif

			//pMauvais->EnableControl (FALSE);

				}
		}
	}*/
	/*if (pEntity==NULL)
			ALERT(at_console,"Personne a arreter\n");*/

			int flags;
#ifdef CLIENT_WEAPONS
	flags = FEV_NOTHOST;
#else
	flags = 0;
#endif

		//PLAYBACK_EVENT_FULL( flags, m_pPlayer->edict(), m_usSifflet2Fire, 0.0, (float *)&g_vecZero, (float *)&g_vecZero, 0.0, 0.0, 0, 0, 0, 0 );
		m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + 5.0;

}


int CSifflet2::AddToPlayer( CBasePlayer *pPlayer ) 
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

BOOL CSifflet2::Deploy( void ) 
{ 
    return DefaultDeploy( "models/v_sifflet.mdl", "models/p_sifflet.mdl", SIFFLET2_IDLE, "hive" ); 
} 


// -------------------------------------------- 
// Holster() - 
// -------------------------------------------- 

void CSifflet2::Holster( int skiplocal /* = 0 */ ) 
{ 
    //SendWeaponAnim( MA_HOLSTER ); 
}

// -------------------------------------------- 
// WeaponIdle() - 
// -------------------------------------------- 

void CSifflet2::WeaponIdle( void ) 
{ 
    ResetEmptySound(); 

    m_pPlayer->GetAutoaimVector( AUTOAIM_5DEGREES ); 

    if( m_flTimeWeaponIdle > UTIL_WeaponTimeBase() ) 
        return; 


    switch( RANDOM_LONG( 0, 1 ) ) 
    { 
        // on joue une animation "idle" au hasard 
        default: 
        case 0: SendWeaponAnim( SIFFLET2_IDLE ); break; 
        case 1: SendWeaponAnim( SIFFLET2_IDLE2 ); break; 
    } 

    // temps aléatoire avant de rappler cette fonction 
    m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + UTIL_SharedRandomFloat( m_pPlayer->random_seed, 10, 15 ); 
}

void CSifflet2::SecondaryAttack( void )
{
	m_pPlayer->SelectItem ("weapon_carnet");

}
