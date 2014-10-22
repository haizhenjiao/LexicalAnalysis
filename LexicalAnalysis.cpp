#include <iostream>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <set>
#include <vector>
#include <string>
#include <cctype>
using namespace std;

void get_digits();
bool get_comments();
int table_insert();

int state = 0, line = 0, column = 0, cnt_word = 0, cnt_char = 0;
bool notEnd = true;
string ifile, ofile;
string buffer, token;
string::iterator forward = buffer.end();
ifstream infile;
ofstream outfile, tablefile;

string words[] = {"auto", "double", "int", "struct", "break", "else", "long", "switch", "case", "enum", "register", "typedef", "char", "extern", "return", "union", "const", "float", "short", "unsigned", "continue", "for", "signed", "void", "default", "goto", "sizeof", "volatile", "do", "if", "static", "while"};
vector<string> table;

int main()
{
    set<string> keywords(words, words + 32);
    char C;
    cout << "Դ�ļ����س�Ĭ��ʹ��in.c����" << endl;
    getline(cin, ifile);

    if(ifile == "") {
        ifile = "in.c";    // �س�Ĭ��
    }

    infile.open(ifile.c_str());

    if(!infile) {
        cout << "�޷���Դ�ļ���" << endl;
        return -1;
    }

    cout << "Ŀ���ļ����س�Ĭ��ʹ��out.txt����" << endl;
    getline(cin, ofile);

    if(ofile == "") {
        ofile = "out.txt";    // �س�Ĭ��
    }

    outfile.open(ofile.c_str());

    if(!outfile) {
        cout << "�޷�����Ŀ���ļ���" << endl;
        return -1;
    }

    tablefile.open("���ű�.txt");

    if(!tablefile) {
        cout << "�޷��������ű�" << endl;
        return -1;
    }

    while(true) {
        if(forward == buffer.end()) {               // ������ĩ
            if(getline(infile, buffer, '\n')) {     // ���������EOF�򷵻�false
                ++line;                             // ��ȡ����
                cnt_char += column;                 // ÿ�λ���ǰ���ϵ�ǰ�е��ַ���
                forward = buffer.begin();
                column = 1;
            } else {
                infile.close();
                outfile.close();
                int i = 1;

                for(vector<string>::iterator it = table.begin(); it != table.end(); ++it) {
                    tablefile << left << i++ << "\t" << *it << endl;
                }

                tablefile.close();
                cout << "��������" << line << endl;
                cout << "���ʸ�����" << cnt_word << endl;
                cout << "�ַ�������" << cnt_char << endl;
                return 0;
            }
        }

        while((forward != buffer.end()) && isspace(*forward)) {     // δ������������������һֱ�����ǿ��ַ�
            ++forward;
            ++column;
        }

        if(forward != buffer.end()) {                               // δ����������ĩβ
            token = "";
            C = *forward;

            switch(C) {
            case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z': case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z': case '_':
                token.append(1, C);
                ++forward;
                ++column;

                while((forward != buffer.end()) && (isalnum(*forward) || *forward == '_')) {        // �Ϸ���ʶ���ɰ����»���
                    token.append(1, *forward++);
                    ++column;
                }

                if(keywords.count(token) == 0) {
                    outfile << "< ID," << table_insert() << " >" << endl;
                } else {
                    outfile << "< key," << token << " >" << endl;
                }

                ++cnt_word;                                      // ��������һ
                break;

            case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                token.append(1, C);
                state = 1;    // num1
                ++forward;
                ++column;
                get_digits();                                    // ��ȡ�޷���ʵ��ʣ�ಿ��
                outfile << "< num," << token << " >" << endl;   // DTB(token)
                notEnd = true;
                state = 0;
                ++cnt_word;
                break;

            case '+':
                token.append(1, C);
                ++forward;
                ++column;

                if(forward == buffer.end()) {
                    outfile << "< arith-op," << token << " >" << endl;
                } else {
                    if(*forward == '+') {
                        token.append(1, *forward++);
                        ++column;
                        outfile << "< arith-op," << token << " >" << endl;
                    } else if(*forward == '=') {
                        token.append(1, *forward++);
                        ++column;
                        outfile << "< asgn-op," << token << " >" << endl;
                    } else {
                        outfile << "< arith-op," << token << " >" << endl;
                    }
                }

                ++cnt_word;
                break;

            case '-':
                token.append(1, C);
                ++forward;
                ++column;

                if(forward == buffer.end()) {
                    outfile << "< arith-op," << token << " >" << endl;
                } else {
                    if(*forward == '-') {
                        token.append(1, *forward++);
                        ++column;
                        outfile << "< arith-op," << token << " >" << endl;
                    } else if(*forward == '=') {
                        token.append(1, *forward++);
                        ++column;
                        outfile << "< asgn-op," << token << " >" << endl;
                    } else if(*forward == '>') {
                        token.append(1, *forward++);
                        ++column;
                        outfile << "< ptr-op," << token << " >" << endl;
                    } else {
                        outfile << "< arith-op," << token << " > " << endl;
                    }
                }

                ++cnt_word;
                break;

            case '*':
                token.append(1, C);
                ++forward;
                ++column;

                if(forward == buffer.end()) {
                    outfile << "< arith-op," << token << " >" << endl;
                } else {
                    if(*forward == '=') {
                        token.append(1, *forward++);
                        ++column;
                        outfile << "< asgn-op," << token << " >" << endl;
                    } else {
                        outfile << "< arith-op," << token << " >" << endl;
                    }
                }

                ++cnt_word;
                break;

            case '/':
                token.append(1, C);
                ++forward;
                ++column;

                if(forward == buffer.end()) {
                    outfile << "< arith-op," << token << " >" << endl;           // ��ĩ����
                    ++cnt_word;
                } else {
                    if(*forward == '=') {                                        // �������ϸ�ֵ
                        token.append(1, *forward++);
                        ++column;
                        outfile << "< asgn-op," << token << " >" << endl;
                        ++cnt_word;
                    } else if(*forward == '/') {                                 // ����ע�ͣ�������ĩ
                        token.append(1, *forward++);
                        ++column;

                        while(forward != buffer.end()) {
                            token.append(1, *forward++);
                            ++column;
                        }

                        outfile << "< comments," << "- >" << endl;
                    } else if(*forward == '*') {                                 // ����ע�Ϳ��Ի���
                        token.append(1, *forward++);
                        ++column;
                        int ret = get_comments();

                        if(ret) {
                            outfile << "< comments," << "- >" << endl;
                        } else {
                            infile.close();
                            outfile.close();
                            int i = 1;

                            for(vector<string>::iterator it = table.begin(); it != table.end(); ++it) {
                                tablefile << left << i++ << "\t" << *it << endl;
                            }

                            tablefile.close();
                            cout << "���������" << line << endl;
                            cout << "���ʸ�����" << cnt_word << endl;
                            cout << "�ַ�������" << cnt_char << endl;
                            return 0;
                        }
                    } else { // ���ź��������ַ���Ϊ��������
                        outfile << "< arith-op," << token << " >" << endl;
                        ++cnt_word;
                    }
                }

                break;

            case '%':
                token.append(1, C);
                ++forward;
                ++column;

                if(forward == buffer.end()) {
                    outfile << "< arith-op," << token << " >" << endl;
                } else {
                    if(*forward == '=') {
                        token.append(1, *forward++);
                        ++column;
                        outfile << "< asgn-op," << token << " >" << endl;
                    } else {
                        outfile << "< arith-op," << token << " >" << endl;
                    }
                }

                ++cnt_word;
                break;

            case '&':
                token.append(1, C);
                ++forward;
                ++column;

                if(forward == buffer.end()) {
                    outfile << "< bit-op," << token << " >" << endl;
                } else {
                    if(*forward == '=') {
                        token.append(1, *forward++);
                        ++column;
                        outfile << "< asgn-op," << token << " >" << endl;
                    } else if(*forward == '&') {
                        token.append(1, *forward++);
                        ++column;
                        outfile << "< log-op," << token << " >" << endl;
                    } else {
                        outfile << "< bit-op," << token << " >" << endl;
                    }
                }

                ++cnt_word;
                break;

            case '|':
                token.append(1, C);
                ++forward;
                ++column;

                if(forward == buffer.end()) {
                    outfile << "< bit-op," << token << " >" << endl;
                } else {
                    if(*forward == '=') {
                        token.append(1, *forward++);
                        ++column;
                        outfile << "< asgn-op," << token << " >" << endl;
                    } else if(*forward == '|') {
                        token.append(1, *forward++);
                        ++column;
                        outfile << "< log-op," << token << " >" << endl;
                    } else {
                        outfile << "< bit-op," << token << " >" << endl;
                    }
                }

                ++cnt_word;
                break;

            case '^':
                token.append(1, C);
                ++forward;
                ++column;

                if(forward == buffer.end()) {
                    outfile << "< bit-op," << token << " >" << endl;
                } else {
                    if(*forward == '=') {
                        token.append(1, *forward++);
                        ++column;
                        outfile << "< asgn-op," << token << " >" << endl;
                    } else {
                        outfile << "< bit-op," << token << " >" << endl;
                    }
                }

                ++cnt_word;
                break;

            case '~':
                token.append(1, C);
                ++forward;
                ++column;
                outfile << "< bit-op," << token << " >" << endl;
                ++cnt_word;
                break;

            case '<':
                token.append(1, C);
                ++forward;
                ++column;

                if(forward == buffer.end()) {
                    outfile << "< rel-op," << token << " >" << endl;
                } else {
                    if(*forward == '=') {
                        token.append(1, *forward++);
                        ++column;
                        outfile << "< rel-op," << token << " >" << endl;
                    } else {
                        outfile << "< rel-op," << token << " >" << endl;
                    }
                }

                ++cnt_word;
                break;

            case '=':
                token.append(1, C);
                ++forward;
                ++column;

                if(forward == buffer.end()) {
                    outfile << "< asgn-op," << token << " >" << endl;
                } else {
                    if(*forward == '=') {
                        token.append(1, *forward++);
                        ++column;
                        outfile << "< rel-op," << token << " >" << endl;
                    } else {
                        outfile << "< asgn-op," << token << " >" << endl;
                    }
                }

                ++cnt_word;
                break;

            case '>':
                token.append(1, C);
                ++forward;
                ++column;

                if(forward == buffer.end()) {
                    outfile << "< rel-op," << token << " >" << endl;
                } else {
                    if(*forward == '=') {
                        token.append(1, *forward++);
                        ++column;
                        outfile << "< rel-op," << token << " >" << endl;
                    } else {
                        outfile << "< rel-op," << token << " >" << endl;
                    }
                }

                ++cnt_word;
                break;

            case '!':
                token.append(1, C);
                ++forward;
                ++column;

                if(forward == buffer.end()) {
                    outfile << "< punct," << token << " >" << endl;
                } else {
                    if(*forward == '=') {
                        token.append(1, *forward++);
                        ++column;
                        outfile << "< rel-op," << token << " >" << endl;
                    } else {
                        outfile << "< punct," << token << " >" << endl;
                    }
                }

                ++cnt_word;
                break;

            case '\"':
                token.append(1, C);
                ++forward;
                ++column;

                while(true) {
                    while((forward != buffer.end()) && (*forward != '\"')) {
                        token.append(1, *forward++);
                        ++column;
                    }

                    if(forward == buffer.end()) {
                        outfile << "< Error(" << line << "," << column << "): missing terminating \" character >" << endl; //error();
                        break;
                    } else if(*(forward - 1) == '\\') {
                        token.append(1, *forward++);
                        ++column;
                        continue;
                    } else {
                        token.append(1, *forward++);
                        ++column;
                        outfile << "< string," << "- >" << endl;
                        break;
                    }
                }

                ++cnt_word;
                break;

            case '\'':
                token.append(1, C);
                ++forward;
                ++column;

                while((forward != buffer.end()) && (*forward != '\'')) {
                    token.append(1, *forward++);
                    ++column;
                }

                if(forward == buffer.end()) {
                    outfile << "< Error(" << line << "," << column << "): missing terminating \' character >" << endl; //error();
                    break;
                } else if(*(forward - 1) == '\\') {
                    token.append(1, *forward++);
                    ++column;

                    if(forward != buffer.end()) {
                        token.append(1, *forward++);
                        ++column;
                    } else {
                        outfile << "< Error(" << line << "," << column << "): missing terminating \' character >" << endl; //error();
                        break;
                    }
                } else {
                    token.append(1, *forward++);
                    ++column;
                }

                outfile << "< char," << "- >" << endl;
                ++cnt_word;
                break;

            case '.':
                token.append(1, C);
                ++forward;
                ++column;

                if((forward != buffer.end()) && isdigit(*forward)) {
                    token.append(1, *forward++);
                    ++column;
                    state = 23;
                    get_digits();
                    notEnd = true;
                    state = 0;
                    outfile << "< num," << token << " >" << endl;   // DTB(token)
                    ++cnt_word;
                    break;
                }

                outfile << "< punct," << token << " >" << endl;
                ++cnt_word;
                break;

            case '#': case '{': case '}': case '[': case ']': case '(': case ')': case '?': case ':': case ',': case ';': case '\\':
                token.append(1, C);
                ++forward;
                ++column;
                outfile << "< punct," << token << " >" << endl;
                ++cnt_word;
                break;

            default:
                ++forward;
                ++column;
                outfile << "< Error(" << line << "," << column << "): invalid character >" << endl; //error();
                break;
            }    // end of switch
        }    // end of if
    }    // end of while

    return 0;
}

int table_insert()
{
    vector<string>::iterator it = find(table.begin(), table.end(), token);

    if(it == table.end()) {
        table.push_back(token);
        it = find(table.begin(), table.end(), token);
    }

    return distance(table.begin(), it) + 1;
}

void get_digits()                             // ��ȡʵ��������E/eָ��
{
    while((forward != buffer.end()) && notEnd) {
        switch(state) {
        case 1:
            if(*forward == '.') {
                token.append(1, *forward++);
                ++column;
                state = 23;
            } else if(*forward == 'E' || *forward == 'e') {
                token.append(1, *forward++);
                ++column;
                state = 4;
            } else if(isdigit(*forward)) {
                token.append(1, *forward++);
                ++column;
                state = 1;
            } else {
                notEnd = false;    // ʶ�����
            }

            break;

        case 23:
            if(*forward == 'E' || *forward == 'e') {
                token.append(1, *forward++);
                ++column;
                state = 4;
            } else if(isdigit(*forward)) {
                token.append(1, *forward++);
                ++column;
                state = 23;
            } else {
                notEnd = false;
            }

            break;

        case 4:
            if(*forward == '+' || *forward == '-') {
                token.append(1, *forward++);
                ++column;
                state = 6;
            } else if(isdigit(*forward)) {
                token.append(1, *forward++);
                ++column;
                state = 5;
            } else {
                outfile << "< Error(" << line << "," << column << "): exponent has no digits >" << endl;//error();
                notEnd = false;
            }

            break;

        case 5:
            if(isdigit(*forward)) {
                token.append(1, *forward++);
                ++column;
                state = 5;
            } else {
                notEnd = false;
            }

            break;

        case 6:
            if(isdigit(*forward)) {
                token.append(1, *forward++);
                ++column;
                state = 5;
            }

            break;

        default:
            break;
        }
    }
}

bool get_comments()
{
    while(true) {
        while((forward != buffer.end()) && (*forward != '*')) {
            token.append(1, *forward++);
            ++column;
        }

        if(forward == buffer.end()) {                            // һ��ע�ͽ�����������һ��
            if(getline(infile, buffer, '\n')) {                  // ���������EOF�򷵻�false
                ++line;
                forward = buffer.begin();
                column = 1;
                continue;
            } else {
                return false;
            }
        } else {                                                 // ����ע�ͽ��������Ǻ�
            token.append(1, *forward++);
            ++column;

            if(forward == buffer.end()) {                        // �� '*/' �����зֿ����������ȡ��һ��
                continue;
            } else if(*forward == '/') {                         // ע�ͽ�����ok
                token.append(1, *forward++);
                ++column;
                return true;
            } else {                                             // �Ǻź��������ַ����Լ�����ȡע��
                continue;
            }
        }
    }
}
