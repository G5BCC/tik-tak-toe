#include <Adafruit_NeoPixel.h>
#include <Keypad.h>

char key;
int display_pin = 13;
int length_game = 9;
int teste_pixel = 0;

int id = 1;

int rl = 255; int gl = 0; int bl = 0; // cor local
int ro = 0; int go = 255; int bo = 0; // cor adversario

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



Adafruit_NeoPixel display1 = Adafruit_NeoPixel(length_game, display_pin, NEO_GRB + NEO_KHZ800);

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


byte rowPins[3] = {9, 8, 7}; //connect to the row pinouts of the keypad
byte colPins[3] = {6, 5, 4}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, 3, 3 );
void jogar(){
	
  
  Serial.print(key);
  switch(key){
   
  	case '1':
    	display1.setPixelColor(0, display1.Color(rl, gl, bl));
    
    	keys[0][0] = id;
    	break;
    case '2':
    	display1.setPixelColor(1, display1.Color(rl, gl, bl));
    
    	keys[0][1] = id;
    	break;
    
    case '3':
    	display1.setPixelColor(2, display1.Color(rl, gl, bl));
    	keys[0][2] = id;
    	break;
    case '4':
    	display1.setPixelColor(3, display1.Color(rl, gl, bl));
    	keys[1][0] = id;
    	break;
    case '5':
    	display1.setPixelColor(4, display1.Color(rl, gl, bl));
    	keys[1][1] = id;
    	break;
    case '6':
    	display1.setPixelColor(5, display1.Color(rl, gl, bl));
    	keys[1][2] = id;
    	break;
    case '7':
    	display1.setPixelColor(6, display1.Color(rl, gl, bl));
    	keys[2][0] = id;
    	break;
    case '8':
    	display1.setPixelColor(7, display1.Color(rl, gl, bl));
    	keys[2][1] = id;
    	break;
    case '9':
    	display1.setPixelColor(8, display1.Color(rl, gl, bl));
    	keys[2][2] = id;
    	break;
    	
  }
}



void switch_vez(){

  if(id == 1) STATE = VEZ2;
  else STATE = VEZ1;
 
}


int verifica_linhas(int linha){
	int sequences = 0;
  for(int i = 0; i<3;i++){
  	
    if(game[linha][i] == id){
    	sequences++;
    }
  }
  return sequences;
}

int verifica_colunas(int coluna){
	int sequences = 0;
  for(int i = 0; i<3;i++){
  	
    if(game[i][coluna] == id){
    	sequences++;
    }
  }
  return sequences;
}

int verifica_diagonal_esquerda_direita(){
	int sequences = 0;
  	int coluna = 0;
  	int linha = 0;
  while(linha != 3){
    if(game[linha][coluna] == id){
    
    	sequences++;
    }
    
    coluna++;
    linha++;
  }
 		
  
  return sequences;


}

int verifica_diagonal_direita_esquerda(){
	int sequences = 0;
  	int coluna = 2;
  	int linha = 0;
  while(linha != 3){
    if(game[linha][coluna] == id){
    
    	sequences++;
    }
    
    coluna--;
    linha++;
  }
 		
  
  return sequences;
}


bool vitoria(){

  for(int i=0; i<3;i++){
	
    if (verifica_linhas(i) == 3 || verifica_colunas(i) == 3){
      	return true;
    }
      
  }
  if( verifica_diagonal_esquerda_direita() == 3|| verifica_diagonal_direita_esquerda() == 3){
    return true;
  }
  return false;
}


bool empate(){


  for(int i =0; i<3;i++){
    for(int j = 0; j<3; j++){
      if (game[i][j] == 0){
        return false;
      }
    }
  }
  return true;
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



void brain(){
  
  switch(STATE){
  
  case OCIOSO:
    delay(500); break;
  case INICIO:
    if(id == 1){ 
    	int start = random(1,2);
    	if (start == 1) STATE = VEZ1;
      	else STATE = VEZ2; 
    }
    else{
    // wait receive state
    }
    break;
    
    
  	case VEZ1:
    if(id == 1){
    	 key = keypad.getKey();
      if(key != NO_KEY){
         jogar();
        if(vitoria()) STATE = VITORIA1;
        else if(empate()){
        	STATE = EMPATE;	
        }
        
        	
      }
      
    } 
    break;
    
  
  	case VITORIA1:
    	Serial.println("JOGADOR 1 VENCEU");
    
    
    	Serial.println("PRECIONE QUALQUER TECLA PARA RESETA");
   		 if(key != NO_KEY) resetar_partida();
         
    
    break;
    	
  	case VEZ2:
    	if(id == 2){
    	 key = keypad.getKey();
     	 if(key != NO_KEY){
         jogar();
        if(vitoria()) STATE = VITORIA1;
        else if(empate()){
        	STATE = EMPATE;	
        }
        
        	
      }
          
          break;
      
  	case VITORIA2:
          Serial.println("JOGADOR 1 VENCEU");
    
    
    	Serial.println("PRECIONE QUALQUER TECLA PARA RESETA");
   		resetar_partida();
          break;
    }
          
    case EMPATE:
          Serial.println("PARTIDA EMPATADA");
    
    
    	Serial.println("PRECIONE QUALQUER TECLA PARA RESETA");
   		resetar_partida(); 
    	STATE = OCIOSO;
    	break;
  }
  

}
  
void setup(){
  Serial.begin(9600);
 display1.begin();
}

void loop()
{
  key = keypad.getKey();
 if(key != NO_KEY) jogar();
  
  display1.show();
  delay(500);
}