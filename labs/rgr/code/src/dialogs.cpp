// dialogs.cpp — реалізація діалогів (РГР)
#include "dialogs.h"
#include "resource.h"
#include <sstream>

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

            if (s.getName().empty())
            {
                MessageBoxW(hDlg, L"Введіть ПІБ студента!", L"Помилка", MB_ICONWARNING);
                return TRUE;
            }
            if (s.getBookNo().empty())
            {
                MessageBoxW(hDlg, L"Введіть номер залікової книжки!", L"Помилка", MB_ICONWARNING);
                return TRUE;
            }
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

// ---- статистика: текст формується тут, показ у діалозі ----
static std::wstring g_statsText;

static INT_PTR CALLBACK StatsDlgProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg)
    {
    case WM_INITDIALOG:
        SetDlgItemTextW(hDlg, IDC_STATS_TEXT, g_statsText.c_str());
        return TRUE;
    case WM_COMMAND:
        if (LOWORD(wp) == IDOK || LOWORD(wp) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wp));
            return TRUE;
        }
        break;
    }
    return FALSE;
}

void StatsDialog(HWND hOwner, const StudentGroup& group)
{
    if (group.size() == 0)
    {
        MessageBoxW(hOwner, L"Список порожній — статистики немає.",
                    L"Статистика", MB_OK | MB_ICONINFORMATION);
        return;
    }
    std::wstringstream ss;
    ss.setf(std::ios::fixed); ss.precision(2);
    ss << L"Студентів у групі:  " << group.size() << L"\r\n";
    ss << L"Середній бал групи:  " << group.groupAvg() << L"\r\n";
    ss << L"Успішність:  " << group.successRate() << L" %\r\n";
    ss << L"Відмінників (усі >= 90):  " << group.excellentCount() << L"\r\n";
    ss << L"Боржників (є < 60):  " << group.debtorCount();
    g_statsText = ss.str();

    DialogBoxParamW(GetModuleHandleW(nullptr), MAKEINTRESOURCEW(IDD_STATS),
                    hOwner, StatsDlgProc, 0);
}
