// StudentGroup.cpp — реалізація контейнера групи
#include "student_group.h"
#include <algorithm>
#include <fstream>
#include <locale>
#include <codecvt>

bool StudentGroup::removeAt(size_t i)
{
    if (i >= students.size()) return false;
    students.erase(students.begin() + i);
    return true;
}

bool StudentGroup::updateAt(size_t i, const Student& s)
{
    if (i >= students.size()) return false;
    students[i] = s;
    return true;
}

const Student* StudentGroup::at(size_t i) const
{
    if (i >= students.size()) return nullptr;
    return &students[i];
}

Student* StudentGroup::at(size_t i)
{
    if (i >= students.size()) return nullptr;
    return &students[i];
}

void StudentGroup::sortByAvg(bool ascending)
{
    std::sort(students.begin(), students.end(),
        [ascending](const Student& a, const Student& b)
        {
            return ascending ? a.avg() < b.avg() : a.avg() > b.avg();
        });
}

void StudentGroup::sortByName()
{
    std::sort(students.begin(), students.end(),
        [](const Student& a, const Student& b)
        {
            return a.getName() < b.getName();
        });
}

double StudentGroup::groupAvg() const
{
    if (students.empty()) return 0.0;
    double sum = 0.0;
    for (const auto& s : students) sum += s.avg();
    return sum / students.size();
}

double StudentGroup::successRate() const
{
    if (students.empty()) return 0.0;
    int ok = 0;
    for (const auto& s : students)
        if (!s.hasDebt()) ok++;
    return 100.0 * ok / students.size();
}

int StudentGroup::excellentCount() const
{
    int n = 0;
    for (const auto& s : students)
        if (s.isExcellent()) n++;
    return n;
}

int StudentGroup::debtorCount() const
{
    int n = 0;
    for (const auto& s : students)
        if (s.hasDebt()) n++;
    return n;
}

// CSV-формат: ПІБ;ЗАЛІКОВКА;g1;g2;g3;g4;g5  (UTF-8, BOM)
bool StudentGroup::saveToFile(const std::wstring& path) const
{
    std::wofstream f(path.c_str(), std::ios::out | std::ios::trunc);
    if (!f.is_open()) return false;
    f.imbue(std::locale(f.getloc(), new std::codecvt_utf8_utf16<wchar_t>));
    f << L'\xFEFF';
    f << L"Name;BookNo;G1;G2;G3;G4;G5\n";
    for (const auto& s : students)
        f << s.toCSV() << L'\n';
    return f.good();   // disk full / write error -> false -> error box in UI
}

bool StudentGroup::loadFromFile(const std::wstring& path)
{
    std::wifstream f(path.c_str());
    if (!f.is_open()) return false;
    f.imbue(std::locale(f.getloc(), new std::codecvt_utf8_utf16<wchar_t>));

    // parse into a temp list first: a corrupt file must not wipe
    // the group that is already loaded in memory
    std::vector<Student> tmp;
    std::wstring line;
    bool first = true;
    int dataLines = 0;
    while (std::getline(f, line))
    {
        if (first)   // заголовок (і BOM) пропускаємо
        {
            first = false;
            if (!line.empty() && line[0] == L'\xFEFF') line.erase(0, 1);
            if (line.find(L"Name") == 0) continue;
        }
        if (line.empty()) continue;
        dataLines++;
        Student s;
        if (s.fromCSV(line)) tmp.push_back(s);
    }
    if (dataLines > 0 && tmp.empty()) return false;   // файл не наш формат
    students.swap(tmp);
    return true;
}
