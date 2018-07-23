#include <iostream>
#include <string>
#include <boost/program_options.hpp>
#include <boost/program_options/errors.hpp> // 'reading_file' excetion

namespace opt = boost::program_options;


int main(int argc, char* argv[])
{
    // Constructing an option describing variable and giving
    // it a textual description "All options"
    auto desc = opt::options_description{"All options"};

    // First param - name to be used in cmd line
    // Second param - type, wrapped in `value<>` class
    // Third param - short description
    desc.add_options()
        // "long-name,short-name"
        ("apples,a", opt::value<int>()->default_value(10), "how many apples do you have")
        ("oranges,o", opt::value<int>(), "how many oranges do you have")
        ("name", opt::value<std::string>(), "your name")
        ("help", "produce help message")
        ;

    // variable storing cmd line arguments
    auto vm = opt::variables_map{};

    // Parsing and storing arguments
    opt::store(opt::parse_command_line(argc, argv, desc), vm);

    // environment variables could also be parsed here using
    // opt::parse_environment()

    opt::notify(vm);
    if( vm.count("help") ){
        std::cout << desc << "\n";
        return 1;
    }
    // Adding missing options from "apples_oranges.cfg" config file
    // parse_config_file alternatively accepts an istreamable object
    try{
        opt::store(opt::parse_config_file<char>("apples_oranges.cfg", desc), vm);
    }
    catch(const opt::reading_file& err){
        std::cout << "Failed to open file 'apples_oranges.cfg': " << err.what();
    }
    opt::notify(vm);
    if(vm.count("name")){
        std::cout << "Hi, " << vm["name"].as<std::string>() << "!\n";
    }
    std::cout << "Fruits count: "
        << vm["apples"].as<int>() + vm["oranges"].as<int>()
        << std::endl;
}
