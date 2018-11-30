
#include "gameSystem.h"
#include <thread>

int main() {
    using namespace std::chrono_literals;

	sinkFleet::GameSystem game;

    srand(static_cast<unsigned>(time(NULL)));
    
    console::Console cons;
    console::SIZE size = cons.consoleSize( );
    cons.consoleSize(size.width, 30);
    size = cons.consoleSize( );
    console::ConsoleBuffer buffer(size.width, size.height - 1);
    buffer.endLine(false);
    console::Keyboard keyboard;

    keyboard.controled(true);
    keyboard.pause(3);
    cons.cursorVisible(false);

	while(game.running()) {
        cons.cursorPos(0, 0);

        keyboard.pullKeys( );
		game.update(keyboard);

        buffer.clear( );
        game.draw(buffer);
        buffer >> std::cout;
	}

	return EXIT_SUCCESS;
}