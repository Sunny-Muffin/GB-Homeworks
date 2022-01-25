#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <algorithm> 
#include <fstream> 
#include <chrono>
#include "Timer.h" 
#include <locale.h>


using namespace std;


// For task 1 ==================================================
template <typename T>
void Swap(T *&x, T *&y);

// For task 2 ==================================================
template <typename T>
void SortPointers(vector <T*> &v);

// For task 3 ==================================================
void CountVowels1(string_view text);
void CountVowels2(string_view text);
void CountVowels3(string_view text);
void CountVowels4(string_view text);


int main()
{
    setlocale(LC_ALL, "Russian");
    
    // Task 1 ==========================================================================================
    {
        cout << "Task 1 ================================================================================" << endl;
        int a = 0;
        int b = 999;
        int* pA = &a;
        int* pB = &b;
        cout << "a = " << a << "  pA = " << pA << " b = " << b << " pB = " << pB << endl;
        cout << "a = " << a << "  *pA = " << *pA << " b = " << b << " *pB = " << *pB << endl;
        cout << "================================================================================" << endl;
        
        Swap(pA, pB);
        cout << "a = " << a << "  pA = " << pA << " b = " << b << " pB = " << pB << endl;
        cout << "a = " << a << "  *pA = " << *pA << " b = " << b << " *pB = " << *pB << endl;
        cout << endl;
    }

    // Task 2 ==========================================================================================
    {
        cout << "Task 2 ================================================================================" << endl;
        vector <int*> pArray;
        int arr[10];

        for (int i = 0; i < 10; ++i)
        {
            if (i % 2 == 0)
                arr[i] = i * 2;
            else
                arr[i] = i - 2;
        }
        for (int i = 0; i < 10; ++i)
        {
            cout << arr[i] << " ";
        }
        
        for (int i = 0; i < 10; ++i)
        {
            pArray.push_back(&arr[i]);
        }
        cout << endl;
        for (int i = 0; i < 10; ++i)
        {
            cout << pArray[i] << " ";
        }
        cout << endl;
        
        SortPointers(pArray);
        for (int i = 0; i < 10; ++i)
        {
            cout << *pArray[i] << " ";
        }
        
        cout << endl << endl;
    }

    // Task 3 ==========================================================================================
    {
        cout << "Task 3 ================================================================================" << endl;
        // не понимаю как это работает, взял из методички
        ifstream file("Bellum et pacem.txt");
        file.seekg(0, ios::end);
        size_t size = file.tellg();
        file.seekg(0);
        string s(size, ' ');
        file.read(&s[0], size);

        CountVowels1(s);
        CountVowels2(s);
        CountVowels3(s);
        CountVowels4(s);
        cout << endl;
    }
    return 0;
}


// For Task 1 ============================================================================================
// в задании сказано, что ф-я Swap "принимает два указателя", именно поэтому сделал указатели на ссылки =)
// если это как-то решается через указатели - буду рад узнать
template <typename T>
void Swap(T *&x, T *&y)
{
    T *pTemp = x;
    x = y;
    y = pTemp;
}

// For Task 2 ============================================================================================
template <typename T>
void SortPointers(vector <T*> &v)
{
    sort(v.begin(), v.end(), [](const auto& v1, const auto& v2) { return *v1 < *v2; });
}

// For Task 3 ============================================================================================
string_view vowels = "ауоиэыяюеё";

bool CompareCharsFind(char c)
{
    return (vowels.find(c) != string::npos);
}

bool CompareCharsFor(char c)
{
    for (int i = 0; i < 10; ++i)
    {
        if (vowels[i] == c)
            return true;
    }
    return false;
}

// Method 1 count_if и find 
void CountVowels1(string_view text)
{
    Timer timer("1) Using count_if и find ");
    int vowCount = count_if(text.begin(), text.end(), CompareCharsFind);
    
    //cout << "Number of vowels in text is: " << vowCount << endl;
    cout << "Количество гласных в тексте: " << vowCount << endl;
    timer.print();
}

// Method 2 count_if и for 
void CountVowels2(string_view text)
{
    Timer timer("2) Using count_if и for  ");
    int vowCount = count_if(text.begin(), text.end(), CompareCharsFor);

    cout << "Количество гласных в тексте: " << vowCount << endl;
    timer.print();
}

// Method 3 цикл for и find 
void CountVowels3(string_view text)
{
    Timer timer("3) Using for and find    ");
    int vowCount = 0;
    for (int i = 0; i < text.length(); ++i)
    {
        if (CompareCharsFind(text[i]))
            vowCount++;
    }

    cout << "Количество гласных в тексте: " << vowCount << endl;
    timer.print();
}

// Method 3 цикл for и for 
void CountVowels4(string_view text)
{
    Timer timer("4) Using for and for     ");
    int vowCount = 0;
    for (int i = 0; i < text.length(); ++i)
    {
        if (CompareCharsFor(text[i]))
            vowCount++;
    }

    cout << "Количество гласных в тексте: " << vowCount << endl;
    timer.print();
}