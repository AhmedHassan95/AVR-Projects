Sleep Mode:

This Project is to run the MC in the sleep Mode (Idle Mode).
 
  	Design Considerations:
 
  	- MCU : ATmega 16 (F_CPU = 1 MHZ).
 	- LCD (20 * 2) connected to PORTC to display messages.
 	- SW connected to pin 2 in PORTB.
 	- Pin 1 in PORTB used for testing that the MCU enter correctly the sleep
	  mode or not.
 
     The sequence of the Program:
 
        - MCU Initialization then, MCU enters the sleep mode (Idle).
        - When the button pressed (Interrupt occurred), MCU wakes up,
          serves the interrupt, and increments the numbers of presses.
        - Also, set pin 1 in PORTB and clear it after the interrupt.
        - Then when the next round came MCU enters the sleep mode again.