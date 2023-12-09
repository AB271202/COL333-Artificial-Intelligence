#include <algorithm>
#include <iostream>
#include <climits>
using namespace std;
#include "board.hpp"

#define f(i, r) for (int i = 0; i < r; i++)

//===== USEFUL FUNCTIONS AND VARIABLES =====//

int wval[12] = {-7, -7, 0, -3, -1, -1, 7, 7, 0, 3, 1, 1}; // Changed rook value from 5 to 7

int SIZE = 7; // Number of heuristics UPDATE TO 7

void disp(int *c)
{
    f(i, SIZE) cout << c[i] << " ";
    cout << endl;
}

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

int endgame(Board *b, PlayerColor original)
{
    // int deadcount = 0;
    // U8 *pieces = &(b->data.b_rook_ws);
    // f(i, 12)
    // {
    //     if (pieces[i] == DEAD)
    //     {
    //         deadcount += 1;
    //     }
    // }
    int blackdeadcount = 0;
    int whitedeadcount = 0;
    U8 *pieces = &(b->data.b_pawn_ws);
    f(i, 12)
    {
        if (pieces[i] == DEAD)
        {
            if (i < 6)
                blackdeadcount += 1;
            else
                whitedeadcount += 1;
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
            if ((b->get_legal_moves()).size()==0)
                return INT_MIN/20;
        return 0;
        // int x = (b->get_legal_moves()).size();
        // if (x == 0 && b->in_check())
        //     return INT_MIN / 20;
        // return x;
    }
    else
    {
        if (b->in_check())
            if ((b->get_legal_moves()).size()==0)
                return INT_MAX/20;
        return 0;
        // int x = (b->get_legal_moves()).size();
        // if (x == 0  && b->in_check())
        //     return INT_MAX / 20;
        // return -1 * x;
    }
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

int h_rook_my_new(Board *b, PlayerColor original)
{
    if (original == WHITE)
    {
        auto bsrook = b->data.w_rook_ws;
        auto wsrook = b->data.w_rook_bs;
        auto wsrook_x = getx(wsrook);
        auto wsrook_y = gety(wsrook);
        auto bsrook_x = getx(bsrook);
        auto bsrook_y = gety(bsrook);
        // Rook bs in outer file

        int piece_count = -1;
        U8 *pieces = &(b->data.b_rook_ws);
        // WHITE SQUARE
        if (wsrook != DEAD)
        {
            // If not on outer ring then return 3
            if (!(wsrook_x == 0 or wsrook_x == 6 or wsrook_y == 0 or wsrook_y == 6))
                piece_count += 3;
            // Check corners
            else if (wsrook_x == 0 and wsrook_y == 0)
            {
                for (int i = 6; i < 12; i++)
                {
                    if (pieces[i] != DEAD)
                    {
                        if (getx(pieces[i]) == 0 or gety(pieces[i]) == 6)
                            piece_count += 1;
                    }
                }
            }
            else if (wsrook_x == 0 and wsrook_y == 6)
            {
                for (int i = 6; i < 12; i++)
                {
                    if (pieces[i] != DEAD)
                    {
                        if (getx(pieces[i]) == 6 or gety(pieces[i]) == 6)
                            piece_count += 1;
                    }
                }
            }
            else if (wsrook_x == 6 and wsrook_y == 6)
            {
                for (int i = 6; i < 12; i++)
                {
                    if (pieces[i] != DEAD)
                    {
                        if (getx(pieces[i]) == 6 or gety(pieces[i]) == 0)
                            piece_count += 1;
                    }
                }
            }
            else if (wsrook_x == 6 and wsrook_y == 0)
            {
                for (int i = 6; i < 12; i++)
                {
                    if (pieces[i] != DEAD)
                    {
                        if (getx(pieces[i]) == 0 or gety(pieces[i]) == 0)
                            piece_count += 1;
                    }
                }
            }
            // Now not corners
            else if (wsrook_x == 0)
            {
                for (int i = 6; i < 12; i++)
                {
                    if (pieces[i] != DEAD)
                    {
                        if (getx(pieces[i]) == 0 or gety(pieces[i]) == 6)
                            piece_count += 1;
                    }
                }
            }
            else if (wsrook_x == 6)
            {
                for (int i = 6; i < 12; i++)
                {
                    if (pieces[i] != DEAD)
                    {
                        if (getx(pieces[i]) == 6 or gety(pieces[i]) == 0)
                            piece_count += 1;
                    }
                }
            }
            else if (wsrook_y == 0)
            {
                for (int i = 6; i < 12; i++)
                {
                    if (pieces[i] != DEAD)
                    {
                        if (getx(pieces[i]) == 0 or gety(pieces[i]) == 0)
                            piece_count += 1;
                    }
                }
            }
            else if (wsrook_y == 6)
            {
                for (int i = 6; i < 12; i++)
                {
                    if (pieces[i] != DEAD)
                    {
                        if (getx(pieces[i]) == 6 or gety(pieces[i]) == 6)
                            piece_count += 1;
                    }
                }
            }
        }
        // BLACKSQUARE
        else if (bsrook != DEAD)
        {

            if (!(bsrook_x == 0 or bsrook_x == 6 or bsrook_y == 0 or bsrook_y == 6))
                piece_count += 3;
            // Check corners
            else if (bsrook_x == 0 and bsrook_y == 0)
            {
                for (int i = 6; i < 12; i++)
                {
                    if (pieces[i] != DEAD)
                    {
                        if (getx(pieces[i]) == 0 or gety(pieces[i]) == 6)
                            piece_count += 1;
                    }
                }
            }
            else if (bsrook_x == 0 and bsrook_y == 6)
            {
                for (int i = 6; i < 12; i++)
                {
                    if (pieces[i] != DEAD)
                    {
                        if (getx(pieces[i]) == 6 or gety(pieces[i]) == 6)
                            piece_count += 1;
                    }
                }
            }
            else if (bsrook_x == 6 and bsrook_y == 6)
            {
                for (int i = 6; i < 12; i++)
                {
                    if (pieces[i] != DEAD)
                    {
                        if (getx(pieces[i]) == 6 or gety(pieces[i]) == 0)
                            piece_count += 1;
                    }
                }
            }
            else if (bsrook_x == 6 and bsrook_y == 0)
            {
                for (int i = 6; i < 12; i++)
                {
                    if (pieces[i] != DEAD)
                    {
                        if (getx(pieces[i]) == 0 or gety(pieces[i]) == 0)
                            piece_count += 1;
                    }
                }
            }
            // Now not corners
            else if (bsrook_x == 0)
            {
                for (int i = 6; i < 12; i++)
                {
                    if (pieces[i] != DEAD)
                    {
                        if (getx(pieces[i]) == 0 or gety(pieces[i]) == 6)
                            piece_count += 1;
                    }
                }
            }
            else if (bsrook_x == 6)
            {
                for (int i = 6; i < 12; i++)
                {
                    if (pieces[i] != DEAD)
                    {
                        if (getx(pieces[i]) == 6 or gety(pieces[i]) == 0)
                            piece_count += 1;
                    }
                }
            }
            else if (bsrook_y == 0)
            {
                for (int i = 6; i < 12; i++)
                {
                    if (pieces[i] != DEAD)
                    {
                        if (getx(pieces[i]) == 0 or gety(pieces[i]) == 0)
                            piece_count += 1;
                    }
                }
            }
            else if (bsrook_y == 6)
            {
                for (int i = 6; i < 12; i++)
                {
                    if (pieces[i] != DEAD)
                    {
                        if (getx(pieces[i]) == 6 or gety(pieces[i]) == 6)
                            piece_count += 1;
                    }
                }
            }
        }
        else
        {
            piece_count = 0;
        }
        return piece_count;
    }
    else
    {
        // I AM BLACK
        auto bsrook = b->data.b_rook_ws;
        auto wsrook = b->data.b_rook_bs;
        auto wsrook_x = getx(wsrook);
        auto wsrook_y = gety(wsrook);
        auto bsrook_x = getx(bsrook);
        auto bsrook_y = gety(bsrook);
        // Rook bs in outer file

        int piece_count = -1;
        U8 *pieces = &(b->data.b_rook_ws);
        // WHITE SQUARE
        if (wsrook != DEAD)
        {
            if (!(wsrook_x == 0 or wsrook_x == 6 or wsrook_y == 0 or wsrook_y == 6))
                piece_count += 3;
            // Check corners
            if (wsrook_x == 0 and wsrook_y == 0)
            {
                f(i, 6)
                {
                    if (pieces[i] != DEAD)
                    {
                        if (getx(pieces[i]) == 0 or gety(pieces[i]) == 6)
                            piece_count += 1;
                    }
                }
            }
            else if (wsrook_x == 0 and wsrook_y == 6)
            {
                f(i, 6)
                {
                    if (pieces[i] != DEAD)
                    {
                        if (getx(pieces[i]) == 6 or gety(pieces[i]) == 6)
                            piece_count += 1;
                    }
                }
            }
            else if (wsrook_x == 6 and wsrook_y == 6)
            {
                f(i, 6)
                {
                    if (pieces[i] != DEAD)
                    {
                        if (getx(pieces[i]) == 6 or gety(pieces[i]) == 0)
                            piece_count += 1;
                    }
                }
            }
            else if (wsrook_x == 6 and wsrook_y == 0)
            {
                f(i, 6)
                {
                    if (pieces[i] != DEAD)
                    {
                        if (getx(pieces[i]) == 0 or gety(pieces[i]) == 0)
                            piece_count += 1;
                    }
                }
            }
            // Now not corners
            else if (wsrook_x == 0)
            {
                f(i, 6)
                {
                    if (pieces[i] != DEAD)
                    {
                        if (getx(pieces[i]) == 0 or gety(pieces[i]) == 6)
                            piece_count += 1;
                    }
                }
            }
            else if (wsrook_x == 6)
            {
                f(i, 6)
                {
                    if (pieces[i] != DEAD)
                    {
                        if (getx(pieces[i]) == 6 or gety(pieces[i]) == 0)
                            piece_count += 1;
                    }
                }
            }
            else if (wsrook_y == 0)
            {
                f(i, 6)
                {
                    if (pieces[i] != DEAD)
                    {
                        if (getx(pieces[i]) == 0 or gety(pieces[i]) == 0)
                            piece_count += 1;
                    }
                }
            }
            else if (wsrook_y == 6)
            {
                f(i, 6)
                {
                    if (pieces[i] != DEAD)
                    {
                        if (getx(pieces[i]) == 6 or gety(pieces[i]) == 6)
                            piece_count += 1;
                    }
                }
            }
        }
        // BLACKSQUARE
        else if (bsrook != DEAD)
        {
            if (!(bsrook_x == 0 or bsrook_x == 6 or bsrook_y == 0 or bsrook_y == 6))
                piece_count += 3;
            // Check corners
            else if (bsrook_x == 0 and bsrook_y == 0)
            {
                f(i, 6)
                {
                    if (pieces[i] != DEAD)
                    {
                        if (getx(pieces[i]) == 0 or gety(pieces[i]) == 6)
                            piece_count += 1;
                    }
                }
            }
            else if (bsrook_x == 0 and bsrook_y == 6)
            {
                f(i, 6)
                {
                    if (pieces[i] != DEAD)
                    {
                        if (getx(pieces[i]) == 6 or gety(pieces[i]) == 6)
                            piece_count += 1;
                    }
                }
            }
            else if (bsrook_x == 6 and bsrook_y == 6)
            {
                f(i, 6)
                {
                    if (pieces[i] != DEAD)
                    {
                        if (getx(pieces[i]) == 6 or gety(pieces[i]) == 0)
                            piece_count += 1;
                    }
                }
            }
            else if (bsrook_x == 6 and bsrook_y == 0)
            {
                f(i, 6)
                {
                    if (pieces[i] != DEAD)
                    {
                        if (getx(pieces[i]) == 0 or gety(pieces[i]) == 0)
                            piece_count += 1;
                    }
                }
            }
            // Now not corners
            else if (bsrook_x == 0)
            {
                f(i, 6)
                {
                    if (pieces[i] != DEAD)
                    {
                        if (getx(pieces[i]) == 0 or gety(pieces[i]) == 6)
                            piece_count += 1;
                    }
                }
            }
            else if (bsrook_x == 6)
            {
                f(i, 6)
                {
                    if (pieces[i] != DEAD)
                    {
                        if (getx(pieces[i]) == 6 or gety(pieces[i]) == 0)
                            piece_count += 1;
                    }
                }
            }
            else if (bsrook_y == 0)
            {
                f(i, 6)
                {
                    if (pieces[i] != DEAD)
                    {
                        if (getx(pieces[i]) == 0 or gety(pieces[i]) == 0)
                            piece_count += 1;
                    }
                }
            }
            else if (bsrook_y == 6)
            {
                f(i, 6)
                {
                    if (pieces[i] != DEAD)
                    {
                        if (getx(pieces[i]) == 6 or gety(pieces[i]) == 6)
                            piece_count += 1;
                    }
                }
            }
        }
        else
        {
            piece_count = 0;
        }
        return piece_count;
    }
}

int h_rook_my(Board *b, PlayerColor original)
{
    // Keep rook in the outer circle and file is empty too!

    if (original == WHITE)
    {

        if (  // Y coordinate
            ( // Rook in outer file
                (gety(b->data.w_rook_bs) == 0 or gety(b->data.w_rook_bs) == 6) or
                (gety(b->data.w_rook_bs) == 0 or gety(b->data.w_rook_bs) == 6)) and // No other pieces
            !(gety(b->data.w_bishop) == 0 or gety(b->data.w_bishop) == 6) and
            !(gety(b->data.w_king) == 0 or gety(b->data.w_king) == 6) and
            !(gety(b->data.w_pawn_bs) == 0 or gety(b->data.w_pawn_bs) == 6) and
            !(gety(b->data.w_pawn_ws) == 0 or gety(b->data.w_pawn_ws) == 6))
        {
            return 1;
        }
        else if ( // X coordinate
            (     // Rook in outer file
                (getx(b->data.w_rook_bs) == 0 or getx(b->data.w_rook_bs) == 6) or
                (getx(b->data.w_rook_bs) == 0 or getx(b->data.w_rook_bs) == 6)) and // No other pieces
            !(getx(b->data.w_bishop) == 0 or getx(b->data.w_bishop) == 6) and
            !(getx(b->data.w_king) == 0 or getx(b->data.w_king) == 6) and
            !(getx(b->data.w_pawn_bs) == 0 or getx(b->data.w_pawn_bs) == 6) and
            !(getx(b->data.w_pawn_ws) == 0 or getx(b->data.w_pawn_ws) == 6))
        {
            return 1;
        }
    }
    else
    {
        if (  // Y coordinate
            ( // Rook in outer file
                (gety(b->data.b_rook_bs) == 0 or gety(b->data.b_rook_bs) == 6) or
                (gety(b->data.b_rook_bs) == 0 or gety(b->data.b_rook_bs) == 6)) and // No other pieces
            !(gety(b->data.b_bishop) == 0 or gety(b->data.b_bishop) == 6) and
            !(gety(b->data.b_king) == 0 or gety(b->data.b_king) == 6) and
            !(gety(b->data.b_pawn_bs) == 0 or gety(b->data.b_pawn_bs) == 6) and
            !(gety(b->data.b_pawn_ws) == 0 or gety(b->data.b_pawn_ws) == 6))
        {
            return 1;
        }
        else if ( // X coordinate
            (     // Rook in outer file
                (getx(b->data.b_rook_bs) == 0 or getx(b->data.b_rook_bs) == 6) or
                (getx(b->data.b_rook_bs) == 0 or getx(b->data.b_rook_bs) == 6)) and // No other pieces
            !(getx(b->data.b_bishop) == 0 or getx(b->data.b_bishop) == 6) and
            !(getx(b->data.b_king) == 0 or getx(b->data.b_king) == 6) and
            !(getx(b->data.b_pawn_bs) == 0 or getx(b->data.b_pawn_bs) == 6) and
            !(getx(b->data.b_pawn_ws) == 0 or getx(b->data.b_pawn_ws) == 6))
        {
            return 1;
        }
    }
    return 0;
}

int h_rook_op(Board *b, PlayerColor original)
{
    return h_rook_my_new(b, ((original == WHITE) ? BLACK : WHITE));
}

int h_pawn_my(Board *b, PlayerColor original)
{
    if (original == WHITE)
    {
        int md_ws=0;
        int md_bs=0;
        
        if (b->data.w_pawn_bs == pos(2,0)) md_bs = man_dist(b->data.w_pawn_bs, pos(4, 5)) + 2;
        else md_bs=man_dist(b->data.w_pawn_bs, pos(4, 5));
        if (b->data.w_pawn_ws == pos(2,1)) md_ws = man_dist(b->data.w_pawn_ws, pos(4, 5)) + 2;
        else md_ws=man_dist(b->data.w_pawn_ws, pos(4, 5));
        
        
        return (((b->data.w_pawn_bs == DEAD) ? 0 : md_bs) + ((b->data.w_pawn_ws == DEAD) ? 0 : md_ws));
    }
    else
    {
        int md_ws=0;
        int md_bs=0;
        
        if (b->data.b_pawn_bs == pos(4,6)) md_bs = man_dist(b->data.b_pawn_bs, pos(2, 1)) + 2;
        else md_bs=man_dist(b->data.b_pawn_bs, pos(2, 1));
        if (b->data.b_pawn_ws == pos(4,5)) md_ws = man_dist(b->data.b_pawn_ws, pos(2, 1)) + 2;
        else md_ws=man_dist(b->data.b_pawn_ws, pos(2, 1));
        return (((b->data.b_pawn_bs == DEAD) ? 0 : md_bs) + ((b->data.b_pawn_ws == DEAD) ? 0 : md_ws));
    }

    /*
   //Ankit's heuristic
    int wbposx = (b->data.w_pawn_bs) % 8;
    int wbposy = (b->data.w_pawn_bs) / 8;
    int wwposx = (b->data.w_pawn_ws) % 8;
    int wwposy = (b->data.w_pawn_ws) / 8;
    int bbposx = (b->data.b_pawn_bs) % 8;
    int bbposy = (b->data.b_pawn_bs) / 8;
    int bwposx = (b->data.b_pawn_ws) % 8;
    int bwposy = (b->data.b_pawn_ws) / 8;
    double dist_to_promw = min(abs(wbposx - 5) + abs(wbposy - 6), abs(wwposx - 5) + abs(wwposy - 6)) + 1.0;
    double dist_to_promb = min(abs(bwposx - 2) + abs(bwposy - 1), abs(bbposx - 2) + abs(bbposy - 1)) + 1.0;
    return (int)(((original == WHITE) ? 1 : -1) * (50.0 / dist_to_promw - 50.0 / dist_to_promb));
    */
}

int h_pawn_op(Board *b, PlayerColor original)
{
    return h_pawn_my(b, ((original == WHITE) ? BLACK : WHITE));
}

int h_pawn(Board *b, PlayerColor original)
{
    return h_pawn_my(b, original);// - h_pawn_op(b,original);
}

int h_board_val(Board *b, PlayerColor original)
{
    U8 *pieces = &(b->data.b_rook_ws);
    int sum = 0;
    f(i, 12)
    {
        if (pieces[i] != DEAD)
        {
            sum += ((original == WHITE) ? wval[i] : -1 * wval[i]);
        }
    }
    return sum;
}

// int eval_state(Board *b, PlayerColor original, int *c) // To change this when running for engine
pair<int, string> eval_state(Board *b, PlayerColor original)
{

    // remember that in some cases a checkmate may be better than losing pieces!
    // Note that the rook heuristics are negative for me, because the more pieces in its file, the worse it is!
    // Note that the pawn heuristics are negative for me, because the farther I am the worse it is!
    // Flipped the rooks now
    int c[] = {50, -30, 30, -10, 0, -2, 1};
    if (endgame(b, original))
    {
        // cout<<endl<<"////////ENDGAME/////////"<<endl;
        c[2] += 10;
        c[5] -= 15;
        c[6] *= 10;
    }
    int (*heuristic_functions[])(Board *, PlayerColor) = {h_board_val, h_check_my, h_check_op, h_rook_my_new, h_rook_op, h_pawn, legal_moves};
    int sum = 0;
    string st;
    f(i, SIZE)
    {
        int x = heuristic_functions[i](b, original);
        sum += c[i] * x;
        st += to_string(x)+" ";
        // cout<<heuristic_functions[i](b, original)<<" ";
    }
    // cout<<endl;
    for (auto move: b->get_legal_moves())
    {
        st+=move_to_str(move)+" ";
    }
    return make_pair(sum, st);
}

// Testing rook heuristic
// int main()
// {
//     Board b = Board();
//     b.data.b_bishop=DEAD;
//     b.data.b_rook_ws=DEAD;
//     b.data.b_rook_bs=DEAD;
//     b.data.b_pawn_bs=DEAD;
//     b.data.b_pawn_ws=DEAD;
//     b.data.w_pawn_ws=DEAD;
//     b.data.w_pawn_bs=DEAD;
//     b.data.w_rook_ws=DEAD;
    
//     b.do_move(str_to_move("d1b5"));
//     b.do_move(str_to_move("e7f6"));
//     b.do_move(str_to_move("d1a4"));
//     b.do_move(str_to_move("e1a7"));
//     cout<<all_boards_to_str(b)<<endl;
//     b.data.player_to_play=BLACK;
//     cout<<(legal_moves(&b, BLACK))<<endl;
//     for(auto move: b.get_legal_moves())
//     {
//         cout<<move_to_str(move)<<endl;
//     }
//     // b.do_move();
// }
/* //COPYING FROM engine.cpp

long long nodes = 0;

bool g(U16 move1, U16 move2)
{
    return dist(move1) > dist(move2);
}

int min_move(Board *b, int count, int flag, int alpha, int beta, PlayerColor original, int *c)
{

    nodes++;
    // cout<<all_boards_to_str((*b))<<" checking node "<<endl;
    if (count < 0)
    {
        return eval_state(b, original, c);
    }

    auto moveset = b->get_legal_moves();

    vector<U16> moves;
    for (auto move : moveset)
    {
        moves.push_back(move);
    }
    sort(moves.begin(), moves.end(), g);

    if (moveset.size() == 0)
    {
        return eval_state(b, original, c);
    }
    else if (flag == 1)
    {
        int max_val = INT32_MIN;
        for (auto move : moves)
        {
            Board *b2 = (*b).copy();
            b2->do_move(move);
            int temp = min_move(b2, count - 1, 0, alpha, beta, original, c);
            max_val = max(max_val, temp);

            // cout << "Count: " << count << endl;
            // cout << "Move done: " << move_to_str(move)<<endl;
            // cout << "Original Evaluation of node: " << eval_state(b, original,c) << endl;
            // cout << "Backed up value: " << temp << endl;
            // cout << "Delta: " << temp - eval_state(b, original,c) << endl;
            // cout << endl;

            alpha = max(alpha, max_val);
            if (alpha >= beta)
            {
                return max_val;
            }

            free(b2);
        }
        return max_val;
    }
    else
    {
        int min_val = INT32_MAX;
        for (auto move : moves)
        {
            Board *b2 = (*b).copy();
            b2->do_move(move);
            min_val = min(min_val, min_move(b2, count - 1, 1, alpha, beta, original, c));

            // cout << "Count: " << count << endl;
            // cout << "Move done: " << move_to_str(move)<<endl;
            // cout << "Original Evaluation of node: " << eval_state(b, original,c) << endl;
            // cout << "Backed up value: " << min_val << endl;
            // cout << "Delta: " << min_val - eval_state(b, original,c) << endl;
            // cout << endl;

            beta = min(beta, min_val);
            if (alpha >= beta)
            {
                return min_val;
            }
            free(b2);
        }
        return min_val;
    }
}

U16 begin_search(Board *b_1, int count, PlayerColor original, int *c)
{
    cout << "in begin search" << endl;
    auto moveset = b_1->get_legal_moves();
    vector<U16> moves;
    for (auto move : moveset)
    {
        moves.push_back(move);
    }
    sort(moves.begin(), moves.end(), g);
    if (moveset.size() == 0)
    {
        return 0;
    }
    else
    {
        int max_val = INT32_MIN;
        U16 best_move = 0;

        int alpha = INT32_MIN;
        int beta = INT32_MAX;

        int inital_evaluation = eval_state(b_1, original, c);
        cout << "Initial Evaluation : " << inital_evaluation << endl;
        for (auto move : moves)
        {
            std::cout << nodes << " nodes covered ";
            Board *b = (*b_1).copy();
            b->do_move(move);
            nodes++;

            int x = min_move(b, count - 1, 0, alpha, beta, original, c);
            std::cout << " testing " << move_to_str(move) << " value " << x << " ";
            if (max_val < x)
            {
                max_val = x;
                best_move = move;
            }
            free(b);
            // b->_undo_last_move(move);
            // b->_flip_player();
        }

        cout << endl;

        for (auto m : moveset)
        {
            std::cout << move_to_str(m) << " ";
        }
        cout << endl;
        cout << "Final Evaluation : " << max_val << endl;
        int delta = max_val - inital_evaluation;
        if (delta > 0)
        {
            // Increase the coefficients which are positive
            f(i, SIZE)
            {
                if (c[i] > 0)
                    c[i] += delta / 10;
            }
        }
        else
        {
            f(i, SIZE)
            {
                if (c[i] < 0)
                    c[i] = delta / 10;
            }
        }
        cout << "best " << move_to_str(best_move) << " " << max_val << " " << endl;
        return best_move;
    }
}

int main()
{
    Board b = Board();
    Board *copy_b = b.copy();
    PlayerColor original = b.data.player_to_play;
    nodes = 0;

    // Note that the rook heuristic has been flipped
    int c[] = {50, -30, 30, -10, 0, 0, 1};
    // cout<<all_boards_to_str(b)<<" original board "<<endl;
    cout << "Initial value of coefficients:" << endl;
    disp(c);
    f(i, 7)
    {
        cout << "Running ply " << i << endl;
        auto move = begin_search(copy_b, i + 1, original, c);
        cout << "Updated coefficients:" << endl;
        disp(c);
        cout << endl;
    }
}
*/