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
    cout << "Դ�ļ����س�Ĭ��ʹ��in.c����" << endl;
    getline(cin, ifile);

    if(ifile == "") {
        ifile = "in.c";    // �س�Ĭ��
    }

    ifstream infile(ifile.c_str());

    if(!infile) {
        cout << "�޷���Դ�ļ���" << endl;
        return 0;
    }

    cout << "Ŀ���ļ����س�Ĭ��ʹ��out.txt����" << endl;
    getline(cin, ofile);

    if(ofile == "") {
        ofile = "out.txt";    // �س�Ĭ��
    }

    ofstream outfile(ofile.c_str());

    if(!outfile) {
        cout << "�޷�����Ŀ���ļ���" << endl;
        return 0;
    }

    while(true) {
        if(forward == buffer.end()) {               // ������ĩ
            if(getline(infile, buffer, '\n')) {     // ���������EOF�򷵻�false
                ++line;                             // ��ȡ����
                forward = buffer.begin();
            } else {
                infile.close();
                outfile.close();
                cout << "��������" << line << endl;
                return 0;
            }
        }

        while((forward != buffer.end()) && isspace(*forward)) {     // δ������������������һֱ�����ǿ��ַ�
            ++forward;
        }

        if(forward != buffer.end()) {                               // δ����������ĩβ
            token = "";
            C = *forward;

            switch(C) {
            case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z': case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z': case '_':
                token.append(1, C);
                ++forward;

                while((forward != buffer.end()) && (isalnum(*forward) || *forward == '_')) {        // �Ϸ���ʶ���ɰ����»���
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

                while((forward != buffer.end()) && notEnd) {                 // ��ȡʵ��������E/eָ��
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
                            notEnd = false;    // ʶ�����
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
                    outfile << "< arith-op," << token << " >" << endl;    // ��ĩ����
                } else {
                    if(*forward == '=') {                                        // ������ֵ
                        token.append(1, *forward++);
                        outfile << "< asgn-op," << token << " >" << endl;
                    } else if(*forward == '/') {                                 // ����ע�ͣ�������ĩ
                        token.append(1, *forward++);

                        while(forward != buffer.end()) {
                            token.append(1, *forward++);
                        }

                        outfile << "< comments," << "- >" << endl;
                    } else if(*forward == '*') {                                 // ����ע�Ϳ��Ի���
                        token.append(1, *forward++);
label_loop:

                        while((forward != buffer.end()) && (*forward != '*')) {
                            token.append(1, *forward++);
                        }

                        if(forward == buffer.end()) {                            // һ��ע�ͽ�����������һ��
                            if(getline(infile, buffer, '\n')) {                  // ���������EOF�򷵻�false
                                ++line;
                                forward = buffer.begin();
                                goto label_loop;
                            } else {
                                infile.close();
                                outfile.close();
                                cout << "��������" << line << endl;
                                return 0;
                            }
                        } else {                                                 // ����ע�ͽ��������Ǻ�
                            token.append(1, *forward++);

                            if(forward == buffer.end()) {                        // �� '*/' �����зֿ��������¶�ȡ
                                goto label_loop;
                            } else if(*forward == '/') {                         // ע�ͽ�����ok
                                token.append(1, *forward++);
                                outfile << "< comments," << "- >" << endl;
                            } else {                                             // �Ǻź��������ַ����Լ�����ȡע��
                                goto label_loop;
                            }
                        }
                    } else { // ���ź��������ַ���Ϊ��������
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
