#include <ncurses.h>
#include <unistd.h>

#define  TIME_DELAY 90 // game speed.

const int board_half_size = 3; // size of board.
const char board_char[3] = "..";
const char ball_char[2] = "@";
const char wall_char[2] = "=";

int board_left_pos = 0, board_right_pos = 0; //default board position.

enum direction {TR, BR, BL, TL} direct = BR; //default direction.
int ball_y = 10, ball_x = 10; //default ball position.

int scr_y, scr_x;

void draw_wall(){
    attron(COLOR_PAIR(2));
    for(int i = 0; i < scr_x; i++){
        mvprintw(0, i, wall_char);
        mvprintw(scr_y-1, i, wall_char);
    }
    attroff(COLOR_PAIR(2));
}

void mv_board(){
    int lpos = (scr_y/2) + board_left_pos;
    int rpos = (scr_y/2) + board_right_pos;

    if(lpos < board_half_size + 1)
        lpos = board_half_size + 1;
    else if(lpos > scr_y - board_half_size - 2)
        lpos = scr_y - board_half_size - 2;

    if(rpos < board_half_size + 1)
        rpos = board_half_size + 1;
    else if(rpos > scr_y - board_half_size - 2)
        rpos = scr_y - board_half_size - 2;

    board_left_pos = lpos - (scr_y/2);
    board_right_pos = rpos - (scr_y/2);

    int left_x = 0, left_y = lpos;
    int right_x = scr_x-1, right_y = rpos;

    attron(COLOR_PAIR(1));
    for(int i = 0; i <= board_half_size; i++){
        mvprintw(left_y - i, left_x, board_char);
        mvprintw(left_y + i, left_x, board_char);

        mvprintw(right_y - i, right_x - 1, board_char);
        mvprintw(right_y + i, right_x - 1, board_char);
    }
    attroff(COLOR_PAIR(1));
}

void mv_ball(){
    if(ball_x > 3 && ball_x < scr_x - 3){
        if(ball_y == 1){
            if(direct == TR)
                direct = BR;
            else if(direct == TL)
                direct = BL;
        }
        else if(ball_y == scr_y - 2){
            if(direct == BR)
                direct = TR;
            else if(direct == BL)
                direct = TL;
        }
    }
    else{
        int lpos = (scr_y/2) + board_left_pos;
        int rpos = (scr_y/2) + board_right_pos;
        bool is_hit = FALSE;
        bool is_conner = FALSE;

        switch (direct) {
            case TR:
                if(ball_y >= rpos - board_half_size && ball_y <= rpos + board_half_size)
                    is_hit = TRUE;
                if(ball_y == 1)
                    is_conner = TRUE;
                break;
            case BR:
                if(ball_y >= rpos - board_half_size && ball_y <= rpos + board_half_size)
                    is_hit = TRUE;
                if(ball_y == scr_y - 2)
                    is_conner = TRUE;
                break;
            case TL:
                if(ball_y >= lpos - board_half_size && ball_y <= lpos + board_half_size)
                    is_hit = TRUE;
                if(ball_y == 1)
                    is_conner = TRUE;
                break;
            case BL:
                if(ball_y >= lpos - board_half_size && ball_y <= lpos + board_half_size)
                    is_hit = TRUE;
                if(ball_y == scr_y - 2)
                    is_conner = TRUE;
        }

        if(is_hit){
            switch (direct) {
                case TR:
                    direct = is_conner ? BL:TL;
                    break;
                case BR:
                    direct = is_conner ? TL:BL;
                    break;
                case TL:
                    direct = is_conner ? BR:TR;
                    break;
                case BL:
                    direct = is_conner ? TR:BR;
            }
        }
        else{
            if(direct == TL || direct == BL){
                ball_y = scr_y - 11;
                ball_x = scr_x - 11;
                direct = TL;
            }
            else{
                ball_y = 10;
                ball_x = 10;
                direct = BR;
            }
        }
    }

    switch (direct) {
        case TR:
            ball_y--;
            ball_x++;
            break;
        case BR:
            ball_y++;
            ball_x++;
            break;
        case TL:
            ball_y--;
            ball_x--;
            break;
        case BL:
            ball_y++;
            ball_x--;
    }
    mvprintw(ball_y, ball_x, ball_char);
}

int main(){
    int key_press;

    initscr();
    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    cbreak();

    start_color();
    init_pair(1, COLOR_RED, COLOR_BLUE);
    init_pair(2, COLOR_RED, COLOR_GREEN);

    for(;;){
        key_press = getch();

        clear();

        getmaxyx(stdscr, scr_y, scr_x);

        draw_wall();

        switch(key_press){
            case '\'':
                board_right_pos--;
                break;
            case '/':
                board_right_pos++;
                break;
            case 'a':
                board_left_pos--;
                break;
            case 'z':
                board_left_pos++;
                break;
        }

        mv_board();
        mv_ball();

        wrefresh(stdscr);
        usleep(TIME_DELAY*1000);
    }

    return 0;
}
