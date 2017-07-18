#include <stdio.h>
#include <string.h>
#include <stdint.h>


typedef enum state {
  FIRST_FLOOR = 0,
  GROUND_FLOOR
} state;


typedef enum event {
  UP = 0,
  DOWN
} event;

typedef void (*state_response)(void);

void state_floor_1() {
  printf("in first floor. GREEN ON\n");
}

void state_floor_g() {
 printf("in ground floor. RED ON\n");
}

state_response state_funcs[] = {
 state_floor_1,
 state_floor_g
};



typedef struct sm_matrix_entry {
 state current_state;
 event evt;
 state next_state;
} sm_matrix_entry;



sm_matrix_entry transition_table[] = {
  {GROUND_FLOOR, DOWN, GROUND_FLOOR},
  {GROUND_FLOOR, UP  , FIRST_FLOOR },
  {FIRST_FLOOR , UP  , FIRST_FLOOR },
  {FIRST_FLOOR , DOWN, GROUND_FLOOR}
};


state present_state = GROUND_FLOOR;
event present_event = UP;

void update_state() {
   uint8_t i = 0;
   for(i = 0;i< (sizeof(transition_table)/sizeof(sm_matrix_entry)) ;i++)
   {
     if( (present_state == transition_table[i].current_state) &&
           (present_event == transition_table[i].evt) )
     {
         state_funcs[transition_table[i].next_state]();
     }
   }
}



void main() {
  present_state = GROUND_FLOOR;
  present_event = UP;
  update_state();
  update_state();
  present_state = GROUND_FLOOR;
  present_event = DOWN;
  update_state();
}
