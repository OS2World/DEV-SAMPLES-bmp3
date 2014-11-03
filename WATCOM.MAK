
# MAKE file for WATCOM.Mak

# Revised:  1996-01-01

# Macro definitions

CC=Wcl386
C_SW=-ms -5s -s -ox -zp4 -w3 
RC_SW=-r 


ViewIco.Exe: AppDefs.Obj GetIco.Obj OpenDlg.Obj\
        Support.Obj ViewDlg.Obj ViewIco.Obj\
        ViewWnd.Obj
 WLink @WATCOM.Lnk
 rc -x viewico.res viewico.exe

AppDefs.Obj: AppDefs.C
 $(CC) $(C_SW) -fo=$*.Obj AppDefs.C

GetIco.Obj: GetIco.C
 $(CC) $(C_SW) -fo=$*.Obj GetIco.C

OpenDlg.Obj: OpenDlg.C
 $(CC) $(C_SW) -fo=$*.Obj OpenDlg.C

Support.Obj: Support.C
 $(CC) $(C_SW) -fo=$*.Obj Support.C

ViewDlg.Obj: ViewDlg.C
 $(CC) $(C_SW) -fo=$*.Obj ViewDlg.C

ViewIco.Obj: ViewIco.C
 $(CC) $(C_SW) -fo=$*.Obj ViewIco.C

ViewWnd.Obj: ViewWnd.C
 $(CC) $(C_SW) -fo=$*.Obj ViewWnd.C

