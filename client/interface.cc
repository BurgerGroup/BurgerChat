#include "interface.h"

InterFace::InterFace() {
    setlocale(LC_ALL,"");
    initscr();
    curs_set(0);
}

InterFace::~InterFace() {
    delwin(header_);
    delwin(output_);
    delwin(input_);
    // delwin(onlinelist_);
    endwin();
}

void InterFace::runHeader() {
    int y, x;
    std::string message = "Welcome To BurgetChat!";
    createHeader();
    Refresh(header_);
    getmaxyx(header_, y, x);
    int startY = y * 4 / 10;
    int startX = x;
    putStrToInterFace(header_, startY++, startX / 2, message);
    // message = ">> Main Menu. Enter 'help' to get help <<";
    // putStrToInterFace(header_, startY, startX / 2 - message.size() / 2, message);
    Refresh(header_); 
}

void InterFace::runInput() {
    // 创建聊天输入窗口
    createInput();
    // 刷新窗口
    Refresh(input_);
    // 向聊天输入框中放置消息
    putStrToInterFace(input_, 1, 1, "Please Enter: ");
    // 刷新窗口
    Refresh(input_);
}

void InterFace::runOutput() {
    // 创建聊天输出窗口
    createOutput();
    // 刷新窗口
    Refresh(output_);
    // 将从消息队列中获取到的数据打印到聊天输出框
    // putStrToInterFace(output_, step++, 1, message);
    // 若屏幕沾满，则 clear
    // Refresh(output_);
    getmaxyx(output_, outputY_, outputX_);
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
    int w = COLS * 2 / 3;
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
    mvwaddstr(w, startY, startX, message.c_str());
}

void InterFace::outputMsg(const std::string& message, bool append) {
    // 处理输出框内文字格式，防止超出边界
    if(!append) {
        clearWin(output_);
        outputStep_ = 1;
    }
    else {
        outputStep_ = getcury(output_); 
        std::cout << outputStep_ << std::endl;
        if (outputStep_ == outputY_ - 1) {
            clearWin(output_);
            outputStep_ = 1;
        }
    }
    putStrToInterFace(output_, outputStep_, 1, message);
    Refresh(output_);
}

std::string InterFace::getInput() {
    std::string msg = getStrFromInterFace(input_);
    int x = 0, lines = 0;
    getyx(input_, lines, x);
    clearWin(input_);
    putStrToInterFace(input_, 1, 1, "Please Enter: ");
    Refresh(input_);
    return msg;
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
}



