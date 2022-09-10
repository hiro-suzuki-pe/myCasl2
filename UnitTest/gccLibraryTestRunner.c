//#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP_RUNNER(fctl)
{
  RUN_TEST_CASE(fctl, open_RDONLY);
  RUN_TEST_CASE(fctl, open_WRONLY);
  RUN_TEST_CASE(fctl, open_CREAT);
  RUN_TEST_CASE(fctl, writeAndReadSmall);
  RUN_TEST_CASE(fctl, writeCharAndReadShort);
  RUN_TEST_CASE(fctl, writeShortAndReadChar);
}