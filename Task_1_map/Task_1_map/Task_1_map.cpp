#include <iostream>
#include <thread>
#include <future>
#include <vector>
#include <random>

void find_min(std::vector<int>& vec_, int& start, std::promise<int> prom) {
	int it_temp = start,
		temp = vec_[start];
	for (int i = start; i < vec_.size(); i++) {
		if (temp > vec_[i]) {
			it_temp = i;
			temp = vec_[i];
		}
	}
	prom.set_value(it_temp);
}

int swap_vec(std::vector<int>& vec_source, int& iter, int& iter_temp) {
	int temp_data = vec_source[iter_temp];
	vec_source[iter_temp] = vec_source[iter];
	vec_source[iter] = temp_data;
	iter++;
	return iter;
}

void sort(std::vector<int>& vec_source) {
	int iter = 0,
		iter_temp = 0;
	do {
		std::promise<int> promise_res;
		std::future<int> future_res = promise_res.get_future();
		std::future<void> fut_it = std::async(std::launch::async, find_min, std::ref(vec_source),
									std::ref(iter), std::move(promise_res));
		future_res.wait();
		iter_temp = future_res.get();
		iter = swap_vec(vec_source, iter, iter_temp);
	} while (iter != (vec_source.size() - 1));
}

void print_vec(std::vector<int>& vec_) {
	for (const auto& st : vec_) {
		std::cout << st << " ";
	}
	std::cout << "\n";
}

int main() {

	setlocale(LC_ALL, "Rus");

	std::vector<int> vec_source(100);
	std::mt19937 gen;
	std::uniform_int_distribution<int> dis(0, 500);
	auto rand_num([=]() mutable { return dis(gen); });
	std::generate(vec_source.begin(), vec_source.end(), rand_num);

	std::cout << "\t Не сортированный вектор: " << "\n";
	print_vec(vec_source);
	sort(vec_source);
	std::cout << "\t Cортированный вектор: " << "\n";
	print_vec(vec_source);

	return 0;
}