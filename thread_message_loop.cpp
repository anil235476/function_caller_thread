#include "thread_message_loop.h"
#include <future>
#include <iostream>
#include <thread>
#include <chrono>
struct test_struct {
	void message_call(int id, void* data) {
		std::cout << "id = " << id << "\n";
	}
};
void test() {
	thread_message_loop<int> message_loop;
	auto f = std::async(std::launch::async, [&message_loop](){
		test_struct a;
		message_loop.run(&a, &test_struct::message_call);
	}
	);
	message_loop.push_message(2, nullptr);
	std::this_thread::sleep_for(std::chrono::seconds(1));
	message_loop.stop();
}