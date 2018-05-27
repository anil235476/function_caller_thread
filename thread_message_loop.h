#ifndef __THREAD_MESSAGE_LOOP_H_
#define __THREAD_MESSAGE_LOOP_H_
#include <queue>
#include <mutex>
#include <atomic>
#include <optional>
#include <condition_variable>
#include <functional>
#include "msg_loop_libraray.h"



template<class MessageID>
class thread_message_loop {
private:
	
	struct _node {
		MessageID id_;
		void* data_;//todo: type safeity issue. check if language support now anything.
	};
	
	thread_msg_lib<_node> lib_;

public:
	~thread_message_loop() { stop(); }

	template<typename Fun, typename ...Args>
	void run(Fun&& f, Args... args) {
		
		if constexpr(sizeof...(args) > 0)
			lib_.run([f, args...](auto m){
			std::invoke(f, args..., m.id_, m.data_);
			});
		else
			lib_.run([f](auto m){
			std::invoke(f, m.id_, m.data_);
		});
		
	}

	void push_message(MessageID id, void* data) {
		lib_.push_message(_node{ id, data });
	}

	void stop() {
		lib_.stop([](_node /*a*/) {//cleanup things
			std::cout << "\n cleaning up of object required memory leak taking place\n";
		});
	}
private:
};


void test();
void test2();

#endif//__THREAD_MESSAGE_LOOP_H_