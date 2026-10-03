// StudentGroup.cpp — реалізація контейнера групи
#include "student_group.h"
#include <algorithm>

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
