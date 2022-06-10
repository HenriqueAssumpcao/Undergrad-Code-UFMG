MAIN:
  # assume que x10 e x11 já foram dados
  add x5,x5,x0 # i = 0
LOOP1:
  addi x6,x11,-1 # x6 = n-1
  bge x5,x6,FIM # se i >= n-1, termino o programa
  add x6,x5,x0 # idx_menor = i
  addi x7,x5,1 #j = i + 1
LOOP2:
  bge x7,x11,CONDICIONAL1 # se j >= tamanho do array, termino o loop interno do j
  # pega array[idx_menor]
  slli x29,x6,2 # x29 = idx_menor*4
  add x30,x10,x29 # x30 aponta para a posição idx_menor do array 
  lw x30,0(x30) # x30 pega o valor dessa posição
  # pega array[j]
  slli x29,x7,2 # x29 = j*4
  add x31,x10,x29 # x30 aponta para a posição j do array 
  lw x31,0(x31) # x30 pega o valor dessa posição
  
  blt x31,x30,CONDICIONAL2# se array[j] < array[idx_menor], vai para CONDICIONAL2 
  addi x7,x7,1 # j++
  beq x0,x0,LOOP2
  
CONDICIONAL1:
  bne x5,x6,SWAP # se idx_menor != i, troca as posições de lugar
  addi x5,x5,1 # i++
  beq x0,x0,LOOP1
  
CONDICIONAL2:
  add x6,x7,x0 # idx_menor = j
  addi x7,x7,1 # j++
  beq x0,x0,LOOP2 # proxima iteração do loop
  
SWAP:
  # pega array[idx_menor]
  slli x29,x6,2 # x29 = idx_menor*4
  add x30,x10,x29 # x30 aponta para a posição idx_menor do array 
  lw x30,0(x30) # x30 pega o valor dessa posição
  # pega array[i]
  slli x29,x5,2 # x29 = i*4
  add x31,x10,x29 # x30 aponta para a posição i do array 
  lw x31,0(x31) # x30 pega o valor dessa posição
  # faz a troca
  add x29,x31,x0 # temp = array[i]
  add x31,x30,x0 # array[i] = array[idx_menor]
  add x30,x29,x0 # array[idx_menor] = temp
  
  addi x5,x5,1 # i++
  beq x0,x0,LOOP1 # proxima iteração do loop
  
FIM:
  jalr x1,x0,0
  