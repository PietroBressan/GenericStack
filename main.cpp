/**
@file main.cpp 

@brief test suite per la classe GenerickStack
**/
#include <iostream>
#include "GenericStack.h" // dbuffer<int>
#include <cassert>   // assert

/**
  @brief Funtore di ricerca di uno specifico carattere

*/
struct charEqlTarget {
  bool operator()(char element) const {
    return element == target;
  }

  charEqlTarget(char Target) : target(Target) {};
  charEqlTarget() : target('0') {};

    private:
    char target;

};

/**
  @brief Funtore per determinare se un numero è divisibile per 5

*/
struct intMutiply5 {
  bool operator()(int a) const {
    if(a % 5 == 0) return true;
    return false;
  }
};

/**
 * test_metodi_specifici
 * 
  @brief test dei metodi specifici che riguardano la classe GenerickStack

*/
void test_metodi_fondamentali(){
    std::cout<<"******** Test metodi fondamentali della classe GenericStack ********"<<std::endl;
    //Costruttore base 
    GenericStack<int> gs(5); 
    assert(gs.size() == 5);
    assert(gs.current_stack_size() == 0);

    //operazione di inserimento
    gs.push(1);
    gs.push(2);
    gs.push(3);
    gs.push(17.8); //La classe riesce a gestire tipi di dato diversi (fin dove il compilatore lo ammette)
    gs.push(1);
    assert(gs.current_stack_size() == 5);
    std::cout<<"-------- contenuto dello stack dopo gli inserimenti"<<std::endl;
    std::cout << "         " << gs ;
    
    try{
        //provo ad inserire un nuovo elemento
        gs.push(22);
    }catch(const std::out_of_range& ex){
        std::cout<<"--------  se si eccede la capacità dello stack viene generato un errore std::out_of_range"<<std::endl;
        std::cout << "         " << ex.what() <<std::endl;
    }
    
    //Copy-constructor
    GenericStack<int> gs_copy(gs); 
    assert(gs_copy.size() == 5);
    assert(gs_copy.current_stack_size() == 5);
    std::cout<<"-------- creo uno stack della struttura di partenza tramite copy-constructor "<<std::endl;
    std::cout<<"-------- contenuto dello stack copiato "<<std::endl;
    std::cout << "         " << gs_copy ;


    //Operatore di assegnamento
    GenericStack<int> gs_equal(8);
    gs_equal = gs;
    assert(gs_equal.size() == 5);
    assert(gs_equal.current_stack_size() == 5);
    std::cout<<"-------- operatore di assegnazione (=) tra due GenerickStack "<<std::endl;
    std::cout<<"-------- contenuto dello stack dopo assegnazione ********"<<std::endl;
    std::cout << "         " << gs_copy;

    GenericStack<int>::const_iterator i1 = gs.begin();
    GenericStack<int>::const_iterator i2 = gs_copy.begin();
    GenericStack<int>::const_iterator i3 = gs_equal.begin();
    //assert per dimostrare che i dati non sono condivisi quando si chiama
    //il copy-constructor e l'operatore di assegnamento.
    assert(i1 != i2);
    assert(i1 != i3);
    assert(i3 != i2);


    
    std::cout<<"-------- Svuoto lo stack "<<std::endl;
    //svuoto lo stack
    //gli elementi dello stack sono ritornati per reference costante
    const int& last_element = gs.pop();
    gs.pop();
    gs.pop();
    gs.pop();
    gs.pop();
    try{
        //provo a togliere un ulteriore elemento
        gs.pop();
    }catch(const std::out_of_range& ex){
        std::cout<<"-------- Se si chiama la pop su uno stack vuoto viene generato un errore std::out_of_range"<<std::endl;
        std::cout << "         " << ex.what() <<std::endl;
    }
    std::cout<<"-------- contenuto dello stack "<<std::endl;
    std::cout << "         " << gs ;
    GenericStack<int> *heap_stack = new GenericStack<int>(8);
    std::cout<<"-------- creazione di uno stack sullo heap "<<std::endl;
    std::cout<<"-------- contenuto dello stack (vuoto)"<<std::endl;
    std::cout << "         " << *heap_stack ;
    std::cout<<"-------- chiamo la delete "<<std::endl;
    delete heap_stack;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
}

/**
 * test_metodi_specifici
 * 
  @brief test dei metodi specifici che riguardano la classe GenerickStack

*/
void test_metodi_specifici(){
    std::cout<<"******** Test metodi specifici della classe GenericStack *******"<<std::endl;

    GenericStack<char> gs(10);
    gs.push('T');
    gs.push('S');
    gs.push('E');
    gs.push('T');
    //stampo la size della struttura dati
    std::cout<<"-------- dimensione dello stack: "<< std::endl;
    std::cout << "         " << gs.size() << std::endl;
    std::cout<<"-------- contenuto dello stack dopo gli inserimenti "<<std::endl;
    std::cout << "         " << gs << std::endl;
    //stampo il numero di elementi nello stack
    std::cout<<"-------- elementi nello stack: "<<std::endl;
    std::cout << "         " << gs.current_stack_size() ;

    std::cout << std::endl;
    std::cout<<"-------- creazione di un GenerickStack tramite iteratori "<<std::endl;

    GenericStack<char>::const_iterator itr = gs.begin();
    GenericStack<char>::const_iterator end = gs.end();

    //Constructor con due iteratori come parametri di input;
    GenericStack<char> gs_from_itr(itr, end);
    assert(gs.current_stack_size() == gs_from_itr.current_stack_size());
    std::cout<<"-------- contenuto dello stack appena creato "<<std::endl;
    std::cout << "         " << gs_from_itr ;
    std::cout<<"-------- svuotamento dello stack "<<std::endl;
    gs_from_itr.flush();
    std::cout<<"-------- contenuto dello stack svuotato "<<std::endl;
    std::cout << "         " << gs_from_itr ;
    std::cout<<"-------- dimensione dello stack svuotato "<<std::endl;
    std::cout << "         " << gs_from_itr.size() <<std::endl;
    std::cout<<"-------- elementi nello stack svuotato "<<std::endl;
    std::cout << "         " << gs_from_itr.current_stack_size() <<std::endl;

    GenericStack<char> gs2(6);
    gs2.push('!');
    gs2.push('O');
    gs2.push('L');
    gs2.push('L');
    gs2.push('E');
    gs2.push('H');
    itr = gs2.begin();
    end = gs2.end();
    std::cout<<"-------- refactor di un GenerickStack tramite iteratori "<<std::endl;
    //la classe riesce a gestire anche il caso in cui gli iteratori di inizio e fine siano invertiti
    gs_from_itr.refactor(end, itr);
    std::cout<<"-------- contenuto dello stack dopo refactor "<<std::endl;
    std::cout << "         " << gs_from_itr ;
    std::cout<<"-------- dimensione dello stack "<<std::endl;
    std::cout << "         " << gs_from_itr.size() <<std::endl;
    std::cout<<"-------- elementi nello stack "<<std::endl;
    std::cout << "         " << gs_from_itr.current_stack_size() <<std::endl;

    char last_element = gs_from_itr.pop();
    std::cout<<"-------- rimozione dell'ultimo elemento nello stack "<<std::endl;
    std::cout<<"-------- contenuto dello stack "<<std::endl;
    std::cout << "         " << gs_from_itr;
    std::cout<<"-------- elemento rimosso "<<std::endl;
    std::cout<< "         " << last_element<<std::endl;
    std::cout<<"-------- inserimento nello stack di un nuovo elemento"<<std::endl;
    gs_from_itr.push('X');
    std::cout<<"-------- nuovo contenuto dello stack "<<std::endl;
    std::cout << "         " << gs_from_itr;
    
    intMutiply5 test;
    charEqlTarget test2('W');
    GenericStack<char> gs_int(3);
    //chiamo la funzione templata checkif e controllo che il comportamento sia corretto
    assert(gs.checkif(15, test));
    assert(gs_from_itr.checkif('W', test2));
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
}

/**
 * test_metodi_iteratori
 * 
  @brief test dei metodi che rigurdano gli iteratori

*/
void test_metodi_iteratori(){
    std::cout<<"******** Test metodi speicifi della classe GenericStack::const_iterator *******"<<std::endl;
    GenericStack<double> gs(7);
    gs.push(3.14);
    gs.push(12.33);
    gs.push(5.689);
    gs.push(28);
    //ulteriore esempio sulla gestione di dati di tipo diverso, se necessario.
    gs.push('o');   
    gs.push(57.32113);
    gs.push(85.0098);
    std::cout<<"-------- contenuto dello stack "<<std::endl;
    std::cout << "         " << gs ;
    GenericStack<double>::const_iterator gs_itr = gs.begin();
    GenericStack<double>::const_iterator gs_end = gs.end();
    std::cout<<"-------- valore puntato dal iteratore di begin "<<std::endl;
    std::cout << "         " << *gs_itr <<std::endl;
    //Il valore puntato da end non fa parte degli elementi dello stack.
    //La sua funzione è di segnaposto per la fine della struttura dati.
    try{
       int end_element = *gs_end; 
    }catch(const std::out_of_range& ex){
        std::cout<<"-------- Se si chiama l'operatore di dereferenziamento sull'iteratore di end viene generato un errore std::out_of_range"<<std::endl;
        std::cout << "         " << ex.what() <<std::endl;
    }
    
    //copy-constructor
    GenericStack<double>::const_iterator gs2_itr(gs_itr);
    //operatore di assegnamento
    GenericStack<double>::const_iterator gs3_itr = gs_itr;
    assert(gs_itr == gs2_itr);
    assert(gs_itr == gs3_itr);
    std::cout<<"-------- numero di elementi dello stack (ottenuto tramite differenza di iteratori) "<<std::endl;
    //La classe gestisce in modo automatico la sottrazione a - b, sia nel caso a > b che nel caso di b > a.
    std::cout << "         " << gs_itr - gs_end <<std::endl;
    
    //confronto quale iteratore "viene" dopo un altro dal punto di vista della memoria.
    if(gs_itr > gs_end){
        std::cout<<"-------- è possibile confrontare gli iteratori tramite gli operatori <> "<<std::endl;
    }

    try{
        gs_end--;
    }catch(const std::out_of_range& ex){
        std::cout<<"-------- Se si chiama l'operatore di decremento sull'iteratore di end viene generato un errore std::out_of_range"<<std::endl;
        std::cout << "         " << ex.what() <<std::endl;
    }
    try{
        *gs_end;
    }catch(const std::out_of_range& ex){
        std::cout<<"-------- Lo stesso avviene se si prova ad accedere all'elemento puntato dall'iteratore di end (placeholder)"<<std::endl;
        std::cout << "         " << ex.what() <<std::endl;
    }
    //*gs_itr = 45.5
}

int main(int argc, char *argv[]) {
    test_metodi_fondamentali();
    test_metodi_specifici();
    test_metodi_iteratori();

    const charEqlTarget cel('X');
    const charEqlTarget cel2('D');
    const charEqlTarget cel3('K');

    //Un ultimo esempio per mostrare come lo stack possa avere al suo interno un qualsiasi tipo di dato T
    GenericStack<charEqlTarget> gs(5);
    gs.push(cel);
    gs.push(cel2);
    gs.push(cel3);

  return 0;
}


