/*******************************************************************************
*
*               Desenhando a letra M em uma matriz de leds
*                     http://squids.com.br/arduino
*
*******************************************************************************/
int linhas[8] = {13, 8, 17, 10, 5, 16, 4, 14}; // Pinos que controlam as linhas
int colunas[8] = {9, 3, 2, 12, 15, 11, 7, 6}; // Pinos que controlam as colunas

//  CATODO COMUM - LINHA | ANODO COMUM - COLUNA
byte ON = 1, OFF = 0;
// ANODO COMUM - LINHA | CATODO COMUM - COLUNA
//byte ON = 0, OFF = 1;

void setup() {
  for (int i = 2; i < 18; i++) { // Vai de 2 até 17 definindo os pinos como saida
    pinMode(i, OUTPUT);
  }
} 

void loop() {
  int i, j;

  // letra M
  digitalWrite(colunas[0], ON); // COLUNA 1
  for(i=0;i<8;i++) {
    digitalWrite(linhas[i], OFF); // ACENDE TODAS LINHAS
  }
  delayMicroseconds(20);
  apagar();
  
  digitalWrite(colunas[1], ON); // COLUNA 2
  digitalWrite(linhas[1], OFF); // LINHA 2 (ACENDE)
  delayMicroseconds(20);  
  apagar();
  
  digitalWrite(colunas[2], ON); // COLUNA 3
  digitalWrite(linhas[1], OFF); // LINHA 2 (ACENDE)
  digitalWrite(linhas[2], OFF); // LINHA 3 (ACENDE) 
  delayMicroseconds(20); 
  apagar();
  
  digitalWrite(colunas[3], ON); // COLUNA 4
  digitalWrite(linhas[2], OFF); // LINHA 3 (ACENDE)
  digitalWrite(linhas[3], OFF); // LINHA 4 (ACENDE)
  delayMicroseconds(20);  
  apagar();
  
  digitalWrite(colunas[4], ON); // COLUNA 5
  digitalWrite(linhas[2], OFF); // LINHA 3 (ACENDE)
  digitalWrite(linhas[3], OFF); // LINHA 4 (ACENDE)
  delayMicroseconds(20);      
  apagar();
  
  digitalWrite(colunas[5], ON); // COLUNA 6
  digitalWrite(linhas[1], OFF); // LINHA 2 (ACENDE)
  digitalWrite(linhas[2], OFF); // LINHA 3 (ACENDE)   
  delayMicroseconds(20); 
  apagar();
  
  digitalWrite(colunas[6], ON); // COLUNA 7
  digitalWrite(linhas[1], OFF); // LINHA 2 (ACENDE)
  delayMicroseconds(20);  
  apagar();
  
  digitalWrite(colunas[7], ON); // COLUNA 8
  for(i=0;i<8;i++) {
    digitalWrite(linhas[i], OFF); // ACENDE TODAS LINHAS
  }
  delayMicroseconds(20);
  apagar();
}

void apagar() {
  for (int i=0;i<8;i++) { // 
    digitalWrite(linhas[i],ON); // apaga todas linhas
  }
  for (int i=0;i<8;i++) { // 
    digitalWrite(colunas[i],OFF); // apaga todas colunas
  } 
}
