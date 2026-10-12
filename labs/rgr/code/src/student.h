#pragma once
// ============================================================
// Student.h — студент групи (РГР, варіант 13)
// Дані: ПІБ, номер залікової книжки, 5 оцінок (0–100).
// Похідні: середній бал, боржник (<60), відмінник (усі >=90).
// ============================================================
#include <string>

class Student
{
public:
    static const int GRADE_COUNT = 5;

private:
    std::wstring name;      // ПІБ
    std::wstring bookNo;    // номер залікової книжки
    int grades[GRADE_COUNT];// оцінки 0..100

public:
    Student();
    Student(const std::wstring& name, const std::wstring& bookNo);

    // --- доступ ---
    const std::wstring& getName()   const { return name; }
    const std::wstring& getBookNo() const { return bookNo; }
    int getGrade(int i) const;
    const int* getGrades() const { return grades; }

    void setName(const std::wstring& v)   { name = v; }
    void setBookNo(const std::wstring& v) { bookNo = v; }
    void setGrade(int i, int v);

    // --- розрахунки успішності ---
    double avg()         const;   // середній бал
    bool   hasDebt()     const;   // є хоча б одна оцінка < 60
    bool   isExcellent() const;   // усі оцінки >= 90

    // --- файл (CSV, поля розділені ;) ---
    std::wstring toCSV() const;                    // рядок для запису
    bool fromCSV(const std::wstring& line);        // парсинг, false = поганий рядок
};
