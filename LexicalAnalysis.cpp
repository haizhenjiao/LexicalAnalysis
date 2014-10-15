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
    string ifile, ofile;
    string buffer, token;
    string::iterator forward = buffer.end();
    int state = 0;
    int line = 0, column = 0;
    char C;
    bool notEnd = true;

    cout << "Please input the source file name (press Enter to use 'in.c') :" << endl;
    getline(cin, ifile);
    if (ifile == "")
        ifile = "in.c";    // �س�Ĭ��

    ifstream infile(ifile.c_str());
    if (!infile)
    {
        cout << "Cannot open the source file!" << endl;
        return 0;
    }
    cout << "Please input the target file name (press Enter to use 'out.txt') :" << endl;
    getline(cin, ofile);
    if (ofile == "")
        ofile = "out.txt";    // �س�Ĭ��

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
            if (getline(infile, buffer, '\n'))      // ���������EOF�򷵻�false
            {
//                outfile << buffer << endl;
                ++line;
                forward = buffer.begin();
            }
            else
            {
                infile.close();
                outfile.close();
                return 0;
            }
        }
        while ((forward != buffer.end()) && isspace(*forward))      // δ������������������һֱ�����ǿ��ַ�
        {
            ++forward;
        }
        if (forward != buffer.end())    // δ��������������
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
                state = 1;    // num1

                ++forward;
num23:
                while ((forward != buffer.end()) && notEnd)
                {
                    switch (state)
                    {
                    case 1:
                        if (*forward == '.')
                        {
                            token.append(1, *forward);
                            ++forward;
                            state = 23;
                        }
                        else if (*forward == 'E' || *forward == 'e')
                        {
                            token.append(1, *forward);
                            ++forward;
                            state = 4;
                        }
                        else if (isdigit(*forward))
                        {
                            token.append(1, *forward);
                            ++forward;
                            state = 1;
                        }
                        else
                        {
                            notEnd = false;    // ʶ�����
                        }
                        break;
                    case 23:
                        if (*forward == 'E' || *forward == 'e')
                        {
                            token.append(1, *forward);
                            ++forward;
                            state = 4;
                        }
                        else if (isdigit(*forward))
                        {
                            token.append(1, *forward);
                            ++forward;
                            state = 23;
                        }
                        else
                        {
                            notEnd = false;
                        }
                        break;
                    case 4:
                        if (*forward == '+' || *forward == '-')
                        {
                            token.append(1, *forward);
                            ++forward;
                            state = 6;
                        }
                        else if (isdigit(*forward))
                        {
                            token.append(1, *forward);
                            ++forward;
                            state = 5;
                        }
                        else
                        {
                            outfile << "< Error: exponent has no digits >" << endl;//error();
                            notEnd = false;
                        }
                        break;
                    case 5:
                        if (isdigit(*forward))
                        {
                            token.append(1, *forward);
                            ++forward;
                            state = 5;
                        }
                        else
                        {
                            notEnd = false;
                        }
                        break;
                    case 6:
                        if (isdigit(*forward))
                        {
                            token.append(1, *forward);
                            ++forward;
                            state = 5;
                        }
                        break;
                    default:
                        break;
                    }
                }

                outfile << "< num, " << token << " >" << endl; //DTB(token)
                token = "";
                notEnd = true;
                state = 0;
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
                    outfile << "< arith-op, " << token << " >" << endl;    // ��ĩ����
                else
                {
                    if (*forward == '=')    // ������ֵ
                    {
                        token.append(1, *forward);
                        ++forward;
                        outfile << "< assign-op, " << token << " >" << endl;
                    }
                    else if (*forward == '/')    // ����ע�ͣ�������ĩ
                    {
                        token.append(1, *forward);
                        ++forward;
                        while (forward != buffer.end())
                        {
                            token.append(1, *forward);
                            ++forward;
                        }
                        outfile << "< comments, - >" << endl;
                    }
                    else if (*forward == '*')      // ����ע�Ϳ��Ի���
                    {
                        token.append(1, *forward);
                        ++forward;
loop:
                        while ((forward != buffer.end()) && (*forward != '*'))
                        {
                            token.append(1, *forward);
                            ++forward;
                        }
                        if (forward == buffer.end())    // һ��ע�ͽ�����������һ��
                        {
                            if (getline(infile, buffer, '\n'))      // ���������EOF�򷵻�false
                            {
                                ++line;
                                forward = buffer.begin();
                                goto loop;
                            }
                            else
                            {
                                infile.close();
                                outfile.close();
                                return 0;
                            }
                        }
                        else    // ����ע�ͽ��������Ǻ�
                        {
                            token.append(1, *forward);
                            ++forward;
                            if (forward == buffer.end())   // �� */ �����зֿ��������¶�ȡ
                            {
                                goto loop;
                            }
                            else if (*forward == '/')   // ע�ͽ�����ok
                            {
                                token.append(1, *forward);
                                ++forward;
                                outfile << "< comments, - >" << endl;

                            }
                            else    // �Ǻź��������ַ����Լ�����ȡע��
                            {
                                goto loop;
                            }
                        }
                    }
                    else    // ���ź��������ַ���Ϊ��������
                    {
                        outfile << "< arith-op, " << token << " >" << endl;
                    }
                }
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
                }
                token = "";
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
                }
                token = "";
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
                }
                token = "";
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
                }
                token = "";
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
                }
                token = "";
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
                }
                token = "";
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
                }
                token = "";
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
                }
                token = "";
                break;

            case '\"':
                token.append(1, C);
                ++forward;
again:
                while ((forward != buffer.end()) && (*forward != '\"'))
                {
                    token.append(1, *forward);
                    ++forward;
                }
                if (forward == buffer.end())
                {
                    outfile << "< Error: missing terminating \" character >" << endl; //error();
                    token = "";
                    break;
                }
                else if (*(forward - 1) == '\\')
                {
                    token.append(1, *forward);
                    ++forward;
                    goto again;
                }
                else
                {
                    token.append(1, *forward);
                    ++forward;
                }
                outfile << "< string, - >" << endl;
                token = "";
                break;

            case '.':
                token.append(1, C);
                ++forward;
                if ((forward != buffer.end()) && isdigit(*forward))
                {
                    token.append(1, *forward);
                    ++forward;
                    state = 23;
                    goto num23;
                }
                outfile << "< other-op, " << token << " >" << endl;
                token = "";
                break;

            case '#': case '{': case '}': case '[': case ']': case '(': case ')': case '?': case ',': case ';': case '\'': case '\\':
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

    return 0;
}
