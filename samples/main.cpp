#include "Limbogger.h"

int main() {
	using namespace Limbogger;

    Logger::Get().SetLogLevel(LogLevel::Debug);

    LOG_INFO("Application started");
    std::this_thread::sleep_for(std::chrono::seconds(1));
    LOG_DEBUG("Debugging details here");
    LOG_WARN("A potential problem detected");
    LOG_ERROR("An error occurred!");
    LOG_FATAL("Fatal issue, shutting down");

	return 0;
}