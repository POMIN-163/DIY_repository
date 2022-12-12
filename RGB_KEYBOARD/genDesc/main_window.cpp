#include "genDesc.hpp"

#define HBYTE(c) (((c) >> 8) & 0xff)
#define LBYTE(c) ((c)&0xff)

// wchar_t *to_unicode(const std::string &str) {
//     std::string ustr;
//     size_t len = str.length() + 1;
//     wchar_t *wstr = (wchar_t *)malloc(sizeof(wchar_t) * len);
//
//     setlocale(LC_ALL, "");
//     mbstowcs(wstr, str.c_str(), len);
//
//     return wstr;
// }

std::string to_unicode_str(std::string str, int& length) {
    std::stringstream ss;
    size_t len = str.length();

    if (str.empty()) {
        length = 0;
        return "";
    }
    wchar_t* wstr = new wchar_t[len + 1];
    MultiByteToWideChar(65001, 0, str.c_str(), -1, wstr, len + 1);

    for (size_t i = 0; i < wcslen(wstr); i++) {
        ss << " 0x" << std::hex << std::setw(2) << setfill('0')
            << LBYTE(wstr[i]) << ", 0x" << std::setw(2) << setfill('0')
            << HBYTE(wstr[i]) << ',';
    }

    length = wcslen(wstr) * 2;
    delete[] wstr;
    return ss.str();
}

void main_window_init(void) {
    ImGuiIO& io = ImGui::GetIO();
    ImGuiStyle& style = ImGui::GetStyle();

    style.FramePadding.x = 15;
    style.FramePadding.y = 10;

    style.WindowRounding = 2;
    style.FrameRounding = 10;
    style.GrabRounding = 10;

    style.WindowPadding.x = 15;
    style.WindowPadding.y = 15;

    style.ItemSpacing.x = 10;
    style.ItemSpacing.y = 10;
    ImGui::StyleColorsLight();

    style.Colors[0].w = 0.7;
}

void main_window_client(int w, int h) {
    static bool no_close = true;
    static char str_desc[1024] = "";

    ImGuiIO& io = ImGui::GetIO();
    ImGuiStyle& style = ImGui::GetStyle();

    // 绘制窗口
    ImGui::Begin(u8"genDesc - by pomin", &no_close,
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoScrollbar);
    ImGui::SetWindowSize(ImVec2(1.0f * w, 1.0f * h));
    ImGui::SetWindowPos(ImVec2(0.0f, 0.0f));
    ImGui::InputText(u8"文本描述符", str_desc, 1024,
        ImGuiInputTextFlags_AutoSelectAll);
    int len = 0;
    stringstream ss;
    string str;
    str += str_desc;
    str = to_unicode_str(str, len);
    len += 2;
    if (!str.empty()) {
        ss << "0x" << std::hex << std::setw(2) << setfill('0') << hex << len
            << ", 0x" << std::hex << std::setw(2) << setfill('0') << hex << 3
            << "," << str;
        ImGui::TextWrapped(ss.str().c_str());
        if (ImGui::Button(u8"复制")) {
            ImGui::SetClipboardText(ss.str().c_str());
        }
    }

    ImGui::End();
}