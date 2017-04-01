# encrypt-messages
Small utility that encrypts user messages and prints the ciphertext in C

## Compilation
To compile fire up a terminal and type
* gcc main.c -o main
* ./main

## Sample output
Valid charecter for messages are:
<space> A B C D E F G H I J K L M N O P Q R S T U V W X Y Z . , : ? ! 
Enter message to crypt: HELLO MALAKA

Message is: HELLO MALAKA

Message length to encrypt: 12
Decical represantetion
[8] [5] [12] [12] [15] [0] [13] [1] [12] [1] [11] [1] 
[01000][00101][01100][01100][01111][00000][01101][00001][01100][00001][01011][00001]
[01000001][01011000][11000111][10000001][10100001][01100000][01010110][00010000]
[65] [88] [199] [129] [161] [96] [86] [16] 
Decoding message: 
HELLO MALAKA
press enter to exit!

