class CHlpsound : public CBaseEntity
{
public:
            void Spawn( void );
			void SetVol(int dist);
            void KeyValue( KeyValueData *pkvd );
			void Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value );
			void EXPORT Think2( void );
			

			int Distance;
			float Unit;
			bool ShowR; // montre le radius :]
			int m_iSpriteTexture;
			int vol2;
            int m_radius; // radius du son
			int szSoundFile; // fichier son
			int vol1; // volume o radius 0
			int Chan; // le channel du son
			int Flag; // flag de lecture d'1 mp3.
			int Mode; // loop ou once
			int Strt; // kan part le mp3 en millisec
			int Mod; // play everywhere or not ? that is the question
			bool Playing; // entrain de jouer ou non ? 
			int Mp3Player; // player ayant l'entité.


};
			void StopSound( CBasePlayer *pPlayer );
			void Init (CBasePlayer *pPlayer);
			void PlayMp3 (CBasePlayer *pPlayer,CBaseEntity *pMp3);



