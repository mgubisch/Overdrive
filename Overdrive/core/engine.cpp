#include "core/engine.h"
#include "core/channel.h"
#include "core/logger.h"
#include "core/system.h"
#include "app/application.h"

#include "opengl.h"

namespace {
	void glfwErrorCallback(int error, const char* description) {
		gLog.error() << "GLFW error (" << error << "): " << description;
	}
}

namespace overdrive {
	namespace core {
		Engine::Engine() {
			Channel::add<OnStop>(this);
		}

		void Engine::setApplication(app::Application* application) {
			setApplication(std::unique_ptr<app::Application>(application));
		}

		void Engine::setApplication(std::unique_ptr<app::Application>&& application) {
			mApplication = std::move(application);

			if (mApplication) {
				mApplication->mEngine = this;
				gLog << "Application set: " << mApplication->getName();
			}
		}

		void Engine::add(System* system) {
			add(std::unique_ptr<System>(system));
		}

		void Engine::add(std::unique_ptr<System>&& system) {
			auto it = mSystemLookup.find(system->getName());
			
			if (it == mSystemLookup.end()) {
				system->mEngine = this;

				mSystemLookup[system->getName()] = system.get();
				mSystems.emplace_back(std::move(system));
			}
			else
				throw std::runtime_error(std::string("Duplicate system added: ") + system->getName());
		}

		void Engine::remove(std::string systemName) {
			System* s = nullptr;
			
			auto it = mSystemLookup.find(systemName);

			if (it != mSystemLookup.end())
				s = it->second;

			if (s)
				remove(s);
			else
				gLog.error() << "Cannot remove system '" << systemName << "', not currently registered";
		}

		void Engine::remove(System* s) {
			// first scan the systemlist for the given system
			auto it = std::remove_if(
				mSystems.begin(), 
				mSystems.end(), 
				[s] (const SystemPtr& ptr) {
					return ptr.get() == s;
				}
			);

			if (it != mSystems.end()) {
				// the system was found, perform a clean shutdown and remove it from the active registry/list
				s->shutdown();

				mSystems.erase(it);

				for (auto jt = mSystemLookup.begin(); jt != mSystemLookup.end(); ++jt) {
					if (jt->second == s) {
						mSystemLookup.erase(jt);
						break;
					}
				}
			}
		}

		void Engine::run() {
			if (initializeDependencies()) {
				initializeSystems();
				mTaskProcessor.start();
				shutdownSystems();

				shutdownDependencies();
			}
		}
		
		void Engine::stop() {
			mTaskProcessor.stop();
		}

		void Engine::initializeSystems() {
			// first, consolidate all settings from all registered subsystems
			for (const auto& system: mSystems)
				mConfig.settings().add(system->mSettings);

			// try and load a config file
			mConfig.load("overdrive.cfg");

			// now perform per-system initialization
			for (auto& system : mSystems) {
				if (!system->initialize())
					gLog.error() << "Failed to initialize subsystem: " << system->getName();
			}

			// if an application was set, initialize it as well
			if (mApplication)
				if (!mApplication->initialize())
					gLog.error() << "Failed to initialize application: " << mApplication->getName();
		}

		void Engine::shutdownSystems() {
			if (mApplication)
				mApplication->shutdown();

			for (auto& system : mSystems)
				system->shutdown();

			mSystems.clear();
			mSystemLookup.clear();
		}

		bool Engine::initializeDependencies() {
			glfwSetErrorCallback(&glfwErrorCallback);

			if (!glfwInit())
				return false;

			return true;
		}

		void Engine::shutdownDependencies() {
			glfwTerminate();
		}

		void Engine::updateSystem(System* system, bool repeating, bool background) {
			assert(system);

			mTaskProcessor.addWork(
				[system] { system->update(); }, 
				repeating, 
				background
			);
		}

		void Engine::operator()(const OnStop&) {
			stop();
		}
	}
}