
/* appdefs.h	    Created:	1995-10-18  Revised:   1995-10-18	*/

/* Module Definition Header						*/

/************************************************************************/
/************************************************************************/
/************************************************************************/
/* DISCLAIMER OF WARRANTIES:						*/
/* -------------------------						*/
/* The following [enclosed] code is sample code	created	by IBM		*/
/* Corporation and Prominare Inc.  This	sample code is not part	of any	*/
/* standard IBM	product	and is provided	to you solely for the purpose	*/
/* of assisting	you in the development of your applications.  The code	*/
/* is provided "AS IS",	without	warranty of any	kind.  Neither IBM nor	*/
/* Prominare shall be liable for any damages arising out of your	*/
/* use of the sample code, even	if they	have been advised of the	*/
/* possibility of such damages.						*/
/************************************************************************/
/************************************************************************/
/************************************************************************/
/*		       D I S C L A I M E R				*/
/* This	code is	provided on an as is basis with	no implied support.	*/
/* It should be	considered freeware that cannot	be rebundled as		*/
/* part	of a larger "*ware" offering without our consent.		*/
/************************************************************************/
/************************************************************************/
/************************************************************************/

/* Copyright ¸ International Business Machines Corp., 1995.		*/
/* Copyright ¸ 1995  Prominare Inc.  All Rights	Reserved.		*/

/* --------------------------------------------------------------------	*/


#define	RGBCLR_PALEGRAY		0x00c9c9c9L

#define	RGBCLR_SHADOW		0x00808080L
#define	RGBCLR_DIALOGBACKGROUND	0x00ccccccL

typedef	struct _BITMAPSTACK
   {
   HBITMAP hbm;			   /* Bitmap Handle			*/
   HBITMAP hbmMask;		   /* Bitmap Handle			*/
   HBITMAP hbmImage;		   /* Bitmap Handle			*/
   RECTL   rclDest;		   /* Destination Rectangle		*/
   RECTL   rclImage;		   /* Image Rectangle			*/
   POINTL  aptlArea[8];		   /* Shadow Points			*/
   LONG	   cColours;		   /* Colours Count			*/
   LONG	   cx;			   /* Bitmap Width			*/
   LONG	   cy;			   /* Bitmap Height			*/
   } BITMAPSTACK, *PBITMAPSTACK;

/************************************************************************/
/************************************************************************/
/*	Global Variable	Definitions					*/
/************************************************************************/
/************************************************************************/

extern FONTMETRICS fm;		   /* Font Metrics Info			*/
extern HSWITCH	   hSwitch;	   /* Task List	Entry Handle		*/
extern HAB	   hAB;		   /* Program Anchor Block Handle	*/

extern HMQ  hmqView;		   /* Program Message Queue Handle	*/
extern HWND hwndView;		   /* Client Window Handle		*/
extern HWND hwndViewFrame;	   /* Frame Window Handle		*/
extern HWND hmenuView;		   /* Menu Handle			*/

extern HPOINTER	hptrArrow;	   /* Arrow Pointer Handle		*/
extern HPOINTER	hptrWait;	   /* Wait Pointer Handle		*/

extern LONG	cxScreen;	   /* Screen Width			*/

extern CHAR	szBitmapFile[CCHMAXPATH]; /* Bitmap Filename Holder	*/

extern BITMAPSTACK abm[32];	   /* Bitmap Stack			*/
extern LONG	cBitmaps;	   /* Bitmap Stack Count		*/
extern HBITMAP	hbmView;	   /* Bitmap Handle			*/

extern PBYTE	pb;		   /* Bitmap File Data Pointer		*/
extern BOOL	f20Bitmap;	   /* 2.x Bitmap Type			*/
extern BOOL	fWindowsBitmap;	   /* Windows Bitmap Type		*/
extern LONG	iBitmap;	   /* Single Bitmap Index		*/
extern LONG	iBitmapSelected;   /* Selected Bitmap Index		*/
extern BOOL	fBitmapArray;	   /* Bitmap Array Flag			*/

extern LONG	lRop;		   /* ROP Value	Index			*/

extern LONG	lPattern;	   /* Pattern				*/
extern LONG	lFillColour;	   /* Fill Colour			*/

/************************************************************************/
/************************************************************************/
/*	Function Prototype Definitions					*/
/************************************************************************/
/************************************************************************/

VOID	InitApp(HWND hwndFrame,	HWND hwndClient, PSZ pszWindowListTitle);
HWND	CreateStdWindow(HWND hwndParent, ULONG flStyle,	ULONG flCreateFlags,
			PSZ pszClientClass, PSZ	pszTitle, ULONG	styleClient,
			HMODULE	hmod, ULONG idResources, PHWND phwndClient,
			LONG x,	LONG y,	LONG cx, LONG cy);
MRESULT	PDSKeyProc(HWND	hWnd, ULONG msg, MPARAM	mp1, MPARAM mp2);
VOID	PDSGetTemplate(HWND hWnd, ULONG	id);

MRESULT	EXPENTRY ViewWndProc(HWND hWnd,	ULONG msg, MPARAM mp1, MPARAM mp2);
MRESULT	EXPENTRY OpenDlgProc(HWND hWnd,	ULONG msg, MPARAM mp1, MPARAM mp2);

MRESULT	EXPENTRY ViewDlgProc(HWND hWnd,	ULONG msg, MPARAM mp1, MPARAM mp2);

VOID GetIconPointer(PSZ	pszFileName, BOOL fIcon);
