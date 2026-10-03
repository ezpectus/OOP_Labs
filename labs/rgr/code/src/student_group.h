#pragma once
// ============================================================
// StudentGroup.h — контейнер групи студентів (РГР, варіант 13)
// vector<Student> + операції: add/edit/remove, сортування,
// статистика групи.
// ============================================================
#include <vector>
#include <string>
#include "student.h"

class StudentGroup
{
private:
    std::vector<Student> students;

public:
    // --- базові операції ---
    void add(const Student& s)        { students.push_back(s); }
    bool removeAt(size_t i);
    bool updateAt(size_t i, const Student& s);
    void clear()                      { students.clear(); }

    size_t size() const               { return students.size(); }
    const Student* at(size_t i) const;
    Student* at(size_t i);

    // --- сортування ---
    void sortByAvg(bool ascending);   // за середнім балом
    void sortByName();                // за ПІБ (алфавіт)

    // --- статистика групи ---
    double groupAvg() const;          // середній бал усієї групи
    double successRate() const;       // % студентів без боргів
    int excellentCount() const;       // к-сть відмінників
    int debtorCount() const;          // к-сть боржників
};
