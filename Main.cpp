#include <iostream>
#include <numeric>
#include "Students.h"
#include "SG.h"

int main()
{
Study::FullName fn;
fn.set_name("Dmitriy");
fn.set_surname("Rybin");
fn.set_patronymic("Valerievich");

Study::Student s;
*s.mutable_fname() = fn;
s.add_grades(5);
s.add_grades(2);
s.add_grades(4);
s.add_grades(3);
s.add_grades(4);
s.add_grades(3);
s.add_grades(4);
s.add_grades(3);
s.set_avggrades(std::accumulate(s.grades().begin(), s.grades().end(), 0) / s.grades().size());

SG::StudentsGroup sg;
sg.add_student(s);
sg.Save();

SG::StudentsGroup new_sg;
new_sg.Open();
std::cout << new_sg.GetAllInfo(fn) << std::endl;


return 0;
}