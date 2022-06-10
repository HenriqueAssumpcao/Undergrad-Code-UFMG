QUESTAO1:
  beq x10,x0,CASO1 # se P = 0
  beq x11,x0,CASO2 # se V = 0
  beq x12,x0,CASO3 # se I = 0
CASO1:
  beq x11,x0,RETURN0 # se V = 0, vai para RETURN0, que seta todas as variaveis para 0 e encerra
  beq x12,x0,RETURN0 # se I = 0, vai para RETURN0, que seta todas as variaveis para 0 e encerra
  mul x10,x11,x12 # P = V * I
  j FIM
CASO2:
  beq x10,x0,RETURN0 # se P = 0, vai para RETURN0, que seta todas as variaveis para 0 e encerra
  beq x12,x0,RETURN0 # se I = 0, vai para RETURN0, que seta todas as variaveis para 0 e encerra
  fcvt.s.w f10,x10 # converte x10 para float
  fcvt.s.w f12,x12 # converte x12 para float
  fdiv.s f11,f10,f12 # V = P/I, salvo em float
  j FIM
CASO3:
  beq x10,x0,RETURN0 # se P = 0, vai para RETURN0, que seta todas as variaveis para 0 e encerra
  beq x11,x0,RETURN0 # se V = 0, vai para RETURN0, que seta todas as variaveis para 0 e encerra
  fcvt.s.w f10,x10 # converte x10 para float
  fcvt.s.w f11,x11 # converte x11 para float
  fdiv.s f12,f10,f11 # I = P/V
  j FIM
  
RETURN0: # seta as variáveis pra 0 caso mais de uma seja 0
  add x10,x0,x0 
  add x11,x0,x0
  add x12,x0,x0
  j FIM
FIM:
  jalr x1, x0, 0
