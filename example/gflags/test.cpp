#include <gflags/gflags.h>
#include <iostream>

DEFINE_string(name,"杨玉萧","名字，比如yyx");
DEFINE_uint32(age,22,"年龄,例如32");
DEFINE_bool(sex,true,"性别：男true,女false");

int main(int argc,char* argv[])
{
    google::ParseCommandLineFlags(&argc,&argv,true);
    std::cout << FLAGS_name << std::endl;
    std::cout << FLAGS_age << std::endl;
    std::cout << FLAGS_sex << std::endl;
    return 0;
}