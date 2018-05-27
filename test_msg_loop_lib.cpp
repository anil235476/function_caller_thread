#include "msg_loop_libraray.h"
#include <future>
#include <iostream>
using namespace std;
struct message {
	int id_;
	int* ptr{ nullptr };
};

bool should_run() {
	cout << "should run called\n";
	return true;
}

void cleanup(message m) {
	cout << "clean up message id = " << m.id_;
	if (m.ptr)
		delete m.ptr;
}

void handle_msg(message m) {
	cout << "handle message id = " << m.id_<<'\n';
	if (m.ptr)
		delete m.ptr;
}

void test_msg_loop_lib() {

	thread_msg_lib<message> lib;
	auto f = async(launch::async, [&lib]() {
		lib.run();
	});
	lib.push_message({ 2, nullptr });
	lib.push_message({ 3, nullptr });
	lib.push_message({ 5, nullptr });
	lib.push_message({ 9, nullptr });
}