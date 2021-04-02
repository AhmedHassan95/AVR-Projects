Security Door lock System:

This Project is to design a Door Lock Security System consists of two ECU's. The First ECU is called HMI (Human Machine Interface) 
ECU responsible for interfacing with the user through "KEYPAD + LCD" to take the user password. The Second ECU is called CONTROL ECU 
connected with "DC MOTOR" to open and close the door, Alarm System "BUZZER + LED" to alert if a stranger trying to open the door,
and "External EEPROM" to store The user password. The two ECU's communicate with each other through UART Protocol.


The First ECU is called HMI (Human Machine Interface) ECU responsible for:

 - Getting the system status from the CONTROL ECU.
 - Taking a new password from the user two times.
 - Sending the password to the CONTROL ECU to store it in the EEPROM.
 - Displaying two options to the user "+" (TO CHANGE PASSWORD), and "-" (TO OPEN DOOR).
 - Sending the user option to the CONTROL ECU.

The Second ECU is called CONTROL ECU responsible for:

 - Sending the system status to the HMI ECU.
 - Storing the new user password in the EEPROM.
 - Checking matching between the current and the old password.
 - Receiving the user option.
 - Changing the user password.
 - Opening the door by rotating the DC motor CW, closing the DC by rotating the DC MOTOR ACW.



 The sequence of the Program:

 Phase 1:
 - Read the password address in the EEPROM.
 - If it is equal to the default value that means this is the first time login.
 - If it is not equal to the default value that means this is not the first-time login.
 - Send the system status to the HMI ECU.
 - Get the system status from the CONTROL ECU.
 - If this is the first time to login to the system, take a new password two times. Check if these two passwords are identical or not.
 - If the two passwords are identical, sending the password to the Control ECU to store it in the EEPROM.
 - If the two passwords are not identical, still in this step until be identical.

 Phase 2
 - If this is not the first time to login to the system, give the user two options "+” (TO CHANGE    PASSWORD), "-” (TO OPEN DOOR).
   Receiving the user option from the CONTROL ECU.

 Phase 3:
 - User choice is (TO CHANGE PASSWORD)
 - Ask the user for the current password
 - Compare the current password with the password stored in the EEPROM
 - If it is matched, Change the password.
 - If it is unmatched, tell the user to try again (for two additional times), it is still unmatched count the Alarm system time (15 Seconds).

 Phase 4:
 - User choice is (TO OPEN DOOR)
 - Ask the user for the current password
 - Compare the current password with the password stored in the EEPROM	
 - If it is matched, open the door.
 - If it is unmatched, tell the user to try again (for two additional times), it is still unmatched count the Alarm system time (15 Seconds).

