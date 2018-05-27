#ifndef __MSG_LOOP_LIB_H_
#define __MSG_LOOP_LIB_H_
#include <queue>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <optional>


template<class Message>
class thread_msg_lib{
private:
	std::queue<Message> message_queue_;
	std::mutex queue_lck_;

	std::condition_variable cv_;
	std::mutex cv_lck_;

	std::atomic_bool run_thread_{ true };
	

public:
	~thread_msg_lib() {
		stop();
		//todo: it should wait here for thread to stop running
	}
	


	template<typename Func >
	void run(Func run_condition) {
		while (run_condition() && run_thread_) {
			auto m = get_next_message();
			if (!m.has_value()) {
				//acquire event mutex
				std::unique_lock<std::mutex> l{ cv_lck_ };
				m = get_next_message();
				if (!m.has_value()) {
					cv_.wait(l);
					continue;
				}
			}

			handle_msg(*m);
		}
		clear_queue();
	}

	void run() {
		run([]() {return true; });
	}

	void push_message(Message msg) {
		//todo: if clean up started don't allow putting messaging
		put_message(msg);
		notify();
	}

private:
	std::optional<Message> get_next_message() {
		std::lock_guard<std::mutex> l{ queue_lck_ };
		if (message_queue_.empty())
			return {};
		std::optional<Message> r = message_queue_.front();
		message_queue_.pop();
		return r;
	}

	void put_message(Message n) {
		std::lock_guard<std::mutex> l{ queue_lck_ };
		message_queue_.push(n);
	}

	void notify() {
		std::lock_guard<std::mutex> l{ cv_lck_ };
		cv_.notify_all();
	}

	void clear_queue() {
		//clean up messaging
		//came out of loop clean the message
		for (auto msg = get_next_message(); msg.has_value();) {
			cleanup(*msg);
		}
	}

	void stop() {
		run_thread_ = false;
		notify();
	}
};

void test_msg_loop_lib();
#endif//__MSG_LOOP_LIB_H_