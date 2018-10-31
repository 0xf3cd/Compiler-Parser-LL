#ifndef GRAMMAR
#define GRAMMAR
#include "Grammar.h"
#endif

#ifndef PARSING_TABLE
#define PARSING_TABLE
#include "ParsingTable.h"
#endif

#ifndef TOKENIZER
#define TOKENIZER
#include "Tokenizer.h"
#endif

#include <list>
using namespace std;

/* snapshot 即快照，用于表示某一次语法分析即结果
 * stack 需要用迭代器进行遍历
 * error 出错码解析如下
 *  3   语法分析先前已经结束，而且没有出错
 *  2   本次步骤正确，而且这一步栈顶为变元，使用了产生式
 *  1   本次步骤正确，而且这一步栈顶为终结符，往后读取一个新的 token
 *  0   本次步骤正确，且这一步为最后一步
 *  -1  本次词法出错，详细错误可以查看 token 情况
 *  -2  本次语法分析出错，栈顶为终结符，但是与读到的 token 不同
 *  -3  本次语法分析出错，读到了井号，但是栈顶并不为井号
 *  -4  本次语法分析出错，栈顶为变元，但是预测分析表内没有对应的产生式
 *  -5  先前已经出错
 */
struct snapshot {
    list<Symbol> stack; //栈的情况（使用 List 容器储存）
    word token; //从词法分析器读入的词
    Symbol symbol; //token 对应的 symbol
    Production production; //本次步骤使用的产生式
    int error; //指示是否出错及出错原因
};

class Parser {
private:
    bool initialized; //指示是否进行了初始化
    bool has_finished; //指示是否读取完毕
    bool has_error; //指示分析过程中是否出错
    Grammar G;
    ParsingTable PT;
    Tokenizer T;
    list<Symbol> L; //用 List 容器模拟栈
    word last_token; //上一次通过词法分析器读入的词

    /* getStackStatus 函数: 返回现有栈的情况
     */
    list<Symbol> getStackStatus();

    /* translate 函数: 将 token 转换成 Symbol
     */
    Symbol translate(word token);

public:
    /* 构造函数: 初始化成员变量
     */
    Parser();

    /* initialize 函数: 打开文法描述文件并进行分析
     * @param source_dir: 源文件的地址
     * @param grammar_dir: 语法描述文件的地址
     * @return: 文法分析结果
     */
    bool initialize(string source_dir, string grammar_dir = "./TestFile/Grammar.txt");

    /* hasInitialized 函数
     * @return: 是否已经读取了文法并成功分析
     */
    bool hasInitialized();

    /* hasFinished 函数
     * @return: 是否已经读取完毕
     */
    bool hasFinished();

    /* hasError 函数
     * @return: 是否出现语法错误
     */
    bool hasError();

    /* getNext 函数: 让语法分析器执行一步分析
     * @return: 一个 snapshot 对象，其中记录了本次分析的结果
     */
    snapshot getNext();
};