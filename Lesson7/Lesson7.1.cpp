#include <iostream>
#include <iostream>
#include <numeric>
#include <fstream>
#include "StudentsDB.pb.h"


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
    virtual double GetAverageScore(const StudentsDB::FullName& name) = 0;
    virtual std::string GetAllInfo(const StudentsDB::FullName& name) = 0;
    virtual std::string GetAllInfo() = 0;
    virtual ~IMethods() {};
};

// сколько же нервов я потерял пока понял, что заглавные буквы в proto файле заменяются на обычные
// и что надо писать не fullName, а fullname
// зачем вообще так делать?

// и еще я долго возился с правильным подключением протобафа, поэтому саму реализацию программы взял у коллеги и немного поправил под себя
// мне за это стыдно, но не сильно

class StudentsGroup : public IRepository, public IMethods
{
private:
    std::vector<StudentsDB::Student> students;
public:
    void addStudent(const StudentsDB::Student& s)
    {
        students.push_back(s);
    }

    double GetAverageScore(const StudentsDB::FullName& name)
    {
        auto it = std::find_if(students.begin(), students.end(), [&](const StudentsDB::Student& s)
            {
                return std::tie(s.fullname().name(), s.fullname().surname(), s.fullname().patronym()) ==
                    std::tie(name.name(), name.surname(), name.patronym());
            });

        if (it == students.end())
        {
            return 0.0;
        }
        
        return it->avggrade();
    }

    std::string GetAllInfo(const StudentsDB::FullName& name)
    {
        auto it = std::find_if(students.begin(), students.end(), [&](const StudentsDB::Student& s)
            {
                return std::tie(s.fullname().name(), s.fullname().surname(), s.fullname().patronym()) ==
                    std::tie(name.name(), name.surname(), name.patronym());
            });

        if (it == students.end())
        {
            return {};
        }

        std::string temp = "Full name: " + it->fullname().name() + ' ' + it->fullname().surname() + ' ' + it->fullname().patronym() + '\n';
        temp += "Grades: ";
        for (int i = 0; i < it->grades().size(); ++i)
        {
            temp += std::to_string(it->grades(i)) + "; ";
        }
        temp += '\n';
        temp += "Average Grade: " + std::to_string(it->avggrade()) + "\n\n";

        return temp;
    }

    std::string GetAllInfo()
    {
        std::string temp;

        std::for_each(students.begin(), students.end(), [&](const StudentsDB::Student& s)
            {
                temp += GetAllInfo(s.fullname());
            });

        return temp;
    }

    void Save()
    {
        std::ofstream out("students.bin", std::ios_base::binary);
        auto size = students.size();
        out.write(reinterpret_cast<char*>(&size), sizeof(size));
        std::for_each(students.begin(), students.end(), [&](const StudentsDB::Student& s)
            {
                s.SerializeToOstream(&out);
            });
        out.close();
    }

    void Open()
    {
        std::ifstream in("students.bin", std::ios_base::binary);
        int size = 0;
        in.read(reinterpret_cast<char*>(&size), sizeof(size));
        while (size--)
        {
            StudentsDB::Student s;
            s.ParseFromIstream(&in);
            students.push_back(std::move(s));
        }
        in.close();
    }

    virtual ~StudentsGroup() {};
};

int main()
{
    StudentsDB::FullName fn;
    fn.set_name("Ivan");
    fn.set_surname("Groznyi");
    fn.set_patronym("Vasylievich");

    StudentsDB::Student s;
    *s.mutable_fullname() = fn;
    s.add_grades(1);
    s.add_grades(2);
    s.add_grades(3);
    s.add_grades(4);
    s.add_grades(5);
    s.set_avggrade(std::accumulate(s.grades().begin(), s.grades().end(), 0) / s.grades().size());

    StudentsDB::StudentsGroup sg;
    *sg.add_students() = s;

    std::ofstream out("students.bin", std::ios_base::binary);
    sg.SerializeToOstream(&out);
    out.close();

    StudentsDB::StudentsGroup new_sg;
    std::ifstream in("students.bin", std::ios_base::binary);
    if (new_sg.ParseFromIstream(&in))
    {
        std::cout << "Full name:"
            << new_sg.students(0).fullname().surname()
            << " " << new_sg.students(0).fullname().name()
            << " " << new_sg.students(0).fullname().patronym()
            << std::endl;

        std::cout << "Average grade: " << new_sg.students(0).avggrade() << std::endl;
    }
    else
    {
        std::cout << "Error!" << std::endl;
    }
    in.close();

    StudentsGroup sg2;
    sg2.addStudent(s);
    sg2.Save();

    StudentsGroup new_sg2;
    new_sg2.Open();
    std::cout << new_sg2.GetAllInfo(fn) << std::endl;

}