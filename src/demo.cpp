#include <string>
#include <fstream>
#include <iostream>
#include <vector>



int main(int argc,  char **argv)
{
    std::vector<int> vec{ 0,1,2,3,4 };
    for (auto it = std::rbegin(vec); it != std::rend(vec); ++it)
    {
        // Access element using dereference operator
        std::cout << *it << " ";
    }
}