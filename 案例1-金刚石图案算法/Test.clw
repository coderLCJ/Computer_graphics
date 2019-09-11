; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CTestView
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "test.h"
LastPage=0

ClassCount=6
Class1=CInputDlg
Class2=CMainFrame
Class3=CTestApp
Class4=CAboutDlg
Class5=CTestDoc
Class6=CTestView

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_DIALOG1

[CLS:CInputDlg]
Type=0
BaseClass=CDialog
HeaderFile=InputDlg.h
ImplementationFile=InputDlg.cpp
LastObject=ID_MDRAW

[CLS:CMainFrame]
Type=0
BaseClass=CFrameWnd
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp

[CLS:CTestApp]
Type=0
BaseClass=CWinApp
HeaderFile=Test.h
ImplementationFile=Test.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=Test.cpp
ImplementationFile=Test.cpp

[CLS:CTestDoc]
Type=0
BaseClass=CDocument
HeaderFile=TestDoc.h
ImplementationFile=TestDoc.cpp

[CLS:CTestView]
Type=0
BaseClass=CView
HeaderFile=TestView.h
ImplementationFile=TestView.cpp
Filter=C
VirtualFilter=VWC
LastObject=IDM_DRAWPIC

[DLG:IDD_DIALOG1]
Type=1
Class=CInputDlg
ControlCount=5
Control1=IDOK,button,1342242817
Control2=IDC_STATIC,static,1342308352
Control3=IDC_EDIT1,edit,1350631552
Control4=IDC_STATIC,static,1342308352
Control5=IDC_EDIT2,edit,1350631552

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=2
Control1=IDOK,button,1342373889
Control2=IDC_STATIC,static,1342177294

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_APP_EXIT
Command2=IDM_DRAWPIC
Command3=ID_APP_ABOUT
CommandCount=3

[MNU:IDR_MAINFRAME]
Type=1
Class=CTestView
Command1=ID_APP_EXIT
Command2=IDM_DRAWPIC
Command3=ID_APP_ABOUT
CommandCount=3

[ACL:IDR_MAINFRAME]
Type=1
Class=?
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

