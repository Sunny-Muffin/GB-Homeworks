#include <iostream>
#include <algorithm>
#include <vector>
#include <random>
#include <string>
#include <numeric>

using namespace std;


// for Task 1 ====================================================================================================
template <typename T>
void insert_sorted(vector<T> &v, T n)
{
    typename vector <T>::iterator it;
    it = v.begin();
    while (it != v.end())
    {
        if (*it >= n)
        {
            v.insert(it, n);
            return;
        }
        ++it;
    }
    if (it == v.end())
    {
        v.push_back(n);
    }
}


int main()
{
    // Task 1 =====================================================================================================
    {
        cout << "Task 1 ===========================================================================================" << endl;
        
        vector <int> vec = { 1,2,3,4,6,7,8,9 };
        sort(vec.begin(), vec.end()); // на случай, если элементы не были отсортированы
        for (int i : vec) cout << i << " ";
        cout << endl << "Enter an integer to insert: ";
        int num;
        cin >> num;
        insert_sorted(vec, num);
        for (int i : vec) cout << i << " ";
        cout << endl;

        vector <float> vec2 = {1.23, 3.45, 5.67, 7.89};
        sort(vec2.begin(), vec2.end()); // на случай, если элементы не были отсортированы
        for (float i : vec2) cout << i << " ";
        cout << endl << "Enter a float to insert: ";
        float num2;
        cin >> num2;
        insert_sorted(vec2, num2);
        for (float i : vec2) cout << i << " ";
        cout << endl << endl;
    }

    // Task 2 =====================================================================================================
    {
        cout << "Task 2 ===========================================================================================" << endl;
        vector <double> a(100);
        vector <int> b(100);
        vector <double> out(100); // сюда запишем результат

        cout << "Analog signal:" << endl;
        generate(a.begin(), a.end(), []() {return (rand() / 1000.00); });
        copy(a.begin(), a.end(), ostream_iterator<double>(cout, "  "));
        cout << endl << endl;
        
        cout << "Digital signal:" << endl;
        copy(a.begin(), a.end(), b.begin());
        copy(b.begin(), b.end(), ostream_iterator<int>(cout, "  "));
        cout << endl << endl;
        
        transform(a.begin(), a.end(), a.begin(), [](double i) {return i * i; }); // возводим каждое число вектора А в квадрат
        transform(b.begin(), b.end(), b.begin(), [](int j) {return j * j; }); // возводим каждое число вектора B в квадрат
        transform(a.begin(), a.end(), b.begin(), out.begin(), minus<>{}); // вычитаем из вектора А вектор В и записываем в вектор out
        //copy(out.begin(), out.end(), ostream_iterator<double>(cout, "  "));
        //cout << endl << endl;

        auto sum = accumulate(out.begin(), out.end(), 0.); // складываем все элементы вектора
        cout << "sum = " << sum << endl;

        cout << endl;
    }

    return 0;
}


