//
// Created by ypf on 2022/1/18.
//

#include <string>
#include <fstream>
#include <iostream>
#include <deque>

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

std::deque<int> removeZero(std::deque<int>& removed_queue){

    while (removed_queue.front() == 0){
        removed_queue.erase(removed_queue.begin());
    }

    if (removed_queue.empty()){
        removed_queue = {0};
    }
    return removed_queue;
}

void printOutResult(std::deque<int>& result_deque, int flag1, int flag2, int sign){

    if (flag1 == flag2 && flag1 == -1) {
        std::cout << "-" ;
    }

    if ( sign == -1) {
        std::cout << "-" ;
    }

    if (flag1 == -1 && flag2 == 0){
        std::cout << "-" ;
    }

    if (flag2 == -1 && flag1 == 0){
        std::cout << "-" ;
    }

    for (int i = 0; i < result_deque.size() ; ++i) {
        std::cout << result_deque[i];
    }
}


void doCalc(std::deque<std::string> lines) {

    std::deque<int> first_number;
    std::deque<int> second_number;
    std::deque<int>::reverse_iterator first_it;
    std::deque<int>::reverse_iterator second_it;

    int flag1 = 1;
    int flag2 = 1;
    int base = 10;
    int sign = 1;

    // get the first number
    std::cout << "The first number is : ";
    for (int i = 0; i < lines[0].size() ; ++i) {
        if (lines[0][0] == '-' ) {
            flag1 = -1;
            lines[0][0] = '0';
        }
        if (lines[0][0] == '+' ) {
            flag1 = 1;
            lines[0][0] = '0';
        }
        first_number.push_back(static_cast<int>(lines[0][i]) - 48);
    }

    removeZero(first_number);
    printOutResult(first_number, flag1, 0,0);
    std::cout << std::endl;

    // get the second number
    std::cout << "The second number is : ";
    for (int i = 0; i < lines[1].size() ; ++i) {
        if (lines[1][0] == '-') {
            flag2 = -1;
            lines[1][0] = '0';
        }
        if (lines[1][0] == '+') {
            flag2 = 1;
            lines[1][0] = '0';
        }
        second_number.push_back(static_cast<int>(lines[1][i]) - 48);
    }

    removeZero(second_number);
    printOutResult(second_number, 0, flag2, 0);
    std::cout << std::endl;

    // do the calculation
    // if the first number and the second number has the same sign (eg: ++ or --)
    std::deque<int> result(std::max(first_number.size(), second_number.size()) + 1, 0);
    std::deque<int>::reverse_iterator result_it;

    if (flag1 == flag2) {

        first_number.push_front(0);
        second_number.push_front(0);
        first_it = std::rbegin(first_number);
        second_it = std::rbegin(second_number);
        result_it = std::rbegin(result);


        for (int i = 0; i < std::min(first_number.size(), second_number.size()) ; ++i) {

            if (*result_it + *first_it + *second_it > base - 1) {
                *result_it = *result_it + *first_it + *second_it - base;
                *(result_it + 1) = *(result_it + 1) + 1;
                first_it++;
                second_it++;
                result_it++;
            } else{
                *result_it = *result_it + *first_it + *second_it;
                first_it++;
                second_it++;
                result_it++;
            }
        }
        removeZero(result);
    } else{
// depends on which number has the minus sign
        if (flag1 == -1 ) {
            if (first_number.size() > second_number.size() ) {
                result = first_number;
                result_it = std::rbegin(result);
                for (int i = 0; i < second_number.size(); ++i) {
                    if ( *first_it - *second_it < 0 ) {
                        *result_it = *result_it - *second_it + base;
                        *(result_it + 1) = *(result_it + 1) - 1;
                        first_it++;
                        second_it++;
                        result_it++;
                    }else{
                        *result_it = *result_it - *second_it;
                        first_it++;
                        second_it++;
                        result_it++;
                    }
                    sign = -1;
                }
            } else{
                result = second_number;
                result_it = std::rbegin(result);
                for (int i = 0; i < first_number.size(); ++i) {
                    if (*second_it - *first_it < 0) {
                        *result_it = *result_it - *first_it + base;
                        *(result_it + 1) = *(result_it + 1) - 1;
                        first_it++;
                        second_it++;
                        result_it++;
                    }else{
                        *result_it = *result_it - *first_it;
                        first_it++;
                        second_it++;
                        result_it++;
                    }
                }
                if ( *(result_it + 1) != 0) {
                    sign = -1 ;
                }
            }
        }
        if (flag2 == -1 ) {
            first_number.push_front(0);
            second_number.push_front(0);

            first_it = std::rbegin(first_number);
            second_it = std::rbegin(second_number);
            result_it = std::rbegin((result));

            for (int i = 0; i < std::min(first_number.size(), second_number.size()) ; ++i) {

                if (*result_it + *first_it - *second_it < 0) {
                    *result_it = *result_it + *first_it - *second_it + base;
                    *(result_it + 1) = *(result_it + 1) - 1;
                    first_it++;
                    second_it++;
                    result_it++;
                } else{
                    *result_it = *result_it + *first_it - *second_it;
                    first_it++;
                    second_it++;
                    result_it++;
                }
            }

            if (*result_it != 0) {

                result.assign(std::max(first_number.size(), second_number.size()), 0);
                first_it = std::rbegin(first_number);
                second_it = std::rbegin(second_number);
                result_it = std::rbegin((result));

                for (int i = 0; i < std::min(first_number.size(), second_number.size()) ; ++i) {
                    if (*result_it + *second_it - *first_it < 0) {
                        *result_it = *result_it + *second_it - *first_it + base;
                        *(result_it + 1) = *(result_it + 1) - 1;
                        first_it++;
                        second_it++;
                        result_it++;
                    } else{
                        *result_it = *result_it + *second_it - *first_it;
                        first_it++;
                        second_it++;
                        result_it++;
                    }
                }
                sign = -1;
            }
            removeZero(result);
        }
    }

    // print out the final result
    std::cout << "The result is :" << std::endl;
    printOutResult(result, flag1, flag2, sign);
}


int main(int argc, char **argv){

    std::string inputFile(argv[1]);
//    std::string method(argv[2]);
    std::deque<std::string> lines = readLines(inputFile);
    doCalc(lines);
    return 0;
}

