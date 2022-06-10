QUESTAO2:
  addi x11,x0,1 # inicializa res = 1
  addi x12,x0,2 # inicializa i = 2
  blt x10,x12,FIM # se n < i(ou seja, n < 2), pula para o FIM
  beq x0,x0,LOOP # pula para o LOOP
LOOP:
  mul x11,x11,x12 # res = res * i
  addi x12,x12,1 # i = i + 1
  blt x10,x12,FIM # checa se n < i(pois o loop é enquanto i <= n)
  beq x0,x0,LOOP # volta para o início do loop
FIM:
  jalr x1,x0,0


