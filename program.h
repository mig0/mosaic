#ifndef __PROGRAM_H__
#define __PROGRAM_H__

#include <string>
#include <map>
using namespace std;

const auto VERSION = "1.0";

extern const char *initial_sav_filename;

map <string, int> parse_options(int &argc, char *argv[], char *envp[]);

#endif /* __PROGRAM_H__ */
