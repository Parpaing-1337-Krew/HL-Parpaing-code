#define	BOMBPARPAING_EXPLODETIME	30
#define	BOMBPARPAING_RADIUS			500
#define	BOMBPARPAING_DMG			500
#define	DEFUSE_TIME					200

class CBombParpaing : public CBasePlayerItem
{
public:
	int ExplodeTime;
	int	DefuseAmount;
	float	ClickTime;
	bool Defusage;
	void	Precache();
	void	Click();
	void	Spawn();
	void	Touch( CBaseEntity *pOther );
	void	Think();
	void	EXPORT	Use(CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value );
	int	ObjectCaps( void ) { return FCAP_CONTINUOUS_USE; }
	void	Defused();
	void   StopUse(CBasePlayer *pPlayer);
	CBasePlayer *pOwner;
};

class CBombParpaingWeapon : public CBasePlayerWeapon
{
public:

	int		m_iPoseAvc;

	void	Precache();
	void	Spawn();
	int		GetItemInfo( ItemInfo *p);
	BOOL	Deploy();
	void	Holster( int skiplocal );
	int		AddToPlayer( CBasePlayer *pPlayer );
	void	PrimaryAttack();
	void	SecondaryAttack();
	void	Touch(CBaseEntity *pOther);
	int iItemSlot( void ) { return 2; }
private:
	unsigned short m_usBombParpaing;
};