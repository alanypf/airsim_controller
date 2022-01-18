//
// Created by ypf on 2022/1/18.
//

#include <string>
#include <fstream>
#include <iostream>
#include <vector>

std::vector<std::string> readLines(const std::string& input_file){

    std::ifstream source;
    source.open(input_file);
    std::vector<std::string> lines;
    std::string single_line;

    while (std::getline(source, single_line)){
        lines.push_back(single_line);
    }
    return lines;
}

void doCalc(std::vector<std::string> lines) {

    std::vector<int> first_number;
    std::vector<int> second_number;
    std::vector<int> result;
    std::vector<int>::reverse_iterator first_it;
    std::vector<int>::reverse_iterator second_it;
    std::vector<int>::reverse_iterator result_it;

    int flag1 = 1;
    int flag2 = 1;
    int base = 10;

    first_number.reserve(lines[0].size());
    second_number.reserve(lines[1].size());

    // get the first number

    std::cout << "The first number is : ";
    for (int i = 0; i < lines[0].size() ; ++i) {
//        if (lines[0][0] == '-') {
//            flag1 = -1;
//        }
        first_number.push_back(static_cast<int>(lines[0][i]) - 48);
        std::cout << first_number[i];
    }

    while (first_number.front() == 0) {
        first_number.erase(first_number.begin());
    }

    std::cout << std::endl;
    first_it = std::rbegin(first_number);


    // get the second number
    std::cout << "The second number is : ";
    for (int i = 0; i < lines[1].size() ; ++i) {
//        if (lines[1][0] == '-') {
//            flag2 = -1;
//        }
        second_number.push_back(static_cast<int>(lines[1][i]) - 48);
        std::cout << second_number[i] ;
    }

    while (second_number.front() == 0) {
        second_number.erase(second_number.begin());
    }
    std::cout << std::endl;
    second_it = std::rbegin(second_number);
    // do the calculation

    if (first_number.size() > second_number.size() ) {
        result = first_number;
        result_it = std::rbegin(result);
        for (int i = 0; i < second_number.size(); ++i) {
            if ( *second_it + *first_it > base - 1 ) {
                *result_it = *result_it + *second_it - base;
                *(result_it + 1) = *(result_it + 1) + 1;
                first_it++;
                second_it++;
                result_it++;
            }else{
                *result_it = *result_it + *second_it;
                first_it++;
                second_it++;
                result_it++;
            }
        }
    } else{
        result = second_number;
        result_it = std::rbegin(result);
        for (int i = 0; i < first_number.size(); ++i) {
            if (*second_it + *first_it > base - 1) {
                *result_it = *result_it + *first_it - base;
                *(result_it + 1) = *(result_it + 1) + 1;
                first_it++;
                second_it++;
                result_it++;
            }else{
                *result_it = *result_it + *first_it;
                first_it++;
                second_it++;
                result_it++;
            }
        }
    }

    std::cout << "The result is :" << std::endl;

    for (int i = 0; i < result.size() ; ++i) {
        std::cout << result[i];
    }

}


int main(int argc, char **argv){

    std::string inputFile(argv[1]);
//    std::string method(argv[2]);
    std::vector<std::string> lines = readLines(inputFile);
    doCalc(lines);
    return 0;
}

