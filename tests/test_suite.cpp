#include "test_suite.h"

#include <stdio.h>
#include <iostream>

TestCase::TestCase(const std::string& class_name, 
                   const std::string& case_name) : 
  class_name_(class_name), 
  case_name_(case_name),
  result_(true) {
  TestSuite::getInstance()->registerTestCase(class_name, case_name, this);
}

TestCase::~TestCase() {
}

TestSuite* TestSuite::getInstance() {
  static TestSuite instance;
  return &instance;
}

TestSuite::TestSuite() {
}

TestSuite::~TestSuite() {
}

void TestSuite::registerTestCase(const std::string& class_name, 
  const std::string& case_name, TestCase* case_instance) {

  test_cases_[class_name][case_name] = case_instance;
  // std::cout << "registerTestCase!" << std::endl;
}

void TestSuite::run() {
  for (auto it = test_cases_.begin(); it != test_cases_.end(); ++it) {
    std::string test_class_name = it->first;

    for (auto jt = it->second.begin(); jt != it->second.end(); ++jt) {
      std::string test_case_name = jt->first;
      TestCase* test_case_instance = jt->second;

      std::cout << test_class_name << "::" << test_case_name << ":";

      test_case_instance->run();

      if (test_case_instance->passed()) {
        std::cout << " PASSED!" << std::endl;
      } else {
        std::cout << "FAILED! " << "(" << test_class_name << "::" 
          << test_case_name << ")" << std::endl;
      }
    }
  }
}


int main(int argc, const char * argv[]) {

  // CaseName.run();

  // std::cout << TOSTR(MERGE(Cl,bbb)) << std::endl;
  TestSuite::getInstance()->run();

  return 0;
}

