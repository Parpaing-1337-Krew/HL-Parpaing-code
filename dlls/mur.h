#define		MUR_HEALTH		10000 
#define		REWARD_MUR		30
#define		REWARD_PARPAING	3

class CMur : public CBaseToggle
{
public:
	int			anim; // anim du mur
	int			team;
	int			avancement;
	void		TraceAttack( entvars_t *pevAttacker, float flDamage, Vector vecDir, TraceResult *ptr, int bitsDamageType);
	int			TakeDamage( entvars_t *pevAttacker, float flDamage );
	void		Spawn( void );
	void		Pose(CBasePlayer *pPlayer);
	void		Precache( void );
	void		Anim (void);
	void		Use(CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value );
	void		Think( void );
	EXPORT void Pose (void);
	int			IPointsForPose(void);
	int			ObjectCaps( void ) { return (CBaseToggle::ObjectCaps() & ~FCAP_ACROSS_TRANSITION) | FCAP_IMPULSE_USE; }
	private:
};

void ResetAvancement (void);
