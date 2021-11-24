# lift-model

## Description of the program
Program lift.cpp simulates a lift responding to queries created by
pressing the required floor button. The lift moves with the speed of one floor per second.
If someone enters or exits on the current floor, the lift opens the doors for one second, 
and then moves on. The lift also has a direction indicator, which shows
which direction the lift is going to move next.

A person standing on a floor enters the lift if the lift is on this floor, the doors are open and
the direction indicator shows where this person needs to go. A person exits the lift as soon as it
stops on the target floor and opens the doors.

### Input format

The program reads input from the file input.txt. The first line should contain two numbers -
the number of floors in the building and the number of lift calls. The following lines should describe requests
in the chronological order, three numbers per line - the second when lift call button was pressed, the floor of the 
call and the destination floor (floors are numbered from one).

### Output format

The program outputs the sequence of actions of the lift to the file output.txt. Each line displays one
second in the following format:

`second` --- `the floor where the lift is located` `direction indicator` `doors open or closed`

The direction indicator is indicated by the symbol:
- "^" up
- "v" down
- "-" does not move

If the doors are closed, the line "\]\[" is displayed, and if the doors are open "\[\_\]".
