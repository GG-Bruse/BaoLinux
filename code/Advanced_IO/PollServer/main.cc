#include "PollServer.hpp"
#include <memory>

int main()
{
    std::unique_ptr<PollServer> svr(new PollServer(100));
    svr->Start();
    return 0;
}