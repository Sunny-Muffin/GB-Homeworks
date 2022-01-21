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

using namespace std;


// For task 1 ============================================================================
struct Person
{
    string name;
    string surname;
    optional <string> patronym; // отчество (optional)
};

bool operator<(const Person& p1, const Person& p2)
{
    if (p1.patronym && p2.patronym) // только когда у обоих есть отчества иммет смысл их (отчества) сравнить
        return tie(p1.name, p1.surname, p1.patronym.value()) < tie(p2.name, p2.surname, p2.patronym.value());
    else
        return tie(p1.name, p1.surname) < tie(p2.name, p2.surname);
}
bool operator==(const Person& p1, const Person& p2)
{
    if (p1.patronym && p2.patronym)
        return tie(p1.name, p1.surname, p1.patronym.value()) == tie(p2.name, p2.surname, p2.patronym.value());
    else
        return tie(p1.name, p1.surname) == tie(p2.name, p2.surname);
 }
ostream& operator<< (ostream& out, const Person& p)
{
    out << p.surname << "   " << p.name << "   " << p.patronym.value_or("") << endl;
    return out;
}

// For task 2 ============================================================================
struct PhoneNumber
{
    int countryCode;
    int cityCode;
    string number;
    optional <int> addNumber; // additional number (optional)
};

bool operator<(const PhoneNumber& n1, const PhoneNumber& n2)
{
    if (n1.addNumber && n2.addNumber) // только когда у обоих есть доб. номера иммет смысл их (доб. номера) сравнить
        return tie(n1.countryCode, n1.cityCode, n1.number, n1.addNumber.value()) < tie(n2.countryCode, n2.cityCode, n2.number, n1.addNumber.value());
    else
        return tie(n1.countryCode, n1.cityCode, n1.number) < tie(n2.countryCode, n2.cityCode, n2.number);
    
    //return tie(n1.countryCode, n1.cityCode, n1.number, n1.addNumber.value()) < tie(n2.countryCode, n2.cityCode, n2.number, n1.addNumber.value());
}
bool operator==(const PhoneNumber& n1, const PhoneNumber& n2)
{
    if (n1.addNumber && n2.addNumber) 
        return tie(n1.countryCode, n1.cityCode, n1.number, n1.addNumber.value()) == tie(n2.countryCode, n2.cityCode, n2.number, n1.addNumber.value());
    else
        return tie(n1.countryCode, n1.cityCode, n1.number) == tie(n2.countryCode, n2.cityCode, n2.number);
    
    
    //return tie(n1.countryCode, n1.cityCode, n1.number, n1.addNumber.value()) == tie(n2.countryCode, n2.cityCode, n2.number, n1.addNumber.value());
}
ostream& operator<< (ostream& out, const PhoneNumber& n)
{
    if (n.addNumber) 
        out << "+" << n.countryCode << "(" << n.cityCode << ")" << n.number << " " << n.addNumber.value() << endl;
    else
        out << "+" << n.countryCode << "(" << n.cityCode << ")" << n.number << " " << endl;
    
    
    //out << "+" << n.countryCode << "(" << n.cityCode << ")" << n.number << " " << n.addNumber.value_or(stoi("")) << endl; // исправить!
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
            cout << "Opened file!" << endl;
            string fName; // "f" for "file"
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
                fPatronym = "";

            /*
            optional <string> patr;
            if (fPatronym != "-")
                patr = fPatronym;
            
            optional <int> add;
            if (fAddNumber != "-")
                add = stoi(fAddNumber);

            phonePairs.emplace_back(Person{ fSurname, fName, patr.value() }, PhoneNumber{ fCountryCode, fCityCode, fNumber, add.value() });
            */
            phonePairs.emplace_back(Person{ fName, fSurname, fPatronym }, PhoneNumber{ fCountryCode, fCityCode, fNumber, stoi(fAddNumber) });
        }
        file.close();
        }
    }

    // Вот так он не принимает
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

    // А вот так (через дружественную) принимает, почему? я же в теле класса, какая ему разница?
    friend ostream& operator<< (ostream& out, const PhoneBook& book)
    {
        for (int i{ 0 }; i < book.phonePairs.size(); ++i)
        {
            out << book.phonePairs[i].first << "   " << book.phonePairs[i].second << endl;
        }
    return out;
    }

    bool compPer(vector<pair <Person, PhoneNumber>>& p1, vector<pair <Person, PhoneNumber>>& p2)
    {
        if (p1[0].first == p2[0].first)
        {
            if (p1[1].first == p2[1].first)
            {
                return p1[2].first < p2[2].first;
            }
            else
                return (p1[1].first < p2[1].first);
        }
        else
            return (p1[0].first < p2[0].first);

    }

    void SortByName()
    {
        sort(phonePairs.begin(), phonePairs.end());
        /*
        cout << "Before sorting: " << endl;
        for (const auto& phonePair : phonePairs)
        {
            cout << phonePair.first << "   " << phonePair.second << endl;
        }
        cout << endl;
        sort(phonePairs.begin(), phonePairs.end());
        cout << "After sorting: " << endl;
        for (const auto& phonePair : phonePairs)
        {
            cout << phonePair.first << "   " << phonePair.second << endl;
        }
        cout << "--------------------------------------------------------------" << endl;
        */

        cout << endl;
    }
    
    
    void SortByPhone()
    {
        sort(phonePairs.begin(), phonePairs.end());
        /*
        cout << "Before sorting: " << endl;
        for (const auto& phonePair : phonePairs)
        {
            cout << phonePair.first << "   " << phonePair.second << endl;
        }
        cout << endl;
        sort(phonePairs.begin(), phonePairs.end());
        cout << "After sorting: " << endl;
        for (const auto& phonePair : phonePairs)
        {
            cout << phonePair.first << "   " << phonePair.second << endl;
        }
        cout << "--------------------------------------------------------------" << endl;
        */
        cout << endl;
    }

    tuple < string, PhoneNumber> GetPhoneNumber(const string& searchSurname)
    {
        PhoneNumber foundPair; // если найдем человека, то номер на время поместим его сюда
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
    // Task 1 ============================================================================
    {
        cout << "Task 1 ======================================================================" << endl;
        Person person1 = { "Name", "Surname"};
        cout << person1 << endl;
        Person person2 = { "Name", "Surname", "Patronym" };
        cout << person2 << endl;
        if (person1 == person2)
            cout << "Works" << endl;
        cout << (person1 < person2) << endl;
        cout << endl;
    }

    // Task 2 ============================================================================
    {
        cout << "Task 2 ======================================================================" << endl;
        PhoneNumber num1 = { 7, 123, "456-78-90", 77};
        cout << num1 << endl;
        PhoneNumber num2 = { 7, 123, "456-78-89"};
        cout << num2 << endl;
        if (num1 < num2)
            cout << "num1 goes BEFOR num2" << endl;
        else
            cout << "num1 goes AFTER num2" << endl;
        cout << (num1 < num2) << endl;
        cout << endl;
    }

    // Task 3 ============================================================================
    {
        cout << "Task 3 ======================================================================" << endl;
        
        ifstream file("PhoneBook.txt"); // путь к файлу PhoneBook.txt
        PhoneBook book(file);
        cout << book;

        cout << "------SortByPhone-------" << endl;
        book.SortByPhone();
        cout << book;

        cout << "------SortByName--------" << endl;
        book.SortByName();
        cout << book;

        cout << "-----GetPhoneNumber-----" << endl;
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

        cout << "----ChangePhoneNumber----" << endl;
        book.ChangePhoneNumber(Person{ "Kotov", "Vasilii", "Eliseevich" }, PhoneNumber{ 7, 123, "15344458", nullopt });
        book.ChangePhoneNumber(Person{ "Mironova", "Margarita", "Vladimirovna" }, PhoneNumber{ 16, 465, "9155448", 13 });
        cout << book;
        

        cout << endl;
    }


    return 0;
}

