#include <exec/types.h>
#include <exec/exec.h>
#include <proto/intuition.h>

#define OS_20 37

static UBYTE VersTag[] = "\0$VER: CDTVPrefs 0.3 (15.1.2023) (c) Jan.2023 by km-l";

extern struct ExecBase *SysBase;
struct Library *PlayerPrefsBase;
struct Screen *pubscreen;
VOID Quit(UBYTE *s);
VOID HidePointer(struct Window *w);
static __chip UWORD ptrData[6];

int main()
{
	if(SysBase->LibNode.lib_Version < OS_20)
    	Quit("This program requires OS2.0 or higher.");

	if(!(PlayerPrefsBase=(struct Library*)OpenLibrary("playerprefs.library", 0)))
		Quit("Playerprefs library will not open!");

	pubscreen=(struct Screen*)LockPubScreen("Workbench");
	HidePointer(pubscreen->FirstWindow);
	UnlockPubScreen(0, pubscreen);

	LONG modified = DoPrefs();
   	
   	if(modified < 0)
   		Quit("Unable to open preferences screen!");
   	else
   	{
   		pubscreen=(struct Screen*)LockPubScreen("Workbench");
		ClearPointer(pubscreen->FirstWindow);
		UnlockPubScreen(0, pubscreen);
		ScreenToFront(pubscreen);
       	Quit(0);
   	}
}

int wbmain(struct WBStartup *wbmsg)
{
	if(SysBase->LibNode.lib_Version < OS_20)
	{
    	DisplayAlert(RECOVERY_ALERT, "\0\260\20This program requires OS2.0 or higher!\0\0", 30);
		Quit(0);
	}

	if(!(PlayerPrefsBase=(struct Library*)OpenLibrary("playerprefs.library", 0)))
	{
		DisplayAlert(RECOVERY_ALERT, "\0\260\20Playerprefs library will not open!\0\0", 30);
		Quit(0);
	}

	pubscreen=(struct Screen*)LockPubScreen("Workbench");
	HidePointer(pubscreen->FirstWindow);
	UnlockPubScreen(0, pubscreen);
	
	LONG modified = DoPrefs();

	if(modified < 0)
	{
		DisplayAlert(RECOVERY_ALERT, "\0\260\20Unable to open preferences screen!\0\0", 30);
		Quit(0);
	}
	else
	{
		pubscreen=(struct Screen*)LockPubScreen("Workbench");
		ClearPointer(pubscreen->FirstWindow);
		UnlockPubScreen(0, pubscreen);
		ScreenToFront(pubscreen);
		Quit(0);
	}
}

VOID Quit(UBYTE *s)
{
	if(PlayerPrefsBase)
    	CloseLibrary((struct Library*)PlayerPrefsBase);
   	if(s)
    {
    	printf("ERROR: %s\n", s);
        exit(40);
    }
    else
        exit(0);
}

void HidePointer(struct Window *w)
{
    static __chip UWORD ptrData[6];

    SetPointer(w, ptrData, 1, 1, 0, 0);
}
