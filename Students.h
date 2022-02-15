#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include "main.pb.h"


void Grades()
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

    Study::StudentsGroup sg;
    *sg.add_student() = s;

    std::ofstream out("student.bin", std::ios_base::binary);
    sg.SerializeToOstream(&out);
    out.close();

    Study::StudentsGroup new_sg;
    std::ifstream in("student.bin", std::ios_base::binary);
    if (new_sg.ParseFromIstream(&in))
    {
        std::cout << new_sg.student(0).fname().surname() << std::endl;
        std::cout << new_sg.student(0).avggrades() << std::endl;
    }
    else
    {
        std::cout << "Error!" << std::endl;
    }
    in.close();
}