#pragma once

// #include "cell.h"
#include "steps_representation.h"
#include <Graph_lib/Graph.h>
#include <Graph_lib/Simple_window.h>

constexpr int a_ascii = 97;  // ascii code of letter 'a'

constexpr int chess_yellow = 133;  // 16

struct Cell;
struct Chessboard;
struct Coordinate;
struct VisualSteps;

struct Figure : Graph_lib::Image
{  // Graph_lib::Image

    enum Type
    {
        black,
        white
    };

    Figure(Graph_lib::Window& win, Type color, std::string path_to_image)
        : Graph_lib::Image(Graph_lib::Point(0, 0), path_to_image)
    {
        this->color = color;
        win.attach(*this);
    }

    // Checks if clicked cell satisfies the conditions of a correct move
    virtual int correct_step (Cell& c1, Cell& c2, Chessboard& chess, bool ensure_king_is_safe = true) = 0;

    // Creates an object "VisualSteps" that is required to show all possible moves of currently clicked figure
    virtual VisualSteps* show_possible_steps (Coordinate position, Chessboard& chess) = 0;

    virtual bool can_take_king (Chessboard& chess, Cell& king_position)
    {
        return correct_step(*(const_cast<Cell*>(cell)), king_position, chess, false);
    }

    virtual bool is_pawn () { return false; }    /////////////////
    virtual bool is_king () { return false; }    //             //
    virtual bool is_bishop () { return false; }  //  could be   //
    virtual bool is_knight () { return false; }  // done better //
    virtual bool is_queen () { return false; }   //             //
    virtual bool is_rook () { return false; }    /////////////////

    bool has_cell () { return cell != nullptr; }

    const Cell*& get_cell () { return cell; }

    virtual bool double_step0 () { return false; }

    virtual void reset_double_step () {}

    bool change_pos_decider (Cell& c);

    void draw_lines () const override { Graph_lib::Image::draw_lines(); }

    void attach (const Cell& c);

    bool is_white () const { return color == white; }

    bool is_black () const { return color == black; }

    void detach () { cell = nullptr; }

  private:
    bool color;
    static constexpr int r = 0.9 * (80) / 2;
    const Cell* cell{nullptr};
};

bool King_is_under_attack (Chessboard& chess, bool is_white);// { return false; }

struct Pawn : Figure
{
    Pawn(Graph_lib::Window& win, Figure::Type color)
        : Figure(win, color, color == Type::white ? "wP.png" : "bP.png"), first_step{true},
        double_step{false}, steps_till_reset{0} {}

    int correct_step (Cell& c1, Cell& c2, Chessboard& chess, bool ensure_king_is_safe = true);
    VisualSteps* show_possible_steps (Coordinate position, Chessboard& chess) override;

    Pawn* deepcopy(Chessboard& chess);
    //friend Pawn* Pawn::deepcopy(Chessboard& chess);

    bool is_pawn () override { return true; }

    bool double_step0 () override { return double_step; }

    void reset_double_step () override;

    private:
        bool double_step;
        int steps_till_reset;
        bool first_step;
};

struct King : Figure
{
    King(Graph_lib::Window& win, Figure::Type color) : Figure(win, color, color == Type::white ? "wK.png" : "bK.png"){};

    int correct_step (Cell& c1, Cell& c2, Chessboard& chess, bool ensure_king_is_safe = true) override;
    VisualSteps* show_possible_steps (Coordinate position, Chessboard& chess) override;

    King* deepcopy(Chessboard& chess);
    //friend King* King::deepcopy(Chessboard& chess);

    bool is_king () override { return true; }
};

struct Bishop : Figure
{
    Bishop(Graph_lib::Window& win, Figure::Type color)
        : Figure(win, color, color == Type::white ? "wB.png" : "bB.png"){};

    int correct_step (Cell& c1, Cell& c2, Chessboard& chess, bool ensure_king_is_safe = true) override;
    VisualSteps* show_possible_steps (Coordinate position, Chessboard& chess) override;

    Bishop* deepcopy(Chessboard& chess);
    //friend Bishop* Bishop::deepcopy(Chessboard& chess);

    bool is_bishop () override { return true; }

  private:
    // HF - help function
    void show_possible_steps_HF (int x, int y, int x0, int y0, int d1, int d2, VisualSteps*& steps_representation,
                                 Chessboard& chess);
};

struct Knight : Figure
{
    Knight(Graph_lib::Window& win, Figure::Type color)
        : Figure(win, color, color == Type::white ? "wN.png" : "bN.png"){};

    int correct_step (Cell& c1, Cell& c2, Chessboard& chess, bool ensure_king_is_safe = true) override;
    VisualSteps* show_possible_steps (Coordinate position, Chessboard& chess) override;

    Knight* deepcopy(Chessboard& chess);
    //friend Knight* Knight::deepcopy(Chessboard& chess);

    bool is_knight () { return true; }
};

struct Queen : Figure
{
    Queen(Graph_lib::Window& win, Figure::Type color)
        : Figure(win, color, color == Type::white ? "wQ.png" : "bQ.png"){};

    int correct_step (Cell& c1, Cell& c2, Chessboard& chess, bool ensure_king_is_safe = true) override;
    VisualSteps* show_possible_steps (Coordinate position, Chessboard& chess) override;

    Queen* deepcopy(Chessboard& chess);
    //friend Queen* Queen::deepcopy(Chessboard& chess);

    virtual bool is_queen () { return true; }

  private:
    void horisontal_possible_steps (Coordinate& position, Chessboard& chess, VisualSteps*& steps_representation);
    void vertical_possible_steps (Coordinate& position, Chessboard& chess, VisualSteps*& steps_representation);
    void diagnal_possible_steps (int x, int y, int x0, int y0, Coordinate& position, Chessboard& chess,
                                 VisualSteps*& steps_representation);
    void show_possible_steps_HF (int x, int y, int x0, int y0, int d1, int d2, VisualSteps*& steps_representation,
                                 Chessboard& chess);
};

struct Rook : Figure
{
    Rook(Graph_lib::Window& win, Figure::Type color) : Figure(win, color, color == Type::white ? "wR.png" : "bR.png"){};

    int correct_step (Cell& c1, Cell& c2, Chessboard& chess, bool ensure_king_is_safe = true) override;
    VisualSteps* show_possible_steps (Coordinate position, Chessboard& chess) override;

    Rook* deepcopy(Chessboard& chess);
    //friend Rook* Rook::deepcopy(Chessboard& chess);

    bool is_rook () { return true; }

  private:
    void horisontal_possible_steps (Coordinate& position, Chessboard& chess, VisualSteps*& steps_representation);
    void vertical_possible_steps (Coordinate& position, Chessboard& chess, VisualSteps*& steps_representation);
};
