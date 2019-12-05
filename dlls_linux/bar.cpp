#include "extdll.h"
#include "util.h"

#include "cbase.h"
#include "player.h"
#include "trains.h"
#include "nodes.h"
#include "weapons.h"
#include "soundent.h"
#include "monsters.h"
#include "shake.h"
#include "decals.h"
#include "gamerules.h"
#include "game.h"
//#include "sql.h"



extern int gmsgAlcool;
extern int gmsgBar;


void CBasePlayer::BarThink(void)
{


if (IsChaos)
	return; // On boi pas quand on est Ko

if ((pev->health <= 95) && (BarTime <= gpGlobals->time) && (m_iTeam != INSPECTEUR))
	{
		BarTime = gpGlobals->time + 1.5;
		TakeHealth(5,DMG_GENERIC);
		g_pGameRules->SetSpeed(this);
		m_iAlcool +=5;

		m_fTimeBar+=1.5; // tps passé dans le bar pour boire pis envoi pour les stats ensuite
		m_iHealhTaken+=5; // vie prise dan sle bar

		MESSAGE_BEGIN( MSG_ONE, gmsgAlcool, NULL ,this->edict());
			WRITE_SHORT(m_iAlcool ); //on uptdate l'état d'étililisme
		MESSAGE_END();
		EMIT_SOUND(ENT(this->pev),CHAN_VOICE,"alcool/action_boire.wav",1,ATTN_NORM);


	}
}

void CBasePlayer::EntreDansLeBar(void)
{
	if (m_iTeam != MACON1  &&  m_iTeam != MACON2)
		return; //pas d'inspecteur dans le bar 
	ALERT(at_console, "affichage sprte bar.cpp\n");
	MESSAGE_BEGIN( MSG_ONE, gmsgBar, NULL ,this->edict());
	WRITE_BYTE(1); //on affiche le sprite
	MESSAGE_END();

	m_fTimeBar=0; // tps passé dans le bar pour boire pis envoi pour les stats ensuite
	m_iHealhTaken=0; // vie prise dan sle bar


	if ((CVAR_GET_FLOAT("mp_derrick")) && (RANDOM_LONG(1, DERRICK_PROB) == 1))
		ClientPrint(pev, HUD_PRINTTALK, UTIL_VarArgs("[Derrick]: Et un jaune pour monsieur %s, un !\n", STRING(pev->netname)));
	WasInBar = true;
}

void CBasePlayer::SortDuBar(void)
{
	if (m_iTeam != MACON1  &&  m_iTeam != MACON2 )
		return; //pas d'inspecteur dans le bar 
	
	char son[128];
	int i;
	i = RANDOM_LONG(0,9);
	switch (i)
	{
	case 0:
	sprintf(son, "voix/macons/boisricard/br_bienbesoin.wav");
	break;
	case 1:
	sprintf(son, "voix/macons/boisricard/br_bonricard.wav");
	break;
	case 2:
	sprintf(son, "voix/macons/boisricard/br_faisdubien.wav");
	break;
	case 3:
	sprintf(son, "voix/macons/boisricard/br_mmmh.wav");
	break;
	case 4:
	sprintf(son, "voix/macons/boisricard/br_plusdeglacons.wav");
	break;
	case 5:
	sprintf(son, "voix/macons/boisricard/br_quelbonheur.wav");
	break;
	case 6:
	sprintf(son, "voix/macons/boisricard/br_quellechaleur.wav");
	break;
	case 7:
	sprintf(son, "voix/macons/boisricard/br_rafraichitidees.wav");
	break;
	case 8:
	sprintf(son, "voix/macons/boisricard/br_rot.wav");
	break;
	case 9:
	sprintf(son, "voix/macons/boisricard/br_jeanmimi.wav");
	break;
	}
	EMIT_SOUND(ENT(pev),CHAN_VOICE,son,1,ATTN_NORM); 

	//if (m_fTimeBar!=0 || m_iHealhTaken!=0)
	  //SQLWasInBar (this,m_fTimeBar,m_iHealhTaken);
	  ALERT(at_console, "desaffichage sprite bar.cpp\n");
	MESSAGE_BEGIN( MSG_ONE, gmsgBar, NULL ,this->edict());
	WRITE_BYTE(0); //on vire le sprite
	MESSAGE_END();
	if ((CVAR_GET_FLOAT("mp_derrick"))  && (RANDOM_LONG(1, DERRICK_PROB) == 1))
		ClientPrint(pev, HUD_PRINTTALK, UTIL_VarArgs("[Derrick]: A bientot monsieur %s.\n", STRING(pev->netname)));
	WasInBar = false;

}
