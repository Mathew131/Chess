#include "cell.h"
#include "figure.h"
#include <iostream>

using namespace Graph_lib;

Cell::Cell(Point xy, Callback cb, Type t)
    : Button{xy, size, size, "", cb}, type{t}
{
}

void Cell::reset_color()
{
    if (!pw)
        error("Cell is not attached to a window");
    if (is_black())
        pw->color(chess_green);
    else
        pw->color(chess_white);
}

void Cell::attach(Graph_lib::Window& win)
{
    Button::attach(win);
    reset_color();
}

Figure& Cell::detach_figure()
{
    Figure* ch = figure;
    figure = nullptr;
    ch->detach();
    return *ch;
}

void Cell::attach_figure(Figure& ch)
{
    ch.attach(*this);
    figure = &ch;
}

// removed const and i don't know how it will turn out
/*const*/ Figure& Cell::get_figure()  // обязательно нужна проверка не
                                      // нулевой ли указатель checker
{
    return *figure;
}

Coordinate Cell::location() const
{

    int N = 8;  // Couldn't find a way to properly use
                // the static constant from "board.h"

    char x = char((loc.x - DFTBOF) / size + a_ascii);
    int y = N - (loc.y - DFTBOF) / size;

    return Coordinate{x, y};
}