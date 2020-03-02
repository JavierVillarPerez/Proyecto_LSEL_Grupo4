/*
 ============================================================================
 Name        : fsm.h
 Author      : Undefined
 Version     :
 Copyright   : Your copyright notice
 Description : fsm template, Ansi-style
 ============================================================================
 */
#ifndef FSM_H

#define FSM_H



typedef struct fsm_t fsm_t;


//Function pointer int (*pf)(int): Puntero que guarda la direccion de una funcion y esa funcion puede ser invocada por una function pointer en un programa => return tipe (declaration name) (argument list)


typedef int (*fsm_input_func_t) (fsm_t*); //fsm_input_func_t es un puntero a una funcion que coge un argumento tipo fsm_t* y devuelve un int.

typedef void (*fsm_output_func_t) (fsm_t*);



typedef struct fsm_trans_t {

  int orig_state;

  fsm_input_func_t in; //Apunta a una funcion tal que int funcion(fsm_t*).

  int dest_state;

  fsm_output_func_t out;

} fsm_trans_t;



struct fsm_t {

  int current_state;

  fsm_trans_t* tt;

};



fsm_t* fsm_new (fsm_trans_t* tt);

void fsm_init (fsm_t* this, fsm_trans_t* tt);

void fsm_fire (fsm_t* this);



#endif
