#ifndef TEST_SUITE_H
#define TEST_SUITE_H

#include <stdio.h>
#include <iostream>
#include <string>
#include <map>

#define EXPECT_EQ(a,b) if (a != b) { \
  std::cout << std::endl << "  Expected: " << a << " == " << b << std::endl; \
  result_ = false; \
}

#define EXPECT_NE(a,b) if (a == b) { \
  std::cout << std::endl << "  Expected: " << a << " != " << b << std::endl; \
  result_ = false; \
}

#define EXPECT_FALSE(a) EXPECT_EQ(a, false)

#define EXPECT_TRUE(a) EXPECT_EQ(a, true)

#define TEST(class_name,case_name) class class_name##case_name : \
 public TestCase { \
 public: \
  class_name##case_name() : TestCase(#class_name, #case_name) {} \
  void run() override; \
} case_name; \
void class_name##case_name::run() 


class TestCase {
 public:
  TestCase(const std::string& class_name, 
           const std::string& case_name);
  virtual ~TestCase();

  virtual void run()=0;

  bool passed() { return result_; }

 protected:
  std::string class_name_;
  std::string case_name_;
  bool result_;

};

class TestSuite {

 public:
  static TestSuite* getInstance();

  void registerTestCase(
    const std::string& class_name, 
    const std::string& case_name, 
    TestCase* case_instance);

  void run();

 private:
  TestSuite();
  TestSuite(const TestSuite&) {}
  ~TestSuite();

  std::map< std::string, std::map< std::string, TestCase* > > test_cases_;
};

#endif  // TEST_SUITE_H