#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/async.h>


int main()
{
    //设置全局策略
    //遇到什么级别日志立即刷新
    spdlog::flush_on(spdlog::level::level_enum::debug);
    //多长时间刷新
    spdlog::flush_every(std::chrono::seconds(1));
    //只显示什么级别及以上的日志
    spdlog::set_level(spdlog::level::debug);
    spdlog::init_thread_pool(3072,1);

    std::shared_ptr<spdlog::logger> logger = spdlog::stdout_color_mt<spdlog::async_factory>("async-logger");
    logger->set_pattern("[%n][%H:%M:%S][%t][%-8l] %v");
    logger->trace("this is trace");
    logger->debug("this is trace");
    logger->info("this is info");
    logger->warn("this is trace");
    logger->error("this is trace");
    logger->critical("this is trace");
    
    return 0;
}