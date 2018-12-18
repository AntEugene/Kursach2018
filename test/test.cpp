#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Server
#include <boost/test/unit_test.hpp>

#include <iostream>
#include <calc.h>

BOOST_AUTO_TEST_SUITE(ServerTest)

BOOST_AUTO_TEST_CASE(ReturnTest)
{
    Test first_test;
    BOOST_CHECK_EQUAL(first_test.add_task(1), 13);
}

BOOST_AUTO_TEST_CASE(NextReturnTest)
{
	Test second_test;
	BOOST_CHECK_EQUAL(second_test.add_task(2), 14);
}
BOOST_AUTO_TEST_SUITE_END()
