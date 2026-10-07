// dialogs.cpp — реалізація діалогів (РГР)
#include "dialogs.h"
#include "resource.h"

// ---- контекст діалогу Add/Edit ----
struct EditCtx
{
    const wchar_t* title;
    Student* result;
    BOOL ok;
};

static int GetGradeField(HWND hDlg, int idc)
{
    wchar_t buf[16];
    GetDlgItemTextW(hDlg, idc, buf, 16);
    return _wtoi(buf);
}

static void SetGradeField(HWND hDlg, int idc, int v)
{
    wchar_t buf[16];
    _itow_s(v, buf, 10);
    SetDlgItemTextW(hDlg, idc, buf);
}

static INT_PTR CALLBACK StudentDlgProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp)
{
    static EditCtx* ctx = nullptr;
    switch (msg)
    {
    case WM_INITDIALOG:
        ctx = reinterpret_cast<EditCtx*>(lp);
        SetWindowTextW(hDlg, ctx->title);
        if (ctx->result)
        {
            SetDlgItemTextW(hDlg, IDC_ED_NAME, ctx->result->getName().c_str());
            SetDlgItemTextW(hDlg, IDC_ED_BOOK, ctx->result->getBookNo().c_str());
            for (int i = 0; i < Student::GRADE_COUNT; i++)
                SetGradeField(hDlg, IDC_ED_G1 + i, ctx->result->getGrade(i));
        }
        return TRUE;

    case WM_COMMAND:
        if (LOWORD(wp) == IDOK)
        {
            Student s;
            wchar_t buf[128];
            GetDlgItemTextW(hDlg, IDC_ED_NAME, buf, 128);
            s.setName(buf);
            GetDlgItemTextW(hDlg, IDC_ED_BOOK, buf, 128);
            s.setBookNo(buf);
            for (int i = 0; i < Student::GRADE_COUNT; i++)
                s.setGrade(i, GetGradeField(hDlg, IDC_ED_G1 + i));
            *ctx->result = s;
            ctx->ok = TRUE;
            EndDialog(hDlg, IDOK);
            return TRUE;
        }
        if (LOWORD(wp) == IDCANCEL)
        {
            ctx->ok = FALSE;
            EndDialog(hDlg, IDCANCEL);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL StudentEditDialog(HWND hOwner, const wchar_t* title, Student& out)
{
    EditCtx ctx;
    ctx.title = title;
    ctx.result = &out;
    ctx.ok = FALSE;
    DialogBoxParamW(GetModuleHandleW(nullptr), MAKEINTRESOURCEW(IDD_STUDENT),
                    hOwner, StudentDlgProc, reinterpret_cast<LPARAM>(&ctx));
    return ctx.ok;
}
