#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
using namespace std;
#include "opt.h"

/**
 * @author llf
 *
 * 全局预处理
 */



struct BigLabel
{
    string label;
    int id;
};

/*
* 判断字符串是不是数字
*/
bool isnum(string s)
{
    stringstream sin(s);
    double t;
    char p;
    if (!(sin >> t))
        /*解释：
            sin>>t表示把sin转换成double的变量（其实对于int和float型的都会接收），如果转换成功，则值为非0，如果转换不成功就返回为0
        */
        return false;
    if (sin >> p)
        /*解释：此部分用于检测错误输入中，数字加字符串的输入形式（例如：34.f），在上面的的部分（sin>>t）已经接收并转换了输入的数字部分，在stringstream中相应也会把那一部分给清除，如果此时传入字符串是数字加字符串的输入形式，则此部分可以识别并接收字符部分，例如上面所说的，接收的是.f这部分，所以条件成立，返回false;如果剩下的部分不是字符，那么则sin>>p就为0,则进行到下一步else里面
          */
        return false;
    else
        return true;
}

/*
* 把字符串中的addr信息删除
*/
void removeADDR(string& s)
{
    string::size_type idx_addr = s.find("addr");
    if (idx_addr != string::npos)
    {
        string s_temp1 = s.substr(idx_addr + 5);
        string::size_type idx_right = s_temp1.find(")");
        int pos_num = idx_right;
        string s_temp2 = s_temp1.substr(0, pos_num);
        s = s_temp2;
    }
}

/*
* 把标号识别提取出来
*/
string loadLabel(const string s1, string& s2)
{
    string label = "#";
    // 找冒号
    string::size_type idx_mh = s1.find(":");

    if (idx_mh != string::npos)  // 找到冒号
    {
        if (s1[idx_mh + 1] == ' ') // 是label
        {
            int pos_num = idx_mh + 1;
            label = s1.substr(0, pos_num);
            s2 = s1.substr(pos_num + 1);
        }
    }

    // 没有或者不是Label，不做操作

    return label;
}

/*
* 模拟回填
*/
void simuBackpatch(const string callabel, const int fourPos, const vector<BigLabel>& Label, vector<Quaternary>& res)
{
    for (int i = 0; i < Label.size(); i++)
    {
        if (callabel == (Label[i].label)) // 开始回填
        {
            res[Label[i].id].res = to_string(fourPos);
        }
    }
}

/*
* 三地址代码转成四元式
* 实际上表示为：vector<string>转成tac[]
* 很多、很多的if else语句判断，可直接跳至586行
*/
vector<Quaternary> three2four(const vector<string> codes)
{

    vector<Quaternary> res;

    vector<BigLabel> Label;

    int tac1Num = 0; // 公用四元式1的四元式个数

    int length = codes.size();

    int i = 0;
    for (; i < length; i++)
    {
        string coding = codes[i];

        // 退出语句
        string::size_type idx_E = coding.find("E:");
        if (idx_E != string::npos)
        {
            Quaternary tac;
            tac.op = "halt";
            tac.arg1 = "#";
            tac.arg2 = "#";
            tac.res = "#";
            tac.label = "E:";
            res.push_back(tac);
            continue;
        }
        string label_temp = loadLabel(codes[i], coding);
        // 跳转语句
        string::size_type idx_goto = coding.find("goto");
        if (idx_goto != string::npos)
        {
            // 分支跳转语句
            string::size_type idx_if = coding.find("if");
            if (idx_if != string::npos)
            {
                int pos_start = (int)idx_if + 3; // if语句判断开始字段
                int pos_end = idx_goto;  // 不包括该位字符
                int pos_num = pos_end - pos_start;
                string s_temp = coding.substr(pos_start, pos_num);

                // 判断跳转语句形式
                // (jrop,x,y,p)
                string::size_type idx_dadeng = coding.find(">=");
                if (idx_dadeng != string::npos)
                {
                    Quaternary tac;

                    tac.op = "j>=";

                    pos_num = idx_dadeng - (idx_if + 3);
                    s_temp = coding.substr((idx_if + 3), pos_num); removeADDR(s_temp);
                    tac.arg1 = s_temp;

                    pos_num = (idx_goto - 1) - (idx_dadeng + 2);
                    s_temp = coding.substr((idx_dadeng + 2), pos_num); removeADDR(s_temp);
                    tac.arg2 = s_temp;

                    s_temp = coding.substr((int)idx_goto + 5);
                    tac.res = s_temp;

                    string s_temp_look = s_temp + ":";
                    string::size_type idx_lookfor;
                    for (int lookfor = 0; lookfor < codes.size(); lookfor++)
                    {
                        idx_lookfor = codes[lookfor].find(s_temp_look);
                        if (idx_lookfor != string::npos)
                        {
                            tac.res = "-1";
                            BigLabel label = { s_temp_look, res.size() };
                            Label.push_back(label);

                            //cout << endl << "lookfor: " << lookfor << endl << endl;
                            break;
                        }
                    }


                    tac.label = label_temp;

                    res.push_back(tac);
                    continue;
                }

                string::size_type idx_xiaodeng = coding.find("<=");
                if (idx_xiaodeng != string::npos)
                {
                    Quaternary tac;

                    tac.op = "j<=";

                    pos_num = idx_xiaodeng - (idx_if + 3);
                    s_temp = coding.substr((idx_if + 3), pos_num); removeADDR(s_temp);
                    tac.arg1 = s_temp;

                    pos_num = (idx_goto - 1) - (idx_xiaodeng + 2);
                    s_temp = coding.substr((idx_xiaodeng + 2), pos_num); removeADDR(s_temp);
                    tac.arg2 = s_temp;

                    s_temp = coding.substr((int)idx_goto + 5);
                    tac.res = s_temp;

                    string s_temp_look = s_temp + ":";
                    string::size_type idx_lookfor;
                    for (int lookfor = 0; lookfor < codes.size(); lookfor++)
                    {
                        idx_lookfor = codes[lookfor].find(s_temp_look);
                        if (idx_lookfor != string::npos)
                        {
                            tac.res = "-1";
                            BigLabel label = { s_temp_look, res.size() };
                            Label.push_back(label);
                            //cout << endl << "lookfor: " << lookfor << endl << endl;
                            break;
                        }
                    }

                    tac.label = label_temp;

                    res.push_back(tac);
                    continue;
                }

                string::size_type idx_dengdeng = coding.find("==");
                if (idx_dengdeng != string::npos)
                {
                    Quaternary tac;


                    tac.op = "j==";

                    pos_num = idx_dengdeng - (idx_if + 3);
                    s_temp = coding.substr((idx_if + 3), pos_num); removeADDR(s_temp);
                    tac.arg1 = s_temp;

                    pos_num = (idx_goto - 1) - (idx_dengdeng + 2);
                    s_temp = coding.substr((idx_dengdeng + 2), pos_num); removeADDR(s_temp);
                    tac.arg2 = s_temp;

                    s_temp = coding.substr((int)idx_goto + 5);
                    tac.res = s_temp;

                    string s_temp_look = s_temp + ":";
                    string::size_type idx_lookfor;
                    for (int lookfor = 0; lookfor < codes.size(); lookfor++)
                    {
                        idx_lookfor = codes[lookfor].find(s_temp_look);
                        if (idx_lookfor != string::npos)
                        {
                            tac.res = "-1";
                            BigLabel label = { s_temp_look, res.size() };
                            Label.push_back(label);
                            //cout << endl << "lookfor: " << lookfor << endl << endl;
                            break;
                        }
                    }

                    tac.label = label_temp;

                    res.push_back(tac);
                    continue;
                }

                string::size_type idx_da = coding.find(">");
                if (idx_da != string::npos)
                {

                    Quaternary tac;

                    tac.op = "j>";

                    pos_num = idx_da - (idx_if + 3);
                    s_temp = coding.substr((idx_if + 3), pos_num); removeADDR(s_temp);
                    tac.arg1 = s_temp;

                    pos_num = (idx_goto - 1) - (idx_da + 1);
                    s_temp = coding.substr((idx_da + 1), pos_num); removeADDR(s_temp);
                    tac.arg2 = s_temp;

                    s_temp = coding.substr((int)idx_goto + 5);
                    tac.res = s_temp;

                    ///*
                    string s_temp_look = s_temp + ":";
                    string::size_type idx_lookfor;
                    for (int lookfor = 0; lookfor < codes.size(); lookfor++)
                    {
                        idx_lookfor = codes[lookfor].find(s_temp_look);
                        if (idx_lookfor != string::npos)
                        {
                            tac.res = "-1";
                            BigLabel label = { s_temp_look, res.size() };
                            Label.push_back(label);
                            //cout << endl << "lookfor: " << lookfor << endl << endl;
                            break;
                        }
                    }
                    //*/

                    tac.label = label_temp;
                    //cout << "test_tac_label: " << tac.label << endl;

                    res.push_back(tac);
                    continue;
                }

                string::size_type idx_deng = coding.find("=");
                if (idx_deng != string::npos)
                {
                    Quaternary tac;

                    tac.op = "j=";

                    pos_num = idx_deng - (idx_if + 3);
                    s_temp = coding.substr((idx_if + 3), pos_num); removeADDR(s_temp);
                    tac.arg1 = s_temp;

                    pos_num = (idx_goto - 1) - (idx_deng + 1);
                    s_temp = coding.substr((idx_deng + 1), pos_num); removeADDR(s_temp);
                    tac.arg2 = s_temp;

                    s_temp = coding.substr((int)idx_goto + 5);
                    tac.res = s_temp;

                    string s_temp_look = s_temp + ":";
                    string::size_type idx_lookfor;
                    for (int lookfor = 0; lookfor < codes.size(); lookfor++)
                    {
                        idx_lookfor = codes[lookfor].find(s_temp_look);
                        if (idx_lookfor != string::npos)
                        {
                            tac.res = "-1";
                            BigLabel label = { s_temp_look, res.size() };
                            Label.push_back(label);
                            //cout << endl << "lookfor: " << lookfor << endl << endl;
                            break;
                        }
                    }

                    tac.label = label_temp;

                    res.push_back(tac);
                    continue;
                }

                string::size_type idx_xiao = coding.find("<");
                if (idx_xiao != string::npos)
                {
                    Quaternary tac;



                    tac.op = "j<";

                    pos_num = idx_xiao - (idx_if + 3);
                    s_temp = coding.substr((idx_if + 3), pos_num); removeADDR(s_temp);
                    tac.arg1 = s_temp;

                    pos_num = (idx_goto - 1) - (idx_xiao + 1);
                    s_temp = coding.substr((idx_xiao + 1), pos_num); removeADDR(s_temp);
                    tac.arg2 = s_temp;

                    s_temp = coding.substr((int)idx_goto + 5);
                    tac.res = s_temp;

                    string s_temp_look = s_temp + ":";
                    string::size_type idx_lookfor;
                    for (int lookfor = 0; lookfor < codes.size(); lookfor++)
                    {
                        idx_lookfor = codes[lookfor].find(s_temp_look);
                        if (idx_lookfor != string::npos)
                        {
                            tac.res = "-1";
                            BigLabel label = { s_temp_look, res.size() };
                            Label.push_back(label);
                            //cout << endl << "lookfor: " << lookfor << endl << endl;
                            break;
                        }
                    }

                    tac.label = label_temp;

                    res.push_back(tac);
                    continue;
                }

                // (jnz,a,-,p)
                Quaternary tac;



                tac.op = "jnz";

                pos_num = (idx_goto)-(idx_if + 3);
                s_temp = coding.substr((idx_if + 3), pos_num); removeADDR(s_temp);
                tac.arg1 = s_temp;

                tac.arg2 = "#";

                s_temp = coding.substr((int)idx_goto + 5);
                tac.res = s_temp;

                string s_temp_look = s_temp + ":";
                string::size_type idx_lookfor;
                for (int lookfor = 0; lookfor < codes.size(); lookfor++)
                {
                    idx_lookfor = codes[lookfor].find(s_temp_look);
                    if (idx_lookfor != string::npos)
                    {
                        tac.res = "-1";
                        BigLabel label = { s_temp_look, res.size() };
                        Label.push_back(label);
                        //cout << endl << "lookfor: " << lookfor << endl << endl;
                        break;
                    }
                }

                tac.label = label_temp;

                res.push_back(tac);
                continue;

            }
            // 普通跳转语句
            else
            {
                int pos_start = (int)idx_goto + 5; // 跳转地点开始字段
                string s_temp = coding.substr(pos_start);
                Quaternary tac;

                tac.op = "j";
                tac.arg1 = "#";
                tac.arg2 = "#";
                tac.res = s_temp;

                string s_temp_look = s_temp + ":";
                string::size_type idx_lookfor;
                for (int lookfor = 0; lookfor < codes.size(); lookfor++)
                {
                    idx_lookfor = codes[lookfor].find(s_temp_look);
                    if (idx_lookfor != string::npos)
                    {
                        tac.res = "-1";
                        BigLabel label = { s_temp_look, res.size() };
                        Label.push_back(label);
                        //cout << endl << "lookfor: " << lookfor << endl << endl;
                        break;
                    }
                }

                tac.label = label_temp;

                res.push_back(tac);
                continue;
            }

        }
        // 运算语句
        // 数组特殊运算语句
        string::size_type idx_value = coding.find(":="); // 老师所给文件有错误，赋值均应该有:=，且一定会有

        string::size_type idx_arrleft = coding.find("[");
        string::size_type idx_arrright = coding.find("]");
        if (idx_arrleft != string::npos)
        {
            string arr_judge = "";
            int pos_num = 0;
            string s_temp = "";
            string::size_type idx_judge = coding.find("+");
            if (idx_judge != string::npos) arr_judge = "+";
            idx_judge = coding.find("-");
            if (idx_judge != string::npos) arr_judge = "-";
            idx_judge = coding.find("*");
            if (idx_judge != string::npos) arr_judge = "*";
            idx_judge = coding.find("/");
            if (idx_judge != string::npos) arr_judge = "/";
            idx_judge = coding.find("mod");
            if (idx_judge != string::npos) arr_judge = "mod";

            // 以下情况只适用于本题案例

            if (arr_judge != "") // 有运算，只判断运算在右部出现，多了一个四元式
            {
                string c_arr = coding.substr(idx_value + 2);
                Quaternary tac;

                tac.op = arr_judge;
                tac.res = "X1";
                tac.label = "#";

                string::size_type idx_judge_new;

                for (;;)
                {
                    idx_judge_new = c_arr.find("+");
                    if (idx_judge_new != string::npos) break;
                    idx_judge_new = c_arr.find("-");
                    if (idx_judge_new != string::npos) break;
                    idx_judge_new = c_arr.find("*");
                    if (idx_judge_new != string::npos) break;
                    idx_judge_new = c_arr.find("/");
                    if (idx_judge_new != string::npos) break;
                    idx_judge_new = c_arr.find("mod");
                    if (idx_judge_new != string::npos) break;
                }

                pos_num = idx_judge_new;
                s_temp = c_arr.substr(0, pos_num); removeADDR(s_temp);
                tac.arg1 = s_temp;

                s_temp = c_arr.substr(idx_judge_new + 1); removeADDR(s_temp);
                tac.arg2 = s_temp;

                res.push_back(tac);

                coding.replace((idx_value + 2), (coding.length() - idx_value - 1), "X1");
            }
            // 标准数组
            if (idx_arrleft < idx_value)
            {
                Quaternary tac;
                tac.op = "[]=";

                s_temp = coding.substr(idx_value + 2); removeADDR(s_temp);
                tac.arg1 = s_temp;

                pos_num = idx_arrleft;
                s_temp = coding.substr(0, pos_num); removeADDR(s_temp);
                tac.arg2 = s_temp;

                pos_num = idx_arrright - (idx_arrleft + 1);
                s_temp = coding.substr(idx_arrleft + 1, pos_num); removeADDR(s_temp);
                tac.res = s_temp;

                tac.label = label_temp;

                res.push_back(tac);
                continue;
            }
            else if (idx_arrleft > idx_value)
            {
                Quaternary tac;
                tac.op = "=[]";

                pos_num = idx_arrleft - (idx_value + 2);
                s_temp = coding.substr(idx_value + 2, pos_num); removeADDR(s_temp);
                tac.arg1 = s_temp;

                pos_num = idx_arrright - (idx_arrleft + 1);
                s_temp = coding.substr(idx_arrleft + 1, pos_num); removeADDR(s_temp);
                tac.arg2 = s_temp;

                pos_num = idx_value;
                s_temp = coding.substr(0, pos_num); removeADDR(s_temp);
                tac.res = s_temp;

                tac.label = label_temp;

                res.push_back(tac);
                continue;

            }

        }
        // 普通运算语句
        else
        {
            int pos_num = idx_value;
            string s_temp = coding.substr(0, pos_num);
            Quaternary tac;

            //cout << "test_tac_label: " << tac.label << endl;

            tac.res = s_temp;

            // 有运算
            string::size_type idx_plus = coding.find("+");
            if (idx_plus != string::npos)
            {
                tac.op = "+";

                pos_num = idx_plus - (idx_value + 2);
                s_temp = coding.substr(idx_value + 2, pos_num); removeADDR(s_temp);
                tac.arg1 = s_temp;

                s_temp = coding.substr(idx_plus + 1); removeADDR(s_temp);
                tac.arg2 = s_temp;

                tac.label = label_temp;

                res.push_back(tac);
                continue;

            }

            string::size_type idx_minus = coding.find("-");
            if (idx_minus != string::npos)
            {
                tac.op = "-";

                pos_num = idx_minus - (idx_value + 2);
                s_temp = coding.substr(idx_value + 2, pos_num); removeADDR(s_temp);
                tac.arg1 = s_temp;

                s_temp = coding.substr(idx_minus + 1); removeADDR(s_temp);
                tac.arg2 = s_temp;

                tac.label = label_temp;

                res.push_back(tac);
                continue;

            }

            string::size_type idx_multi = coding.find("*");
            if (idx_multi != string::npos)
            {
                tac.op = "*";

                pos_num = idx_multi - (idx_value + 2);
                s_temp = coding.substr(idx_value + 2, pos_num); removeADDR(s_temp);
                tac.arg1 = s_temp;

                s_temp = coding.substr(idx_multi + 1); removeADDR(s_temp);
                tac.arg2 = s_temp;

                tac.label = label_temp;

                res.push_back(tac);
                continue;
            }

            string::size_type idx_devide = coding.find("/");
            if (idx_devide != string::npos)
            {
                tac.op = "/";

                pos_num = idx_devide - (idx_value + 2);
                s_temp = coding.substr(idx_value + 2, pos_num); removeADDR(s_temp);
                tac.arg1 = s_temp;

                s_temp = coding.substr(idx_devide + 1); removeADDR(s_temp);
                tac.arg2 = s_temp;

                tac.label = label_temp;

                res.push_back(tac);
                continue;
            }

            string::size_type idx_mod = coding.find("mod");
            if (idx_devide != string::npos)
            {
                tac.op = "mod";

                pos_num = idx_mod - (idx_value + 2);
                s_temp = coding.substr(idx_value + 2, pos_num); removeADDR(s_temp);
                tac.arg1 = s_temp;

                s_temp = coding.substr(idx_mod + 3); removeADDR(s_temp);
                tac.arg2 = s_temp;

                tac.label = label_temp;

                res.push_back(tac);
                continue;
            }

            // 没有运算
            tac.op = ":=";

            pos_num = coding.length() - (idx_value + 2);
            s_temp = coding.substr(idx_value + 2, pos_num); removeADDR(s_temp);
            tac.arg1 = s_temp;

            s_temp = coding.substr(idx_value + 2); removeADDR(s_temp);
            tac.arg2 = s_temp;
            tac.arg2 = "#";

            tac.label = label_temp;

            res.push_back(tac);
            continue;
        }
    }

    for (int i = 0; i < res.size(); i++)
    {
        res[i].id = i;
        // 开始模拟回填
        ///*
        if (res[i].label != "")
        {
            simuBackpatch(res[i].label, i, Label, res);
        }
        //*/
    }
    return res;
}

// 测试：打印tac[]
void showTac(Quaternary tac[], int length)
{
    for (int i = 0; i < length; i++)
    {
        cout << tac[i].op << " " << tac[i].arg1 << " " << tac[i].arg2 << " " << tac[i].res << endl;
    }
}


vector<string> readFile(const string fileName)
{
    ifstream  infile;
    vector<string> codes;
    string code;

    // 按行读文件，判断本句三地址代码是否是程序入口语句
    infile.open(fileName, ios::out | ios::in);
    if (!infile.is_open())
    {
        cout << "Error: opening file fail" << endl;
        exit(1);
    }

    while (infile.eof() == false) {
        getline(infile, code);
        codes.push_back(code);
    }
    infile.close();
    return codes;
}

// 将三地址代码转换为四元式
vector<Quaternary> readTacToQuaternary(string filename)
{

    /*vector<string> a = readFile(filename);
    for (int i = 0; i < a.size(); i++)
        cout << a[i] << endl;*/
    return three2four(readFile(filename));

}

// 直接读取四元式文件
vector<Quaternary> readQuaternary(string filename)
{
    ifstream in(filename, ios::in);
    if (!in.is_open())
    {
        cout << "Error: opening file fail" << endl;
        exit(1);
    }

    vector<Quaternary> result;
    int n = 0;
    while (!in.eof())
    {
        Quaternary qua;
        qua.id = n++;
        in >> qua.op >> qua.arg1 >> qua.arg2 >> qua.res;
        result.push_back(qua);
    }

    return result;
    in.close();
}