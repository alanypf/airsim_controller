#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <deque>


int getMode(std::string mode){
    int mode_int = std::stoi(mode);
    return mode_int;
}

std::deque<std::string> readLines(const std::string& input_file){

    std::ifstream source;
    source.open(input_file);
    std::deque<std::string> lines;
    std::string single_line;

    while (std::getline(source, single_line)){
        lines.push_back(single_line);
    }
    return lines;
}


int main(int argc,  char **argv)
{
//    std::vector<int> vec{ 0,1,2,3,4 };
//    for (auto it = std::rbegin(vec); it != std::rend(vec); ++it)
//    {
//        // Access element using dereference operator
//        std::cout << *it << " ";
//    }

    std::string inputFile(argv[1]);
    std::deque<std::string> lines = readLines(inputFile);

    int number = getMode(lines[2]);

    std::cout << number << std::endl;
    std::cout << static_cast<char>(number + 'A') << std::endl;

    return 0;
}