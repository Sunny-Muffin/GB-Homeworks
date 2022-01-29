#include <iostream>
#include <vector>
#include <list>
#include <array>
#include <iterator>

using namespace std;

// For Task 1 ================================================================================
void ListPushBack(list<float>& ls);

// For Task 2 ================================================================================
class Matrix
{
private:
	int m_rows;
	int m_columns;
	vector <vector<int>> mat;

public:
	Matrix(int rows, int columns)
	{
		if (rows < 2 || columns < 2) 
		{
			cerr << "Seems that it is not a matrix" << endl;
		}
		else
		{
			m_rows = rows;
			m_columns = columns;
		}
	}

	void GenerateMatrix()
	{
		for (int i = 0; i < m_columns; ++i)
		{
			vector<int> temp;
			for (int j = 0; j < m_rows; ++j)
			{
				temp.push_back(i * 3 + j);
			}
			mat.push_back(temp);
		}
	}

	void PrintMatrix()
	{
		for (int i = 0; i < m_columns; ++i)
		{
			for (int j = 0; j < m_rows; ++j)
			{
				cout << mat[i][j] << " ";
			}
			cout << endl;
		}
	}

	float Countdeterminant3x3(vector<vector<int>> vec)
	{
		float det =
			vec[0][0] * vec[1][1] * vec[2][2] -
			vec[0][0] * vec[1][2] * vec[2][1] -
			vec[0][1] * vec[1][0] * vec[2][2] +
			vec[0][1] * vec[1][2] * vec[2][0] +
			vec[0][2] * vec[1][0] * vec[2][1] -
			vec[0][2] * vec[1][1] * vec[2][0];
		return det;
	}

	float Countdeterminant()
	{
		if (m_rows == 2 && m_columns == 2)
		{
			return (mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0]);
		}
		else if (m_rows == 3 && m_columns == 3)
		{
			return Countdeterminant3x3(mat);
		}
		else
		{
			// я пытался придумать реализацию решения матрицы 4*4 (и выше) через матрицы 3*3, но не додумался, как их автоматизировать
			// то есть можно расписать решение вручную, но мне кажется это не рационально
			cout << "Sorry, I can't calcculate determonan for matrix 4x4 and larger";
			return 0;
		}
	}
};

// For Task 3 ================================================================================
template <typename T>
class MyArray
{
private:
	T* arr;
	int m_n;
public:
	class MyIterator;
	MyArray(int n = 1) : m_n(n) { arr = new T(n); }
	T& operator[] (const int& n)
	{
		if (n > 0 && n < m_n)
			return arr[n];
		return arr[0];
	}
	MyIterator begin() { return arr; }
	MyIterator end() { return arr + m_n; }

	friend ostream& operator<< (ostream& s, const MyArray<T>& n)
	{
		s << arr[n];
		return s;
	}

	class MyIterator
	{
		T* cur;
	public:
		MyIterator(T* first) : cur(first) {}
		T& operator+ (int n) { return *(cur + n); }
		T& operator- (int n) { return *(cur - n); }

		T& operator++ (int n) { return *cur++; }
		T& operator-- (int n) { return *cur--; }
		T& operator++ () { return *++cur; }
		T& operator-- () { return *--cur; }

		bool operator!= (const MyIterator& it) { return cur != it.cur; }
		bool operator== (const MyIterator& it) { return cur == it.cur; }
		T& operator* () { return *cur; }
	};
};

int main()
{
	// Task 1 ===========================================================================
	{
		cout << "Task 1 =====================================================================" << endl;
		list<float> list = {1.5, 2.3, 6.4, 10.11, 3.67};
	
		cout << "Before pushing" << endl;
		for (float i : list)
			cout << i << " ";
		cout << endl;
	
		cout << "After pushing" << endl;
		ListPushBack(list);
		for(float j : list)
			cout << j << " ";
		cout << endl;
	}


	// Task 2 ===========================================================================
	{
		cout << "Task 2 =====================================================================" << endl;

		Matrix matrix(3, 3);
		matrix.GenerateMatrix();
		matrix.PrintMatrix();
		float det = matrix.Countdeterminant();
		cout << "determinant = " << det << endl;

		cout << endl;
	}
	
	// Task 3 ===========================================================================
	{
		cout << "Task 3 =====================================================================" << endl;
		MyArray <int> arr(3);
		arr[0] = 1;
		arr[1] = 2;
		arr[2] = 3;

		auto it = MyArray<int>::MyIterator(arr.begin());

		cout << "2: " << it + 2 << endl;

		while (it != arr.end())
		{
			cout << *it << endl;
			it++;
		}
		cout << endl;
	}
	return 0;
}

void ListPushBack(list<float> &ls)
{
	double avg = 0;
	double sum = 0;
	
	for (float i : ls)
		sum += i;

	cout << "sum = " << sum << endl;
	avg = sum / 10.0;
	cout << "avg = " << avg << endl;
	
	ls.push_back(avg);
}

