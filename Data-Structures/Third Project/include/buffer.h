#ifndef BUFFER
#define BUFFER
#include <iostream>
#include <string>
#include <cassert>
const int HEAD_FLAG = -1;
struct BufferCell{
    /*
    Struct da estrutura BufferCell.
    Atributos:
      int value:
        Valor inteiro armazenado pela BufferCell
      BufferCell *next:
        Ponteiro para BufferCell, indicando o próximo elemento do buffer
    */
    int value;
    BufferCell *next;
    BufferCell(int value, BufferCell* next):value(value),next(next){}; 
    BufferCell(int value):value(value),next(nullptr){}; 
};

class Buffer{
    /*
    Classe da estrutura Buffer.
    Atributos privados:
      BufferCell* front:
        Ponteiro para BufferCell, indicando a célula cabeça do buffer
      BufferCell* back:
        Ponteiro para BufferCell, indicando a última célula do buffer
      int size:
        Número de células armazenadas no buffer(sem contar a célula cabeça). É inicializado por default com 0
    */
    private:
    BufferCell* front;
    BufferCell* back;
    int size = 0;
    public:
    Buffer(){
            /*
        Método construtor. Inicializa o atributo front com uma nova BufferCell que será a célula cabeça. Também inicializa o atributo back igual ao front
    */
        this->front = new BufferCell(HEAD_FLAG);
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
    void push_back(int value){
            /*
        Método que insere um elemento no fim do buffer.
        Recebe:
          int value:
            Objeto do tipo int a ser inserido no fim do buffer
        Não retorna nenhum valor.
    */
        BufferCell* new_cell = new BufferCell(value);
        this->back->next = new_cell;
        this->back = new_cell;
        this->size++;
    }
    void push_front(int value){
            /*
        Método que insere um elemento no início do buffer.
        Recebe:
          int value:
            Objeto do tipo int a ser inserido no início do buffer
        Não retorna nenhum valor.
    */
        BufferCell* new_cell = new BufferCell(value);
        new_cell->next = this->front->next;
        this->front->next = new_cell;
        this->size++;
    }
    int pop_front(){
            /*
        Método que remove a primeira célula do buffer e retorna o seu valor. 
        Não recebe nenhum valor de entrada.
        Retorna:
          int value:
            Objeto do tipo int armazenado pela primeira célula do buffer.
        Joga uma instância de EmptyBufferError se o buffer estiver vazio.
    */
        if(this->is_empty()){
            throw EmptyBufferError();
        }
        BufferCell* temp = this->front;
        this->front = temp->next;
        int value = this->front->value;
        this->front->value = HEAD_FLAG;
        this->size--;
        if(this->front->next == nullptr){
            this->back = this->front;
        }
        delete temp;
        return value;
    }
    void clear(){
            /*
            Método que deleta todas as células do buffer, com exceção da célula cabeça. Esse método chama o método Item<T> pop_front até que o buffer esteja vazio.
            Não recebe nenhuma entrada e não retorna nenhum valor.
    */
        if(this->is_empty()){
            return;
        }
        while(!this->is_empty()){
            this->pop_front();
        }
    }
    void copy_content(Buffer &b2){
            /*
            Método que copia o conteúdo de um segundo buffer para o buffer em questão.
            Recebe:
                Buffer &b2:
                    Objeto do tipo Buffer passado por referência.
            Não retorna nenhum valor
    */
        this->clear();
        if(b2.get_size() != 0){
            BufferCell *temp = b2.front->next;
            while(temp != nullptr){
                this->push_back(temp->value);
                temp = temp->next;
            }
        }
    };
};

#endif