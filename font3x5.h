#ifndef __FONT3x5_H__
#define __FONT3x5_H__

const char FIRST_FONT_CHAR = ' ';
const char LAST_FONT_CHAR  = '~';

constexpr int font3x5[LAST_FONT_CHAR - FIRST_FONT_CHAR + 1][5] = {
	{ 0, 0, 0, 0, 0 },  // ' '
	{ 2, 2, 2, 0, 2 },  // '!'
	{ 5, 5, 0, 0, 0 },  // '"'
	{ 5, 7, 5, 7, 5 },  // '#'
	{ 6, 1, 2, 4, 3 },  // '$'
	{ 5, 4, 2, 1, 5 },  // '%'
	{ 3, 1, 7, 1, 7 },  // '&'
	{ 2, 2, 0, 0, 0 },  // '''
	{ 2, 1, 1, 1, 2 },  // '('
	{ 1, 2, 2, 2, 1 },  // ')'
	{ 2, 7, 2, 5, 0 },  // '*'
	{ 0, 2, 7, 2, 0 },  // '+'
	{ 0, 0, 0, 2, 2 },  // ','
	{ 0, 0, 7, 0, 0 },  // '-'
	{ 0, 0, 0, 0, 2 },  // '.'
	{ 4, 2, 2, 1, 1 },  // '/'
	{ 7, 5, 5, 5, 7 },  // '0'
	{ 3, 2, 2, 2, 7 },  // '1'
	{ 7, 4, 7, 1, 7 },  // '2'
	{ 7, 4, 7, 4, 7 },  // '3'
	{ 5, 5, 7, 4, 4 },  // '4'
	{ 7, 1, 7, 4, 7 },  // '5'
	{ 7, 1, 7, 5, 7 },  // '6'
	{ 7, 5, 4, 4, 4 },  // '7'
	{ 7, 5, 7, 5, 7 },  // '8'
	{ 7, 5, 7, 4, 4 },  // '9'
	{ 0, 2, 0, 2, 0 },  // ':'
	{ 0, 2, 0, 2, 2 },  // ';'
	{ 4, 2, 1, 2, 4 },  // '<'
	{ 0, 7, 0, 7, 0 },  // '='
	{ 1, 2, 4, 2, 1 },  // '>'
	{ 7, 4, 6, 0, 2 },  // '?'
	{ 7, 5, 5, 1, 7 },  // '@'
	{ 7, 5, 7, 5, 5 },  // 'A'
	{ 7, 5, 3, 5, 7 },  // 'B'
	{ 7, 1, 1, 1, 7 },  // 'C'
	{ 3, 5, 5, 5, 3 },  // 'D'
	{ 7, 1, 7, 1, 7 },  // 'E'
	{ 7, 1, 7, 1, 1 },  // 'F'
	{ 7, 1, 5, 5, 7 },  // 'G'
	{ 5, 5, 7, 5, 5 },  // 'H'
	{ 7, 2, 2, 2, 7 },  // 'I'
	{ 4, 4, 4, 5, 7 },  // 'J'
	{ 5, 5, 3, 5, 5 },  // 'K'
	{ 1, 1, 1, 1, 7 },  // 'L'
	{ 5, 7, 5, 5, 5 },  // 'M'
	{ 7, 5, 5, 5, 5 },  // 'N'
	{ 7, 5, 5, 5, 7 },  // 'O'
	{ 7, 5, 7, 1, 1 },  // 'P'
	{ 7, 5, 5, 3, 5 },  // 'Q'
	{ 7, 5, 3, 5, 5 },  // 'R'
	{ 7, 1, 7, 4, 7 },  // 'S'
	{ 7, 2, 2, 2, 2 },  // 'T'
	{ 5, 5, 5, 5, 7 },  // 'U'
	{ 5, 5, 5, 5, 2 },  // 'V'
	{ 5, 5, 5, 7, 5 },  // 'W'
	{ 5, 5, 2, 5, 5 },  // 'X'
	{ 5, 5, 7, 2, 2 },  // 'Y'
	{ 7, 4, 2, 1, 7 },  // 'Z'
	{ 3, 1, 1, 1, 3 },  // '['
	{ 1, 2, 2, 4, 4 },  // '\'
	{ 3, 2, 2, 2, 3 },  // ']'
	{ 2, 5, 0, 0, 0 },  // '^'
	{ 0, 0, 0, 0, 7 },  // '_'
	{ 1, 2, 0, 0, 0 },  // '`'
	{ 0, 6, 5, 5, 6 },  // 'a'
	{ 1, 7, 5, 5, 7 },  // 'b'
	{ 0, 7, 1, 1, 7 },  // 'c'
	{ 4, 7, 5, 5, 7 },  // 'd'
	{ 0, 6, 5, 3, 6 },  // 'e'
	{ 4, 2, 7, 2, 2 },  // 'f'
	{ 0, 3, 5, 6, 3 },  // 'g'
	{ 1, 7, 5, 5, 5 },  // 'h'
	{ 2, 0, 2, 2, 7 },  // 'i'
	{ 2, 0, 2, 2, 3 },  // 'j'
	{ 1, 5, 3, 5, 5 },  // 'k'
	{ 3, 2, 2, 2, 6 },  // 'l'
	{ 0, 5, 7, 5, 5 },  // 'm'
	{ 0, 7, 5, 5, 5 },  // 'n'
	{ 0, 7, 5, 5, 7 },  // 'o'
	{ 0, 7, 5, 7, 1 },  // 'p'
	{ 0, 7, 5, 7, 4 },  // 'q'
	{ 0, 7, 1, 1, 1 },  // 'r'
	{ 0, 6, 3, 6, 3 },  // 's'
	{ 2, 7, 2, 2, 6 },  // 't'
	{ 0, 5, 5, 5, 7 },  // 'u'
	{ 0, 5, 5, 5, 2 },  // 'v'
	{ 0, 5, 5, 7, 5 },  // 'w'
	{ 0, 5, 2, 5, 5 },  // 'x'
	{ 0, 5, 5, 7, 4 },  // 'y'
	{ 0, 7, 6, 1, 7 },  // 'z'
	{ 6, 2, 3, 2, 6 },  // '{'
	{ 2, 2, 2, 2, 2 },  // '|'
	{ 3, 2, 6, 2, 3 },  // '}'
	{ 0, 4, 7, 1, 0 },  // '~'
};

static const int *get_font3x5_char_bitmap(char ch) {
	if (ch < FIRST_FONT_CHAR || ch > LAST_FONT_CHAR)
		ch = '?';
	return font3x5[ch - FIRST_FONT_CHAR];
}

#endif /* __FONT3x5_H__ */
