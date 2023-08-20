#include "program.h"
#include <iostream>
#include <getopt.h>

map <string, string> parse_options(int &argc, char *argv[], char *envp[]) {
	option longopts[] = {
		{ "help",       no_argument, NULL, 'h' },
		{ "version",    no_argument, NULL, 'v'},
		{ 0 },
	};

	while (1) {
		const int opt = getopt_long(argc, argv, "hv", longopts, 0);
		if (opt == -1) {
			break;
		}
		switch (opt) {
		case 'h':
			cout << R"(Usage: mosaic [OPTIONS]
Rich editor for GAN Mosaic

Options:
	-h --help     show this help and exit
	-v --version  show program version and exit
)";
			exit(0);
		case 'v':
			cout << VERSION << endl;
			exit(0);
		default:
			exit(1);
		}
	}

	argc = 0;  // don't allow anyone else to parse options

	return {
		{ "example-flag", "" },
	};
}
