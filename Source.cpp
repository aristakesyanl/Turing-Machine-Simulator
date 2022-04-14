#include"Turing.h"
#include<fstream>
#include<iostream>
#include<sstream>
#include<string>
int main(int argc, char* argv[]) {
    
    for (int i = 1; i < argc; i++) {

        std::string str(argv[i]);
        Turing a(str);
        std::string test;
        std::cin>>test;
        std::cout << a.run(test) << std::endl;
        a.debug(test);
    }
    
}
