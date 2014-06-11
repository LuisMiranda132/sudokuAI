/**
 * Coder: Modulo que implementa el codificador y el decodificador
 *
**/

#include <stdio.h>
#include <vector>
#include <string>
#include <cstdarg>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#define NUM 9
#define BOARD 81

/*******************************************************************************

                             FUNCIONES AUXILIARES

*******************************************************************************/

/**
 * addClause: Agrega una clausula nueva a un vector que representa las clausulas
 *            del SATLIB a generar
 * @param r: Vector de clausulas
 * @param c: Numero actual de clausulas(se modifica)
 * @param i: Cantidad de terminos a agregar
 * @param ...: Serie de integers representando los terminos a agregar a la
 *             clausula
**/
void addClause(std::vector<int>& r,int& c, int i, ...){
  va_list arguments;
  va_start(arguments,i);
  for(int x=0;x<i;++x){
    r.push_back(va_arg(arguments,int));
  }
  va_end(arguments);

  r.push_back(0);
  ++c;
}


/**
 * addClauseStep: Agrega un pedazo de clausula al vector, sin aumentar el numero
 *                de clausulas agregadas
 * @param r: Vector de clausulas
 * @param i: Numero de terminos a agregar
 * @param ...: Serie de integers representando los terminos a agregar
**/
void addClauseStep(std::vector<int>& r, int i, ...){
  va_list arguments;
  va_start(arguments,i);
  for(int x=0;x<i;++x){
    r.push_back(va_arg(arguments,int));
  }
  va_end(arguments);
}

/**
 * boardIndex: Retorna el numero de termino que representa el valor j en el
 *             index i en el vector de casillas del sudoku
 * @param i: Indice en el vector de casillas
 * @param j: Valor en el indice dado en el vector de casillas
 * @return Numero de termino
**/
int boardIndex(int i,int j){
  return i*NUM+j;
}

/**
 * addRowColumnClause: Agrega clausulas de columnas y filas
 * @param r: Vector de clausulas
 * @param c: Numero actual de clausulas
 * @param i: Index actual en la representacion de sudoku
 * @param curRowEnd: Final actual de fila
 * @param b: Valor en casilla actual. Valor 0 representa vacio
**/
void addRowColumnClause(std::vector<int>& r, int&c, int i, int curRowEnd, int b){
  //Si la casilla original estaba vacia, se agregan clausulas de manera
  //incremental para cada posible valor en la casilla
  if(!b){
    for(int j=1;j<NUM+1;++j){
      //No se pueden repetir valores en la misma casilla
      for(int k=j+1;k<NUM+1;++k){
        addClause(r,c,2,-boardIndex(i,j),-boardIndex(i,k));
      }
      //No pueden haber valores repetidos en la misma fila
      for(int k=1;(i + k) <= curRowEnd;++k){ 
        addClause(r,c,2,-boardIndex(i,j),-boardIndex(i+k,j));
      }
      //No pueden haber valores repetidos en la misma columna
      for(int k=1;(i+NUM*k) < BOARD;++k){
        addClause(r,c,2,-boardIndex(i,j),-boardIndex((i+NUM*k),j));
      }
    }
  //Si la casilla no esta vacia, se agregan clausulas que previenen que el mismo
  //valor este en la misma fila o columna
  }else{
    //No puede existir otra casilla con el mismo valor que la original en la
    //misma fila
    for(int k=1;i+k <= curRowEnd;++k){
      addClause(r,c,1,-boardIndex(i+k,b));
    }
    //No puede existir otra casilla con el mismo valor que la original en la
    //misma columna
    for(int k=1;(i+NUM*k) < BOARD;++k){
      addClause(r,c,1,-boardIndex((i+NUM*k),b));
    }
  }
}

/**
 * addSquareClause: Agrega clausulas de cuadrado
 * @param r: Vector de clausulas
 * @param c: Numero actual de clausulas
 * @param i: Index actual en la representacion de sudoku
 * @param mini: Valor que especifica si estamos en la fila 0, 1 o 2 de un
 *              recuadro
 * @param mini2: Valor que especifica si estamos en la columna 0,1 o 2 de un
 *               recuadro
 * @param b: Valor en casilla actual. Valor 0 representa vacio
**/
void addSquareClause(std::vector<int>& r,int& c, int i, int mini, int mini2,int b){
  //Valores de casillas para los que se colocaran nuevas clausulas
  int c1,c2,c3,c4;
  
  //Los siguientes switchs determinan el valor de c1,c2,c3 y c4
  switch(mini){
    case 0:
      switch(mini2){
        case 0:
          c1 = i + NUM + 1; c2 = c1 + 1; c3 = i + 2*NUM + 1; c4 = c3 + 1;
          break;
        case 1:
          c1 = i + NUM - 1; c2 = c1 + 2; c3 = i + 2*NUM - 1; c4 = c3 + 2;
          break;
        default:
          c1 = i + NUM - 2; c2 = c1 + 1; c3 = i + 2*NUM - 2; c4 = c3 + 1;
      }
      break;
    case 1:
      switch(mini2){
        case 0:
          c1 = i - NUM + 1; c2 = c1 + 1; c3 = i + NUM + 1; c4 = c3 + 1;
          break;
        case 1:
          c1 = i - NUM - 1; c2 = c1 + 2; c3 = i + NUM -1; c4 = c3 + 2;
          break;
        default:
          c1 = i - NUM - 2; c2 = c1 + 1; c3 = i + NUM - 2; c4 = c3 + 1;
      }
      break;
    default:
      switch(mini2){
        case 0:
          c1 = i - 2*NUM + 1; c2 = c1 + 1; c3 = i - NUM + 1; c4 = c3 + 1;
          break;
        case 1:
          c1 = i - 2*NUM - 1; c2 = c1 + 2; c3 = i - NUM - 1; c4 = c3 + 2;
          break;
        default:
          c1 = i - 2*NUM -2; c2 = c1 + 1; c3 = i - NUM - 2; c4 = c3 + 1;
      }
  }

  //Finalmente se agregan las clausulas, dependiendo de si la casilla original
  //estaba vacia o no
  if(!b){
    for(int j=1;j<NUM+1;++j){
      addClause(r,c,2,-boardIndex(i,j),-boardIndex(c1,j));
      addClause(r,c,2,-boardIndex(i,j),-boardIndex(c2,j));
      addClause(r,c,2,-boardIndex(i,j),-boardIndex(c3,j));
      addClause(r,c,2,-boardIndex(i,j),-boardIndex(c4,j));
    }
  }else{
    addClause(r,c,1,-boardIndex(c1,b));
    addClause(r,c,1,-boardIndex(c2,b));
    addClause(r,c,1,-boardIndex(c3,b));
    addClause(r,c,1,-boardIndex(c4,b));
  }
}    
        

/**
 * printResults: Retorna un string con la codificacion SATLIB de un sudoku
 * @param vars: Numero de terminos
 * @param c: Numero de clausulas
 * @param r: Vector de clausulas
 * @return Representacion del SATLIB del problema
**/
std::string printResults(int vars, int c, std::vector<int>& r){
  std::string res = "p cnf ";
  res.append(std::to_string(vars) + " " + std::to_string(c) + "\n");
  for(int i=0;i<r.size();++i){
    if(r[i] == 0){ 
      res.append("0\n");
    }else{
      res.append(std::to_string(r[i]) + " ");
    }
  }
  return res;
}

/******************************************************************************
        
                               CODER Y DECODER

******************************************************************************/


/**
 * code: codifica una representacion de sudoku en SATLIB
 * @param board: Vector de caracteres que representa a la instancia de sudoku a
 *               codificar
 * @return: String que contiene la codificacion
**/
std::string code(std::string& board){
  //Vector de clausulas. Cada clausula esta separada por un 0
  std::vector<int> c;
  //Contador de clausulas
  int clauses = 0;
  //Final de fila actual. Usado para establecer clausulas
  int curRowEnd = NUM-1;
  //Codigo de minitablero
  int mini = 0;
  int mini2 = 0;

  for(int i=0;i<board.size();++i){
    //Si la casilla no es vacia, agregar una clausula donde dicha casilla
    //contiene el valor obtenido. Tambien agregar clausulas de forma que ninguna
    //casilla en la misma fila, columna o cuadrado 3X3 tenga el mismo valor que
    //la casilla original
    if(board[i] != '.'){
      addClause(c,clauses,1,boardIndex(i,board[i] - '0'));
      addRowColumnClause(c,clauses,i,curRowEnd,board[i] - '0');
      addSquareClause(c,clauses,i,mini,mini2,board[i] - '0');
    //Si la casilla es vacia, se hace lo mismo que el caso anterior, excepto que
    //tambien se agrega una clausula que dice que debe haber algun valor en la
    //casilla. addRowColumClause se encarga de agregar las clausulas que
    //previenen la repeticion del mismo valor en la misma casilla
    }else{
      for(int j=1;j<NUM;++j){
        addClauseStep(c,1,boardIndex(i,j));
      }
      addClause(c,clauses,1,boardIndex(i,NUM));
      addRowColumnClause(c,clauses,i,curRowEnd,0);
      addSquareClause(c,clauses,i,mini,mini2,0);  
    }

    //Se actualizan los valores de los cuadrados y final de fila de ser
    //necesario
    mini2 = ((mini2 + 1) % 3 == 0) ? 0 : mini2 + 1;
    if(i == curRowEnd){
      curRowEnd = curRowEnd + NUM;
      mini = ((mini + 1) % 3  == 0) ? 0 : mini + 1;
    }
  }
  return printResults(NUM*BOARD,clauses,c);  
}  

/**
 * decode: Convierte un resultado de minisat en una representacion resuelta de
 *         sudoku y la imprime en pantalla    
 * @param res: String que contiene los resultados de minisat
**/ 
void decode(std::string& res){
  printf("Solution: ");

  //Se va leyendo de el output de minisat hasta leer un 0
  std::stringstream stream(res);
  while(1){
    int n;
    stream >> n;
    //Si el valor es positivo, imprimir ese valor, pues es parte de la solucion
    if(n > 0) printf("%d",(n % NUM == 0) ? NUM : n % NUM);
    if(n == 0) break;
  }
  printf("\n");
}

