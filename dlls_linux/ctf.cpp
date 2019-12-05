#include	"extdll.h"
#include	"util.h"
#include	"cbase.h"
#include	"gamerules.h"
#include	"player.h"

#include	"weapons.h"

#include "ctf.h"

LINK_ENTITY_TO_CLASS(item_flag, CFlag);

void CFlag::Precache()
{
	PRECACHE_MODEL("models/flag.mdl");
	PRECACHE_SOUND("take_flag.wav");
}

void CFlag::Spawn()
{
	Precache();
	SET_MODEL( ENT(pev), "models/flag.mdl" );
	pev->takedamage	= DAMAGE_NO;
	pev->movetype	= MOVETYPE_STEP;
	pev->solid		= SOLID_TRIGGER;
	pev->sequence	= 0; // sequence 0 (pas de parpaing)
	pev->rendermode = kRenderTransTexture;
	pev->renderamt = 0;
	a = 1;
	m_iState = STATE_OFF;
	SetThink(&CFlag::Think);
	SetTouch(NULL);
	pev->nextthink = gpGlobals->time + 0.1;

}

void CFlag::Touch( CBaseEntity *pOther)
{
	if (m_iState == STATE_OFF)
		return;
	if (!pOther->IsPlayer())
		return;
	CBasePlayer *pPlayer = (CBasePlayer*)pOther;

	
	if ((pPlayer->m_iTeam != MACON1) && (pPlayer->m_iTeam != MACON2)) 
		return;





if (pPlayer->m_iHasParpaing == 1)
{

if(pPlayer->MsgTime <= gpGlobals->time)
{
ClientPrint(pPlayer->pev, HUD_PRINTTALK, UTIL_VarArgs("Transporter des parpaings ou boire... il faut choisir\n"));
pPlayer->MsgTime = gpGlobals->time + 5;
return;
}
else
return;
}








	
	Spawn();
	EMIT_SOUND(ENT(pev), CHAN_ITEM, "take_flag.wav", 1, ATTN_NORM);
	pPlayer->AddPoints(REWARD_FLAG, FALSE );
	g_pGameRules->m_iActiveFlags--;
	if (g_pGameRules->m_iActiveFlags > 1)
		ClientPrint(pPlayer->pev, HUD_PRINTTALK, UTIL_VarArgs("Il reste %i bouteilles.\n", g_pGameRules->m_iActiveFlags));
	else if (g_pGameRules->m_iActiveFlags == 1)
		ClientPrint(pPlayer->pev, HUD_PRINTTALK, UTIL_VarArgs("Allez, une derniere pour la route !\n"));
	else
		ClientPrint(pPlayer->pev, HUD_PRINTTALK, UTIL_VarArgs("A la tienne, %s.\n", STRING(pPlayer->pev->netname)));
	if (!g_pGameRules->m_iActiveFlags)
			g_pGameRules->m_flCtfTime = gpGlobals->time;
}	

void CFlag::Think()
{
	pev->nextthink = gpGlobals->time + 1;
}	

void CHalfLifeMultiplay::CtfInit()
{
	m_iCtf = 1;
	m_iActiveFlags = 0;
	CBaseEntity *pEnt = NULL;
	while(pEnt = UTIL_FindEntityByClassname( pEnt, "item_flag"))
		{
		if (pEnt && RANDOM_LONG(0,1))
			{
				CFlag *pFlag = (CFlag*)pEnt;
				pFlag->m_iState = STATE_ON;
				pFlag->pev->renderamt = 255;
//				pFlag->SetTouch(pFlag->Touch);
				m_iActiveFlags ++;
			}
		}
	Vector v1(0,255,0);
	Vector v2(0,0,0);
	if (m_iActiveFlags > 1)
		UTIL_MessageEffectAll( UTIL_VarArgs( "Capture %i bouteilles", m_iActiveFlags), 1, -1, -1, 0, v1, 255, v2,0,1,1,1,0);
	else if (m_iActiveFlags == 1)
		UTIL_MessageEffectAll( UTIL_VarArgs( "Capture %i bouteille", m_iActiveFlags), 1, -1, -1, 0, v1, 255, v2,0,1,1,1,0);
	else
	{
		m_iCtf = 0;
		m_flCtfTime = gpGlobals->time + CVAR_GET_FLOAT("mp_ctf_time");
	}
}

void CHalfLifeMultiplay::CtfTest()
{
	if (gpGlobals->time < m_flCtfTime)
		return;
	if ((m_iActiveFlags == 0) && (m_iCtf)) 
	{
			m_iCtf = 0;
			m_flCtfTime = gpGlobals->time + CVAR_GET_FLOAT("mp_ctf_time");
			return;
	}
	if ((m_iCtf) || (RANDOM_LONG(0,1)))
	{
		m_flCtfTime = gpGlobals->time + CVAR_GET_FLOAT("mp_ctf_time");
		return;
	}
	CtfInit();
}
