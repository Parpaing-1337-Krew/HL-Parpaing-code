#include	"extdll.h"
#include	"util.h"
#include	"cbase.h"
#include	"player.h"
#include	"triggers.h"
#include	"dayofparpaing.h"


LINK_ENTITY_TO_CLASS( trigger_capture_point, CCapturePoint );

void CCapturePoint::Spawn()
{
	InitTrigger();
	SetTouch(Touch);
	m_iCaptureAmount = 0;
	m_flNextTouch = 0;
}

void CCapturePoint::Touch(CBaseEntity *pOther)
{
	if (m_flNextTouch > gpGlobals->time)
		return;
	if (!pOther->IsPlayer())
	{
		return;
	}
	else
	{
		CBasePlayer *pPlayer = (CBasePlayer*)pOther;
		m_iCaptureAmount ++;
		ClientPrint(pPlayer->pev, HUD_PRINTCENTER, UTIL_VarArgs("Amount : %i", m_iCaptureAmount));
		m_flNextTouch = gpGlobals->time + 1;
	}
}