#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <atomic>
#include <syncstream>
#include <vector>
#include <cstdlib>

std::mutex m;
using namespace std::chrono_literals;


// For Task 1 ========================================================================================================
void Pcout(std::string s)
{
	std::lock_guard lg(m);
	std::cout << s << std::endl;
}

std::osyncstream pcout(std::cout); // оказывается в C++20 есть готовая потокобезопасная оболочка для cout
		// надеюсь это не будет считаться читерством =)
void Pcout2(std::string s2)
{
	std::lock_guard lg(m);
	pcout << s2 << std::endl;
}


// For Task 2 ========================================================================================================
void FindSimpleNum(int count)
{
	std::lock_guard lg(m);
	int temp = count; // нам нужна эта переменная только для сохранения начального значения, чтобы потом вывсести его в консоль
	int progress = 0;
	count -= 2; // на этом моменте мы уже отбросили 2 простых числа (1 не считается)
	int i = 3; // начинаем с 3 и пойдем только по нечетным (+=2), нам четные не нужны
	while (count != 0)
	{
		i += 2; // сразу прибавляем 2 для правильности выполнения алгоритма
		// а вот тут мы запускаем цикл проверки каждого последующего числа на простоту
		bool isSimple = true;
		for (int j = 3; j <= ceil(sqrt(abs(i))); j += 2)
		{
			if (i % j == 0)
			{
				isSimple = false;
				continue; // если мы попали на "сложное" число, то можно сразу идти дальше по циклу и не тратить время
			}
		}
		if (isSimple)
		{
			//std::cout << "Found simple number: " << i << std::endl;
			count--; // а если мы всё таки не нашли делитель у числа, значит оно простое и наш счетчик можно уменьшить
			//std::cout << "Counter is: " << count << std::endl;
		}
		int progTemp = progress;
		progress = 100 - (count * 100 / temp);
		if (progTemp != progress)
			std::cout << "progress: " << progress << "%" << std::endl;
	}
	std::cout << "The " << temp << " simple number is : " << i << std::endl;
}

void ImitationOfWork()
{
	std::lock_guard lg(m);
	std::this_thread::sleep_for(1s);
	std::cout << "doing smthing..." << std::endl;
	std::this_thread::sleep_for(1s);
}


// For Task 3 ========================================================================================================
void Master(std::vector <int> &v, int randNum)
{
	std::lock_guard lg(m);
	std::this_thread::sleep_for(1s);
	v.push_back(randNum);
}

void Thief(std::vector <int> &v)
{
	std::lock_guard lg(m);
	std::this_thread::sleep_for(0.5s);
	auto maxNum = std::max_element(v.begin(), v.end());
	std::cout << "Thief steals number: " << *maxNum << std::endl;
	v.erase(maxNum);
	std::cout << "Size of vectr: " << v.size() << std::endl;
}


int main()
{
	// Task 1 ========================================================================================================
	{
		std::cout << "Task 1 =========================================================================================" << std::endl;
		std::thread t1{ Pcout, "I was called first! (1)" };
		std::thread t2{ Pcout, "I was called second! (2)" };
		std::thread t3{ Pcout, "I was called third! (3)" };
		std::thread t4{ Pcout, "I was called fourth! (4)" };
		std::thread t5{ Pcout, "I was called fifth! (5)" };
		t1.join();
		t2.join();
		t3.join();
		t4.join();
		t5.join();

		std::cout << std::endl;

		// потоки ниже выводятся в консоль позже всего, но всегда в правильном порядке интересненько
		std::thread th1{ Pcout2, "I was called first! (11)" };
		std::thread th2{ Pcout2, "I was called second! (22)" };
		std::thread th3{ Pcout2, "I was called third! (33)" };
		std::thread th4{ Pcout2, "I was called fourth! (44)" };
		std::thread th5{ Pcout2, "I was called fifth! (55)" };
		th1.join();
		th2.join();
		th3.join();
		th4.join();
		th5.join();

		std::cout << std::endl;
	}
	
	// Task 2 ========================================================================================================
	{
		std::cout << "Task 2 =========================================================================================" << std::endl;

		int num;
		std::cout << "Which simple number you want to know? i =  ";
		std::cin >> num;
		int count = num;
		// сначала отбросим числа 1...3, чтобы избежать дальнейших ошибок
		if (num == 1 )
			std::cout << "1 is NOT a simple number" << std::endl;
		else if (num == 2 || num == 3)
			std::cout << "The " << (num-1) << " simple number is: " << num << std::endl;
		else
		{
			std::thread thr1 (FindSimpleNum, num);
			std::thread thr2 (ImitationOfWork);
			thr1.join();
			thr2.join();
		}
		std::cout << std::endl;
	}

	// Task 3 ========================================================================================================
	// в этом задании вор не успевает забирать число, только иногда получается уменьшить вектор, 
	// но он быстро возвращается к исходному количеству
	{
		std::cout << "Task 3 =========================================================================================" << std::endl;
		int cycles = 0;
		std::vector<int> v{ 3,5,1,2,7,5,34,67,32,45 };
		while (cycles < 20) // цикл НЕ бесконечный, потому что в конце выполняется первое задание =D 
		{
			int rnum = rand() % 100;
			std::cout << "Master puts number " << rnum << std::endl;
			std::thread thr1(Master, ref(v), rnum);
			std::thread thr2(Thief, ref(v));
			thr1.join();
			thr2.join();
			++cycles;
		}
		std::cout << std::endl;
	}

	return 0;
}


