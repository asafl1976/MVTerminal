#include <windows.h>
#include <windowsx.h>  
#include <stdio.h>                        
#include <string.h>
#include "about.h"                     


//*************************************************************************
                         

BOOL AboutDlg_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)  
   {
   char szBuffer[256];
      
   LPCSTR lpszLibsUsed = (LPCSTR) lParam;
                                                             
   strcpy(szBuffer, "Sample application using ");
   _fstrcat(szBuffer, lpszLibsUsed);
   SetDlgItemText(hwnd, IDC_SAMPLEUSING, szBuffer);
                  
#ifdef WIN32
   {
   SYSTEM_INFO systemInfo;
   MEMORYSTATUS memoryStatus;
   
   // Get type of processor.
   //
   GetSystemInfo(&systemInfo);
   switch (systemInfo.dwProcessorType)
      {
      case PROCESSOR_INTEL_386:
         SetDlgItemText(hwnd, IDC_PROCESSOR, "Processor: 386");
         break;
      case PROCESSOR_INTEL_486:        
         SetDlgItemText(hwnd, IDC_PROCESSOR, "Processor: 486");
         break;
      case PROCESSOR_INTEL_PENTIUM:
         SetDlgItemText(hwnd, IDC_PROCESSOR, "Processor: Pentium");
         break;
      default:                               
         SetDlgItemText(hwnd, IDC_PROCESSOR, "Processor: Unknown");
         break;
      }                                        
         
   GlobalMemoryStatus(&memoryStatus);
   
   sprintf(szBuffer, "Memory Available: %ld KB Free", 
         memoryStatus.dwAvailPhys / 1024L);
   SetDlgItemText(hwnd, IDC_MEMORYAVAIL, szBuffer);         
   }    
#else 
   { 
   DWORD dwFlags;

   // Display CPU and mode.
   dwFlags = GetWinFlags();    
   sprintf(szBuffer, "%s %s Mode",
      (dwFlags & WF_CPU286) ? "286" :
      (dwFlags & WF_CPU386) ? "386" :
      (dwFlags & WF_CPU486) ? "486" :
      "Unknown",
      (dwFlags & WF_ENHANCED) ? "Enhanced" : "Standard");
   SetDlgItemText(hwnd, IDC_PROCESSOR, szBuffer);
   
   // Display free space.
   sprintf(szBuffer, "Memory Available: %d KB Free", GetFreeSpace(0) / 1024);
   SetDlgItemText(hwnd, IDC_MEMORYAVAIL, szBuffer);
   }
#endif
   
   return TRUE;   
   }


//*************************************************************************


HBRUSH AboutDlg_OnCtlColor(HWND hwnd, HDC hdc, HWND hwndChild, int type)
   {
   SetBkMode(hdc, TRANSPARENT);
   return (HBRUSH) GetStockObject(LTGRAY_BRUSH);
   }
                            

//*************************************************************************


void AboutDlg_OnDrawItem(HWND hwnd, const DRAWITEMSTRUCT FAR* lpDrawItem)
   {                                    
   HICON hIcon;
   HBITMAP hBitmap;
   HPEN hpen, hpenOld;
   HBRUSH hBrush;
   HDC hmemDC;
   BITMAP bm;  
   RECT rect;
   int cxIcon, cyIcon;
   int cxClient, cyClient;  
               
   switch (lpDrawItem->CtlID)
      {
      case IDC_BIGICON:
                                       
         cxIcon = GetSystemMetrics(SM_CXICON);
         cyIcon = GetSystemMetrics(SM_CYICON);
      
         GetClientRect(lpDrawItem->hwndItem, &rect); 
         cxClient = rect.right;
         cyClient = rect.bottom;
      
         // Load icon.                                 

#ifdef WIN32
                                                                      
         if (0 == (hIcon = (HICON) GetClassLong(GetParent(hwnd), GCL_HICON)))
            break;
      
#else
         if (0 == (hIcon = (HICON) GetClassWord(GetParent(hwnd), GCW_HICON)))
            break;

#endif

         // Create off-screen bitmap.
         if (0 == (hBitmap = CreateCompatibleBitmap(lpDrawItem->hDC, cxIcon, cyIcon)))
            break;               
         if (0 == (hmemDC = CreateCompatibleDC(lpDrawItem->hDC)))
            break;
      
         // Copy the bits already on the screen to an off-screen bitmap.
         SelectObject(hmemDC, hBitmap);
         StretchBlt(hmemDC, 0, 0, cxIcon, cyIcon, lpDrawItem->hDC, 
            2, 2, cxClient, cyClient, SRCCOPY);
      
         // Draw the icon on the off-screen bitmap.
         DrawIcon(hmemDC, 0, 0, hIcon);
                  
         // Draw a border around the icon.
         hpen = GetStockObject(BLACK_PEN);
         hpenOld = SelectObject(lpDrawItem->hDC, hpen);
         Rectangle(lpDrawItem->hDC, 0, 0, cxClient - 4, cyClient - 4);  
         if (hpenOld)
            SelectObject(lpDrawItem->hDC, hpenOld);
                                      
         // Draw a shadow below the icon.
         hBrush = GetStockObject(DKGRAY_BRUSH);
         rect.left = cxClient - 4;
         rect.top = 4;
         rect.right = cxClient;
         rect.bottom = cyClient;
         FillRect(lpDrawItem->hDC, &rect, hBrush);
      
         // Draw a shadow to the right of the icon.
         rect.left = 4;
         rect.top = cyClient - 4;
         rect.right = cxClient;
         rect.bottom = cyClient;
         FillRect(lpDrawItem->hDC, &rect, hBrush);
                  
         // Copy the off-screen bitmap to the screen.
         StretchBlt(lpDrawItem->hDC, 2, 2, cxClient - 8,
            cyClient - 8, hmemDC, 0, 0, cxIcon, cyIcon, SRCCOPY);                            
         break;                                    
         
      case IDC_DARTLOGO:
         GetClientRect(lpDrawItem->hwndItem, &rect);
         cxClient = rect.right;
         cyClient = rect.bottom;
                  
         // Create off-screen bitmap.
         if (0 == (hmemDC = CreateCompatibleDC(lpDrawItem->hDC)))
            break;
                  
         hBitmap = LoadBitmap(GetWindowInstance(hwnd), "DARTLOGO");
         SelectObject(hmemDC, hBitmap);
                  
         GetObject(hBitmap, sizeof(BITMAP), (LPSTR) &bm);
                  
         StretchBlt(lpDrawItem->hDC, 0, 0, cxClient, cyClient, hmemDC, 0, 0,
            bm.bmWidth, bm.bmHeight, SRCCOPY);                 
         break;
   
      }  
   }
               
          
//*************************************************************************


LRESULT CALLBACK AboutDlg_DlgProc(HWND hwnd, UINT msg, UINT wParam, LONG lParam)
   {
   switch (msg)
      {     
      case WM_COMMAND:
         if (IDOK != wParam) break;
      case WM_CLOSE:
         EndDialog(hwnd, 0); 
         break;

      HANDLE_MSG(hwnd, WM_INITDIALOG, AboutDlg_OnInitDialog);
      HANDLE_MSG(hwnd, WM_DRAWITEM, AboutDlg_OnDrawItem);

#ifdef WIN32

      HANDLE_MSG(hwnd, WM_CTLCOLORBTN, AboutDlg_OnCtlColor);
      HANDLE_MSG(hwnd, WM_CTLCOLORDLG, AboutDlg_OnCtlColor);
      HANDLE_MSG(hwnd, WM_CTLCOLOREDIT, AboutDlg_OnCtlColor);
      HANDLE_MSG(hwnd, WM_CTLCOLORLISTBOX, AboutDlg_OnCtlColor);
      HANDLE_MSG(hwnd, WM_CTLCOLORMSGBOX, AboutDlg_OnCtlColor);
      HANDLE_MSG(hwnd, WM_CTLCOLORSCROLLBAR, AboutDlg_OnCtlColor);
      HANDLE_MSG(hwnd, WM_CTLCOLORSTATIC, AboutDlg_OnCtlColor);

      
#else

      HANDLE_MSG(hwnd, WM_CTLCOLOR, AboutDlg_OnCtlColor);

#endif
      }           
      return FALSE;
   }


//*************************************************************************

                                         
void AboutDlg_Do(HWND hwndOwner, LPCSTR lpszLibsUsed)
   {                                              
   DialogBoxParam(GetWindowInstance(hwndOwner), "ABOUTDLG", hwndOwner, 
         (DLGPROC) AboutDlg_DlgProc, (LPARAM) (LPCSTR) lpszLibsUsed);
   }
