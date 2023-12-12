#include "steps_representation.h"
#include "board.h"

DangerSign::DangerSign(Point center, Chessboard& chess_)
    : Circle{center, c_size / 2}
{
    for (int i = center.x - c_size / 4; i <= center.x + c_size / 4;
         i += c_size / 2)
    {
        for (int j = center.y - c_size / 4; j <= center.y + c_size / 4;
             j += c_size / 2)
        {
            std::unique_ptr<Circle> t_c{new Circle{{i, j}, circle_radius}};
            t_c->set_fill_color(Graph_lib::Color::red);
            t_c->set_color(Graph_lib::Color::red);
            circle_of_circles.push_back(std::move(t_c));
        }
    }
    for (int i = center.x - c_size / 4 - c_size / 8;
         i <= center.x + c_size / 4 + c_size / 8;
         i += c_size / 4 + c_size / 8)
    {
        std::unique_ptr<Circle> t_c{
            new Circle{{i, center.y}, circle_radius}};
        t_c->set_fill_color(Graph_lib::Color::red);
        t_c->set_color(Graph_lib::Color::red);
        circle_of_circles.push_back(std::move(t_c));
    }
    for (int j = center.y - c_size / 4 - c_size / 8;
         j <= center.y + c_size / 4 + c_size / 8;
         j += c_size / 2 + c_size / 4)
    {
        std::unique_ptr<Circle> t_c{
            new Circle{{center.x, j}, circle_radius}};
        t_c->set_fill_color(Graph_lib::Color::red);
        t_c->set_color(Graph_lib::Color::red);
        circle_of_circles.push_back(std::move(t_c));
    }

    chess = &chess_;
}

DangerSign::~DangerSign()
{
    for (int i = (int)circle_of_circles.size() - 1; i >= 0; i--)
    {
        chess->detach(*circle_of_circles[i]);

        chess->detach(*this);
    }
}

void DangerSign::draw_lines() const
{
    for (int i = 0; i < int(circle_of_circles.size()); i++)
        circle_of_circles[i]->draw_lines();
}

RedCross::RedCross(Point center, Chessboard& chess_)
    : Rectangle{center, c_size, c_size}
{
    int x = center.x;
    int y = center.y;

    rectangle_1.reset(new Closed_polyline{
        {x - c_size / 2 + 2 * dist, y - c_size / 2 + dist},
        {x + c_size / 2 - dist, y + c_size / 2 - 2 * dist},
        {x + c_size / 2 - 2 * dist, y + c_size / 2 - dist},
        {x - c_size / 2 + dist, y - c_size / 2 + 2 * dist}});

    rectangle_1->set_color(Graph_lib::Color::red);
    rectangle_1->set_fill_color(Graph_lib::Color::red);

    rectangle_2.reset(new Closed_polyline{
        {x + c_size / 2 - 2 * dist, y - c_size / 2 + dist},
        {x + c_size / 2 - dist, y - c_size / 2 + 2 * dist},
        {x - c_size / 2 + 2 * dist, y + c_size / 2 - dist},
        {x - c_size / 2 + dist, y + c_size / 2 - 2 * dist}});
    rectangle_2->set_color(Graph_lib::Color::red);
    rectangle_2->set_fill_color(Graph_lib::Color::red);

    chess = &chess_;
}

RedCross::~RedCross()
{
    chess->detach(*rectangle_1);

    chess->detach(*rectangle_2);

    chess->detach(*this);
}

void RedCross::draw_lines() const
{
    rectangle_1->draw_lines();
    rectangle_2->draw_lines();
}

Frame::Frame(Point center, Chessboard& chess_)
    : Rectangle({center.x - c_size / 2, center.y - c_size / 2}, c_size,
                c_size)
{

    Rectangle* h_r1 =
        new Rectangle{{center.x - c_size / 2, center.y - c_size / 2},
                      rc_width,
                      rc_length};
    Rectangle* h_r2 = new Rectangle{
        {center.x + c_size / 2 - rc_width, center.y - c_size / 2},
        rc_width,
        rc_length};
    Rectangle* h_r3 = new Rectangle{
        {center.x - c_size / 2, center.y + c_size / 2 - rc_length},
        rc_width,
        rc_length};
    Rectangle* h_r4 = new Rectangle{{center.x + c_size / 2 - rc_width,
                                     center.y + c_size / 2 - rc_length},
                                    rc_width,
                                    rc_length};

    horisontal_rectangles.push_back(h_r1);
    horisontal_rectangles.push_back(h_r2);
    horisontal_rectangles.push_back(h_r3);
    horisontal_rectangles.push_back(h_r4);

    // There are probably ways to avoid Copy+Paste but there's
    // something with Graph_lib that doesn't allow it to be easy

    Rectangle* v_r1 =
        new Rectangle{{center.x - c_size / 2, center.y - c_size / 2},
                      rc_length,
                      rc_width};
    Rectangle* v_r2 = new Rectangle{
        {center.x + c_size / 2 - rc_length, center.y - c_size / 2},
        rc_length,
        rc_width};
    Rectangle* v_r3 = new Rectangle{
        {center.x - c_size / 2, center.y + c_size / 2 - rc_width},
        rc_length,
        rc_width};
    Rectangle* v_r4 = new Rectangle{{center.x + c_size / 2 - rc_length,
                                     center.y + c_size / 2 - rc_width},
                                    rc_length,
                                    rc_width};

    vertical_rectangles.push_back(v_r1);
    vertical_rectangles.push_back(v_r2);
    vertical_rectangles.push_back(v_r3);
    vertical_rectangles.push_back(v_r4);

    for (int i = 0; i < 4; i++)
    {
        horisontal_rectangles[i].set_color(chess_yellow);
        horisontal_rectangles[i].set_fill_color(chess_yellow);

        vertical_rectangles[i].set_color(chess_yellow);
        vertical_rectangles[i].set_fill_color(chess_yellow);
    }

    chess = &chess_;
}

Frame::~Frame()
{
    for (int i = (int)horisontal_rectangles.size() - 1; i >= 0; i--)
    {
        chess->detach(horisontal_rectangles[i]);
        chess->detach(vertical_rectangles[i]);
    }
    chess->detach(*this);
}

void Frame::draw_lines() const
{
    for (int i = 0; i < 4; i++)
    {

        horisontal_rectangles[i].draw_lines();
        vertical_rectangles[i].draw_lines();
    }
}

VisualSteps::~VisualSteps()
{
    for (int i = int(possible_steps.size() - 1); i >= 0; i--)
        chess->detach(possible_steps[i]);
}