#include "program.h"
#include <iostream>
#include <getopt.h>

map <string, string> parse_options(int &argc, char *argv[], char *envp[]) {
	option longopts[] = {
		{ "help",         no_argument, NULL, 'h' },
		{ "version",      no_argument, NULL, 'v' },
		{ "new-instance", no_argument, NULL, 'n' },
		{ 0 },
	};

	bool new_instance = false;

	while (1) {
		const int opt = getopt_long(argc, argv, "hvn", longopts, 0);
		if (opt == -1) {
			break;
		}
		switch (opt) {
		case 'h':
			cout << R"(Usage: mosaic [OPTIONS]
Rich editor for GAN Mosaic

Options:
	-h --help          show this help and exit
	-v --version       show program version and exit
	-n --new-instance  create new graphical instance
)";
			exit(0);
		case 'v':
			cout << VERSION << endl;
			exit(0);
		case 'n':
			new_instance = true;
			break;
		default:
			exit(1);
		}
	}

	argc = 0;  // don't allow anyone else to parse options

	return {
		{ "example-flag", "" },
		{ "new-instance", new_instance ? "requested" : "" },
	};
}
