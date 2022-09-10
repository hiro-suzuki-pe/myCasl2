#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
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

TEST(fctl, open_RDONLY)
{
  int fd =  open("AllTests.exe", O_RDONLY);
  if (fd == -1)
    printf("open(%d): %s\n", errno, strerror(errno));
  TEST_ASSERT_EQUAL(3,fd);
  close(fd);
}

TEST(fctl, open_WRONLY)
{
  int fd =  open("Dummy.dat", O_WRONLY);
  TEST_ASSERT_EQUAL(3,fd);
  close(fd);

  fd =  open("Dummy.dat", O_WRONLY);
  TEST_ASSERT_EQUAL(3,fd);
  close(fd);
}

TEST(fctl, open_CREAT)
{
  int fd =  open("test_creat.dat", O_CREAT);
  TEST_ASSERT_EQUAL(3,fd);
  close(fd);
  
  fd =  open("test_creat.dat", O_RDONLY);
  TEST_ASSERT_EQUAL(3,fd);
  close(fd);
  
  fd =  open("test_creat.dat", O_WRONLY);
  TEST_ASSERT_EQUAL(3,fd);
  close(fd);
}

TEST(fctl, writeAndReadSmall)
{
  int fd =  open("writeAndReadSmall.dat", O_WRONLY|O_CREAT|O_BINARY, S_IRWXU|S_IRWXU|S_IRWXO);
  char bf[] = "Small file.\n";
  int n = write(fd, bf, strlen(bf));
  TEST_ASSERT_EQUAL(strlen(bf), n);
  close(fd);

  char rbf[100];
  fd =  open("writeAndReadSmall.dat", O_RDONLY|O_BINARY);
  n = read(fd, rbf, strlen(bf));
  TEST_ASSERT_EQUAL(strlen(bf), n);
  rbf[n] = '\0';
  TEST_ASSERT_EQUAL_STRING(bf, rbf);
  close(fd);
}
TEST(fctl, writeAndReadShort)
{
  union{
    char c[1024];
    short s[512];
  } u;
  strcpy(u.c, "1234567890");
  int fd =  open("writeAndReadShort.dat", O_WRONLY|O_CREAT|O_BINARY, S_IRWXU|S_IRWXU|S_IRWXO);
  int n = write(fd, u.s, strlen(u.c));
  TEST_ASSERT_EQUAL(strlen(u.c), n);
  close(fd);

  char cb[1024];
  fd =  open("writeAndReadShort.dat", O_RDONLY|O_BINARY);
  n = read(fd, cb, strlen(u.c));
  TEST_ASSERT_EQUAL(strlen(u.c), n);
  cb[n] = '\0';
  TEST_ASSERT_EQUAL_STRING(u.c, cb);
  TEST_ASSERT_EQUAL(0x3231, u.s[0]);
  close(fd);
}
TEST(fctl, writeCharAndReadShort)
{
  int fd =  open("writeAndReadSmall.dat", O_WRONLY|O_CREAT|O_BINARY, S_IRWXU|S_IRWXU|S_IRWXO);
  char bf[] = "Small file.\n";
  int n = write(fd, bf, strlen(bf));
  TEST_ASSERT_EQUAL(strlen(bf), n);
  close(fd);

  char rbf[100];
  fd =  open("writeAndReadSmall.dat", O_RDONLY|O_BINARY);
  n = read(fd, rbf, strlen(bf));
  TEST_ASSERT_EQUAL(strlen(bf), n);
  rbf[n] = '\0';
  TEST_ASSERT_EQUAL_STRING(bf, rbf);
  close(fd);
}
TEST(fctl, writeShortAndReadChar)
{
  union{
    char c[1024];
    short s[512];
  } u;
  u.s[0] = 0x3132; u.s[1] = 0x3334;
  int fd =  open("writeShortAndReadChar.dat", O_WRONLY|O_CREAT|O_BINARY, S_IRWXU|S_IRWXU|S_IRWXO);
  int n = write(fd, u.c, 4);
  close(fd);

  char cb[1024];
  fd =  open("writeShortAndReadChar.dat", O_RDONLY|O_BINARY);
  n = read(fd, u.c, 4);
    u.c[4] = '\0';
  TEST_ASSERT_EQUAL_STRING("2143", u.c);
  close(fd);
}
