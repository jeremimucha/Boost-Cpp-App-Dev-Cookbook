#include <iostream>
#include <boost/lexical_cast.hpp>
#include <boost/type_index.hpp>

using boost::typeindex::type_id;

int main()
{
    const auto s = boost::lexical_cast<std::string>(42);
    assert(s == "42");
    std::cout << "boost::lexical_cast<std::string>(42) = " << s << "\n";
    std::cout << "typeof(s) = " << type_id<decltype(s)>().pretty_name().c_str();
}
