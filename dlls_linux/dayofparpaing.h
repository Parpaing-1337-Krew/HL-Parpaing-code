class CCapturePoint : public CBaseTrigger
{
public:
	int		m_iCaptureAmount;
	float	m_flNextTouch;

	void	Spawn( void );
	void	Touch(CBaseEntity *pOther);
};



