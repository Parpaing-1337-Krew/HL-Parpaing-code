#include "hud.h"
#include "cl_util.h"
#include "parsemsg.h"
#include <string.h>
#include <stdio.h>
#include "fmod.h"
#include "fmod_errors.h"
#include "loaddll.h"

cvar_t	*cl_mp3;


DECLARE_MESSAGE ( m_Mp3, Mp3 );

int CHudMp3::Init ( void )
{

cl_mp3		= gEngfuncs.pfnRegisterVariable( "cl_mp3","1", FCVAR_ARCHIVE );

HOOK_MESSAGE ( Mp3 );
m_iFlags |= HUD_ACTIVE;

gHUD.AddHudElem ( this );
return 1;

};


int CHudMp3::VidInit ( void )
{
return 1;

};

int CHudMp3::MsgFunc_Mp3 ( const char *pszName, int iSize, void *pbuf )
{

if (!cl_mp3->value==1) // si l'utilisateur est sourd , ca rien à de faire plus ramer son ordi.
return 1;

	BEGIN_READ ( pbuf, iSize );

	char* song; // chanson
	int mod; // action
	int chan; // channel
	int Flag,Mode; // flag de lecture 
	int everywhere; // tout est dans le nom
	int vol;
	mod = READ_BYTE();
	
	switch ( mod )
	{
	case 1: // loading mp3 -> en théorie exécuté une seule fois
		{
			LoadLibrary( "hl-parpaing/fmod.dll" );
			FSOUND_SetOutput(FSOUND_OUTPUT_DSOUND);
			FSOUND_SetBufferSize(200);
			FSOUND_SetDriver(0);
			FSOUND_Init(44100, 16, 0);
			break;
		}

	case 2: // demande la lecture d'1 chan
		{

			chan = READ_BYTE();
			song = READ_STRING();
			gEngfuncs.Con_DPrintf( "Playing %s on channel %i\n",song,chan ); // equivalent de ALERT mais coté client
			Mode = READ_BYTE();
			everywhere = READ_BYTE(); 
			vol = READ_BYTE(); // vol

			Flag = FSOUND_NORMAL; // commun pour toutes les lectures.
			if (Mode==1)		// loop
			Flag |= FSOUND_LOOP_NORMAL;


			stream = FSOUND_Stream_OpenFile(song, Flag ,0);

			FSOUND_Stream_Play(chan, stream);

			if (everywhere==1)
			FSOUND_SetVolume(chan,vol); 
				else
			FSOUND_SetVolume(chan,0); // on met à zero pour l'instant. 

		}
		break;
	case 3: // stop un chan
		{
			chan = READ_BYTE();
			FSOUND_StopSound(chan);

		}
		break;
	case 4: // stop tous les sons
		{
			FSOUND_Close();
		}
		break;
	case 5: // set vol
		{
			chan = READ_BYTE();
			vol = READ_BYTE(); 
			FSOUND_SetVolume(chan,vol); 

		}
		break;
	}

return 1;

}
int CHudMp3::Draw ( float flTime )
{
return 1;
}



