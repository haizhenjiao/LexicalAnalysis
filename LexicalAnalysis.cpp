#include <iostream>
#include <fstream>
#include <set>
#include <string>
#include <cctype>
using namespace std;

string words[] = {"auto", "double", "int", "struct", "break", "else", "long", "switch", "case", "enum", "register", "typedef", "char", "extern", "return", "union", "const", "float", "short", "unsigned", "continue", "for", "signed", "void", "default", "goto", "sizeof", "volatile", "do", "if", "static", "while"};

int main()
{
    set<string> keywords(words, words + 32);
    string ifile, ofile, buffer, token;
    string::iterator forward = buffer.end();
    int state = 0, line = 0, column = 0;
    char C;
    bool notEnd = true;
    cout << "源文件（回车默认使用in.c）：" << endl;
    getline(cin, ifile);

    if(ifile == "") {
        ifile = "in.c";    // 回车默认
    }

    ifstream infile(ifile.c_str());

    if(!infile) {
        cout << "无法打开源文件！" << endl;
        return 0;
    }

    cout << "目标文件（回车默认使用out.txt）：" << endl;
    getline(cin, ofile);

    if(ofile == "") {
        ofile = "out.txt";    // 回车默认
    }

    ofstream outfile(ofile.c_str());

    if(!outfile) {
        cout << "无法创建目标文件！" << endl;
        return 0;
    }

    while(true) {
        if(forward == buffer.end()) {               // 读到行末
            if(getline(infile, buffer, '\n')) {     // 读到错误或EOF则返回false
                ++line;                             // 读取新行
                forward = buffer.begin();
            } else {
                infile.close();
                outfile.close();
                cout << "总行数：" << line << endl;
                return 0;
            }
        }

        while((forward != buffer.end()) && isspace(*forward)) {     // 未读到缓冲区结束，则一直读到非空字符
            ++forward;
        }

        if(forward != buffer.end()) {                               // 未读到缓冲区末尾
            token = "";
            C = *forward;

            switch(C) {
            case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z': case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z': case '_':
                token.append(1, C);
                ++forward;

                while((forward != buffer.end()) && (isalnum(*forward) || *forward == '_')) {        // 合法标识符可包含下划线
                    token.append(1, *forward++);
                }

                if(keywords.count(token) == 0) {
                    outfile << "< ID," << token << " >" << endl;
                } else {
                    outfile << "< key," << token << " >" << endl;
                }

                break;

            case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                token.append(1, C);
                state = 1;    // num1
                ++forward;
label_num:

                while((forward != buffer.end()) && notEnd) {                 // 读取实数，包含E/e指数
                    switch(state) {
                    case 1:
                        if(*forward == '.') {
                            token.append(1, *forward++);
                            state = 23;
                        } else if(*forward == 'E' || *forward == 'e') {
                            token.append(1, *forward++);
                            state = 4;
                        } else if(isdigit(*forward)) {
                            token.append(1, *forward++);
                            state = 1;
                        } else {
                            notEnd = false;    // 识别结束
                        }

                        break;

                    case 23:
                        if(*forward == 'E' || *forward == 'e') {
                            token.append(1, *forward++);
                            state = 4;
                        } else if(isdigit(*forward)) {
                            token.append(1, *forward++);
                            state = 23;
                        } else {
                            notEnd = false;
                        }

                        break;

                    case 4:
                        if(*forward == '+' || *forward == '-') {
                            token.append(1, *forward++);
                            state = 6;
                        } else if(isdigit(*forward)) {
                            token.append(1, *forward++);
                            state = 5;
                        } else {
                            outfile << "< Error:" << "exponent has no digits >" << endl;//error();
                            notEnd = false;
                        }

                        break;

                    case 5:
                        if(isdigit(*forward)) {
                            token.append(1, *forward++);
                            state = 5;
                        } else {
                            notEnd = false;
                        }

                        break;

                    case 6:
                        if(isdigit(*forward)) {
                            token.append(1, *forward++);
                            state = 5;
                        }

                        break;

                    default:
                        break;
                    }
                }

                outfile << "< num," << token << " >" << endl; //DTB(token)
                notEnd = true;
                state = 0;
                break;

            case '+':
                token.append(1, C);
                ++forward;

                if(forward == buffer.end()) {
                    outfile << "< arith-op," << token << " >" << endl;
                } else {
                    if(*forward == '+') {
                        token.append(1, *forward++);
                        outfile << "< arith-op," << token << " >" << endl;
                    } else if(*forward == '=') {
                        token.append(1, *forward++);
                        outfile << "< asgn-op," << token << " >" << endl;
                    } else {
                        outfile << "< arith-op," << token << " >" << endl;
                    }
                }

                break;

            case '-':
                token.append(1, C);
                ++forward;

                if(forward == buffer.end()) {
                    outfile << "< arith-op," << token << " >" << endl;
                } else {
                    if(*forward == '-') {
                        token.append(1, *forward++);
                        outfile << "< arith-op," << token << " >" << endl;
                    } else if(*forward == '=') {
                        token.append(1, *forward++);
                        outfile << "< asgn-op," << token << " >" << endl;
                    } else if(*forward == '>') {
                        token.append(1, *forward++);
                        outfile << "< ptr-op," << token << " >" << endl;
                    } else {
                        outfile << "< arith-op," << token << " > " << endl;
                    }
                }

                break;

            case '*':
                token.append(1, C);
                ++forward;

                if(forward == buffer.end()) {
                    outfile << "< arith-op," << token << " >" << endl;
                } else {
                    if(*forward == '=') {
                        token.append(1, *forward++);
                        outfile << "< asgn-op," << token << " >" << endl;
                    } else {
                        outfile << "< arith-op," << token << " >" << endl;
                    }
                }

                break;

            case '/':
                token.append(1, C);
                ++forward;

                if(forward == buffer.end()) {
                    outfile << "< arith-op," << token << " >" << endl;    // 行末除号
                } else {
                    if(*forward == '=') {                                        // 除法赋值
                        token.append(1, *forward++);
                        outfile << "< asgn-op," << token << " >" << endl;
                    } else if(*forward == '/') {                                 // 单行注释，读到行末
                        token.append(1, *forward++);

                        while(forward != buffer.end()) {
                            token.append(1, *forward++);
                        }

                        outfile << "< comments," << "- >" << endl;
                    } else if(*forward == '*') {                                 // 多行注释可以换行
                        token.append(1, *forward++);
label_loop:

                        while((forward != buffer.end()) && (*forward != '*')) {
                            token.append(1, *forward++);
                        }

                        if(forward == buffer.end()) {                            // 一行注释结束，继续下一行
                            if(getline(infile, buffer, '\n')) {                  // 读到错误或EOF则返回false
                                ++line;
                                forward = buffer.begin();
                                goto label_loop;
                            } else {
                                infile.close();
                                outfile.close();
                                cout << "总行数：" << line << endl;
                                return 0;
                            }
                        } else {                                                 // 读到注释结束符的星号
                            token.append(1, *forward++);

                            if(forward == buffer.end()) {                        // 若 '*/' 被换行分开，则重新读取
                                goto label_loop;
                            } else if(*forward == '/') {                         // 注释结束符ok
                                token.append(1, *forward++);
                                outfile << "< comments," << "- >" << endl;
                            } else {                                             // 星号后是其他字符，仍继续读取注释
                                goto label_loop;
                            }
                        }
                    } else { // 除号后是其他字符，为单个除号
                        outfile << "< arith-op," << token << " >" << endl;
                    }
                }

                break;

            case '%':
                token.append(1, C);
                ++forward;

                if(forward == buffer.end()) {
                    outfile << "< arith-op," << token << " >" << endl;
                } else {
                    if(*forward == '=') {
                        token.append(1, *forward++);
                        outfile << "< asgn-op," << token << " >" << endl;
                    } else {
                        outfile << "< arith-op," << token << " >" << endl;
                    }
                }

                break;

            case '&':
                token.append(1, C);
                ++forward;

                if(forward == buffer.end()) {
                    outfile << "< bit-op," << token << " >" << endl;
                } else {
                    if(*forward == '=') {
                        token.append(1, *forward++);
                        outfile << "< asgn-op," << token << " >" << endl;
                    } else if(*forward == '&') {
                        token.append(1, *forward++);
                        outfile << "< log-op," << token << " >" << endl;
                    } else {
                        outfile << "< bit-op," << token << " >" << endl;
                    }
                }

                break;

            case '|':
                token.append(1, C);
                ++forward;

                if(forward == buffer.end()) {
                    outfile << "< bit-op," << token << " >" << endl;
                } else {
                    if(*forward == '=') {
                        token.append(1, *forward++);
                        outfile << "< asgn-op," << token << " >" << endl;
                    } else if(*forward == '|') {
                        token.append(1, *forward++);
                        outfile << "< log-op," << token << " >" << endl;
                    } else {
                        outfile << "< bit-op," << token << " >" << endl;
                    }
                }

                break;

            case '^':
                token.append(1, C);
                ++forward;

                if(forward == buffer.end()) {
                    outfile << "< bit-op," << token << " >" << endl;
                } else {
                    if(*forward == '=') {
                        token.append(1, *forward++);
                        outfile << "< asgn-op," << token << " >" << endl;
                    } else {
                        outfile << "< bit-op," << token << " >" << endl;
                    }
                }

                break;

            case '~':
                token.append(1, C);
                ++forward;
                outfile << "< bit-op," << token << " >" << endl;
                break;

            case '<':
                token.append(1, C);
                ++forward;

                if(forward == buffer.end()) {
                    outfile << "< rel-op," << token << " >" << endl;
                } else {
                    if(*forward == '=') {
                        token.append(1, *forward++);
                        outfile << "< rel-op," << token << " >" << endl;
                    } else {
                        outfile << "< rel-op," << token << " >" << endl;
                    }
                }

                break;

            case '=':
                token.append(1, C);
                ++forward;

                if(forward == buffer.end()) {
                    outfile << "< asgn-op," << token << " >" << endl;
                } else {
                    if(*forward == '=') {
                        token.append(1, *forward++);
                        outfile << "< rel-op," << token << " >" << endl;
                    } else {
                        outfile << "< asgn-op," << token << " >" << endl;
                    }
                }

                break;

            case '>':
                token.append(1, C);
                ++forward;

                if(forward == buffer.end()) {
                    outfile << "< rel-op," << token << " >" << endl;
                } else {
                    if(*forward == '=') {
                        token.append(1, *forward++);
                        outfile << "< rel-op," << token << " >" << endl;
                    } else {
                        outfile << "< rel-op," << token << " >" << endl;
                    }
                }

                break;

            case '!':
                token.append(1, C);
                ++forward;

                if(forward == buffer.end()) {
                    outfile << "< punct," << token << " >" << endl;
                } else {
                    if(*forward == '=') {
                        token.append(1, *forward++);
                        outfile << "< rel-op," << token << " >" << endl;
                    } else {
                        outfile << "< punct," << token << " >" << endl;
                    }
                }

                break;

            case '\"':
                token.append(1, C);
                ++forward;
label_str:

                while((forward != buffer.end()) && (*forward != '\"')) {
                    token.append(1, *forward++);
                }

                if(forward == buffer.end()) {
                    outfile << "< Error: missing terminating \" character >" << endl; //error();
                    break;
                } else if(*(forward - 1) == '\\') {
                    token.append(1, *forward++);
                    goto label_str;
                } else {
                    token.append(1, *forward++);
                }

                outfile << "< string," << "- >" << endl;
                break;

            case '\'':
                token.append(1, C);
                ++forward;

                while((forward != buffer.end()) && (*forward != '\'')) {
                    token.append(1, *forward++);
                }

                if(forward == buffer.end()) {
                    outfile << "< Error: missing terminating \' character >" << endl; //error();
                    break;
                } else if(*(forward - 1) == '\\') {
                    token.append(1, *forward++);

                    if(forward != buffer.end()) {
                        token.append(1, *forward++);
                    } else {
                        outfile << "< Error:" << "missing terminating \' character >" << endl; //error();
                        break;
                    }
                } else {
                    token.append(1, *forward++);
                }

                outfile << "< char," << "- >" << endl;
                break;

            case '.':
                token.append(1, C);
                ++forward;

                if((forward != buffer.end()) && isdigit(*forward)) {
                    token.append(1, *forward++);
                    state = 23;
                    goto label_num;
                }

                outfile << "< punct," << token << " >" << endl;
                break;

            case '#': case '{': case '}': case '[': case ']': case '(': case ')': case '?': case ':': case ',': case ';': case '\\':
                token.append(1, C);
                ++forward;
                outfile << "< punct," << token << " >" << endl;
                break;

            default:
                //error();
                break;
            }    // end of switch
        }    // end of if
    }    // end of while

    return 0;
}
