#include "msg_loop_libraray.h"
#include <future>
#include <iostream>
#include "../unit_test/catch.hpp"
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

vector<int> ids;
void handle_msg(message m) {
	cout << "handle message id = " << m.id_<<'\n';
	ids.push_back(m.id_);
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
	this_thread::sleep_for(3s);
	lib.stop(cleanup);
}

TEST_CASE("thread_msg_lib", "[thread]") {
	test_msg_loop_lib();
	REQUIRE(ids.size() == 4);
	REQUIRE(ids[0] == 2);
	REQUIRE(ids[1] == 3);
	REQUIRE(ids[3] == 9);
}