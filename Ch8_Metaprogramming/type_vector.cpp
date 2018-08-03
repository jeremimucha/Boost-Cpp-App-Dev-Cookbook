#include <iostream>
#include <string>
#include <type_traits>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/back.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/unique.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/sizeof.hpp>
#include <boost/mpl/max_element.hpp>


template<typename... Ts>
struct variant
{
    using types = boost::mpl::vector<Ts...>;
};

struct declared
{
    unsigned char data[4096];
};

struct non_defined;

using types = variant<
    volatile int,
    const int,
    const long,
    declared,
    non_defined,
    std::string
    >::types;

static_assert((!boost::mpl::empty<types>::value), "Types list can't be empty");
static_assert((std::is_same_v<non_defined, boost::mpl::at_c<types,4>::type>),
    "Type at index 4 must by 'non_defined'");
static_assert((std::is_same_v<boost::mpl::back<types>::type, std::string>),
    "Last type must be std::string");

// remove const/volatile
using noncv_types = boost::mpl::transform<types, std::remove_cv_t<boost::mpl::_1>>;
