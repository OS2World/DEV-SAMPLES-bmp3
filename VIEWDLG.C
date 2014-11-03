#pragma	title("Icon/Pointer Viewer  --  Version 1  --  (ViewDlg.C)")
#pragma	subtitle("   Module Purpose - Interface Definitions")

#define	INCL_GPI		   /* Include OS/2 PM GPI Interface	*/
#define	INCL_WIN		   /* Include OS/2 PM Windows Interface	*/

#include <malloc.h>
#include <os2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "appdefs.h"
#include "viewico.h"

/* This	module contains	routine	used to	handle the main	application	*/
/* client window.							*/

/* Filename:   ViewDlg.C						*/

/*  Version:   1							*/
/*  Created:   1995-10-18						*/
/*  Revised:   1995-10-18						*/

/* Routines:   MRESULT EXPENTRY	ViewDlgProc(HWND hWnd, ULONG msg,	*/
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


POINTL		 ptlSRCAND;
POINTL		 ptlSRCPAINT;
POINTL		 ptlSRCINVERT;

#pragma	subtitle("   Module Purpose - Dialog Procedure")
#pragma	page( )

/* --- ViewROPDlgProc ---------------------------------- [ Public ] ---	*/
/*									*/
/*     This function is	used to	process	the messages for the dialog	*/
/*     procedure.							*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     HWND   hWnd; = Dialog Window Handle				*/
/*     ULONG  msg;  = PM Message					*/
/*     MPARAM mp1;  = Message Parameter	1				*/
/*     MPARAM mp2;  = Message Parameter	2				*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     ViewROPDlgProc =	Message	Handling Result				*/
/*									*/
/* --------------------------------------------------------------------	*/

MRESULT	EXPENTRY ViewDlgProc(HWND hWnd,	ULONG msg, MPARAM mp1, MPARAM mp2)

{
HPS	hPS;			   /* Presentation Space Handle		*/
MRESULT	mr;			   /* Message Return			*/
RECTL	rcl;			   /* Window Rectangle			*/
POINTL	aptl[4];		   /* Display Points			*/

switch ( msg )
   {
/************************************************************************/
/* Perform dialog initialization					*/
/************************************************************************/

   case	WM_INITDLG :
       WinQueryWindowRect(hWnd,	&rcl);
       if ( abm[iBitmap].hbmImage )
	   {
	   ptlSRCAND.x	  = (rcl.xRight	- rcl.xLeft) / 2L - abm[iBitmap].cx / 2L - 5L -	abm[iBitmap].cx;
	   ptlSRCPAINT.x  = (rcl.xRight	- rcl.xLeft) / 2L - abm[iBitmap].cx / 2L;
	   ptlSRCINVERT.x = (rcl.xRight	- rcl.xLeft) / 2L + abm[iBitmap].cx / 2L + 5L;

	   ptlSRCAND.y = ptlSRCPAINT.y = ptlSRCINVERT.y	= (rcl.yTop - rcl.yBottom) / 2L	- (abm[iBitmap].cy * 2L	+ 10L) / 2L;
	   }
       else
	   {
	   ptlSRCAND.x	  = (rcl.xRight	- rcl.xLeft) / 2L - 5L - abm[iBitmap].cx;
	   ptlSRCINVERT.x = (rcl.xRight	- rcl.xLeft) / 2L + 5L;

	   ptlSRCAND.y = ptlSRCINVERT.y	= (rcl.yTop - rcl.yBottom) / 2L	- (abm[iBitmap].cy * 2L	+ 10L) / 2L;
	   }
       break;

/************************************************************************/
/* Process push	button selections					*/
/************************************************************************/

   case	WM_COMMAND :
       switch (	SHORT1FROMMP(mp1) )
	   {
	   case	DID_OK :
	       WinDismissDlg(hWnd, TRUE);
	       break;

	   }
       break;

   case	WM_PAINT :
       mr = WinDefDlgProc(hWnd,	msg, mp1, mp2);

       memset(aptl, 0, sizeof(POINTL) *	4);

       aptl[1].x = (aptl[0].x =	ptlSRCAND.x) + abm[iBitmap].cx;
       aptl[1].y = (aptl[0].y =	ptlSRCAND.y) + abm[iBitmap].cy;

       aptl[3].x = abm[iBitmap].cx;
       aptl[3].y = abm[iBitmap].cy;
       GpiSetBackColor(hPS = WinGetPS(hWnd), CLR_WHITE);

       GpiWCBitBlt(hPS,	abm[iBitmap].hbmMask, 4L, aptl,	ROP_SRCAND, BBO_OR);

       aptl[0].y += (abm[iBitmap].cy + 10L);
       aptl[1].y += (abm[iBitmap].cy + 10L);
       GpiSetBackColor(hPS, CLR_WHITE);
       GpiWCBitBlt(hPS,	abm[iBitmap].hbmMask, 4L, aptl,	ROP_SRCCOPY, BBO_OR);
       if ( abm[iBitmap].hbmImage )
	   {
	   aptl[1].x = (aptl[0].x = ptlSRCPAINT.x) + abm[iBitmap].cx;
	   aptl[1].y = (aptl[0].y = ptlSRCPAINT.y) + abm[iBitmap].cy;
	   GpiWCBitBlt(hPS, abm[iBitmap].hbmMask, 4L, aptl, ROP_SRCAND,	BBO_OR);
	   GpiWCBitBlt(hPS, abm[iBitmap].hbmImage, 4L, aptl, ROP_SRCPAINT, BBO_OR);

	   aptl[0].y +=	(abm[iBitmap].cy + 10L);
	   aptl[1].y +=	(abm[iBitmap].cy + 10L);
	   GpiWCBitBlt(hPS, abm[iBitmap].hbmImage, 4L, aptl, ROP_SRCCOPY, BBO_OR);
	   }

       aptl[1].x = (aptl[0].x =	ptlSRCINVERT.x)	+ abm[iBitmap].cx;
       aptl[1].y = (aptl[0].y =	ptlSRCINVERT.y)	+ abm[iBitmap].cy;

       GpiSetBackColor(hPS, CLR_WHITE);
       GpiWCBitBlt(hPS,	abm[iBitmap].hbmMask, 4L, aptl,	ROP_SRCAND, BBO_OR);

       if ( abm[iBitmap].hbmImage )
	   GpiWCBitBlt(hPS, abm[iBitmap].hbmImage, 4L, aptl, ROP_SRCPAINT, BBO_OR);
       aptl[3].y = (aptl[2].y =	abm[iBitmap].cy) << 1UL;
       GpiSetBackColor(hPS, CLR_BLACK);
       GpiWCBitBlt(hPS,	abm[iBitmap].hbmMask, 4L, aptl,	ROP_SRCINVERT, BBO_OR);

       aptl[0].y += (abm[iBitmap].cy + 10L);
       aptl[1].y += (abm[iBitmap].cy + 10L);
       GpiSetBackColor(hPS, CLR_WHITE);
       GpiWCBitBlt(hPS,	abm[iBitmap].hbmMask, 4L, aptl,	ROP_SRCCOPY, BBO_OR);
       WinReleasePS(hPS);
       return(mr);

/************************************************************************/
/* Close requested, exit dialogue					*/
/************************************************************************/

   case	WM_CLOSE :
       WinDismissDlg(hWnd, FALSE);
       break;

			/* Pass	through	unhandled messages		*/
   default :
       return(WinDefDlgProc(hWnd, msg, mp1, mp2));
   }
return(0L);
}
