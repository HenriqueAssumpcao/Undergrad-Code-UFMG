#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include "buffer.h"


int main(int argc, char *argv[]){
    std::ifstream infile(argv[1]); // Lê o arquivo indicado pela linha de comando
    if(!infile){
        // Caso o arquivo não exista, encerra o programa
        std::cerr << "Error: Unable to open file!" << std::endl;
        return 0;
    }
    int num_serv;
    infile >> num_serv; // Lê o número de buffers da primeira linha do arquivo
    if(num_serv <= 0){
        return 0;
    }

    Buffer<std::string> *servers = new Buffer<std::string>[num_serv]; // Inicializa o array de buffers
    Buffer<std::string> hist; // Inicializa o historico de consciencias enviadas

    std::string txt; // Variável que irá armazenar todo o texto de uma dada linha do arquivo
    std::string delim = " ", token; // Variáveis auxiliares para o parsing das strings
    int pos = 0; // Variável auxiliar para o parsing das strings
    int tgt_serv, dest_serv, serv_pos; // Variáveis auxiliares para execução de comandos

    while(std::getline(infile,txt)){ // Loop while que lê cada linha do arquivo contendo os comandos
        if(txt.size() == 0){
            continue;
        }
        // Recebe o comando
        pos = txt.find(delim);
        token = txt.substr(0,pos);
        txt.erase(0,pos + delim.length());
        // Identifica o comando
        if(token == "INFO"){
            // Recebe os parâmetros específicos do comando
            pos = txt.find(delim);
            token = txt.substr(0,pos);
            txt.erase(0,pos + delim.length());
            tgt_serv = atoi(token.c_str());
            // Checa se o comando realiza operações válidas nos buffers
            if((tgt_serv >= num_serv) || (tgt_serv < 0)){
                continue;
            }
            // Executa o comando
            servers[tgt_serv].push_back(txt);
        }
        if(token == "WARN"){
            // Recebe os parâmetros específicos do comando
            pos = txt.find(delim);
            token = txt.substr(0,pos);
            txt.erase(0,pos + delim.length());
            tgt_serv = atoi(token.c_str());
            serv_pos = atoi(txt.c_str());
            // Checa se o comando realiza operações válidas nos buffers
            if((tgt_serv >= num_serv) || (tgt_serv < 0)){
                continue;
            }
            else if((serv_pos >= servers[tgt_serv].get_size()) || (serv_pos < 0)){
                continue;
            }
            // Executa o comando
            Item<std::string> curr_item = servers[tgt_serv].pop_pos(serv_pos);
            servers[tgt_serv].push_front(curr_item);
        }
        if(token == "TRAN"){
            // Recebe os parâmetros específicos do comando
            pos = txt.find(delim);
            token = txt.substr(0,pos);
            txt.erase(0,pos + delim.length());
            tgt_serv = atoi(token.c_str());
            dest_serv = atoi(txt.c_str());
            // Checa se o comando realiza operações válidas nos buffers
            if((tgt_serv >= num_serv) || (tgt_serv < 0) || (dest_serv >= num_serv) || (dest_serv < 0)){
                continue;
            }
            // Executa o comando
            while(!servers[tgt_serv].is_empty()){
                Item<std::string> curr_item = servers[tgt_serv].pop_front();
                servers[dest_serv].push_back(curr_item);
            }
            servers[tgt_serv].clear();
        }
        if(token == "ERRO"){
            // Recebe os parâmetros específicos do comando
            tgt_serv = atoi(txt.c_str());
           // Checa se o comando realiza operações válidas nos buffers
            if((tgt_serv >= num_serv) || (tgt_serv < 0)){
                continue;
            }
            // Executa o comando
            std::cout << token << " " << txt << std::endl;
            servers[tgt_serv].print();
            servers[tgt_serv].clear();
        }
        if(token == "SEND"){
            // Executa o comando
            Item<std::string> curr_item;
            for(int i = 0; i < num_serv; i++){
                try{ // Caso o buffer esteja vazio, não remove seu primeiro item
                    curr_item = servers[i].pop_front();
                }
                catch(Buffer<std::string>::EmptyBufferError e){
                    continue;
                }
                hist.push_back(curr_item);
            }
        }
        if(token == "FLUSH"){
            // Executa o comando
            hist.print();
            for(int i = 0; i < num_serv; i++){
                servers[i].print();
            }
        }
    }
    infile.close();
    delete[] servers;

}