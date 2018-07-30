#include <iostream>
#include <boost/numeric/conversion/cast.hpp>


int main()
{
// this is fine
    const auto s1 = boost::numeric_cast<unsigned short>(42);
    std::cout << "boost::numeric_cast<unsigned short>(42) = " << s1 << "\n";

// this will throw
    try{
        const auto s2 = boost::numeric_cast<unsigned>(-1);
        (void)s2;
    }
    catch(const boost::numeric::bad_numeric_cast& e){
        std::cout << e.what() << "\n";
    }

// so will this
    try{
        const auto s3 = boost::numeric_cast<unsigned short>(100000);
        (void)s3;
    }
    catch(const boost::numeric::positive_overflow& e){
        std::cout << e.what() << "\n";
    }

// exception types:
    // boost::numeric::positive_overflow;
    // boost::numeric::negative_overflow;
    // boost::numeric::bad_numeric_cast;
}
