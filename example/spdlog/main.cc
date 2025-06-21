#include "logger.hpp"

#include <gflags/gflags.h>

DEFINE_bool(run_mode,false,"运行模式:true-发布  false-调试");
DEFINE_string(file_name,"","发布模式下日志输出目标文件");
DEFINE_int32(level,0,"发布模式下日志最低输出等级");


int main(int argc,char* argv[])
{
    google::ParseCommandLineFlags(&argc,&argv,true);
    init_logger(FLAGS_run_mode,FLAGS_file_name,static_cast<spdlog::level::level_enum>(FLAGS_level));
    LOG_TRACE("hehe");
    LOG_DEBUG("hehe");
    LOG_INFO("hehe");
    LOG_WARN("hehe");
    LOG_ERROR("hehe");
    LOG_FATAL("hehe");
    return 0;
}