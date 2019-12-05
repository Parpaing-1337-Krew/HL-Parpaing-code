#define MOTD_TITLE_X		XRES(16)
#define MOTD_TITLE_Y		YRES(16)

#define MOTD_WINDOW_X				XRES(500)
#define MOTD_WINDOW_Y				YRES(0)
#define MOTD_WINDOW_SIZE_X			XRES(640)
#define MOTD_WINDOW_SIZE_Y			YRES(50)

#define MOTD_TPS_SIZE_Y			YRES(0)
#define MOTD_51_SIZE_Y			YRES(23)
#define MOTD_RICARD_SIZE_Y			YRES(35)
#define MOTD_TAB_SIZE			YRES(7)


//-----------------------------------------------------------------------------
// Purpose: Displays the MOTD and basic server information
//-----------------------------------------------------------------------------
class CMessageWindowPanel : public CMenuPanel ,  public vgui::CDefaultInputSignal
{
public:
	CMessageWindowPanel( const char *szMOTD, const char *szTitle, int iShadeFullScreen, int iRemoveMe, int x, int y, int wide, int tall );
	void UpdateTime(int m_iMin,int m_iSec);

private:
	CTransparentPanel *m_pBackgroundPanel;
};

class CMapMenuPanel : public CMenuPanel
{

private:
Label *pTimeLabel;
Label *pFond;
Label *pFond2;
Label *pHLabel;
Label *p51;
Label *pRicard;
Label *pInsLabel;
Label *pInfos;

CImageLabel			*m_pImage;



public:
CMapMenuPanel(int iTrans, int iRemoveMe, int x,int y,int wide,int tall);
CTransparentPanel *m_pBackgroundPanel;
CTransparentPanel *m_pInfos;
virtual void setSize(int wide,int tall);

void Update( void );
void RebuildData( void );
void Initialize( void );
void Open( void )
{
RebuildData();
setVisible(true);
}

};
