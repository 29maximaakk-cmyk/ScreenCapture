// screenshot.cpp
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <chrono>
#include <thread>

#ifdef _WIN32
#include <windows.h>
#elif __APPLE__
// macOS использует screencapture
#else
// Linux использует scrot или gnome-screenshot
#endif

using namespace std;

string getTimestamp() {
    time_t now = time(nullptr);
    char buf[80];
    strftime(buf, sizeof(buf), "%Y%m%d_%H%M%S", localtime(&now));
    return string(buf);
}

void executeCommand(const string& cmd) {
    int ret = system(cmd.c_str());
    if (ret != 0) {
        cerr << "Ошибка выполнения команды: " << cmd << endl;
        exit(1);
    }
}

int main(int argc, char* argv[]) {
    string output = "";
    int delay = 0;
    string region = "";
    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "-o" && i+1 < argc) output = argv[++i];
        else if (arg == "-d" && i+1 < argc) delay = stoi(argv[++i]);
        else if (arg == "-r" && i+1 < argc) region = argv[++i];
        else if (arg == "-h" || arg == "--help") {
            cout << "Usage: screenshot [options]\n"
                 << "  -o <file>   Output file\n"
                 << "  -d <sec>    Delay in seconds\n"
                 << "  -r <x,y,w,h> Region\n"
                 << "  -h          Help\n";
            return 0;
        }
    }

    if (delay > 0) {
        cout << "Waiting " << delay << " seconds..." << endl;
        this_thread::sleep_for(chrono::seconds(delay));
    }

    if (output.empty()) {
        output = "screenshot_" + getTimestamp() + ".png";
    }

#ifdef _WIN32
    // Используем PowerShell для захвата экрана
    string cmd = "powershell -command \"Add-Type -AssemblyName System.Drawing; " \
                 "$bitmap = New-Object System.Drawing.Bitmap([System.Windows.Forms.Screen]::PrimaryScreen.Bounds.Width, " \
                 "[System.Windows.Forms.Screen]::PrimaryScreen.Bounds.Height); " \
                 "$graphics = [System.Drawing.Graphics]::FromImage($bitmap); " \
                 "$graphics.CopyFromScreen(0,0,0,0,$bitmap.Size); " \
                 "$bitmap.Save('" + output + "', [System.Drawing.Imaging.ImageFormat]::Png);\"";
    executeCommand(cmd);
#elif __APPLE__
    string cmd = "screencapture -x " + output;
    if (!region.empty()) {
        // screencapture не поддерживает регион, используем -R x,y,w,h
        // но формат отличается: -R x,y,w,h
        // заменяем запятые на пробелы? Нет, -R x,y,w,h
        cmd = "screencapture -R " + region + " " + output;
    }
    executeCommand(cmd);
#else
    // Linux: используем scrot или gnome-screenshot
    string cmd;
    if (system("which scrot > /dev/null 2>&1") == 0) {
        cmd = "scrot " + output;
        if (!region.empty()) {
            // scrot поддерживает -s для выбора области, но не координаты.
            // Используем imagemagick import для региона.
            // Проверим наличие import
            if (system("which import > /dev/null 2>&1") == 0) {
                // region в формате x,y,w,h
                stringstream ss(region);
                int x, y, w, h;
                char ch;
                ss >> x >> ch >> y >> ch >> w >> ch >> h;
                cmd = "import -window root -crop " + to_string(w) + "x" + to_string(h) + "+" + to_string(x) + "+" + to_string(y) + " " + output;
            } else {
                cerr << "Для региона требуется установить ImageMagick (import)" << endl;
                return 1;
            }
        }
    } else if (system("which gnome-screenshot > /dev/null 2>&1") == 0) {
        cmd = "gnome-screenshot -f " + output;
        if (!region.empty()) {
            cerr << "gnome-screenshot не поддерживает регион." << endl;
            return 1;
        }
    } else {
        cerr << "Не найдена утилита для скриншотов (scrot, gnome-screenshot или import)." << endl;
        return 1;
    }
    executeCommand(cmd);
#endif
    cout << "Скриншот сохранён в " << output << endl;
    return 0;
}
