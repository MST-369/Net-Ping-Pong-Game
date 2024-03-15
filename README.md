# Net-Ping-pong-Game


***********HOW TO START THE GAME?**********************

1. Compile the serverp.c in one terminal and client.c in another terminal.
2. After the successful connection between server and client, a prompt will come on screen asking for name.
3. Game will start and first pass will be on server side.


**************GAME CONTROLS****************************

1. 'W' , 'S' keys are controls to control the paddle.


*************POINTS TO MENTION*************************

1. When a player misses the pong the system generates a random serve(from anywhere) in the other players court.
2. Atlast, system only declares the winner and his/her score on both players screen.
3. Press 'q' on terminals to end the game.
4. In the instructions, They mentioned to use poll or select system call to control the paddle when the ball is in other court.
   But instead of those system calls, implemented the multiple thread concept so that the paddle moves irrespective of the ball presence.
   
*******************************************************


NOTE: please do press either 'W' or 'S' for 1 sec to get complete paddle on screen.
please refer the video attachment for furthur clarification.



![Picture1](https://github.com/MST-369/Net-Ping-pong-Game/assets/145525421/5e1eb551-f4d7-49f5-9a01-6950a10cc846)
