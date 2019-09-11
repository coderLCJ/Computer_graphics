; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CLeftPortion
LastTemplate=CFormView
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Test.h"
LastPage=0

ClassCount=6
Class1=CTestApp
Class2=CTestDoc
Class3=CTestView
Class4=CMainFrame

ResourceCount=3
Resource1=IDR_MAINFRAME
Class5=CAboutDlg
Resource2=IDD_ABOUTBOX
Class6=CLeftPortion
Resource3=IDD_DIALOG1

[CLS:CTestApp]
Type=0
HeaderFile=Test.h
ImplementationFile=Test.cpp
Filter=N

[CLS:CTestDoc]
Type=0
HeaderFile=TestDoc.h
ImplementationFile=TestDoc.cpp
Filter=N

[CLS:CTestView]
Type=0
HeaderFile=TestView.h
ImplementationFile=TestView.cpp
Filter=C
LastObject=CTestView
BaseClass=CView
VirtualFilter=VWC


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
LastObject=CMainFrame
BaseClass=CFrameWnd
VirtualFilter=fWC




[CLS:CAboutDlg]
Type=0
HeaderFile=Test.cpp
ImplementationFile=Test.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=2
Control1=IDOK,button,1342373889
Control2=IDC_STATIC,static,1342177294

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_APP_EXIT
Command2=IDM_PLAY
Command3=ID_APP_ABOUT
CommandCount=3

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_APP_EXIT
Command2=IDM_PLAY
Command3=ID_APP_ABOUT
CommandCount=3

[DLG:IDD_DIALOG1]
Type=1
Class=CLeftPortion
ControlCount=14
Control1=IDC_STATIC,button,1342177287
Control2=IDC_AMBIENT,button,1342242819
Control3=IDC_DIFFUSE,button,1342242819
Control4=IDC_STATIC,button,1342177287
Control5=IDC_STATIC,button,1342177287
Control6=IDC_SPECULAR,button,1342242819
Control7=IDC_GOLD,button,1342308361
Control8=IDC_SILVER,button,1342177289
Control9=IDC_RUBY,button,1342177289
Control10=IDC_BERYL,button,1342177289
Control11=IDC_LEFTTOP,button,1342308361
Control12=IDC_RIGHTTOP,button,1342177289
Control13=IDC_LEFTDOWN,button,1342177289
Control14=IDC_RIGHTDOWN,button,1342177289

[CLS:CLeftPortion]
Type=0
HeaderFile=LeftPortion.h
ImplementationFile=LeftPortion.cpp
BaseClass=CFormView
Filter=D
LastObject=IDC_AMBIENT
VirtualFilter=VWC

