## Game Description

This project is for course ENGG1340 in HKU. Our game, **Solitudo** , is a implementation of a game named HITORI. HITORI, translated as “one person” in Japanese, is a one-player puzzle game originated from Nikoli Co., Ltd., commonly played in a square grid. We enhanced the game with hints, undo and redo function, load saved game capability. We designed the logo in ASCII art. To enjoy a bit of fun, we added some emoticon and  coloured playing board and instrustions.

#### Game Rule
The player will be asked to whiten or blacken the cells based on the following basic rules:
*   No two cells should be blackened in adjacent, either horizontally or vertically.
*   No two cells with the same number should be whitened in the same row/column.
*   All whitened cells should be connected horizontally or vertically with one another.

#### Solving Technique
We advise the player to solve the board using the following hints:
*   Whiten all cells in adjacent with a black cell horizontally and vertically.
*   Search for two consecutive cells will same number in the same row/column, and then blacken all other cells with this number in the same row/column.
*   Whiten all cells which blocks the white cells in connecting one another if necessary.
*   If there are sandwich triple (three in a row), the middle one must be whitened and other two must be blackened.
*   If there are sandwich pair (e.g. ABA) in a row, the middle one must be whitened.
*   If four same number is situated in the corner, the black cell must go from the corner.

## Features and functions
Our version of game will be divided into three modes. 
*   Tutorial mode, with steps guiding the player through the game. 
*   Practice mode, the player will be prompted if any wrongly marked cell is found. 
*   Normal mode, with undo/redo features to allow change of answers, counting the time of one game is also available.


#### Features and Function implemented vis-à-vis to coding requirements
1.  Generation of random game sets or events.
    *   Constructing a Latin square and hence the basic of game board.
    *   Mark a random amount of cells as black without breaking the rule.
    *   Change the number of black cell to create duplcates.

2.  Data Structures for storing game status
    *   A struct “Cell” will be uesd to store the status of each cell, containing the basic information of the cell, e.g. number & status (unmarked / whiten / blacken)
    *   A vector of vector of “Cell” will be used to store a board

3.  Dynamic memory management
    *   Undo / Redo function were implemented, with “Redo” means “un-undo”.
    *   An object of struct “action” will be dynamically created for each user action (excluding the “undo” / “redo” action.)
    *   All “action” object will be stored in the undo stack. Action object will be popped from the undo stack and pushed to the redo stack when user perform the “undo” action and vise versa for “redo” action.
    *	Memory of those dynamically created “action” will be deallocated when undo/redo stack rest
    

4.  File input/output
    *   Import/Export function were implemented. User can export the game status (including the action stacks) into a file. User can export the game status into a file and end the program. Next time, user can start the program and import the file to continue the game.

5.  Program codes in multiple files
    *   There will be several files in this project. Changes may be made if necessary. 
	    *   cell.cpp / cell.h - contain the struct definition of the struct “cell” and functions to manipulate the cell
	    *   action.cpp / action.h – contain the struct definition of the struct “action” and handle undo / redo action.
	    *	board.cpp / board.h - the game board generator and other utility function for checking the board
	    *   main.cpp – the main program
	    
6.   Non-standard Libraries used
	*   <termios.h> and <unistd.h> - used in the readkey() function (in main.cpp) to interact with linux terminal settings, such that we can read a key without pressing enter


## Compilation and execution
Use `make` to compile the program and use `./main` to execute the program. Please note that this program only support running on Linux platform with ANSI escape sequences supported terminal
	
