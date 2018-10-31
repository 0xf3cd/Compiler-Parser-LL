 //预测分析表
#include <map>
#include <set>

#ifndef SYMBOL
#define SYMBOL
#include "Symbol.h"
#endif

#ifndef PRODUCTION
#define PRODUCTION
#include "Production.h"
#endif

#ifndef GRAMMAR
#define GRAMMAR
#include "Grammar.h"
#endif

using namespace std;

typedef map<Symbol, Production> ROW; //预测分析表中，某个变元对应的某一行，为 （终结符， 产生式） 键值对
typedef map<Symbol, ROW> TABLE; //预测分析表，为（变元, ROW）键值对

class ParsingTable {
private:
    TABLE table;

    set<Symbol> column_head; //列名

    Production ERROR;

    set<Symbol> arguments;

    /* initialize 函数: 生成空的预测分析表
     * @param Grammar: 为文法对象的引用
     */
    void initialize(Grammar &G);
public:
    /* 构造函数: 初始化表示 ERROR 的产生式迭代器
     */
    ParsingTable();

    /* generate 函数: 根据文法对象，生成对应的预测分析表
     * @param Grammar: 为文法对象的引用
     */
    void generate(Grammar &G);

    /* showParsingTable 函数: 展示分析预测表
     */
    void showParsingTable();

    /* getProduction 函数: 根据输入的 变元 和 终结符 返回查询式 出错返回 ERROR
     */
    Production getProduction(Symbol arg, Symbol ter);
};
