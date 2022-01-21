#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <tuple>
#include <utility>
#include <optional>
#include <cassert>
#include <algorithm>
#include <any>
#include <iomanip>

using namespace std;

// Проблемы, которые я так и не решил:
// 1) в текстовом документе пришлось идти на "допущения" и там, где нет отчества и доб. номера ставить прочерки "-"
// 2) не разобрался как работает optional, в примерах всё просто, а у меня отладчик местами ругается на .value()
// 2.1) как следствие предыдущего пункта - устроил порнографию с if-else, чем не горжусь

// Задание было очень интересное и очень сложное, спасибо, выздаравливайте =)

// For task 1 ============================================================================
struct Person
{
    string surname;
    string name;
    optional <string> patronym;
};

bool operator<(const Person& p1, const Person& p2)
{
    if (p1.patronym && p2.patronym) // только когда у обоих есть отчества иммет смысл их (отчества) сравнить
        return tie(p1.surname, p1.name, p1.patronym.value()) < tie(p2.surname, p2.name, p2.patronym.value());
    else
        return tie(p1.surname, p1.name) < tie(p2.surname, p2.name);
}
bool operator==(const Person& p1, const Person& p2)
{
    if (p1.patronym && p2.patronym)
        return tie(p1.surname, p1.name, p1.patronym.value()) == tie(p2.surname, p2.name, p2.patronym.value());
    else
        return tie(p1.surname, p1.name) == tie(p2.surname, p2.name);
 }
ostream& operator<< (ostream& out, const Person& p)
{
    if (p.patronym)
        out << setw(14) << p.surname << setw(12) << p.name << setw(17) << p.patronym.value();
    else
        out << setw(14) << p.surname << setw(12) << p.name << setw(21);
    return out;
}

// For task 2 ============================================================================
struct PhoneNumber
{
    int countryCode;
    int cityCode;
    string number;
    optional <int> addNumber;
};

bool operator<(const PhoneNumber& n1, const PhoneNumber& n2)
{
    if (n1.addNumber && n2.addNumber) // только когда у обоих есть доб. номера иммет смысл их (доб. номера) сравнить
        return tie(n1.countryCode, n1.cityCode, n1.number, n1.addNumber.value()) < tie(n2.countryCode, n2.cityCode, n2.number, n1.addNumber.value());
    else
        return tie(n1.countryCode, n1.cityCode, n1.number) < tie(n2.countryCode, n2.cityCode, n2.number);
}
bool operator==(const PhoneNumber& n1, const PhoneNumber& n2)
{
    if (n1.addNumber && n2.addNumber) 
        return tie(n1.countryCode, n1.cityCode, n1.number, n1.addNumber.value()) == tie(n2.countryCode, n2.cityCode, n2.number, n1.addNumber.value());
    else
        return tie(n1.countryCode, n1.cityCode, n1.number) == tie(n2.countryCode, n2.cityCode, n2.number);
 }
ostream& operator<< (ostream& out, const PhoneNumber& n)
{
    if (n.addNumber) 
        out << "+" << n.countryCode << "(" << n.cityCode << ")" << n.number << " " << n.addNumber.value();
    else
        out << "+" << n.countryCode << "(" << n.cityCode << ")" << n.number << " ";
    return out;
}

// For task 3 ============================================================================
class PhoneBook
{
protected:
    vector<pair <Person, PhoneNumber>> phonePairs;
public:
    PhoneBook(ifstream& file)
    {
        //assert(file);
        if (file.is_open())
        {
            string fName;
            string fSurname;
            string fPatronym;

            int fCountryCode;
            int fCityCode;
            string fNumber;
            string fAddNumber;
        
        while (!file.eof())
        {
            file >> fSurname >> fName >> fPatronym >> fCountryCode >> fCityCode >> fNumber >> fAddNumber;
            
            if (fPatronym == "-")
            {
                if (fAddNumber == "-")
                    phonePairs.emplace_back(Person{ fSurname, fName }, PhoneNumber{ fCountryCode, fCityCode, fNumber });
                else
                    phonePairs.emplace_back(Person{ fSurname, fName }, PhoneNumber{ fCountryCode, fCityCode, fNumber, stoi(fAddNumber) });
            }
            else
            {
                if (fAddNumber == "-")
                    phonePairs.emplace_back(Person{ fSurname, fName, fPatronym }, PhoneNumber{ fCountryCode, fCityCode, fNumber });
                else
                    phonePairs.emplace_back(Person{ fSurname, fName, fPatronym }, PhoneNumber{ fCountryCode, fCityCode, fNumber, stoi(fAddNumber) });
            }
            
            // так, как вы писали в телеграме не сработало, отладчик ругается на .value()
            // скорее всего я допустил ошибку, но не понимаю какую
            /*
            optional <string> patr;
            if (fPatronym != "-")
                patr = fPatronym;
            
            optional <int> add;
            if (fAddNumber != "-")
                add = stoi(fAddNumber);

            phonePairs.emplace_back(Person{ fSurname, fName, patr.value() }, PhoneNumber{ fCountryCode, fCityCode, fNumber, add.value() });
            */
            
        }
        file.close();
        }
    }

    // Вот так компилятор не принимает:
    /*
    ostream& operator<< (ostream& out)
    {
        for (const auto& phonePair : phonePairs)
        {
            out << phonePair.first << "   " << phonePair.second << endl;
        }
        return out;
    }
    */

    // А вот так (через дружественную) принимает, почему? 
    friend ostream& operator<< (ostream& out, const PhoneBook& book)
    {
        for (int i{ 0 }; i < book.phonePairs.size(); ++i)
        {
            out << book.phonePairs[i].first << "    " << book.phonePairs[i].second << endl;
        }
    return out;
    }

    void SortByName()
    {
        sort(phonePairs.begin(), phonePairs.end(), 
            [](pair <Person, PhoneNumber> p1, pair <Person, PhoneNumber> p2)
            {return p1.first < p2.first; });
        // я так понимаю расписывать отдельно сортирвки по фамилии и, если совпадут, по имени и т.д. смысла нет
        // судя по всему ф-я sort прекрасно справляется с этим сама
    }
    
    void SortByPhone()
    {
        sort(phonePairs.begin(), phonePairs.end(),
            [](pair <Person, PhoneNumber> p1, pair <Person, PhoneNumber> p2)
            {return p1.second < p2.second; });
    }

    tuple < string, PhoneNumber> GetPhoneNumber(const string& searchSurname)
    {
        PhoneNumber foundPair; // если найдем человека, то номер на время поместим сюда
        int pairCount = 0;
        for (const auto& phonePair : phonePairs)
        {
            if (phonePair.first.surname == searchSurname)
            {
                pairCount++;
                foundPair = phonePair.second;
            }
        }

        if (pairCount == 0)
            return { "not found", foundPair };
        else if (pairCount == 1)
            return { "", foundPair };
        else
            return { "found more than 1", foundPair };
    }
    
    void ChangePhoneNumber(const Person& person, const PhoneNumber& number)
    {
        for (auto& phonePair : phonePairs)
        {
            if (phonePair.first == person)
            {
                phonePair.second = number;
            }
        }
    }
};

int main()
{
    cout << "Task 3 ======================================================================" << endl;

    ifstream file("PhoneBook.txt"); // путь к файлу PhoneBook.txt
    PhoneBook book(file);
    cout << book;

    cout << endl << "------SortByPhone-------" << endl;
    book.SortByPhone();
    cout << book;

    cout << endl << "------SortByName--------" << endl;
    book.SortByName();
    cout << book;

    cout << endl << "-----GetPhoneNumber-----" << endl;
    // лямбда функция, которая принимает фамилию и выводит номер телефона этого	человека, либо строку с ошибкой
    auto print_phone_number = [&book](const string& surname) {
        cout << surname << "\t";
        auto answer = book.GetPhoneNumber(surname);
        if (get<0>(answer).empty())
            cout << get<1>(answer);
        else
            cout << get<0>(answer);
        cout << endl;
    };

    // вызовы лямбды
    print_phone_number("Ivanov");
    print_phone_number("Petrov");

    cout << endl << "----ChangePhoneNumber----" << endl;
    book.ChangePhoneNumber(Person{ "Kotov", "Vasilii", "Eliseevich" }, PhoneNumber{ 7, 123, "15344458", nullopt });
    book.ChangePhoneNumber(Person{ "Mironova", "Margarita", "Vladimirovna" }, PhoneNumber{ 16, 465, "9155448", 13 });
    cout << book;

    cout << endl;
    
    return 0;
}

