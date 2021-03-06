#include "stdafx.h"
#include "logger.h"
#include "log_sink.h"

namespace overdrive {
	namespace core {
		Logger::Logger(const std::string& filename):
			mActive(util::Active::create())
		{
			add(makeConsoleSink());
			add(makeFileSink(filename));
		}

		LogMessage Logger::operator()(
			eLogLevel level,
			const std::string& filename,
			int line
		) {
			return LogMessage(
				this,
				level,
				filename,
				line
			);
		}

		void Logger::add(const LogSink& sink) {
			using std::find;
			using std::begin;
			using std::end;

			auto it = find(begin(mSinks), end(mSinks), sink);

			if (it == end(mSinks))
				mSinks.push_back(std::move(sink));
			else {} // silently fail when the sink is already present
		}

		void Logger::remove(const LogSink& sink) {
			using std::find;
			using std::begin;
			using std::end;

			auto it = find(begin(mSinks), end(mSinks), sink);

			if (it != end(mSinks))
				mSinks.erase(it);
			else {} // silently fail when the sink is not contained
		}

		void Logger::removeAll() {
			mSinks.clear();
		}

		size_t Logger::getSinkCount() const {
			return mSinks.size();
		}

		void Logger::flush(const LogMessage* message) {
			std::string msg = message->mBuffer.str();

			
			// [NOTE] at this stage, single threaded is actually preferable
			// Singlethreaded version:
			for (auto&& sink: mSinks)
				sink.write(message->mMeta, msg);
			/*
			// Threadsafe background writer
			auto&& sinks = mSinks;
			auto&& meta = message->mMeta;

			mActive->send([=] {
				for (auto&& sink : sinks)
					sink.write(meta, msg);
			});
			*/
		}

		Logger& Logger::instance() {
			static Logger gLogger("overdrive.log");

			return gLogger;
		}

		//Logger gLogger("overdrive.log");
	}
}
