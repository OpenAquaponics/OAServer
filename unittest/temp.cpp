

#include <iostream>
#if 0
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#endif

#ifndef json_included
#define json_included
#include "json/json.h"
#endif

using namespace std;

int main(int argc, char **argv)
{

std::string example = "{\"array\":[\"item1\", \"item2\"], \"not an array\":\"asdf\"}";
Json::Value value;
Json::Reader reader;
Json::Value::Members members;

bool parsed = reader.parse(example, value, false);

members = value.getMemberNames();

std::cout << parsed << endl;
std::cout << value.toStyledString();

for(int i = 0; i < members.size(); i++) {
  cout << members[i] << endl;
}

return 0;
}
