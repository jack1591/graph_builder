#include <iostream>
#include "main.cpp"
#include <gtest/gtest.h>
#include <curl/curl.h>
#include <tinyxml2.h>
using namespace std;
TEST(AddTest, HandlesPositiveInput) {
    EXPECT_EQ(add(1, 2), 3); 
}