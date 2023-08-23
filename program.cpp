#include "program.h"
#include <iostream>
#include <getopt.h>

bool parse_size(char *str, int &size_x, int &size_y) {
	if (str == nullptr)
		return false;

	char *first = str;
	while (*str != '\0' && *str != 'x') {
		if (*str < '0' || *str > '9')
			return false;
		str++;
	}

	if (*str != 'x')
		return false;
	*str = '\0';
	int x = stoi(first);
	*str = 'x';
	str++;

	char *second = str;
	while (*str != '\0') {
		if (*str < '0' || *str > '9')
			return false;
		str++;
	}

	int y = stoi(second);
	if (!x || !y)
		return false;

	size_x = x;
	size_y = y;
	return true;
}

map <string, int> parse_options(int &argc, char *argv[], char *envp[]) {
	option longopts[] = {
		{ "help",         no_argument,       NULL, 'h' },
		{ "version",      no_argument,       NULL, 'v' },
		{ "new-instance", no_argument,       NULL, 'n' },
		{ "size",         required_argument, NULL, 's' },
		{ 0 },
	};

	bool new_instance = false;
	int size_y = 30;
	int size_x = 30;

	parse_size(getenv("MOSAIC_SIZE"), size_x, size_y);

	while (1) {
		const int opt = getopt_long(argc, argv, "hvns:", longopts, 0);
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
	-s --size          set grid size (default: 30x30)
)";
			exit(0);
		case 'v':
			cout << VERSION << endl;
			exit(0);
		case 'n':
			new_instance = true;
			break;
		case 's':
			if (!parse_size(optarg, size_x, size_y)) {
				cerr << "Invalid size " << optarg << ", should be in format <X>x<Y>" << endl;
				exit(1);
			}
			break;
		default:
			exit(1);
		}
	}

	if (size_x < 6 && size_y < 6) {
		cerr << "Requested size " << size_x << 'x' << size_y << " is too small" << endl;
		exit(1);
	}
	if (size_x > 200 || size_y > 100) {
		cerr << "Requested size " << size_x << 'x' << size_y << " is too large" << endl;
		exit(1);
	}

	argc = 0;  // don't allow anyone else to parse options

	return {
		{ "new-instance", new_instance ? 1 : 0 },
		{ "size_y", size_y },
		{ "size_x", size_x },
	};
}
