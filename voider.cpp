#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <fstream>
#include <random>
#include <chrono>
#include <thread>
#include <cmath>
#include <iomanip>
#include <sstream>

#ifdef _WIN32
#include <windows.h>
#ifndef NOMINMAX
#define NOMINMAX
#endif
#endif

namespace Colors {
    constexpr const char* Reset = "\033[0m";
    constexpr const char* Red = "\033[31m";
    constexpr const char* Yellow = "\033[33m";
    constexpr const char* Cyan = "\033[36m";
    constexpr const char* Green = "\033[32m";
    constexpr const char* Rainbow[] = {
        "\033[31m", "\033[33m", "\033[32m", "\033[36m", "\033[34m", "\033[35m"
    };
}

#ifdef _WIN32
class Console {
public:
    static void init() {
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hConsole != INVALID_HANDLE_VALUE) {
            GetConsoleScreenBufferInfo(hConsole, &originalInfo);
        }
        enableVirtualTerminal();
    }

    static void setColor(int colorIndex) {
        if (hConsole == INVALID_HANDLE_VALUE) return;
        
        WORD attrs = originalInfo.wAttributes;
        attrs &= ~(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        
        switch(colorIndex % 6) {
            case 0: attrs |= FOREGROUND_RED | FOREGROUND_INTENSITY; break;
            case 1: attrs |= FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY; break;
            case 2: attrs |= FOREGROUND_GREEN | FOREGROUND_INTENSITY; break;
            case 3: attrs |= FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY; break;
            case 4: attrs |= FOREGROUND_BLUE | FOREGROUND_INTENSITY; break;
            case 5: attrs |= FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY; break;
        }
        SetConsoleTextAttribute(hConsole, attrs);
    }

    static void setYellow() {
        if (hConsole == INVALID_HANDLE_VALUE) return;
        WORD attrs = originalInfo.wAttributes;
        attrs &= ~(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        attrs |= FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
        SetConsoleTextAttribute(hConsole, attrs);
    }

    static void setCyan() {
        if (hConsole == INVALID_HANDLE_VALUE) return;
        WORD attrs = originalInfo.wAttributes;
        attrs &= ~(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        attrs |= FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
        SetConsoleTextAttribute(hConsole, attrs);
    }

    static void reset() {
        if (hConsole == INVALID_HANDLE_VALUE) return;
        SetConsoleTextAttribute(hConsole, originalInfo.wAttributes);
    }

    static void clearLine() {
        if (hConsole == INVALID_HANDLE_VALUE) return;
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
            COORD coord = {0, csbi.dwCursorPosition.Y};
            DWORD written;
            FillConsoleOutputCharacterA(hConsole, ' ', csbi.dwSize.X, coord, &written);
            SetConsoleCursorPosition(hConsole, coord);
        }
    }

    static bool isWindows() { return true; }
    
    static std::string borderStart(int line, int total) {
        if (line == 0) return "+ ";
        else if (line == total - 1) return "+ ";
        else return "| ";
    }
    
    static std::string borderEnd(int line, int total) {
        if (line == 0) return " +";
        else if (line == total - 1) return " +";
        else return " |";
    }

private:
    static HANDLE hConsole;
    static CONSOLE_SCREEN_BUFFER_INFO originalInfo;

    static void enableVirtualTerminal() {
        DWORD dwMode = 0;
        if (GetConsoleMode(hConsole, &dwMode)) {
            dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(hConsole, dwMode);
        }
    }
};

HANDLE Console::hConsole = INVALID_HANDLE_VALUE;
CONSOLE_SCREEN_BUFFER_INFO Console::originalInfo = {};

#else
class Console {
public:
    static void init() {}

    static void setColor(int colorIndex) {
        std::cout << Colors::Rainbow[colorIndex % 6];
    }

    static void setYellow() {
        std::cout << Colors::Yellow;
    }

    static void setCyan() {
        std::cout << Colors::Cyan;
    }

    static void reset() {
        std::cout << Colors::Reset;
    }

    static void clearLine() {
        std::cout << "\r\x1b[K" << std::flush;
    }

    static bool isWindows() { return false; }
    
    static std::string borderStart(int line, int total) {
        if (line == 0) return "\u250c ";
        else if (line == total - 1) return "\u2514 ";
        else return "\u2502 ";
    }
    
    static std::string borderEnd(int line, int total) {
        if (line == 0) return " \u2510";
        else if (line == total - 1) return " \u2518";
        else return " \u2502";
    }
};
#endif

class RandomEngine {
public:
    static RandomEngine& instance() {
        static RandomEngine inst;
        return inst;
    }

    std::mt19937& gen() {
        return engine;
    }

    int getInt(int min, int max) {
        std::uniform_int_distribution<> dist(min, max);
        return dist(engine);
    }

private:
    RandomEngine() {
        std::random_device rd;
        engine.seed(rd());
    }
    std::mt19937 engine;
};

std::string toLower(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return result;
}

std::string toUpper(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return std::toupper(c); });
    return result;
}

std::string capitalizeStr(const std::string& str) {
    if (str.empty()) return str;
    std::string result = str;
    result[0] = std::toupper(static_cast<unsigned char>(result[0]));
    for (size_t i = 1; i < result.length(); ++i) {
        result[i] = std::tolower(static_cast<unsigned char>(result[i]));
    }
    return result;
}

std::string reverseStr(const std::string& str) {
    std::string result = str;
    std::reverse(result.begin(), result.end());
    return result;
}

std::string repeat(const std::string& str, int times, const std::string& separator) {
    if (times <= 0) return "";
    std::string result;
    size_t estimatedSize = str.length() * times + separator.length() * (times - 1);
    result.reserve(estimatedSize);
    for (int i = 0; i < times; ++i) {
        result += str;
        if (i < times - 1) result += separator;
    }
    return result;
}

std::string stripColorCodes(const std::string& str) {
    std::string result;
    result.reserve(str.length());
    for (size_t i = 0; i < str.length(); ++i) {
        if (str[i] == '\033' && i + 1 < str.length() && str[i+1] == '[') {
            i += 2;
            while (i < str.length() && !std::isalpha(static_cast<unsigned char>(str[i]))) {
                ++i;
            }
        } else {
            result += str[i];
        }
    }
    return result;
}

std::string glitch(const std::string& str, int intensity) {
    std::string result = str;
    auto& gen = RandomEngine::instance().gen();
    std::uniform_int_distribution<> glitchChar(33, 126);
    std::uniform_real_distribution<> prob(0.0, 1.0);

    for (size_t i = 0; i < result.length(); ++i) {
        if (result[i] == '\033') {
            while (i < result.length() && result[i] != 'm') ++i;
            continue;
        }
        if (prob(gen) < 0.1 * intensity) {
            result[i] = static_cast<char>(glitchChar(gen));
        }
    }
    return result;
}

void printRainbowText(const std::string& text, int offset) {
    for (size_t i = 0; i < text.length(); ++i) {
        Console::setColor(offset + static_cast<int>(i));
        std::cout << text[i];
    }
    Console::reset();
}

void printLine(int lineIndex, int totalLines, const std::string& content, 
               bool hasNumber, bool hasBorder, bool rainbow) {
    if (hasNumber) {
        Console::setYellow();
        std::cout << (lineIndex + 1) << ": ";
        Console::reset();
    }
    
    if (hasBorder) {
        Console::setCyan();
        std::cout << Console::borderStart(lineIndex, totalLines);
        Console::reset();
    }
    
    if (rainbow) {
        printRainbowText(content, lineIndex);
    } else {
        std::cout << content;
    }
    
    if (hasBorder) {
        std::cout << " ";
        Console::setCyan();
        std::cout << Console::borderEnd(lineIndex, totalLines);
        Console::reset();
    }
    
    std::cout << std::endl;
}

void clearLine() {
    Console::clearLine();
}

void typewriterEffect(const std::string& text, int delayMs, int effect, int param) {
    if (delayMs <= 0) {
        std::cout << text << std::endl;
        return;
    }

    if (effect == 0) {
        for (size_t i = 0; i <= text.length(); ++i) {
            clearLine();
            std::cout << text.substr(0, i);
            std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
        }
        std::cout << std::endl;
    }
    else if (effect == 1) {
        int wave = (param > 0) ? param : 3;
        for (size_t i = 0; i <= text.length(); ++i) {
            clearLine();
            std::cout << text.substr(0, i);
            for (int j = 1; j <= wave && i + j <= text.length(); ++j) {
                std::cout << text[i + j - 1];
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
        }
        std::cout << std::endl;
    }
    else if (effect == 2) {
        std::vector<std::string> frames;
        size_t len = text.length();
        for (size_t i = 0; i <= len; ++i) {
            std::string frame;
            frame.reserve(len);
            for (size_t j = 0; j < len; ++j) {
                if (j < i) frame += text[j];
                else if (j == i && i < len) frame += "#";
                else frame += ".";
            }
            frames.push_back(frame);
        }
        for (size_t i = len; i > 0; --i) {
            std::string frame;
            frame.reserve(len);
            for (size_t j = 0; j < len; ++j) {
                if (j < i - 1) frame += text[j];
                else if (j == i - 1) frame += "#";
                else frame += ".";
            }
            frames.push_back(frame);
        }
        frames.push_back(text);

        for (const auto& frame : frames) {
            clearLine();
            std::cout << frame;
            std::this_thread::sleep_for(std::chrono::milliseconds(delayMs / 2));
        }
        std::cout << std::endl;
    }
    else if (effect == 3) {
        int steps = (param > 0) ? param : 10;
        for (int i = 0; i <= steps; ++i) {
            float progress = static_cast<float>(i) / steps;
            int pos = static_cast<int>(progress * text.length());
            clearLine();
            std::cout << text.substr(0, pos);
            if (pos < static_cast<int>(text.length())) {
                std::cout << "_";
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
        }
        clearLine();
        std::cout << text << std::endl;
    }
    else if (effect == 4) {
        size_t len = text.length();
        for (int i = len; i >= 0; --i) {
            clearLine();
            std::cout << std::string(len - i, ' ') << text.substr(0, i);
            std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
        }
        std::cout << std::endl;
    }
    else if (effect == 5) {
        size_t len = text.length();
        std::string current(len, '_');
        std::vector<bool> revealed(len, false);
        int revealedCount = 0;
        auto& gen = RandomEngine::instance().gen();
        std::uniform_int_distribution<> dis(0, len - 1);

        while (revealedCount < static_cast<int>(len)) {
            int idx = dis(gen);
            if (!revealed[idx]) {
                revealed[idx] = true;
                current[idx] = text[idx];
                revealedCount++;
                clearLine();
                std::cout << current;
                std::this_thread::sleep_for(std::chrono::milliseconds(delayMs * 2));
            }
        }
        std::cout << std::endl;
    }
    else {
        std::cout << text << std::endl;
    }
}

void printUsage(const char* programName) {
    std::cout << "voider v0.1\n\n";
    std::cout << "by quik // unlicense\n\n";
    std::cout << "usage: " << programName << " [options] <lines> <repetitions> [word]\n\n";
    std::cout << "MAIN OPTIONS:\n";
    std::cout << "  -u, --uppercase     - convert to UPPERCASE\n";
    std::cout << "  -l, --lowercase     - convert to lowercase\n";
    std::cout << "  -c, --capitalize    - capitalize first letter\n";
    std::cout << "  -r, --reverse       - reverse the word\n";
    std::cout << "  -s, --separator <char> - separator between repetitions\n";
    std::cout << "  -p, --pattern <n>   - pattern (1-normal, 2-alternating)\n\n";

    std::cout << "APPEARANCE EFFECTS:\n";
    std::cout << "  -d, --delay <ms>    - delay between characters\n";
    std::cout << "  -e, --effect <n>    - effect type:\n";
    std::cout << "                        0 - normal typewriter\n";
    std::cout << "                        1 - wave (with -w parameter)\n";
    std::cout << "                        2 - scanning\n";
    std::cout << "                        3 - blinking cursor\n";
    std::cout << "                        4 - reverse effect\n";
    std::cout << "                        5 - random reveal\n";
    std::cout << "  -w, --wave <size>   - wave size (for effect 1)\n";
    std::cout << "  -f, --fade          - fade out at the end\n\n";

    std::cout << "VISUAL EFFECTS:\n";
    std::cout << "  -b, --border        - add border\n";
    std::cout << "  -n, --number        - number lines\n";
    std::cout << "  -a, --rainbow       - rainbow text\n";
    std::cout << "  -g, --glitch <n>    - glitch effect (1-3 intensity)\n";
    std::cout << "  -m, --mirror        - mirror display\n";
    std::cout << "  -x, --matrix        - matrix style\n\n";

    std::cout << "ADDITIONAL:\n";
    std::cout << "  -o, --output <file> - save to file\n";
    std::cout << "  -h, --help          - show this help\n\n";

    std::cout << "EXAMPLES:\n";
    std::cout << "  " << programName << " 3 4 void\n";
    std::cout << "  " << programName << " -d 50 -e 1 -w 3 3 3 hello\n";
    std::cout << "  " << programName << " -a -b -n 4 2 rainbow\n";
    std::cout << "  " << programName << " -d 30 -e 2 -f 3 3 matrix\n";
    std::cout << "  " << programName << " -g 2 -x 2 5 glitch\n";
}

struct Config {
    bool upper = false;
    bool lower = false;
    bool capitalize = false;
    bool reverse = false;
    bool addNumbers = false;
    bool addBorder = false;
    bool fadeOut = false;
    bool mirror = false;
    bool rainbow = false;
    bool matrix = false;
    std::string separator = " ";
    std::string outputFile;
    int pattern = 1;
    int delayMs = 0;
    int effectType = 0;
    int waveSize = 0;
    int glitchIntensity = 0;
    int lines = 0;
    int repetitions = 0;
    std::string word;
};

bool parseArgs(int argc, char* argv[], Config& cfg) {
    if (argc < 2) {
        printUsage(argv[0]);
        return false;
    }

    std::vector<std::string> args;
    for (int i = 1; i < argc; ++i) {
        args.push_back(argv[i]);
    }

    std::vector<std::string> positionalArgs;

    for (size_t i = 0; i < args.size(); ++i) {
        const std::string& arg = args[i];

        if (arg == "-h" || arg == "--help") {
            printUsage(argv[0]);
            return false;
        } else if (arg == "-u" || arg == "--uppercase") {
            cfg.upper = true;
        } else if (arg == "-l" || arg == "--lowercase") {
            cfg.lower = true;
        } else if (arg == "-c" || arg == "--capitalize") {
            cfg.capitalize = true;
        } else if (arg == "-r" || arg == "--reverse") {
            cfg.reverse = true;
        } else if (arg == "-n" || arg == "--number") {
            cfg.addNumbers = true;
        } else if (arg == "-b" || arg == "--border") {
            cfg.addBorder = true;
        } else if (arg == "-f" || arg == "--fade") {
            cfg.fadeOut = true;
        } else if (arg == "-m" || arg == "--mirror") {
            cfg.mirror = true;
        } else if (arg == "-a" || arg == "--rainbow") {
            cfg.rainbow = true;
        } else if (arg == "-x" || arg == "--matrix") {
            cfg.matrix = true;
        } else if (arg == "-s" || arg == "--separator") {
            if (i + 1 < args.size()) {
                cfg.separator = args[++i];
            }
        } else if (arg == "-o" || arg == "--output") {
            if (i + 1 < args.size()) {
                cfg.outputFile = args[++i];
            }
        } else if (arg == "-d" || arg == "--delay") {
            if (i + 1 < args.size()) {
                cfg.delayMs = std::atoi(args[++i].c_str());
                if (cfg.delayMs < 0) cfg.delayMs = 0;
            }
        } else if (arg == "-e" || arg == "--effect") {
            if (i + 1 < args.size()) {
                cfg.effectType = std::atoi(args[++i].c_str());
                if (cfg.effectType < 0 || cfg.effectType > 5) cfg.effectType = 0;
            }
        } else if (arg == "-w" || arg == "--wave") {
            if (i + 1 < args.size()) {
                cfg.waveSize = std::atoi(args[++i].c_str());
                if (cfg.waveSize < 1) cfg.waveSize = 1;
            }
        } else if (arg == "-g" || arg == "--glitch") {
            if (i + 1 < args.size()) {
                cfg.glitchIntensity = std::atoi(args[++i].c_str());
                if (cfg.glitchIntensity < 1 || cfg.glitchIntensity > 3) cfg.glitchIntensity = 1;
            }
        } else if (arg == "-p" || arg == "--pattern") {
            if (i + 1 < args.size()) {
                cfg.pattern = std::atoi(args[++i].c_str());
                if (cfg.pattern < 1 || cfg.pattern > 2) cfg.pattern = 1;
            }
        } else {
            positionalArgs.push_back(arg);
        }
    }

    if (positionalArgs.size() < 2) {
        std::cout << "Error: missing required arguments.\n";
        printUsage(argv[0]);
        return false;
    }

    cfg.lines = std::atoi(positionalArgs[0].c_str());
    cfg.repetitions = std::atoi(positionalArgs[1].c_str());

    if (positionalArgs.size() >= 3) {
        cfg.word = positionalArgs[2];
    }

    if (cfg.lines <= 0 || cfg.repetitions <= 0) {
        std::cout << "Error: number of lines and repetitions must be positive.\n";
        return false;
    }

    return true;
}

int main(int argc, char* argv[]) {
    Console::init();

    Config cfg;
    if (!parseArgs(argc, argv, cfg)) {
        return (argc < 2) ? 0 : 1;
    }

    std::string processedWord = cfg.word;

    if (cfg.upper) processedWord = toUpper(processedWord);
    else if (cfg.lower) processedWord = toLower(processedWord);
    else if (cfg.capitalize) processedWord = capitalizeStr(processedWord);

    if (cfg.reverse) processedWord = reverseStr(processedWord);

    if (cfg.glitchIntensity > 0) {
        processedWord = glitch(processedWord, cfg.glitchIntensity);
    }

    std::string cleanWord = stripColorCodes(processedWord);

    if (cfg.mirror) {
        std::string reversed = cleanWord;
        std::reverse(reversed.begin(), reversed.end());
        cleanWord = cleanWord + " | " + reversed;
    }

    std::vector<std::string> contentLines;
    contentLines.reserve(cfg.lines);

    for (int i = 0; i < cfg.lines; ++i) {
        std::string content;
        if (cfg.pattern == 1) {
            content = repeat(cleanWord, cfg.repetitions, cfg.separator);
        } else if (cfg.pattern == 2) {
            if (i % 2 == 0) {
                content = repeat(toUpper(cleanWord), cfg.repetitions, cfg.separator);
            } else {
                content = repeat(toLower(cleanWord), cfg.repetitions, cfg.separator);
            }
        }
        contentLines.push_back(content);
    }

    if (!cfg.outputFile.empty()) {
        std::ofstream file(cfg.outputFile);
        if (file.is_open()) {
            for (int i = 0; i < cfg.lines; ++i) {
                std::string line;
                if (cfg.addNumbers) {
                    line += std::to_string(i + 1) + ": ";
                }
                if (cfg.addBorder) {
                    line += Console::borderStart(i, cfg.lines);
                }
                line += contentLines[i];
                if (cfg.addBorder) {
                    line += " ";
                    line += Console::borderEnd(i, cfg.lines);
                }
                file << line << std::endl;
            }
            file.close();
            std::cout << "Result saved to " << cfg.outputFile << std::endl;
        } else {
            std::cout << "Error: could not open file " << cfg.outputFile << std::endl;
        }
    }

    if (cfg.delayMs > 0) {
        for (size_t i = 0; i < contentLines.size(); ++i) {
            std::string displayLine;
            if (cfg.addNumbers) {
                displayLine += std::to_string(i + 1) + ": ";
            }
            if (cfg.addBorder) {
                displayLine += Console::borderStart(i, cfg.lines);
            }
            displayLine += contentLines[i];
            if (cfg.addBorder) {
                displayLine += " ";
                displayLine += Console::borderEnd(i, cfg.lines);
            }

            std::string cleanDisplay = stripColorCodes(displayLine);
            typewriterEffect(cleanDisplay, cfg.delayMs, cfg.effectType, cfg.waveSize);

            if (cfg.fadeOut && i == contentLines.size() - 1) {
                std::this_thread::sleep_for(std::chrono::milliseconds(cfg.delayMs * 3));
                for (int j = cleanDisplay.length(); j >= 0; --j) {
                    clearLine();
                    std::cout << cleanDisplay.substr(0, j);
                    std::this_thread::sleep_for(std::chrono::milliseconds(cfg.delayMs / 2));
                }
                clearLine();
            }

            if (i < contentLines.size() - 1) {
                std::this_thread::sleep_for(std::chrono::milliseconds(cfg.delayMs * 2));
            }
        }
    } else {
        for (int i = 0; i < cfg.lines; ++i) {
            printLine(i, cfg.lines, contentLines[i], 
                     cfg.addNumbers, cfg.addBorder, cfg.rainbow);
        }
    }

    return 0;
}
