#include "SymbolTable.h"

bool SymbolTable::search(string name, int level) {
  if (!head)
    return false;
  Node *temp = this->head;
  while (temp != NULL) {
    if (temp->symbol.name == name && temp->symbol.scope == level)
      return true;
    temp = temp->next;
  }
  return false;
}

// bool SymbolTable::check_assign(string type) {}

bool isNumber(const string &s) {
  for (char const &ch : s) {
    if (std::isdigit(ch) == 0)
      return false;
  }
  return true;
}
bool isLabel(const string &s) {
  if (s[0] == '\'' && s[s.size() - 1] == '\'')
    return false;
  return true;
}

bool SymbolTable::check_exit(string name, int level) {
  Node *temp = head;
  while (temp != NULL) {
    if (temp->symbol.name == name && temp->symbol.scope <= level)
      return true;
    temp = temp->next;
  }
  return false;
}

bool SymbolTable::count_exit(string name, int scope) {

  Node *temp = head;
  while (temp != NULL) {
    if (temp->symbol.name == name && temp->symbol.scope > scope)
      return true;
    temp = temp->next;
  }
  return false;
}

void SymbolTable::push(Symbol symbol) {
  Node *newNode = new Node(symbol);
  if (head == NULL) {
    this->head = newNode;
    this->tail = newNode;
  } else {
    newNode->next = head;
    this->head->prev = newNode;
    this->head = newNode;
  }
  this->count++;
  cout << "success" << endl;
}

void SymbolTable::pop() {
  if (head == NULL)
    return;
  //   if (this->count == 1) {
  //     delete head;
  //     delete tail;
  //     head->next = NULL;
  //     head->prev = NULL;
  //     tail->next = NULL;
  //     tail->prev = NULL;
  //     this->count--;
  //     return;
  //   }
  Node *temp = head;
  head = head->next;
  this->count--;
//   temp->next = NULL;
//   temp->prev = NULL;
  delete temp;

  if (head != NULL)
    head->prev = NULL;
}

void SymbolTable::pop_sc(int level) {
  if (head == NULL)
    return;
  Node *temp = this->head;
  while (temp != NULL) {
    if (temp->symbol.scope != level)
      return;
    temp = temp->next;
    this->pop();
  }
}

void SymbolTable::assign(string name, string type, int level, string dataLine) {

  Node *temp = this->head;
  string mer1, mer2;
  bool isExit = false;
  bool compare = false, isExit1 = false, isExit2 = false;
  if (!check_exit(name, level))
    throw Undeclared(dataLine);
  while (temp != NULL) {
    if (isLabel(type) && !isNumber(type)) {
      if (temp->symbol.name == type && mer1 == "") {
        isExit1 = true;
        mer1 = temp->symbol.type;
      }
      if (temp->symbol.name == name && mer2 == "") {
        isExit2 = true;
        mer2 = temp->symbol.type;
      }
      if (mer1 == mer2 & mer1 != "")
        compare = true;
      if (isExit1 && isExit2)
        isExit = true;
    } else {
      if (temp->symbol.name == name) {
        isExit = true;
        if ((temp->symbol.type == "number" && isNumber(type)) ||
            (temp->symbol.type == "string" && !isNumber(type) &&
             !isLabel(type))) {
          compare = true;
          break;
        }
      }
    }
    temp = temp->next;
  }
  if (name == type && isExit) {
    cout << "success" << endl;
    return;
  }
  if (!isExit)
    throw Undeclared(dataLine);
  if (!compare)
    throw TypeMismatch(dataLine);
  cout << "success" << endl;
}

void SymbolTable::lookup(string name, int level, string dataLine) {
  if (!check_exit(name, level))
    throw Undeclared(dataLine);
  Node *temp = head;
  while (temp != NULL) {
    if (temp->symbol.name == name) {
      cout << temp->symbol.scope << endl;
      break;
    }
    temp = temp->next;
  }
}

void SymbolTable::print(int level) {
  if (head == NULL)
    return;
  Node *temp = tail;
  bool first_element = true;
  while (temp != NULL) {
    if (!count_exit(temp->symbol.name, temp->symbol.scope)) {
      if (first_element) {
        cout << temp->symbol.name << "//" << temp->symbol.scope;
        first_element = false;
      } else
        cout << " " << temp->symbol.name << "//" << temp->symbol.scope;
    }
    temp = temp->prev;
  }
  cout << endl;
}
void SymbolTable::rprint(int level) {
  if (head == NULL)
    return;
  Node *temp = head;

  bool first_element = true;
  while (temp != NULL) {
    if (!count_exit(temp->symbol.name, temp->symbol.scope)) {
      if (first_element) {
        cout << temp->symbol.name << "//" << temp->symbol.scope;
        first_element = false;
      } else
        cout << " " << temp->symbol.name << "//" << temp->symbol.scope;
    }
    temp = temp->next;
  }
  cout << endl;
}

void SymbolTable::print() {
  Node *p = head;
  while (p != NULL) {
    cout << p->symbol.name << "//" << p->symbol.scope << endl;
    p = p->next;
  }
}

void SymbolTable::run(string filename) {
  fstream newfile;
  newfile.open(filename, ios::in);
  string dataLine, method, name, type;
  int level = 0;
  while (getline(newfile, dataLine)) {
    int start = 0;
    int end = dataLine.find(" ");
    method = dataLine.substr(start, end - start);
    start = end + 1;
    end = dataLine.find(" ", start);
    name = dataLine.substr(start, end - start);
    start = end + 1;
    end = (int)dataLine.size();
    type = dataLine.substr(start, end - start);
    if (method != "INSERT" && method != "ASSIGN" && method != "LOOKUP" &&
        method != "PRINT" && method != "BEGIN" && method != "END" &&
        method != "RPRINT") {
      throw InvalidInstruction(dataLine);
    } else {
      if (method == "BEGIN") {
        level++;
      } else if (method == "END") {
        pop_sc(level);
        level--;
        if (level < 0) {
          throw(UnknownBlock());
        }
      }
      if (method == "INSERT") {
        if (!search(name, level)) {
          Symbol newSymbol(name, type, level);
          push(newSymbol);
        } else
          throw Redeclared(dataLine);
      }
      if (method == "ASSIGN") {
        assign(name, type, level, dataLine);
      }
      if (method == "LOOKUP") {
        lookup(name, level, dataLine);
      }
      if (method == "PRINT")
        print(level);
      if (method == "RPRINT")
        rprint(level);
    }
  }
  //   print();

  if (level != 0) {
    throw UnclosedBlock(level);
  }
}