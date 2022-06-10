#include <iostream>
#include <string>
#include "utils.h"
#include "buffer.h"
#ifndef TREE
#define TREE
const std::string NOT_FOUND_FLAG = "-2";
template <class T>
struct node{
    /*
    Struct da estrutura node.
    Atributos:
      Item<T> item:
        Atributo Item<T> indicando o item armazenado pelo nó
      node *right:
        Ponteiro para node, indicando o nó filho da direita
      node *left:
        Ponteiro para node, indicando o nó filho da esquerda
    Possui apenas uma sobrecarga no operador de atribuição, que garante que dois itens diferentes sejam copiados corretamente.
    */
    Item<T> item;
    node* right;
    node* left;
    node():right(nullptr),left(nullptr){};
    node<T>& operator = (const node<T>& other){
        this->right = other->right;
        this->left = other->left;
        this->item.copy(other->item);
        return *this;
    }
};
template <class T>
class binary_tree{
    /*
    Classe da estrutura Buffer.
    Atributos privados:
      node<T> root:
        Ponteiro para node, indicando a raiz da árvore
    */
    private:
    node<T>* root;
    void insert_recursive(node<T>* &p, Item<T> &item){
    /*
    Método que insere recursivamente um nó na árvore, de acordo com sua chave string, respeitando a ordem alfabética entre as strings.
    É vazio dentro da classe e deve ser especificado para cada tipo.
    Recebe:
      node<T>* &p:
        referência à ponteiro para node, indicando o nó atual no caminhamento da árvore
      Item<T> &item:
        referência à item, indicando o item que se deseja inserir na àrvore
    Não retorna nenhum valor.
    */
    }
    void delete_recursive(node<T>* p){
    /*
    Método que deleta todos os nós da árvore de forma recursiva.
    Recebe:
      node<T>* p:
        ponteiro para node, indicando o nó atual no caminhamento da árvore
    Não retorna nenhum valor.
    */
        if(p != nullptr){
            delete_recursive(p->left);
            delete_recursive(p->right);
            delete p;
        }
    }
    void print_in_order(node<T> *p){
    /*
    Método que improve as chaves de cada nó de acordo com o caminhamento in-ordem.
    Recebe:
      node<T>* p:
        ponteiro para node, indicando o nó atual no caminhamento da árvore
    Não retorna nenhum valor.
    */
        if(p != nullptr){
            print_in_order(p->left);
            std::cout << p->item.key << " ";
            print_in_order(p->right);
        }
    }
    Item<T>* recursive_search(node<T> *node, std::string key){
    /*
    Método que realiza uma pesquisa na árvore de modo recusivo
    Recebe:
      node<T>* node:
        ponteiro para node, indicando o nó atual no caminhamento da árvore
      std::string key:
        string indicando a chave que está sendo buscada
    Retorna:
      Item<T>*:
        ponteiro para Item<T>, contendo ou o item do nó encontrado ou um item com uma flag indicando que essa chave não pertence à árvore.
        
    */
        if(node == nullptr){
            Item<T> *aux = new Item<T>();
            aux->key = NOT_FOUND_FLAG;
            return aux;
        }
        if(compare_strings_alphabet(key,node->item.key,2)){
            return recursive_search(node->left,key);
        }
        else if(compare_strings_alphabet(key,node->item.key,1)){
            return recursive_search(node->right,key);
        }
        else{
            Item<T> *temp = new Item<T>();
            temp->copy(node->item);
            return temp;
        }
    }
    void find_prev(node<T> *q, node<T>* &r){
    /*
    Método que encontra o nó com maior chave na sub-árvore da direita de um determinado nó
    É vazio dentro da classe e deve ser especificado para cada tipo
    Recebe:
      node<T> *q:
        ponteiro para node, que indica o nó o qual se quer encontrar o filho da direita com maior chave
      node<T>* &r:
        referência para ponteiro para node, que indica o nó filho atual no caminhamento
    Não retorna nenhum valor
    */
    }
    void recursive_remove(node<T>* &tgt_node, std::string key){
        if(tgt_node == nullptr){
            throw("Item is not on the tree");
        }
        if(compare_strings_alphabet(key,tgt_node->item.key,2)){
            return recursive_remove(tgt_node->left,key);
        }
        else if(compare_strings_alphabet(key,tgt_node->item.key,1)){
            return recursive_remove(tgt_node->right,key);
        }
        else{
            node<T> *aux;
            if(tgt_node->right == nullptr){
                aux = tgt_node;
                tgt_node = tgt_node->left;
                free(aux);
            }
            else if(tgt_node->left == nullptr){
                aux = tgt_node;
                tgt_node = tgt_node->right;
                free(aux);
            }
            else{
                find_prev(tgt_node,tgt_node->left);
            }
        }
    }
    public:
    binary_tree(){
    /*
    Método construtor, que inicializa a raiz como ponteiro nulo.
    */
        this->root = nullptr;
    }
    ~binary_tree(){
    /*
    Método destrutor, que chama o método clear.
    */
        this->clear();
    }
    void insert(Item<T> &item){
    /*
    Chama o método insert_recursive na raiz da árvore
    Recebe:
      Item<T> &item:
        referência à item, indicando o item que se deseja inserir na àrvore
    Não retorna nenhum valor.
    */
        insert_recursive(this->root,item);
    }
    Item<T> search_and_remove(std::string key){
    /*
    Chama os métodos recursive_search e recursive_remove na raiz da árvore, para uma mesma chave.
    Recebe:
      std::string key:
       string indicando a chave que se deseja procurar e remover da árvore
    Retorna:
      Item<T> item:
        Item<T>, contendo ou o item do nó encontrado ou um item com uma flag indicando que essa chave não pertence à árvore
    */
        Item<T>* result = this->recursive_search(this->root,key);
        this->recursive_remove(this->root,key);
        return *result;
    }

    void print(){
    /*
    Método que chama o método print_in_order.
    Não recebe nem retorna nenhum valor.
    */
        this->print_in_order(this->root);
    }
    void clear(){
    /*
    Método que chama o delete_recursive.
    Não recebe nem retorna nenhum valor.
    */
        this->delete_recursive(this->root);
        this->root = nullptr;
    }
};
template <>
void binary_tree<Buffer>::find_prev(node<Buffer> *q, node<Buffer>* &r){
    /*
    Especificação do método find_prev para o tipo Buffer, que encontra o nó com maior chave na sub-árvore da direita de um determinado nó
    Recebe:
      node<Buffer> *q:
        ponteiro para node, que indica o nó o qual se quer encontrar o filho da direita com maior chave
      node<Buffer>* &r:
        referência para ponteiro para node, que indica o nó filho atual no caminhamento
    Não retorna nenhum valor
    */
        if(r->right != nullptr){
            find_prev(q,r->right);
            return;
        }
        q->item.copy(r->item);
        q = r;
        r = r->left;
        free(q);
};
template <>
void binary_tree<Buffer>::insert_recursive(node<Buffer>* &p, Item<Buffer> &item){
    /*
    Especificação do método insert_recursive para o tipo Buffer, que insere recursivamente um nó na árvore, de acordo com sua chave string, respeitando a ordem alfabética entre as strings.
    Recebe:
      node<Buffer>* &p:
        referência à ponteiro para node, indicando o nó atual no caminhamento da árvore
      Item<Buffer> &item:
        referência à item, indicando o item que se deseja inserir na àrvore
    Não retorna nenhum valor.
    */
        if(p == nullptr){
            p = new node<Buffer>(); 
            p->item.copy(item);
        }
        else{
            if(compare_strings_alphabet(item.key,p->item.key,2)){
                insert_recursive(p->left,item);
            }
            else if(compare_strings_alphabet(item.key,p->item.key,1)){
                insert_recursive(p->right,item);
            }
            else{
                int curr_value;
                while(!item.data.is_empty()){
                    curr_value = item.data.pop_front();
                    p->item.data.push_back(curr_value);
                }
            }
        }
    }
#endif