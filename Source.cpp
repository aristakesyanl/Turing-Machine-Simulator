int main(int argc, char* argv[]) {
    
    for (int i = 1; i < argc; i++) {

        std::string str(argv[i]);
        Turing a(str);
        std::cout<<"If you want to run the program in mode type 'e' else type 'd'"<<std::endl;
        char st;
        std::cin>>st;
        if(st=='e'){
            std::cout<<"Enter Tape Input:"<<std::endl;
            std::string test;
            std::cin>>test;
            std::cout << a.run(test) << std::endl;
        }
        else if(st=='d'){
            std::cout<<"Enter Tape Input:"<<std::endl;
            std::string test;
            std::cin>>test;
            a.debug(test);

        }
        else{
            std::cerr<<"Invalid Instruction"<<std::endl;
        }
    }
    
}
