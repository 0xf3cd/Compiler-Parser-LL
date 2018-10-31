#ifndef PARSER
#define PAESER
#include "Parser.h"
#endif

using namespace std;

/* 构造函数，初始化成员变量
 */
Parser::Parser() {
    initialized = false;
    has_finished = false;
    has_error = false;
}

/* getStackStatus函数: 返回现有栈的情况
 */
list<Symbol> Parser::getStackStatus() {
    list<Symbol> status;
    list<Symbol>::iterator it;//1, it2;

    for(it = L.begin(); it != L.end(); it++) {
        status.push_back(*it);
    }

    return status;
}

/* translate 函数: 将 token 转换成 Symbol
 */
Symbol Parser::translate(word token) {
    Symbol s;
    if(token.type == 7) { //ID
        s.name = "ID";
    } else if(token.type == 8) { //num
        s.name = "num";
    } else {
        s.name = token.value;
    }
    s.no = token.type;
    return s;
}

/* initialize initialize 函数: 打开文法描述文件并进行分析
 * @param source_dir: 源文件的地址
 * @param grammar_dir: 语法描述文件的地址
 * @return: 文法分析结果
 */
bool Parser::initialize(string source_dir, string grammar_dir) {
    //读取文件
    if(G.analyzeGrammar(grammar_dir)) {
        cout << "READ IN OK" << endl;
    } else {
        cerr << "FAILED TO READ IN" << endl;
        initialized = false;
        return false;
    }

    if(G.isLL1()) {
       cout << "IS LL(1) GRAMMAR" << endl;
    } else {
        cerr << "NOT LL(1) GRAMMAR" << endl;
        initialized = false;
        return false;
    }

    PT.generate(G);

    if(T.openSourceFile(source_dir)) {
        cout << "OPEN SOURCE FILE OK" << endl;
    } else {
        cerr << "FAILED TO OPEN SOURCE FILE" << endl;
        initialized = false;
        return false;
    }

    // S.push(G.getEndSymbol());
    // S.push(G.getStartSymbol());
    L.push_front(G.getStartSymbol());
    L.push_back(G.getEndSymbol());

    last_token =  T.readNextWord(); //读入第一个单词

    initialized = true;
    return true;
}

/* hasInitialized
 * @return: 是否已经读取了文法并成功分析
 */
bool Parser::hasInitialized() {
    return initialized;
}

/* hasFinished 函数
 * @return: 是否已经读取完毕
 */
bool Parser::hasFinished() {
    return has_finished;
}

/* hasError 函数
 * @return: 是否出现语法错误
 */
bool Parser::hasError() {
    return has_error;
}

/* getNext 函数: 让语法分析器执行一步分析
 * @return: 一个 snapshot 对象，其中记录了本次分析的结果
 */
snapshot Parser::getNext() {
    snapshot ss; //ss for SnapShot
    Symbol now_symbol; //当前词法分析器读入的词语对应的 Symbol
    Symbol stack_top; //栈顶 Symbol
    Production now_production; //对应的产生式
    vector<Symbol>::const_reverse_iterator it_v; //产生式右部的反向迭代器

    //跳过注释符号
    while(last_token.type >= 26) {
        last_token = T.readNextWord();
    }

    //确定此时的栈的情况
    ss.stack = getStackStatus();
    ss.token = last_token;

    if(has_finished) {
        //如果已经分析完毕

        ss.error = 3; //3表示先前已经分析完毕
        return ss;
    }
    if(has_error) {
        //如果先前已经出错

        ss.error = -5; //表示先前已经出错
        return ss;
    }

    //如若词法分析出错
    if(last_token.type < 0) {
        has_error = true;

        ss.error = -1; //-1 表示词法出错
        return ss;
    }
    //否则词法分析正确
    //得到栈顶元素和当前读入的符号
    stack_top = L.front();
    L.pop_front(); //弹栈
    now_symbol = translate(last_token);
    ss.symbol = now_symbol;

    if(G.isTerminal(stack_top)) { //如果此时栈顶是终结符
        if(stack_top == now_symbol) { 
            //栈顶为终结符且与读到的单词一致，则正确
            ss.error = 1; //1表示往后读一个单词
            last_token = T.readNextWord();
            return ss;
        } else { 
            //栈顶为终结符且与读到的单词不一致，出错
            has_error = true;

            ss.error = -2; //-2 表示分析过程出错
            return ss;
        }
    } else if (stack_top.no == 0) {
        if(stack_top == now_symbol) {
            //如果栈顶和读到的字符都为终结符，则说明读取完毕
            has_finished = true;

            ss.error = 0; //0表示读取完毕
            return ss;
        } else {
            //读到了井号，但是栈顶并不为井号
            has_error = true;

            ss.error = -3; //-3 表示分析过程出错
            return ss;
        }
    } else { //如果此时栈顶为变元
        now_production = PT.getProduction(stack_top, now_symbol);
        if(now_production.left == "" || now_production.left == "ERROR") {
            //没有对应产生式
            has_error = true;

            ss.error = -4; //-4 表示分析过程出错
            return ss;
        } else {
            //否则产生式存在且正确
            ss.production = now_production;
            vector<Symbol> &right = now_production.right;
            for(it_v = right.rbegin(); it_v != right.rend(); it_v++) {
                if(*it_v != G.getEpsilon()) {
                    L.push_front(*it_v);
                }
            }
            ss.error = 2; //2 表示使用了产生式
            return ss;
        }
    }

    return ss;
}