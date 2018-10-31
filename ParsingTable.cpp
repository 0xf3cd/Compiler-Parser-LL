#ifndef PARSING_TABLE
#define PARSING_TABLE
#include "ParsingTable.h"
#endif

#include <iostream>

using namespace std;

/* 构造函数: 初始化表示 ERROR 的产生式迭代器
 */
ParsingTable::ParsingTable() {
    ERROR.left = Symbol("ERROR");
}

/* initialize 函数: 生成空的预测分析表
 * @param Grammar: 为文法对象的引用
 */
void ParsingTable::initialize(Grammar &G) {
    arguments = G.getArguments();
    const set<Symbol> &terminals = G.getTerminals();
    set<Symbol>::iterator it_a, it_t; //变元和终结符的迭代器
    ROW new_row;
    Symbol end_symbol = G.getEndSymbol();

    for(it_a = arguments.begin(); it_a != arguments.end(); it_a++) { //遍历所有变元
        new_row.clear();
        for(it_t = terminals.begin(); it_t != terminals.end(); it_t++) { //遍历所有终结符
            new_row[*it_t] = ERROR;
        } 
        new_row[end_symbol] = ERROR;
        table[*it_a] = new_row;
    }

    for(it_t = terminals.begin(); it_t != terminals.end(); it_t++) { //遍历所有终结符
        column_head.insert(*it_t);
    }
    column_head.insert(end_symbol);
}

/* generateParseTable 函数: 根据文法对象，生成对应的预测分析表
 * @param Grammar: 为文法对象的引用
 */
void ParsingTable::generate(Grammar &G) {
    const set<Production> &productions = G.getProductions();
    map<Production, FIRST_SET> &first = G.getFirst();
    map<Symbol, FOLLOW_SET> &follow = G.getFollow();
    set<Production>::iterator it_p; //遍历产生式的迭代器
    Symbol left; //用来指代产生式左部
    FIRST_SET::iterator it_FIRST; //遍历 FIRST 集的迭代器
    FOLLOW_SET::iterator it_FOLLOW; //遍历 FOLLOW 集合的迭代器

    const Symbol EPSILON = G.getEpsilon(); //表示空字符
    Production prod_to_epsilon;
    prod_to_epsilon.right.push_back(EPSILON);

    //初始化，将预测分析表内所有空格内都填入错误
    initialize(G);
    
    for(it_p = productions.begin(); it_p != productions.end(); it_p++) { //遍历产生式
        //产生式形如 A -> α
        left = it_p -> left; //left 为产生式左部变元，即为 A
        FIRST_SET &FIRST = first[*it_p]; //遍历到的产生式对应的 FIRST 集合

        for(it_FIRST = FIRST.begin(); it_FIRST != FIRST.end(); it_FIRST++) {
            if(*it_FIRST == EPSILON) { //如果这个产生式对应的 FIRST 集合存在 ε
                //将所有 FOLLOW 填入预测分析表对应的格子中
                prod_to_epsilon.left = left; //调整产生式为 A -> ε
                FOLLOW_SET &FOLLOW = follow[left];

                for(it_FOLLOW = FOLLOW.begin(); it_FOLLOW != FOLLOW.end(); it_FOLLOW++) {
                    table[left][*it_FOLLOW] = prod_to_epsilon; //将表格内相应各自换为 A -> ε
                }
            } else {
                table[left][*it_FIRST] = *it_p;
            }
        }
    }
}

/* showParsingTable 函数: 展示分析预测表
 */
void ParsingTable::showParsingTable() {
    TABLE::iterator it_table; //预测表中每一行的迭代器
    set<Symbol>::iterator it_col; //列迭代器
    set<Symbol>::iterator it_arg;

    //输出列名
    cout << "\t\t";
    for(it_col = column_head.begin(); it_col != column_head.end(); it_col++) {
        cout << it_col -> name << "\t\t";
    }
    cout << endl;

    for(it_arg = arguments.begin(); it_arg != arguments.end(); it_arg++) {
        //*it_arg 为变元
        cout << it_arg -> name << "\t\t";
        for(it_col = column_head.begin(); it_col != column_head.end(); it_col++) {
            cout << table[*it_arg][*it_col] << "\t";
        }

        cout << endl;
    }
}

/* getProduction 函数: 根据输入的 变元 和 终结符 返回查询式 出错返回 ERROR
 */
Production ParsingTable::getProduction(Symbol arg, Symbol ter) {
    if(table.find(arg) == table.end()) {
        return ERROR;
    }
    ROW r = table[arg];

    if(r.find(ter) == r.end()) {
        return ERROR;
    }
    return r[ter];
}