
# MAKE file for Borland.Mak

# Revised:  1996-01-01

# Macro definitions

CC=BCC
RC=Rc
C_SW=-Ox -v- -w -c 
RC_SW=-r 


ViewIco.Exe: AppDefs.Obj GetIco.Obj OpenDlg.Obj\
        Support.Obj ViewDlg.Obj ViewIco.Obj\
        ViewWnd.Obj
 TLink @OS2PM_2x.Lnk
 rc -x viewico.res viewico.exe

AppDefs.Obj: AppDefs.C
 $(CC) $(C_SW) -o$*.Obj AppDefs.C

GetIco.Obj: GetIco.C
 $(CC) $(C_SW) -o$*.Obj GetIco.C

OpenDlg.Obj: OpenDlg.C
 $(CC) $(C_SW) -o$*.Obj OpenDlg.C

Support.Obj: Support.C
 $(CC) $(C_SW) -o$*.Obj Support.C

ViewDlg.Obj: ViewDlg.C
 $(CC) $(C_SW) -o$*.Obj ViewDlg.C

ViewIco.Obj: ViewIco.C
 $(CC) $(C_SW) -o$*.Obj ViewIco.C

ViewWnd.Obj: ViewWnd.C
 $(CC) $(C_SW) -o$*.Obj ViewWnd.C

