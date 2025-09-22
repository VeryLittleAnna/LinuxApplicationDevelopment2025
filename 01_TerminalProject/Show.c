#include <stdio.h>
#include <ncurses.h>
#include <locale.h>
#include <stdlib.h>
#include <sys/param.h>
#include <string.h>

#define DX 7
#define DY 3
#define BUFFER_SIZE 256

#ifndef min
#define min(a, b) (((int) a) < ((int) b) ? ((int) a) : ((int) b))
#endif

/*comments to display 
very_long_line_1_very_long_line_2_very_long_line_3_very_long_line_4_very_long_line_5_very_long_line_6_very_long_line_7_very_long_line_8

|\---/|
| o_o |
 \_^_/

*/


void draw_border(WINDOW *screen, const char *filename) {
    box(screen, 0, 0);
    int max_len = min(getmaxx(screen) - 4, strlen(filename));
    mvwaddnstr(screen, 0, 2, filename, max_len);
    wrefresh(screen);
}

void shrink_str(char *str, int width) {
    int str_cur_len = strnlen(str, BUFFER_SIZE);
    if (str_cur_len > 0 && str[str_cur_len - 1] == '\n') {
        str[str_cur_len - 1] = '\0';
        str_cur_len--;
    }
    int len = min(str_cur_len, width);
    str[len] = '\0';
}


int display_first_page(WINDOW *win, FILE *file, int height, int width, char *buffer) {
    scrollok(win, TRUE);
    int line_cnt = 0;
    while (line_cnt < height && fgets(buffer, BUFFER_SIZE, file)) {
        shrink_str(buffer, width);
        wprintw(win, "%s\n", buffer);
        line_cnt++;
    }
    wrefresh(win);
    return line_cnt;
}

void update(WINDOW *win, FILE *file, int width, char *buffer) {
    if (!fgets(buffer, BUFFER_SIZE, file)) {
        return;
    }
    shrink_str(buffer, width);
    wprintw(win, "%s\n", buffer);
    wrefresh(win);
}



int main(int argc, char *argv[]){
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }
    WINDOW *screen, *win;

    setlocale(LC_ALL, "");
    initscr();
    noecho();
    cbreak();
    refresh();

    screen = newwin(LINES - 2*DY, COLS - 2*DX, DY, DX);
    draw_border(screen, argv[1]);

    win = newwin(LINES - 2*DY - 2, COLS - 2*DX-2, DY+1, DX+1);
    int height = getmaxy(win);
    int width = getmaxx(win);

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        delwin(win);
        delwin(screen);
        endwin();
        fprintf(stderr, "Error opening file: %s\n", argv[1]);
        return 1;
    }

    int line_cnt = 0;
    char buffer[BUFFER_SIZE] = {0};
    int c = 0;

    line_cnt = display_first_page(win, file, height, width, buffer);
    keypad(win, TRUE);
    while((c = wgetch(win)) != 27) {
        if (c == 32) {
            update(win, file, width, buffer);
            line_cnt++;
        }
    }

    delwin(win);
    delwin(screen);
    endwin();
    fclose(file);
    return 0;
}
