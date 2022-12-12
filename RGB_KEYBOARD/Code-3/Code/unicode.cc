/**
 * @author: Pomin
 * @date: 2022-12-11 13:57:19
 * @github: https://github.com/POMIN-163
 * @lastedit: 2022-12-11 13:57:19
 * @description: gbk Âë×ª Unicode Âë
 */
#include <Windows.h>
#include <wchar.h>

#include <iostream>
#include <string>

#define HBYTE(c) (((c) >> 8) & 0xff)
#define LBYTE(c) ((c)&0xff)

wchar_t *to_unicode(const std::string &str) {
    std::string ustr;
    char char_unicode[5];
    size_t len = str.length() + 1;
    static wchar_t *wstr = (wchar_t *)malloc(sizeof(wchar_t) * len);

    setlocale(LC_ALL, "");
    mbstowcs(wstr, str.c_str(), len);

    return wstr;
}

wchar_t *to_unicode2(const std::string &str) {
    size_t len = str.length();
    static wchar_t *wstr = (wchar_t *)malloc(sizeof(wchar_t) * len);
    MultiByteToWideChar(936, 0, str.c_str(), -1, wstr, len);
    return wstr;
}

int main(int argc, char const *argv[]) {
    if (argc == 1) return 1;
    std::string str = argv[1];

    {
        wchar_t *res = to_unicode(str);
        for (size_t i = 0; i < wcslen(res); i++) {
            printf("0x%02x, ", LBYTE(res[i]));
            printf("0x%02x, ", HBYTE(res[i]));
        }
    }
    std::cout << std::endl;
    {
        wchar_t *res = to_unicode2(str);
        for (size_t i = 0; i < wcslen(res); i++) {
            printf("0x%02x, ", LBYTE(res[i]));
            printf("0x%02x, ", HBYTE(res[i]));
        }
    }
    return 0;
}
