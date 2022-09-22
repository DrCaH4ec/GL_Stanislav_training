# Rock-paper-scissors game
This is the rock-paper-scissors terminal game which has been created using bash.
The game itself is located in file [game](./game).

***

## How to use
Before launching make sure the game file has the permissions for executing. 
I.e., in the terminal open the folder where [game](./game) lives and type the command:

`sudo chmod +x ./game`

Then, execute the file by typing:

`./game`

Now, you can play rock-paper-scissors!

***

## How to play
Terminal dialog will suggest you to make a choice from following:

>Please choose: rock (r) - paper (p) - scissors (s)

Please note that you should give one of the corresponding chars in lower case.
If you type something else the game will warn you about the wrong input and will
suggest you to try again.
```
Let's play the rock-paper-scissors game!
Please choose: rock (r) - paper (p) - scissors (s)
wrong input
Invalid value, please try again:
```

***

## Rules
When you picked an item (rock, paper or scissors) computer will make a random choice
and the program will compare your picks by the following rules:

* Rock beats scissors:
* Scissors cuts paper;
* Paper covers rock;
* Alike picks means tie.

In case of tie the programm will suggest you to try again:
```
Let's play the rock-paper-scissors game!
Please choose: rock (r) - paper (p) - scissors (s)
r
You have chosen rock
Machine have chosen rock
Tie, try another time:
```