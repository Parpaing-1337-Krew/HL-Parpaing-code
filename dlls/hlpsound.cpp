//=========================================================
// hlp mp3 ent
//	codé dans :
// - hlp.h (déclaration)
// - hlpsound.cpp (fichier principal)
// - player.cpp (ajustement du son)
// - client.cpp (stop lors deconnection)
//=========================================================

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "player.h"
#include "hlp.h"
#include "gamerules.h"
#include "../engine/shake.h"
#include	"game.h"


extern int gmsgMp3;
int ChannelMp3; // channel mp3
bool AllMp3Init; // tout les mp3 ont 'il été initialisé ?

void CHlpsound::KeyValue( KeyValueData *pkvd )

{

            if (FStrEq(pkvd->szKeyName, "radius"))
            {
                        m_radius = atoi(pkvd->szValue);
                        pkvd->fHandled = TRUE;
            } else if(FStrEq(pkvd->szKeyName, "vol1"))
            {
                        vol1 = atoi(pkvd->szValue);
                        pkvd->fHandled = TRUE;
            }else if(FStrEq(pkvd->szKeyName, "mode"))
            {
                        Mode = atoi(pkvd->szValue); //. loop or once
                        pkvd->fHandled = TRUE;
            }
			else if(FStrEq(pkvd->szKeyName, "mod"))
            {
                        Mod = atoi(pkvd->szValue); // every where or not
                        pkvd->fHandled = TRUE;
            }
			else if(FStrEq(pkvd->szKeyName, "start"))
            {
                        Strt = atoi(pkvd->szValue);
                        pkvd->fHandled = TRUE;
            }else if (FStrEq(pkvd->szKeyName, "roomtype"))
            {
					szSoundFile = ALLOC_STRING(pkvd->szValue);
                        pkvd->fHandled = TRUE;
            }
                        CBaseEntity::KeyValue( pkvd );
}
LINK_ENTITY_TO_CLASS( hlp_mp3sound, CHlpsound );


void CHlpsound :: Spawn( void )
{	
	m_iSpriteTexture = PRECACHE_MODEL( "sprites/shockwave.spr" );

	if (!g_pGameRules->IsMultiplayer()) // donc c du solo et la ca plante donc petit msg + on enleve l'entité.
	{
		ALERT(at_console, "Les mp3 ne marchent pas en solo...\n"); //petit msg charmant
		UTIL_Remove( this );// kill l'entité
		return; // au cas ou...
	}

	//nouvo code = verif si le fichier exist
	FILE* fp;
	fp = fopen( (char*)STRING(szSoundFile), "r" );
	if ( !fp )
	{
		ALERT(at_console, "[ERROR] %s n existe pas.\n",(char*)STRING(szSoundFile) );
		UTIL_Remove( this );// kill l'entité
		return;
	}
	else
	{
		ALERT(at_console, "[OK] mp3 : %s\n",(char*)STRING(szSoundFile));
		fclose(fp); // on ferme le fichier kan même
	}
	
	// si les variables sont vide !
	if (!m_radius)
		m_radius=600;
	if (!vol1)
		vol1=255;
	if (!Mode)
		Mode=1;
	if (!Mod)
		Mod=1;
	if (!Strt)
		Strt=0;

	SetThink(Think2); // le think
	pev->nextthink = gpGlobals->time + 1.0; 
}

void Init(CBasePlayer *pPlayer)
{

	ALERT (at_console,"---| Demarrage configuration mp3 |---\n");
	// pas en solo !
	if (!g_pGameRules->IsMultiplayer()) // donc c du solo et la ca plante donc petit msg + on enleve l'entité.
	{
	ALERT(at_console, "Les mp3 ne marchent pas en solo...\n"); //petit msg charmant
	return; // au cas ou...
	}



	// envoit de la config son
	MESSAGE_BEGIN( MSG_ONE, gmsgMp3, NULL, pPlayer->pev );
	WRITE_BYTE (1);
	MESSAGE_END();
	ALERT (at_console,"* Config de la dll son (fmod).\n");
	//envoit de l'initialisation de tous les mp3 !
	CBaseEntity* pMp3 = NULL;
	ChannelMp3=0;
	while ( pMp3 = UTIL_FindEntityByClassname( pMp3, "hlp_mp3sound" )) // cherche toute les entitées mp3
	{
		if (pMp3)
		{
			ALERT (at_console,"* Initialise %s\n",(char*)STRING(GetClassPtr((CHlpsound *)pMp3->pev)->szSoundFile));
			GetClassPtr((CHlpsound *)pMp3->pev)->Chan = ChannelMp3;


				 if (GetClassPtr((CHlpsound *)pMp3->pev)->Strt!=-1){
				PlayMp3 (pPlayer,pMp3);
				GetClassPtr((CHlpsound *)pMp3->pev)->Playing = 1; // c'est bon , ca joue.
			}

			ALERT(at_console,"\tChannel \t: %i\n",ChannelMp3);
			//ALERT(at_console,"PLayMp3 : %i,%i\n",ChannelMp3,GetClassPtr((CHlpsound *)pMp3->pev)->Chan);


			if (GetClassPtr((CHlpsound *)pMp3->pev)->Strt==-1)
				ALERT(at_console,"\tStart \t\t: triggered\n");
			else 
				ALERT(at_console,"\tStart \t\t: %i seconde(s)\n",GetClassPtr((CHlpsound *)pMp3->pev)->Strt);

			if (GetClassPtr((CHlpsound *)pMp3->pev)->Mode==2)
				ALERT(at_console,"\tPlaymode \t\t: once\n");
			else
				ALERT(at_console,"\tPlaymode \t\t: loop\n");

			if (GetClassPtr((CHlpsound *)pMp3->pev)->Mod==1)
				ALERT(at_console,"\tSound \t\t: play everywhere\n");
			else
				ALERT(at_console,"\tSound \t\t: play with radius\n");
				ALERT(at_console,"\tVolume 1 \t\t: %i\n",GetClassPtr((CHlpsound *)pMp3->pev)->vol1);



		ChannelMp3++;
		}
	}

	ALERT (at_console,"---| Fin configuration mp3 |---\n");

}

void PlayMp3 (CBasePlayer *pPlayer,CBaseEntity *pMp3)
{


			MESSAGE_BEGIN( MSG_ONE, gmsgMp3, NULL, pPlayer->pev );
			WRITE_BYTE( 2 );
			WRITE_BYTE( GetClassPtr((CHlpsound *)pMp3->pev)->Chan ); // Chan
			//ALERT(at_console,"PLayMp3 : %i\n",GetClassPtr((CHlpsound *)pMp3->pev)->Chan);
			WRITE_STRING((char*)STRING(GetClassPtr((CHlpsound *)pMp3->pev)->szSoundFile));
			// options
			WRITE_BYTE( GetClassPtr((CHlpsound *)pMp3->pev)->Mode ); // 0= loop 1=once
			WRITE_BYTE( GetClassPtr((CHlpsound *)pMp3->pev)->Mod   ); // play everywhere ? 2=non 1=yes
			WRITE_BYTE( GetClassPtr((CHlpsound *)pMp3->pev)->vol1  ); // volume
			//---
			MESSAGE_END();
}


void CHlpsound :: Think2 (void)
{
	
	//ALERT (at_console,"%i\n",m_radius);


	CBaseEntity *pPlayer=NULL;
	while ( pPlayer = UTIL_FindEntityByClassname( pPlayer, "player" ))
	{
		if (pPlayer)
		{
			Distance = int((pev->origin - pPlayer->pev->origin).Length());
			vol2 = 255 -(vol1 * Distance / m_radius);
			if (Distance<m_radius && Mod==2) 
			{
				if (vol2<0)
					vol2=0;
			//ALERT (at_console,"%i:%i\n",Distance,vol2); //info du volume en fonction de la distance.
				MESSAGE_BEGIN( MSG_ONE, gmsgMp3, NULL, pPlayer->pev );
					WRITE_BYTE (5);
					WRITE_BYTE (Chan);
					WRITE_BYTE(vol2); 
				MESSAGE_END();
			}
			else
			{
				MESSAGE_BEGIN( MSG_ONE, gmsgMp3, NULL, pPlayer->pev );
					WRITE_BYTE (5);
					WRITE_BYTE (Chan);
					WRITE_BYTE(0); 
				MESSAGE_END();
			}
		}
	}
	if (CVAR_GET_FLOAT("mp_mp3")==1)
	{
		MESSAGE_BEGIN( MSG_PAS, SVC_TEMPENTITY, pev->origin );
			WRITE_BYTE( TE_BEAMCYLINDER );
			WRITE_COORD( pev->origin.x);
			WRITE_COORD( pev->origin.y);
			WRITE_COORD( pev->origin.z + 16);
			WRITE_COORD( pev->origin.x);
			WRITE_COORD( pev->origin.y);
			WRITE_COORD( pev->origin.z + m_radius); // reach damage radius over .3 seconds
			WRITE_SHORT( m_iSpriteTexture );
			WRITE_BYTE( 0 ); // startframe
			WRITE_BYTE( 0 ); // framerate
			WRITE_BYTE( 10 ); // life
			WRITE_BYTE( 16 );  // width
			WRITE_BYTE( 0 );   // noise
			if (vol2>=10)
			{
				WRITE_BYTE( vol2  ); // rouge
				WRITE_BYTE( 0 ); // vert
				WRITE_BYTE( 0  ); // bleu
			}
			else
			{
				WRITE_BYTE( 255 ); // vert
				WRITE_BYTE( 255  ); // bleu
				WRITE_BYTE( 255  ); // rouge
			}
			WRITE_BYTE( 255 ); //brightness
			WRITE_BYTE( 0 );		// speed
			MESSAGE_END();
	}
pev->nextthink = gpGlobals->time + 0.25; 
}


void StopSound(CBasePlayer *pPlayer )
{
	MESSAGE_BEGIN( MSG_ONE, gmsgMp3, NULL, pPlayer->pev );
	WRITE_BYTE (4); // 1=loading 2=play etc
	MESSAGE_END();
}
// demarrage avec un trigger...
void CHlpsound::Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
{



	if (Strt==-1 && !Playing) 
	{
	PlayMp3 (GetClassPtr((CBasePlayer *)pActivator->pev),this);
	Playing=1;
	} else {
	MESSAGE_BEGIN( MSG_ONE, gmsgMp3, NULL, GetClassPtr((CBasePlayer *)pActivator->pev)->pev);
	WRITE_BYTE (3); // 1=loading 2=play etc
	WRITE_BYTE(Chan); //channel à stopper
	MESSAGE_END();
	Playing=0;
	}

}