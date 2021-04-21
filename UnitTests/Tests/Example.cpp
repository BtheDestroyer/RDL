#include <gtest/gtest.h>
#include "Backend/ProgramManager/ProgramManager.h" // Include whatever headers from Backend/ that are relevant to your tests

// Rename TestingClass to something that describes the category of your tests
class TestingClass : public testing::Test
{
  // This is run one time before all of your tests.
  // Treat it like a class constructor - any allocations, initial setup, etc. should go here.
  void SetUp() override
  {
  }

  // This is run one time after all of your tests.
  // Treat it like a class destructor.
  void TearDown() override
  {
  }

public:
  // All variables should be public for ease of access/use with the testing framework
};

// Declare a test function with: TEST_F(RelevantTestingClass, TestName)
// Standard unit testing convention is to form test functions following the sentence framework: Given (A), when (B), we expect (C)
// In this example: Given a=5 and b=4, when result=a+b, we expect result=9
TEST_F(TestingClass, TestAddition)
{
  // given
  // (set up inputs to the operation here)
  // (only perform operations with known outputs - ideally only constants)
  // (anything non-constexpr should have its own test to ensure its evaluation is correct)
  constexpr int a = 5;
  constexpr int b = 4;

  // when
  // (perform the operation here)
  // (this should be as short as possible - ideally only one line)
  int result = a + b;

  // expect
  // (perform tests here)
  EXPECT_EQ(result, 9);
}
