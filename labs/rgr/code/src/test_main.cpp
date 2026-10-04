// test_main.cpp — консольна перевірка класів Student/StudentGroup
// (робочий файл під час розробки моделі; UI ще не підключено)
#include <cstdio>
#include "student.h"
#include "student_group.h"

int main()
{
    StudentGroup g;

    Student a(L"Шевченко Іван", L"ІМ-051-01");
    int ga[] = {95, 92, 91, 90, 94};
    for (int i = 0; i < 5; i++) a.setGrade(i, ga[i]);
    g.add(a);

    Student b(L"Бондар Петро", L"ІМ-051-03");
    int gb[] = {55, 60, 48, 72, 51};
    for (int i = 0; i < 5; i++) b.setGrade(i, gb[i]);
    g.add(b);

    printf("students: %d\n", (int)g.size());
    printf("group avg: %.2f\n", g.groupAvg());
    printf("success: %.1f %%\n", g.successRate());
    printf("excellent: %d, debtors: %d\n",
           g.excellentCount(), g.debtorCount());

    // file roundtrip check
    if (g.saveToFile(L"test_group.csv"))
    {
        StudentGroup g2;
        if (g2.loadFromFile(L"test_group.csv"))
            printf("roundtrip: %d students\n", (int)g2.size());
    }
    return 0;
}
