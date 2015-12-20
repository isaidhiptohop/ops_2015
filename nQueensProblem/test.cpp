#include <iostream>
#include <random>
#include <chrono>

int main ()
{
    unsigned int seed = 
        std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator (seed); 
    std::uniform_int_distribution<int> distribution (0, 9);

    int i = 20;
    while (i--) std::cout << distribution (generator) << std::endl;
}
