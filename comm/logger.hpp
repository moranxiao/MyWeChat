#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

// 两种模式：true-发布模式  false-调试模式

namespace common
{

    std::shared_ptr<spdlog::logger> g_default_logger;
    void init_logger(bool mode, const std::string &file, spdlog::level::level_enum level)
    {
        spdlog::init_thread_pool(8192, 1);
        if (mode == true)
        {
            // 发布模式根据level参数选择显示哪些等级
            // 输出打印到文件
            g_default_logger = spdlog::basic_logger_mt<spdlog::async_factory>("default_logger", file);
            g_default_logger->set_level(level);
            g_default_logger->flush_on(level);
        }
        else
        {
            // 调试模式显示所有等级
            // 输出为打印到终端
            g_default_logger = spdlog::stdout_color_mt<spdlog::async_factory>("default_logger");
            g_default_logger->set_level(spdlog::level::level_enum::trace);
            g_default_logger->flush_on(spdlog::level::level_enum::trace);
        }
        g_default_logger->set_pattern("[%n][%H:%M:%S][%t][%-8l]%v");
    }

#define LOG_TRACE(format, ...) g_default_logger->trace(std::string("[{}:{}]") + format, __FILE__, __LINE__, ##__VA_ARGS__)
#define LOG_DEBUG(format, ...) g_default_logger->debug(std::string("[{}:{}]") + format, __FILE__, __LINE__, ##__VA_ARGS__)
#define LOG_INFO(format, ...) g_default_logger->info(std::string("[{}:{}]") + format, __FILE__, __LINE__, ##__VA_ARGS__)
#define LOG_ERROR(format, ...) g_default_logger->error(std::string("[{}:{}]") + format, __FILE__, __LINE__, ##__VA_ARGS__)
#define LOG_WARN(format, ...) g_default_logger->warn(std::string("[{}:{}]") + format, __FILE__, __LINE__, ##__VA_ARGS__)
#define LOG_FATAL(format, ...) g_default_logger->critical(std::string("[{}:{}]") + format, __FILE__, __LINE__, ##__VA_ARGS__)

}