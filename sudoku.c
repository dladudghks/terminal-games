int sudoku_solved(char *sudoku);

/* -------------------------------------------------------------------------- */

static void square(int *x, int *y, int i)
{
	*x = i % 3;
	*y = i / 3;
}

static void row(int *x, int *y, int i)
{
	*x = i;
	*y = 0;
}

static void col(int *x, int *y, int i)
{
	*x = 0;
	*y = i;
}

static int solved(char *sudoku, int x, int y, void fn(int *x, int *y, int i))
{
	int c, i, xinc, yinc;
	char tmp[9] = { 0 };

	for (i = 0; i < 9; i++) {
		fn(&xinc, &yinc, i);
		c = sudoku[(x + xinc) + (y + yinc)*9];
		if (c >= '1' && c <= '9')
			tmp[c - '1']++;
	}
	for (i = 0; i < 9; i++)
		if (tmp[i] != 1)
			return 0;
	return 1;
}

int sudoku_solved(char *sudoku)
{
	int i;

	for (i = 0; i < 9; i++)
		if (!solved(sudoku, i, 0, col)
		 || !solved(sudoku, 0, i, row)
		 || !solved(sudoku, i%3*3, i/3*3, square))
			return 0;
	return 1;
}

/* -------------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <termbox.h>

char *puzzles[];
int npuzzles;

int sudoku(char *puzzle);

int main(void)
{
	if (tb_init()) {
		fprintf(stderr, "sudoku: can't open display\n");
		return EXIT_FAILURE;
	}
	srand(time(NULL));
	atexit(tb_shutdown);
	tb_select_input_mode(TB_INPUT_MOUSE);
	tb_select_output_mode(TB_OUTPUT_NORMAL);
	while (sudoku(puzzles[rand() % npuzzles]))
		;
	return 0;
}

static int mouse2selection(int x, int y)
{
	x -= tb_width()/2 - 4*9/2;
	y -= tb_height()/2 - 2*9/2;
	if (x < 0 || y < 0 || x >= 4*9 || y >= 2*9 || x%4 == 0 || y%4 == 0)
		return -1;
	return (y / 2)*9 + x/4;
}

static void display(char *puzzle, char *current, int selection, int solved);

int sudoku(char *puzzle)
{
	struct tb_event ev;
	char s[9*9 + 1];
	int sel, solved;

	strcpy(s, puzzle);
	sel = -1;
	solved = 0;
	display(puzzle, s, sel, solved);
	for (; tb_poll_event(&ev) > 0; display(puzzle, s, sel, solved)) {
		if (ev.type == TB_EVENT_MOUSE && ev.key == TB_KEY_MOUSE_LEFT)
			sel = mouse2selection(ev.x, ev.y);
		if (ev.type != TB_EVENT_KEY)
			continue;
		if (ev.key == TB_KEY_CTRL_C)
			break;
		if (solved && ev.key == '\r')
			return 1;
		if ((ev.key == TB_KEY_ARROW_LEFT || ev.key == TB_KEY_ARROW_RIGHT
		  || ev.key == TB_KEY_ARROW_UP || ev.key == TB_KEY_ARROW_DOWN)
		 && sel == -1)
			sel = 0;
		else if (ev.key == TB_KEY_ARROW_LEFT && sel-1 >= 0)
			sel--;
		else if (ev.key == TB_KEY_ARROW_RIGHT && sel+1 < 9*9)
			sel++;
		else if (ev.key == TB_KEY_ARROW_UP && sel-9 >= 0)
			sel -= 9;
		else if (ev.key == TB_KEY_ARROW_DOWN && sel+9 < 9*9)
			sel += 9;
		else if (sel >= 0 && sel < 9*9 && puzzle[sel] == ' '
		      && ev.ch >= '1' && ev.ch <= '9')
			s[sel] = ev.ch;
		else if (sel >= 0 && sel < 9*9 && puzzle[sel] == ' '
		      && (ev.key == TB_KEY_DELETE || ev.key == TB_KEY_BACKSPACE2
		       || ev.key == TB_KEY_SPACE || ev.ch == '0'))
			s[sel] = ' ';
		solved = sudoku_solved(s);
	}
	return 0;
}

void display(char *puzzle, char *current, int selection, int solved)
{
	char *s = {
		"╔═══╤═══╤═══╦═══╤═══╤═══╦═══╤═══╤═══╗\n"
		"║ * │ * │ * ║ * │ * │ * ║ * │ * │ * ║\n"
		"╟───┼───┼───╫───┼───┼───╫───┼───┼───╢\n"
		"║ * │ * │ * ║ * │ * │ * ║ * │ * │ * ║\n"
		"╟───┼───┼───╫───┼───┼───╫───┼───┼───╢\n"
		"║ * │ * │ * ║ * │ * │ * ║ * │ * │ * ║\n"
		"╠═══╪═══╪═══╬═══╪═══╪═══╬═══╪═══╪═══╣\n"
		"║ * │ * │ * ║ * │ * │ * ║ * │ * │ * ║\n"
		"╟───┼───┼───╫───┼───┼───╫───┼───┼───╢\n"
		"║ * │ * │ * ║ * │ * │ * ║ * │ * │ * ║\n"
		"╟───┼───┼───╫───┼───┼───╫───┼───┼───╢\n"
		"║ * │ * │ * ║ * │ * │ * ║ * │ * │ * ║\n"
		"╠═══╪═══╪═══╬═══╪═══╪═══╬═══╪═══╪═══╣\n"
		"║ * │ * │ * ║ * │ * │ * ║ * │ * │ * ║\n"
		"╟───┼───┼───╫───┼───┼───╫───┼───┼───╢\n"
		"║ * │ * │ * ║ * │ * │ * ║ * │ * │ * ║\n"
		"╟───┼───┼───╫───┼───┼───╫───┼───┼───╢\n"
		"║ * │ * │ * ║ * │ * │ * ║ * │ * │ * ║\n"
		"╚═══╧═══╧═══╩═══╧═══╧═══╩═══╧═══╧═══╝\n"
	};
	int i, x, y;
	uint32_t c;

	tb_clear();
	x = tb_width()/2 - 4*9/2;
	y = tb_height()/2 - 2*9/2;
	i = 0;
	while (*(s += tb_utf8_char_to_unicode(&c, s)) != '\0') {
		if (selection == i && c == '*')
			tb_set_cursor(x, y);
		if (c != '*')
			tb_change_cell(x, y, c, 0, 0);
		else {
			tb_change_cell(x, y, current[i],
			               (puzzle[i] == current[i] || solved)
			                 ? TB_GREEN : 0,
			               0);
			i++;
		}
		y += (c == '\n');
		x = (c == '\n') ? tb_width()/2 - 4*9/2: x + 1;
	}
	tb_present();
}

char *puzzles[] = {
	"9  4    6 143  8  6   197        2 8 95  "
	" 67 8 1        716   4  6  452 5    2  1",

	" 9 8 16     3     1   6  9 6 4  2  7 25 3"
	" 14 8  7  5 2 8  2   3     5     64 3 8 ",

	"9   1 356 5   6  86   8   2 7    5    5 2"
	" 1    3    6 1   4   55  3   2 742 6   3",

	"   8  51      3  695 24    5    4238     "
	"    4829    1    72 692  4      71  6   ",

	"8 7       9  2  8 2 6 5 1 9 1 8    64   3"
	"   57    4 2 1 5 4 8 7 8  7  3       6 4",

	"  6  31954    528  15 7     3 9     7    "
	"   9     1 3     4 82  682    11423  9  ",

	"  89   7 2  7    89 5 3  6 72   14      4"
	"      35   29 4  8 9 73    5  1 8   95  ",

	" 4      6 156   3 2     19   4192  8    4"
	"    6  8359   38     5 7   864 1      2 ",

	"1  7  9  9 6  47      9  568 2    9   314"
	"92   9    4 325  1      95  8 4  7  6  2",

	" 7  6  2 6    2 8   18   43  7 3 49    9 "
	"4    98 1 5  31   78   8 3    4 4  8  6 ",

	"8  6       4 8 61  27 4  5     92 68  9  "
	" 7  16 83     1  5 38  73 1 4       6  1",

	"78   314   24 9  3    2   8   9    4 3  4"
	"  6 9    2   5   1    4  2 83   216   57",

	"  5 9   6789    341  3    7 9   38 5  4  "
	" 3  6 78   2 5    1  397    1584   8 7  ",

	"1  3 7  4 57      83   5 2 7      9 9 618"
	"23 5 2      8 7 6   39      14 4  7 8  2",

	"    5 3 7 24  7 6   31   8  49     6 3 64"
	"5 9 6     23  6   85   8 4  61 7 1 6    ",

	"   1     7   5 8    87 4 2 51   3  8 64 7"
	" 51 3  5   42 2 6 89    9 2   6     7   ",

	"5    49 8  75 94 3    7   572        51  "
	" 78        466   4    3 86 15  9 27    1",

	"   8745     3  81 5   9     4   369  8 64"
	"7 3  635   2     3   7 59  2     7958   ",

	" 85  9       15 98  6  21 7  9  72 6 6   "
	"  7 2 86  5  8 41  9  61 29       4  76 ",

	"  72       64    19   37 2 672 1     5 97"
	"2 8     5 172 8 19   41    38       89  ",

	"  9      8   9 3122    5 9 73   245 6    "
	"   3 984   71 6 3    5387 2   9      8  ",

	"    6 9 7     483   687  2   9  1 8 2  6 "
	"8  9 4 3  5   6  921   125     9 8 3    ",

	"9     3 2 21  3  6    9 51  5  8      645"
	"21      6  7  97 4    5  7  92 3 4     1",

	"5 9     3 1 2   7  249 3   75842 9       "
	"      6 39725   5 769  6   2 8 2     5 4",

	"    7    3  2  84 5 4  1 62  94   8 1  7 "
	"9  3 3   89  46 5  2 7 53  2  8    1    ",

	" 7 5 1  6    6  5253     1      3 983  1 "
	"8  519 2      2     8761  7    7  9 4 6 ",

	"6958      1     2 2   75  8573     6   5 "
	"4   1     2854  31   2 5     1      2734",

	"       844 5  739      6  7 4  1 7  6 175"
	"49 8  9 3  2 2  3      164  5 383       ",

	"2   1 6   4 97   3 57      798   235    5"
	"    635   814      49 9   38 5   2 6   8",

	"    6  9 2   593 1  94  87 9 6 1   8    3"
	"    3   4 7 9 28  54  4 537   2 1  8    ",

	" 4 7     89 1 5   1   68 5   4 8  1 6  57"
	"3  9 5  4 8   6 35   2   6 4 93     7 4 ",

	"78 2    42 6 54 9          4 81      7 3 "
	"6 1      75 8          2 54 9 63    1 87",

	"        4    7  916 8 4  3    7   46 6798"
	"135 82   3    1  5 9 323  9    5        ",

	"  5 6   9 6   9 47 8    365 7 6 8   1   9"
	"   6   4 7 3 839    5 42 8   9 5   4 6  ",

	" 7 3    8 2     4   96 43  7    319  98  "
	" 56  621    4  12 54   4     1 2    6 3 ",

	" 38     62    7 451 7 2       97    3  51"
	"6  4    42       5 2 184 2    36     49 ",

	" 13  82  2  6  8      235     28  7 7 1 3"
	" 9 8 3  19     295      6  2  9  48  12 ",

	"3 6 159   7   25 1 4 3       5 3   7 3  5"
	"  8 6   8 4       3 4 8 39   1   712 3 6",

	"61    349   5 4  6 4   3    6 4 2  3 21  "
	" 97 9  8 1 6    1   3 2  7 5   174    52",

	" 6 3  84   45    2 1  4 35 6 3  2 8 1    "
	"   6 5 4  1 3 71 6  3 9    42   32  1 6 ",

	"    7 9    34  8 228   3  4  5    9 1  32"
	"9  8 3    1  5  1   493 7  45    4 8    ",

	" 6 3     38       9 47  23   1 3   6 4952"
	"718 7   9 5   72  53 1       78     4 5 ",

	" 2569  1      25943     8 6   9     98  5"
	"  72     1   4 2     75174      3  2794 ",

	"     6    5 1 28    8 4   28  9   64 64 2"
	" 13 51   4  93   5 7    78 3 5    2     ",

	"693 2   1   1    2    8 9  57   93   4 21"
	"5 9   97   15  7 5    9    8   2   7 549",

	"  67   5 1    48   3     216581  3      6"
	"      9  361247     8   24    5 9   62  ",

	" 9 84 2 1      49 6  1    89   2 1  4 6 1"
	" 3 9  1 5   42    9  3 67      3 9 84 1 ",

	"8  451 9 6 42   1 5   6 8   726        5 "
	"4        346   3 4   5 5   67 4 4 835  1",

	"   4   1 284 1   9   8  6 3 1 5 93       "
	"      32 6 8 3 9  8   8   4 596 6   2   ",

	" 7   83 429   5  86 8 3  5 3 9       5   "
	"  6       9 5 1  4 5 75  6   397 49   1 ",

	" 9    5 6     4 92 7  2  8    486  3 2  5"
	"  6 8  293    8  7  4 51 6     2 9    7 ",

	"7    3 46   4 782   9  85   9   4 73   3 "
	"9   34 5   8   87  3   546 2   13 8    2",

	"  5 2   43   14 8       73 218   467    8"
	"    934   518 89       7 39   15   6 2  ",

	"      7  7  246   38 1      3 72  41 71 6"
	" 38 54  81 6      2 74   513  9  8      ",

	" 23 698   1 2     4   81 3   4   1   65 7"
	" 42   7   6   5 64   9     7 5   892 71 ",

	"  49  6   6    19 1 96     25  6  19   32"
	"9   98  7  43     59 2 78    3   2  68  ",

	" 4  351  2   7  9       6 3964   538    6"
	"    817   4261 5       8  2   9  684  5 ",

	"       4 1   7 9 853        6 5  37  523 "
	"489  13  8 6        294 9 3   5 2       ",

	"   2  79    7    68 9 6     8695  37  5 7"
	" 8  74  1625     4 9 54    8    61  7   ",

	"   8 9 4  8  6  31 753     9    7 1   2 5"
	" 8   5 1    9     835 26  9  7  3 4 1   ",

	" 36  528    63   11  9  65 89   1     3  "
	" 8     8   62 19  6  87   93    642  93 ",

	"   8       1 6 25 6  3  4 1     654 16   "
	"  28 952     5 9  8  2 72 5 3       3   ",

	"3     8 5   5  23 12 3    4     91  8 3 6"
	" 9 2  27     7    8 29 86  4   5 4     7",

	"  93 25   4695 3 25  7     6 8     5 7   "
	"  3 3     9 8     5  34 5 7361   36 94  ",

	"    5  475    9    82  6   6 5 8273  7  6"
	"  9  2947 1 6   3  46    6    294  2    ",

	"    2 8 7 1   56    9486    7    45 38  6"
	"  12 94    6    6125    28   9 8 1 4    ",

	" 1  5     4536  8 9 3    5 42       75  3"
	"  19       46 7    5 4 6  2537     9  6 ",

	" 9     3 4682       2 968  253   9     8 "
	"3     7   346  534 2       5713 2     9 ",

	"  31 768 5   2  9         3  6  3 1 3 5  "
	" 2 7 1 6  5  9         4  9   2 325 19  ",

	"5  2           352  7435   98    2 14 3  "
	" 7 57 5    98   7985  379           2  7",

	" 26 87 4 38 54   2  7     1     9 1 8   5"
	"   9 1 2     2     6  7   65 84 6 82 19 ",

	"3589  6 2  2 65      2       95  3 7 85  "
	" 91 4 3  95       1      34 2  5 6  2138",

	"  9 6 4 7 68  4     4  18  6  8    5   53"
	"7   3    6  9  14  2     6  18 8 6 7 5  ",

	" 3 94   8  4         36 1  96 1  45  1  7"
	"  8  87  5 61  9 13         3  3   86 1 ",

	"3 72   916 93 4     5        1  2  52  83"
	"9  44  1  3        5     4 18 998   31 6",

	"  1  43 7   95 2 14      5     9 7 4  2 3"
	" 5  9 6 1     7      56 4 27   2 53  1  ",

	" 4  6 25  17 8 6       7  887 59      2  "
	" 9      38 711  7       6 1 32  83 4  1 ",

	"56  8      47   367 3   8      1  5 6  35"
	"9  4 2  4      6   7 983   25      9  82",

	"18    65        39  45 92   5  1 8  9  82"
	"5  7  2 7  9   82 39  46        93    81",

	"   3  6 7  8 19 24      19 21  4    3    "
	"   6    2  31 56      92 56 8  4 7  8   ",

	"  7    6 8  17 45 91   4    7   85 32  5 "
	"7  84 83   2    6   14 84 31  5 3    2  ",

	"   4     4 2 675  79   2 48  4    5 3  5 "
	"6  9 5    8  13 7   25  782 1 4     1   ",

	"5   6   9  2  8  58   3 624  3    8  9  2"
	"  5  5    4  385 9   17  3  5  2   7   3",

	"  15  2   28 9    7  2   46     9 12 8  1"
	"  6 51 8     95   1  8    4 53   2  54  ",

	"  5     3  2 7 9 6     157    7   59  892"
	"47  92   6    135     5 6 3 1  7     8  ",

	"2 45  9  8   67 4  1 43   24     8   5   "
	"  7   1     37   26 3  4 97   8  6  37 9",

	"  1     62 534 8         3 91  5  2  5 2 "
	"3 1  7  8  43 6         8 794 55     1  ",

	"329           43 85   76   7   21   21   "
	"  96   56   2   74   58 46           214",

	"3 18  47   8   2     4  9 3 42  7  68  6 "
	"4  21  5  74 7 5  8     4   3   13  98 4",

	"12 4  573   3    43   762  5 1 34        "
	"       96 3 1  481   28    3   239  7 18",

	"   976   7      1 6 3 4     6 73   2 82 5"
	" 63 9   21 4     9 3 1 2      5   215   ",

	" 7  1 2491    2    56  9     57 3  1     "
	"    9  6 45     9  75    4    6763 2  9 ",

	"   5       41 6 2 6   2 4  29   3  8 73 5"
	" 96 1  8   73  1 3   5 2 9 51       7   ",

	" 92 5   615   3   6  21   82 7 9       48"
	"6       2 6 98   41  7   9   649   7 35 ",

	"49   15   53   8 1 61 5  94   42     4   "
	"  1     13   81  3 64 6 4   27   75   83",

	"1    4  5   3 79 6 3   518   5  961    2 "
	"1    146  2   761   4 5 34 6   9  5    2",

	"39  1  6   7  495       8  9   6 7   82 3"
	" 69   6 2   1  3       741  2   6  5  84",

	"   9  2 6    4   1  4 86    4   2 9 93 45"
	"1 27 5 3   6    71 3  1   6    3 5  4   ",

	" 5     3982   4    1 95 7 2    8   45  13"
	"7  86   9    7 5 63 2    5   8798     5 ",

	"    7    3   4 7  76251     361   9  2 7 "
	"9 6  9   581     56182  8 2   4    9    ",

	"6  3   2  94  2 3  2 76     6 1 75 2    3"
	"    2 89 5 1     21 7  5 4  36  8   3  1",

	" 93    8  863  4    7  12    19    2   4 "
	"7   9    65    91  3    5  487  1    69 ",

	"  5 7 1 36  8  952   12      6   3 1 1   "
	"  7 7 8   5      54   389  7  54 2 6 7  ",

	"1   48       6 3   4 973 86 1 3  87      "
	"     92  4 6 46 182 3   1 3       79   8",

	" 5 7  6 27    61   16 93     5 49 1   3  "
	" 4   2 36 5     17 98   19    43 9  4 5 ",

	" 46  2  7 1  4    3 96       576   318   "
	"  946   892       57 8    3  4 8  4  36 ",

	"  45     97 8  5 3 56      1   5  4 7 923"
	"86 1 8  7   2      86 3 8  2 15     92  ",

	"        93   65    4  925  9 7  362   4 1"
	" 3   367  4 1  364  5    53   25        ",

	" 54     69  6374  6 3 1     6   4    4  5"
	"  9    2   5     8 7 3  2376  97     52 ",

	"93 8       14    8   59 4  69  4 5    375"
	"86    8 3  41  9 75   8    23       4 52",

	" 7  8    8 9     5 2597     63      7 439"
	"81 2      39     5784 1     5 9    4  3 ",

	"61 5   4  7   2 8 35      6 2 3  8     4 "
	"7     3  1 9 2      13 3 2   5  9   4 67",

	"     54 2  1 78  6  5  2 1  7      4 6 8 "
	"1 9 4      6  1 3  5  8  25 6  9 21     ",

	"  26   3  69  1  2  3 2 8  2 5 9   4   5 "
	"3   1   6 5 3  1 8 7  9  2  36  8   72  ",

	" 7 6 9    9  3 7   8  7 1  36    8  7  8 "
	"2  1  8    96  6 1  3   4 2  1    3 4 5 ",

	"    67   5 7 1   8329  86  2 8     6 4   "
	"  8 1     4 9  12  5636   8 9 4   45    ",

	"          74  1 98 2  874 3  5 769  2    "
	"   4  139 8  4 296  8 93 1  26          ",

	"135      8  3  2 9   7  1  513 79     8 3"
	" 7     18 352  9  6   2 1  3  8      513",

	"3  4  16  7  9  5   9  5 4 7   1  86   8 "
	"6   48  3   2 4 1  6   6  4  9  31  7  4",

	"  2 5   458    3 2 3 9  8 5   2  4 9 1   "
	"  7 9 3  5   1 6  9 2 3 8    462   7 1  ",

	"51 7  3    2    6   6 9 4   68  7 1 2   1"
	"   6 4 6  97   5 8 1   9    2    3  1 94",

	"45 2   9  8  5   4 6 4 8      93 2    9  "
	" 7    8 25      1 6 4 8   7  2  4   2 68",

	"   1  5 2   62   1 2 5 94    27    45 3  "
	" 9 64    37    69 5 8 3   67   9 7  2   ",

	"65  9   49   147   7 3     81   3  2 4   "
	"  6 3  4   51     1 4   374   54   2  38",

	"2 73  8   5 1   4     48  2  58    38   9"
	"   77    45  9  27     2   6 7   3  12 5",

	"  2 6 8   1 3854 9 4   7  1     85  7    "
	"   3  16     2  7   3 1 9843 7   4 2 6  ",

	"71   549  84  1    6     5   27   61  61 "
	"25  17   39   4     1    5  73  518   49",

	"8       6 6  17 2 4372     179    3    7 "
	"5    8    672     6159 5 98  6 7       8",

	"  9  27 6 6   9   5 4 3   2 2   8   6 74 "
	"15 9   9   2 4   1 3 5   6   9 3 62  8  ",

	"  56     19  8     763 4     24    6 8 56"
	"7 9 7    15     8 374     4  19     28  ",

	"4          124  5    13   6 236  9 4  6 7"
	" 5  7 5  936 2   61    1  536          1",

	" 3 8 6 9 96    1 32 7       8  4   9  938"
	"54  5   1  3       9 63 1    25 7 5 9 8 ",

	"712  5 8      7   5     1 72  54 6 13  1 "
	"6  21 8 32  96 4     8   6      5 8  463",

	"1  3249  3 2 8     79     3 3   9    9  7"
	"  1    5   7 4     75     6 4 2  5243  1",

	"      2   18 2   7    4 3 5   4  8 284 51"
	"3 796 1  7   7 6 5    3   9 75   9      ",

	"6 27  5  17 45       2  91 4 61  8   1   "
	"  9   5  94 1 68  5       78 56  1  47 9",

	"69 7       2    7  846   1514 2   6      "
	"     6   5 8493   265  7    4       3 21",

	"  6 48 72  4  5   98 2         5  4 25  1"
	"  89 4  7         9 57   5  2  43 72 6  ",

	"6  1 3 95  7 5  1 1 58     7685       3  "
	" 7       4938     51 4 5  4 3  34 9 8  7",

	"    3      1    623  6 8 7 17 3   2  5 18"
	"2 9  2   4 81 1 9 3  686    5      4    ",

	"  2  8   98  2 6   546     1  5  7 6 3 1 "
	"6 4 5 9  4  2     795   5 1  68   4  3  ",

	"9    6      2  5 6 6  75 84   7 2859     "
	"    8519 4   41 68  2 2 8  7      1    8",

	"7  3   8  2 5  46 6  9 8  13  4   2  7 1 "
	"2 3  1   5  81  8 9  6 65  3 4  9   6  7",

	"1    86 3 4        3216  5 7   19842     "
	"    28345   7 9  8573        9 3 67    1",

	" 72  3     9 6   25     38 46 1  2   5  4"
	"  7   1  7 54 34     16   1 7     3  86 ",

	"9   2    5 7  8  1  81  75  9      3  597"
	"14  4      1  89  43  3  8  1 2    5   8",

	"    6978  3 2   56     8  3 9  2 8  1 7  "
	" 3 4  3 7  6 7  8     65   2 3  8254    ",

	"        6 62   871 1 5 6   6 8 91    3  5"
	"  6    63 2 8   7 9 3 294   18 1        ",

	"  2 4 5  58  9  14       721    28  9   6"
	"   3  43    731       72  3  98  6 7 2  ",

	"   3 9 8 7   6  2  8   2 97  7 25     4  "
	" 6     41 2  85 2   4  7  5   8 9 8 7   ",

	"     4  63  21  5    3 5 285   2 6   8 74"
	"3 1   2 6   387 6 1    9  37  16  4     ",

	"948 1  6 2  4   7       9  6 93   58 2   "
	"  4 48   13 7  6       1   7  6 9  6 415",

	" 1  6   3     4 1 2    3 97 2  97  6 4   "
	"  2 8  65  4 78 2    5 9 3     3   7  8 ",

	" 29  6 7461  87 2 4            2 6  15   "
	"  47  3 7            8 4 81  3683 6  75 ",
};

int npuzzles = sizeof puzzles / sizeof *puzzles;

