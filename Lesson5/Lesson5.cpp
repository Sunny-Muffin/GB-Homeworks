#include <iostream>
#include <set>
#include <map>
#include <vector>
#include <string>

using namespace std;

// For task 1 ======================================================================================================

template<typename T>
void f(T start, T finish)
{
	for (T i = start; i != finish; ++i)
	{
		T j;
		for (j = start; j != i; ++j)
		{
			if (*i == *j)
			{
				break;
			}
		}
		if (i == j)
			cout << *i << " ";
	}
	cout << endl;

	/*
	// как-то можно поместить вместо string шаблонную переменную? Было бы намного удобнее и быстрее
	// или как-то можно используя начало и конец вектора (не зная его тип) скопировать его в set?
	set <string> temp;
	copy(start, finish, inserter(temp, temp.end()));
	copy(temp.begin(), temp.end(), ostream_iterator<string>(cout, " "));
	cout << endl;
	*/
}

int main()
{
	// Task 1 ======================================================================================================
	{
		cout << "Task 1 ============================================================================================" << endl;

		std::vector<string> v{"aaa", "aab", "aab", "aac", "aaa", "aab", "aac", "aaa"};
		std::vector<string>::const_iterator i = v.begin();
		std::vector<string>::const_iterator j = v.end();
		copy(i, j, ostream_iterator<string>(cout, " "));
		cout << endl;
		f(i, j);

		std::vector<int> ve{ 1, 2, 3, 2, 1, 4, 3, 1 };
		std::vector<int>::const_iterator k = ve.begin();
		std::vector<int>::const_iterator l = ve.end();
		copy(k, l, ostream_iterator<int>(cout, " "));
		cout << endl;
		f(k, l);

		cout << endl;
	}


	// Task 2 ======================================================================================================
	{
		cout << "Task 2 ============================================================================================" << endl;
		string text;
		cout << "Enter some sentences: " << endl;
		getline(cin, text);
		
		multiset <string> sentences;
		string temp;

		cout << endl;
		for (auto i = text.begin(); i != text.end(); ++i)
		{
			
			if (*i == '.' || *i == '?' || *i == '!')
			{
				temp += *i;
				if (i != (text.end() - 1))
					temp += *(++i); // убираем пробелы в начале следующего предложения для красоты вывода
				sentences.insert(temp);
				temp.clear();
			}
			else 
			{
				temp += *i;
			}
				
		}
		// copy(sentences.begin(), sentences.end(), ostream_iterator<string>(cout, " "));
		// я не понял как через COPY вывести текст с переносом строк, поэтому:
		multiset<string>::iterator it = sentences.begin();
		for (it; it != sentences.end(); ++it)
		{
			cout << *it << endl;
		}
		cout << endl;
	}
	return 0;
}


