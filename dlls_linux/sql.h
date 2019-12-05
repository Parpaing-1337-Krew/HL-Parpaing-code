#include <winsock.h>
#include "mysql.h"

void SQLAddRequete(char* requete );
void SQLShow (void);
BOOL SQLIp (void);
BOOL SQLExist(char* requete );
const char *SQLGetPlayerInfo(CBasePlayer *pPlayer);

void SQLServerStatus (void);
void SQLPoseParpaing (CBasePlayer *pPlayer);
void SQLDeathNotice (char* pVictim,char* pKiller,char* pevInflictor);
void SQLWasInBar (CBasePlayer *pPlayer,float m_fTimeBar,int m_iHealhTaken);
void SQLBlame (CBasePlayer *pAttacker,CBasePlayer *pVictim);


