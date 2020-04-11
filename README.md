## Game Description

Our game, **solitudo** , is a variant of a game named HITORI. HITORI means one person in Japanese, is an individual puzzle game commonly with square or rectangular grid. Our team intend to develop the game into hexagonal grid.

#### Game Rule
The player is asked to whiten or blacken the cells based on the following basic rules:
* No two cells should be blackened in adjacent.
* No two cells with the same number should be whitened in the same row/column.
* All remaining cells which blocks the white cell in connecting each other should be whitened.

#### Solving Technique
Solving Techniques, search under the same row/column the following patterns:
* All cells in adjacent with a black cell must be white
* If there are two consecutive cells will same number in the same row/column, all other cell with this number in the same row/column should be black
* If there are sandwich triple (three in a row), the middle one must be white and other two must be black, and cells around two black cell must be white
* If there are sandwich pair (e.g. ABA) in a row, the middle one must be white
* If four same number is situated in the conern, the black cell must go from the corner.

## Features and functions
Our version of game will be divided into three modes. 
* Tutorial mode, with steps guiding the player through the game. 
* Practical mode, the player will be prompted if any wrong blackened cell is found. 
* Normal mode, with undo/redo features to allow change of answers, counting the time of one game is also available.


#### Features and Function to be implemented vis-à-vis to coding requirements
1. Generation of random game sets or events.
The game board will be generated in the following way:
	* constructs a Latin square
	* adding random black cell without breaking the rule
	* adding random number to black cell

1. Data Structures for storing game status
We will use create a struct “Cell” to store the status of each cell, it will contain the basic information of the cell, e.g. number & status (unmarked / whiten / blacken)
We will use vector of vector of “Cell” to store a board

1. Dynamic memory management
Undo / Redo function will be implemented which “Redo” mean “un-undo”.
An object of struct “action” will be dynamically created for each user action (excluding the “undo” / “redo” action.
All “action” object will be stored in the undo stack. Action object will be popped from the undo stack and pushed to the redo stack when user perform the “undo” action and vise versa for “redo” action.

1. File input/output
Import/Export function will be implemented. User can export the game status (including the action stacks) into a file. User can export the game status into a file and end the program. Next time, user can start the program and import the file to continue the game.

1. Program codes in multiple files
There will be several file in this project, changes may be made if necessary. 
	* cell.cpp / cell.h - contain the struct definition of the struct “cell” and functions to manipulate the cell
	* action.cpp / action.h – contain the struct definition of the struct “action” and handle undo / redo action.
	* export.cpp / export.h – contain the code for import / export the game status
	* generator.cpp / generator.h – contain code for board generation
	* solver.cpp / solver.h – contain code for solving the board
	* main.cpp – the main program
