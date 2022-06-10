#include <iostream>
#include <list>
#include <map>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include "client.h"
#include "store.h"
#include "point.h"

void read_data(char *argv[],std::vector<Client> &clients, std::vector<Store> &stores){
    /*
    Função que lê o arquivo de entrada e processa os dados.
    Recebe:
      char *argv[]: Argumentos dados na linha de comando
      std::vector<Client> &clients: Vetor de clientes passado por referência, para que possa ter clientes adicionados durante o processamento.
      std::vector<Store> &stores: Vetor de lojas passado por referência, para que possa ter lojas adicionadas durante o processamento.
    */
    std::ifstream infile(argv[1]); // Lê o arquivo indicado pela linha de comando
    if(!infile){
        // Caso o arquivo não exista, encerra o programa
        std::cerr << "Error: Unable to open file!" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string txt; // Variável que irá armazenar todo o texto de uma dada linha do arquivo
    std::string delim = " ", token; // Variáveis auxiliares para o parsing das strings
    int pos = 0; // Variável auxiliar para o parsing das strings
    int num_stores;
    int counter = 0,client_counter = 0; // Contador geral do loop e contador específico para o id dos clientes
    while(std::getline(infile,txt)){ // Loop while que lê cada linha do arquivo contendo os dados
        if(counter == 1){ // Lê o número de lojas
            num_stores = std::stoi(txt);
        }
        else if(1 < counter && counter <= (1+num_stores)){ // Lê as informações de todas as lojas
            int stock,x,y;
            pos = txt.find(delim);
            token = txt.substr(0,pos);
            txt.erase(0,pos + delim.length());
            stock = std::stoi(token);
        
            pos = txt.find(delim);
            token = txt.substr(0,pos);
            txt.erase(0,pos + delim.length());

            x = std::stoi(token);
            y = std::stoi(txt);

            stores.push_back(Store(counter-2,stock,x,y));
        }
        else if (counter > (2+num_stores)){ // Lê as informações de todos os clientes
            int age,x,y;
            std::string pay_method,state;

            pos = txt.find(delim);
            token = txt.substr(0,pos);
            txt.erase(0,pos + delim.length());
            age = std::stoi(token);

            pos = txt.find(delim);
            token = txt.substr(0,pos);
            txt.erase(0,pos + delim.length());
            state = token;

            pos = txt.find(delim);
            token = txt.substr(0,pos);
            txt.erase(0,pos + delim.length());
            pay_method = token;

            pos = txt.find(delim);
            token = txt.substr(0,pos);
            txt.erase(0,pos + delim.length());
            x = std::stoi(token);
            y = std::stoi(txt);

            clients.push_back(Client(client_counter,age,state,pay_method,x,y));
            client_counter++;

        }
        counter++;
        }
    infile.close();
}

int main(int argc, char *argv[]){

    std::vector<Client> clients; // vetor de clientes
    std::vector<Store> stores; // vetor de lojas
    read_data(argv,clients,stores); // carrega as informações de input nos vetores

    std::vector<Client> sorted_clients = clients;
    std::sort(sorted_clients.begin(),sorted_clients.end(),&client_bigger); // ordena uma cópia do vetor de clientes de acordo com a ordem definida em client_bigger
    std::list<int> unmatched_stores; // fila de ids de lojas que ainda não estão cheias
    int* original_stocks = new int[stores.size()]; // array que contem os valores originais dos estoques que serão utilizados na impressão dos resultados finais
    for(unsigned int i = 0; i < stores.size();i++){ // inicializa a lista de preferências de cada loja e a fila de lojas não cheias
        stores[i].set_preferences(sorted_clients);
        unmatched_stores.push_back(stores[i].get_id());
        original_stocks[i] = stores[i].get_stock();
    }

    int* matchings = new int[clients.size()]; // array de agendamentos. Cada posição i indica que o cliente i está agendado à loja matchings[i]
    for(unsigned int i = 0; i < clients.size(); i++){
        matchings[i] = UNMATCHED_CLIENT; // inicializa todas suas posições com o valor UNMATCHED_CLIENT=-1
    }

    while(unmatched_stores.size() != 0){ // loop do algoritmo principal
        int curr_store_id = unmatched_stores.front();
        Store* curr_store = &stores[curr_store_id];
        if(curr_store->is_pref_empty()){ // checa se a lista de preferências da loja atual está vazia, i.e. a loja já fez ofertas a todos os clientes
            unmatched_stores.pop_front(); // remove a loja atual da fila e passa para a próxima
            curr_store_id = unmatched_stores.front();
            curr_store = &stores[curr_store_id];
        }
        Client curr_client = curr_store->get_next_client();
        if(matchings[curr_client.get_id()] == UNMATCHED_CLIENT){ // checa se o cliente atual está sem agendamento
            matchings[curr_client.get_id()] = curr_store->get_id(); // agenda o cliente 
            curr_store->decrease_stock(1); // diminui o estoque da loja em 1
            if(curr_store->get_stock() == 0){ // checa se o estoque é igual a zero, se sim remove a loja da fila
                unmatched_stores.pop_front();
            }
        }
        else{
            int matched_id = matchings[curr_client.get_id()];
            float curr_dist = grid_distance(curr_client.get_pos(),curr_store->get_pos()), comp_dist = grid_distance(curr_client.get_pos(),stores[matched_id].get_pos());
            if(curr_dist < comp_dist || ((curr_dist == comp_dist) && (curr_store->get_id() < stores[matched_id].get_id()))){ 
                // checa se o cliente que já possui agendamento prefere a loja atual em vez da loja a qual ele está pareado
                matchings[curr_client.get_id()] = curr_store->get_id(); // troca o agendamento do cliente
                if (stores[matched_id].get_stock() == 0){
                    unmatched_stores.push_back(matched_id); //coloca a loja que perdeu o agendamento na fila de lojas não cheias
                }
                stores[matched_id].decrease_stock(-1); // aumenta o estoque da loja que perdeu o agendamento em 1
                curr_store->decrease_stock(1); // diminui o estoque da loja que ganhou o agendamento em 1
                if(curr_store->get_stock() == 0){ // checa se o estoque é igual a zero, se sim remove a loja da fila
                    unmatched_stores.pop_front();
                }
            }
        }
    }

    
    for(unsigned int i = 0; i < stores.size(); i++){ // imprime os resultados no formato esperado
        int cout_counter = 0;
        int curr_store_id = stores[i].get_id();
        std::cout << stores[i].get_id() << std::endl;
        for(unsigned int j = 0; j < sorted_clients.size(); j++){
            int curr_client_id = sorted_clients[j].get_id();
            if(matchings[curr_client_id] == curr_store_id){
                if(cout_counter < original_stocks[i] - 1){
                    std::cout << curr_client_id << " ";
                }
                else{
                    std::cout << curr_client_id;
                }
                cout_counter++;
            }
        }
        std::cout << std::endl;
    }

}
