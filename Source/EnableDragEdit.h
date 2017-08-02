/*----------------------------------------------------------------------
EnableDragEdit.h - Single function to enable dragging files onto an edit
control
Copyright (C) 2002  Russ Freeman

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

http://www.gnu.org/copyleft/lesser.html

Owner:	russf@gipsysoft.com
Website: http://www.gipsysoft.com/
Purpose:	
	Windows API style function to enable an edit control to handle drop operations
	for files or folders, or both.

History
31-10-2002	Created

Usage:
	Simply call it, pass a window handle and the options you want.

	If you send uFlags as zero then the edit will not accept dropped files.

	If you set uFlags without DRAGEDIT_FILENAME and without DRAGEDIT_FOLDERNAME then
	the edit control will not accept dropped files.

----------------------------------------------------------------------*/
#ifndef ENABLEDRAGEDIT_H
#define ENABLEDRAGEDIT_H

#define DRAGEDIT_FILENAME	1
#define DRAGEDIT_FOLDERNAME	2
#define DRAGEDIT_MULTIPLE	4
#define DRAGEDIT_EXPAND_SHORTCUTS 8
#define DRAGEDIT_ACCUMULATE_NAMES 16

BOOL EnableDragEdit( HWND hwndTarget, UINT uFlags );

#ifdef __AFX_H__

	//
	//	Inline MFC version
	inline BOOL EnableDragEdit( CWnd *pWnd, UINT uFlags )
	{
		return ::EnableDragEdit( pWnd->GetSafeHwnd(), uFlags );
	}

#endif	//	__AFX_H__

#endif //ENABLEDRAGEDIT_H