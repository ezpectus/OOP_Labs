// Student.cpp — реалізація класу Student
#include "student.h"

Student::Student()
{
    for (int i = 0; i < GRADE_COUNT; i++) grades[i] = 0;
}

Student::Student(const std::wstring& n, const std::wstring& b)
    : name(n), bookNo(b)
{
    for (int i = 0; i < GRADE_COUNT; i++) grades[i] = 0;
}

int Student::getGrade(int i) const
{
    if (i < 0 || i >= GRADE_COUNT) return 0;
    return grades[i];
}

void Student::setGrade(int i, int v)
{
    if (i < 0 || i >= GRADE_COUNT) return;
    grades[i] = v;
}

double Student::avg() const
{
    double sum = 0.0;
    for (int i = 0; i < GRADE_COUNT; i++) sum += grades[i];
    return sum / GRADE_COUNT;
}

bool Student::hasDebt() const
{
    for (int i = 0; i < GRADE_COUNT; i++)
        if (grades[i] < 60) return true;
    return false;
}

bool Student::isExcellent() const
{
    for (int i = 0; i < GRADE_COUNT; i++)
        if (grades[i] < 90) return false;
    return true;
}
