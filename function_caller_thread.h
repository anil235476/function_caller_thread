#ifndef __FUNCTION_CALLER_THREAD_H_
#define __FUNCTION_CALLER_THREAD_H_
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <chrono>

class function_caller_thread {
public:
	void start() {
		is_run_thread_ = true;
		run();
	}

	void add_function(std::function<void()>&& f) {

		{
			std::lock_guard<std::mutex> lck_{ queue_lck_ };
			queue_.push(move(f));

		}
		cv_.notify_all();
		
	}

	void exit_thread() {
		is_run_thread_ = false;
		//cv_.notify_all();
	}


private:
	void run() {
		while (is_run_thread_) {
			std::unique_lock<std::mutex> lck_{ queue_lck_ };
			if (queue_.empty()) {
				cv_.wait_for(lck_, std::chrono::seconds(3));
			}
			else {
				queue_.front()();
				queue_.pop();
			}
		}
	}

	std::queue < std::function<void()>> queue_;
	std::mutex queue_lck_;
	bool is_run_thread_{ false };
	std::condition_variable cv_;
};

#endif//__FUNCTION_CALLER_THREAD_H_
