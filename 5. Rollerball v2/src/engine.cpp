#include <algorithm>
#include <random>
#include <iostream>
#include <climits>
// #include <cstring>

// #include<unistd.h> // REMOVE THIS

#include "board.hpp"
#include "engine.hpp"
#include "butils.hpp"

using namespace std;
using namespace chrono;


#define f(i, r) for (int i = 0; i < r; i++)
#define getx2(p) ((p == DEAD) ? (3) : (p & 0x7))
#define gety2(p) ((p == DEAD) ? (3) : ((p) >> 3))
#define now() steady_clock::now()
#define duration(start, end) duration_cast<milliseconds>(end - start)
#define cannot_continue duration(start, now()).count() > timelimit

//===== USEFUL FUNCTIONS AND VARIABLES =====//

std::chrono::steady_clock::time_point start;
static int nummoves = 0;
int timelimit = 0;

// w_rook_1, w_rook_2, w_king, w_bishop, w_knight_1, w_knight_2, w_pawn_1, w_pawn_2, w_pawn_3, w_pawn_4, b_rook_1, b_rook_2, b_king, b_bishop, b_knight_1, b_knight_2, b_pawn_1, b_pawn_2, b_pawn_3, b_pawn_4

int wval[12] = {7, 7, 0, 3, 1, 1, 
                -7, -7, 0, -3, -1, -1};

int wval1[16] = {7, 7, 0, 5, 1, 1, 1, 1,
                 -7, -7, 0, -5, -1, -1, -1, -1};

int wval2[20] = {7, 7, 0, 5, 3, 3, 1, 1, 1, 1,
                 -7, -7, 0, -5, -3, -3, -1, -1, -1, -1};

int SIZE = 3;

/// @brief Piecewise Heuristics
// SEVEN_THREE
float rook_val[49] = {12.0, 11.0, 10.0, 9.0, 8.0, 14.0, 12.0,
                      14.0, 5.0, 4.0, 3.0, 2.0, 5.0, 11.0,
                      8.0, 2.0, 0.0, 0.0, 0.0, 4.0, 10.0,
                      9.0, 3.0, 0.0, 0.0, 0.0, 3.0, 9.0,
                      10.0, 4.0, 0.0, 0.0, 0.0, 2.0, 8.0,
                      11.0, 5.0, 2.0, 3.0, 4.0, 5.0, 14.0,
                      12.0, 14.0, 8.0, 9.0, 10.0, 11.0, 12.0};
// Changed this so that pawn doesn't remain in outer circle
float pawn_val[49] = {2.0, 7.0, 8.0, 9.0, 10.0, 1.0, 1.0,
                      2.0, 7.0, 8.0, 9.0, 10.0, 2.0, 2.0,
                      2.0, 5.0, 0.0, 0.0, 0.0, 3.0, 2.0,
                      2.0, 4.0, 0.0, 0.0, 0.0, 4.0, 2.0,
                      2.0, 3.0, 0.0, 0.0, 0.0, 5.0, 2.0,
                      2.0, 2.0, 10.0, 9.0, 8.0, 7.0,2.0,
                      1.0, 1.0, 10.0, 9.0, 8.0, 7.0, 2.0};

float bishop_val[49] = {1.0, 1.0, 2.0, 7.0, 4.0, 3.0, 1.0,
                        3.0, 3.0, 3.0, 4.0, 3.0, 3.0, 1.0,
                        4.0, 3.0, 0.0, 0.0, 0.0, 3.0, 2.0,
                        7.0, 4.0, 0.0, 0.0, 0.0, 4.0, 7.0,
                        2.0, 3.0, 0.0, 0.0, 0.0, 3.0, 4.0,
                        1.0, 3.0, 3.0, 4.0, 3.0, 3.0, 3.0,
                        1.0, 3.0, 4.0, 7.0, 2.0, 1.0, 1.0};

// EIGHT_FOUR
float rook_val1[64] =  {14.0,  13.0,  12.0, 11.0, 10.0, 9.0,  16.0, 14.0,
                        16.0,  6.0,   5.0,  4.0,  3.0,  2.0,  6.0,  13.0,
                        9.0,   2.0,   0.0,  0.0,  0.0,  0.0,  5.0,  12.0,
                        10.0,  3.0,   0.0,  0.0,  0.0,  0.0,  4.0,  11.0,
                        11.0,  4.0,   0.0,  0.0,  0.0,  0.0,  3.0,  10.0,
                        12.0,  5.0,   0.0,  0.0,  0.0,  0.0,  2.0,  9.0,
                        13.0,  6.0,   2.0,  3.0,  4.0,  5.0,  6.0,  16.0,
                        14.0,  16.0,  9.0,  10.0, 11.0, 12.0, 13.0, 14.0};

float bishop_val1[64] ={1.0,  2.0,  2.0,  2.0,  5.0,  4.0,  3.0,  1.0,
                        3.0,  3.0,  3.0,  3.0,  4.0,  3.0,  3.0,  2.0,
                        4.0,  3.0,  0.0,  0.0,  0.0,  0.0,  3.0,  2.0,
                        5.0,  4.0,  0.0,  0.0,  0.0,  0.0,  3.0,  2.0,
                        2.0,  3.0,  0.0,  0.0,  0.0,  0.0,  4.0,  5.0,
                        2.0,  3.0,  0.0,  0.0,  0.0,  0.0,  3.0,  4.0,
                        2.0,  3.0,  3.0,  4.0,  3.0,  3.0,  3.0,  3.0,
                        1.0,  3.0,  4.0,  5.0,  2.0,  2.0,  2.0,  1.0};

float pawn_val1[64] =  {5.0,  10.0, 11.0, 12.0, 13.0, 14.0,  3.0,  3.0,
                        10.0, 10.0, 11.0, 12.0, 13.0, 14.0,  3.0,  5.0,
                        9.0,  9.0,  0.0,  0.0,  0.0,  0.0,  6.0,   6.0,
                        8.0,  8.0,  0.0,  0.0,  0.0,  0.0,  7.0,   7.0,
                        7.0,  7.0,  0.0,  0.0,  0.0,  0.0,  8.0,   8.0,
                        6.0,  6.0,  0.0,  0.0,  0.0,  0.0,  9.0,   9.0,
                        5.0,  3.0,  14.0, 13.0, 12.0, 11.0, 10.0,  10.0,
                        3.0,  3.0,  14.0, 13.0, 13.0, 11.0, 10.0,  5.0};

// EIGHT_TWO
float rook_val2[64] = {14.0, 13.0, 12.0, 11.0, 10.0, 16.0, 15.0, 14.0,
                       15.0, 7.0, 6.0, 5.0, 4.0, 9.0, 8.0, 13.0,
                       16.0, 8.0, 7.0, 6.0, 5.0, 9.0, 8.0, 12.0,
                       10.0, 3.0, 3.0, 0.0, 0.0, 4.0, 4.0, 11.0,
                       11.0, 4.0, 4.0, 0.0, 0.0, 3.0, 3.0, 10.0,
                       12.0, 8.0, 7.0, 5.0, 6.0, 7.0, 8.0, 16.0,
                       13.0, 8.0, 9.0, 4.0, 5.0, 6.0, 7.0, 15.0,
                       14.0, 15.0, 16.0, 10.0, 11.0, 12.0, 13.0, 14.0};

float pawn_val_w2[64] = {6.0, 7.0, 8.0, 9.0, 10.0, 0.0, 0.0, 0.0,
                        6.0, 7.0, 8.0, 9.0, 10.0, 0.0, 0.0, 0.0,
                        6.0, 7.0, 8.0, 9.0, 10.0, 0.0, 0.0, 0.0,
                        5.0, 5.0, 5.0, 0.0,  0.0, 0.0, 0.0, 0.0,
                        4.0, 4.0, 4.0, 0.0,  0.0, 0.0, 0.0, 0.0,
                        3.0, 3.0, 3.0, 2.0,  1.0, 0.0, 0.0, 0.0,
                        2.0, 2.0, 2.0, 2.0,  1.0, 0.0, 0.0, 0.0,
                        1.0, 1.0, 1.0, 1.0,  1.0, 0.0, 0.0, 0.0};

float pawn_val_b2[64] = {0.0, 0.0, 0.0, 1.0,  1.0, 1.0, 1.0, 1.0,
                        0.0, 0.0, 0.0, 1.0,  2.0, 2.0, 2.0, 2.0,
                        0.0, 0.0, 0.0, 1.0,  2.0, 3.0, 3.0, 3.0,
                        0.0, 0.0, 0.0, 0.0,  0.0, 4.0, 4.0, 4.0,
                        0.0, 0.0, 0.0, 0.0,  0.0, 5.0, 5.0, 5.0,
                        0.0, 0.0, 0.0, 10.0, 9.0, 8.0, 7.0, 6.0,
                        0.0, 0.0, 0.0, 10.0, 9.0, 8.0, 7.0, 6.0,
                        0.0, 0.0, 0.0, 10.0, 9.0, 8.0, 7.0, 6.0};

float bishop_val2[64] = {0.0, 4.0, 0.0, 7.0, 0.0, 7.0, 0.0, 1.0,
                         3.0, 0.0, 5.0, 0.0, 3.0, 0.0, 1.0, 0.0,
                         0.0, 6.0, 0.0, 4.0, 0.0, 2.0, 0.0, 1.0,
                         7.0, 0.0, 5.0, 0.0, 0.0, 0.0, 4.0, 0.0,
                         0.0, 6.0, 0.0, 0.0, 0.0, 2.0, 0.0, 1.0,
                         1.0, 0.0, 3.0, 4.0, 3.0, 0.0, 3.0, 0.0,
                         0.0, 1.0, 0.0, 3.0, 0.0, 5.0, 0.0, 3.0,
                         1.0, 0.0, 7.0, 0.0, 7.0, 0.0, 4.0, 0.0};

float knight_val2[64] = {2.0, 3.0, 4.0, 4.0, 4.0, 4.0, 3.0, 2.0,
                         3.0, 4.0, 5.0, 5.0, 5.0, 5.0, 4.0, 3.0,
                         4.0, 5.0, 6.0, 7.0, 7.0, 6.0, 5.0, 4.0,
                         4.0, 5.0, 7.0, 0.0, 0.0, 7.0, 5.0, 4.0,
                         4.0, 5.0, 7.0, 0.0, 0.0, 7.0, 5.0, 4.0,
                         4.0, 5.0, 6.0, 7.0, 7.0, 6.0, 5.0, 4.0,
                         3.0, 4.0, 5.0, 5.0, 5.0, 5.0, 4.0, 3.0,
                         2.0, 3.0, 4.0, 4.0, 4.0, 4.0, 3.0, 2.0};


int dist(U16 move)
{
    int x0 = getx(getp0(move));
    int y0 = gety(getp0(move));
    int x1 = getx(getp1(move));
    int y1 = gety(getp1(move));
    return abs(x1 - x0) + abs(y1 - y0);
}

int man_dist(U8 pos1, U8 pos2)
{
    int x0 = getx(pos1);
    int y0 = gety(pos1);
    int x1 = getx(pos2);
    int y1 = gety(pos2);
    return abs(x1 - x0) + abs(y1 - y0);
}

bool g(U16 move1, U16 move2) { return dist(move1) > dist(move2); }

int endgame(Board *b, PlayerColor original)
{
    int blackdeadcount = 0;
    int whitedeadcount = 0;
    U8 pieces[20];
    int numpieces;

    if (b->data.board_type == SEVEN_THREE)
    {
        pieces[0]=b->data.w_rook_1,
        pieces[1]=b->data.w_rook_2,
        pieces[2]=b->data.w_king,
        pieces[3]=b->data.w_bishop,
        pieces[4]=b->data.w_pawn_1,
        pieces[5]=b->data.w_pawn_2,
        pieces[6]=b->data.b_rook_1,
        pieces[7]=b->data.b_rook_2,
        pieces[8]=b->data.b_king,
        pieces[9]=b->data.b_bishop,
        pieces[10]=b->data.b_pawn_1,
        pieces[11]=b->data.b_pawn_2;
        numpieces = 12;
    }
    else if (b->data.board_type == EIGHT_FOUR)
    {
        pieces[0]=b->data.w_rook_1,
        pieces[1]=b->data.w_rook_2,
        pieces[2]=b->data.w_king,
        pieces[3]=b->data.w_bishop,
        pieces[4]=b->data.w_pawn_1,
        pieces[5]=b->data.w_pawn_2,
        pieces[6]=b->data.w_pawn_3,
        pieces[7]=b->data.w_pawn_4,
        pieces[8]=b->data.b_rook_1,
        pieces[9]=b->data.b_rook_2,
        pieces[10]=b->data.b_king,
        pieces[11]=b->data.b_bishop,
        pieces[12]=b->data.b_pawn_1,
        pieces[13]=b->data.b_pawn_2,
        pieces[14]=b->data.b_pawn_3,
        pieces[15]=b->data.b_pawn_4;
        numpieces = 16;
    }
    else
    {
        pieces[0]=b->data.w_rook_1,
        pieces[1]=b->data.w_rook_2,
        pieces[2]=b->data.w_king,
        pieces[3]=b->data.w_bishop,
        pieces[4]=b->data.w_knight_1,
        pieces[5]=b->data.w_knight_2,
        pieces[6]=b->data.w_pawn_1,
        pieces[7]=b->data.w_pawn_2,
        pieces[8]=b->data.w_pawn_3,
        pieces[9]=b->data.w_pawn_4,
        pieces[10]=b->data.b_rook_1,
        pieces[11]=b->data.b_rook_2,
        pieces[12]=b->data.b_king,
        pieces[13]=b->data.b_bishop,
        pieces[14]=b->data.b_knight_1,
        pieces[15]=b->data.b_knight_2,
        pieces[16]=b->data.b_pawn_1,
        pieces[17]=b->data.b_pawn_2,
        pieces[18]=b->data.b_pawn_3,
        pieces[19]=b->data.b_pawn_4;
        numpieces = 20;
    }
    f(i, numpieces)
    {
        if (pieces[i] == DEAD)
        {
            if (i < numpieces / 2)
                whitedeadcount += 1;
            else
                blackdeadcount += 1;
        }
    }
    return (blackdeadcount >= 4 or whitedeadcount >= 4);
}

//===== HEURISTIC DEFINITION BEGINS HERE =====//

int legal_moves(Board *b, PlayerColor original)
{
    if (original == b->data.player_to_play)
    {
        if (b->in_check())
            if ((b->get_legal_moves()).size() == 0)
                return INT_MIN / 20;
        return 0;
    }
    else
    {
        if (b->in_check())
            if ((b->get_legal_moves()).size() == 0)
                return INT_MAX / 20;
        return 0;
    }
}

int h_board_val(Board *b, PlayerColor original)
{
    int numpieces;
    int* cur_wval;
    U8 pieces[20];

    if (b->data.board_type == SEVEN_THREE)
    {
        
        pieces[0]=b->data.w_rook_1,
        pieces[1]=b->data.w_rook_2,
        pieces[2]=b->data.w_king,
        pieces[3]=b->data.w_bishop,
        pieces[4]=b->data.w_pawn_1,
        pieces[5]=b->data.w_pawn_2,
        pieces[6]=b->data.b_rook_1,
        pieces[7]=b->data.b_rook_2,
        pieces[8]=b->data.b_king,
        pieces[9]=b->data.b_bishop,
        pieces[10]=b->data.b_pawn_1,
        pieces[11]=b->data.b_pawn_2;
        numpieces = 12;
        cur_wval=wval;
    }
    else if (b->data.board_type == EIGHT_FOUR)
    {

        pieces[0]=b->data.w_rook_1,
        pieces[1]=b->data.w_rook_2,
        pieces[2]=b->data.w_king,
        pieces[3]=b->data.w_bishop,
        pieces[4]=b->data.w_pawn_1,
        pieces[5]=b->data.w_pawn_2,
        pieces[6]=b->data.w_pawn_3,
        pieces[7]=b->data.w_pawn_4,
        pieces[8]=b->data.b_rook_1,
        pieces[9]=b->data.b_rook_2,
        pieces[10]=b->data.b_king,
        pieces[11]=b->data.b_bishop,
        pieces[12]=b->data.b_pawn_1,
        pieces[13]=b->data.b_pawn_2,
        pieces[14]=b->data.b_pawn_3,
        pieces[15]=b->data.b_pawn_4;
        numpieces = 16;
        cur_wval=wval1;
    }
    else
    {
        pieces[0]=b->data.w_rook_1,
        pieces[1]=b->data.w_rook_2,
        pieces[2]=b->data.w_king,
        pieces[3]=b->data.w_bishop,
        pieces[4]=b->data.w_knight_1,
        pieces[5]=b->data.w_knight_2,
        pieces[6]=b->data.w_pawn_1,
        pieces[7]=b->data.w_pawn_2,
        pieces[8]=b->data.w_pawn_3,
        pieces[9]=b->data.w_pawn_4,
        pieces[10]=b->data.b_rook_1,
        pieces[11]=b->data.b_rook_2,
        pieces[12]=b->data.b_king,
        pieces[13]=b->data.b_bishop,
        pieces[14]=b->data.b_knight_1,
        pieces[15]=b->data.b_knight_2,
        pieces[16]=b->data.b_pawn_1,
        pieces[17]=b->data.b_pawn_2,
        pieces[18]=b->data.b_pawn_3,
        pieces[19]=b->data.b_pawn_4;
        numpieces = 20;
        cur_wval=wval2;
    }
    int sum = 0;
    f(i, numpieces)
    {
        if (pieces[i] != DEAD)
            sum += ((original == WHITE) ? cur_wval[i] : -1 * cur_wval[i]);
    }
    return sum;
    
}

int h_check_my(Board *b, PlayerColor original)
{
    // My King in check
    return (b->in_check() and b->data.player_to_play == original);
}

int h_check_op(Board *b, PlayerColor original)
{
    // Op King in check
    return h_check_my(b, ((original == WHITE) ? BLACK : WHITE));
}

int check_dead(int pos){
    if (pos == DEAD) {
        return pos(3,3);
    }
    else{
        return pos;
    }
}


pair<int, std::string> eval_state(Board *b, PlayerColor original, Engine *e)
{
    // remember that in some cases a checkmate may be better than losing pieces!
    // Note that the rook heuristics are negative for me, because the more pieces in its file, the worse it is!
    // Note that the pawn heuristics are negative for me, because the farther I am the worse it is!
    // Flipped the rooks now
    if (b->data.board_type == SEVEN_THREE)
    {
        int my_wsrook_x, op_wsrook_x;
        int my_wsrook_y, op_wsrook_y;
        int my_bsrook_y, op_bsrook_y;
        int my_bsrook_x, op_bsrook_x;
        int my_bishop_x, op_bishop_x;
        int my_bishop_y, op_bishop_y;
        int my_wpawn_x, op_wpawn_x;
        int my_wpawn_y, op_wpawn_y;
        int my_bpawn_x, op_bpawn_x;
        int my_bpawn_y, op_bpawn_y;
        if (original == WHITE)
        {
            my_wsrook_x = getx2(b->data.w_rook_1);
            my_wsrook_y = gety2(b->data.w_rook_1);
            my_bsrook_x = getx2(b->data.w_rook_2);
            my_bsrook_y = gety2(b->data.w_rook_2);
            my_bishop_x = getx2(b->data.w_bishop);
            my_bishop_y = gety2(b->data.w_bishop);
            my_wpawn_x = getx2(b->data.w_pawn_1);
            my_wpawn_y = gety2(b->data.w_pawn_1);
            my_bpawn_x = getx2(b->data.w_pawn_2);
            my_bpawn_y = gety2(b->data.w_pawn_2);
            op_wsrook_x = getx2(b->data.b_rook_1);
            op_wsrook_y = gety2(b->data.b_rook_1);
            op_bsrook_x = getx2(b->data.b_rook_2);
            op_bsrook_y = gety2(b->data.b_rook_2);
            op_bishop_x = getx2(b->data.b_bishop);
            op_bishop_y = gety2(b->data.b_bishop);
            op_wpawn_x = getx2(b->data.b_pawn_1);
            op_wpawn_y = gety2(b->data.b_pawn_1);
            op_bpawn_x = getx2(b->data.b_pawn_2);
            op_bpawn_y = gety2(b->data.b_pawn_2);
        }
        else
        {
            my_wsrook_x = getx2(b->data.b_rook_1);
            my_wsrook_y = gety2(b->data.b_rook_1);
            my_bsrook_y = gety2(b->data.b_rook_2);
            my_bsrook_x = getx2(b->data.b_rook_2);
            my_bishop_x = getx2(b->data.b_bishop);
            my_bishop_y = gety2(b->data.b_bishop);
            my_wpawn_x = getx2(b->data.b_pawn_1);
            my_wpawn_y = gety2(b->data.b_pawn_1);
            my_bpawn_x = getx2(b->data.b_pawn_2);
            my_bpawn_y = gety2(b->data.b_pawn_2);
            op_wsrook_x = getx2(b->data.w_rook_1);
            op_wsrook_y = gety2(b->data.w_rook_1);
            op_bsrook_y = gety2(b->data.w_rook_2);
            op_bsrook_x = getx2(b->data.w_rook_2);
            op_bishop_x = getx2(b->data.w_bishop);
            op_bishop_y = gety2(b->data.w_bishop);
            op_wpawn_x = getx2(b->data.w_pawn_1);
            op_wpawn_y = gety2(b->data.w_pawn_1);
            op_bpawn_x = getx2(b->data.w_pawn_2);
            op_bpawn_y = gety2(b->data.w_pawn_2);
        }

        int rook_eval = rook_val[(my_wsrook_x)*7 + (my_wsrook_y)] + rook_val[(my_bsrook_x)*7 + (my_bsrook_y)] - rook_val[(op_wsrook_x)*7 + (op_wsrook_y)] - rook_val[(op_bsrook_x)*7 + (op_bsrook_y)];
        int pawn_eval = pawn_val[(my_wpawn_x)*7 + (my_wpawn_y)] + pawn_val[(my_bpawn_x)*7 + (my_bpawn_y)] - (pawn_val[(op_wpawn_x)*7 + (op_wpawn_y)] + pawn_val[(op_bpawn_x)*7 + (op_bpawn_y)]);
        int bishop_eval = bishop_val[(my_bishop_x)*7 + (my_bishop_y)] - (bishop_val[(op_bishop_x)*7 + (op_bishop_y)]);
        int sum = 7 * (rook_eval / 1.0) + 5 * (bishop_eval / 1.0) + pawn_eval;
        int c[] = {-30, 30, 1};

        if (endgame(b, original))
        {
            sum = 7 * (rook_eval / 1.0) + 5 * (bishop_eval / 1.0) + pawn_eval;
        }
        int (*heuristic_functions[])(Board *, PlayerColor) = {h_check_my, h_check_op, legal_moves};
        string st;
        f(i, SIZE)
        {
            int x = heuristic_functions[i](b, original);
            sum += c[i] * x;
            st += to_string(x) + " ";
        }
        sum += 50 * h_board_val(b,original);
        return make_pair(sum, st);
    }
    else if (b->data.board_type == EIGHT_FOUR)
    {
    // First calculating white - black
        int rook_eval = rook_val1[check_dead(b->data.w_rook_1)] + rook_val1[check_dead(b->data.w_rook_2)] 
                        - rook_val1[check_dead(b->data.b_rook_1)] - rook_val1[check_dead(b->data.b_rook_2)];
        int pawn_eval =   pawn_val1[check_dead(b->data.w_pawn_1)] + pawn_val1[check_dead(b->data.w_pawn_2)] 
                        + pawn_val1[check_dead(b->data.w_pawn_3)] + pawn_val1[check_dead(b->data.w_pawn_4)] 
                        - pawn_val1[check_dead(b->data.b_pawn_1)] - pawn_val1[check_dead(b->data.b_pawn_2)] 
                        - pawn_val1[check_dead(b->data.b_pawn_3)] - pawn_val1[check_dead(b->data.b_pawn_4)];
        int bishop_eval = bishop_val1[check_dead(b->data.w_bishop)] - bishop_val1[check_dead(b->data.b_bishop)];
        // Check if me or op
        rook_eval = ((original==WHITE)? 1:-1)*rook_eval;
        pawn_eval = ((original==WHITE)? 1:-1)*pawn_eval;
        bishop_eval = ((original==WHITE)? 1:-1)*bishop_eval;

        int sum = 7*(rook_eval/1.0)+5*(bishop_eval/1.0)+pawn_eval;
        
        int c[] = {-30, 30, 1};

        if (endgame(b, original))
        {
            sum = 7 * (rook_eval / 1.0) + 5 * (bishop_eval / 1.0) + pawn_eval;
        }

        int (*heuristic_functions[])(Board *, PlayerColor) = {h_check_my, h_check_op, legal_moves};
        string st;
        f(i, SIZE)
        {
            int x = heuristic_functions[i](b, original);
            sum += c[i] * x;
            st += to_string(x) + " ";
        }
        sum += 70 * h_board_val(b,original);
        return make_pair(sum, st);
    }
    else
    {
        // First calculating white - black
        int rook_eval = rook_val2[check_dead(b->data.w_rook_1)] + rook_val2[check_dead(b->data.w_rook_2)] 
                      - rook_val2[check_dead(b->data.b_rook_1)] - rook_val2[check_dead(b->data.b_rook_2)];
        int pawn_eval =   pawn_val_w2[check_dead(b->data.w_pawn_1)] + pawn_val_w2[check_dead(b->data.w_pawn_2)] 
                        + pawn_val_w2[check_dead(b->data.w_pawn_3)] + pawn_val_w2[check_dead(b->data.w_pawn_4)] 
                        - pawn_val_b2[check_dead(b->data.b_pawn_1)] - pawn_val_b2[check_dead(b->data.b_pawn_2)] 
                        - pawn_val_b2[check_dead(b->data.b_pawn_3)] - pawn_val_b2[check_dead(b->data.b_pawn_4)];
        int bishop_eval = bishop_val2[check_dead(b->data.w_bishop)] - bishop_val2[check_dead(b->data.b_bishop)];
        int knight_eval = knight_val2[check_dead(b->data.w_knight_1)] + knight_val2[check_dead(b->data.w_knight_2)]
                        - knight_val2[check_dead(b->data.b_knight_1)]-knight_val2[check_dead(b->data.b_knight_2)];
        // Check if me or op
        rook_eval   = ((original==WHITE)? 1:-1)*rook_eval;
        pawn_eval   = ((original==WHITE)? 1:-1)*pawn_eval;
        bishop_eval = ((original==WHITE)? 1:-1)*bishop_eval;
        knight_eval = ((original==WHITE)? 1:-1)*knight_eval;

        int sum = 7 * (rook_eval / 5.0) + 5 * (bishop_eval / 3.0) + 3 * (knight_eval / 1.0) + pawn_eval;
        
        int c[] = {-30, 30, 1};

        if (endgame(b, original))
        {
            sum = 7 * (rook_eval / 1.0) + 5 * (bishop_eval / 1.0) + 5 * (knight_eval / 1.0) + pawn_eval;
        }

        int (*heuristic_functions[])(Board *, PlayerColor) = {h_check_my, h_check_op, legal_moves};
        string st;
        f(i, SIZE)
        {
            int x = heuristic_functions[i](b, original);
            sum += c[i] * x;
            st += to_string(x) + " ";
        }
        sum += 100 * h_board_val(b,original);
        return make_pair(sum, st);
    }
}

string pieces_to_string(const Board &b){
    int pieces[20];
    int numpieces;
    string s="";
    if (b.data.board_type == SEVEN_THREE)
    {
        pieces[0]=b.data.w_rook_1,
        pieces[1]=b.data.w_rook_2,
        pieces[2]=b.data.w_king,
        pieces[3]=b.data.w_bishop,
        pieces[4]=b.data.w_pawn_1,
        pieces[5]=b.data.w_pawn_2,
        pieces[6]=b.data.b_rook_1,
        pieces[7]=b.data.b_rook_2,
        pieces[8]=b.data.b_king,
        pieces[9]=b.data.b_bishop,
        pieces[10]=b.data.b_pawn_1,
        pieces[11]=b.data.b_pawn_2;
        numpieces = 12;
    }
    else if (b.data.board_type == EIGHT_FOUR)
    {
        pieces[0]=b.data.w_rook_1,
        pieces[1]=b.data.w_rook_2,
        pieces[2]=b.data.w_king,
        pieces[3]=b.data.w_bishop,
        pieces[4]=b.data.w_pawn_1,
        pieces[5]=b.data.w_pawn_2,
        pieces[6]=b.data.w_pawn_3,
        pieces[7]=b.data.w_pawn_4,
        pieces[8]=b.data.b_rook_1,
        pieces[9]=b.data.b_rook_2,
        pieces[10]=b.data.b_king,
        pieces[11]=b.data.b_bishop,
        pieces[12]=b.data.b_pawn_1,
        pieces[13]=b.data.b_pawn_2,
        pieces[14]=b.data.b_pawn_3,
        pieces[15]=b.data.b_pawn_4;
        numpieces = 16;
    }
    else
    {
        pieces[0]=b.data.w_rook_1,
        pieces[1]=b.data.w_rook_2,
        pieces[2]=b.data.w_king,
        pieces[3]=b.data.w_bishop,
        pieces[4]=b.data.w_knight_1,
        pieces[5]=b.data.w_knight_2,
        pieces[6]=b.data.w_pawn_1,
        pieces[7]=b.data.w_pawn_2,
        pieces[8]=b.data.w_pawn_3,
        pieces[9]=b.data.w_pawn_4,
        pieces[10]=b.data.b_rook_1,
        pieces[11]=b.data.b_rook_2,
        pieces[12]=b.data.b_king,
        pieces[13]=b.data.b_bishop,
        pieces[14]=b.data.b_knight_1,
        pieces[15]=b.data.b_knight_2,
        pieces[16]=b.data.b_pawn_1,
        pieces[17]=b.data.b_pawn_2,
        pieces[18]=b.data.b_pawn_3,
        pieces[19]=b.data.b_pawn_4;
        numpieces = 20;
    }
    s+="[";
    f(i,numpieces){
            auto position = check_dead(pieces[i]);
            s+= "("+to_string(getx(position)) + ","+ to_string(gety(position)) + "), ";
    }
    s+="]";
    return s;
}


//===== MAIN ALGORITHM BEGINS HERE =====//

pair<int, std::string> min_move(Board *b, int count, int flag, int alpha, int beta, PlayerColor original, Engine *e)
{
    std::string bestp;
    if (count < -1)
        return eval_state(b, original,e);
    if (count < 0)
    {
        // Quiscence for check did not test yet
        if (b->in_check() && b->data.player_to_play == original)
        {
            auto moveset = b->get_legal_moves();
            int min_val = INT_MAX;
            for (auto move : moveset)
            {
                Board b2 = Board(*b);
                b2.do_move_(move);
                if (cannot_continue)
                    return make_pair(INT_MIN, "");
                auto t = min_move(&b2, count - 1, 1, alpha, beta, original, e);
                if (cannot_continue)
                    return make_pair(INT_MIN, "");
                if (min_val > get<0>(t))
                {
                    min_val = get<0>(t);
                    bestp = move_to_str(move) + get<1>(t);
                }
                beta = min(beta, min_val);
                if (alpha >= beta)
                {
                    return make_pair(min_val, bestp);
                }
            }
            return make_pair(min_val, bestp);
        }
        else if (b->in_check())
        {
            auto moveset = b->get_legal_moves();
            int max_val = INT_MIN;
            for (auto move : moveset)
            {
                Board b2 = Board(*b);
                b2.do_move_(move);
                if (cannot_continue)
                    return make_pair(INT_MAX, "");
                auto t = min_move(&b2, count - 1, 0, alpha, beta, original, e);
                if (cannot_continue)
                    return make_pair(INT_MAX, "");
                if (max_val > get<0>(t))
                {
                    max_val = get<0>(t);
                    bestp = move_to_str(move) + get<1>(t);
                }
                alpha = max(alpha, max_val);
                if (alpha >= beta)
                {
                    return make_pair(max_val, bestp);
                }
            }
            return make_pair(max_val, bestp);
        }
        auto x = eval_state(b, original,e);
        return make_pair(x.first, x.second);
    }
    auto moveset = b->get_legal_moves();
    vector<U16> moves;
    for (auto move : moveset)
        moves.push_back(move);
    if (moveset.size() == 0)
    {
        auto x = eval_state(b, original,e);
        return make_pair(x.first, x.second);
    }
    else if (flag == 1)
    {
        int max_val = INT32_MIN;
        for (auto move : moves)
        {
            Board b2 = Board(*b);
            b2.do_move_(move);
            if (cannot_continue)
                return make_pair(INT_MAX, "");
            auto t = min_move(&b2, count - 1, 0, alpha, beta, original, e);
            if (cannot_continue)
                return make_pair(INT_MAX, "");
            if (max_val < get<0>(t))
            {
                max_val = get<0>(t);
                bestp = move_to_str(move) + get<1>(t);
            }
            alpha = max(alpha, max_val);
            if (alpha >= beta)
                return make_pair(max_val, bestp);
        }
        return make_pair(max_val, bestp);
    }
    else
    {
        int min_val = INT32_MAX;
        for (auto move : moves)
        {
            Board b2 = Board(*b);
            b2.do_move_(move);
            if (cannot_continue)
                return make_pair(INT_MIN, "");
            auto t = min_move(&b2, count - 1, 1, alpha, beta, original, e);
            if (min_val > get<0>(t))
            {
                min_val = get<0>(t);
                bestp = move_to_str(move) + get<1>(t);
            }
            beta = min(beta, min_val);
            if (alpha >= beta)
            {
                return make_pair(min_val, bestp);
            }
        }
        return make_pair(min_val, bestp);
    }
}

pair<U16, int> begin_search(Board *b_1, int count, Engine *e, PlayerColor original, int* cur_max_val, U16 cur_best_move)
{
    auto moveset = b_1->get_legal_moves();
    vector<U16> moves;
    std::string bestp;
    for (auto move : moveset)
        moves.push_back(move);
    sort(moves.begin(), moves.end(), g);
    if (moveset.size() == 0)
        {
            // cout<<"LOSE"<<endl;
            return make_pair(0, INT_MIN);
        }
    else
    {
        int max_val = INT32_MIN;
        U16 best_move = 0;
        int alpha = INT32_MIN;
        int beta = INT32_MAX;
        for (auto move : moves)
        {
            Board b = Board(*b_1);
            b.do_move_(move);
            auto x = min_move(&b, count - 1, 0, alpha, beta, original, e);
            // Important otherwise leaving better moves due to INT_MIN
            if (cannot_continue)
                    return make_pair(best_move, max_val);
            cout << move_to_str(move) << " " << x.first << " " << x.second <<" end"<< endl;
            if (max_val < x.first)
            {
                max_val = x.first;
                bestp = x.second;
                best_move = move;
                if (cannot_continue)
                    return make_pair(best_move, max_val);
            }
            if (move==cur_best_move){
                *cur_max_val = x.first;
            }
            if (x.second.length() < 20 && x.first > 10000)
                return make_pair(move, max_val);

            alpha = max(alpha, max_val);
            if (alpha >= beta or cannot_continue)
                return make_pair(best_move, max_val);
        }
        return make_pair(best_move, max_val);
    }
}

void Engine::find_best_move(const Board &b)
{   
    if ((this->time_left.count())<500){
        this->best_move=0;
        return;
    }
    nummoves++;
    // cout<<nummoves<<endl;
    using namespace std::chrono;
    start = now();
    int depth = 2;
    if (b.data.board_type == SEVEN_THREE)
    {
        {

            if (b.data.player_to_play==WHITE)
                {
                    auto it = this->openingdb_73_w.find(pieces_to_string(b));
                    if (it!=this->openingdb_73_w.end()){
                        this->best_move = str_to_move(it->second);
                        return;
                    }
                }
            else
                {
                    auto it = this->openingdb_73_b.find(pieces_to_string(b));
                    if (it!=this->openingdb_73_b.end()){
                        this->best_move = str_to_move(it->second);
                        return;
                    }
                }
        }
        if((this->time_left).count()>30000)
        {
            timelimit=2500;
        }
        else if ((this->time_left).count()>10000)
        {
            timelimit=2000;
        }
        else
        {
            timelimit=this->time_left/10;
        }
        depth=2;
    }
    else if (b.data.board_type==EIGHT_FOUR)
    {
        {

            if (b.data.player_to_play==WHITE)
                {
                    auto it = this->openingdb_84_w.find(pieces_to_string(b));
                    if (it!=this->openingdb_84_w.end()){
                        this->best_move = str_to_move(it->second);
                        return;
                    }
                }
            else
                {
                    auto it = this->openingdb_84_b.find(pieces_to_string(b));
                    if (it!=this->openingdb_84_b.end()){
                        this->best_move = str_to_move(it->second);
                        return;
                    }
                }
        }
        if((this->time_left).count()>30000)
            {
                timelimit=3000;
            }
        else if ((this->time_left).count()>10000)
        {
            timelimit=2000;
        }
        else
        {
            timelimit=this->time_left/10;
        }
    }
    else
    {
        {

            if (b.data.player_to_play==WHITE)
                {
                    auto it = this->openingdb_82_w.find(pieces_to_string(b));
                    if (it!=this->openingdb_82_w.end()){
                        this->best_move = str_to_move(it->second);
                        return;
                    }
                }
            else
                {
                    auto it = this->openingdb_82_b.find(pieces_to_string(b));
                    if (it!=this->openingdb_82_b.end()){
                        this->best_move = str_to_move(it->second);
                        return;
                    }
                }
        }
        if((this->time_left).count()>30000)
            {
                timelimit=4000;
            }
        else if ((this->time_left).count()>10000)
        {
            timelimit=2000;
        }
        else
        {
            timelimit=this->time_left/10;
        }
        depth=2;
    }

    Board copy_b = Board(b);
    PlayerColor original = b.data.player_to_play;
    int initial_depth = depth;
    int cur_max_val = INT_MIN;
    U16 cur_best_move=0;
    while ((duration(start, now())).count() < timelimit)
    {
        auto move = begin_search(&copy_b, depth, this, original,&cur_max_val, cur_best_move);
        // if (move.second==INT_MIN) {sleep(20);break;}
        cout << "Best move at depth " << depth << ": " << move_to_str(move.first) << " with value " << move.second << endl;
        if (move.first==0 && depth == initial_depth) {
            this->best_move=0;
            return; 
        }
        if (cannot_continue)
        {
            if (cur_max_val < move.second)
            {
                this->best_move = move.first;
                cur_best_move = move.first;
                cur_max_val = move.second;
            }
        }
        else
        {
            this->best_move = move.first;
            cur_best_move = move.first;
            cur_max_val = move.second;
        }
        if (cur_max_val >= 100000000)
            break;
        depth++;
    }

    // std::cout << std::endl
    //           << "Time taken (chrono): " << ((duration(start, now())).count()) << "milliseconds" << std::endl
    //           << std::endl;
}
