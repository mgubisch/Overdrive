#ifndef OVERDRIVE_CORE_CHANNEL_H
#define OVERDRIVE_CORE_CHANNEL_H

#include "core/channelqueue.h"
#include "util/unique_resource.h"

namespace overdrive {
	namespace core {
		class Channel {
		public:
			template <typename tEvent, class tHandler>
			static void add(tHandler* handler);

			template <typename tEvent, class tHandler>
			static void remove(tHandler* handler);

			template <typename tEvent>
			static void broadcast(const tEvent& message);
		};

		// This is the contract that an object should fulfill for correct usage with a Channel
		// Note that this is not an actual required base class, even though it can be used as one
		template <typename tMessage>
		class MessageHandler {
		public:
			MessageHandler();
			virtual ~MessageHandler();

			virtual void operator()(const tMessage&) = 0;
		};
	}
}

#include "core/channel.inl"

#endif
