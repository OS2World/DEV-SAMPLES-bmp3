#pragma	title("Icon/Pointer Viewer  --  Version 1.0  --  (GetIco.C)")
#pragma	subtitle("   Icon Load - Interface Definitions")

#pragma	info(noext)

#define	INCL_BITMAPFILEFORMAT	   /* Include OS/2 Bitmap Information	*/
#define	INCL_DOS		   /* Include OS/2 DOS Kernal		*/
#define	INCL_GPI		   /* Include OS/2 GPI Interface	*/

#include <malloc.h>
#include <os2.h>
#include <string.h>

#include "appdefs.h"
#include "winbmp.h"


/* This	module contains	the functions to load a	requested .ICO file.	*/

/* Filename:   GetIco.C							*/

/*  Version:   1.0							*/
/*  Created:   1995-10-18						*/
/*  Revised:   1995-10-18						*/

/* Routines:   static HBITMAP CreateImage(HBITMAP hbmMask,		*/
/*					  HBITMAP hbmImage, LONG cx,	*/
/*					  LONG cy);			*/
/*	       static VOID BuildImageStack(				 */
/*				   PBITMAPARRAYFILEHEADER2 pbafh2,	*/
/*				   ULONG cbFile);			*/
/*	       static VOID BuildImageStack1(				 */
/*				   PBITMAPARRAYFILEHEADER pbafh,	*/
/*				   ULONG cbFile);			*/
/*	       static HBITMAP GetSingleImage(PBITMAPFILEHEADER2	pbfh2,	 */
/*					    ULONG cbFile);		*/
/*	       static HBITMAP GetSingleImage1(PBITMAPFILEHEADER	pbfh,	 */
/*					     ULONG cbFile);		*/
/*	       VOID GetIcon(PSZ	pszFileName);				*/
/*	       static PBITMAPFILEHEADER2 pbfh2GetWindowsBmp(		*/
/*			   PBYTE pbData, ULONG cbData, PULONG pcbFile);	*/


/* Copyright ¸ 1995  CodeSmithery Corp.	 All Rights Reserved.		*/

/* --------------------------------------------------------------------	*/

/* --- Module Prototype	Definitions -----------------------------------	*/

static VOID BuildImageStack(PBITMAPARRAYFILEHEADER2 pbafh2, ULONG ulType);
static VOID BuildImageStack1(PBITMAPARRAYFILEHEADER pbafh, ULONG ulType);

static VOID GetSingleImage(PBITMAPFILEHEADER2 pbfh2, ULONG ulType);
static VOID GetSingleImage1(PBITMAPFILEHEADER pbfh, ULONG ulType);

static PBITMAPFILEHEADER2 pbfh2GetWindowsIco(PBYTE pbData);
static PBITMAPFILEHEADER2 pbfh2GetWindowsCur(PBYTE pbData);

static VOID CreateImage(HBITMAP	hbmMask, HBITMAP hbmImage, LONG	cx, LONG cy, LONG iImage);

#pragma	subtitle("   Images Create - Icon/Pointer Bitmap Create Function")
#pragma	page( )

/* --- CreateImage ------------------------------------	[ Private ] ---	*/
/*									*/
/*     This function is	used to	create the final image for an icon or	*/
/*     pointer.	 This is done since the	OS/2 APIs will stretch the	*/
/*     image to	the default pointer or icon size.  The idea is to show	*/
/*     the actual size recorded	within the file.			*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     Nothing								*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     Nothing								*/
/*									*/
/* --------------------------------------------------------------------	*/

static VOID CreateImage(HBITMAP	hbmMask, HBITMAP hbmImage, LONG	cx, LONG cy, LONG iImage)

{
BITMAPINFOHEADER2 bmInfo;	   /* Bitmap Information Header		*/
HBITMAP		  hbm;		   /* Bitmap Handle			*/
HDC		  hDC;		   /* Screen Device Context Handle	*/
HPS		  hpsMem;	   /* Memory Presentation Space	Handle	*/
LONG		  alBmpFormats[2]; /* Bitmap Formats Array		*/
POINTL		  aptl[4];	   /* Bitmap Region Points Array	*/
SIZEL		  sizl;		   /* Size Holder			*/

		       /* Get the memory device	context	handle and	*/
		       /* calculate the	width and height of the	window/	*/
		       /* dialogue and save for	the window/dialogue	*/
		       /* before creating the memory presentation space	*/
		       /* that is to be	used for the bitmap		*/
sizl.cx	= cx;
sizl.cy	= cy;
		       /* Get the bitmap format	before creating	the	*/
		       /* bitmap.  Since different video configurations	*/
		       /* exist, need to find the number of planes and	*/
		       /* bits for the current configuration.		*/

GpiQueryDeviceBitmapFormats(hpsMem = GpiCreatePS(hAB, hDC = DevOpenDC(hAB, OD_MEMORY, "*", 0L, (PDEVOPENDATA)NULL, (HDC)NULL),
						 &sizl,	PU_PELS	| GPIT_MICRO | GPIA_ASSOC), 2L,	alBmpFormats);

		       /* Initialize the bitmap	header information for	*/
		       /* the window/dialogue being copied to the	*/
		       /* clipboard					*/

memset(&bmInfo,	0, sizeof(BITMAPINFOHEADER2));
bmInfo.cbFix	 = sizeof(BITMAPINFOHEADER2);
bmInfo.cx	 = (ULONG)cx;
bmInfo.cy	 = (ULONG)cy;
bmInfo.cPlanes	 = (USHORT)alBmpFormats[0];
bmInfo.cBitCount = (USHORT)alBmpFormats[1];

GpiSetBitmap(hpsMem, hbm = GpiCreateBitmap(hpsMem, &bmInfo, 0L,	NULL, NULL));

memset(aptl, 0,	sizeof(POINTL) * 4);

aptl[1].x = cx;
aptl[1].y = cy;

aptl[3].x = cx + 1L;
aptl[3].y = cy + 1L;

GpiSetColor(hpsMem, CLR_WHITE);
GpiSetBackColor(hpsMem,	CLR_BLACK);

GpiErase(hpsMem);

GpiWCBitBlt(hpsMem, hbmMask, 4L, aptl, ROP_SRCAND, BBO_OR);
if ( hbmImage )
   GpiWCBitBlt(hpsMem, hbmImage, 4L, aptl, ROP_SRCPAINT, BBO_OR);
aptl[2].y = cy;
aptl[3].y = cy * 2L + 1L;
GpiWCBitBlt(hpsMem, hbmMask, 4L, aptl, ROP_SRCINVERT, BBO_OR);

		       /* Set the memory bitmap	as the current bitmap	*/

GpiSetBitmap(hpsMem, (HBITMAP)NULL);

		       /* Destroy the memory device context		*/

GpiAssociate(hpsMem, (HDC)NULL);
DevCloseDC(hDC);
		       /* Destroy the presentation spaces used		*/
GpiDestroyPS(hpsMem);

abm[iImage].cx = cx;
abm[iImage].cy = cy;

abm[iImage].hbmMask = hbmMask;

abm[iImage].hbmImage = hbmImage;

abm[iImage].hbm	= hbm;
}
#pragma	subtitle("   Icon/Pointer Retrieve - Build Image Stack Function")
#pragma	page( )

/* --- BuildImageStack --------------------------------	[ Private ] ---	*/
/*									*/
/*     This function is	used to	build the icon or pointer images for	*/
/*     a icon or pointer array and place the icons or pointers within	*/
/*     the image stack.							*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     PBITMAPARRAYFILEHEADER2 pbafh2; = Bitmap	Array Header Pointer	*/
/*     ULONG		       ulType; = Image Type			*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     Nothing								*/
/*									*/
/* --------------------------------------------------------------------	*/

static VOID BuildImageStack(PBITMAPARRAYFILEHEADER2 pbafh2, ULONG ulType)

{
HPS			hPS;	   /* Presentation Space		*/
PBITMAPARRAYFILEHEADER2	pbafhOrg;  /* Bitmap Array File	Header Origin	*/
PBITMAPFILEHEADER2	pbfh2Clr;  /* Colour Bitmap File Header		*/
PBITMAPFILEHEADER2	pbfh2Mask; /* Mask Bitmap File Header		*/

pbafhOrg = (PBITMAPARRAYFILEHEADER2)pbafh2;

pbfh2Clr  = (PBITMAPFILEHEADER2)((PBYTE)(pbfh2Mask = &pbafh2->bfh2) + sizeof(BITMAPFILEHEADER2)	+ 2 * sizeof(RGB2));

		       /* Save the start of the	bitmap file header	*/
		       /* since	all offsets are	from the beginning of	*/
		       /* the header					*/

hPS = WinGetPS(HWND_DESKTOP);

if ( pbfh2Mask->usType == ulType )

		       /* Initialize the pointer information		*/

   CreateImage(GpiCreateBitmap(hPS, &pbfh2Mask->bmp2, CBM_INIT,
			      (PBYTE)pbafhOrg +	pbfh2Mask->offBits, (PBITMAPINFO2)(PVOID)&pbfh2Mask->bmp2),
	       GpiCreateBitmap(hPS, &pbfh2Clr->bmp2, CBM_INIT, (PBYTE)pbafhOrg + pbfh2Clr->offBits,
			       (PBITMAPINFO2)(PVOID)&pbfh2Clr->bmp2), pbfh2Clr->bmp2.cx, pbfh2Clr->bmp2.cy, cBitmaps++);
else
		       /* Initialize the pointer information		*/

   CreateImage(GpiCreateBitmap(hPS, &pbfh2Mask->bmp2, CBM_INIT,
					(PBYTE)pbafhOrg	+ pbfh2Mask->offBits, (PBITMAPINFO2)(PVOID)&pbfh2Mask->bmp2),
			       (HBITMAP)NULL, pbfh2Mask->bmp2.cx, pbfh2Mask->bmp2.cy / 2L, cBitmaps++);

		       /* Scan the bitmap array	for the	desired	icon	*/
		       /* type based on	the current screen display	*/
while (	pbafh2->offNext	)
   {
		       /* Point	to the next array header		*/

   pbafh2 = (PBITMAPARRAYFILEHEADER2)((PBYTE)pbafhOrg +	pbafh2->offNext);
   pbfh2Clr  = (PBITMAPFILEHEADER2)((PBYTE)(pbfh2Mask =	&pbafh2->bfh2) + sizeof(BITMAPFILEHEADER2) + 2 * sizeof(RGB2));

   if (	pbfh2Mask->usType == ulType )

		       /* Initialize the pointer information		*/

       CreateImage(GpiCreateBitmap(hPS,	&pbfh2Mask->bmp2, CBM_INIT, (PBYTE)pbafhOrg + pbfh2Mask->offBits,
				   (PBITMAPINFO2)(PVOID)&pbfh2Mask->bmp2),
		   GpiCreateBitmap(hPS,	&pbfh2Clr->bmp2, CBM_INIT, (PBYTE)pbafhOrg + pbfh2Clr->offBits,
				   (PBITMAPINFO2)(PVOID)&pbfh2Clr->bmp2), pbfh2Clr->bmp2.cx, pbfh2Clr->bmp2.cy,	cBitmaps);
   else
		       /* Initialize the pointer information		*/

      CreateImage(GpiCreateBitmap(hPS, &pbfh2Mask->bmp2, CBM_INIT, (PBYTE)pbafhOrg + pbfh2Mask->offBits,
				 (PBITMAPINFO2)(PVOID)&pbfh2Mask->bmp2),
		  (HBITMAP)NULL, pbfh2Mask->bmp2.cx, pbfh2Mask->bmp2.cy	/ 2L, cBitmaps);

   if (	++cBitmaps == 32L )
       break;
   }
		       /* Release the memory presentation space		*/
WinReleasePS(hPS);

}
#pragma	subtitle("   Icon/Pointer Retrieve - Build Image Stack Function")
#pragma	page( )

/* --- BuildImageStack1	-------------------------------	[ Private ] ---	*/
/*									*/
/*     This function is	used to	build the icon or pointer images for	*/
/*     a icon or pointer array and place the icons or pointers within	*/
/*     the image stack.							*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     PBITMAPARRAYFILEHEADER pbafh;  =	Bitmap Array Header Pointer	*/
/*     ULONG		      ulType; =	Image Type			*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     Nothing								*/
/*									*/
/* --------------------------------------------------------------------	*/

static VOID BuildImageStack1(PBITMAPARRAYFILEHEADER pbafh, ULONG ulType)

{
HPS		       hPS;	   /* Presentation Space		*/
PBITMAPARRAYFILEHEADER pbafhOrg;   /* Bitmap Array File	Header Origin	*/
PBITMAPFILEHEADER      pbfhClr;	   /* Colour Bitmap File Header		*/
PBITMAPFILEHEADER      pbfhMask;   /* Mask Bitmap File Header		*/

pbafhOrg = (PBITMAPARRAYFILEHEADER)pbafh;

pbfhClr	 = (PBITMAPFILEHEADER)((PBYTE)(pbfhMask	= &pbafh->bfh) + sizeof(BITMAPFILEHEADER) + 2 *	sizeof(RGB2));


hPS = WinGetPS(HWND_DESKTOP);

		       /* Save the start of the	bitmap file header	*/
		       /* since	all offsets are	from the beginning of	*/
		       /* the header					*/

if ( pbfhMask->usType == ulType	)
   CreateImage(GpiCreateBitmap(hPS, (PBITMAPINFOHEADER2)&pbfhMask->bmp,	CBM_INIT,
			       (PBYTE)pbafhOrg + pbfhMask->offBits, (PBITMAPINFO2)(PVOID)&pbfhMask->bmp),
	       GpiCreateBitmap(hPS, (PBITMAPINFOHEADER2)&pbfhClr->bmp, CBM_INIT, (PBYTE)pbafhOrg + pbfhClr->offBits,
			       (PBITMAPINFO2)(PVOID)&pbfhClr->bmp), pbfhClr->bmp.cx, pbfhClr->bmp.cy, cBitmaps++);
else
   CreateImage(GpiCreateBitmap(hPS, (PBITMAPINFOHEADER2)&pbfhMask->bmp,	CBM_INIT,
			       (PBYTE)pbafhOrg + pbfhMask->offBits, (PBITMAPINFO2)(PVOID)&pbfhMask->bmp),
	       (HBITMAP)NULL, pbfhMask->bmp.cx,	pbfhMask->bmp.cy / 2L, cBitmaps++);

		       /* Scan the bitmap array	for the	desired	icon	*/
		       /* type based on	the current screen display	*/
while (	pbafh->offNext )
   {
		       /* Point	to the next array header		*/

   pbafh = (PBITMAPARRAYFILEHEADER)((PBYTE)pbafhOrg + pbafh->offNext);
   pbfhClr  = (PBITMAPFILEHEADER)((PBYTE)(pbfhMask = &pbafh->bfh) + sizeof(BITMAPFILEHEADER) + 2 * sizeof(RGB2));


   if (	pbfhMask->usType == ulType )

		       /* Initialize the pointer information		*/

       CreateImage(GpiCreateBitmap(hPS,	(PBITMAPINFOHEADER2)&pbfhMask->bmp, CBM_INIT, (PBYTE)pbafhOrg +	pbfhMask->offBits,
				   (PBITMAPINFO2)(PVOID)&pbfhMask->bmp),
		   GpiCreateBitmap(hPS,	(PBITMAPINFOHEADER2)&pbfhClr->bmp, CBM_INIT, (PBYTE)pbafhOrg + pbfhClr->offBits,
				   (PBITMAPINFO2)(PVOID)&pbfhClr->bmp),	pbfhClr->bmp.cx, pbfhClr->bmp.cy, cBitmaps);
   else
		       /* Initialize the pointer information		*/

       CreateImage(GpiCreateBitmap(hPS,	(PBITMAPINFOHEADER2)&pbfhMask->bmp, CBM_INIT, (PBYTE)pbafhOrg +	pbfhMask->offBits,
				  (PBITMAPINFO2)(PVOID)&pbfhMask->bmp),
		  (HBITMAP)NULL, pbfhMask->bmp.cx, pbfhMask->bmp.cy / 2L, cBitmaps);

   if (	++cBitmaps == 32L )
       break;
   }
		       /* Release the memory presentation space		*/
WinReleasePS(hPS);
}
#pragma	subtitle("   Icon/Pointer Retrieve - Retrieve Single Image Function")
#pragma	page( )

/* --- GetSingleImage ---------------------------------	[ Private ] ---	*/
/*									*/
/*     This function is	used to	load a single icon from	an OS/2	2.x	*/
/*     icon or pointer file.						*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     PBITMAPFILEHEADER2 pbafh2; = Bitmap File	Header Pointer		*/
/*     ULONG		  ulType; = Image Type				*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     Nothing								*/
/*									*/
/* --------------------------------------------------------------------	*/

static VOID GetSingleImage(PBITMAPFILEHEADER2 pbfh2, ULONG ulType)

{
HPS		   hPS;		   /* Presentation Space		*/
PBITMAPFILEHEADER2 pbfh2Clr;	   /* Colour Bitmap File Header		*/
PBITMAPFILEHEADER2 pbfh2Mask;	   /* Mask Bitmap File Header		*/

		       /* Check	to see that the	size of	the bitmap	*/
		       /* info header is correct otherwise the icon	*/
		       /* cannot be created				*/

if ( pbfh2->bmp2.cbFix != sizeof(BITMAPINFOHEADER2) )

		       /* Invalid header size				*/
   return;

cBitmaps = 1L;

pbfh2Clr = (PBITMAPFILEHEADER2)((PBYTE)(pbfh2Mask = pbfh2) + sizeof(BITMAPFILEHEADER2) + 2 * sizeof(RGB2));


hPS = WinGetPS(HWND_DESKTOP);

if ( pbfh2Mask->usType == ulType )

		       /* Initialize the pointer information		*/

   CreateImage(GpiCreateBitmap(hPS, &pbfh2Mask->bmp2, CBM_INIT,	(PBYTE)pbfh2 + pbfh2Mask->offBits,
			       (PBITMAPINFO2)(PVOID)&pbfh2Mask->bmp2),
	       GpiCreateBitmap(hPS, &pbfh2Clr->bmp2, CBM_INIT, (PBYTE)pbfh2 + pbfh2Clr->offBits,
			       (PBITMAPINFO2)(PVOID)&pbfh2Clr->bmp2), pbfh2Clr->bmp2.cx, pbfh2Clr->bmp2.cy, 0);
else
		       /* Initialize the pointer information		*/

   CreateImage(GpiCreateBitmap(hPS = WinGetPS(HWND_DESKTOP), &pbfh2Mask->bmp2, CBM_INIT,
			       (PBYTE)pbfh2 + pbfh2Mask->offBits, (PBITMAPINFO2)(PVOID)&pbfh2Mask->bmp2),
	       (HBITMAP)NULL, pbfh2Mask->bmp2.cx, pbfh2Mask->bmp2.cy / 2L, 0);

		       /* Release the desktop presentation space	*/
WinReleasePS(hPS);

}
#pragma	subtitle("   Icon/Pointer Retrieve - Retrieve Single Image Function")
#pragma	page( )

/* --- GetSingleImage1 --------------------------------	[ Private ] ---	*/
/*									*/
/*     This function is	used to	load a single icon from	an OS/2	1.x	*/
/*     icon or pointer file.						*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     PBITMAPFILEHEADER pbafh;	 = Bitmap File Header Pointer		*/
/*     ULONG		 ulType; = Image Type				*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     Nothing								*/
/*									*/
/* --------------------------------------------------------------------	*/

static VOID GetSingleImage1(PBITMAPFILEHEADER pbfh, ULONG ulType)

{
HPS		  hPS;		   /* Presentation Space		*/
PBITMAPFILEHEADER pbfhClr;	   /* Colour Bitmap File Header		*/
PBITMAPFILEHEADER pbfhMask;	   /* Mask Bitmap File Header		*/

		       /* Check	to see that the	size of	the bitmap	*/
		       /* info header is correct otherwise the icon	*/
		       /* cannot be created				*/

if ( pbfh->bmp.cbFix !=	sizeof(BITMAPINFOHEADER) )

		       /* Invalid header size				*/
   return;

cBitmaps = 1L;

pbfhClr	 = (PBITMAPFILEHEADER)((PBYTE)(pbfhMask	= pbfh)	+ sizeof(BITMAPFILEHEADER) + 2 * sizeof(RGB));

hPS = WinGetPS(HWND_DESKTOP);

if ( pbfhMask->usType == ulType	)

		       /* Initialize the pointer information		*/

   CreateImage(GpiCreateBitmap(hPS, (PBITMAPINFOHEADER2)&pbfhMask->bmp,	CBM_INIT,
			       (PBYTE)pbfh + pbfhMask->offBits,	(PBITMAPINFO2)(PVOID)&pbfhMask->bmp),
	       GpiCreateBitmap(hPS, (PBITMAPINFOHEADER2)&pbfhClr->bmp, CBM_INIT, (PBYTE)pbfh + pbfhClr->offBits,
			       (PBITMAPINFO2)(PVOID)&pbfhClr->bmp), pbfhClr->bmp.cx, pbfhClr->bmp.cy, 0);
else
		       /* Initialize the pointer information		*/

   CreateImage(GpiCreateBitmap(hPS, (PBITMAPINFOHEADER2)&pbfhMask->bmp,	CBM_INIT,
			       (PBYTE)pbfh + pbfhMask->offBits,	(PBITMAPINFO2)(PVOID)&pbfhMask->bmp),
	       (HBITMAP)NULL, pbfhMask->bmp.cx,	pbfhMask->bmp.cy / 2L, 0);

		       /* Release the desktop presentation space	*/
WinReleasePS(hPS);

}
#pragma	subtitle("   Icon/Pointer Retrieve - Icon/Pointer Retrieve Function")
#pragma	page( )

/* --- GetIcon ----------------------------------------- [ Public ] ---	*/
/*									*/
/*     This function is	used to	load the requested icon	file and to	*/
/*     create a	icon set that can be displayed within the client	*/
/*     window to allow the user	to select the icon image to convert.	*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     PSZ pszFileName;	= Icon Filename					*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     Nothing								*/
/*									*/
/* --------------------------------------------------------------------	*/

VOID GetIconPointer(PSZ	pszFileName, BOOL fIcon)

{
FILESTATUS3	       fs;	   /* File Status Information		*/
HFILE		       hFile;	   /* File Handle			*/
PBITMAPARRAYFILEHEADER pbafh = 0;  /* Bitmap File Header Pointer	*/
ULONG		       cbRead;	   /* Bytes Read			*/
ULONG		       ulResult;   /* File Open	Fail Result		*/
register INT i;			   /* Loop Counter			*/

		       /* Open the requested icon file			*/

if ( DosOpen(pszFileName, &hFile, &ulResult, 0UL, FILE_NORMAL, OPEN_ACTION_OPEN_IF_EXISTS | OPEN_ACTION_FAIL_IF_NEW,
	     OPEN_ACCESS_READONLY | OPEN_SHARE_DENYNONE, NULL) )

		       /* Could	not open the file			*/
   return;
		       /* Get the file information to allow the	proper	*/
		       /* amount of memory to be allocated for the	*/
		       /* icon information				*/

DosQueryFileInfo(hFile,	FIL_STANDARD, (PVOID)&fs, sizeof(FILESTATUS3));

		       /* Allocate memory for the icon information    */

if ( (pbafh = (PBITMAPARRAYFILEHEADER)malloc(fs.cbFile)) == NULL )

		       /* Memory allocation failed			*/
   return;
		       /* Read into memory the icon file	      */

if ( DosRead(hFile, (PVOID)pbafh, fs.cbFile, (PULONG)&cbRead) || (cbRead != fs.cbFile) )
   {
		       /* Release the memory allocated for the icon	*/
		       /* information					*/
   free(pbafh);
		       /* Close	the icon file				*/
   DosClose(hFile);
		       /* Error	occurred in reading the	icon file	*/
   return;
   }
else
		       /* Close	the icon file				*/
   DosClose(hFile);

for ( i	= 0; i < cBitmaps; i++ )
   {
   GpiDeleteBitmap(abm[i].hbmMask);

   if (	abm[i].hbmImage	)
       GpiDeleteBitmap(abm[i].hbmImage);

   GpiDeleteBitmap(abm[i].hbm);
   }

memset(abm, 0, 32UL * sizeof(BITMAPSTACK));
cBitmaps = 0L;

if ( (pbafh->cbSize == sizeof(BITMAPARRAYFILEHEADER)) || (pbafh->cbSize	== sizeof(BITMAPFILEHEADER)) )
   {
   fWindowsBitmap = f20Bitmap =	fBitmapArray = FALSE;

		       /* Check	to see if the icon is monochrome      */

   if (	(pbafh->usType == BFT_ICON)    || (pbafh->usType == BFT_COLORICON) ||
	(pbafh->usType == BFT_POINTER) || (pbafh->usType == BFT_COLORPOINTER) )

		       /* Monochrome icon, convert into	a memory      */
		       /* icon and a valid icon	handle		    */

       GetSingleImage1((PBITMAPFILEHEADER)(PVOID)pbafh,	fIcon ?	BFT_COLORICON :	BFT_COLORPOINTER);
   else
		       /* Check	to see if the icon is a	bitmap array	*/
		       /* thereby indicating a coloured	icon	      */

       if ( pbafh->usType == BFT_BITMAPARRAY )
	   {
		       /* Coloured icon, convert into a	memory icon */
		       /* icon and a valid icon	handle		    */

	   BuildImageStack1((PBITMAPARRAYFILEHEADER)pbafh, fIcon ? BFT_COLORICON : BFT_COLORPOINTER);
	   fBitmapArray	= TRUE;
	   }
   }
else
   if (	(pbafh->cbSize == sizeof(BITMAPARRAYFILEHEADER2)) || (pbafh->cbSize == sizeof(BITMAPFILEHEADER2)) )
       {
       f20Bitmap = TRUE;
       fWindowsBitmap =	fBitmapArray = FALSE;

		       /* Check	to see if the icon is monochrome      */

       if ( (pbafh->usType == BFT_ICON)	   || (pbafh->usType ==	BFT_COLORICON) ||
	    (pbafh->usType == BFT_POINTER) || (pbafh->usType ==	BFT_COLORPOINTER) )

		       /* Monochrome icon, convert into	a memory      */
		       /* icon and a valid icon	handle		    */

	   GetSingleImage((PBITMAPFILEHEADER2)(PVOID)pbafh, fIcon ? BFT_COLORICON : BFT_COLORPOINTER);
       else
		       /* Check	to see if the icon is a	bitmap array	*/
		       /* thereby indicating a coloured	icon	      */

	   if (	pbafh->usType == BFT_BITMAPARRAY )
	       {
		       /* Coloured icon, convert into a	memory icon */
		       /* icon and a valid icon	handle		    */

	       BuildImageStack((PBITMAPARRAYFILEHEADER2)(PVOID)pbafh, fIcon ? BFT_COLORICON : BFT_COLORPOINTER);
	       fBitmapArray = TRUE;
	       }
       }
   else
		       /* Appears to be	a Windows icon,	try to	      */
		       /* convert it to	an OS/2	2.x format icon	      */
       {
       fBitmapArray = FALSE;
       if ( fIcon )
	   pbafh = (PBITMAPARRAYFILEHEADER)pbfh2GetWindowsIco((PBYTE)pbafh);
       else
	   pbafh = (PBITMAPARRAYFILEHEADER)pbfh2GetWindowsCur((PBYTE)pbafh);

		       /* Check	to see if the icon is monochrome      */

       if ( (pbafh->usType == BFT_ICON)	   || (pbafh->usType ==	BFT_COLORICON) ||
	    (pbafh->usType == BFT_POINTER) || (pbafh->usType ==	BFT_COLORPOINTER) )

		       /* Monochrome icon, convert into	a memory      */
		       /* icon and a valid icon	handle		    */

	   GetSingleImage((PBITMAPFILEHEADER2)(PVOID)pbafh, fIcon ? BFT_COLORICON : BFT_COLORPOINTER);
       else
		       /* Check	to see if the icon is a	bitmap array	*/
		       /* thereby indicating a coloured	icon	      */

	   if (	pbafh->usType == BFT_BITMAPARRAY )
	       {
			/* Coloured icon, convert into a memory	icon */
		       /* icon and a valid icon	handle		    */

	       BuildImageStack((PBITMAPARRAYFILEHEADER2)(PVOID)pbafh, fIcon ? BFT_COLORICON : BFT_COLORPOINTER);
	       fBitmapArray = TRUE;
	       }

       fWindowsBitmap =	f20Bitmap = TRUE;
       }

if ( pb	)
   free(pb);
pb = (PBYTE)pbafh;

}
#pragma	subtitle("   Icon/Pointer Retrieve - Windows Icon Image Retrieve Function")
#pragma	page( )

/* --- pbfh2GetWindowsIco ------------------------------ [ Public ] ---	*/
/*									*/
/*     This function is	used to	convert	a Windows icon to an OS/2	*/
/*     2.x type	of icon.						*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     PBYTE pbIcon; = Pointer to Windows Icon Data			*/
/*     ULONG cbData; = Windows Icon Data Size				*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     pbfh2GetWindowsIco = Convert Icon Data Pointer			*/
/*									*/
/* --------------------------------------------------------------------	*/

static PBITMAPFILEHEADER2 pbfh2GetWindowsIco(PBYTE pbIcon)

{
PBITMAPFILEHEADER2	pbfh2Clr;  /* Bitmap File Header Pointer	*/
PBITMAPFILEHEADER2	pbfh2Mask; /* Bitmap File Header Pointer	*/
PwinBITMAPINFOHEADER	pwbmpi;	   /* Bitmap File Header		*/
RGB2		       *prgb2;	   /* RGB Table	Pointer			*/
PBITMAPARRAYFILEHEADER2	pbafh2;	   /* Bitmap Array File	Header Pointer	*/
ULONG			cClrs;	   /* Colours Table Count		*/
ULONG			cbXorAnd;  /* Bytes Read			*/
PBYTE			pbImage;   /* Image Pointer			*/
PBYTE			pbWinImage;/* Image Pointer			*/
PBYTE			pbOS2Ico;  /* Data Pointer			*/
PwinICONDIR		pwid;	   /* Icon Directory Pointer		*/
ULONG			cbImage;   /* Image Size			*/
ULONG			cbMask;	   /* Mask Size				*/
ULONG			cbFile;	   /* File Size				*/
register INT i;			   /* Loop Counter			*/

pwid = (PwinICONDIR)pbIcon;
if ( pwid->idCount )
   for ( i = 0,	cbFile = 0UL; i	< pwid->idCount; i++ )
       {
       pwbmpi =	(PwinBITMAPINFOHEADER)(pbIcon +	pwid->idEntries[i].dwImageOffset);

       cbImage = (((pwbmpi->biWidth * pwbmpi->biBitCount + 31L)	& 0xffffffe0) >> 3) * pwbmpi->biHeight / 2L;
       cbMask  = (((pwbmpi->biWidth + 31L) & 0xffffffe0) >> 3) * pwbmpi->biHeight;

       if ( (cClrs = (ULONG)(1 << pwbmpi->biBitCount)) == 2 )
	   cbFile += sizeof(BITMAPARRAYFILEHEADER2) + 2	* sizeof(RGB2) + cbMask;
       else
	   cbFile += sizeof(BITMAPARRAYFILEHEADER2) + sizeof(BITMAPFILEHEADER2)	+ (cClrs + 2) *	sizeof(RGB2) + cbImage + cbMask;
       }
else
   {
   pwbmpi = (PwinBITMAPINFOHEADER)(pbIcon + pwid->idEntries[i].dwImageOffset);

   cbImage = (((pwbmpi->biWidth	* pwbmpi->biBitCount + 31L) & 0xffffffe0) >> 3)	* pwbmpi->biHeight / 2L;
   cbMask  = (((pwbmpi->biWidth	+ 31L) & 0xffffffe0) >>	3) * pwbmpi->biHeight;

   if (	(cClrs = (ULONG)(1 << pwbmpi->biBitCount)) == 2	)
       cbFile += sizeof(BITMAPFILEHEADER2) + 2 * sizeof(RGB2) +	cbMask;
   else
       cbFile += sizeof(BITMAPFILEHEADER2) * 2 + (cClrs	+ 2) * sizeof(RGB2) + cbImage +	cbMask / 2L;
   }
		       /* Initialize the bitmap	array header		*/

memset(pbOS2Ico	= (PBYTE)malloc(cbFile), 0, cbFile);

if ( pwid->idCount == 1	)
   {
   pbfh2Mask = (PBITMAPFILEHEADER2)pbOS2Ico;
   pbWinImage =	(PBYTE)(pwbmpi = (PwinBITMAPINFOHEADER)(pbIcon + pwid->idEntries[0].dwImageOffset)) + sizeof(winBITMAPINFOHEADER);

   cClrs   = (ULONG)(1 << pwbmpi->biBitCount);
   cbMask  = (((pwbmpi->biWidth	+ 31L) & 0xffffffe0) >>	3) * pwbmpi->biHeight;

		       /* Initialize the first icon header for the    */
		       /* icon					      */

   pbfh2Mask->usType   = (USHORT)(cClrs	== 2 ? BFT_ICON: BFT_COLORICON);
   pbfh2Mask->cbSize   = sizeof(BITMAPFILEHEADER2);
   pbfh2Mask->xHotspot = (SHORT)(pwbmpi->biWidth  / 2);
   pbfh2Mask->yHotspot = (SHORT)(pwbmpi->biHeight / 4);
   pbfh2Mask->offBits  = cClrs == 2 ? sizeof(BITMAPFILEHEADER2)	+ sizeof(RGB2) * 2 : sizeof(BITMAPFILEHEADER2) * 2 + sizeof(RGB2) * (cClrs + 2);
   pbfh2Mask->bmp2.cbFix	 =	   sizeof(BITMAPINFOHEADER2);
   pbfh2Mask->bmp2.cx		 =  (ULONG)pwbmpi->biWidth;
   pbfh2Mask->bmp2.cy		 =  (ULONG)pwbmpi->biHeight;
   pbfh2Mask->bmp2.cPlanes	 = (USHORT)1;
   pbfh2Mask->bmp2.cBitCount	 = (USHORT)1;
   pbfh2Mask->bmp2.cclrImportant =	   2;

   prgb2 = (RGB2 *)((PBYTE)pbfh2Mask + sizeof(BITMAPFILEHEADER2));
   prgb2[0].bBlue     =	(BYTE)0x00;
   prgb2[0].bGreen    =	(BYTE)0x00;
   prgb2[0].bRed      =	(BYTE)0x00;
   prgb2[0].fcOptions =	(BYTE)0x00;
   prgb2[1].bBlue     =	(BYTE)0xff;
   prgb2[1].bGreen    =	(BYTE)0xff;
   prgb2[1].bRed      =	(BYTE)0xff;
   prgb2[1].fcOptions =	(BYTE)0x00;

   if (	cClrs != 2 )
       {
		       /* Initialize the icon information for the     */
		       /* icon based on	the values from	the Windows   */
		       /* 3.1 icon				      */

       pbfh2Clr	= (PBITMAPFILEHEADER2)((PBYTE)pbfh2Mask	+ sizeof(BITMAPFILEHEADER2) + sizeof(RGB2) * 2);
       pbfh2Clr->usType	  = (USHORT)BFT_COLORICON;
       pbfh2Clr->cbSize	  = sizeof(BITMAPFILEHEADER2);
       pbfh2Clr->xHotspot = (SHORT)(pwbmpi->biWidth  / 2);
       pbfh2Clr->yHotspot = (SHORT)(pwbmpi->biHeight / 4);
       pbfh2Clr->offBits  = sizeof(BITMAPFILEHEADER2) *	2 + sizeof(RGB2) * (cClrs + 2) + cbMask;

       pbfh2Clr->bmp2.cbFix	    = sizeof(BITMAPINFOHEADER2);
       pbfh2Clr->bmp2.cx	    = (ULONG)pwbmpi->biWidth;
       pbfh2Clr->bmp2.cy	    = (ULONG)pwbmpi->biHeight /	2UL;
       pbfh2Clr->bmp2.cPlanes	    =	     pwbmpi->biPlanes;
       pbfh2Clr->bmp2.cBitCount	    =	     pwbmpi->biBitCount;
       pbfh2Clr->bmp2.cclrImportant = cClrs;

       pbImage	  = (PBYTE)pbfh2Clr + sizeof(BITMAPFILEHEADER2);

		       /* When a colour	table present, write out the	*/
		       /* colour table					*/
       if ( cClrs )
	   {
	   memcpy(pbImage, pbWinImage, sizeof(winRGBQUAD) * cClrs);
	   pbImage    += sizeof(RGB2) *	cClrs;
	   pbWinImage += sizeof(winRGBQUAD) * cClrs;
	   }

       if ( (cbXorAnd =	cbMask / 2) != 0 )
	   {
	   memcpy(pbImage += cbXorAnd, pbWinImage + pwbmpi->biSizeImage	- cbXorAnd, cbXorAnd);
	   pbImage += cbXorAnd;
	   }

       memcpy(pbImage, pbWinImage, pwbmpi->biSizeImage - cbXorAnd);
       }
   else
       memcpy((PBYTE)pbfh2Mask + sizeof(BITMAPFILEHEADER2) + 2 * sizeof(RGB2), pbWinImage + sizeof(winRGBQUAD) * 2, pwbmpi->biSizeImage);
   }
else
   {
   pbafh2 = (PBITMAPARRAYFILEHEADER2)pbOS2Ico;
   for ( i = 0;	i < pwid->idCount; i++ )
       {
       pwbmpi =	(PwinBITMAPINFOHEADER)(pbIcon +	pwid->idEntries[i].dwImageOffset);

       pbafh2->usType  = (USHORT)BFT_BITMAPARRAY;
       pbafh2->cbSize  = sizeof(BITMAPARRAYFILEHEADER2);

       if ( i != (pwid->idCount	- 1) )
	   if (	(cClrs = (ULONG)(1 << pwbmpi->biBitCount)) == 2	)
	       pbafh2->offNext = (ULONG)((PBYTE)pbafh2 - pbOS2Ico) + sizeof(BITMAPARRAYFILEHEADER2) + sizeof(RGB2) * 2;
	   else
	       pbafh2->offNext = (ULONG)((PBYTE)pbafh2 - pbOS2Ico) + sizeof(BITMAPARRAYFILEHEADER2) + sizeof(BITMAPFILEHEADER2)	+
				 sizeof(RGB2) *	(cClrs + 2);
       else
	   cClrs = (ULONG)(1 <<	pwbmpi->biBitCount);
       pbfh2Mask = &pbafh2->bfh2;

		       /* Initialize the first icon header for the    */
		       /* icon					      */

       pbfh2Mask->usType   = (USHORT)(cClrs == 2 ? BFT_ICON: BFT_COLORICON);
       pbfh2Mask->cbSize   = sizeof(BITMAPFILEHEADER2);
       pbfh2Mask->xHotspot = (SHORT)(pwbmpi->biWidth  /	2);
       pbfh2Mask->yHotspot = (SHORT)(pwbmpi->biHeight /	4);
       pbfh2Mask->bmp2.cbFix	     =	      sizeof(BITMAPINFOHEADER2);
       pbfh2Mask->bmp2.cx	     = (ULONG)pwbmpi->biWidth;
       pbfh2Mask->bmp2.cy	     = (ULONG)pwbmpi->biHeight;
       pbfh2Mask->bmp2.cPlanes	     = (USHORT)1;
       pbfh2Mask->bmp2.cBitCount     = (USHORT)1;
       pbfh2Mask->bmp2.cclrImportant =	      2;

       prgb2 = (RGB2 *)((PBYTE)pbfh2Mask + sizeof(BITMAPFILEHEADER2));
       prgb2[0].bBlue	  = (BYTE)0x00;
       prgb2[0].bGreen	  = (BYTE)0x00;
       prgb2[0].bRed	  = (BYTE)0x00;
       prgb2[0].fcOptions = (BYTE)0x00;
       prgb2[1].bBlue	  = (BYTE)0xff;
       prgb2[1].bGreen	  = (BYTE)0xff;
       prgb2[1].bRed	  = (BYTE)0xff;
       prgb2[1].fcOptions = (BYTE)0x00;

		       /* Initialize the icon information for the     */
		       /* icon based on	the values from	the Windows   */
		       /* 3.1 icon				      */
       if ( cClrs != 2 )
	   {
	   pbfh2Clr = (PBITMAPFILEHEADER2)((PBYTE)pbfh2Mask + sizeof(BITMAPFILEHEADER2)	+ sizeof(RGB2) * 2);
	   pbfh2Clr->usType   =	(USHORT)BFT_COLORICON;
	   pbfh2Clr->cbSize   =	sizeof(BITMAPFILEHEADER2);
	   pbfh2Clr->xHotspot =	(SHORT)(pwbmpi->biWidth	 / 2);
	   pbfh2Clr->yHotspot =	(SHORT)(pwbmpi->biHeight / 4);

	   pbfh2Clr->bmp2.cbFix		= sizeof(BITMAPINFOHEADER2);
	   pbfh2Clr->bmp2.cx		= (ULONG)pwbmpi->biWidth;
	   pbfh2Clr->bmp2.cy		= (ULONG)pwbmpi->biHeight / 2UL;
	   pbfh2Clr->bmp2.cPlanes	=	 pwbmpi->biPlanes;
	   pbfh2Clr->bmp2.cBitCount	=	 pwbmpi->biBitCount;
	   pbfh2Clr->bmp2.cclrImportant	= cClrs;

		       /* When a colour	table present, write out the	*/
		       /* colour table					*/

	   memcpy(pbImage = (PBYTE)pbfh2Clr + sizeof(BITMAPFILEHEADER2), pbWinImage = (PBYTE)pwbmpi + sizeof(winBITMAPINFOHEADER),
		  sizeof(winRGBQUAD) * cClrs);
	   pbafh2 = (PBITMAPARRAYFILEHEADER2)(pbImage += sizeof(RGB2) *	cClrs);
	   }
       else
	   pbImage = (PBYTE)(pbafh2 = (PBITMAPARRAYFILEHEADER2)((PBYTE)pbfh2Mask + sizeof(BITMAPFILEHEADER2) + sizeof(RGB2) * 2));
       }

   pbafh2 = (PBITMAPARRAYFILEHEADER2)pbOS2Ico;

   for ( i = 0;	i < pwid->idCount; i++ )
       {

       pbWinImage = (PBYTE)(pwbmpi = (PwinBITMAPINFOHEADER)(pbIcon + pwid->idEntries[i].dwImageOffset))	+ sizeof(winBITMAPINFOHEADER);

       cbMask  = (((pwbmpi->biWidth + 31L) & 0xffffffe0) >> 3) * pwbmpi->biHeight;

       pbfh2Mask = &pbafh2->bfh2;

		       /* Initialize the first icon header for the    */
		       /* icon					      */

       pbfh2Mask->offBits = (ULONG)(pbImage - pbOS2Ico);

       if ( (cClrs = (ULONG)(1 << pwbmpi->biBitCount)) != 2 )
	   {
		       /* Initialize the icon information for the     */
		       /* icon based on	the values from	the Windows   */
		       /* 3.1 icon				      */

	   pbfh2Clr = (PBITMAPFILEHEADER2)((PBYTE)pbfh2Mask + sizeof(BITMAPFILEHEADER2)	+ sizeof(RGB2) * 2);
	   pbfh2Clr->offBits = (ULONG)(pbImage - pbOS2Ico) + cbMask;

		       /* When a colour	table present, write out the	*/
		       /* colour table					*/

	   if (	(cClrs = (ULONG)(1 << pwbmpi->biBitCount)) != 0	)
	       pbWinImage += sizeof(winRGBQUAD)	* cClrs;

	   if (	(cbXorAnd = cbMask / 2UL) != 0 )
	       {
	       memcpy(pbImage += cbXorAnd, pbWinImage +	pwbmpi->biSizeImage - cbXorAnd,	cbXorAnd);
	       pbImage += cbXorAnd;
	       }

	   memcpy(pbImage, pbWinImage, pwbmpi->biSizeImage - cbXorAnd);
	   pbImage += pwbmpi->biSizeImage - cbXorAnd;
	   pbafh2 = (PBITMAPARRAYFILEHEADER2)((PBYTE)pbfh2Clr +	sizeof(BITMAPFILEHEADER2) + sizeof(RGB2) * cClrs);
	   }
       else
	   {
	   memcpy(pbImage, pbWinImage +	sizeof(winRGBQUAD) * 2,	pwbmpi->biSizeImage);
	   pbImage += pwbmpi->biSizeImage;
	   pbafh2 = (PBITMAPARRAYFILEHEADER2)((PBYTE)pbfh2Mask + sizeof(BITMAPFILEHEADER2) + sizeof(RGB2) * 2);
	   }
       }
   }

free(pbIcon);

return((PBITMAPFILEHEADER2)pbOS2Ico);
}
#pragma	subtitle("   Icon/Pointer Retrieve - Windows Cursor Image Retrieve Function")
#pragma	page( )

/* --- pbfh2GetWindowsCur ------------------------------ [ Public ] ---	*/
/*									*/
/*     This function is	used to	convert	a Windows cursor to an OS/2	*/
/*     2.x type	of icon.						*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     PBYTE pbCur;  = Pointer to Windows Cursor Data			*/
/*     ULONG cbData; = Windows Icon Data Size				*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     pbfh2GetWindowsCur = Convert Icon Data Pointer			*/
/*									*/
/* --------------------------------------------------------------------	*/

static PBITMAPFILEHEADER2 pbfh2GetWindowsCur(PBYTE pbCur)

{
PBITMAPFILEHEADER2	pbfh2Clr;  /* Bitmap File Header Pointer	*/
PBITMAPFILEHEADER2	pbfh2Mask; /* Bitmap File Header Pointer	*/
PBITMAPARRAYFILEHEADER2	pbafh2;	   /* Bitmap Array File	Header Pointer	*/
PwinBITMAPINFOHEADER	pwbmpi;	   /* Bitmap File Header		*/
RGB2		       *prgb2;	   /* RGB Table	Pointer			*/
ULONG			cClrs;	   /* Colours Table Count		*/
ULONG			cbXorAnd;  /* Bytes Read			*/
PBYTE			pbImage;   /* Image Pointer			*/
PBYTE			pbWinImage;/* Image Pointer			*/
PBYTE			pbOS2Ptr;  /* Data Pointer			*/
PwinCURSORDIR		pwcd;	   /* Cursor Directory Pointer		*/
ULONG			cbImage;   /* Image Size			*/
ULONG			cbMask;	   /* Mask Size				*/
ULONG			cbFile;	   /* File Size				*/
register INT i;			   /* Loop Counter			*/

pwcd = (PwinCURSORDIR)pbCur;
if ( pwcd->cdCount )
   for ( i = 0,	cbFile = 0UL; i	< pwcd->cdCount; i++ )
       {
       pwbmpi =	(PwinBITMAPINFOHEADER)(pbCur + pwcd->cdEntries[i].dwImageOffset);

       cbImage = (((pwbmpi->biWidth * pwbmpi->biBitCount + 31L)	& 0xffffffe0) >> 3) * pwbmpi->biHeight / 2L;
       cbMask  = (((pwbmpi->biWidth + 31L) & 0xffffffe0) >> 3) * pwbmpi->biHeight;

       if ( (cClrs = (ULONG)(1 << pwbmpi->biBitCount)) == 2 )
	   cbFile += sizeof(BITMAPARRAYFILEHEADER2) + 2	* sizeof(RGB2) + cbMask;
       else
	   cbFile += sizeof(BITMAPARRAYFILEHEADER2) + sizeof(BITMAPFILEHEADER2)	+ (cClrs + 2) *	sizeof(RGB2) +
		       cbImage + cbMask;
       }
else
   {
   pwbmpi = (PwinBITMAPINFOHEADER)(pbCur + pwcd->cdEntries[i].dwImageOffset);

   cbImage = (((pwbmpi->biWidth	* pwbmpi->biBitCount + 31L) & 0xffffffe0) >> 3)	* pwbmpi->biHeight / 2L;
   cbMask  = (((pwbmpi->biWidth	+ 31L) & 0xffffffe0) >>	3) * pwbmpi->biHeight;

   if (	(cClrs = (ULONG)(1 << pwbmpi->biBitCount)) == 2	)
       cbFile += sizeof(BITMAPFILEHEADER2) + 2 * sizeof(RGB2) +	cbMask;
   else
       cbFile += sizeof(BITMAPFILEHEADER2) * 2 + (cClrs	+ 2) * sizeof(RGB2) + cbImage +	cbMask / 2L;
   }
		       /* Initialize the bitmap	array header		*/

memset(pbOS2Ptr	= (PBYTE)malloc(cbFile), 0, cbFile);

if ( pwcd->cdCount == 1	)
   {
   pbfh2Mask = (PBITMAPFILEHEADER2)pbOS2Ptr;
   pbWinImage =	(PBYTE)(pwbmpi = (PwinBITMAPINFOHEADER)(pbCur +	pwcd->cdEntries[0].dwImageOffset)) + sizeof(winBITMAPINFOHEADER);

   cClrs = (ULONG)(1 <<	pwbmpi->biBitCount);
   cbMask  = (((pwbmpi->biWidth	+ 31L) & 0xffffffe0) >>	3) * pwbmpi->biHeight;

		       /* Initialize the first icon header for the    */
		       /* icon					      */

   pbfh2Mask->usType   = (USHORT)(cClrs	== 2 ? BFT_POINTER: BFT_COLORPOINTER);
   pbfh2Mask->cbSize   = sizeof(BITMAPFILEHEADER2);
   pbfh2Mask->xHotspot = (SHORT)pwcd->cdEntries[0].wXHotspot;
   pbfh2Mask->yHotspot = (SHORT)pwcd->cdEntries[0].wYHotspot;
   pbfh2Mask->offBits  = cClrs == 2 ? sizeof(BITMAPFILEHEADER2)	+ sizeof(RGB2) * 2 : sizeof(BITMAPFILEHEADER2) * 2 + sizeof(RGB2) * (cClrs + 2);
   pbfh2Mask->bmp2.cbFix	 =	   sizeof(BITMAPINFOHEADER2);
   pbfh2Mask->bmp2.cx		 =  (ULONG)pwbmpi->biWidth;
   pbfh2Mask->bmp2.cy		 =  (ULONG)pwbmpi->biHeight;
   pbfh2Mask->bmp2.cPlanes	 = (USHORT)1;
   pbfh2Mask->bmp2.cBitCount	 = (USHORT)1;
   pbfh2Mask->bmp2.cclrImportant =	   2;

   prgb2 = (RGB2 *)((PBYTE)pbfh2Mask + sizeof(BITMAPFILEHEADER2));
   prgb2[0].bBlue     =	(BYTE)0x00;
   prgb2[0].bGreen    =	(BYTE)0x00;
   prgb2[0].bRed      =	(BYTE)0x00;
   prgb2[0].fcOptions =	(BYTE)0x00;
   prgb2[1].bBlue     =	(BYTE)0xff;
   prgb2[1].bGreen    =	(BYTE)0xff;
   prgb2[1].bRed      =	(BYTE)0xff;
   prgb2[1].fcOptions =	(BYTE)0x00;

   if (	cClrs != 2 )
       {
		       /* Initialize the icon information for the     */
		       /* icon based on	the values from	the Windows   */
		       /* 3.1 icon				      */

       pbfh2Clr	= (PBITMAPFILEHEADER2)((PBYTE)pbfh2Mask	+ sizeof(BITMAPFILEHEADER2) + sizeof(RGB2) * 2);
       pbfh2Clr->usType	  = (USHORT)BFT_COLORPOINTER;
       pbfh2Clr->cbSize	  = sizeof(BITMAPFILEHEADER2);
       pbfh2Clr->xHotspot = (SHORT)pwcd->cdEntries[0].wXHotspot;
       pbfh2Clr->yHotspot = (SHORT)pwcd->cdEntries[0].wYHotspot;
       pbfh2Clr->offBits  = sizeof(BITMAPFILEHEADER2) *	2 + sizeof(RGB2) * (cClrs + 2) + cbMask;

       pbfh2Clr->bmp2.cbFix	    = sizeof(BITMAPINFOHEADER2);
       pbfh2Clr->bmp2.cx	    = (ULONG)pwbmpi->biWidth;
       pbfh2Clr->bmp2.cy	    = (ULONG)pwbmpi->biHeight /	2UL;
       pbfh2Clr->bmp2.cPlanes	    =	     pwbmpi->biPlanes;
       pbfh2Clr->bmp2.cBitCount	    =	     pwbmpi->biBitCount;
       pbfh2Clr->bmp2.cclrImportant = cClrs;

       pbImage	  = (PBYTE)pbfh2Clr + sizeof(BITMAPFILEHEADER2);

		       /* When a colour	table present, write out the	*/
		       /* colour table					*/
       if ( cClrs )
	   {
	   memcpy(pbImage, pbWinImage, sizeof(winRGBQUAD) * cClrs);
	   pbImage    += sizeof(RGB2) *	cClrs;
	   pbWinImage += sizeof(winRGBQUAD) * cClrs;
	   }

       if ( (cbXorAnd =	cbMask / 2UL) != 0 )
	   {
	   memcpy(pbImage += cbXorAnd, pbWinImage + pwbmpi->biSizeImage	- cbXorAnd, cbXorAnd);
	   pbImage += cbXorAnd;
	   }

       memcpy(pbImage, pbWinImage, pwbmpi->biSizeImage - cbXorAnd);
       }
   else
       {
       memcpy((PBYTE)pbfh2Mask + sizeof(BITMAPFILEHEADER2) + 2 * sizeof(RGB2), pbWinImage + sizeof(winRGBQUAD) * 2, pwbmpi->biSizeImage);
       }
   }
else
   {
   pbafh2 = (PBITMAPARRAYFILEHEADER2)pbOS2Ptr;
   for ( i = 0;	i < pwcd->cdCount; i++ )
       {
       pwbmpi =	(PwinBITMAPINFOHEADER)(pbCur + pwcd->cdEntries[i].dwImageOffset);

       pbafh2->usType  = (USHORT)BFT_BITMAPARRAY;
       pbafh2->cbSize  = sizeof(BITMAPARRAYFILEHEADER2);

       if ( i != (pwcd->cdCount	- 1) )
	   if (	(cClrs = (ULONG)(1 << pwbmpi->biBitCount)) == 2	)
	       pbafh2->offNext = (ULONG)((PBYTE)pbafh2 - pbOS2Ptr) + sizeof(BITMAPARRAYFILEHEADER2) + sizeof(RGB2) * 2;
	   else
	       pbafh2->offNext = (ULONG)((PBYTE)pbafh2 - pbOS2Ptr) + sizeof(BITMAPARRAYFILEHEADER2) + sizeof(BITMAPFILEHEADER2)	+
				 sizeof(RGB2) *	(cClrs + 2);
       else
	   cClrs = (ULONG)(1 <<	pwbmpi->biBitCount);
       pbfh2Mask = &pbafh2->bfh2;

		       /* Initialize the first icon header for the    */
		       /* icon					      */

       pbfh2Mask->usType   = (USHORT)(cClrs == 2 ? BFT_POINTER:	BFT_COLORPOINTER);
       pbfh2Mask->cbSize   = sizeof(BITMAPFILEHEADER2);
       pbfh2Mask->xHotspot = (SHORT)pwcd->cdEntries[i].wXHotspot;
       pbfh2Mask->yHotspot = (SHORT)pwcd->cdEntries[i].wYHotspot;
       pbfh2Mask->bmp2.cbFix	     =	      sizeof(BITMAPINFOHEADER2);
       pbfh2Mask->bmp2.cx	     = (ULONG)pwbmpi->biWidth;
       pbfh2Mask->bmp2.cy	     = (ULONG)pwbmpi->biHeight;
       pbfh2Mask->bmp2.cPlanes	     = (USHORT)1;
       pbfh2Mask->bmp2.cBitCount     = (USHORT)1;
       pbfh2Mask->bmp2.cclrImportant =	      2;

       prgb2 = (RGB2 *)((PBYTE)pbfh2Mask + sizeof(BITMAPFILEHEADER2));
       prgb2[0].bBlue	  = (BYTE)0x00;
       prgb2[0].bGreen	  = (BYTE)0x00;
       prgb2[0].bRed	  = (BYTE)0x00;
       prgb2[0].fcOptions = (BYTE)0x00;
       prgb2[1].bBlue	  = (BYTE)0xff;
       prgb2[1].bGreen	  = (BYTE)0xff;
       prgb2[1].bRed	  = (BYTE)0xff;
       prgb2[1].fcOptions = (BYTE)0x00;

		       /* Initialize the icon information for the     */
		       /* icon based on	the values from	the Windows   */
		       /* 3.1 icon				      */

       if ( cClrs != 2 )
	   {
	   pbfh2Clr = (PBITMAPFILEHEADER2)((PBYTE)pbfh2Mask + sizeof(BITMAPFILEHEADER2)	+ sizeof(RGB2) * 2);
	   pbfh2Clr->usType   =	(USHORT)BFT_COLORPOINTER;
	   pbfh2Clr->cbSize   =	sizeof(BITMAPFILEHEADER2);
	   pbfh2Clr->xHotspot =	(SHORT)pwcd->cdEntries[i].wXHotspot;
	   pbfh2Clr->yHotspot =	(SHORT)pwcd->cdEntries[i].wYHotspot;

	   pbfh2Clr->bmp2.cbFix		= sizeof(BITMAPINFOHEADER2);
	   pbfh2Clr->bmp2.cx		= (ULONG)pwbmpi->biWidth;
	   pbfh2Clr->bmp2.cy		= (ULONG)pwbmpi->biHeight / 2UL;
	   pbfh2Clr->bmp2.cPlanes	=	 pwbmpi->biPlanes;
	   pbfh2Clr->bmp2.cBitCount	=	 pwbmpi->biBitCount;
	   pbfh2Clr->bmp2.cclrImportant	= cClrs;

	   pbImage    =	(PBYTE)pbfh2Clr	+ sizeof(BITMAPFILEHEADER2);
	   pbWinImage =	(PBYTE)pwbmpi +	sizeof(winBITMAPINFOHEADER);

		       /* When a colour	table present, write out the	*/
		       /* colour table					*/
	   if (	cClrs )
	       {
	       memcpy(pbImage, pbWinImage, sizeof(winRGBQUAD) *	cClrs);
	       pbImage	  += sizeof(RGB2) * cClrs;
	       }
	   pbafh2 = (PBITMAPARRAYFILEHEADER2)pbImage;
	   }
       else
	   pbImage = (PBYTE)(pbafh2 = (PBITMAPARRAYFILEHEADER2)((PBYTE)pbfh2Mask + sizeof(BITMAPFILEHEADER2) + sizeof(RGB2) * 2));
       }

   pbafh2 = (PBITMAPARRAYFILEHEADER2)pbOS2Ptr;

   for ( i = 0;	i < pwcd->cdCount; i++ )
       {

       pbWinImage = (PBYTE)(pwbmpi = (PwinBITMAPINFOHEADER)(pbCur + pwcd->cdEntries[i].dwImageOffset)) + sizeof(winBITMAPINFOHEADER);

       cbMask  = (((pwbmpi->biWidth + 31L) & 0xffffffe0) >> 3) * pwbmpi->biHeight;

       pbfh2Mask = &pbafh2->bfh2;

		       /* Initialize the first icon header for the    */
		       /* icon					      */

       pbfh2Mask->offBits = (ULONG)(pbImage - pbOS2Ptr);

       if ( (cClrs = (ULONG)(1 << pwbmpi->biBitCount)) != 2 )
	   {
		       /* Initialize the icon information for the     */
		       /* icon based on	the values from	the Windows   */
		       /* 3.1 icon				      */

	   pbfh2Clr = (PBITMAPFILEHEADER2)((PBYTE)pbfh2Mask + sizeof(BITMAPFILEHEADER2)	+ sizeof(RGB2) * 2);
	   pbfh2Clr->offBits = (ULONG)(pbImage - pbOS2Ptr) + cbMask;

		       /* When a colour	table present, write out the	*/
		       /* colour table					*/

	   if (	(cClrs = (ULONG)(1 << pwbmpi->biBitCount)) != 0	)
	       pbWinImage += sizeof(winRGBQUAD)	* cClrs;

	   if (	(cbXorAnd = cbMask / 2UL) != 0 )
	       {
	       memcpy(pbImage += cbXorAnd, pbWinImage +	pwbmpi->biSizeImage - cbXorAnd,	cbXorAnd);
	       pbImage += cbXorAnd;
	       }

	   memcpy(pbImage, pbWinImage, pwbmpi->biSizeImage - cbXorAnd);
	   pbImage += pwbmpi->biSizeImage - cbXorAnd;
	   pbafh2 = (PBITMAPARRAYFILEHEADER2)((PBYTE)pbfh2Clr +	sizeof(BITMAPFILEHEADER2) + sizeof(RGB2) * cClrs);
	   }
       else
	   {
	   memcpy(pbImage, pbWinImage +	sizeof(winRGBQUAD) * 2,	pwbmpi->biSizeImage);
	   pbImage += pwbmpi->biSizeImage;
	   pbafh2 = (PBITMAPARRAYFILEHEADER2)((PBYTE)pbfh2Mask + sizeof(BITMAPFILEHEADER2) + sizeof(RGB2) * 2);
	   }
       }
   }

free(pbCur);

return((PBITMAPFILEHEADER2)pbOS2Ptr);
}
