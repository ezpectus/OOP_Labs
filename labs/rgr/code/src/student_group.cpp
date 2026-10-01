// StudentGroup.cpp — реалізація контейнера групи
#include "student_group.h"

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
