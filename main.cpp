
#include <functional>
#include <iostream>
#include "function_caller_thread.h"
#include "thread_message_loop.h"
#include "msg_loop_libraray.h"
using namespace std;

struct Foo {
	Foo(int num) : num_(num) {/* cout << "Foo constructor\n";*/ };
	Foo(const Foo&) = delete;
	~Foo() {/* cout << "destructor\n";*/ }
	void print_add(int i) const { std::cout << num_ + i << '\n'; }
	void print_add_no_const(int i) { std::cout << this->num_ + i << '\n'; }
	int num_;
};

void print_num(int i)
{
	std::cout << i << '\n';
}

struct PrintNum {
	void operator()(int i) const
	{
		std::cout << i << '\n';
	}
};



int main()
{

	function_caller_thread a;
	thread t{ &function_caller_thread::start, &a };

	{
		function<void()> f = bind([](int a) {
			cout << " a = " << a << endl;
		}, 23);
		a.add_function(move(f));
	}

	{
		Foo f{ 100 };
		function<void()> func = bind(&Foo::print_add_no_const, &f, 100);
		a.add_function(move(func));

	}

	{
		function<void()> f = bind(print_num, 1213);
		a.add_function(move(f));
	}

	{
		function<void()> f = bind(PrintNum{}, 5678);
		a.add_function(move(f));
	}

	cout << "come out\n";

	this_thread::sleep_for(3s);
	a.exit_thread();
	t.join();

	cout << "test start for thread  message looop\n";
	test();
	cout << "test 2 starts\n";
	test2();

	cout << "\ntest start for library \n";
		test_msg_loop_lib();
    return 0;
}

