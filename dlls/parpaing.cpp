/***
*
*	Copyright (c) 1996-2001, Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id 
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "monsters.h"
#include "weapons.h"
#include "nodes.h"
#include "player.h"
#include "gamerules.h"

#include "decals.h"
#include	"mur.h"
#include	"special_parpaing.h"

#define	PARPAING_BODYHIT_VOLUME 128
#define	PARPAING_WALLHIT_VOLUME 512

LINK_ENTITY_TO_CLASS( weapon_parpaing, CParpaing );

int Action;


enum gauss_e {
	PARPAING_IDLE = 0,
	PARPAING_IDLE2,
	PARPAING_DRAW,
	PARPAING_ATTACK1,
	PARPAING_ATTACK2,
	PARPAING_POSAGE
};

void CParpaing::Spawn( )
{
	Precache( );
	pev->classname = MAKE_STRING("weapon_parpaing");
	m_iId = WEAPON_PARPAING;
	SET_MODEL(ENT(pev), "models/w_parpaing.mdl");
	m_iClip = -1;
	onSol = 0;
	FallInit();// get ready to fall down.
}


void CParpaing::Precache( void )
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

	PRECACHE_MODEL("sprites/gargeye1.spr");

	m_usParpaing = PRECACHE_EVENT ( 1, "events/parpaing.sc" );
}

int CParpaing::GetItemInfo(ItemInfo *p)
{
	p->pszName = STRING(pev->classname);
	p->pszAmmo1 = NULL;
	p->iMaxAmmo1 = -1;
	p->pszAmmo2 = NULL;
	p->iMaxAmmo2 = -1;
	p->iMaxClip = WEAPON_NOCLIP;
	p->iSlot = 0;
	p->iPosition = 1;
	p->iId = WEAPON_PARPAING;
	p->iWeight = PARPAING_WEIGHT;
	return 1;
}

BOOL CParpaing::Deploy( )
{
	return DefaultDeploy( "models/v_parpaing.mdl", "models/p_parpaing.mdl", PARPAING_DRAW, "parpaing" );
}

void CParpaing::Holster( int skiplocal /* = 0 */ )
{
	m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + 0.5;
	SendWeaponAnim( PARPAING_DRAW );
}


void FindHullIntersectionParpaing( const Vector &vecSrc, TraceResult &tr, float *mins, float *maxs, edict_t *pEntity )
{
	int			i, j, k;
	float		distance;
	float		*minmaxs[2] = {mins, maxs};
	TraceResult tmpTrace;
	Vector		vecHullEnd = tr.vecEndPos;
	Vector		vecEnd;

	distance = 1e6f;

	vecHullEnd = vecSrc + ((vecHullEnd - vecSrc)*2);
	UTIL_TraceLine( vecSrc, vecHullEnd, dont_ignore_monsters, pEntity, &tmpTrace );
	if ( tmpTrace.flFraction < 1.0 )
	{
		tr = tmpTrace;
		return;
	}

	for ( i = 0; i < 2; i++ )
	{
		for ( j = 0; j < 2; j++ )
		{
			for ( k = 0; k < 2; k++ )
			{
				vecEnd.x = vecHullEnd.x + minmaxs[i][0];
				vecEnd.y = vecHullEnd.y + minmaxs[j][1];
				vecEnd.z = vecHullEnd.z + minmaxs[k][2];

				UTIL_TraceLine( vecSrc, vecEnd, dont_ignore_monsters, pEntity, &tmpTrace );
				if ( tmpTrace.flFraction < 1.0 )
				{
					float thisDistance = (tmpTrace.vecEndPos - vecSrc).Length();
					if ( thisDistance < distance )
					{
						tr = tmpTrace;
						distance = thisDistance;
					}
				}
			}
		}
	}
}


/*void CBasePlayer::hasp()
{
	ClientPrint(pev, HUD_PRINTTALK, UTIL_VarArgs("%i",m_iHasParpaing));
}*/
int CParpaing::AddToPlayer( CBasePlayer *pPlayer )
{

	// hlp (fab) <- oh oui il est fier de lui , c'est plus clean qu'avant et ca devrai marcher !
	Puissance = 0;
	CBaseEntity *pEntity = CBaseEntity::Instance(pev->owner);
	CBasePlayer *pOw;
	pOw = GetClassPtr((CBasePlayer *)pEntity->pev);


	if ((pPlayer->m_iTeam==INSPECTEUR) || (pPlayer->m_iHasParpaing))
		return FALSE;
	
	if (gpGlobals->time>DernierTouche) 
		pOw=NULL; // reset la team du parpaing


	if (pOw!=NULL)
	{
	if (pPlayer->m_iTeam!=pOw->m_iTeam && pOw->m_iTeam!=0) 
	pPlayer->TakeDamage (pOw->pev,pPlayer->pev,30,DMG_SHOCK); // ahhh l'attaque des parpaing volant.au secours
	}

	if (pPlayer->pev->health<40)
	return FALSE;


	if ( CBasePlayerWeapon::AddToPlayer( pPlayer ) )
	{
		MESSAGE_BEGIN( MSG_ONE, gmsgWeapPickup, NULL, pPlayer->pev );
			WRITE_BYTE( m_iId );
		MESSAGE_END();

		if (!pPlayer->m_iHasParpaing)
		pPlayer->m_iHasParpaing = 1;
		m_flNextSecondaryAttack = 1;
		#ifndef CLIENT_DLL
		g_pGameRules->SetSpeed(pPlayer);
			#endif
	      SendWeaponAnim( PARPAING_ATTACK1 );
		return TRUE;
	}
	return FALSE;
}

//=========================================================
// Sets up movetype, size, solidtype for a new weapon. 
//=========================================================
void CParpaing :: Reset (void )
{
	UTIL_Remove( this );
}
void CParpaing :: FallInit( void )
{
	pev->movetype = MOVETYPE_TOSS;
	pev->solid = SOLID_BBOX;

	UTIL_SetOrigin( pev, pev->origin );
	//UTIL_SetSize(pev, Vector( 0, 0, 0), Vector(0, 0, 0) );//pointsize until it lands on the ground.
	UTIL_SetSize(pev, Vector(-16,-16,0), Vector(16,16,8)); //BLP Important ! regler la taille du parpaing !

	SetTouch( DefaultTouch );
	SetThink( FallThink2 );
	pev->nextthink = gpGlobals->time + 0.1;
}
void CParpaing::FallThink2 ( void )
{

	pev->nextthink = gpGlobals->time + 0.1;

	if ( pev->flags & FL_ONGROUND )
	{
		//BLP
	//	SetTouch( DefaultTouch );
	// detection avec le solid le plus proche (on sait jamais...)

		TraceResult trac;
		UTIL_TraceLine( pev->origin, pev->origin - gpGlobals->v_up * 16, ignore_monsters, edict(), &trac );
		CBaseEntity *pHit = CBaseEntity::Instance( trac.pHit );

		if ((pev->origin - trac.vecEndPos).Length()>=10)
		{
		// certainement une collision avec un autre parpaing
		pev->velocity = pev->velocity + Vector (0,0,100);
		} else {
		// pas de collision : on peut dire qu'il est au sol.
		

		//ALERT (at_console,"distance : %f\n",(pev->origin - trac.vecEndPos).Length());

		// ici on va mettre un decal de cassure par terre
		TraceResult trace;
		UTIL_TraceLine( pev->origin, pev->origin + Vector(0,0,-20), ignore_monsters, edict(), &trace );
		if ( trace.flFraction < 1.0 )
		{
			switch( RANDOM_LONG(0,3) )
			{
			case 0:
		UTIL_DecalTrace( &trace, DECAL_PARPAING1 );
				case 1:
		UTIL_DecalTrace( &trace, DECAL_PARPAING2 );
				case 2:
		UTIL_DecalTrace( &trace, DECAL_PARPAING3 );
				case 3:
		UTIL_DecalTrace( &trace, DECAL_PARPAING4 );
			}
		}

		DernierTouche= gpGlobals->time + 0.1;

		if ( !FNullEnt( pev->owner ) )
		{
			int pitch = 95 + RANDOM_LONG(0,29);
			EMIT_SOUND_DYN(ENT(pev), CHAN_VOICE, "weapons/parpaing.wav", 1, ATTN_NORM, 0, pitch);
		}

		pev->angles.z = 0;
		pev->angles.x = 0;

		Materialize();
		}
	}
		pev->angles.z += 10;
		pev->angles.x += 10;

}


#ifndef CLIENT_DLL
void CParpaing::BounceTouch( CBaseEntity *pOther )
{

if (FClassnameIs(pOther->pev, "weapon_parpaing"))
{
pev->velocity = pev->velocity + Vector (0,0,100);

}
	if (pOther->pev->takedamage)
	{

		TraceResult tr = UTIL_GetGlobalTrace( );
		entvars_t	*pevOwner;

		pevOwner = VARS( pev->owner );

		// UNDONE: this needs to call TraceAttack instead
		ClearMultiDamage( );

		if ( pOther->IsPlayer() )
		{
			pOther->TraceAttack(pevOwner, gSkillData.plrDmgParpaing * 2 , pev->velocity.Normalize(), &tr, DMG_BULLET | DMG_NEVERGIB ); 
			//DefaultTouch( pOther );
		}
		else if (( strcmp( STRING(pOther->pev->classname), "mur1")) && ( strcmp( STRING(pOther->pev->classname), "mur2")))
		{
			pOther->TraceAttack(pevOwner, gSkillData.plrDmgParpaing * 2 , pev->velocity.Normalize(), &tr, DMG_BULLET | DMG_NEVERGIB ); 
			pOther->pev->velocity = pev->velocity /2;
			pOther->pev->velocity.z = 200; 
		}

		ApplyMultiDamage( pev, pevOwner );

		pev->owner = NULL;
	}
	FallInit();
}




//007:j'arrivais pas à corriger ton code bob alors j'ai recommencé...


void CParpaingVolant::Spawn ( void )
{
	pev->movetype = MOVETYPE_BOUNCE;
	pev->classname = MAKE_STRING( "ParpaingVolant" );
	
	pev->solid = SOLID_BBOX;

	SET_MODEL(ENT(pev), "models/w_parpaing.mdl");
UTIL_SetSize(pev, Vector(-16,-16,0), Vector(16,16,8));





}






//transforme un parpaing volant (CParpaingVolant de type CBaseMonster) en arme parpaing (CParpaing de type CBaseWeapon)
void CParpaingVolant::Mutation (void)
{

//ALERT(at_console,"###Mutation###\n");
CParpaing *pParpaing = GetClassPtr((CParpaing *)NULL );
	pParpaing->pev->classname = MAKE_STRING("weapon_parpaing");
	pParpaing->Spawn();
	pParpaing->pev->origin = this->pev->origin;
	pParpaing->pev->angles = this->pev->angles;
	pParpaing->pev->velocity = Vector(0,0,-70);
pParpaing->FallInit();

UTIL_SetSize(pParpaing->pev, Vector(-16,-16,0), Vector(16,16,8));
UTIL_Remove( this );
return;


}













void CParpaingVolant::Touch(CBaseEntity *pOther)
{

//ALERT(at_console,"Puissance : %f",pev->velocity.Length());
if(NbrRebond > 4)
{
//ALERT(at_console,"1 rebonds...\n");
Mutation();
return;	
}
	
	
	
	
	NbrRebond++;
	
/*
ALERT(at_console,"pouet");


if (pOther->IsPlayer())
{
//Il semble que l'on ai touché un autre gars

CBasePlayer *pCible;

pCible = (CBasePlayer *)pOther;

ClientPrint( pCible->pev, HUD_PRINTCONSOLE, "#JOUEUR#" );

ALERT(at_console,"la cible est détectée\n");




if(pCible->m_iTeam == this->Team)
ALERT(at_console,"même équipe\n");




}
*/



if (pOther->pev->takedamage)
	{
		TraceResult tr = UTIL_GetGlobalTrace( );
		entvars_t	*pevOwner;

		pevOwner = VARS( pev->owner );

		// UNDONE: this needs to call TraceAttack instead
		ClearMultiDamage( );

		if ( pOther->IsPlayer() )
		{
			pOther->TraceAttack(pevOwner, gSkillData.plrDmgParpaing*2, pev->velocity.Normalize(), &tr, DMG_NEVERGIB ); 
		}
		else
		{
			pOther->TraceAttack(pevOwner, gSkillData.plrDmgParpaing*2, pev->velocity.Normalize(), &tr, DMG_BULLET | DMG_NEVERGIB ); 
		}

		ApplyMultiDamage( pev, pevOwner );
	}






	
	if (pev->velocity.Length() <= 150)
	{
	
		//on le suprime et on le remplace par "l'arme parpaing"
		Mutation();
		return;
	}
	else
	{
	
	
	
		pev->velocity = pev->velocity * (0.25);

	}













}




void CParpaing::CreateFlyingParpaing()
{

	
	CParpaingVolant *pParpaing = GetClassPtr( (CParpaingVolant *)NULL );
	pParpaing->Spawn();
	pParpaing->pev->gravity = 0.8; //parpaing aérodynamique :D
	UTIL_SetOrigin( pParpaing->pev, m_pPlayer->pev->origin + m_pPlayer->pev->view_ofs + gpGlobals->v_forward * 16 );
	pParpaing->pev->velocity = gpGlobals->v_forward * (150+100 * Puissance) ;
	pParpaing->pev->angles = UTIL_VecToAngles (pParpaing->pev->velocity);
	pParpaing->pev->owner = ENT(m_pPlayer->pev);
	pParpaing->pev->avelocity.x = RANDOM_FLOAT ( -100, -500 );//aucune idée de ce à quoi ca serre
	pParpaing->SetTouch( Touch );
	pParpaing->Team = m_pPlayer->m_iTeam;
	
	
Puissance = 0;
g_pGameRules->SetSpeed(m_pPlayer);

}

void CParpaing::CreateSpecialParpaing(int m_iType)
{
	CParpaing *pParpaing = GetClassPtr((CParpaing *)NULL );
	switch(m_iType)
	{
	case ICE:
		pParpaing = GetClassPtr((CIceParpaing *)NULL );
	break;

	}
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

	switch(m_iType)
	{
	case ICE:
		pParpaing->pev->avelocity = Vector (0, 60, 0);
		pParpaing->pev->classname = MAKE_STRING("weapon_iceparpaing");
		pParpaing->pev->movetype = MOVETYPE_BOUNCE;
		pParpaing->pev->solid = SOLID_BBOX;
		pParpaing->pev->gravity = 1;
		pParpaing->pev->friction = 1;
		pParpaing->SetTouch( NULL );
	break;
	}

}
#endif


void CParpaing::SecondaryAttack()
{
if (Puissance >= 6 && Lancement)
return; 
	
	if (Lancement)
	{
	//lancement déjà enclanché
	if (LancementTime < gpGlobals->time)
	{
	Puissance ++;
	LancementTime = gpGlobals->time + 0.2;
	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 0.3;
	}





	}
	else
	{

	Lancement = true;
	Puissance = 1;
	LancementTime = gpGlobals->time + 0.2;
	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 0.3;

	}
	
	
	
	
	


}

void CParpaing::PrimaryAttack()
{

	if (m_pPlayer->ParpaingTime!=0)
	return;
//		m_pPlayer->SetAnimation( PLAYER_ATTACK1 );



	if (! Swing( 1 ))
	{
		SetThink( SwingAgain );
		pev->nextthink = gpGlobals->time + 1.0;
	}
}


void CParpaing::Smack( )
{
	DecalGunshot( &m_trHit, BULLET_PLAYER_PARPAING );
}


void CParpaing::SwingAgain( void )
{
	Swing( 0 );
}


int CParpaing::Swing( int fFirst )
{
	int fDidHit = FALSE;

	TraceResult tr;

	UTIL_MakeVectors (m_pPlayer->pev->v_angle);
	Vector vecSrc	= m_pPlayer->GetGunPosition( );
	Vector vecEnd	= vecSrc + gpGlobals->v_forward * 32;

	UTIL_TraceLine( vecSrc, vecEnd, dont_ignore_monsters, ENT( m_pPlayer->pev ), &tr );

#ifndef CLIENT_DLL
	if ( tr.flFraction >= 1.0 )
	{
		UTIL_TraceHull( vecSrc, vecEnd, dont_ignore_monsters, head_hull, ENT( m_pPlayer->pev ), &tr );
		if ( tr.flFraction < 1.0 )
		{
			// Calculate the point of intersection of the line (or hull) and the object we hit
			// This is and approximation of the "best" intersection
			CBaseEntity *pHit = CBaseEntity::Instance( tr.pHit );
			if ( !pHit || pHit->IsBSPModel() )
				FindHullIntersectionParpaing( vecSrc, tr, VEC_DUCK_HULL_MIN, VEC_DUCK_HULL_MAX, m_pPlayer->edict() );
			vecEnd = tr.vecEndPos;	// This is the point on the actual surface (the hull could have hit space)
		}
	}
#endif

	PLAYBACK_EVENT_FULL( FEV_NOTHOST, m_pPlayer->edict(), m_usParpaing, 
	0.0, (float *)&g_vecZero, (float *)&g_vecZero, 0, 0, 0,
	0.0, 0, 0.0 );


	if ( tr.flFraction >= 1.0 )
	{
		if (fFirst)
		{
			// miss
			m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + 1.0;
			
			// player "shoot" animation
			m_pPlayer->SetAnimation( PLAYER_ATTACK1 );
		}
	}
	else
	{


  //      SendWeaponAnim( PARPAING_ATTACK1 );
 


		// player "shoot" animation
		m_pPlayer->SetAnimation( PLAYER_ATTACK1 );
		
#ifndef CLIENT_DLL

		// hit
		fDidHit = TRUE;
		CBaseEntity *pEntity = CBaseEntity::Instance(tr.pHit);

		ClearMultiDamage( );

		if ( (m_flNextPrimaryAttack + 1 < UTIL_WeaponTimeBase() ) || g_pGameRules->IsMultiplayer() )
		{
			// first swing does full damage
			pEntity->TraceAttack(m_pPlayer->pev, gSkillData.plrDmgParpaing, gpGlobals->v_forward, &tr, DMG_CLUB | DMG_NEVERGIB ); 
			/*if (!strcmp(STRING(pEntity->pev->classname), "mur1"))
			{	
				if (m_pPlayer->m_iTeam ==2 )
				{

					CMur *mur1 =(CMur*)pEntity;
					if(mur1->avancement > 0 )
					{
						mur1->avancement --;
						if (mur1->avancement <= 0)
						{
							mur1->avancement=0;
							if (CVAR_GET_FLOAT("mp_derrick"))
								UTIL_ClientPrintAll( HUD_PRINTTALK, UTIL_VarArgs("[Derrick]: Un mur detruit par %s ! Ca t'amuse p'tit con ?\n", STRING(m_pPlayer->pev->netname)));
						}
					}
					mur1->Anim(); // remet à jour l'anim du mur
				} else 
					PoseParpaing();

			}

			if (!strcmp(STRING(pEntity->pev->classname), "mur2"))
			{	
				if (m_pPlayer->m_iTeam ==1 )
				{

					CMur *mur2 =(CMur*)pEntity;
					if(mur2->avancement > 0 )
					{
						mur2->avancement --;
						if (mur2->avancement <= 0)
						{
							mur2->avancement=0;
							if (CVAR_GET_FLOAT("mp_derrick"))
								UTIL_ClientPrintAll( HUD_PRINTTALK, UTIL_VarArgs("[Derrick]: Un mur detruit par %s ! Ca t'amuse p'tit con ?\n", STRING(m_pPlayer->pev->netname)));
						}
					}
					mur2->Anim(); // remet à jour l'anim du mur
				} else				
				PoseParpaing();

			}*/
		
		}
		else
		{
			// subsequent swings do half
			pEntity->TraceAttack(m_pPlayer->pev, gSkillData.plrDmgParpaing / 2, gpGlobals->v_forward, &tr, DMG_CLUB | DMG_NEVERGIB ); 
		}	
		ApplyMultiDamage( m_pPlayer->pev, m_pPlayer->pev );

		// play thwack, smack, or dong sound
		float flVol = 1.0;
		int fHitWorld = TRUE;

		if (pEntity)
		{
			if ( pEntity->Classify() != CLASS_NONE && pEntity->Classify() != CLASS_MACHINE )
			{
				// play thwack or smack sound
				switch( RANDOM_LONG(0,2) )
				{
				case 0:
					EMIT_SOUND(ENT(m_pPlayer->pev), CHAN_ITEM, "weapons/cbar_hitbod1.wav", 1, ATTN_NORM); break;
				case 1:
					EMIT_SOUND(ENT(m_pPlayer->pev), CHAN_ITEM, "weapons/cbar_hitbod2.wav", 1, ATTN_NORM); break;
				case 2:
					EMIT_SOUND(ENT(m_pPlayer->pev), CHAN_ITEM, "weapons/cbar_hitbod3.wav", 1, ATTN_NORM); break;
				}
				m_pPlayer->m_iWeaponVolume = PARPAING_BODYHIT_VOLUME;
				if ( !pEntity->IsAlive() )
					  return TRUE;
				else
					  flVol = 0.1;

				fHitWorld = FALSE;
			}
		}

		// play texture hit sound
		// UNDONE: Calculate the correct point of intersection when we hit with the hull instead of the line

		if (fHitWorld)
		{
			float fvolbar = TEXTURETYPE_PlaySound(&tr, vecSrc, vecSrc + (vecEnd-vecSrc)*2, BULLET_PLAYER_PARPAING);

			if ( g_pGameRules->IsMultiplayer() )
			{
				// override the volume here, cause we don't play texture sounds in multiplayer, 
				// and fvolbar is going to be 0 from the above call.

				fvolbar = 1;
			}

			// also play parpaing strike
			switch( RANDOM_LONG(0,1) )
			{
			case 0:
				EMIT_SOUND_DYN(ENT(m_pPlayer->pev), CHAN_ITEM, "weapons/cbar_hit1.wav", fvolbar, ATTN_NORM, 0, 98 + RANDOM_LONG(0,3)); 
				break;
			case 1:
				EMIT_SOUND_DYN(ENT(m_pPlayer->pev), CHAN_ITEM, "weapons/cbar_hit2.wav", fvolbar, ATTN_NORM, 0, 98 + RANDOM_LONG(0,3)); 
				break;
			}

			// delay the decal a bit
			m_trHit = tr;
		}

		m_pPlayer->m_iWeaponVolume = flVol * PARPAING_WALLHIT_VOLUME;
#endif
		m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + 1.0;
		
		SetThink( Smack );
		pev->nextthink = UTIL_WeaponTimeBase() + 0.2;

		
	}
	return fDidHit;
}

void CParpaing::WeaponIdle( void ) 
{ 
    ResetEmptySound(); 


	
	if (Lancement)
	{
			
m_pPlayer->SetAnimation( PLAYER_ATTACK1 );

// TRES IMPORTANT !!!!!!!!!!!!!
// LORS DE LA COMPILATION DU CLIENT IL FAUT NEUTRALISER CETTE FONCTION !!!!
// ME DEMANDEZ PAS POURQUOI, J'EN SAIS RIEN, CA PLANTE !
//	return;  // BLP : <= à décommenter si compilation du client
// (fab) un peu plus propre ce ke j'ai fé :] 
	// Wéwé...
#ifndef CLIENT_DLL
	CreateFlyingParpaing();

	m_pPlayer->m_iHasParpaing = 0;
	m_flNextSecondaryAttack = 6;
	m_pPlayer->pev->weapons &= ~(1<<16);
	DestroyItem();
	g_pGameRules->SetSpeed(m_pPlayer);
	g_pGameRules->GetNextBestWeapon( m_pPlayer, this );
	Lancement = false;
#endif
	
	}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	m_pPlayer->GetAutoaimVector( AUTOAIM_5DEGREES ); 

    if( m_flTimeWeaponIdle > UTIL_WeaponTimeBase() ) 
        return; 


    switch( RANDOM_LONG( 0, 1 ) ) 
    { 
        // on joue une animation "idle" au hasard 
        default: 
        case 0: SendWeaponAnim( PARPAING_IDLE ); break; 
        case 1: SendWeaponAnim( PARPAING_IDLE2 ); break; 
    } 

    // temps aléatoire avant de rappler cette fonction 
    m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + UTIL_SharedRandomFloat( m_pPlayer->random_seed, 10, 15 ); 
}


void CParpaing::PoseParpaing(void) 
{
	m_pPlayer->PoseParpaing = 1;
	ALERT(at_console,"pose du parpaing \n");
	EMIT_SOUND(ENT(m_pPlayer->pev),CHAN_VOICE,"weapons/pose_parpaing.wav",1,ATTN_NORM);
	m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + 0.1;
}
