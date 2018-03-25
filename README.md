# C Console Calculator
It's a console calculator written in C for self-learning purposes.

## How to run
You can immediately check this program out by visiting [OnlineGDB](https://onlinegdb.com/S1YYn1S5G). Simply follow the link and then click "Run" button at the top to make the service to compile and run the program.

If you prefer to deal with binaries directly, you may download the executable file ```calc.exe``` and launch it via your favourite shell (say, Bash):
```
./calc.exe
```
It's also worth to note that you can launch the program with ```h``` (that means "help") argument to read a manual (which is essentially a brief version of this README.md file):
```
./calc.exe h
```
To do so, download directories ```bin``` and ```doc``` and place them side by side. Then you'll need to run the executable file from ```bin``` directory.

## How to use. The basics
Please note that each value you want to feed the program, except the ```%``` symbol, must be put on a separate line (in other words, hit Enter every time you put the value to the command prompt), like that:
<pre>
<b>Input:</b>
6
+
50%
r
</pre>
<pre>
<b>Output (here and below, there's a brief version of output):</b>
[6.000000 + 3.000000 = 9.000000]
[&radic;9.000000 = 3.000000]
</pre>
If you'd like to close the program, just type <code>quit</code>. The subtotal value will be shown, and then the program will be closed.

## How to use. In more details
The values are needed to be placed on the separate lines. These values can be: 
* *any numbers* including *percentage* like ```10%```;
* *binary operators* (addition ```+```, subtraction ```-```, multiplication ```*```, division ```/```, exponentiation ```^```);
* *square root* ```r``` (there's it's kinda a postfix operator here which returns the square root of the subtotal value).
* *commands* such as ```=``` (get the subtotal value), ```quit``` (close the program), ```c``` (reset all the calculations), and operations with the memory of the calculator: ```m+```, ```m-```, ```mc```, ```mr```, ```mca``` (i.e., "memory clear all" - see details below), ```mpa``` ("memory print all"); there's a separate chapter there describing how to use all these memory commands.

This calculator is able to save the subtotal, so if you've misspelled while entering a value, feel free to overwrite it. The following example is ```(4 + 2) ^ 2``` expression:
<pre>
<b>Input:</b>
4
-
+
50
50%
^
2
</pre>
<pre>
<b>Output:</b>
[4.000000 + 50.000000 = 54.000000] // 4 + 50
[4.000000 + 2.000000 = 6.000000] // oh, I meant not 50 but 50%
[6.000000 ^ 2 = 36.000000]
</pre>
This feature also provides safety for calculations considering mathematically incorrect results, like in the following example (click to expand).
<details>
<summary>Example of bad results</summary>
<pre>
<b>Input:</b>
10
/
0
2
</pre>
<pre>
<b>Output:</b>
***
[10.000000 / 0.000000] Invalid operation!
Subtotal is: 10.000000
~~~
Invalid input: "0". Input's been withdrawn. Please:
1) enter different number to finish this expression, or
2) enter operator to make different expression with subtotal shown above.
***
[10.000000 / 2.000000 = 5.000000]
Subtotal is: 5.000000
***
</pre>
</details>
The program also ignores input which's meaningless in the given context but this is not very exciting to see the examples of such a behavior. However, let's mention that the synthax like ```1 + 2 * 3 / 2``` (all in one line) is allowable technically. But this is not a designed feature but only a side effect. Such the syntax has no parentheses support (so there's no correct order of arithmetic operations), besides, it sometimes shows a weird behavior. The expression shown above returns:
[1.000000 + 2.000000 = 3.000000]
[3.000000 * 3.000000 = 9.000000]
[9.000000 / 2.000000 = 4.500000]

## How to use the memory functions
This calculator supports a number of memory cells, up to 20 cells exactly. Here's the more detailed list of the commands:
* ```m+``` or ```m+#``` (where ```#``` is the index of the certain cell: ```[1; 20]```): add the current subtotal to the value of #th cell. Please note that if the index hasn't been specified explicitly, it would be considered 1 automatically, so commands ```m+``` and ```m+1``` are identical.
* ```m-``` or ```m-#```: substract the subtotal from the value of #th cell.
* ```mc``` or ```mc#``` ("memory clear"): set the value of #th cell to 0.
* ```mr``` or ```mr#``` ("memory recall"): recall the value from #th cell.
* ```mca``` ("memory clear all"): set the values of all the cells to 0.
* ```mpa``` ("memory print all"): print the values of the cells whose contents aren't 0.
Here's an example of using those (click to expand).
<details>
<summary>Using memory functions</summary>
<pre>
<b>Input:</b>
2
m+
m+2
m+2
mpa
^
mr2
/
mr
mca
=
</pre>
<pre>
<b>Output:</b>
***
Subtotal is: 2.000000
***
Memory cell #1 has: 2.000000 // m+
***
Memory cell #2 has: 2.000000 // m+2
***
Memory cell #2 has: 4.000000 // m+2
***
Memory cell #1 has: 2.000000 // mpa
Memory cell #2 has: 4.000000
***
[2.000000 ^ 4 = 16.000000] // mr2
Subtotal is: 16.000000 / Memory cell #1 has: 2.000000
***
[16.000000 / 2.000000 = 8.000000] // mr
Subtotal is: 8.000000 / Memory cell #1 has: 2.000000
***
All memory cells have 0. // mca
***
Subtotal is: 8.000000 // =
***
</pre>
</details>