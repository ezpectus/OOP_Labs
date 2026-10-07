#pragma once
// dialogs.h — модальні діалоги програми (РГР)
#include <windows.h>
#include "student.h"

// Діалог вводу/редагування студента.
// Повертає TRUE якщо OK і out заповнений валідними даними.
BOOL StudentEditDialog(HWND hOwner, const wchar_t* title, Student& out);
