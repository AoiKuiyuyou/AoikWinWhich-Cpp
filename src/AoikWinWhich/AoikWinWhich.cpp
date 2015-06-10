//
#include "stdafx.h"
#include <iostream>
#include <string>
#include <list>
#include <set>
#include <algorithm>
#include <windows.h>
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

//
using namespace std;

//
#ifdef _UNICODE
#define WA_COUT wcout
#define WA_STRING wstring
#else
#define WA_COUT cout
#define WA_STRING string
#endif

// Modified from http://stackoverflow.com/questions/236129/split-a-string-in-c/7408245#7408245
// --- BEG
void string_split(list<WA_STRING> &tokens, const WA_STRING &text, char sep) {
    int start = 0, end = 0;
    while ((end = text.find(sep, start)) != WA_STRING::npos) {
        tokens.push_back(text.substr(start, end - start));
        start = end + 1;
    }
    tokens.push_back(text.substr(start));
}
// --- END

// Modified from http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring/15649849#15649849
// --- BEG
WA_STRING string_strip(const WA_STRING& s) {
    const WA_STRING& chars = TEXT(" ");
    size_t begin = 0;
    size_t end = s.size() - 1;
    for (; begin < s.size(); begin++)
        if (chars.find_first_of(s[begin]) == WA_STRING::npos)
            break;
    for (; end > begin; end--)
        if (chars.find_first_of(s[end]) == WA_STRING::npos)
            break;
    return s.substr(begin, end - begin + 1);
}
// --- END

//
WA_STRING string_tolower(WA_STRING str) {
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

// Modified from http://stackoverflow.com/questions/874134/find-if-string-endswith-another-string-in-c/874160#874160
BOOL string_endswith(WA_STRING const &str, WA_STRING const &end) {
    if (str.length() >= end.length()) {
        return (0 == str.compare(str.length() - end.length(), end.length(), end));
    }
    else {
        return false;
    }
}

//
void list_strip(list<WA_STRING>& item_s) {
    std::transform(item_s.begin(), item_s.end(), item_s.begin(), string_strip);
}

//
void list_remove_empty(list<WA_STRING>& item_s) {
    list<WA_STRING>::iterator iter = item_s.begin();

    while (iter != item_s.end()) {
        // if current item is empty string
        if (*iter == TEXT("")) {
            iter = item_s.erase(iter);
        }
        else {
            ++iter;
        }
    }
}

//
void list_tolower(list<WA_STRING>& item_s) {
    std::transform(item_s.begin(), item_s.end(), item_s.begin(), string_tolower);
}

// Modified from http://stackoverflow.com/questions/4885676/remove-duplicates-from-a-listint/4885787#4885787
// --- BEG
void list_uniq(list<WA_STRING>& item_s) {
    list<WA_STRING>::iterator iter = item_s.begin();

    set<WA_STRING> item_s_met;

    while (iter != item_s.end()) {
        // if current item has been met before
        if (item_s_met.find(*iter) != item_s_met.end()) {
            iter = item_s.erase(iter);
        }
        else {
            item_s_met.insert(*iter);
            ++iter;
        }
    }
}
// --- END

//
BOOL prog_has_ext_in(WA_STRING prog, list<WA_STRING> ext_s) {
    for (list<WA_STRING>::const_iterator iter = ext_s.begin(); iter != ext_s.end(); ++iter) {
        if (string_endswith(prog, *iter)) {
            return true;
        }
    }
    return false;
}

//
BOOL file_exists(LPCTSTR szPath) {
    DWORD dwAttrib = GetFileAttributes(szPath);

    return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
        !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

//
list<WA_STRING>* find_exe_paths(LPCTSTR prog) {
    // An environment variable has a maximum size limit of 32,767 characters,
    // including the null-terminating character.
    TCHAR buf[32767];

    // 8f1kRCu
    DWORD res = GetEnvironmentVariable(TEXT("PATHEXT"), buf, 32767);

    // 4fpQ2RB
    // If "res" indicates error
    if (res == 0) {
        return NULL;
    }

    // 6qhHTHF
    // Split into a list of extensions
    list<WA_STRING> ext_s;

    WA_STRING env_pathext(buf);

    string_split(ext_s, env_pathext, ';');

    // 2pGJrMW
    // Strip
    list_strip(ext_s);

    // 2gqeHHl
    // Remove empty.
    // Must be done after the stripping at 2pGJrMW.
    list_remove_empty(ext_s);

    // 2zdGM8W
    // Convert to lowercase
    list_tolower(ext_s);

    // 2fT8aRB
    // Uniquify
    list_uniq(ext_s);

    //
    list<WA_STRING> dir_path_s;

    // 4ysaQVN
    res = GetEnvironmentVariable(TEXT("PATH"), buf, sizeof buf);

    // 5gGwKZL
    // If "res" indicates error
    if (res == 0) {
        // 7bVmOKe
        // Go ahead with "dir_path_s" being empty
        ;
    }
    else {
        // 6mPI0lg
        // Split into a list of dir paths
        WA_STRING env_path(buf);

        string_split(dir_path_s, env_path, ';');
    }

    // 5rT49zI
    // Insert empty dir path to the beginning.
    //
    // Empty dir handles the case that "prog" is a path, either relative or
    // absolute. See code 7rO7NIN.
    dir_path_s.push_front(TEXT(""));

    // 2klTv20
    // Uniquify
    list_uniq(dir_path_s);

    // 9gTU1rI
    // Check if "prog" ends with one of the file extension in "ext_s".
    //
    // "ext_s" are all in lowercase, ensured at 2zdGM8W.
    BOOL prog_has_ext = prog_has_ext_in(string_tolower(prog), ext_s);

    // 6bFwhbv
    list<WA_STRING> *exe_path_s = new list<WA_STRING>();

    for (list<WA_STRING>::const_iterator iter = dir_path_s.begin(); iter != dir_path_s.end(); ++iter) {
        // 7rO7NIN
        // Synthesize a path
        WA_STRING dir_path = *iter;

        WA_STRING path;

        // If dir path is empty string
        if (dir_path == TEXT("")) {
            path = WA_STRING(prog);
        }
        else {
            // "PathCombine" will not writes more than MAX_PATH chars to "buf"
            assert(sizeof buf >= MAX_PATH);

            LPTSTR res = PathCombine(buf, dir_path.c_str(), prog);

            // If "res" indicates error
            if (res == NULL) {
                // Ignore
                continue;
            }

            //
            path = WA_STRING(buf);
        }

        // 6kZa5cq
        // If "prog" ends with executable file extension
        if (prog_has_ext) {
            // 3whKebE
            if (file_exists(path.c_str())) {
                // 2ffmxRF
                exe_path_s->push_back(path);
            }
        }

        // 2sJhhEV
        // Assume user has omitted the file extension
        for (list<WA_STRING>::const_iterator iter = ext_s.begin(); iter != ext_s.end(); ++iter) {
            // 6k9X6GP
            // Synthesize a path with one of the file extensions in PATHEXT
            WA_STRING ext = *iter;

            WA_STRING path_2 = path + ext;

            // 6kabzQg
            if (file_exists(path_2.c_str())) {
                exe_path_s->push_back(path_2);
            }
        }
    }

    // 8swW6Av
    // Uniquify
    list_uniq(*exe_path_s);

    // 7y3JlnS
    return exe_path_s;
}

// 4zKrqsC
// Program entry
int _tmain(int argc, _TCHAR* argv[])
{
    //
    int exit_code = 0;

    // 9mlJlKg
    // If not exactly one command argument is given
    if (argc != 2) {
        // 7rOUXFo
        // print program usage
        string usage_txt = "Usage: aoikwinwhich PROG\n"
            "\n"
            "#/ PROG can be either name or path\n"
            "aoikwinwhich notepad.exe\n"
            "aoikwinwhich C:\\Windows\\notepad.exe\n"
            "\n"
            "#/ PROG can be either absolute or relative\n"
            "aoikwinwhich C:\\Windows\\notepad.exe\n"
            "aoikwinwhich Windows\\notepad.exe\n"
            "\n"
            "#/ PROG can be either with or without extension\n"
            "aoikwinwhich notepad.exe\n"
            "aoikwinwhich notepad\n"
            "aoikwinwhich C:\\Windows\\notepad.exe\n"
            "aoikwinwhich C:\\Windows\\notepad";

        //
        cout << usage_txt << endl;

        // 3nqHnP7
        exit_code = 1;

        return exit_code;
    }

    // 9m5B08H
    // Get executable name or path
    LPCTSTR prog = argv[1];

    // 8ulvPXM
    // Find executable paths
    //
    // Freed at 2jUVFP0
    list<WA_STRING>* exe_path_s = find_exe_paths(prog);

    // 5fWrcaF
    // If has found none
    if (!exe_path_s || !exe_path_s->size()) {
        // 3uswpx0
        exit_code = 2;
    }
    // If has found some
    else {
        // 9xPCWuS
        // Print result
        for (list<WA_STRING>::const_iterator iter = exe_path_s->begin();
            iter != exe_path_s->end();
            ++iter) {
            WA_COUT << WA_STRING(*iter) << endl;
        }

        // 4s1yY1b
        exit_code = 0;
    }

    // 2jUVFP0
    if (exe_path_s) {
        delete(exe_path_s);
    }

    //
    return exit_code;
}
