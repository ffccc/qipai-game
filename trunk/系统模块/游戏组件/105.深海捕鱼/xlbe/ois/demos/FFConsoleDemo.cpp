//////////////////////////////// OS Nuetral Headers ////////////////
//Define this to make OIS use dynamic linking imports:
#define OIS_DYNAMIC_LIB

#include "OIS.h"

#include <iostream>
#include <vector>
#include <sstream>

////////////////////////////////////Needed Windows Headers////////////
#if defined OIS_WIN32_PLATFORM
#  define WIN32_LEAN_AND_MEAN
#  include "windows.h"
#  include "resource.h"
   LRESULT DlgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
//////////////////////////////////////////////////////////////////////
////////////////////////////////////Needed Linux Headers//////////////
#elif defined OIS_LINUX_PLATFORM
#  include <X11/Xlib.h>
   void checkX11Events();
#endif
//////////////////////////////////////////////////////////////////////
using namespace OIS;

//-- Some local prototypes --//
void doStartup();

//-- Some hacky globals --//
bool appRunning = true;	//Global Exit Flag

InputManager *g_InputManager = 0;
Keyboard *g_kb  = 0;		//Keyboard Device
JoyStick* g_joys[4] = {0,0,0,0};   //This demo supports up to 4 controllers
ForceFeedback* g_ff[4] = {0,0,0,0};//Array to hold ff interface for each joy

///// OS Specific Globals //////
#if defined OIS_WIN32_PLATFORM
  HWND hWnd = 0;
#elif defined OIS_LINUX_PLATFORM
  Display *xDisp = 0;
  Window xWin = 0;
#endif

//////////// Common Event handler class ////////
class EventHandler : public KeyListener, public JoyStickListener
{
	bool configuringFF;
public:
	EventHandler() : configuringFF(false) {}
	~EventHandler() {}

	bool keyPressed( const KeyEvent &arg )
	{
		if( arg.key == KC_ESCAPE || arg.key == KC_Q )
			appRunning =false;

		return true;
	}

	bool keyReleased( const KeyEvent &arg )
	{
		return true;
	}

	bool buttonPressed( const JoyStickEvent &arg, int button )
	{
		return true;
	}
	bool buttonReleased( const JoyStickEvent &arg, int button )
	{
		return true;
	}
	bool axisMoved( const JoyStickEvent &arg, int axis )
	{
		return true;
	}
	bool povMoved( const JoyStickEvent &arg, int pov )
	{
		return true;
	}

	void displayHelp()
	{
		std::cout << "\n\tEsc or Q <-----> Exit App"
				  << "\n\tH        <-----> This Help Menu"
				  << "\n\tF        <-----> Create an FF Effect\n";
	}
};

//Create a global instance
EventHandler handler;

int main()
{
	try
	{
		doStartup();
		std::cout << "\nThis is a simple command line Force Feedback testing demo..."
			<< "\nAll connected joystick type devices will be created and if FF "
			<< "Support is found, it will be availiable.\n** Hit 'q' or ESC to exit. Or 'h' for help **\n";

		//Main polling loop
		while(appRunning)
		{
			//This fires off buffered events for keyboards
			g_kb->capture();
			//This fires off buffered events for each joystick we have
			for( int i = 0; i < 4 ; ++i )
				if( g_joys[i] )	g_joys[i]->capture();

			//Throttle down CPU usage & handle OS events
			#if defined OIS_WIN32_PLATFORM
			  Sleep( 30 );
			  MSG  msg;
			  while( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
			  {
				  TranslateMessage( &msg );
				  DispatchMessage( &msg );
			  }
			#elif defined OIS_LINUX_PLATFORM
			  checkX11Events();
			  usleep( 300 );
			#endif
		}
	}
	catch( const Exception &ex )
	{
		#if defined OIS_WIN32_PLATFORM
		  MessageBox(0, ex.eText, "Exception Raised!", MB_OK);
		#else
		  std::cout << "\nOIS Exception Caught!\n" << "\t" << ex.eText << "[Line " << ex.eLine << " in " << ex.eFile << "]\nExiting App\n";
		#endif
	}

	if( g_InputManager )
	{
		g_InputManager->destroyInputObject( g_kb );

		for(int i = 0; i < 4; ++i)
			g_InputManager->destroyInputObject( g_joys[i] );

		InputManager::destroyInputSystem(g_InputManager);
	}

	#if defined OIS_LINUX_PLATFORM
	  // Be nice to X and clean up the x window
	  XDestroyWindow(xDisp, xWin);
	  XCloseDisplay(xDisp);
	#elif defined OIS_WIN32_PLATFORM && _DEBUG
	  system("pause");
	#endif

	return 0;
}

void doStartup()
{
	ParamList pl;

	#if defined OIS_WIN32_PLATFORM
	  //Create a capture window for Input Grabbing
	  hWnd = CreateDialog( 0, MAKEINTRESOURCE(IDD_DIALOG1), 0,(DLGPROC)DlgProc);
	  if( hWnd == NULL )
		OIS_EXCEPT(E_General, "Failed to create Win32 Window Dialog!");

	  ShowWindow(hWnd, SW_SHOW);

	  std::ostringstream wnd;
	  wnd << (size_t)hWnd; 

	  pl.insert(std::make_pair( std::string("WINDOW"), wnd.str() ));
	#elif defined OIS_LINUX_PLATFORM
	  //Connects to default X window
	  if( !(xDisp = XOpenDisplay(0)) )
		  OIS_EXCEPT(E_General, "Error opening X!");
	  //Create a window
	  xWin = XCreateSimpleWindow(xDisp,DefaultRootWindow(xDisp), 0,0, 100,100, 0, 0, 0);
	  //bind our connection to that window
	  XMapWindow(xDisp, xWin);
	  //Select what events we want to listen to locally
	  XSelectInput(xDisp, xWin, StructureNotifyMask);
	  XEvent evtent;
	  //Wait for Window to show up
	  do  {	XNextEvent(xDisp, &evtent); } while(evtent.type != MapNotify);

	  std::ostringstream wnd;
	  wnd << xWin;

	  pl.insert(std::make_pair(std::string("WINDOW"), wnd.str()));
	#endif

	//Do the creation
	g_InputManager = InputManager::createInputSystem(pl);
	InputManager &im = *g_InputManager;
	std::cout << "Input Manager (" << im.inputSystemName() << ") Created..\n";

	//Create a simple keyboard
	g_kb = (Keyboard*)im.createInputObject( OISKeyboard, true );
	g_kb->setEventCallback( &handler );

	//This demo only uses at max 4 joys
	int numSticks = (im.getNumberOfDevices(OISJoyStick) > 4) ? 4 : im.getNumberOfDevices(OISJoyStick);
	bool ffFound = false;
	for( int i = 0; i < numSticks; ++i ) 
	{
		//Create the stick
		g_joys[i] = (JoyStick*)im.createInputObject( OISJoyStick, true );
		g_joys[i]->setEventCallback( &handler );
		//Check for FF, if so, dump info
		g_ff[i] = (ForceFeedback*)g_joys[i]->queryInterface( Interface::ForceFeedback );
		if( g_ff[i] )
		{
			ffFound = true;
			std::cout << "Created buffered joystick **with ForceFeedback** support.\n";
			const ForceFeedback::SupportedEffectList &list = g_ff[i]->getSupportedEffects();
			ForceFeedback::SupportedEffectList::const_iterator i = list.begin(),
				e = list.end();
			for( ; i != e; ++i)
				std::cout << "Force =  " << i->first << " Type = " << i->second << std::endl;
		}
		else
			std::cout << "Joystick **without** FF support\n";
	}
	if( ffFound == false )
		OIS_EXCEPT( E_General, "At least one Force Feedback device is required for this demo.\n\n" );
}

#if defined OIS_WIN32_PLATFORM
LRESULT DlgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	return FALSE;
}
#endif

#if defined OIS_LINUX_PLATFORM
//This is just here to show that you still recieve x11 events, as the lib only needs mouse/key events
void checkX11Events()
{
	XEvent event;

	//Poll x11 for events
	while( XPending(xDisp) > 0 )
	{
		XNextEvent(xDisp, &event);
	}
}
#endif
