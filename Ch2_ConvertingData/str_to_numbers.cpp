#include <iostream>
#include <boost/lexical_cast.hpp>


int main()
{
    auto i = boost::lexical_cast<int>("42");
    std::cout << "boost::lexical_cast<int>(\"42\") = " << i << "\n";

// lexical_cast warns (throws) if if the requested type can't hold the given value
    try {
        auto s = boost::lexical_cast<short>("10000000");
        assert(false);
        (void)s;
    }
    catch(const boost::bad_lexical_cast& e) {
        std::cout << e.what() << "\n";
    }
// also warns if the input is not a number
    try {
        auto nan = boost::lexical_cast<int>("Not a number");
        assert(false);
        (void)nan; // suppress warning about unused variable
    }
    catch(const boost::bad_lexical_cast& e) {
        std::cout << e.what() << "\n";
    }
}
