#include	"extdll.h"
#include	"util.h"
#include	"cbase.h"
#include	"monsters.h"
#include	"schedule.h"
#include	"soundent.h"
#include	"decals.h"
#include	"gamerules.h"
#include	"player.h"

#include	"weapons.h"

#include	"defuse_the_parpaing.h"
#include	"mur.h"

LINK_ENTITY_TO_CLASS( item_bombparpaing, CBombParpaing );

int m_iSpriteTexture;
float DefuseTime;

void CBombParpaing::Precache()
{
	PRECACHE_MODEL( "models/w_parpaing.mdl" ); // MiK : pas oublier de précacher sinon caca prout
	PRECACHE_SOUND( "feedback/c4_click.wav");
	m_iSpriteTexture = PRECACHE_MODEL( "sprites/shockwave.spr" );
}

void CBombParpaing::Spawn()
{
	Precache();
	DefuseTime = 0;
	Defusage = false;
	SET_MODEL( ENT(pev), "models/w_parpaing.mdl" ); // MiK : ici tu fous le bon model
	FallInit();
	UTIL_SetSize(pev, Vector(-16,-16,0), Vector(16,16,8)); //BLP Important ! regler la taille du parpaing !
	pev->renderfx = kRenderFxGlowShell;
	pev->renderamt = 10;
	pev->rendercolor = Vector( 255, 0, 0 );	// RGB
	SetThink( Think );
	SetUse( Use );
	ExplodeTime = gpGlobals->time + BOMBPARPAING_EXPLODETIME;
	ClickTime = gpGlobals->time + 1;
	DefuseAmount = 0;
	pev->solid		= SOLID_BBOX; //Douanier : Ca c'est indispensable les enfants sinon le parpaing y coule comme une merde.


}

void CBombParpaing::Touch(CBaseEntity *pOther)
{
	return;
}

void CBombParpaing::Use(CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
{
	CBasePlayer *pPlayer;
	if ( pActivator->IsPlayer() )
	pPlayer = (CBasePlayer*)pActivator;
	
	if (pPlayer->IsDefusing == false)
	{
		pPlayer->IsDefusing= true;
		Defusage = true;
		DefuseAmount=0;
	}
	Defusage = true;
}




//douanier : la fonction qu'elle est utile
void CBombParpaing::StopUse(CBasePlayer *pPlayer)
{
//lancé UNE SEULE FOIS quand le joueur lache le bouton use

	pPlayer->StopDefuse();
	pPlayer->IsDefusing = false;
	pPlayer->DefuseDraw= false;
	Defusage = false;
	DefuseAmount=0;
}








void CBombParpaing::Defused()
{
	UTIL_Remove(this);
	UTIL_ClientPrintAll(HUD_PRINTCENTER, "Counter-Parpaingists win");
	g_pGameRules->m_iDefuse = 0;
	g_pGameRules->m_flDefuseTime = gpGlobals->time + CVAR_GET_FLOAT("mp_defuse_time");
}

void CBombParpaing::Click()
{
	if (gpGlobals->time > ClickTime)
	{
		EMIT_SOUND( ENT(pev), CHAN_ITEM, "feedback/c4_click.wav", 1, ATTN_NORM);
			
		if (ExplodeTime - gpGlobals->time > 15)
			ClickTime = gpGlobals->time + 1;
		else if (ExplodeTime - gpGlobals->time > 5)
			ClickTime = gpGlobals->time + 0.5;
		else if (ExplodeTime - gpGlobals->time > 2)
			ClickTime = gpGlobals->time + 0.2;
		else
			ClickTime = gpGlobals->time + 0.1;
	}
}

void CBombParpaing::Think()
{
	Click();

	if (DefuseTime <= gpGlobals->time && Defusage)
	{
	DefuseTime = gpGlobals->time + 1;
	DefuseAmount++;
	}
		
	


	if (DefuseAmount>= 10)
		Defused();
	if ( ExplodeTime <= gpGlobals->time )
	{
	
	MESSAGE_BEGIN( MSG_PAS, SVC_TEMPENTITY, pev->origin );
		WRITE_BYTE( TE_BEAMCYLINDER );
		WRITE_COORD( pev->origin.x);
		WRITE_COORD( pev->origin.y);
		WRITE_COORD( pev->origin.z + 16);
		WRITE_COORD( pev->origin.x);
		WRITE_COORD( pev->origin.y);
		WRITE_COORD( pev->origin.z + 16 + BOMBPARPAING_RADIUS / .2); // reach damage radius over .3 seconds
		WRITE_SHORT( m_iSpriteTexture );
		WRITE_BYTE( 0 ); // startframe
		WRITE_BYTE( 0 ); // framerate
		WRITE_BYTE( 2 ); // life
		WRITE_BYTE( 16 );  // width
		WRITE_BYTE( 0 );   // noise

		WRITE_BYTE( 245   );
		WRITE_BYTE( 202 );
		WRITE_BYTE( 27  );

		WRITE_BYTE( 255 ); //brightness
		WRITE_BYTE( 0 );		// speed
	MESSAGE_END();

	MESSAGE_BEGIN( MSG_PAS, SVC_TEMPENTITY, pev->origin );
		WRITE_BYTE( TE_BEAMCYLINDER );
		WRITE_COORD( pev->origin.x);
		WRITE_COORD( pev->origin.y);
		WRITE_COORD( pev->origin.z + 16);
		WRITE_COORD( pev->origin.x);
		WRITE_COORD( pev->origin.y);
		WRITE_COORD( pev->origin.z + 16 + ( BOMBPARPAING_RADIUS / 2 ) / .2); // reach damage radius over .3 seconds
		WRITE_SHORT( m_iSpriteTexture );
		WRITE_BYTE( 0 ); // startframe
		WRITE_BYTE( 0 ); // framerate
		WRITE_BYTE( 2 ); // life
		WRITE_BYTE( 16 );  // width
		WRITE_BYTE( 0 );   // noise

		WRITE_BYTE( 245   );
		WRITE_BYTE( 202 );
		WRITE_BYTE( 27  );
		
		WRITE_BYTE( 255 ); //brightness
		WRITE_BYTE( 0 );		// speed
	MESSAGE_END();
	
	
	MESSAGE_BEGIN( MSG_PVS, SVC_TEMPENTITY, pev->origin );
			WRITE_BYTE( TE_EXPLOSION);		
			WRITE_COORD( pev->origin.x );
			WRITE_COORD( pev->origin.y );
			WRITE_COORD( pev->origin.z );
			WRITE_SHORT( g_sModelIndexFireball );
			WRITE_BYTE( 50  ); // scale * 10
			WRITE_BYTE( 15  ); // framerate
			WRITE_BYTE( TE_EXPLFLAG_NONE );
		MESSAGE_END();
		
		UTIL_Remove(this);
		
		TraceResult tr;
		UTIL_TraceLine ( pev->origin, pev->origin + Vector ( 0, 0, -32 ),  ignore_monsters, ENT(pev), & tr);
		TraceResult *pTrace = &tr;
		UTIL_DecalTrace( pTrace, DECAL_SCORCH1 );
		
		UTIL_ClientPrintAll(HUD_PRINTCENTER, "Parpaingists wins");
		
		g_pGameRules->m_iDefuse = 0;
		g_pGameRules->m_flDefuseTime = gpGlobals->time + CVAR_GET_FLOAT("mp_defuse_time");	CBaseEntity *pEntity = NULL;
		
		int distance;
		while ((pEntity = UTIL_FindEntityInSphere( pEntity, pev->origin, BOMBPARPAING_RADIUS )) != NULL)
		{
			if (pEntity->pev->takedamage)
			{
				distance = (pev->origin - pEntity->pev->origin).Length();
				if (( !strcmp( STRING(pEntity->pev->classname), "mur1")) || ( !strcmp( STRING(pEntity->pev->classname), "mur2")))
				{
					CMur *pMur = (CMur*)pEntity;
					if (pMur)
					pMur->TakeDamage(pOwner->pev , MUR_HEALTH);
				}
				else if ( !strcmp( STRING(pEntity->pev->classname), "monster_megret"))
				{
					pEntity->pev->health += RANDOM_LONG(100,1000);
					EMIT_SOUND_DYN(ENT(pev), CHAN_VOICE, "megret/spawn.wav", 0.8, ATTN_NORM, 0, 80 + RANDOM_LONG(0,39) );

				}
				else
				{
					float Degats = (((BOMBPARPAING_RADIUS-((float)distance))/BOMBPARPAING_RADIUS)*BOMBPARPAING_DMG);

					if (Degats < 0)
						Degats = 0;

						pEntity->TakeDamage(pev, pev, Degats, DMG_BLAST | DMG_NEVERGIB);
				}
			}
		}

	}

	

	pev->nextthink = gpGlobals->time + 0.1;
	
}


LINK_ENTITY_TO_CLASS( weapon_bombparpaing, CBombParpaingWeapon );

void CBombParpaingWeapon::Precache()
{
	PRECACHE_MODEL("models/v_parpaing.mdl");
	PRECACHE_MODEL("models/w_parpaing.mdl");
	PRECACHE_MODEL("models/p_parpaing.mdl");
	PRECACHE_SOUND("weapons/cbar_hit1.wav");
	PRECACHE_SOUND("weapons/cbar_hit2.wav");
	PRECACHE_SOUND("weapons/cbar_hitbod1.wav");
	PRECACHE_SOUND("weapons/cbar_hitbod2.wav");
	PRECACHE_SOUND("weapons/cbar_hitbod3.wav");
	PRECACHE_SOUND("weapons/cbar_miss1.wav");

	m_usBombParpaing = PRECACHE_EVENT ( 1, "events/bombparpaing.sc" );
}

void CBombParpaingWeapon::Touch(CBaseEntity *pOther)
{
	if (pOther->IsPlayer())
	{
		CBasePlayer *pPlayer = (CBasePlayer*)pOther;
		if (pPlayer->m_iHasParpaing)
			return;
		else
			DefaultTouch(pOther);
	}
}
int CBombParpaingWeapon::AddToPlayer( CBasePlayer *pPlayer )
{
	if ((pPlayer->m_iHasParpaing)
		||	(pPlayer->m_iTeam == INSPECTEUR)
		||	(pPlayer->pev->health < 40))
		return FALSE;
	pPlayer->m_iHasParpaing=2;
	if ( CBasePlayerWeapon::AddToPlayer( pPlayer ) )
	{
		MESSAGE_BEGIN( MSG_ONE, gmsgWeapPickup, NULL, pPlayer->pev );
			WRITE_BYTE( m_iId );
		MESSAGE_END();
		return TRUE;
	}
	return FALSE;
}

void CBombParpaingWeapon::Spawn()
{
	Precache();
	m_iId = WEAPON_BOMBPARPAING;
	SET_MODEL(ENT(pev), "models/w_parpaing.mdl");
	FallInit();
	UTIL_SetSize(pev, Vector(-16,-16,0), Vector(16,16,8)); //BLP Important ! regler la taille du parpaing !
	pev->renderfx = kRenderFxGlowShell;
	pev->renderamt = 20;
	pev->rendercolor = Vector( 200, 200, 200 );	// RGB

	m_iClip = -1;

}


int CBombParpaingWeapon::GetItemInfo(ItemInfo *p)
{
	p->pszName = STRING(pev->classname);
	p->pszAmmo1 = NULL;
	p->iMaxAmmo1 = -1;
	p->pszAmmo2 = NULL;
	p->iMaxAmmo2 = -1;
	p->iMaxClip = WEAPON_NOCLIP;
	p->iSlot = 2;
	p->iPosition = 0;
	p->iId = WEAPON_BOMBPARPAING;
	p->iWeight = PARPAING_WEIGHT;
	return 1;
}

BOOL CBombParpaingWeapon::Deploy( )
{
	return DefaultDeploy( "models/v_parpaing.mdl", "models/p_parpaing.mdl", 2, "bombparpaing" );
}

void CBombParpaingWeapon::Holster( int skiplocal /* = 0 */ )
{
	m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + 0.5;
	SendWeaponAnim( 1 );  // BBk changement de n° pour plus jouer le holdster de la truelle
}

void CBombParpaingWeapon::PrimaryAttack()
{
	return;

#ifndef CLIENT_DLL
	CBombParpaing *pBombParpaing = GetClassPtr((CBombParpaing *)NULL );
	UTIL_MakeVectors( Vector( 0, m_pPlayer->pev->v_angle.y, 0 ) );
	pBombParpaing->pev->origin = m_pPlayer->pev->origin + gpGlobals->v_forward * 64;
	pBombParpaing->pev->classname = MAKE_STRING("item_bombparpaing");
	pBombParpaing->pev->velocity = gpGlobals->v_forward * 100;
	pBombParpaing->Spawn();
	DestroyItem();
	m_pPlayer->m_iHasParpaing=0;
	g_pGameRules->GetNextBestWeapon( m_pPlayer, this );
	UTIL_ClientPrintAll(HUD_PRINTCENTER, "Bombe-Parpaing has been planted");
	if ((CVAR_GET_FLOAT("mp_derrick"))  && (RANDOM_LONG(1, DERRICK_PROB) == 1))
		UTIL_ClientPrintAll(HUD_PRINTTALK, "[Derrick]: Rho le saligaud ! Tous aux abris !\n");
#endif
}

void CBombParpaingWeapon::SecondaryAttack()
{
#ifndef CLIENT_DLL
	CBombParpaingWeapon *pBombParpaingWeapon = GetClassPtr((CBombParpaingWeapon *)NULL );
	pBombParpaingWeapon->pev->classname = MAKE_STRING("weapon_bombparpaing");
	pBombParpaingWeapon->Spawn();
	pBombParpaingWeapon->SetTouch(Touch);
	pBombParpaingWeapon->pev->movetype = MOVETYPE_TOSS;
	UTIL_SetSize(pBombParpaingWeapon->pev, Vector(-16,-16,0), Vector(16,16,8)); //BLP Important ! regler la taille du parpaing !
	
	Vector anglesAim = m_pPlayer->pev->v_angle + m_pPlayer->pev->punchangle;
	UTIL_MakeVectors( anglesAim );
	anglesAim.x		= -anglesAim.x;
	Vector vecSrc	 = m_pPlayer->GetGunPosition( ) - gpGlobals->v_up * 2;
	Vector vecDir	 = gpGlobals->v_forward;
	pBombParpaingWeapon->pev->origin = vecSrc;
	pBombParpaingWeapon->pev->angles = anglesAim;
	pBombParpaingWeapon->pev->owner = m_pPlayer->edict();
	pBombParpaingWeapon->pev->velocity = m_pPlayer->pev->velocity + vecDir * 150;
	pBombParpaingWeapon->pev->speed = m_pPlayer->pev->speed + 300;
	pBombParpaingWeapon->pev->friction = 2;


	DestroyItem();
	UTIL_MakeVectors( Vector( 0, m_pPlayer->pev->v_angle.y, 0 ) );

	UTIL_ClientPrintAll(HUD_PRINTCENTER, "Bomb has been dropped");
	m_pPlayer->pev->weapons &= ~(1<<20);
	m_pPlayer->m_iHasParpaing=0;
	g_pGameRules->GetNextBestWeapon( m_pPlayer, this );
#endif
}
/*#ifndef CLIENT_DLL
	CParpaing *pParpaing = GetClassPtr((CParpaing *)NULL );
	pParpaing->pev->classname = MAKE_STRING("weapon_parpaing");
	pParpaing->Spawn();
	pParpaing->SetTouch(BounceTouch);
	pParpaing->pev->movetype = MOVETYPE_TOSS;
	UTIL_SetSize(pParpaing->pev, Vector(-16,-16,0), Vector(16,16,8)); //BLP Important ! regler la taille du parpaing !
	Vector anglesAim = m_pPlayer->pev->v_angle + m_pPlayer->pev->punchangle;
	UTIL_MakeVectors( anglesAim );
	anglesAim.x		= -anglesAim.x;
	Vector vecSrc	 = m_pPlayer->GetGunPosition( ) - gpGlobals->v_up * 2;
	Vector vecDir	 = gpGlobals->v_forward;
	pParpaing->pev->origin = vecSrc;
	pParpaing->pev->angles = anglesAim;
	pParpaing->pev->owner = m_pPlayer->edict();
	pParpaing->pev->velocity = m_pPlayer->pev->velocity + vecDir * 300;
	pParpaing->pev->speed = m_pPlayer->pev->speed + 300;
	pParpaing->pev->friction = 2;
	m_pPlayer->m_iHasParpaing = 0;
	m_flNextSecondaryAttack = 3;
	m_pPlayer->pev->weapons &= ~(1<<16);
	DestroyItem();
	g_pGameRules->GetNextBestWeapon( m_pPlayer, this );

#endif
*/
//=========================================================
//=========================================================
void CHalfLifeMultiplay :: DefuseInit( void )
{
	//int i = RANDOM_LONG(0, gpGlobals->maxClients);
	int i = RANDOM_LONG(0, 1);
	CBaseEntity *pPlayer = UTIL_PlayerByIndex( i );
	if ( pPlayer )
	{
		CBasePlayer *pPlayer2 = (CBasePlayer*)pPlayer;
		if (( pPlayer2->m_iTeam != SPECTATEUR) && ( pPlayer2->m_iTeam != INSPECTEUR))
		{
			if ((pPlayer2->m_iHasParpaing) || (pPlayer2->pev->health < 40)) 
			{
				m_flDefuseTime = gpGlobals->time + CVAR_GET_FLOAT("mp_defuse_time");
				return;
			}
			else
			{
				UTIL_MessageEffectAll( "Parpaing-Strike : Defuse the Parpaing", 1, -1, -1, 0, Vector(0, 255, 0), 255, Vector(0,0,0),0,1,1,1,0);
				m_iDefuse = 1;
				pPlayer2->GiveNamedItem("weapon_bombparpaing");
				ClientPrint(pPlayer2->pev, HUD_PRINTTALK, "Vous avez la bombe. Vous pouvez la poser pres d'un mur adverse.\n");
				if ((CVAR_GET_FLOAT("mp_derrick"))  && (RANDOM_LONG(1, DERRICK_PROB) == 1))
					ClientPrint(pPlayer2->pev, HUD_PRINTTALK, "[Derrick]: Ou alors tu la poses sur TON mur, ou meme n'importe ou, c'est ton probleme...\n");
			}
			if ((CVAR_GET_FLOAT("mp_derrick"))  && (RANDOM_LONG(1, DERRICK_PROB) == 1))
				UTIL_ClientPrintAll(HUD_PRINTTALK, "[Derrick]: Quelqu'un pour un CS ?\n");
		}
	}
	else
	{
		m_flDefuseTime = gpGlobals->time + CVAR_GET_FLOAT("mp_defuse_time");
	}
}
	
//=========================================================
//=========================================================
void CHalfLifeMultiplay :: DefuseEnd( void )
{
	g_pGameRules->m_iDefuse = 0;
	g_pGameRules->m_flDefuseTime = gpGlobals->time + CVAR_GET_FLOAT("mp_defuse_time");
}

//=========================================================
//=========================================================
void CHalfLifeMultiplay :: DefuseTest( void )
{
	if (gpGlobals->time < m_flDefuseTime) return;
	if ((m_iDefuse) || (RANDOM_LONG(0,1)))
	{
		m_flDefuseTime = gpGlobals->time + CVAR_GET_FLOAT("mp_defuse_time");
		return;
	}
	DefuseInit();

}