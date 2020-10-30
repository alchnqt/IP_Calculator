#include "ipv_4.h"
int main(int argc, char** argv)
{
    using std::string;
    using std::cout;
    using std::cin;
    using std::endl;
    using Lab8_KSIS::ipv_4;
    using Lab8_KSIS::_address_;
    if (argc <= 2)
    {
        std::cerr << "Unexpectable error!\n";
    }
    try
    {
        auto ip1 = ipv_4<unsigned>(argv[1], argv[2]);
        std::cout << ip1 << endl;
    }
    catch (Lab8_KSIS::ipv4_excpetion e)
    {
        std::cerr << e.get_message();
    }
    return 0;
}
