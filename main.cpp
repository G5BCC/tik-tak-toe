#include <Adafruit_NeoPixel.h>
#include <Keypad.h>
#include <SoftwareSerial.h>

using namespace std;

// P2P Variaveis
#define TxPin 2
#define RxPin 3

char c;
String recData;

SoftwareSerial MySerial = SoftwareSerial(RxPin, TxPin);


char key;
int display_pin = 13;
int length_game = 9;


int id = 1; // ID do arduino

// CORES
int rl = 255; int gl = 0; int bl = 0; // cor local
int ro = 0; int go = 255; int bo = 0; // cor adversario

// BARRA DE LED
Adafruit_NeoPixel display1 = Adafruit_NeoPixel(length_game, display_pin, NEO_GRB + NEO_KHZ800);

// ESTADOS
enum {
	OCIOSO,
  	INICIO,
  	VEZ1,
  	VITORIA1,
  	VEZ2,
  	VITORIA2,
  	EMPATE,
  	RESET,

} STATE = OCIOSO;


// TABULEIRO
int game[3][3] = {
  {0,0,0},
  {0,0,0},
  {0,0,0}
};


char keys[3][3] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'}
};


// VARIAVEIS DO TECLADO
byte rowPins[3] = {9, 8, 7}; 
byte colPins[3] = {6, 5, 4}; 

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, 3, 3 );



// FUNÇÕES DE COMUNICAÇÃO

void filter_string(){
    int pos = 0;
    String string = recData;
    String tempString = "";
    int after_delimiter = 20;
  while(pos != after_delimiter+5){

    if(string[pos] == '|') after_delimiter = pos;
    else if(pos > after_delimiter) tempString += recData[pos];
    
    pos++;
  }

  recData = tempString;



}

void transmitting(String data){
    recData = "";
    recData = ".. |" + data;
    Serial.println("Transmitting: " + recData);
    MySerial.println(recData);




}
void receiver(){
    delay(1000);
    if(MySerial.available() > 0) {
            recData = "";
            while(MySerial.available()) {
                c = MySerial.read();
                recData += c;
            }
            filter_string();
            Serial.println("SS Data received = " + recData);
        }
  

}


void check_sender(String data){
	receiver();
  	if(recData == data) transmitting("OOOOOOOOOOOO");
  	else transmitting(data);
  
}


void check_receiver(){
	transmitting(recData);
  delay(4000);
  	receiver();
  for(int i = 0; i<8; i++){
  
    if(recData[i] == 'O'){ 
      
      
      Serial.println("RECEBIMENTO OK");
      }
    else receiver();
  }
}
// FUNÇÕES GERAIS   

bool jogar(){
  Serial.print(key);
  switch(key){
   
        case '1':
            display1.setPixelColor(0, display1.Color(rl, gl, bl));
        if(game[0][0] == 0) {game[0][0] = id; return true;}
            return false;
        
            break;
        case '2':
            display1.setPixelColor(1, display1.Color(rl, gl, bl));
        
            if(game[0][0] == 0) {game[0][0] = id; return true;}
            return false;
            break;
        
        case '3':
            display1.setPixelColor(2, display1.Color(rl, gl, bl));
            if(game[0][1] == 0) {game[0][0] = id; return true;}
            return false;
            break;
        case '4':
            display1.setPixelColor(3, display1.Color(rl, gl, bl));
            if(game[0][2] == 0) {game[0][1] = id; return true;}
            return false;
            break;
        case '5':
            display1.setPixelColor(4, display1.Color(rl, gl, bl));
            if(game[1][1] == 0) {game[1][1] = id; return true;}
            return false;
            break;
        case '6':
            display1.setPixelColor(5, display1.Color(rl, gl, bl));
            if(game[0][2] == 0) {game[0][1] = id; return true;}
            break;
        case '7':
            display1.setPixelColor(6, display1.Color(rl, gl, bl));
            if(game[2][0] == 0) {game[2][0] = id; return true;}
            return false;
            break;
        case '8':
            display1.setPixelColor(7, display1.Color(rl, gl, bl));
            if(game[2][1] == 0) {game[2][1] = id; return true;}
            return false;
            break;
        case '9':
            display1.setPixelColor(8, display1.Color(rl, gl, bl));
            if(game[0][2] == 0) {game[0][1] = id; return true;}
            return false;
            break;
    
    	
  }
}



void switch_vez(){
  if(id == 1) STATE = VEZ2;
  else STATE = VEZ1;
}


int verifica_linhas(int linha){
    int sequences = 0;
    Serial.println("LINHAS");
    for(int i = 0; i<3;i++){
        Serial.print(linha);
        Serial.print(" | ");
        Serial.println(i);
        if(game[linha][i] == id) sequences++;
        
    }
    Serial.println(sequences);
    return sequences;
}

int verifica_colunas(int coluna){
    int sequences = 0;
    Serial.println("COLUNAS");
    for(int i = 0; i<3;i++){
        Serial.print(i);
        Serial.print(" | ");
        Serial.println(coluna);
        if(game[i][coluna] == id) sequences++;
        
    }
    Serial.println(sequences);
    return sequences;
}

int verifica_diagonal_esquerda_direita(){
    int sequences = 0;
    int coluna = 0;
    int linha = 0;
    Serial.println("DED");
    while(linha != 3){
        Serial.print(linha);
        Serial.print(" | ");
        Serial.println(coluna);
        if(game[linha][coluna] == id) sequences++;
        
        coluna++;
        linha++;
    }
            
    Serial.println(sequences);
    return sequences;


}

int verifica_diagonal_direita_esquerda(){
    int sequences = 0;
    int coluna = 2;
    int linha = 0;
    
    Serial.println("DDE");
    while(linha != 3){

        Serial.print(linha);
        Serial.print(" | ");
        Serial.println(coluna);

        if(game[linha][coluna] == id) sequences++;
        coluna--;
        linha++;
    }
    Serial.print("SEQUENCIA: ");
    Serial.println(sequences);
    return sequences;
}


bool vitoria(){
    Serial.println("VITORIA VALIDADO");
    bool resultado = false;
    for(int i=0; i<3;i++){
        if (verifica_linhas(i) == 3 
        || verifica_colunas(i) == 3) resultado = true;
    }
    if( verifica_diagonal_esquerda_direita() == 3
    || verifica_diagonal_direita_esquerda() == 3) resultado = true;
    
    
    return resultado;
}


bool empate(){

  int column = 0; 
  int row = 0;
  bool stop = false;
  bool result = true;
  Serial.println("----EMPATE-----");
  while(!stop){
    column = 0;
    
  	while(!stop){
  	Serial.print(row);
    Serial.print(" | ");
    Serial.println(column);
      if(game[row][column] == 0){
      
      	stop = true;
        result = false;
      }
      column++;
      if(column==3) break;
  
  	}
    row++;
    if(row == 3) break;
  }

 return result;
}


void resetar_partida(){
    if(key != NO_KEY){
        for(int i = 0; i<10;i++){
            display1.setPixelColor(i, display1.Color(0, 0, 0));
        }
        for(int i = 0; i<3; i++){
            for(int j=0;j<3;j++){
                game[i][j] = 0;
            }
        }
    }
}


// STATE MACHINE FUNCTIONS

void ocioso(){

    if(key != NO_KEY) STATE = INICIO;
    delay(500);

}


void inicio(){

    if(id == 1){ 

        int start = random(1,2);
        if (start == 1) STATE = VEZ1;
        else STATE = VEZ2; 

    }else{
        // wait receive state
    }
}


void vez1(){

    if(id == 1){
            
        if(key != NO_KEY){
                if(jogar()){
                
                    if(vitoria()){ STATE = VITORIA1; }
                    else if(empate()) STATE = EMPATE;
                    else switch_vez();

                }else Serial.println("JOGADA INVALIDA");
            }
        
        } 
        delay(500);
}


void vitoria1(){
    Serial.println("JOGADOR 1 VENCEU");
    Serial.println("PRECIONE QUALQUER TECLA PARA RESETA");
    if(key != NO_KEY) resetar_partida();

}


void vez2(){
    if(id == 2){ 
        key = keypad.getKey();
            if(key != NO_KEY){
                jogar();
                if(vitoria()) STATE = VITORIA2;
                else if(empate()) STATE = EMPATE;
                else switch_vez();	
        }
    }
}


void vitoria2(){
    Serial.println("JOGADOR 1 VENCEU");
    Serial.println("PRECIONE QUALQUER TECLA PARA RESETA");
   	resetar_partida();

}

void empate_state(){
    Serial.println("PARTIDA EMPATADA");
    Serial.println("PRECIONE QUALQUER TECLA PARA RESETA");
   	resetar_partida(); 
    STATE = OCIOSO;

}
void brain(){
  
    switch(STATE){
    
        case OCIOSO:
            ocioso();
            break;
        case INICIO:
            inicio();
            break;
        case VEZ1:
            vez1();
            break;
        case VITORIA1:
            vitoria1();
            break;
        case VEZ2:
            vez2();
            break;
        case VITORIA2:
            vitoria2();
            break;
        case EMPATE:
            empate_state();
            break;
    }
  

}
  
  
  
  
void setup(){
    Serial.begin(9600);
    pinMode(TxPin, OUTPUT);
    pinMode(RxPin, INPUT);

    MySerial.begin(9600);
    Serial.begin(9600);
    display1.begin();
}

void loop(){

    Serial.print("ESTADO: ");
    Serial.println(STATE);
    key = keypad.getKey();
    
    brain();
    display1.show();

    delay(500);
  
}