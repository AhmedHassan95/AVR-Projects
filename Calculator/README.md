Simple Calculator:

This Project is to design a simple calculator to perform the simple operations.
This operations include (Addition, Subtraction, Multiplication, Division) between two operands.
					
 		Design Considerations:
 
		- MCU : ATmega 16 (F_CPU = 1 MHZ).
 		- Display result on LCD (4 * 16) in PORTD.
 		- Result should consider Negative number, Floating Point,
 		  More than one operand.
	
	
Note: "Project doesn't work correctly without these Steps"	  

As “There is no Floating Point Unit in AVR”. We can do that using a software library, 
so follow these steps to add it:

1- After create your Project -> Right Click “Choose Properties” -> C/C++ Build “Setting”
   -> AVR C Linker “Libraries” -> Choose Add Button in “Libraries (-l)” -> Then Add (printf_flt).
   
   
2- Also AVR C Linker “General” -> Then in “Other Arguments Tab” -> Then Add (-uvfprintf).
