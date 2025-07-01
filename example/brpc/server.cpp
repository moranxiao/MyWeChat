#include <brpc/server.h>

#include "main.pb.h"


class EchoServiceImp : public EchoService{

}

int main()
{
    EchoServiceImp echo();
    
    return 0;
}