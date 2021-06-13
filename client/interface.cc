#include "interface.h"
#include <burger/base/Log.h>

InterFace::InterFace() {
    initscr();
    if(has_colors() == FALSE) {
        printf("you terminal does not support color\n");
    }
    start_color();

    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_RED,  COLOR_BLACK);
    init_pair(3, COLOR_YELLOW,  COLOR_BLACK);

    colors_["GREEN"] = 1;
    colors_["RED"] = 2;
    colors_["YELLOW"] = 3;
    curs_set(0);
}

InterFace::~InterFace() {
    DEBUG("Destroy windows.");
    delwin(header_);
    delwin(output_);
    delwin(input_);
    // delwin(onlinelist_);
    endwin();
}

void InterFace::runHeader() {
    int y, x;
    createHeader();
    Refresh(header_);
    changeHeader();
}

void InterFace::runInput() {
    // 创建聊天输入窗口
    createInput();
    // 刷新窗口
    Refresh(input_);
    // 向聊天输入框中放置消息
    wattron(input_, COLOR_PAIR(1));
    putStrToInterFace(input_, 1, 1, "Please Enter: ");
    wattroff(input_, COLOR_PAIR(1));
    // 刷新窗口
    Refresh(input_);
}

void InterFace::runOutput() {
    // 创建聊天输出窗口
    createOutput();
    // 刷新窗口
    Refresh(output_);
    getmaxyx(output_, outputY_, outputX_);
}

void InterFace::changeHeader(const std::string& message, const std::string& color) {
    assert(header_);
    clearWin(header_);
    int x, y;
    getmaxyx(header_, y, x);
    int startY = y * 4 / 10;
    int startX = x;
    int delta = static_cast<int>(message.size() / 2);

    int colorNum = 1;  // default : GREEN
    if(colors_.find(color) != colors_.end()) {
        colorNum = colors_[color];
    }
    wattron(header_, COLOR_PAIR(colorNum));
    putStrToInterFace(header_, startY++, startX / 2 - delta, message);
    wattroff(header_, COLOR_PAIR(colorNum));
    Refresh(header_); 
}


void InterFace::outputMsg(const std::string& message, const std::string& color, bool append) {
    // 处理输出框内文字格式，防止超出边界
    if(!append) {
        clearWin(output_);
        outputStep_ = 1;
    } else {
        outputStep_ = getcury(output_) + 1; 
        // std::cout << outputStep_ << std::endl;
        if (outputStep_ == outputY_) {
            clearWin(output_);
            outputStep_ = 1;
        }
    }
    // todo : toupper
    int colorNum = 1;  // default : GREEN
    if(colors_.find(color) != colors_.end()) {
        colorNum = colors_[color];
    }
    wattron(output_, COLOR_PAIR(colorNum));
    putStrToInterFace(output_, outputStep_, 1, message);
    wattroff(output_, COLOR_PAIR(colorNum));
    Refresh(output_);
}

std::string InterFace::getInput() {
    std::string msg = getStrFromInterFace(input_);
    int x = 0, lines = 0;
    getyx(input_, lines, x);
    clearWin(input_);
    wattron(input_, COLOR_PAIR(1));
    putStrToInterFace(input_, 1, 1, "Please Enter: ");
    wattroff(input_, COLOR_PAIR(1));
    Refresh(input_);
    return msg;
}

void InterFace::createHeader() {
    int y = 0;
    int x = 0;
    int h = LINES / 5;
    int w = COLS;
    header_ = createNewInterFace(h, w, y, x);
}

void InterFace::createOutput() {
    int y = LINES / 5;
    int x = 0;
    int h = LINES * 3 / 5;
    int w = COLS;
    output_ = createNewInterFace(h, w, y, x);
}

void InterFace::createInput() {
    int y = LINES * 4 / 5;
    int x = 0;
    int h = LINES / 5;
    int w = COLS;
    input_ = createNewInterFace(h, w, y, x);
}

// void InterFace::createOnlineList() {
//     int y = LINES / 5;
//     int x = COLS * 2 / 3;
//     int h = LINES * 3 / 5;
//     int w = COLS * 1 / 3;
//     onlinelist_ = createNewInterFace(h, w, y, x);
// }

std::string InterFace::getStrFromInterFace(WINDOW* w) {
    char buf[1024] = {0};
    wgetnstr(w, buf, sizeof(buf));
    return buf;
}

void InterFace::putStrToInterFace(WINDOW* w, int startY, int startX, const std::string& message) {
    // mvwaddstr(w, startY, startX, message.c_str());
    mvwprintw(w, startY, startX,  message.c_str());
}

WINDOW* InterFace::createNewInterFace(int h, int w, int startY, int startX) {
    WINDOW* local = newwin(h, w, startY, startX);
    box(local, 0, 0);
    return local;
}

void InterFace::clearWin(WINDOW* w) {
    wmove(w, 0, 0);
    wclrtobot(w);
    box(w, 0, 0);
    Refresh(w);
}



