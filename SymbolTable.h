#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"
class Symbol {
private:
  string name, type;
  int scope;

public:
  Symbol(){};
  ~Symbol(){};
  Symbol(const string &name, const string &type, const int scope) {
    this->name = name;
    this->type = type;
    this->scope = scope;
  };
  friend class SymbolTable;
};
class Node {
private:
  Symbol symbol;
  Node *next, *prev;

public:
  Node(Node *head){

  };
  Node(Symbol symbol) {
    this->symbol = symbol;
    next = NULL;
    prev = NULL;
  };
  friend class SymbolTable;
};
class SymbolTable {
private:
  Node *head;
  Node *tail;
  int count;

public:
  SymbolTable() {
    this->head = NULL;
    this->tail = NULL;
    this->count = 0;
  }
  ~SymbolTable() {}
  void run(string filename);
  void push(Symbol symbol);
  void pop_sc(int level);
  void assign(string name,string type, int level, string dataLine);
  bool search(string name, int level);
  void print();
  void pop();
  bool check_exit(string name, int level);
  bool check_assign(string type);
  void lookup(string name, int level, string dataLine);
  void print(int level);
  void rprint(int level);
  bool count_exit(string name, int scope);
};
#endif