#include <iostream>
#include <unistd.h>
int main()
{
    std::cout << "I am running!!!" << std::endl;
    sleep(3);
    int err = 10 / 0;//除0错误
    return 0;
}