/***
*
*	Copyright (c) 1996-2001, Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id 
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*	All Rights Reserved.
*
*   This source code contains proprietary and confidential information of
*   Valve LLC and its suppliers.  Access to this code is restricted to
*   persons who have executed a written SDK license with Valve.  Any access,
*   use or distribution of this code by or to any unlicensed person is illegal.
*
****/
//=========================================================
// cockroach
//=========================================================


#define		ROACH_IDLE				0
#define		ROACH_BORED				1
#define		ROACH_SCARED_BY_ENT		2
#define		ROACH_SCARED_BY_LIGHT	3
#define		ROACH_SMELL_FOOD		4
#define		ROACH_EAT				5

//=========================================================
// Megret
//=========================================================
#define MEGRET_TIME					300	//temps en secondes avant la resurrection de megret
#define MEGRET_HEALTH				1000
#define MEGRET_NOM					"Bruno Maigrelet"
#define	MEGRET_SURNOM				"Nabot"

#define	REWARD_MEGRET				10

//=========================================================
// Monster's Anim Events Go Here
//=========================================================
class CMegret : public CBaseMonster
{
public:
	void EXPORT	FUse(CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value );
	void Spawn( void );
	void Precache( void );
	void SetYawSpeed( void );
	void EXPORT MonsterThink ( void );
	void Move ( float flInterval );
	void PickNewDest ( int iCondition );
	void EXPORT Touch ( CBaseEntity *pOther );
	void Killed( entvars_t *pevAttacker, int iGib );
	void TraceAttack( entvars_t *pevAttacker, float flDamage, Vector vecDir, TraceResult *ptr, int bitsDamageType);

	float	m_flLastLightLevel;
	float	m_flNextSmellTime;
	int		Classify ( void );
	void	Look ( int iDistance );
	int		ISoundMask ( void );
	
	// UNDONE: These don't necessarily need to be save/restored, but if we add more data, it may
	BOOL	m_fLightHacked;
	int		m_iMode;
	// -----------------------------
};