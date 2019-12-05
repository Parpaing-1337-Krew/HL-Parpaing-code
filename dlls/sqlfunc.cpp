#include "extdll.h"
#include "util.h"
#include "sql.h"
#include "time.h"
#include "cbase.h"
#include "player.h"
#include "mur.h"
#include "game.h"
#include "gamerules.h"
#include "shellapi.h"


char Addr[20]; // adresse ip
bool getIP; // a deja cherché l'ip du seveur

char* SQLRequet[30];

 // stockage temporaire de max 1 minute pour pas flooder le server mysql.
unsigned char row=0; // pointeur de la ligne , zero au debut . max= 30 

void SQLAddRequete(char* requete)
{
// ajoute les requetes dans le tableau temporaire

ShellExecute(NULL, "open","hl-parpaing\\dlls\\sql.exe", requete, NULL,SW_HIDE );

}



BOOL SQLIp (void)
{
   byte ip[4]; 

	if (!getIP)
	{

   LPHOSTENT   lpHost; 
   unsigned long IP; 
   char szHostName[256]; 
   DWORD cbHostName; 

   WSADATA WSAData; 
   WSAStartup( MAKEWORD(1,1), &WSAData ); 

   cbHostName = sizeof( szHostName ); 
   GetComputerName( szHostName, &cbHostName ); 
   lpHost = gethostbyname( szHostName ); 
   IP = ( (LPIN_ADDR)lpHost->h_addr )->s_addr; 

   ip[0] = (IP >> 0) & 0x000000FF; 
   ip[1] = (IP >> 8) & 0x000000FF; 
   ip[2] = (IP >> 16) & 0x000000FF; 
   ip[3] = (IP >> 24) & 0x000000FF; 

		sprintf(Addr,"%i.%i.%i.%i:%i\n",ip[0],ip[1],ip[2],ip[3],(int)CVAR_GET_FLOAT("port"));
		getIP=1;
	}

if(ip[0]==127 && ip[1]==0 && ip[2]==0 && ip[3]==1)
return false;
else
return true;


}


const char *SQLGetPlayerInfo(CBasePlayer *pPlayer)
{
return UTIL_VarArgs("%s,,,%i,,,%i",STRING(pPlayer->pev->netname),pPlayer->m_iTeam,(int)pPlayer->pev->frags);
}










///////////////////////////////////////////////////////////////////////////////////////////

// pose d'un parpaing
void SQLPoseParpaing (CBasePlayer *pPlayer)
{

SQLIp(); // recherche l'ip.

	CBaseEntity *mur;

	if (pPlayer->m_iTeam==1)
		mur = UTIL_FindEntityByClassname(NULL, "mur1");
	else
		mur = UTIL_FindEntityByClassname(NULL, "mur2");

	CMur *pMur;
	pMur = GetClassPtr((CMur *)mur->pev);

	// c'est okay pour l'heure :]
	time_t heure=time(NULL);

	char infos[60];
	sprintf(infos,"%s,,,%i,,,%i",STRING(pPlayer->pev->netname),pPlayer->m_iTeam,(int)pPlayer->pev->frags);
	SQLAddRequete (UTIL_VarArgs("insert into hlp_game values ('','%i','%s','1','%s','%i','','0')",heure,Addr,infos, pMur->avancement));
}

void SQLDeathNotice (char* pVictim,char* pKiller,char* pevInflictor)
{
	SQLIp(); // recherche l'ip.

	time_t heure=time(NULL);

	SQLAddRequete (UTIL_VarArgs("insert into hlp_game values ('','%i','%s','2','%s','%s','%s','0')",heure,Addr,pVictim,pKiller, pevInflictor));
}

void SQLWasInBar (CBasePlayer *pPlayer,float m_fTimeBar,int m_iHealhTaken)
{
	SQLIp(); // recherche l'ip.

	time_t heure=time(NULL);
	char infos[60];
	sprintf(infos,"%s,,,%i,,,%i",STRING(pPlayer->pev->netname),pPlayer->m_iTeam,(int)pPlayer->pev->frags);
	SQLAddRequete (UTIL_VarArgs("insert into hlp_game values ('','%i','%s','4','%s','%i','%i','0')",heure,Addr,infos,(int)m_fTimeBar, m_iHealhTaken));

}

void SQLBlame (CBasePlayer *pAttacker,CBasePlayer *pVictim)
{
SQLIp(); // recherche l'ip.

time_t heure=time(NULL);

	char infos[60];
	char infos2[60];
	sprintf(infos,"%s,,,%i,,,%i",STRING(pVictim->pev->netname),pVictim->m_iTeam,(int)pVictim->pev->frags);
	sprintf(infos2,"%s,,,%i,,,%i",STRING(pAttacker->pev->netname),pAttacker->m_iTeam,(int)pAttacker->pev->frags);

SQLAddRequete (UTIL_VarArgs("insert into hlp_game values ('','%i','%s','3','%s','%s','','0')",heure,Addr,infos2,infos));

}

void SQLShow (void)
{
int a;
a=0;
ALERT(at_console,"Liste des requetes en attente :\n");
while (a<row)
	{	
	ALERT(at_console,"[%i] %s\n",a,SQLRequet[a]);
		a++;
	}
ALERT(at_console,"Prochain envoi : %i sec\n",(int)(g_pGameRules->SQLNxtSend - gpGlobals->time));


}


void SQLServerStatus (void)
{
// envoie ip,nom du server,map,nombre de joueurs et leur nom
SQLIp(); // recherche l'ip.

char namelist[256];
time_t heure=time(NULL);

// players et maxplayers
sprintf( namelist, "%i",gpGlobals->maxClients);
			for (int i = 1;i<= gpGlobals->maxClients; i++)
			{
				CBaseEntity *pPlayer2 = UTIL_PlayerByIndex( i );
				if ( pPlayer2 )
				{
				CBasePlayer *pPlayer3;
				pPlayer3 = GetClassPtr((CBasePlayer *)pPlayer2->pev);

				sprintf( namelist, "%s;%s,,,%i,,,%i",namelist,STRING(pPlayer3->pev->netname),pPlayer3->m_iTeam,(int)pPlayer3->pev->frags);
				}
			}

int password;
if ( !strcmp(CVAR_GET_STRING("sv_password"),""))
password=0;
else
password=1;

SQLAddRequete (UTIL_VarArgs("insert into hlp_server values ('%i','%s','%s','%s','%s','%i','%i')",heure,Addr,CVAR_GET_STRING("hostname"),( char * )STRING( gpGlobals->mapname ), namelist,(int)CVAR_GET_FLOAT("sv_dbhidden"),password));
}

