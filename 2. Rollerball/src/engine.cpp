#include <algorithm>
#include <random>
#include <iostream>
#include <climits>

#include "board.hpp"
#include "engine.hpp"
// #include "learning.cpp"
// #include <map>
using namespace std;

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


long long nodes = 0;

// map<U16,vector<U16>> catalog;

bool g(U16 move1, U16 move2)
{
    return dist(move1) > dist(move2);
}

pair<int, std::string> min_move(Board *b, int count, int flag, int alpha, int beta, PlayerColor original)
{
    nodes++;
    std::string bestp;
    if (count<-1)
        {return eval_state(b,original);}
    if (count < 0)
    {
        // Quiscence for check did not test yet
        if (b->in_check() && b->data.player_to_play == original)
        {
            auto moveset = b->get_legal_moves();
            int min_val=INT_MAX;
            for (auto move : moveset)
            {
                Board *b2 = (*b).copy();
                b2->do_move(move);
                auto t = min_move(b2, count - 1, 1, alpha, beta, original);
                // min_val = min(min_val,min_move(b2,count-1,1,alpha,beta));
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
                free(b2);
                // b->_undo_last_move(move);
                // b->_flip_player();
            }
            // cout<<"opponents's best val"<<min_val<<endl;
            return make_pair(min_val, bestp);
        }
        auto x=eval_state(b, original);
        return make_pair(get<0>(x), get<1>(x));
    }
    auto moveset = b->get_legal_moves();
    vector<U16> moves;
    for (auto move : moveset)
    {
        moves.push_back(move);
    }
    // sort(moves.begin(),moves.end(),g);
    // Eval here sort on basis of pair<eval, move> ?
    // vector<pair<int,U16>> moves;
    if (moveset.size() == 0)
    {
        // cout<<"here every time"<<endl;
        auto x=eval_state(b, original);
        return make_pair(get<0>(x), get<1>(x));
    }
    else if (flag == 1)
    {
        int max_val = INT32_MIN;
        for (auto move : moves)
        {
            // b->do_move(move);
            Board *b2 = (*b).copy();
            b2->do_move(move);
            auto t = min_move(b2, count - 1, 0, alpha, beta, original);
            // max_val = max(max_val,min_move(b2,count-1,0,alpha,beta));
            if (max_val < get<0>(t))
            {
                
                max_val = get<0>(t);
                bestp = move_to_str(move) + get<1>(t);
            }
            
            // if(get<0>(t)>10000 && get<1>(t).size()==0) return make_pair(INT_MIN, "");
            alpha = max(alpha, max_val);
            if (alpha >= beta)
            {
                return make_pair(max_val, bestp);
            }
            // b->_undo_last_move(move);
            // b->_flip_player();
            free(b2);
        }
        // cout<<"here every time 2"<<endl;
        return make_pair(max_val, bestp);
    }
    else
    {
        int min_val = INT32_MAX;
        for (auto move : moves)
        {
            Board *b2 = (*b).copy();
            b2->do_move(move);
            auto t = min_move(b2, count - 1, 1, alpha, beta, original);
            // min_val = min(min_val,min_move(b2,count-1,1,alpha,beta));
            if (min_val > get<0>(t))
            {
                min_val = get<0>(t);
                bestp = move_to_str(move) + get<1>(t);
            }
            // if(get<0>(t)>10000 && get<1>(t).size()==0) return make_pair(INT_MAX, "");
            beta = min(beta, min_val);
            if (alpha >= beta)
            {
                return make_pair(min_val, bestp);
            }
            free(b2);
            // b->_undo_last_move(move);
            // b->_flip_player();
        }
        // cout<<"opponents's best val"<<min_val<<endl;
        return make_pair(min_val, bestp);
    }
}

U16 begin_search(Board *b_1, int count, Engine *e, PlayerColor original)
{
    cout << "in begin search" << endl;
    auto moveset = b_1->get_legal_moves();
    vector<U16> moves;
    std::string bestp;
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
        e->best_move=moves[0];
        int max_val = INT32_MIN;
        U16 best_move = 0;
        // for (auto m : moveset) {
        //     std::cout << move_to_str(m)<<" ";
        // }
        // int count = 0;
        int alpha = INT32_MIN;
        int beta = INT32_MAX;
        for (auto move : moves)
        {
            // std::cout << nodes << " nodes covered ";
            Board *b = (*b_1).copy();
            b->do_move(move);
            nodes++;
            // max_val = max(max_val,min_move(b,count+1));
            auto x = min_move(b, count - 1, 0, alpha, beta, original);
            // std::cout << " testing " << move_to_str(move) << " value " << get<0>(x) << " path " << get<1>(x) << endl;
            if (max_val < get<0>(x))
            {
                max_val = get<0>(x);
                // bestp = get<1>(x);
                best_move = move;
                e->best_move = best_move;
                if (e->search==false) return best_move;
            }
            // cout<<"get1x size"<<get<1>(x).size()<<endl;
            // cout<<"get1x"<<get<1>(x)<<endl;
            if (get<1>(x).length()<30 && get<0>(x)>10000) return move;

            free(b);
            alpha = max(alpha, max_val);
            if (alpha >= beta)
            {
                return best_move;
                // for (auto m : moveset)
                // {
                //     std::cout << move_to_str(m) << " ";
                // }
                // std::cout << move_to_str(best_move) << " " << max_val << " ";
                // cout << "best " << move_to_str(best_move) << endl;
            }
                if (e->search==false) return best_move;

        }
        // for (auto m : moveset)
        // {
        //     std::cout << move_to_str(m) << " ";
        // }
        // std::cout << move_to_str(best_move) << " " << max_val << " ";
        // cout << "best " << move_to_str(best_move) << endl;
        return best_move;
    }
}

void Engine::find_best_move(const Board &b)
{
    // HARDCODING
    // Hardcode c2b3 and the subsequent move
    Board n = Board();
    if (all_boards_to_str(n)==all_boards_to_str(b)&&b.data.player_to_play==WHITE)
    {
        this->best_move=str_to_move("c2b3");
        return;
    }
    // Hardcode c2b2
    n.do_move(str_to_move("c2b2"));
    if (all_boards_to_str(n)==all_boards_to_str(b) && b.data.player_to_play==BLACK)
    {
        this->best_move=str_to_move("c6b6");
        return;
    }

    // A bad move for white
    Board nbad = Board();
    nbad.do_move(str_to_move("c2b1"));
    if (all_boards_to_str(nbad)==all_boards_to_str(b) && b.data.player_to_play==BLACK)
    {
        this->best_move=str_to_move("c6b6");
        return;
    }

    Board n1 = Board();
    n1.do_move(str_to_move("c2b3"));
    n1.do_move(str_to_move("e6f6"));
    if (all_boards_to_str(n1)==all_boards_to_str(b) && b.data.player_to_play==WHITE)
    {
        this->best_move=str_to_move("e2f2");
        return;
    }


    ///// HARDCODING ENDS /////
//415 nodes covered  testing d6e7r value 434 path f4g3e7g7g3f49 0 0 1 0 3 0 b7b6 g7a1 b7a7 e2e1 b7e7 c6f5 e2d1 b7d7 e2d2 e2f1 g7c1 c6g4 g7d1 g7f1 c6b5 g7g1 g7g3 g7e1 g7g5 b7f7 e2f2 c6e6 b7c7 g7g6 g7g4 g7b1 g7g2 c6d7 g7f7
//569 nodes covered  testing d6e6b value 450 path f4g3b7g7g3f49 0 0 0 0 0 0 g7d1 g7f1 g7a1 e6f5 g7e1 g7g4 g7b1 g7g6 e6f7 e6f3 g7g2 e6d7 g7g5 e6g6 g7g3 e6g4 c6b7 g7g1 c6b5 e2e1 e2f2 e2f1 e2d2 e2d1 g7c1 c6d7 g7f7
//779 nodes covered  testing d6e6r value 450 path f4g3b7g7g3f49 0 0 0 0 0 0 g7a1 g7d1 g7f1 g7e1 g7g4 g7b1 e6f6 g7g6 g7g2 e6e7 g7f7 c6d7 g7g5 e6g6 g7g3 c6b7 e2e1 g7g1 c6b5 e6d6 e2f2 e2f1 e2d2 e2d1 g7c1
    Board *copy_b = b.copy();
    nodes = 0;
    PlayerColor original = b.data.player_to_play;
    auto move = begin_search(copy_b, 3, this, original);
    // std::cout << move_to_str(move) << " result in find_best_move ";
    // std::cout << nodes << " result in find_best_move ";
    this->best_move = move;

}
