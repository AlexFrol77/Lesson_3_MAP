#include <iostream>
#include <thread>
#include <future>
#include <random>
#include <iterator>
#include <algorithm>
#include <vector>

void pow_vec(int& a) {
	a *= 2;
}

void forEach(std::vector<int>::iterator it_start, std::vector<int>::iterator it_end) {
	for_each(it_start, it_end, pow_vec);
}

void parallel_for_each(std::vector<int>::iterator it_start, 
	std::vector<int>::iterator it_end) {
	int temp = std::distance(it_start, it_end);
	if (temp < 5) {
		forEach(it_start, it_end);
	}
	else {
		forEach(it_start, it_end - temp / 2);
		std::async(std::launch::async, forEach, it_start + temp / 2, it_end);
	}
}

void print_vec(std::vector<int>& vec) {
	std::cout << "\n";
	for (const auto& it : vec) {
		std::cout << it << " ";
	}
	std::cout << "\n";
}

int main() {

	setlocale(LC_ALL, "Rus");

	std::mt19937 gen;
	std::uniform_int_distribution<int> dist(0, 100);
	auto gen_num([=]() mutable { return dist(gen); });
	std::vector<int> my_vec(10);
	generate(my_vec.begin(), my_vec.end(), gen_num);

	print_vec(my_vec);
	parallel_for_each(my_vec.begin(), my_vec.end());
	print_vec(my_vec);
	 
	return 0;
}