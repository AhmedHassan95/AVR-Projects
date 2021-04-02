Signal Parameters Calculation:

This project is Design a system to calculate the duty Cycle of any
input signal using only Timer and Interrupt, then display the result on LCD (2 * 16).
 
  	Design Considerations:
 
  	- MCU : ATmega 16 (F_CPU = 1 MHZ).
 	- Use Timer1 Overflow Mode (NO_Prescaler).
 	- Use External Interrupt0 (INT0), to enter the input Signal into MCU.
 	- Display result on LCD (2 * 16) in PORTC.
