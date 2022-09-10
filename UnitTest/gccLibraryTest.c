//#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP(fctl);

//sometimes you may want to get at local data in a module.
//for example: If you plan to pass by reference, this could be useful
//however, it should often be avoided

TEST_SETUP(fctl)
{
  //This is run before EACH TEST
}

TEST_TEAR_DOWN(fctl)
{
}

TEST(fctl, NoTest)
{
}
