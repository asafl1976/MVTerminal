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

	Inspired by Ahmad Shyazli's DragEdit:
		-->http://www.codeguru.com/editctrl/DragEdit.html

	<rant>
	I am always disappointed when I see some great little utility function or class
	that is cluttered up with MFC. For the most part this is done needlessly as MFC
	can quite happily use Windows API functions.

	I liked Ahmad Shyazli's DragEdit as it was a clean piece of code but it too was
	wrapped in MFC stuff. Using it required adding a class and a member variable. If
	you already have your own edit control class then you would have to pick the DragEdit
	code out and retrofit it into your edit. This is just too much.

	Also, it's easy to wrap this code into a single API so why not do it! This has the
	side benefit that non-MFC developers can use it...and if it's wrapped in a DLL
	developers using other programming tools can also use it.

	I also noticed that it would be very easy to extend the basic functionality of
	DragEdit to accept multiple files and folders, or both.

	I'd never used these APIs before and without DragEdit it as inspiration it would
	have been a drag ;-)

	</rant>
	
	What else could be done...

	* Duplicate filtering. We could remove (or not add!) duplicate files.

History
31-10-2002	Created
----------------------------------------------------------------------*/
#include "stdafx.h"
#include <tchar.h>
#include "EnableDragEdit.h"
#include <afxmt.h>

class CData
{
public:
	UINT m_uFlags;
};

static LPCTSTR g_szOldProc = _T("DragEditOldProc");
static LPCTSTR g_szData = _T("DragEditData");

//
//	Handy little helper macro. Gives the number of elements in an array
#define countof( x ) ( sizeof( x ) / sizeof( x[ 0 ] ) )

//
//	This is the string that goes in between filenames.
static const TCHAR g_szFilenameDelimiter[] = _T("; ");
#define DELIMTER_LENGTH countof( g_szFilenameDelimiter )


static bool IsZeroLength( LPCTSTR pcszFilename )
//
//	Utility function to have a quick look at at whether the file is 
//	zero length.
//	Returns true if it is.
{
	bool bRetValue = false;

	WIN32_FIND_DATA wfd;
	HANDLE hFind = FindFirstFile( pcszFilename, &wfd );

	if( hFind != INVALID_HANDLE_VALUE )
	{
		bRetValue = wfd.nFileSizeHigh == 0 && wfd.nFileSizeLow == 0;

		FindClose(hFind);
	}

	return bRetValue;
}


static bool ExpandShortcut( LPTSTR pszBuffer, UINT uBufferLength )
//
//	Useful utility function that will expand a filename if it's a shortcut.
//	return true if it is a shortcut.
{
	bool bRetVal = false;
	//
	//	For some reason the IShellLink will read zero length files and return that they are
	//	shortcuts. It will of course return an empty resolved filename so it's not much use.
	//	We have a quick check to see if the file is zero length
	if( pszBuffer && !IsZeroLength( pszBuffer ) )
	{
		
    IShellLink* psl;

    HRESULT hres = ::CoCreateInstance( CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*) &psl );
    if( SUCCEEDED( hres ) )
    {

      // Get a pointer to the persist file interface
      IPersistFile* ppf;
      if( SUCCEEDED( psl->QueryInterface( IID_IPersistFile, (LPVOID*) &ppf ) ) )
			{
        //
				// Load shortcut and get it's path...watch for unicode though!
#ifdef _UNICODE
        if( SUCCEEDED(ppf->Load( pszBuffer, STGM_READ ) ) )
#else	//	_UNICODE
				//
				//	Convert to wide...
				WORD szWidePath[ MAX_PATH + MAX_PATH ];
				(void)::MultiByteToWideChar( CP_ACP, 0, pszBuffer, -1, (LPWSTR)szWidePath, countof( szWidePath ) );

        if( SUCCEEDED(ppf->Load( (LPWSTR)szWidePath, STGM_READ ) ) )
#endif	//	_UNICODE
				{

					if( SUCCEEDED( psl->GetPath( pszBuffer, uBufferLength, NULL, SLGP_UNCPRIORITY ) ) )
					{
						bRetVal = true;
					}

				}

				ppf->Release();
			}

			psl->Release();
    }
	}
	return bRetVal;
}


static void OnDrop( HDROP hDrop, HWND hwndTarget, CData *pData )
{
	//
	//	We need to initialise COM because our host may not have. It's currently harmless to do it multiple
	//	times and it's best to be safe. If we didn't do it we would be unable to expand shortcuts sent to us.
	(void)::CoInitialize( NULL );

	//
	//	Allocate one byte. We do this because it makes the loop below much simpler and the overal logic
	//	much-much cleaner.
	LPTSTR pszTextToUse = (LPTSTR)malloc( 1 * sizeof( TCHAR ) );
	*pszTextToUse = _T('\000');

	//
	//	If we are accumulating file/folder names then we need to initilise our
	if( pData->m_uFlags & DRAGEDIT_ACCUMULATE_NAMES )
	{
		const UINT uLength = ::GetWindowTextLength( hwndTarget );
		if( uLength )
		{

			LPTSTR pszNew = (LPTSTR)realloc( pszTextToUse, ( uLength + DELIMTER_LENGTH + 1 ) * sizeof( TCHAR ) );
			if( pszNew )
			{

				pszTextToUse = pszNew;
				(void)::GetWindowText( hwndTarget, pszTextToUse, uLength + 1 );
			}
		}
	}

	//
	//	Get the number of files dropped.
	const UINT uFileCount = ::DragQueryFile( hDrop, 0xFFFFFFFF, NULL, 0 );

	//
	// Iterate the list of files dropped.
	for( UINT uFileIndex = 0 ; uFileIndex < uFileCount; uFileIndex++ )
	{
		TCHAR szPath[ MAX_PATH + MAX_PATH ];

		//
		// Get the filename...We could get the length of the filename, allocate a buffer and use that.
		// It makes it more complex for an unlikely situation - maybe if we have problems we'll do it later.
		(void)::DragQueryFile( hDrop, uFileIndex, szPath, countof( szPath ) );

		if( pData->m_uFlags & DRAGEDIT_EXPAND_SHORTCUTS )
		{
			(void)ExpandShortcut( szPath, countof( szPath ) );
		}

		//
		//	Check to see if we allow folders...
		DWORD dwFileAttributes = GetFileAttributes( szPath );
		if( !( pData->m_uFlags & DRAGEDIT_FOLDERNAME ) &&  dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
		{
			continue;
		}

		//
		//	Check for "not folders" e.g. files
		if( !( pData->m_uFlags & DRAGEDIT_FILENAME ) && !( dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
		{
			continue;
		}

		if( _tcslen( pszTextToUse ) > 0 )
		{
			(void)strcat_s( pszTextToUse,strlen( g_szFilenameDelimiter),g_szFilenameDelimiter );
		}

		//
		//	Attempt to reallocate our string.
		//	All being well it will work, but if it doesn't then we'll just exit out loop. This way at least something will
		//	happen okay and we don't actually need to handle the error. However, silently failing is a Bad Thing and I ought
		//	to try harder in the creative department ;-)
		LPTSTR pszNew = (LPTSTR)realloc( pszTextToUse, ( _tcslen( pszTextToUse ) + _tcslen( szPath ) + DELIMTER_LENGTH )  * sizeof( TCHAR ) );
		if( !pszNew )
		{
			break;
		}
		pszTextToUse = pszNew;

		//
		//	Add our new path to the current set of paths.
		(void)strcat_s( pszTextToUse,strlen( szPath), szPath );

		//
		//	If we don't allow multiple names to be used then we should break out now.
		if( !( pData->m_uFlags & DRAGEDIT_MULTIPLE ) )
		{
			break;
		}
	}

	//
	//	Signal the end of our drag operation
	::DragFinish( hDrop );

	//
	//	Give our finished text to our window and clean up the resources we have used.
	(void)::SetWindowText( hwndTarget, pszTextToUse );
	free( pszTextToUse );

	::CoUninitialize();
}


static LRESULT CALLBACK SubClassedProc ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	WNDPROC	WndProc = reinterpret_cast< WNDPROC>( ::GetProp( hwnd, g_szOldProc ) );
	CData *pData = reinterpret_cast< CData * >( ::GetProp( hwnd, g_szData ) );
	ASSERT( WndProc );

	switch( message )
	{
		case WM_NCDESTROY:
		{
			//
			//	Clean up our junk so we don't leak.
			SetWindowLong( hwnd, GWL_WNDPROC, (LONG) WndProc );
			RemoveProp( hwnd, g_szOldProc );
			RemoveProp( hwnd, g_szData );

			delete pData;
		}
		break;

		case WM_DROPFILES:
		{
			OnDrop( (HDROP) wParam, hwnd, pData );
		}
		break;
	}

	return CallWindowProc( WndProc, hwnd, message, wParam, lParam );
}


BOOL EnableDragEdit( HWND hwndTarget, UINT uFlags )
{
	if( IsWindow( hwndTarget ) )
	{
		//
		//	If we set the flags as zero then we don't want any more drag-drop activity.
		//	If we don't specify files or folders then it makes sense to not accept drag operations
		if( uFlags == 0 || !( uFlags & DRAGEDIT_FOLDERNAME || uFlags & DRAGEDIT_FILENAME ) )
		{
			DragAcceptFiles( hwndTarget, FALSE );
		}
		else
		{
			//
			//	Check to see if we are already attached. If so then we only need to alter the flags.
			CData *pData = reinterpret_cast< CData * >( ::GetProp( hwndTarget, g_szData ) );
			if( pData )
			{			

				DragAcceptFiles( hwndTarget, TRUE );
				pData->m_uFlags = uFlags;
			}
			else
			{
				DragAcceptFiles( hwndTarget, TRUE );

				//
				//	Create our data object. We later give this to our subclassed edit control so we can 
				CData *pData = new CData;
				pData->m_uFlags = uFlags;

				VERIFY( ::SetProp( hwndTarget, g_szData, reinterpret_cast<HANDLE>( pData ) ) );

				//
				//	Subclass the edit control so we can catch some handy messages
				FARPROC lpfnWndProc = reinterpret_cast<FARPROC>( SetWindowLong( hwndTarget, GWL_WNDPROC, (LONG) SubClassedProc ) );
				ASSERT( lpfnWndProc != NULL );
				VERIFY( ::SetProp( hwndTarget, g_szOldProc, reinterpret_cast<HANDLE>( lpfnWndProc ) ) );
			}
		}
	}
	else
	{
		//
		//	Set the last error to be something meaningful
		SetLastErrorEx( ERROR_INVALID_WINDOW_HANDLE, SLE_ERROR );
	}

	return FALSE;
}
