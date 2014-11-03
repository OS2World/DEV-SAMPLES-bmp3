#pragma	title("Icon/Pointer Viewer  --  Version 1  --  (ViewWnd.C)")
#pragma	subtitle("   Module Purpose - Interface Definitions")

#define	INCL_GPI		   /* Include OS/2 PM GPI Interface	*/
#define	INCL_WIN		   /* Include OS/2 PM Windows Interface	*/

#include <malloc.h>
#include <os2.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "appdefs.h"
#include "viewico.h"

/* This	module contains	routine	used to	handle the main	application	*/
/* client window.							*/

/* Filename:   ViewWnd.C						*/

/*  Version:   1							*/
/*  Created:   1995-10-18						*/
/*  Revised:   1995-10-18						*/

/* Routines:   static VOID SizeBitmapStack(HWND	hWnd);			*/
/*	       MRESULT EXPENTRY	ViewWndProc(HWND hWnd, ULONG msg,	*/
/*					    MPARAM mp1,	MPARAM mp2);	*/


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

HBITMAP	hbmROP = 0;		   /* Bitmap Handle			*/
LONG	lScale = 100L;		   /* Scale Factor			*/
RECTL	rclDest;		   /* Destination Rectangle		*/
RECTL	rclImage;		   /* Image Rectangle			*/
POINTL	aptlArea[8];		   /* Shadow Points			*/
RECTL	rclROP;			   /* ROP Image	Rectangle		*/
LONG	cxROP;			   /* ROP Image	Width			*/
LONG	cyROP;			   /* ROP Image	Height			*/
LONG	cxImage;		   /* Bitmap Image Width		*/
LONG	cyImage;		   /* Bitmap Image Height		*/
LONG	lHorzRange;		   /* Horizontal Scroll	Bar Range	*/
LONG	lVertRange;		   /* Vertical Scroll Bar Range		*/
LONG	lHorzPos;		   /* Horizontal Scroll	Bar Position	*/
LONG	lVertPos;		   /* Vertical Scroll Bar Position	*/
LONG	cxWindow;		   /* Window Width			*/
LONG	cyWindow;		   /* Window Height			*/

LONG	iImage;			   /* Bitmap Image Index		*/

BOOL	fBitmapArray = TRUE;	   /* Bitmap Array Display Flag		*/

static VOID SizeBitmapStack(HWND hWnd);

#pragma	subtitle("   Module Purpose - Bitmap Stack Sizing Function")
#pragma	page( )

/* --- SizeBitmapStack --------------------------------	[ Private ] ---	*/
/*									*/
/*     This function is	used to	calculate the size and position	of	*/
/*     the bitmaps from	the bitmap array for the window.		*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     HWND hWnd; = Window Handle					*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     Nothing								*/
/*									*/
/* --------------------------------------------------------------------	*/

static VOID SizeBitmapStack(HWND hWnd)

{
BITMAPINFOHEADER bmp;		   /* Bitmap Information Header		*/
RECTL		 rcl;		   /* Window Rectangle			*/
LONG		 cx;		   /* Bitmap Width			*/
LONG		 cy;		   /* Bitmap Height			*/
LONG		 xImage;	   /* Image Starting Point		*/
register INT i;			   /* Loop Counter			*/


WinQueryWindowRect(hWnd, &rcl);
	
for ( i	= 0, cx	= 10 * (cBitmaps - 1), cy = 0L;	i < cBitmaps; i++ )
   {
   GpiQueryBitmapParameters(abm[i].hbm,	&bmp);

   cx += bmp.cx;
   if (	bmp.cy > cy )
       cy = bmp.cy;
   }

rclImage.xRight	= (rclImage.xLeft   = ((rcl.xRight - rcl.xLeft)	/ 2L) -	(cx / 2L)) + cx;
rclImage.yTop	= (rclImage.yBottom = ((rcl.yTop - rcl.yBottom)	/ 2L) -	(cy / 2L)) + cy;

xImage = rclImage.xLeft;

rclImage.xLeft	 -= 3L;
rclImage.yBottom -= 3L;
rclImage.xRight	 += 3L;
rclImage.yTop	 += 3L;

for ( i	= 0; i < cBitmaps; i++ )
   {
   GpiQueryBitmapParameters(abm[i].hbm,	&bmp);

   abm[i].rclDest.xRight = (abm[i].rclDest.xLeft   = xImage) + bmp.cx;
   abm[i].rclDest.yTop	 = (abm[i].rclDest.yBottom = ((rcl.yTop	- rcl.yBottom) / 2L) - (bmp.cy / 2L)) +	bmp.cy;

		       /* Form the final boundary points		*/

   abm[i].aptlArea[0].x	= abm[i].rclDest.xLeft	- 2L;
   abm[i].aptlArea[0].y	= abm[i].rclDest.yTop	+ 2L;
   abm[i].aptlArea[1].x	= abm[i].rclDest.xRight	+ 2L;
   abm[i].aptlArea[1].y	= abm[i].rclDest.yTop	+ 2L;

   abm[i].aptlArea[2].x	= abm[i].rclDest.xRight	 + 2L;
   abm[i].aptlArea[2].y	= abm[i].rclDest.yBottom - 2L;
   abm[i].aptlArea[3].x	= abm[i].rclDest.xLeft	 - 2L;
   abm[i].aptlArea[3].y	= abm[i].rclDest.yBottom - 2L;

   abm[i].aptlArea[4].x	= abm[i].rclDest.xLeft	- 1L;
   abm[i].aptlArea[4].y	= abm[i].rclDest.yTop	+ 1L;
   abm[i].aptlArea[5].x	= abm[i].rclDest.xRight	+ 1L;
   abm[i].aptlArea[5].y	= abm[i].rclDest.yTop	+ 1L;

   abm[i].aptlArea[6].x	= abm[i].rclDest.xRight	 + 1L;
   abm[i].aptlArea[6].y	= abm[i].rclDest.yBottom - 1L;
   abm[i].aptlArea[7].x	= abm[i].rclDest.xLeft	 - 1L;
   abm[i].aptlArea[7].y	= abm[i].rclDest.yBottom - 1L;

   xImage += (bmp.cx + 10L);
   }
}
#pragma	subtitle("   Client Window - Client Window Procedure")
#pragma	page( )

/* --- ROPsTestWndProc --------------------------------- [ Public ] ---	*/
/*									*/
/*     This function is	used to	process	the messages sent to the	*/
/*     applications client window.					*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     HWND   hWnd; = Window Handle					*/
/*     ULONG  msg;  = PM Message					*/
/*     MPARAM mp1;  = Message Parameter	1				*/
/*     MPARAM mp2;  = Message Parameter	2				*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     ROPsTestWndProc = Message Handling Result			*/
/*									*/
/* --------------------------------------------------------------------	*/

MRESULT	EXPENTRY ViewWndProc(HWND hWnd,	ULONG msg, MPARAM mp1, MPARAM mp2)

{
CHAR			szTitle[300];  /* Title	Bar Text		*/
HPS			hPS;	   /* Presentation Space Handle		*/
PCHAR			pch;	   /* String Character Pointer		*/
RECTL			rcl;	   /* Window Rectangle			*/
POINTL			ptl;	   /* Display Point			*/
register INT i;			   /* Index				*/

switch ( msg )
   {
/************************************************************************/
/* Window being	created, perform window	initialization			*/
/************************************************************************/

   case	WM_CREATE :

       hptrWait	 = WinQuerySysPointer(HWND_DESKTOP, SPTR_WAIT,	FALSE);
       hptrArrow = WinQuerySysPointer(HWND_DESKTOP, SPTR_ARROW,	FALSE);

       cxScreen	= WinQuerySysValue(HWND_DESKTOP, SV_CXSCREEN);

       hmenuView = WinWindowFromID(WinQueryWindow(hWnd,	QW_PARENT), FID_MENU);

		       /************************************************/
		       /* PDSGetTemplate is used to allow controls in  */
		       /* windows.  Do not remove this function	if you */
		       /* intend to include controls within the	window.*/
		       /************************************************/

       PDSGetTemplate(hWnd, WIN_VIEWER);
       break;

/************************************************************************/
/* Window being	sized							*/
/************************************************************************/

   case	WM_SIZE	:
       SizeBitmapStack(hWnd);
       break;

/************************************************************************/
/* Perform menu	initialization						*/
/************************************************************************/

   case	WM_INITMENU :
       switch (	SHORT1FROMMP(mp1) )
	   {
	   case	IDM_FILE :
	       break;

	   }
       break;

/************************************************************************/
/* Process key press from keyboard					*/
/************************************************************************/

   case	WM_CHAR	:
		       /************************************************/
		       /* PDSKeyProc is	used to	allow controls in      */
		       /* windows.  Do not remove this function	if you */
		       /* intend to include controls within the	window.*/
		       /************************************************/

       return(PDSKeyProc(hWnd, msg, mp1, mp2));

/************************************************************************/
/* Process control selections						*/
/************************************************************************/

   case	WM_COMMAND :
       switch (	SHORT1FROMMP(mp1) )
	   {
	   case	IDM_OPEN :
	       if ( WinDlgBox(HWND_DESKTOP, hwndViewFrame, (PFNWP)OpenDlgProc,
			      (HMODULE)NULL, DLG_OPEN, NULL) )
		   {
		   WinSetPointer(HWND_DESKTOP, hptrWait);
		   if (	(pch = strrchr(szBitmapFile, '.')) != NULL )
		       if ( !memicmp(pch, ".ico", 5) )
			   GetIconPointer(szBitmapFile,	TRUE);
		       else
			   GetIconPointer(szBitmapFile,	FALSE);
		   else
		       GetIconPointer(szBitmapFile, FALSE);
		   SizeBitmapStack(hWnd);
		   WinInvalidateRect(hWnd, NULL, FALSE);
		   strcat(memcpy(szTitle, "Icon/Pointer Viewer - ", 23), szBitmapFile);
		   if (	fWindowsBitmap )
		       strcat(szTitle, " [Windows 3.x format]");
		   else
		       if ( f20Bitmap )
			   strcat(szTitle, " [OS/2 2.x format]");
		       else
			   strcat(szTitle, " [OS/2 1.x format]");
		   WinSetWindowText(hwndViewFrame, szTitle);
		   WinSetPointer(HWND_DESKTOP, hptrArrow);
		   }
	       break;
	   }
       break;
   case	WM_BUTTON2DOWN :

/************************************************************************/
/* Button 2 being depressed						*/
/************************************************************************/

       ptl.x = (LONG)(SHORT)SHORT1FROMMP(mp1);
       ptl.y = (LONG)(SHORT)SHORT2FROMMP(mp1);
       for ( i = 0; i <	cBitmaps; i++ )
	   if (	WinPtInRect(hAB, &abm[i].rclDest, &ptl)	)
	       {
	       iBitmap = i;
	       WinDlgBox(HWND_DESKTOP, hwndViewFrame, (PFNWP)ViewDlgProc,
			 (HMODULE)NULL,	DLG_VIEW, NULL);
	       break;
	       }
       break;

/************************************************************************/
/* Erase window	background						*/
/************************************************************************/

   case	WM_ERASEBACKGROUND :
       WinQueryWindowRect(hWnd,	&rcl);
       WinFillRect((HPS)LONGFROMMP(mp1), &rcl, CLR_PALEGRAY);
       break;

/************************************************************************/
/* Paint client	window							*/
/************************************************************************/

   case	WM_PAINT :
       GpiCreateLogColorTable(hPS = WinBeginPaint(hWnd,	(HPS)NULL, &rcl), 0UL, LCOLF_RGB, 0L, 0L, (PLONG)NULL);

		       /* Display the bitmap array images side-by-side	*/

       WinFillRect(hPS,	&rcl, RGBCLR_PALEGRAY);
       for ( i = 0; i <	cBitmaps; i++ )
	   {
		       /* Draw the bitmap from the array		*/

	   WinDrawBitmap(hPS, abm[i].hbm, (PRECTL)NULL,	(PPOINTL)(PVOID)&abm[i].rclDest,
			 RGB_WHITE, RGB_BLACK, DBM_NORMAL);

		       /* Draw the 3-D frame around the	image		*/

	   GpiSetColor(hPS, RGBCLR_SHADOW);
	   GpiMove(hPS,	&abm[i].aptlArea[7]);
	   GpiPolyLine(hPS, 2L,	&abm[i].aptlArea[4]);

	   GpiSetColor(hPS, RGB_WHITE);
	   GpiPolyLine(hPS, 2L,	&abm[i].aptlArea[6]);

	   GpiSetColor(hPS, RGB_BLACK);
	   GpiMove(hPS,	&abm[i].aptlArea[3]);
	   GpiPolyLine(hPS, 2L,	&abm[i].aptlArea[0]);

	   GpiSetColor(hPS, RGBCLR_PALEGRAY);
	   GpiPolyLine(hPS, 2L,	&abm[i].aptlArea[2]);
	   }
       WinEndPaint(hPS);
       break;

/************************************************************************/
/* Window being	destroyed, perform clean-up operations			*/
/************************************************************************/

   case	WM_DESTROY :

       for ( i = 0; i <	cBitmaps; i++ )
	   {
	   GpiDeleteBitmap(abm[i].hbmMask);

	   if (	abm[i].hbmImage	)
	       GpiDeleteBitmap(abm[i].hbmImage);

	   GpiDeleteBitmap(abm[i].hbm);
	   }

       if ( pb )
	   free(pb);
       break;

		       /* Default message processing			*/
   default :
       return(WinDefWindowProc(hWnd, msg, mp1, mp2));
   }
return(0L);
}
