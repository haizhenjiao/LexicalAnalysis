#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include <string>
#include <cctype>
using namespace std;

string words[] = {"auto", "double", "int", "struct", "break", "else", "long", "switch", "case", "enum", "register", "typedef", "char", "extern", "return", "union", "const", "float", "short", "unsigned", "continue", "for", "signed", "void", "default", "goto", "sizeof", "volatile", "do", "if", "static", "while"};

int main()
{
    set<string> keywords(words, words + 32);
    string ifile, ofile;
    string buffer, token;
    string::iterator forward = buffer.end();
    stringstream ss;
    int num = 0;
    int line = 0, column = 0;
    char C;

    cout << "Please input the source file name (press Enter to use 'in.c') :" << endl;
    getline(cin, ifile);
    if(ifile == "")
        ifile = "in.c";    // 回车默认

    ifstream infile(ifile.c_str());
    if (!infile)
    {
        cout << "Cannot open the source file!" << endl;
        return 0;
    }
    cout << "Please input the target file name (press Enter to use 'out.txt') :" << endl;
    getline(cin, ofile);
    if(ofile == "")
        ofile = "out.txt";    // 回车默认

    ofstream outfile(ofile.c_str());
    if (!outfile)
    {
        cout << "Cannot open the target file!" << endl;
        return 0;
    }

    while (1)
    {
        if (forward == buffer.end())
        {
            if (getline(infile, buffer, '\n'))      // 读到错误或EOF则返回false
            {
//                outfile << buffer << endl;
                ++line;
                forward = buffer.begin();
            }
            else
            {
                break;
            }
        }
        while ((forward != buffer.end()) && isspace(*forward))      // 未读到缓冲区结束，则一直读到非空字符
        {
            ++forward;
        }
        if (forward != buffer.end())    // 未读到缓冲区结束
        {
            C = *forward;
            switch (C)
            {
            case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z': case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z': case '_':
                token.append(1, C);
                ++forward;
                while ((forward != buffer.end()) && (isalnum(*forward) || *forward == '_'))
                {
                    token.append(1, *forward);
                    ++forward;
                }
                if (keywords.count(token) == 0)
                {
                    outfile << "< ID, " << token << " >" << endl;
                }
                else
                {
                    outfile << "< key, " << token << " >" << endl;
                }
                token = "";
                break;

            case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                token.append(1, C);
                ++forward;
                while ((forward != buffer.end()) && isdigit(*forward))
                {
                    token.append(1, *forward);
                    ++forward;
                }
                ss << token;
                ss >> num;
                outfile << "< num, " << num << " >" << endl;//DTB()
                token = "";
                break;

            case '+':
                token.append(1, C);
                ++forward;
                if (forward == buffer.end())
                    outfile << "< arith-op, " << token << " >" << endl;
                else
                {
                    if (*forward == '+')
                    {
                        token.append(1, *forward);
                        ++forward;
                        outfile << "< arith-op, " << token << " >" << endl;
                    }
                    else if (*forward == '=')
                    {
                        token.append(1, *forward);
                        ++forward;
                        outfile << "< assign-op, " << token << " >" << endl;
                    }
                    else
                    {
                        outfile << "<arith-op, " << token << " >" << endl;
                    }
                }
                token = "";
                break;

            case '-':
                token.append(1, C);
                ++forward;
                if (forward == buffer.end())
                    outfile << "< arith-op, " << token << " >" << endl;
                else
                {
                    if (*forward == '-')
                    {
                        token.append(1, *forward);
                        ++forward;
                        outfile << "< arith-op, " << token << " >" << endl;
                    }
                    else if (*forward == '=')
                    {
                        token.append(1, *forward);
                        ++forward;
                        outfile << "< assign-op, " << token << " >" << endl;
                    }
                    else if (*forward == '>')
                    {
                        token.append(1, *forward);
                        ++forward;
                        outfile << "< ptr-op, " << token << " >" << endl;
                    }
                    else
                    {
                        outfile << "< arith-op, " << token << " > " << endl;
                    }
                }
                token = "";
                break;

            case '*':
                token.append(1, C);
                ++forward;
                if (forward == buffer.end())
                    outfile << "< arith-op, " << token << " >" << endl;
                else
                {
                    if (*forward == '=')
                    {
                        token.append(1, *forward);
                        ++forward;
                        outfile << "< assign-op, " << token << " >" << endl;
                    }
                    else
                    {
                        outfile << "< arith-op, " << token << " >" << endl;
                    }
                }
                token = "";
                break;

            case '/':
                token.append(1, C);
                ++forward;
                if (forward == buffer.end())
                    outfile << "< arith-op, " << token << " >" << endl;
                else
                {
                    if (*forward == '=')
                    {
                        token.append(1, *forward);
                        ++forward;
                        outfile << "< assign-op, " << token << " >" << endl;
                    }
                    else if (*forward == '/')
                    {
                        token.append(1, *forward);
                        forward = buffer.end();
                    }
                    else if (*forward == '*')      ////////////// 注释可以换行
                    {
                        token.append(1, *forward);
loop:
                        while (*forward != '*')
                            ++forward;
                        ++forward;
                        if (*forward == '/')
                        {
                            ++forward;
                            goto end;
                        }
                        goto loop;
                    }
                    else
                    {
                        outfile << "< arith-op, " << token << " >" << endl;
                    }
                }
end:
                token = "";
                break;

            case '%':
                token.append(1, C);
                ++forward;
                if (forward == buffer.end())
                    outfile << "< arith-op, " << token << " >" << endl;
                else
                {
                    if (*forward == '=')
                    {
                        token.append(1, *forward);
                        ++forward;
                        outfile << "< assign-op, " << token << " >" << endl;
                    }
                    else
                    {
                        outfile << "< arith-op, " << token << " >" << endl;
                    }
                }           token = "";
                break;

            case '&':
                token.append(1, C);
                ++forward;
                if (forward == buffer.end())
                    outfile << "< bit-op, " << token << " >" << endl;
                else
                {
                    if (*forward == '=')
                    {
                        token.append(1, *forward);
                        ++forward;
                        outfile << "< assign-op, " << token << " >" << endl;
                    }
                    else if (*forward == '&')
                    {
                        token.append(1, *forward);
                        ++forward;
                        outfile << "< log-op, " << token << " >" << endl;
                    }
                    else
                    {
                        outfile << "< bit-op, " << token << " >" << endl;
                    }
                }           token = "";
                break;

            case '|':
                token.append(1, C);
                ++forward;
                if (forward == buffer.end())
                    outfile << "< bit-op, " << token << " >" << endl;
                else
                {
                    if (*forward == '=')
                    {
                        token.append(1, *forward);
                        ++forward;
                        outfile << "< assign-op, " << token << " >" << endl;
                    }
                    else if (*forward == '|')
                    {
                        token.append(1, *forward);
                        ++forward;
                        outfile << "< log-op, " << token << " >" << endl;
                    }
                    else
                    {
                        outfile << "< bit-op, " << token << " >" << endl;
                    }
                }           token = "";
                break;

            case '^':
                token.append(1, C);
                ++forward;
                if (forward == buffer.end())
                    outfile << "< bit-op, " << token << " >" << endl;
                else
                {
                    if (*forward == '=')
                    {
                        token.append(1, *forward);
                        ++forward;
                        outfile << "< assign-op, " << token << " >" << endl;
                    }
                    else
                    {
                        outfile << "< bit-op, " << token << " >" << endl;
                    }
                }           token = "";
                break;

            case '~':
                token.append(1, C);
                ++forward;

                outfile << "< bit-op, " << token << " >" << endl;
                token = "";
                break;

            case '<':
                token.append(1, C);
                ++forward;
                if (forward == buffer.end())
                    outfile << "< rel-op, " << token << " >" << endl;
                else
                {
                    if (*forward == '=')
                    {
                        token.append(1, *forward);
                        ++forward;
                        outfile << "< rel-op, LE >" << endl;
                    }
                    else
                    {
                        outfile << "< rel-op, LT >" << endl;
                    }
                }           token = "";
                break;

            case '=':
                token.append(1, C);
                ++forward;
                if (forward == buffer.end())
                    outfile << "< assign-op, - >" << endl;
                else
                {
                    if (*forward == '=')
                    {
                        token.append(1, *forward);
                        ++forward;
                        outfile << "< rel-op, EQ >" << endl;
                    }
                    else
                    {
                        outfile << "< assign-op, - >" << endl;
                    }
                }           token = "";
                break;

            case '>':
                token.append(1, C);
                ++forward;
                if (forward == buffer.end())
                    outfile << "< rel-op, GT > " << endl;
                else
                {
                    if (*forward == '=')
                    {
                        token.append(1, *forward);
                        ++forward;
                        outfile << "< rel-op, GE >" << endl;
                    }
                    else
                    {
                        outfile << "< rel-op, GT >" << endl;
                    }
                }           token = "";
                break;

            case '!':
                token.append(1, C);
                ++forward;
                if (forward == buffer.end())
                    outfile << "< other-op, " << token << " >" << endl;
                else
                {
                    if (*forward == '=')
                    {
                        token.append(1, *forward);
                        ++forward;
                        outfile << "< rel-op, NE >" << endl;
                    }
                    else
                    {
                        outfile << "< other-op, " << token << " >" << endl;
                    }
                }           token = "";
                break;

            case '\"':
                token.append(1, C);
                ++forward;

                while ((forward != buffer.end()) && (*forward != '\"'))
                {
                    token.append(1, *forward);
                    ++forward;
                }
                if (forward == buffer.end())
                {
                    outfile << "< Error: excepted '\"' >" << endl; //error();
                    token = "";
                    break;
                }
                ++forward;
                outfile << "< string, - >" << endl;
                token = "";
                break;

            case '#': case '{': case '}': case '[': case ']': case '(': case ')': case '.': case '?': case ',': case ';': case '\'': case '\\':
                token.append(1, C);
                ++forward;
                outfile << "< other-op, " << token << " >" << endl;
                token = "";
                break;

            default:
                //error();
                break;
            }    // end of switch
        }    // end of if
    }    // end of while

    infile.close();
    outfile.close();

    return 0;
}
