#include "board.h"
#include "cell.h"
#include "figure.h"
#include "steps_representation.h"
#include <iostream>

extern const Graph_lib::Point Chessboard_location;

void demo(Chessboard& chess) {
    DangerSign ds{chess.at('d', 4).center(), chess};
    RedCross rc{chess.at('c', 4).center(), chess};
    chess.attach(rc);
    chess.attach(ds);
}

int main ()
{
    Chessboard chess{Chessboard_location};

    demo(chess);
    
    chess.standard_fill();
    
    chess.wait_for_button();
    
    return 0;
}
