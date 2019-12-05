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
#include "teamplay_gamerules.h"
#include "hlp.h"
#include "mur.h"

#include "auto-help.h"

	
void CBasePlayer::AutoHelpUnset(int id)
{
	AutoHelpList[id][AH_STATE] = 1;
}

void CBasePlayer::AutoHelpSet(int id, int time)
{
	AutoHelpList[id][AH_TIME] = gpGlobals->time + time;
	if (AutoHelpList[id][AH_STATE] == 2)
			AutoHelpList[id][AH_STATE] = 0;
}

void CBasePlayer::AutoHelpInit()
{
	for (int i=0;i<AH_NB_ID;i++)
	{
		AutoHelpList[i][AH_TIME] = 0;
		AutoHelpList[i][AH_STATE] = 2;
	}
}

void CBasePlayer::AutoHelpCheck()
{
	for (int i=0;i<AH_NB_ID;i++)
	{
		if ((AutoHelpList[i][AH_TIME]<gpGlobals->time) && (AutoHelpList[i][AH_STATE] == 0))
			AutoHelp(i);
	}
}

void CBasePlayer::AutoHelp(int id)
{
	if (AutoHelpList[id][AH_STATE]) 
		return;
	char msg[128];
	switch (id)
	{
	case 0:
		sprintf(msg, "Bienvenue dans HL-Parpaing.\nJe suis l'inspecteur DERRICK, votre guide.\n\n\n");
		break;
	case 1:
		sprintf(msg, "Les MACONS construisent un mur,\nles INSPECTEURS doivent les en empecher.\n\n\n");
		break;
	case 2:
		sprintf(msg, "L'arme que vous tenez est une TRUELLE, tres utile pour detruire les murs.\n\n\n");
		break;
	case 3:
		sprintf(msg, "Utilisez le SIFFLET pour arreter les MACONS, le CARNET pour les verbaliser.\n\n\n");
		break;
	case 4:
		sprintf(msg, "Vous venez d'obtenir un PARPAING, prenez-en soin.\n\n\n");
		break;
	case 5:
		sprintf(msg, "Lancez-le en utilisant la touche de tir secondaire.\n\n\n");
		break;
	case 6:
		sprintf(msg, "Le parpaing change de direction selon vos mouvements.\n\n\n");
		break;
	case 7:
		sprintf(msg, "Prenez le parpaing, placez-vous face à un mur.\n\n\n");
		break;
	case 8:
		sprintf(msg, "Pressez la touche USE.\nSi rien ne se passe, rapprochez-vous et recommencez.\n\n\n");
		break;
	case 9:
		sprintf(msg, "Ce mur appartient a l'ennemi.\nCherchez le votre.\n\n\n");
		break;
	case 10:
		sprintf(msg, "Pressez la touche USE pendant 5 secondes sans vous eloigner.\n\n\n");
		break;
	case 11:
		sprintf(msg, "Vous venez de poser votre premier parpaing.\nBravo.\n\n\n");
		break;
	case 12:
		sprintf(msg, "L'avancement du mur s'affiche entre parentheses.\n\n\n");
		break;
	case 13:
		sprintf(msg, "Le parpaing change de direction selon vos mouvements.\n\n\n");
		break;
	}
	AutoHelpList[id][AH_STATE]=1;
	Vector v1(255,50,255);
	Vector v2(0,0,0);
	UTIL_MessageEffect( this, msg, 2, -1, 1, 0, v1, 255, v2,0, 1, 1, 3, 0);
}
