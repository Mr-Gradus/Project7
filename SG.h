#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include "main.pb.h"

namespace SG
{
    class IRepository
    {
    public:
        virtual void Open() = 0; // бинарная десериализация в файл
        virtual void Save() = 0; // бинарная сериализация в файл
        virtual ~IRepository() {};
    };

    class IMethods
    {
    public:
        virtual double GetAverageScore(const Study::FullName& name) = 0;
        virtual std::string GetAllInfo(const Study::FullName& name) = 0;
        virtual std::string GetAllInfo() = 0;
        virtual ~IMethods() {};
    };

    class StudentsGroup : public IRepository, public IMethods
    {
    private:
        std::vector<Study::Student> m_students;
    public:
        void add_student(const Study::Student& s)
        {
            m_students.push_back(s);
        }

        double GetAverageScore(const Study::FullName& name)
        {
            auto it = std::find_if(m_students.begin(), m_students.end(), [&](const Study::Student& s)
                {
                    return std::tie(s.fname().name(), s.fname().surname(), s.fname().patronymic()) ==
                        tie(name.name(), name.surname(), name.patronymic());
                });

            if (it == m_students.end())
            {
                return 0.0;
            }

            return it->avggrades();
        }

        std::string GetAllInfo(const Study::FullName& name)
        {
            auto it = std::find_if(m_students.begin(), m_students.end(), [&](const Study::Student& s)
                {
                    return std::tie(s.fname().name(), s.fname().surname(), s.fname().patronymic()) ==
                        tie(name.name(), name.surname(), name.patronymic());
                });

            if (it == m_students.end())
            {
                return {};
            }

            std::string temp = "Name: " + it->fname().name() + ' ' + it->fname().surname() + ' ' + it->fname().patronymic() + '\n';
            temp += "Grades: ";
            for (int i = 0; i < it->grades().size(); ++i)
            {
                temp += std::to_string(it->grades(i)) + "; ";
            }
            temp += '\n';
            temp += "Average grade: " + std::to_string(it->avggrades()) + "\n\n";

            return temp;
        }

        std::string GetAllInfo()
        {
            std::string temp;

            std::for_each(m_students.begin(), m_students.end(), [&](const Study::Student& s)
                {
                    temp += GetAllInfo(s.fname());
                });

            return temp;
        }

        void Save()
        {
            std::ofstream out("data.bin", std::ios_base::binary);
            auto size = m_students.size();
            out.write(reinterpret_cast<char*>(&size), sizeof(size));
            std::for_each(m_students.begin(), m_students.end(), [&](const Study::Student& s)
                {
                    s.SerializeToOstream(&out);
                });
            out.close();
        }

        void Open()
        {
            std::ifstream in("data.bin", std::ios_base::binary);
            size_t size = 0;
            in.read(reinterpret_cast<char*>(&size), sizeof(size));
            while (size--)
            {
                Study::Student s;
                s.ParseFromIstream(&in);
                m_students.push_back(std::move(s));
            }
            in.close();
        }

        virtual ~StudentsGroup() {};
    };
}