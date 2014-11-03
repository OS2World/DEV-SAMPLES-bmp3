
# MAKE file for ViewIco.MAK     Release 5

# Revised:  1995-10-18

# Copyright ¸ 1987-1995  Prominare Inc.

# MAKE file created by Prominare Builder  Release 5

Update: $(OBJDIR)AppDefs.Obj $(OBJDIR)GetIco.Obj $(OBJDIR)OpenDlg.Obj\
        $(OBJDIR)Support.Obj $(OBJDIR)ViewDlg.Obj $(OBJDIR)ViewIco.Obj\
        $(OBJDIR)ViewWnd.Obj

$(OBJDIR)AppDefs.Obj: AppDefs.C
 $(CC) $(C_SW)-Fo$*.Obj AppDefs.C

$(OBJDIR)GetIco.Obj: GetIco.C
 $(CC) $(C_SW)-Fo$*.Obj GetIco.C

$(OBJDIR)OpenDlg.Obj: OpenDlg.C
 $(CC) $(C_SW)-Fo$*.Obj OpenDlg.C

$(OBJDIR)Support.Obj: Support.C
 $(CC) $(C_SW)-Fo$*.Obj Support.C

$(OBJDIR)ViewDlg.Obj: ViewDlg.C
 $(CC) $(C_SW)-Fo$*.Obj ViewDlg.C

$(OBJDIR)ViewIco.Obj: ViewIco.C
 $(CC) $(C_SW)-Fo$*.Obj ViewIco.C

$(OBJDIR)ViewWnd.Obj: ViewWnd.C
 $(CC) $(C_SW)-Fo$*.Obj ViewWnd.C

