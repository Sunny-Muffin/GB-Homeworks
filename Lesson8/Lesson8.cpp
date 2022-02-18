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

#include <gtest/gtest.h>

using namespace std; // первое задание я делал с подключением std, поэтому переделывать сейчас не рационально

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
        }
        file.close();
        }
    }

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
    }
    
    void SortByPhone()
    {
        sort(phonePairs.begin(), phonePairs.end(),
            [](pair <Person, PhoneNumber> p1, pair <Person, PhoneNumber> p2)
            {return p1.second < p2.second; });
    }

    tuple < string, PhoneNumber> GetPhoneNumber(const string& searchSurname)
    {
        PhoneNumber foundPair; 
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


class TestPhoneBook : public testing::Test {
protected:
    PhoneBook* book;
    void SetUp() override {
        ifstream file("PhoneBook.txt");
        book = new PhoneBook(file);
    }
    void TearDown() override {
        delete book;
    }
};

TEST_F(TestPhoneBook, number_found) {
    string surname = "Ivanov";
    auto result = book->GetPhoneNumber(surname);
    ASSERT_EQ(get<0>(result), "");
}

TEST_F(TestPhoneBook, number_not_found) {
    string surname = "Petrov";
    auto result = book->GetPhoneNumber(surname);
    ASSERT_EQ(get<0>(result), "not found");
}

TEST_F(TestPhoneBook, several_numbers_found) {
    string surname = "Solovev";
    auto result = book->GetPhoneNumber(surname);
    ASSERT_EQ(get<0>(result), "found more than 1");
}

TEST_F(TestPhoneBook, change_number) {
    string surname = "Kotov";
    auto result = book->GetPhoneNumber(surname);
    auto num = get<1>(result);
    ASSERT_EQ(num.countryCode, 7);
    ASSERT_EQ(num.cityCode, 367);
    ASSERT_EQ(num.number, "7508888");
    ASSERT_EQ(num.addNumber, nullopt);
        
    book->ChangePhoneNumber(Person{ "Kotov", "Vasilii", "Eliseevich" }, PhoneNumber{ 7, 123, "15344458", nullopt });

    result = book->GetPhoneNumber(surname);
    num = get<1>(result);
    ASSERT_EQ(num.countryCode, 7);
    ASSERT_EQ(num.cityCode, 123);
    ASSERT_EQ(num.number, "15344458");
    ASSERT_EQ(num.addNumber, nullopt);
}

// не понял как проверить сортировку, нам ведь для этого понадобится отсортированная телефонная книга, чтобы сравнить значения
// но если она отсортирована, то значит мы уже применили какой-то метод
// а тогда смысл тестирования пропадает, т.к. мы будем сравнивать правильность сортировки одного метода

int main()
{
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}

