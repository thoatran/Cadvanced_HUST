#include "symbolTable.c"

void printPhoneBook(SymbolTable *book){
  int i;
  Entry *res = book->entries;
  while(i < book->total){
    printf("%s  %ld\n", (char *)res->key,(long)res->value);
    res ++;
    i++ ;
  }
}
void main(){
  SymbolTable phoneBook = createSymbolTable(makePhone, comparePhone);
  char name[] = "Tran Thi Thoa";
  long number = 123454464;
  addEntry(name, &number, &phoneBook);
  printPhoneBook(&phoneBook);
  dropSymbolTable(&phoneBook);
  return;
}
