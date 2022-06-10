#ifndef BUFFER
#define BUFFER
#include <iostream>
#include <string>
const int HEAD_FLAG = -1;
template <class T>
struct Item{
    /*
    Struct da estrutura Item. 
    Atributos:
      T content:
        Conteúdo armazenado pelo Item. É do tipo T definido no template. É inicializado com Value Initialization(https://en.cppreference.com/w/cpp/language/value_initialization)
      int key:
        Chave identificadora do Item
    */
    T content{};
    int key;
    Item(T c){
            /*
        Método construtor. Inicializa apenas o atributo content
    */
        this->content = c;
    };
    Item() = default; // Método construtor default
    
};
template <class T>
struct BufferCell{
    /*
    Struct da estrutura BufferCell.
    Atributos:
      Item<T> item:
        Atributo Item armazenado pelo BufferCell
      BufferCell<T> *next:
        Ponteiro para BufferCell<T>, indicando o próximo elemento do buffer
    */
    Item<T> item;
    BufferCell<T> *next;
    BufferCell(Item<T> item, BufferCell<T>* next):item(item),next(next){}; // Método construtor. Inicializa os atributos item e next
    BufferCell(Item<T> item):item(item),next(nullptr){}; // Método construtor que inicializa o atributo item e armazena nullptr em next
};


template <class T>
class Buffer{
    /*
    Classe da estrutura Buffer.
    Atributos privados:
      BufferCell<T>* front:
        Ponteiro para BufferCell<T>, indicando a célula cabeça do buffer
      BufferCell<T>* back:
        Ponteiro para BufferCell<T>, indicando a última célula do buffer
      int size:
        Número de células armazenadas no buffer(sem contar a célula cabeça). É inicializado por default com 0
      int key_counter:
        Contador que identifica a chave identificadora de itens armazenados nas células do buffer. É inicializado por default com 0
    */
    private:
    BufferCell<T>* front;
    BufferCell<T>* back;
    int size = 0;
    int key_counter = 0;
    public:
    Buffer(){
            /*
        Método construtor. Inicializa o atributo front com uma nova BufferCell<T> que será a célula cabeça. Também inicializa o atributo back igual ao front
    */
        Item<T> head_item;
        head_item.key = HEAD_FLAG;
        this->front = new BufferCell<T>(head_item);
        this->back = this->front;
    };
    ~Buffer(){
            /*
        Método destrutor. Chama o método clear() e depois deleta o conteúdo do atributo front
    */
        this->clear();
        delete this->front;
    }
    class EmptyBufferError{
            /*
            Classe que representa um erro ao se tentar acessar um buffer vazio.
            Atributos públicos:
              std::string message:
                Mensagem que informa o erro
    */
        public:
        std::string message = "EmptyBufferError: Trying to access an empty Buffer!";
    };
    bool is_empty(){
            /*
        Método que informa se o buffer está vazio. Não recebe valores como entrada.
        Não recebe nenhum valor de entrada.
        Retorna:
          Valor de tipo bool
    */
        if(this->size == 0){
            return true;
        }
        return false;
    }
    int get_size(){
            /*
        Método get que retorna o atributo size do Buffer.
        Não recebe nenhum valor de entrada.
        Retorna:
          int this->size
    */
        return this->size;
    }
    void push_back(Item<T> item){
            /*
        Método que insere um elemento no fim do buffer.
        Recebe:
          Item<T> item:
            Objeto do tipo Item<T> a ser inserido no fim do buffer
        Não retorna nenhum valor.
    */
        item.key = this->key_counter;
        BufferCell<T>* new_cell = new BufferCell<T>(item);
        this->back->next = new_cell;
        this->back = new_cell;
        this->size++;
        this->key_counter++;
    }
    void push_back(T content){
            /*
        Método sobrecarregado que instancia um objeto do tipo Item e chama o método void push_back(Item<T> item) para inserir um Item no fim do buffer.
        Recebe:
          T content:
            Valor do conteúdo a ser armazenado pelo Item que será inserido no fim do buffer.
        Não retorna nenhum valor.
    */
        this->push_back(Item<T>(content));
    }
    void push_front(Item<T> item){
            /*
        Método que insere um elemento no início do buffer.
        Recebe:
          Item<T> item:
            Objeto do tipo Item<T> a ser inserido no início do buffer
        Não retorna nenhum valor.
    */
        item.key = this->key_counter;
        BufferCell<T>* new_cell = new BufferCell<T>(item);
        new_cell->next = this->front->next;
        this->front->next = new_cell;
        this->size++;
        this->key_counter++;
    }
    void push_front(T content){
            /*
        Método sobrecarregado que instancia um objeto do tipo Item e chama o método void push_front(Item<T> item) para inserir um Item no início do buffer. 
        Recebe:
          T content:
            Valor do conteúdo a ser armazenado pelo Item que será inserido no fim do buffer.
        Não retorna nenhum valor.
    */
        this->push_front(Item<T>(content));
    }
    Item<T> pop_front(){
            /*
        Método que remove a primeira célula do buffer e retorna o seu Item. 
        Não recebe nenhum valor de entrada.
        Retorna:
          Item<T> item:
            Objeto do tipo Item<T> armazenado pela primeira célula do buffer.
        Joga uma instância de EmptyBufferError se o buffer estiver vazio.
    */
        if(this->is_empty()){
            throw EmptyBufferError();
        }
        BufferCell<T>* temp = this->front;
        Item<T> item = this->front->next->item;
        this->front = this->front->next;
        this->front->item.key = HEAD_FLAG;
        delete temp;
        this->size--;
        if(this->front->next == nullptr){
            this->back = this->front;
        }
        return item;
    }
    Item<T> pop_pos(int pos){
            /*
        Método que remove a célula da posição pos do buffer e retorna o seu Item.
        Recebe:
          int pos:
            Valor que representa a posição da célula a ser removida do buffer.
        Retorna:
          Item<T> item:
            Objeto do tipo Item<T> armazenado pela célula da posição pos do buffer.
        Joga uma instância de EmptyBufferError se o buffer estiver vazio.
    */
        if(this->is_empty()){
            throw EmptyBufferError();
        }
        BufferCell<T> *temp = this->front, *aux;
        int curr_pos = -1;
        while(curr_pos != (pos-1)){
            temp = temp->next;
            curr_pos++;
        }
        aux = temp->next;
        Item<T> item = aux->item;
        temp->next = aux->next;
        delete aux;
        this->size--;
        if(temp->next == nullptr){
            this->back = temp;
        }
        return item;
    }
    void print(){
            /*
            Método que imprime os valores do buffer de acordo com a saída padrão stdout. 
            Não recebe nenhuma entrada e não retorna nenhum valor.
            É inicializado vazio dentro da classe.
    */
    }
    void clear(){
            /*
            Método que deleta todas as células do buffer, com exceção da célula cabeça. Esse método chama o método Item<T> pop_front até que o buffer esteja vazio.
            Não recebe nenhuma entrada e não retorna nenhum valor.
    */
        this->key_counter = 0;
        if(this->is_empty()){
            return;
        }
        while(!this->is_empty()){
            this->pop_front();
        }
    }
};
template <>
void Buffer<std::string>::print(){
            /*
        Especificação do método void print() da classe Buffer para o tipo std::string.
        Não recebe nenhuma entrada e não retorna nenhum valor.
    */
    if(this->is_empty()){
        return;
    }
    BufferCell<std::string>* temp = this->front->next;
    std::string aux;
    while(temp != nullptr){
        aux = temp->item.content;
        char aux2 = '\"';
        for(unsigned int i = 0; i < aux.size(); i++){
            if(aux[i] != aux2)
            std::cout << aux[i];
        }
        std::cout << std::endl;
        temp = temp->next;
    }
}
#endif