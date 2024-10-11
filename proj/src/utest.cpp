#define UNIT_TEST // предотвращает компиляцию main() в main.cpp
#include "main.cpp" 
#include <gtest/gtest.h>
#include <string>
#include <set>
#include <vector>
using namespace std;
string s = "../src/my-proj.tar";
set<string> example = {"project","artem","qwerty","rybinsk","mike","true","false","abcde","detective","twin","bin","bash"};
vector<string> example_names = {"first/abc1.txt","first/my-proj/1/","first/my-proj/2/bin/ahahaha/ahahah1.txt"};

TEST(AddTest, HandlesPositiveInput_cd1) {
    add_files(s.c_str(),"");
    EXPECT_TRUE(cd(s.c_str(), "", "first/my-proj/2/") == "first/my-proj/2/");
}


TEST(AddTest, HandlesPositiveInput_cd2) {
    add_files(s.c_str(),"");
    EXPECT_TRUE(cd(s.c_str(), "", "fourth/") == "");
}

TEST(AddTest, HandlesPositiveInput_cd3) {
    add_files(s.c_str(),"");
    EXPECT_TRUE(cd(s.c_str(), "", "first/abc2.txt") == "");
}


TEST(AddTest, HandlesPositiveInput_cd4) {
    add_files(s.c_str(),"");
    EXPECT_TRUE(cd(s.c_str(), "", "first/my-proj/..") == "first/");
}

TEST(AddTest, HandlesPositiveInput_whoami) {
    hostname = "PC_folene";
    EXPECT_TRUE(whoami() == "PC_folene");
}

TEST(AddTest, HandlesPositiveInput_ls) {
    add_files(s.c_str(),"");
    EXPECT_TRUE(files_in_dir[0]=="first/" && files_in_dir[1]=="second/" && files_in_dir[2]=="tekst.txt" && files_in_dir[3]=="third/");
}

TEST(AddTest, HandlesPositiveInput_uniq1) {
    add_files(s.c_str(),"first/");
    EXPECT_TRUE(uniq(s.c_str(),"first/","abc2.txt") == example);
}

TEST(AddTest, HandlesPositiveInput_uniq2) {
    add_files(s.c_str(),"first/");
    EXPECT_TRUE(uniq(s.c_str(),"first/","abc1.txt") != example);
}

TEST(AddTest, HandlesPositiveInput_find1) {
    add_files(s.c_str(),"");
    EXPECT_TRUE(find(s.c_str(),"1") == example_names);
}

TEST(AddTest, HandlesPositiveInput_find2) {
    add_files(s.c_str(),"");
    EXPECT_TRUE(find(s.c_str(),"2/") != example_names);
}
