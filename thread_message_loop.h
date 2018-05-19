#ifndef __THREAD_MESSAGE_LOOP_H_
#define __THREAD_MESSAGE_LOOP_H_
#include <queue>
#include <mutex>
#include <atomic>
#include <optional>
#include <condition_variable>

template<class MessageID>
class thread_message_loop {
private:
	struct _node {
		MessageID id_;
		void* data_;//todo: type safeity issue. check if language support now anything.
	};
	
	std::queue<_node> message_queue_;
	std::mutex queue_lck_;

	std::condition_variable cv_;
	std::mutex cv_lck_;

	std::atomic_bool run_message_{ true };

public:
	~thread_message_loop() { stop(); }
	template<typename Caller, typename Fun>
	void run(Caller* caller, Fun f) {
		while (run_message_) {
		   auto m =	get_next_message();
		   if (!m.has_value()) {
			   //acquire event mutex
			   std::unique_lock<std::mutex> l{ cv_lck_ };
			   m = get_next_message();
			   if (!m.has_value()) {
				   cv_.wait(l);
				   continue;
			   }
		   }
		   (caller->*f)(m->id_, m->data_);
		}
	}

	void push_message(MessageID id, void* data) {
		put_message(_node{ id, data });
		notify();
	}

	void stop() {
		run_message_ = false;
		notify();
	}
private:
	std::optional<_node> get_next_message() {
		std::lock_guard<std::mutex> l{ queue_lck_ };
		if (message_queue_.empty())
			return {};
		std::optional<_node> r = message_queue_.front();
		message_queue_.pop();
		return r;
	}

	void put_message(_node n) {
		std::lock_guard<std::mutex> l{ queue_lck_ };
		message_queue_.push(n);
	}

	void notify() {
		std::lock_guard<std::mutex> l{ cv_lck_ };
		cv_.notify_all();
	}
};


void test();

#endif//__THREAD_MESSAGE_LOOP_H_