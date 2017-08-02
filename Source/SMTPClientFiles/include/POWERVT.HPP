//***************************************************************************
//
//  Module: PowerVt.hpp
//
//  Purpose:
//     Declares CPowerVt class
//             
//***************************************************************************
//
//  Written by Dart Communication Application Programming Group.
//  Copyright (c) 1994 Dart Communications.  All Rights Reserved.
//
//***************************************************************************
//
// .mjb.1 12/17/96 PrintEvent() added to support PowerVT 3.0.
/* .mjb.2 5/16/97  ClickEvent() 
		cType parameter added
			0 - left down 
			1 - left up
			2 - right down
			3 - right up
			4 - left double click
			5 - right double click 
		fwKeys parameter added. Combination of:
			MK_LBUTTON          1
			MK_RBUTTON          2
			MK_SHIFT            4
			MK_CONTROL          8
			MK_MBUTTON          16   */


#ifndef POWERVT_HPP_DEFINED
#define POWERVT_HPP_DEFINED
#include "powertcp.h"

// typedefs used here...
typedef enum
{
   Normal,
   Auto,
   Controller
} PRINTMODE;

// Classes used...
class CVt;

//
// CPowerVt
// Defines the functions required to provide VT320 emulator to user
// 
// 3/12/97 Maintenance: Changed class name from CPowerVt220 to CPowerVt
//         This coincides with upgrading to VT320 functionality
//         When we upgrade to VT420, no name change will occur
//
class CPowerVt
{
friend class CVt;
private:
	CVt &Vt;  
protected:
	// event notifications...
	virtual void HostCommandEvent (PT_ATTRIBUTE Attr, DWORD Value, BOOL FAR *Ignore); 
	virtual void KeyDownEvent (UINT FAR *KeyCode, UINT FAR *Shift, BOOL Extended);
	virtual void KeyPressEvent (LPBYTE Data, UINT ByteCnt) = 0; // pure virtual
	virtual void ClickEvent (WORD xPos, WORD yPos, WORD cType, WORD fwKeys); // 
	virtual void NewLine (LPSTR LineText, UINT ByteCnt); // notification of line text
	virtual void PrintEvent (LPBYTE lpData, UINT nByteCnt, PRINTMODE PrintMode); // notification data to print
public:
	HWND Create (HWND hParentWnd);
	void Display (LPVOID Data, UINT ByteCnt);
	DWORD Get (PT_ATTRIBUTE);
	void GetStyle (LPSTR Buffer, UINT FAR *Size);
	PT_ATTRIBUTE GetTerminal();
	void GetText (LPSTR Buffer, UINT FAR *Size);
	BOOL Set (PT_ATTRIBUTE Attribute, DWORD Value);
	CPowerVt (HINSTANCE);
	~CPowerVt (void);
	int operator = (CPowerVt &);

	// djl - add functions for easier buffer access
	void GetTextEx(LPSTR Buffer, UINT FAR *Size, int nStartLine);
	void GetStyleEx(LPSTR Buffer, UINT FAR *Size, int nStartLine);
};

#endif // POWERVT_HPP_DEFINED

