#include "console.h"

namespace console
{
    ConsoleBuffer::ConsoleBuffer(int width, int height) :
        drawFlags(flag::reset), space(0), precise(0), endNewLine(true),
        width(width), height(height), curX(0), curY(0),
        strBuffer(width * height, ' ') { }

    ConsoleBuffer::~ConsoleBuffer( ) { }

    bool ConsoleBuffer::inBounds( ) {
        return curX >= 0 && curY >= 0 && curX < width && curY < height;
    }

    void ConsoleBuffer::setCursorPos(int x, int y) {
        curX = x;
        curY = y;
    }

    int ConsoleBuffer::getX( ) { return curX; }
    int ConsoleBuffer::getY( ) { return curY; }
    int ConsoleBuffer::size( ) { return width * height; }
    int ConsoleBuffer::getWidth( ) { return width; }
    int ConsoleBuffer::getHeight( ) { return height; }
    int ConsoleBuffer::spacing( ) { return space; }
    int ConsoleBuffer::spacing(int space) {
        int oldSpace = this->space;
        this->space = space;
        return oldSpace;
    }

    int ConsoleBuffer::precision( ) { return precise; }
    int ConsoleBuffer::precision(int precise) {
        int oldPrecise = this->precise;
        this->precise = precise;
        return oldPrecise;
    }

    void ConsoleBuffer::write(const std::string &str) {
        int flags = drawFlags & ~(flag::baseFlags & flag::floatFlags
            & flag::numFlags);
        write(str, flags);
    }

    void ConsoleBuffer::write(const char *cstr) {
        int flags = drawFlags & ~(flag::baseFlags & flag::floatFlags
            & flag::numFlags);
        write(cstr, flags);
    }

    void ConsoleBuffer::print(const std::string &str) {
        int flags = drawFlags & ~(flag::baseFlags & flag::floatFlags
            & flag::numFlags);
        write(str, flags);
        curY++;
        curX = 0;
    }

    void ConsoleBuffer::print(const char *cstr) {
        int flags = drawFlags & ~(flag::baseFlags & flag::floatFlags
            & flag::numFlags);
        write(cstr, flags);
        curY++;
        curX = 0;
    }

    void ConsoleBuffer::print( ) {
        curY++;
        curX = 0;
    }

    void ConsoleBuffer::put(char c) { strBuffer[curX + curY * width] = c; }

    void ConsoleBuffer::clear( ) {
        for(int i = 0; i < width * height; i++) {
            strBuffer[i] = 0;
        }
    }

    int ConsoleBuffer::setFlags(int flags) {
        int oldFlags = drawFlags;
        drawFlags |= flags;
        int localf = flags & flag::sidedFlags;
        if(localf) {
            drawFlags &= ~flag::sidedFlags;
            if(localf & flag::right)
                drawFlags |= flag::right;
            else if(localf & flag::left)
                drawFlags |= flag::left;
            else if(localf & flag::center)
                drawFlags |= flag::center;
            else if(localf & flag::intern)
                drawFlags |= flag::intern;
        }
        localf = flags & flag::baseFlags;
        if(localf) {
            drawFlags &= ~flag::baseFlags;
            if(localf & flag::dec)
                drawFlags |= flag::dec;
            else if(localf & flag::oct)
                drawFlags |= flag::oct;
            else if(localf & flag::hex)
                drawFlags |= flag::hex;
        }
        return oldFlags;
    }

    int ConsoleBuffer::unsetFlags(int flags) {
        int oldFlags = drawFlags;
        int localf = flags & (~flag::baseFlags & ~flag::sidedFlags
            & ~flag::floatFlags);
        drawFlags &= ~flags;
        return oldFlags;
    }

    int ConsoleBuffer::resetFlags( ) {
        int oldFlags = drawFlags;
        drawFlags = flag::reset;
        return oldFlags;
    }

    int ConsoleBuffer::resetFlags(int flags) {
        int oldFlags = drawFlags;
        int localf = flags & (~flag::baseFlags & ~flag::sidedFlags
            & ~flag::floatFlags);
        (drawFlags &= ~localf) |= (localf & flag::reset);
        return oldFlags;
    }

    int ConsoleBuffer::flags( ) {
        return drawFlags;
    }

    bool ConsoleBuffer::endLine( ) {
        return endNewLine;
    }

    bool ConsoleBuffer::endLine(bool set) {
        bool oldEndNewLine = endNewLine;
        endNewLine = set;
        return oldEndNewLine;
    }

    void ConsoleBuffer::writeOut(std::ostream &out) {
        for(int i = 0; i < height; i++) {
            out << strBuffer.substr(i * width, width);
			if (endNewLine)
				out << std::endl;
        }
    }

    std::ostream& ConsoleBuffer::operator>>(std::ostream& out) {
        writeOut(out);
		return out;
    }

    void ConsoleBuffer::operator()(int x, int y) {
        curX = x;
        curY = y;
    }

    ConsoleBuffer& ConsoleBuffer::operator<<
        (ConsoleBuffer&(*func)(ConsoleBuffer&)) {
        return ((*func)(*this));
    }

    ConsoleBuffer& ConsoleBuffer::operator<<(short value) {
        int flags = drawFlags & ~flag::floatFlags;
        write(value, flags);
        return *this;
    }

    ConsoleBuffer& ConsoleBuffer::operator<<(unsigned short value) {
        int flags = drawFlags & ~flag::floatFlags;
        write(value, flags);
        return *this;
    }

    ConsoleBuffer& ConsoleBuffer::operator<<(int value) {
        int flags = drawFlags & ~flag::floatFlags;
        write(value, flags);
        return *this;
    }

    ConsoleBuffer& ConsoleBuffer::operator<<(unsigned int value) {
        int flags = drawFlags & ~flag::floatFlags;
        write(value, flags);
        return *this;
    }

    ConsoleBuffer& ConsoleBuffer::operator<<(long value) {
        int flags = drawFlags & ~flag::floatFlags;
        write(value, flags);
        return *this;
    }

    ConsoleBuffer& ConsoleBuffer::operator<<(unsigned long value) {
        int flags = drawFlags & ~flag::floatFlags;
        write(value, flags);
        return *this;
    }

    ConsoleBuffer& ConsoleBuffer::operator<<(long long value) {
        int flags = drawFlags & ~flag::floatFlags;
        write(value, flags);
        return *this;
    }

    ConsoleBuffer& ConsoleBuffer::operator<<(unsigned long long value) {
        int flags = drawFlags & ~flag::floatFlags;
        write(value, flags);
        return *this;
    }

    ConsoleBuffer& ConsoleBuffer::operator<<(float value) {
        int flags = drawFlags & ~flag::baseFlags;
        write(value, flags);
        return *this;
    }

    ConsoleBuffer& ConsoleBuffer::operator<<(double value) {
        int flags = drawFlags & ~flag::baseFlags;
        write(value, flags);
        return *this;
    }

    ConsoleBuffer& ConsoleBuffer::operator<<(long double value) {
        int flags = drawFlags & ~flag::baseFlags;
        write(value, flags);
        return *this;
    }

    ConsoleBuffer& ConsoleBuffer::operator<<(char value) {
        int flags = drawFlags & (~flag::baseFlags & ~flag::floatFlags
            & ~flag::numFlags);
        write(value, flags);
        return *this;
        
    }

    ConsoleBuffer & ConsoleBuffer::operator<<(unsigned char value) {
        int flags = drawFlags & (~flag::baseFlags & ~flag::floatFlags
            & ~flag::numFlags);
        write(value, flags);
        return *this;
    }

    ConsoleBuffer& ConsoleBuffer::operator<<(const char *value) {
        int flags = drawFlags & (~flag::baseFlags & ~flag::floatFlags
            & ~flag::numFlags);
        write(value, flags);
        return *this;
    }

    ConsoleBuffer& ConsoleBuffer::operator<<(const std::string &value) {
        int flags = drawFlags & (~flag::baseFlags & ~flag::floatFlags
            & ~flag::numFlags);
        write(value, flags);
        return *this;
    }

    //ConsoleBuffer& ConsoleBuffer::operator=(const ConsoleBuffer &buffer) {
    //
    //}

    ConsoleBuffer& endl(ConsoleBuffer &out) {
        out.print( );
        return out;
    }

    ConsoleBuffer& left(ConsoleBuffer &out) {
        out.setFlags(flag::left);
        return out;
    }

    ConsoleBuffer& right(ConsoleBuffer &out) {
        out.setFlags(flag::right);
        return out;
    }

    ConsoleBuffer& center(ConsoleBuffer &out) {
        out.setFlags(flag::center);
        return out;
    }

    ConsoleBuffer& intern(ConsoleBuffer &out) {
        out.setFlags(flag::intern);
        return out;
    }

    void setBufferSpacing(ConsoleBuffer &out, int length) {
        out.spacing(length);
    }

    Manipulator<int> setS(int length) {
        return Manipulator<int>(setBufferSpacing, length);
    }

    void setBufferBase(ConsoleBuffer &out, int base) {
        int f = flag::dec;
        switch(base) {
        case 8: f = flag::oct; break;
        case 16: f = flag::hex; break;
        default:
        case 10: f = flag::dec; break;
        }
        out.setFlags(f);
    }

    Manipulator<int> setBase(int base) {
        return Manipulator<int>(setBufferBase, base);
    }

    void setBufferPrecision(ConsoleBuffer &out, int precise) {
        out.precision(precise);
    }

    Manipulator<int> setPrecision(int precise) {
        return Manipulator<int>(setBufferPrecision, precise);
    }

    void setBufferX(ConsoleBuffer &out, int pos) {
        out(pos, out.getY( ));
    }

    Manipulator<int> setX(int pos) {
        return Manipulator<int>(setBufferX, pos);
    }


    void setBufferY(ConsoleBuffer &out, int pos) {
        out(out.getX( ), pos);
    }

    Manipulator<int> setY(int pos) {
        return Manipulator<int>(setBufferY, pos);
    }

    Keyboard::Keyboard( ) :
        capsLock(false), keyControl(false), pauseTime(0u),
        lastHeld(0u), keys { 0u }, keyStates { 0u } { }

    Keyboard::~Keyboard( ) { }

    void Keyboard::pullKeys( ) {
#if defined(_WIN32)
        if(GetConsoleWindow( ) == GetForegroundWindow( )) {
#endif
            for(unsigned i = 0u; i < MAX_KEY_SIZE; i++) {
#if defined(_WIN32)
                unsigned keyState = GetKeyState(i);
                unsigned highKey = HIBYTE(keyState);
                unsigned lowKey = LOBYTE(keyState);
                keys[i] = highKey != 0 ? ++keys[i] : 0u;
                keyStates[i] = keyState;
                if(i == VK_CAPITAL) {
                    capsLock = lowKey > 0u ? true : false;
                }
#endif
                if(keys[i] < keys[lastHeld]) {
                    lastHeld = i;
                } else if(lastHeld != 0u && keys[lastHeld] != 0u) {
                    lastHeld = 0u;
                }
            }
#if defined(_WIN32)
        }
#endif
    }

    bool Keyboard::keyPressed(unsigned vKey, InputType type) {
        bool pressed = false;
        int time = keys[vKey];
        if(!keyControl) {
            pressed = time != 0;
        } else {
            pressed = (time && type == HOLD) || (time == 1 && (type == REPEAT ||
                type == SINGLE)) || (time >= pauseTime && time % 2 == 0 && type == REPEAT);
        }
        return pressed;
    }

    unsigned char Keyboard::getKeyState(unsigned vKey) {
        return keyStates[vKey];
    }

    unsigned char Keyboard::getReadableKey(unsigned vKey, bool shift, bool ignoreCapsLock) {
        unsigned char cKey = 0u;
        if(!ignoreCapsLock)
            cKey = shift && !capsLock || capsLock && !shift
            ? readShiftKeys[vKey] : readKeys[vKey];
        else
            cKey = shift ? readShiftKeys[vKey] : readKeys[vKey];
        return cKey;
    }

    unsigned char Keyboard::getNextReadKey( ) {
        bool shift = keyPressed(VK_SHIFT, HOLD);

        unsigned char c = '\0';
        short leastTime = SHRT_MAX;
        if(GetConsoleWindow( ) == GetForegroundWindow( )) {
            for(unsigned key = 0; key < MAX_KEY_SIZE; key++) {
                if(key == VK_SHIFT)
                    continue;
                bool pressed = keyPressed(key);
                short time = keyPressedTime(key);
                if(pressed && time < leastTime) {
                    unsigned char ch = shift && !capsLock
                        || capsLock && !shift ? readShiftKeys[key]
                        : readKeys[key];
                    if(ch != '\0')
                        c = ch;
                }
            }
        }
        return c;
    }

    unsigned short Keyboard::keyPressedTime(unsigned vKey) {
        return keys[vKey];
    }

    bool Keyboard::capsLockOn( ) {
        return capsLock;
    }

    bool Keyboard::controled( ) {
        return keyControl;
    }

    bool Keyboard::controled(bool set) {
        bool oldSet = keyControl;
        keyControl = set;
        return oldSet;
    }

    unsigned short Keyboard::pause( ) {
        return pauseTime;
    }

    unsigned short Keyboard::pause(unsigned short set) {
        unsigned short oldSet = pauseTime;
        pauseTime = set;
        return oldSet;
    }


    Console::Console( ) :
        consoleHandleIn(GetStdHandle(STD_INPUT_HANDLE)),
        consoleHandleOut(GetStdHandle(STD_OUTPUT_HANDLE)),
        screenInfo( ), cursorInfo( ), oldCursorInfo( ) {

        GetConsoleCursorInfo(consoleHandleOut, &cursorInfo);
        GetConsoleCursorInfo(consoleHandleOut, &oldCursorInfo);
        GetConsoleScreenBufferInfo(consoleHandleOut, &screenInfo);

        origionalSize = screenInfo.dwSize;
        originalAttrib = screenInfo.wAttributes;
    }

    Console::~Console( ) {
        resetConsole( );
    }

    bool Console::cursorVisible( ) {
        return cursorInfo.bVisible != 0;
    }

    bool Console::cursorVisible(bool show, unsigned size) {
        bool oldShow = cursorInfo.bVisible != 0;
        cursorInfo.bVisible = show;
        cursorInfo.dwSize = size;
        SetConsoleCursorInfo(consoleHandleOut, &cursorInfo);
        return oldShow;
    }

    unsigned Console::cursorSize( ) {
        return cursorInfo.dwSize;
    }

    unsigned Console::cursorSize(unsigned size) {
        unsigned oldSize = cursorInfo.dwSize;
        cursorInfo.dwSize = size;
        SetConsoleCursorInfo(consoleHandleOut, &cursorInfo);
        return oldSize;
    }

    SIZE Console::consoleSize( ) {
        return SIZE { screenInfo.dwSize.X, screenInfo.dwSize.Y };
    }

    SIZE Console::consoleSize(SIZE size) {
        SIZE oldSize { screenInfo.dwSize.X, screenInfo.dwSize.Y };
        screenInfo.dwSize.X = size.width;
        screenInfo.dwSize.Y = size.height;
        SetConsoleScreenBufferSize(consoleHandleOut, screenInfo.dwSize);
        return oldSize;
    }

    SIZE Console::consoleSize(unsigned width, unsigned height) {
        SIZE oldSize { screenInfo.dwSize.X, screenInfo.dwSize.Y };
        screenInfo.dwSize.X = width;
        screenInfo.dwSize.Y = height;
        SetConsoleScreenBufferSize(consoleHandleOut, screenInfo.dwSize);
        return oldSize;
    }

    COORD Console::cursorPos( ) {
        COORD pos { };
        GetConsoleScreenBufferInfo(consoleHandleOut, &screenInfo);
        return screenInfo.dwCursorPosition;
    }

    COORD Console::cursorPos(COORD pos) {
        GetConsoleScreenBufferInfo(consoleHandleOut, &screenInfo);
        SetConsoleCursorPosition(consoleHandleOut, pos);
        return screenInfo.dwCursorPosition;
    }

    COORD Console::cursorPos(short x, short y) {
        COORD pos { x, y };
        GetConsoleScreenBufferInfo(consoleHandleOut, &screenInfo);
        SetConsoleCursorPosition(consoleHandleOut, pos);
        return screenInfo.dwCursorPosition;
    }

    COORD Console::cursorPosReset( ) {
        GetConsoleScreenBufferInfo(consoleHandleOut, &screenInfo);
        SetConsoleCursorPosition(consoleHandleOut, resetPos);
        return screenInfo.dwCursorPosition;
    }

    WORD Console::color( ) {
        GetConsoleScreenBufferInfo(consoleHandleOut, &screenInfo);
        return screenInfo.wAttributes;
    }

    WORD Console::color(WORD color) {
        GetConsoleScreenBufferInfo(consoleHandleOut, &screenInfo);
        SetConsoleTextAttribute(consoleHandleOut, color & 0xff);
        return screenInfo.wAttributes;
    }

    WORD Console::color(WORD foreground, WORD background) {
        GetConsoleScreenBufferInfo(consoleHandleOut, &screenInfo);
        WORD color = (foreground & 0xf) | ((background & 0xf) << 4);
        SetConsoleTextAttribute(consoleHandleOut, color);
        return screenInfo.wAttributes;
    }

    void Console::clear( ) {
        DWORD count;
        GetConsoleScreenBufferInfo(consoleHandleOut, &screenInfo);
        FillConsoleOutputCharacter(consoleHandleOut, ' ',
            screenInfo.dwSize.X * screenInfo.dwSize.Y, resetPos, &count);
        FillConsoleOutputAttribute(consoleHandleOut, screenInfo.wAttributes,
            screenInfo.dwSize.X * screenInfo.dwSize.Y, resetPos, &count);
        cursorPosReset( );
    }

    void Console::resetConsole( ) {
        SetConsoleCursorInfo(consoleHandleOut, &oldCursorInfo);
        SetConsoleScreenBufferSize(consoleHandleOut, origionalSize);
    }

    const COORD Console::resetPos = { 0, 0 };
}