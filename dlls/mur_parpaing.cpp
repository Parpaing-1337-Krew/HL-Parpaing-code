#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "animation.h"
#include "effects.h"
#include "player.h"
#include "gamerules.h"
#include "weapons.h"
#include "monsters.h"
#include "nodes.h"
#include "soundent.h"
#include "decals.h"
#include "mur.h"
#include "sql.h"



extern int gmsgTime;
LINK_ENTITY_TO_CLASS( mur1, CMur );
LINK_ENTITY_TO_CLASS( mur2, CMur );
int Tps=0;


void CMur::TraceAttack( entvars_t *pevAttacker, float flDamage, Vector vecDir, TraceResult *ptr, int bitsDamageType)
{
	TakeDamage( pevAttacker, flDamage );
}

int CMur :: TakeDamage( entvars_t *pevAttacker, float flDamage)
{
	if (pev->health==0)
		return 0;
	CBasePlayer *pAttacker = NULL;
	CBaseEntity *ktmp = CBaseEntity::Instance( pevAttacker );
	if ( ktmp && (ktmp->Classify() == CLASS_PLAYER) )
		pAttacker = (CBasePlayer*)ktmp;
	if (pAttacker->HasNamedPlayerItem("weapon_truelle")) //dommage fois 2.5
	{
		flDamage *=2.5;
	}
	if (pAttacker->HasNamedPlayerItem("weapon_parpaing")) //dommages divisé par 3.5
	{
		flDamage /= 3.5;
	}

	pev->health -= flDamage;
	if (pev->health <= 0)
	{
		pev->health = 0;
	}
	avancement = pev->health/(MUR_HEALTH-1)*100;
	
	if (pev->health == 0)
	{
		switch( RANDOM_LONG(0,1) )
		{
		case 0:
			EMIT_SOUND(ENT(pev), CHAN_ITEM, "debris/bustconcrete1.wav", 1, ATTN_NORM); break;
		case 1:
			EMIT_SOUND(ENT(pev), CHAN_ITEM, "debris/bustconcrete2.wav", 1, ATTN_NORM); break;
		}
		if ((CVAR_GET_FLOAT("mp_derrick"))
			&& (RANDOM_LONG(1, DERRICK_PROB) == 1)
			&&(ktmp && (ktmp->Classify() == CLASS_PLAYER)))
		{
			UTIL_ClientPrintAll( HUD_PRINTTALK, UTIL_VarArgs("[Derrick]: Un mur detruit par %s ! Ca t'amuse p'tit con ?\n", STRING(pAttacker->pev->netname)));
			UTIL_LogPrintf("Un mur a ete detruit grace a %s\n", STRING(pAttacker->pev->netname));
		}
		else if  ((CVAR_GET_FLOAT("mp_derrick"))  && (RANDOM_LONG(1, DERRICK_PROB) == 1))
		{
			UTIL_ClientPrintAll( HUD_PRINTTALK, "[Derrick]: Un mur a ete detruit !\n");
			UTIL_LogPrintf("Un mur a ete detruit\n");
		}
		else
		{
			UTIL_LogPrintf("Un mur a ete detruit\n");
		}

	}
			
	Anim();
	CBaseEntity *mur1 = UTIL_FindEntityByClassname(NULL, "mur1");
	CBaseEntity *mur2 = UTIL_FindEntityByClassname(NULL, "mur2");



		MESSAGE_BEGIN ( MSG_ALL, gmsgTime);
		WRITE_SHORT ( 2 );
		WRITE_BYTE ( GetClassPtr((CMur *)mur1->pev )->avancement ); // 51
		WRITE_BYTE ( GetClassPtr((CMur *)mur2->pev )->avancement ); // ricard
		MESSAGE_END();



	return 1;
}

#define PI        (3.14159265358979323846264338327950288f)

void CMur :: Spawn( void )
{
	avancement = 0;
	Precache();
	SET_MODEL( ENT(pev), "models/mur.mdl" ); // MiK : ici tu fous le bon model
	pev->health		= 0;
	pev->takedamage	= DAMAGE_YES;
	pev->movetype	= MOVETYPE_STEP;
	pev->solid		= SOLID_BBOX;
	pev->sequence	= 0; // sequence 0 (pas de parpaing)
	
	//SetModelCollisionBox(); // SetSize auto
	

	// code un peu brut ne permettant que 2 position de mur, on va essayé de faire mieux.....
	if (pev->angles.y==0 || pev->angles.y==180)
	UTIL_SetSize( pev, Vector(-80,-15,0), Vector(80,15,85)); // MiK : ici tu regle la taille en fonction de la HitBox (la taille c comme sous WC normalement)
	else
	UTIL_SetSize( pev, Vector(-15,-80,0), Vector(15,80,85)); // MiK : ici tu regle la taille en fonction de la HitBox (la taille c comme sous WC normalement)


	// (fab) aller on va tenter de la trigo (en thérie c du cours de 3eme , ca devrai se faire , mais le contexte est inédit...)
	// on ma rarement demandé dans un controle de rechercher les coordonnée d'un hitbox pour un mur de parpaing.
	/*xa	= cos (pev->angles.y) * 80; // 80 = demi longueur d'un mur 
	ya	= sin (pev->angles.y) * 80; // 80 = demi longueur d'un mur 
	xb	=	-xa; // les coordonnée de b sont justes une symetrie centrale....
	yb	=	-ya;*/

	/*UTIL_MakeVectors(pev->angles);


	float longe = sqrt(6625); // pythagore 
	float angl = acos((80/sqrt(6625))) / (2*PI/360); // acos = resultat en radian, donc on passe en degree pour l'addition suivante
	float angletotal = pev->angles.y + angl;
	angletotal = angletotal * (2*PI/360);// on reconverti en radian pour le cos ....

	ALERT(at_console,"%f\n",angletotal);
	int xa,ya,xb,yb;
	xa	= cos (angletotal) * sqrt(6625); 
	ya	= sin (angletotal) * sqrt(6625); // 80 = demi longueur d'un mur 
	xb	=	-xa; // les coordonnée de b sont justes une symetrie centrale....
	yb	=	-ya;

	// faut ke xa et ya et xb et yb soit de même signe...
	if (xa<0 && ya<0)
	{
	xa=xa*-1;
	ya=ya*-1;
	xb=xb*-1;
	yb=yb*-1;
	}

	if (xa>0 && ya<0)
	{
	ya=ya*-1;
	yb=yb*-1;
	}

	ALERT (at_console,"mur xa :%i ya:%i xb : %i yb:%i\n",xa,ya,xb,yb);
	UTIL_SetSize( pev, Vector(xb,yb,0), Vector(xa,ya,85)); 
*/
	pev->nextthink = gpGlobals->time + 0.1;
	pev->frame = RANDOM_FLOAT(0,255);
 	//SetUse( Use2 );
	if (!strcmp(STRING(pev->classname), "mur1"))
	{
		team = 1;
		if (CVAR_GET_FLOAT("mp_murcolor") != 0)
		{
		pev->renderfx = kRenderFxGlowShell;
		pev->renderamt = 10;
		pev->rendercolor = Vector( 50, 50, 255 );	// RGB
		}
	}
	if (!strcmp(STRING(pev->classname), "mur2"))
	{
		team = 2;
		if (CVAR_GET_FLOAT("mp_murcolor") != 0)
		{
		pev->renderfx = kRenderFxGlowShell;
		pev->renderamt = 10;
		pev->rendercolor = Vector( 255, 50, 50 );	// RGB
		}
	}
}
void CMur :: Precache( void )
{
	PRECACHE_MODEL( "models/mur.mdl" ); // MiK : pas oublier de précacher sinon caca prout
	PRECACHE_SOUND( "debris/bustconcrete1.wav");
	PRECACHE_SOUND( "debris/bustconcrete2.wav");

}
void CMur :: Anim (void)
{
		anim = avancement/10;
		pev->sequence = anim;
}
void CMur :: Think( void )
{
	StudioFrameAdvance();
	pev->nextthink = gpGlobals->time + 0.1;
}

int CMur :: IPointsForPose( void )
{
	return CVAR_GET_FLOAT("mp_valeurparpaing");
}


void CMur::Use(CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
 {
 	CBasePlayer *pPlayer;
	if ( pActivator->IsPlayer() )
		pPlayer = (CBasePlayer*)pActivator;
	if ((pPlayer->m_pActiveItem) && ( pPlayer->m_pActiveItem->m_iId == 16) && pPlayer->m_iTeam == team)
	{
		pPlayer->PoseParpaing = 1;
		EMIT_SOUND(ENT(pPlayer->pev),CHAN_VOICE,"weapons/pose_parpaing.wav",1,ATTN_NORM);
	}
	else
	{
		char msg[50];
		sprintf(msg,"Avancement : %i %%",avancement);
		switch(team)
		{
		case 1:
			UTIL_MessageEffect(pPlayer,msg,1,-1,-1,0,Vector(50,50,255),255,Vector(0,0,0),0,1,1,1,0);
			break;
		case 2:
			UTIL_MessageEffect(pPlayer,msg,1,-1,-1,0,Vector(255,50,0),255,Vector(0,0,0),0,1,1,1,0);
			break;
		}
	}

}




void CMur::Pose(CBasePlayer *pPlayer)
{

	//ALERT(at_console,"on pose...\n");
	//ICI tu met les actions quise passe kan un joueur "utilise" le mur
	//BLP : ;> oki, merci MiK


		pPlayer->RemovePlayerItem(pPlayer->m_pActiveItem);
		pPlayer->SelectItem ("weapon_truelle");
		pPlayer->pev->weapons &= ~(1<<16);
		pPlayer->m_iHasParpaing = 0;


		g_pGameRules->SetSpeed(pPlayer);
		pPlayer->AddPoints(REWARD_PARPAING, 1);
		pPlayer->m_flNextDecalTime = gpGlobals->time;
		UTIL_LogPrintf( "%s pose un parpaing pour %s\n",STRING(pPlayer->pev->netname),pPlayer->m_szTeamName) ;
		//pPlayer->UpdateScore();
		//pPlayer->UpdateTeamName(0);
		pev->health += 1000;
		avancement += 10;  // (fab) je trouve ca zarb ....pas linéaire
		
		if (avancement>100)
			avancement=100;
		
		g_pGameRules->SetSpeed(pPlayer);

		SQLPoseParpaing(pPlayer);

				
	CBaseEntity *mur1 = UTIL_FindEntityByClassname(NULL, "mur1");
	CBaseEntity *mur2 = UTIL_FindEntityByClassname(NULL, "mur2");

		MESSAGE_BEGIN ( MSG_ALL, gmsgTime);
		WRITE_SHORT ( 2 );
		WRITE_BYTE ( GetClassPtr((CMur *)mur1->pev )->avancement ); // 51
		WRITE_BYTE ( GetClassPtr((CMur *)mur2->pev )->avancement ); // ricard
		MESSAGE_END();




		UTIL_LogPrintf( "Avancement du mur %s : %i %%\n",pPlayer->m_szTeamName,avancement);
		char msg[50];
		sprintf(msg,"%s pose un parpaing (%i%%)",STRING(pPlayer->pev->netname),avancement);
		EMIT_SOUND(ENT(pev),CHAN_VOICE,"feedback/alarm.wav",1,ATTN_NORM);
		if (avancement < 100)
		{
			for (int i = 1;i<= gpGlobals->maxClients; i++)
			{
				CBaseEntity *pPlayer2 = UTIL_PlayerByIndex( i );
				if ( pPlayer2 )
				{
						switch(team)
					{
					case 1:
						UTIL_MessageEffect(pPlayer2,msg,1,-1,-1,0,Vector(50,50,255),255,Vector(0,0,0),0,1,1,1,0);
						break;
					case 2:
						UTIL_MessageEffect(pPlayer2,msg,1,-1,-1,0,Vector(255,50,50),255,Vector(0,0,0),0,1,1,1,0);
						break;
					}
				}
			}
		}
		else
		{

			// envoi un msg au client pour faire afficher ke le roudn est fini
		/*		MESSAGE_BEGIN ( MSG_BROADCAST, gmsgTime );
				WRITE_BYTE ( 0 );
				WRITE_BYTE ( 100 );
				MESSAGE_END();*/

			UTIL_LogPrintf( "%s a fini un mur",pPlayer->m_szTeamName);
			pPlayer->AddPoints(REWARD_MUR, 0);
			char msg[50];
			sprintf(msg,"%s construit un mur !",pPlayer->m_szTeamName);
			for (int i = 1;i<= gpGlobals->maxClients; i++)
			{
				CBaseEntity *pPlayer2 = UTIL_PlayerByIndex( i );
				if ( pPlayer2 )
				{
					switch(team)
					{
					case 1: // team bleu (51)
						EMIT_SOUND(ENT(pPlayer2->pev),CHAN_VOICE,"voix/commentaires/macons_gagnent/macons[51]_gagnent.wav",1,ATTN_NORM);
						UTIL_MessageEffect(pPlayer2,msg,1,-1,-1,0,Vector(50,50,255),255,Vector(0,0,0),0,1,1,1,0);
					break;
					case 2: // team rouge (ricard)
						EMIT_SOUND(ENT(pPlayer2->pev),CHAN_VOICE,"voix/commentaires/macons_gagnent/macons[ricard]_gagnent.wav",1,ATTN_NORM);
						UTIL_MessageEffect(pPlayer2,msg,1,-1,-1,0,Vector(255,50,50),255,Vector(0,0,0),0,1,1,1,0);
					break;
					} 
				}
			}
			pev->health=0;
			avancement = 0;
			ResetAvancement();
		}
		Anim();
}

void ResetAvancement (void)
{
	CBaseEntity *mur1 = UTIL_FindEntityByClassname(NULL, "mur1");
	CBaseEntity *mur2 = UTIL_FindEntityByClassname(NULL, "mur2");

		GetClassPtr((CMur *)mur1->pev )->avancement =0;
		GetClassPtr((CMur *)mur2->pev )->avancement =0;
		GetClassPtr((CMur *)mur1->pev )->pev->health =0;
		GetClassPtr((CMur *)mur2->pev )->pev->health =0;
		GetClassPtr((CMur *)mur1->pev )->Anim();
		GetClassPtr((CMur *)mur2->pev )->Anim();


		MESSAGE_BEGIN ( MSG_ALL, gmsgTime);
		WRITE_SHORT ( 2 );
		WRITE_BYTE ( GetClassPtr((CMur *)mur1->pev )->avancement ); // 51
		WRITE_BYTE ( GetClassPtr((CMur *)mur2->pev )->avancement ); // ricard
		MESSAGE_END();

		int nbmacon=0;



		for (int i = 1;i<= gpGlobals->maxClients; i++)
		{
			CBaseEntity *pPlayer = UTIL_PlayerByIndex( i );
			if (pPlayer)
			{
				CBasePlayer *pPlayer2 = (CBasePlayer*)pPlayer;
				if (pPlayer2->m_iTeam == MACON1 || pPlayer2->m_iTeam == MACON2)
				nbmacon++;
			}
		}

		MESSAGE_BEGIN ( MSG_ALL, gmsgTime);
		WRITE_SHORT ( 4 );
		WRITE_BYTE ( nbmacon ); 
		WRITE_BYTE ( 0 ); 
		MESSAGE_END();
}