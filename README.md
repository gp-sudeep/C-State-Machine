One design challenge that most embedded programmers would have come across would be to effectively respond to events. Responses to events would be a decision based on the history of events and the current event. The time tested approach to create a design capable of effectively tackling this design problem are state machines. State machines are way of visualizing a event-response problem. Visualizing a problem makes it easy to translate them to programs. There are multiple ways in which a state machine can be implemented in 'C'. I would be discussing an approach that I have used for quite some time and has proven to be apt for most problems.
Lets start with an example where we would be designing a controller for an elevator. The elevator can be at one of two floors: Ground or First. There is one button that controls the elevator, and it has two values: Up or Down. Also, there are two lights in the elevator that indicate the current floor: Red for Ground, and Green for First. At each time step, the controller checks the current floor and current input, changes floors and lights in the obvious way. The first step towards implementing a state machine is to represent the state machine using
a state transition diagram. The state transition diagram represents the different states and the events that would result in transition between states. Following is the state machine for the elevator problem:


![state_diagram](http://sudeepchandrasekaran.com/media/Capture_0H5O0X8.PNG)



The state transition diagram doesn't need much of explanation. Following are the states present in the state machine:

1. first floor
2. ground floor

Define a enum that with the respective states.


```
typedef enum state {   
  FIRST_FLOOR = 0,  
  GROUND_FLOOR  
} state;
```



Following are the events present in the state machine:

1. up button press
2. down button press


Define the events in an enum:


```
typedef enum event {
  UP = 0,
  DOWN
} event;
```

Now the next step is to code the relation between events and states. This is done by using a table called as state transition table. The program looks at this table every time an event occurs and decided which state it need to move to next. Following information is contained in every row of the table:

```
typedef struct sm_matrix_entry {
state current_state;
event evt;
state next_state;
} sm_matrix_entry;
```

The total number of rows in the table is equal to the the total number of state transitions possible. Below is the state transition table for the elevator problem :

```
sm_matrix_entry transition_table[] = {
  {GROUND_FLOOR, DOWN, GROUND_FLOOR},
  {GROUND_FLOOR, UP  , FIRST_FLOOR },
  {FIRST_FLOOR , UP  , FIRST_FLOOR },
  {FIRST_FLOOR , DOWN, GROUND_FLOOR}
};
```

We are almost done, we need to define what action needs to be done in each state. The action that needs to be done are defined in state functions The number of state functions are equal to the number of states present in the system. The state functions in our case are simple as below :

```
void state_floor_1() {
  printf("in first floor. GREEN ON\n");
}

void state_floor_g() {
  printf("in ground floor. RED ON\n");
}
```

Pointers for each of the state functions are stored in a table to enable easy look up.

```
state_response state_funcs[] = {
state_floor_1,
state_floor_g
};
```

The last step in implementing the state machine is to have a function that will monitor the events and decide which state to transition to.

```
void update_state() {
   uint8_t i = 0;
   static state present_state = GROUND_FLOOR;
   for(i = 0;i< (sizeof(transition_table)/sizeof(sm_matrix_entry)) ;i++)
   {
     if( (present_state == transition_table[i].current_state) &&
           (present_event == transition_table[i].evt) )
     {
         present_state = transition_table[i].next_state;
         state_funcs[present_state]();
         break;
     }
   }
}
```