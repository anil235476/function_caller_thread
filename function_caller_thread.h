#ifndef __FUNCTION_CALLER_THREAD_H_
#define __FUNCTION_CALLER_THREAD_H_
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <atomic>
#include <optional>

class function_caller_thread {
	using Func = std::function<void()>;
public:
	void start() {
		is_run_thread_ = true;
		run();
	}

	void add_function(Func&& f) {

		{
			std::lock_guard<std::mutex> lck_{ queue_lck_ };
			queue_.push(move(f));

		}
		{
			std::lock_guard<std::mutex> lck_{ cv_lck_ };
			cv_.notify_all();
		}
		
		
	}

	void exit_thread() {
		is_run_thread_ = false;
		cv_.notify_all();
	}


private:
	void run() {
		while (is_run_thread_) {
			auto f = get_function();
			if (!f.has_value()) {
				//get event mutex
				std::unique_lock<std::mutex> lc_{ cv_lck_ };
				f = get_function();
				if (!f.has_value()) {
					cv_.wait(lc_);
					continue;
				}
					
			}

		  (*f)();
			
		}
	}

	std::optional<Func> get_function() {
		std::lock_guard<std::mutex> lck_{ queue_lck_ };
		if (queue_.empty())
			return std::optional<Func>{};
		std::optional<Func>  r{ queue_.front() };
		queue_.pop();
		return r;

	}

	std::queue<Func> queue_;
	std::mutex queue_lck_;
	std::atomic_bool is_run_thread_{ false };
	std::mutex cv_lck_;
	std::condition_variable cv_;
};

#endif//__FUNCTION_CALLER_THREAD_H_
