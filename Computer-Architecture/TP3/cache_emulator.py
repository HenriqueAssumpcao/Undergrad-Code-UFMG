import argparse
import math

class cache_block():
    # Classe que representa o bloco de memória cache. Todos seus atributos são inicializados como zero. Contém o valid bit, dirty bit, a tag e os dados.
    def __init__(self,num_words): 
        self.valid_bit = 0
        self.dirty_bit = 0
        self.tag = 0
        self.data = [[] for i in range(num_words)] # Note que os dados são uma lista que no caso contém espaço para 4 palavras de 32 bits
    def __str__(self):
        # Método que auxilia na debugagem do código
        return f"tag:{self.tag},valid_bit:{self.valid_bit},dirty_bit:{self.dirty_bit},data:{self.data}"


def read_data(input_file):
    # Lê os dados do arquivo de entrada e os armazena em duas listas
    data = []
    instructions = []
    for i in input_file:
        i = i.rstrip()
        inputs = i.split(' ')
        instructions.append(i)
        if len(inputs) == 3:
            data.append([int(inputs[0]),inputs[2][:32]])
        else:
            data.append(int(inputs[0]))
    return data,instructions

if __name__ == "__main__":
    """
    Funcionamento do programa:
    O programa implementado simula o funcionamento de uma hierarquia de memória, onde possuímos um cache com 64 blocos e 16 bytes por bloco(4 palavras),
    e uma memória de dados capaz de armazenar 1024 palavras de 32 bits. A política de mapeamento utilizada é a de mapeamento direto, e utiliza-se o Write Back para lidar
    com os writes.
    1. A memória é representada por uma lista, onde cada elemento representa um endereço.
      É importante reforçar que cada endereço representa uma PALAVRA, e não o byte de uma palavra, ou seja, cada elemento da lista é um endereço da memória que
      armazena uma palavra de 32 bits. Também podemos pensar que cada endereço representa um bloco de 4 bytes. Por simplicidade, representaremos cada bloco como uma unidade,
      ou seja, não utilizaremos um endereço para cada byte. Essa decisão não afeta a correture do programa, e apenas simplifica a implementação realizada. Para garantir a corretude, mapeamos o enderço de byte da entrada para um endereço de palavra, i.e., o endereço 5 de byte é mapeado para a palavra de endereço 1 (5//4 = 1)
    2. Os blocos são uma lista de objetos do tipo cache_block.
    3. O programa itera sobre as instruções fornecidas, processando os Writes e os Reads da seguinte forma:
      3.1. Reads: O programa mapeia o endereço de palavra fornecido para um bloco e um índice dentro do bloco e checa pelo bit de validade. Se o bit de validade for falso, o programa contabiliza o miss e então
           buscamos o dado na memória,e como cada bloco pode armazenar 4 palavras, buscamos também as 3 palavras adjacentes à essa palavra. 
           Por exemplo, se o endereço X é mapeado para a posição 3 do bloco B e o bit de validade estiver falso, buscamos o dado referente a X na memória, 
           bem como os dados referentes a X-3,X-2,X-1. Colocamos então a palavra X na variável variável retrieved_data, que é uma lista contendo os dados de todos os Reads.
           Se o bit de validade estiver aceso e a tag do endereço for a correta, o programa contabiliza um hit e armazena a palavra do endereço desejado na 
           variável retrieved_data. Se o bit de validade estiver aceso e a tag for errada, cheacamos pelo dirty bit. Se o dirty bit estiver aceso, realizamos o write back,
           i.e., escrevemos os dados desse bloco na memória e então buscamos pelos dados desejados. Caso contrário, apenas escrevemos os dados no bloco.
           Reads.
      3.2. Writes: O programa mapeia o endereço de palavra fornecido para um bloco e um índice dentro do bloco. Se o dirty bit for verdadeiro realiza-se o write back, e então ele escreve o dado de entrada,
      bem como os dados dos endereços adjacentes, no bloco de cache em questão. Por exemplo, o write recebe um endereço A e um dado X de entrada, que é então mapeado para um bloco Y no índice
      2, o programa escreve X no índice 2 do bloco Y, e escreve os dados de A-2,A-1,A+1 nos índices adjacentes do bloco. 
    """
    # Argumentos da linha de comando
    parser = argparse.ArgumentParser()
    parser.add_argument('--input_path',type=str,help='path to input file',default='inputs.txt')
    parser.add_argument('--mem_size',type=int,help='memory size',default=1024)
    parser.add_argument('--num_blocks',type=int,help='number of blocks',default=64)
    parser.add_argument('--word_size',type=int,help='word size',default=32)
    parser.add_argument('--num_words_per_block',type=int,help='number of words per block',default=4)

    args = parser.parse_args()
    

    input_file = open(args.input_path,"r")
    data,instructions = read_data(input_file)
    
    # Parâmetros da memória e da cache
    mem_size = args.mem_size # tamanho da memória
    num_blocks = args.num_blocks # Número de blocos
    word_size = args.word_size # Tamanho de cada endereço/palavra

    tag_size = int(math.log2(num_blocks))

    memory = [i for i in range(mem_size)]
    num_words = args.num_words_per_block 
    blocks = [cache_block(num_words) for _ in range(num_blocks)]

    # Variáveis que irão armazenar a solução
    solution = []
    hit_count = 0
    miss_count = 0 
    write_count = 0
    read_count = 0

    retrieved_data = [] # Armazena as palavras retornadas pelas operações de READ

    for inst_counter,curr_data in enumerate(data):
        if isinstance(curr_data,int): # Operação de read
            read_count += 1

            mem_addr = curr_data // num_words # endereço de memória, dividimos número de palavras pois o endereço é de byte e devemos mapea-lo para a palavra equivalente
            block_idx = (mem_addr % num_blocks) // num_words # índice do bloco que deverá ser acessado
            block_offset = (mem_addr % num_blocks) % num_words # offset dentro do bloco, i.e., qual posição a palavra irá ocupar, pois o bloco armazena 4 palavras
            curr_tag = mem_addr // num_blocks  # tag do endereço atual
            mem_addr_start = mem_addr - block_offset # endereço de mesma tag que mapeia para a primeira posição do mesmo bloco, i.e., dado um endereço X que é mapeado para a segunda posição do bloco Y, essa variável armazena X-2

            tgt_block = blocks[block_idx] # bloco que se deseja acessar

            if not tgt_block.valid_bit: # Caso o valid bit não esteja aceso, é um Miss
                tgt_block.valid_bit = 1  # Ligamos o valid bit
                tgt_block.dirty_bit = 0  # Não ligamos o dirty bit, pois é um read
                tgt_block.tag = curr_tag # Setamos a tag

                for idx in range(num_words): # Escrevemos a palavra desejada, bem como as adjacentes
                    tgt_block.data[idx] = memory[mem_addr_start + idx] 
                # Contabilizamos o Miss e retornamos o dado procurado
                retrieved_data.append(tgt_block.data[block_offset])
                solution.append(instructions[inst_counter] + ' M\n') # variável que formata a solução para o formato pedido
                miss_count += 1 

            else: # Caso o valid bit esteja aceso
                if tgt_block.tag == curr_tag: # Se a tag for a correta, é um Hit, dai retornamos o dado armazenado e contabilizamos o hit
                    solution.append(instructions[inst_counter] + ' H\n') # variável que formata a solução para o formato pedido
                    retrieved_data.append(tgt_block.data[block_offset])
                    hit_count += 1
                else: # Caso a tag seja incorreta, é um Miss
                    if tgt_block.dirty_bit: # Se o dirty bit estiver ligado, devemos realizar o WRITE BACK
                        prev_mem_addr_start = (tgt_block.tag * num_blocks) + (block_idx * num_words) # achamos o mem_addr_start dos dados antigos armazenados no bloco
                        for idx in range(num_words): # Escrevemos os 4 dados na memória de dados
                            memory[prev_mem_addr_start + idx] = tgt_block.data[idx]
                        tgt_block.dirty_bit = 0 # Setamos o dirty bit pra zero
                
                    for idx in range(num_words): # Colocamos os dados que queriamos acessar no bloco
                        tgt_block.data[idx] = memory[mem_addr_start + idx] 
                    tgt_block.tag = curr_tag #setamos a tag
                    # Contabilizamos o Miss e retornamos o dado procurado
                    retrieved_data.append(tgt_block.data[block_offset])
                    solution.append(instructions[inst_counter] + ' M\n') # variável que formata a solução para o formato pedido
                    miss_count += 1 
        else: #Operação de Write, não contabilizamos Hits ou Misses
            solution.append(instructions[inst_counter] + " W\n") # variável que formata a solução para o formato pedido
            write_count += 1

            mem_addr = curr_data[0]  // num_words # Endereço de memória
            tgt_data = curr_data[1] # Dados que devem ser escritos
 

            block_idx = (mem_addr % num_blocks) // num_words # índice do bloco que deverá ser acessado
            block_offset = (mem_addr % num_blocks) % num_words # offset dentro do bloco, i.e., qual posição a palavra irá ocupar, pois o bloco armazena 4 palavras
            curr_tag = mem_addr // num_blocks  # tag do endereço atual
            mem_addr_start = mem_addr - block_offset # endereço de mesma tag que mapeia para a primeira posição do mesmo bloco, i.e., dado um endereço X que é mapeado para a segunda posição do bloco Y, essa variável armazena X-2

            tgt_block = blocks[block_idx]
            if tgt_block.dirty_bit: # Checamos pelo dirty bit, e caso ele esteja aceso realizamos o WRITE BACK
                prev_mem_addr_start = (tgt_block.tag * num_blocks) + (block_idx * num_words)
                for idx in range(num_words):
                    memory[prev_mem_addr_start + idx] = tgt_block.data[idx]
            tgt_block.valid_bit = 1  # setamos o valid bit para um
            tgt_block.dirty_bit = 1  # setamos o dirty bit para um 
            tgt_block.data[block_offset] = tgt_data # escrevemos o dado na posição correta
            for idx in range(num_words): # escrevemos os dados de endereços adjacentes nas demais posições
                if idx != block_offset:
                    tgt_block.data[idx] = memory[mem_addr_start + idx]
            tgt_block.tag =  curr_tag  #setamos a tag

       
    stats = f"READS: {read_count}\nWRITES: {write_count}\nHITS: {hit_count}\nMISSES:{miss_count}\nHIT RATE: {hit_count/(hit_count+miss_count)}\nMISS RATE: {miss_count/(hit_count+miss_count)}\n\n"
    solution.insert(0,stats)
    with open("results.txt",'w') as outfile:
        outfile.writelines(solution)
