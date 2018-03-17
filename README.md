# C Console Calculator
It's a simple calculator for a shell written for self-learning purposes.

## How to run
You can immediately check this program out by visiting [OnlineGDB](https://onlinegdb.com/rJKGU0PtG). Simply follow the link and then click "Run" button at the top to make the service to compile and run the program.

If you prefer to deal with binaries directly, you may download the file called ```calc.exe``` and launch it via your favourite shell (say, Bash):
```
./calc.exe
```
It's also worth to note that you can launch the program with ```h``` (which means "help") argument to read a manual (which is a brief version of this README.md file, actually), like so:
```
./calc.exe h
```
To do so, download directories ```bin``` and ```doc``` and place them side by side. Then you'll need to run the executable file from ```bin``` directory.

## How to use. The basics
Please note that each value you want to feed the program, except the ```%``` symbol, must be put on a separate line (in other words, hit Enter every time you put the value to console), like that:
<pre>
<b>Input:</b>
6
+
50%
r
=
</pre>
<pre>
<b>Output (here and below, there's a brief version of output):</b>
[6.000000 + 3.000000 = 9.000000]
[&radic;9.000000 = 3.000000]
</pre>
If you'd like to close the program, you could just type <code>=</code> or <code>quit</code>. It would show the subtotal value and then be closed.

## How to use. In more details
The values are needed to be placed on the separate lines. These values can be: 
* *any numbers* including *percentage* like ```10%```;
* *binary operators* (addition ```+```, subtraction ```-```, multiplication ```*```, division ```/```, exponentiation ```^```);
* *square root* ```r``` (there's it's kinda a postfix operator here which returns the square root of the subtotal value).

This calculator is able to save the subtotal, so if you've misspelled while entering a value, feel free to overwrite it. The following example reflects ```(3 + 1.5) * 2``` expression:
<pre>
<b>Input:</b>
3
-
+
50
50%
*
2
=
</pre>
<pre>
<b>Output:</b>
[3.000000 + 50.000000 = 53.000000] // 3 + 50
[3.000000 + 1.500000 = 4.500000] // oh, I meant not 50 but 50%
[4.500000 * 2.000000 = 9.000000]
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
=
</pre>
<pre>
<b>Output:</b>
[10.000000 / 0.000000] Invalid operation!
Subtotal is: 10.000000
Invalid input: "0". Input's been withdrawn. Please: 1) enter different number to finish this expression, or 2) enter operator to make different expression with subtotal shown above.
[10.000000 / 2.000000 = 5.000000]
</pre>
</details>

The program also ignores input which's meaningless in the given context but this is not very exciting to see the examples of such a behavior.