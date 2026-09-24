// toolbar.h - toolbar window with icon buttons
#pragma once
#include <windows.h>

HWND CreateEditorToolbar(HWND hParent, HINSTANCE hInst);
LPCTSTR ToolTipText(int cmdId);
