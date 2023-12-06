#include "board.h"
#include "cell.h"
#include <iostream>


Cell& Sub_Vector_ref::operator[](int i)
{
    if(i < 1 || i > 8)
        Graph_lib::error("Out of range: vertical");
    return v[i-1];
}

MyWindow::MyWindow(Point xy, int w, int h, const std::string& title)
    : Simple_window{xy, w, h, title}, quit_button{Point{x_max() - 70, 20}, 70, 20, "Quit", cb_quit}
{
    attach(quit_button);
}

void MyWindow::cb_quit(Address, Address widget)
{  // void*
    auto& btn = Graph_lib::reference_to<Graph_lib::Button>(widget);
    dynamic_cast<MyWindow&>(btn.window()).quit();
}

Cell::Type type_of_cell (int i, int j)
{
    if (i % 2 == 0)
        return (j % 2 == 0) ? Cell::dark_green : Cell::white;
    else
        return (j % 2 == 0) ? Cell::white : Cell::dark_green;
}

std::string letters ()
{
    std::string s(Chessboard::N_max, '\0');
    for (size_t i = 0; i < s.size(); ++i)
    {
        s[i] = 'a' + i;
    }
    return s;
}

std::string digits ()
{
    std::string s(Chessboard::N_max, '\0');

    for (size_t i = 0; i < s.size(); ++i)
    {
        s[i] = '1' + i;
    }
    return s;
}

Chessboard::Chessboard(Point xy) : MyWindow{xy, width, height, "Chessboard"}, x_labels{letters()}, y_labels{digits()}
{
    size_range(width, height, width, height);
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            cells.push_back(new Cell{Point{margin + j * Cell::size, margin + (N - 1 - i) * Cell::size}, cb_clicked,
                                     type_of_cell(i, j)});
            attach(cells[cells.size() - 1]);
        }
    }

    for (int i = 0; i < N; ++i)
    {
        // board corners: left bottom
        constexpr Point lb{margin + Cell::size / 2, margin + N * Cell::size + 10};
        // and right bottom
        constexpr Point rb{margin - 10, margin + N * Cell::size - Cell::size / 2};
        x_labels.add(Point{lb.x + i * Cell::size, lb.y});
        y_labels.add(Point{rb.x, rb.y - i * Cell::size});
    }
    attach(x_labels);
    attach(y_labels);

    all_possible_steps = nullptr;

    step_chooser = step_color::white;
}

void Chessboard::standard_fill()
{
    // белые пешки
    for(int i = 0; i < 8; i++)
    {
        Pawn* temp_pawn = new Pawn(*this, Figure::Type::white);
        pawns.push_back(temp_pawn);
        at(char(a_ascii+i),2).attach_figure(pawns[i]);
    }

    // чёрные пешки
    for(int i = 8; i < 16; i++)
    {
        Pawn* temp_pawn = new Pawn(*this, Figure::Type::black);
        pawns.push_back(temp_pawn);
        at(char(a_ascii+i%8),7).attach_figure(pawns[i]);
    }


    // белые кони
    Knight* wn0 = new Knight{*this, Figure::Type::white};
    Knight* wn1 = new Knight{*this, Figure::Type::white};

    knights.push_back(wn0);
    knights.push_back(wn1);

    at('b',1).attach_figure(knights[0]);
    at('g',1).attach_figure(knights[1]);

    // черыне кони
    Knight* bn0 = new Knight{*this, Figure::Type::black};
    Knight* bn1 = new Knight{*this, Figure::Type::black};

    knights.push_back(bn0);
    knights.push_back(bn1);

    at('b',8).attach_figure(knights[2]);
    at('g',8).attach_figure(knights[3]);

    // белые слоны
    Bishop* wb0 = new Bishop{*this, Figure::Type::white};
    Bishop* wb1 = new Bishop{*this, Figure::Type::white};

    bishops.push_back(wb0);
    bishops.push_back(wb1);

    at('c',1).attach_figure(bishops[0]);
    at('f',1).attach_figure(bishops[1]);

    // черные слоны
    Bishop* bb0 = new Bishop{*this, Figure::Type::black};
    Bishop* bb1 = new Bishop{*this, Figure::Type::black};

    bishops.push_back(bb0);
    bishops.push_back(bb1);

    at('c',8).attach_figure(bishops[2]);
    at('f',8).attach_figure(bishops[3]);

    // белые ладьи
    Rook* wr0 = new Rook{*this, Figure::Type::white};
    Rook* wr1 = new Rook{*this, Figure::Type::white};

    rooks.push_back(wr0);
    rooks.push_back(wr1);

    at('a',1).attach_figure(rooks[0]);
    at('h',1).attach_figure(rooks[1]);

    // черные ладьи
    Rook* br0 = new Rook{*this, Figure::Type::black};
    Rook* br1 = new Rook{*this, Figure::Type::black};

    rooks.push_back(br0);
    rooks.push_back(br1);

    at('a',8).attach_figure(rooks[2]);
    at('h',8).attach_figure(rooks[3]);

    // Белый король
    King* wk = new King{*this, Figure::Type::white};

    kings.push_back(wk);

    at('e',1).attach_figure(kings[0]);

    // Черный король
    King* bk = new King{*this, Figure::Type::black};

    kings.push_back(bk);

    at('e',8).attach_figure(kings[1]);

    // Белый ферзь
    Queen* wq = new Queen{*this, Figure::Type::white};

    queens.push_back(wq);

    at('d',1).attach_figure(queens[0]);

    // Черный ферзь
    Queen* bq = new Queen{*this, Figure::Type::black};

    queens.push_back(bq);

    at('d',8).attach_figure(queens[1]);
}

void Chessboard::clicked(Cell& c)
{  // для шашек. Для всего остального удалить
    // if (!c.is_black()) return; для контроля, кто должен ходить
    //  в c также лежит информация о том, стоит ли фигура на это клетке. (предположительно)
    //  если стоит, то has_checker() == true;
    // std::cout << "*\n";
    // std::cout << "1\n";
    if (!selected)
    {
        // all_possible_steps = nullptr;
        selected = &c;
        c.activate();  // подсвечивает
        if (decide() == false)
        {
            c.deactivate();
            selected = nullptr;
            return;
        }
        // Create visual representation of moves for current figure
        all_possible_steps = c.get_figure().show_possible_steps(c.location(), *this);
        // std::cout << "2\n";
    }
    else
    {
        // std::cout << "3\n";
        if (selected->has_figure())
        {
            int a = selected->get_figure().correct_step(*selected, c, *this);
            if (a)
            {
                // if()
                //  move_figure
                Cell& c1 = *selected;
                int x = c.location().x, y = c.location().y;

                int b;
                if (step_chooser == black)
                    b = 1;
                else
                    b = -1;
                if (c.has_figure())
                {
                    // taking the figure from the opponent
                    detach(c.detach_figure());            // убираем фигуру врага
                    c.attach_figure(c1.detach_figure());  // переносим свою
                }
                else if (1 <= y + b && y + b < 7 && (*this)[x][y + b].has_figure() && (a == 2 || a == 3))
                {
                    detach((*this)[x][y + b].detach_figure());  // *this = chess
                    (*this)[x][y].attach_figure(c1.detach_figure());
                }
                else
                {
                    c.attach_figure(c1.detach_figure());
                }

                step_swap();
                reset_double_steps();
            }
        }

        selected->deactivate();

        // Clear the screen from visual representation of possible moves for the current figure
        if (all_possible_steps != nullptr)
        {
            delete all_possible_steps;
            all_possible_steps = nullptr;
        }
        selected = nullptr;
    }
    Fl::redraw();
}

bool Chessboard::decide()
{
    if (!selected->has_figure())
        return false;
    if (step_chooser == step_color::white && selected->get_figure().is_black())
        return false;
    else if (step_chooser == step_color::black && selected->get_figure().is_white())
        return false;
    return true;
}

void Chessboard::reset_double_steps()
{
    for(int i = 0; i < cells.size(); i++)
        if(cells[i].has_figure() && cells[i].get_figure().is_pawn())
            cells[i].get_figure().reset_double_step();
}

bool Chessboard::out_of_range(Coordinate pos)
{
    if ((int(pos.x) < a_ascii) || (int(pos.x) > a_ascii + 7))
        return true;
    else if ((pos.y < 1) || (pos.y > 8))
        return true;
    return false;
}

Sub_Vector_ref Chessboard::operator[](char c)
{
    if(c < 'a' || c > 'h')
        Graph_lib::error("Out of range: horisontal");
    Graph_lib::Vector_ref<Cell> tempv;
    for(int i = 1; i <= N; i++)
        tempv.push_back(at(c, i));
    Sub_Vector_ref subv{tempv};
    return subv;
}

Chessboard* Chessboard::deepcopy()
{
    Chessboard* chess = new Chessboard{Chessboard_location};

    //copying pawns
    for(int i = 0; i < pawns.size(); i++)
    {
        Pawn* temp_pawn = pawns[i].deepcopy(*chess);
        chess->pawns.push_back(temp_pawn);

        if(pawns[i].has_cell())
        {
            char tx = pawns[i].get_cell()->location().x;
            int ty = pawns[i].get_cell()->location().y;
            (*chess)[tx][ty].attach_figure(chess->pawns[chess->pawns.size()-1]);
        }
        else
            chess->detach(chess->pawns[chess->pawns.size()-1]);
    }
    //copying kings
    for(int i = 0; i < kings.size(); i++)
    {
        King* temp_king = kings[i].deepcopy(*chess);
        chess->kings.push_back(temp_king);

        if(kings[i].has_cell())
        {
            char tx = kings[i].get_cell()->location().x;
            int ty = kings[i].get_cell()->location().y;
            (*chess)[tx][ty].attach_figure(chess->kings[chess->kings.size()-1]);
        }
        else
            chess->detach(chess->kings[chess->kings.size()-1]);
    }
    //copying bishops
    for(int i = 0; i < bishops.size(); i++)
    {
        Bishop* temp_bishop = bishops[i].deepcopy(*chess);
        chess->bishops.push_back(temp_bishop);

        if(bishops[i].has_cell())
        {
            char tx = bishops[i].get_cell()->location().x;
            int ty = bishops[i].get_cell()->location().y;
            (*chess)[tx][ty].attach_figure(chess->bishops[chess->bishops.size()-1]);
        }
        else
            chess->detach(chess->bishops[chess->bishops.size()-1]);
    }
    //copying knights
    for(int i = 0; i < knights.size(); i++)
    {
        Knight* temp_knight = knights[i].deepcopy(*chess);
        chess->knights.push_back(temp_knight);

        if(knights[i].has_cell())
        {
            char tx = knights[i].get_cell()->location().x;
            int ty = knights[i].get_cell()->location().y;
            (*chess)[tx][ty].attach_figure(chess->knights[chess->knights.size()-1]);
        }
        else
            chess->detach(chess->knights[chess->knights.size()-1]);
    }
    //copying queens
    for(int i = 0; i < queens.size(); i++)
    {
        Queen* temp_queen = queens[i].deepcopy(*chess);
        chess->queens.push_back(temp_queen);

        if(queens[i].has_cell())
        {
            char tx = queens[i].get_cell()->location().x;
            int ty = queens[i].get_cell()->location().y;
            (*chess)[tx][ty].attach_figure(chess->queens[chess->queens.size()-1]);
        }
        else
            chess->detach(chess->queens[chess->queens.size()-1]);
    }
    //copying rooks
    for(int i = 0; i < rooks.size(); i++)
    {
        Rook* temp_rook = rooks[i].deepcopy(*chess);
        chess->rooks.push_back(temp_rook);

        if(rooks[i].has_cell())
        {
            char tx = rooks[i].get_cell()->location().x;
            int ty = rooks[i].get_cell()->location().y;
            (*chess)[tx][ty].attach_figure(chess->rooks[chess->rooks.size()-1]);
        }
        else
            chess->detach(chess->rooks[chess->rooks.size()-1]);
    }
    
    return chess;
}
