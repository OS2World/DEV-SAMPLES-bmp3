
# MAKE file for IBM.Mak

# Revised:  1996-01-01

# Macro definitions

CC=Icc
C_SW=-G5 -O+ -W3 -C 
RC_SW=-r 


ViewIco.Exe: AppDefs.Obj GetIco.Obj OpenDlg.Obj\
        Support.Obj ViewDlg.Obj ViewIco.Obj\
        ViewWnd.Obj
 ILink /FREE /BASE:0x10000 /ALIGN:4 /EXEPACK @IBM.Lnk
 rc -x viewico.res viewico.exe

AppDefs.Obj: AppDefs.C
 $(CC) $(C_SW) -Fo$*.Obj AppDefs.C

GetIco.Obj: GetIco.C
 $(CC) $(C_SW) -Fo$*.Obj GetIco.C

OpenDlg.Obj: OpenDlg.C
 $(CC) $(C_SW) -Fo$*.Obj OpenDlg.C

Support.Obj: Support.C
 $(CC) $(C_SW) -Fo$*.Obj Support.C

ViewDlg.Obj: ViewDlg.C
 $(CC) $(C_SW) -Fo$*.Obj ViewDlg.C

ViewIco.Obj: ViewIco.C
 $(CC) $(C_SW) -Fo$*.Obj ViewIco.C

ViewWnd.Obj: ViewWnd.C
 $(CC) $(C_SW) -Fo$*.Obj ViewWnd.C

