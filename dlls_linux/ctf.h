#define	STATE_OFF	0
#define	STATE_ON	1

#define	CTF_TIME	50
#define	REWARD_FLAG	1


class CFlag:public CBasePlayerItem
{
	public:
		void Spawn();
		void Precache();
		void Touch(CBaseEntity *pOther);
		void Think();

		int	a;
		bool m_iState;

};
