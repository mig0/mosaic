#ifndef __PROGRAM_H__
#define __PROGRAM_H__

#include <string>
#include <map>
using namespace std;

const auto VERSION = "1.0";

map <string, string> parse_options(int &argc, char *argv[], char *envp[]);

#endif /* __PROGRAM_H__ */
