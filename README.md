# C Console Calculator
It's a simple calculator for a shell written for self-learning purposes.

## How to run
You can just download the binary file called ```calc.exe``` and launch it via your shell (say, Bash):
```
./calc.exe
```
Also, you can launch the program with ```h``` (which means "help") argument to read a manual (which is a brief version of this README.md file, actually), like so:
```
./calc.exe h
```
To do so, download directories ```bin``` and ```doc``` (and place them side by side), then you need to run the executable file from ```bin``` directory.

## How to use
Please note that each value you want to feed the program must be put on a separate line (in other words, hit Enter every time you put the value to console).
<details>
<summary>In this way</summary>
<pre>
<b>Input:</b>
6
+
50
%
r
=
</pre>
<pre>
<b>Output:</b>
[6.000000 + 3.000000 = 9.000000]
[&radic;9.000000 = 3.000000]
Result is: 3.000000
</pre>
</details>
If you'd like to close the program, you could just type ```=```. It would show the result of calculations or, at least, the subtotal value, and then would be closed.

## Some important things to note. Examples of use.
The program handles input in accordance with its inner system of types of values. There're a number of such types:
* _number_ type means any valid number like ```1```, ```-0.25```, etc.
* _operator_ type means binary operators of addition ```+```, subtraction ```-```, multiplication ```*```, division ```/```, and exponentiation ```^```. Example: ```5 ^ 2 = 25```.
* _percent_ type means percentage ```%```. ```%``` gets the percentage of the given number. Example: ```1 + 50 % = 1.5```.
* _root_ type means square root ```r```. Example: ```9 r = 3```.

The program has some input restrictions according to these types. Here's a few examples. Say, you can't input several numbers or operators in a row. This won't be the cause of the crash or something, the program will just receive only the 1st value of the given type and will ignore the rest.
<pre>
<b>Input:</b>
3
2
1
+
10
=
</pre>
<pre>
<b>Output:</b>
Invalid input: "2". Please enter operator.
Invalid input: "1". Please enter operator.
[3.000000 + 10.000000 = 13.000000]
Result is: 13.000000
</pre>
</details>
<details>
<summary>The same is correct for operators, as well.</summary>
<pre>
<b>Input:</b>
3
+
-
10
=
</pre>
<pre>
<b>Output:</b>
Invalid input: "-". Please enter number.
[3.000000 + 10.000000 = 13.000000]
Result is: 13.000000
</pre>
</details>
However, this rule doesn't spread on a unary square root:
<pre>
[&radic;81.000000 = 9.000000]
[&radic;9.000000 = 3.000000]
Result is: 3.000000
</pre>
Also, the program ignores meaningless input.
<details>
<summary>Click to expand</summary>
<pre>
<b>Input:</b>
something
3
something
+
1
=
</pre>
<pre>
<b>Output:</b>
Invalid input: "something". Please enter something meaningful.
Invalid input: "something". Please enter something meaningful.
[3.000000 + 1.000000 = 4.000000]
Result is: 4.000000
</pre>
</details>
It's also worth to note that when you do exponentiation, the program checks if the exponent's correct, and then it can "fix" the incorrect exponent.
<details>
<summary>Click to expand</summary>
<pre>
<b>Input:</b>
3
^
-2.75
=
</pre>
<pre>
<b>Output:</b>
Please note that exponent's been converted into 2.
[3.000000 ^ 2 = 9.000000]
Result is: 9.000000
</pre>
</details>
Finally, in the case of an invalid operation, the program prints what it's been able to calculate, and then it closes itself.
<details>
<summary>Click to expand</summary>
<pre>
<b>Input:</b>
2
+
50
%
/
0
+
</pre>
<pre>
<b>Output:</b>
[2.000000 + 1.000000 = 3.000000]
[3.000000 / 0.000000] Invalid operation!
Subtotal was before invalid operation occured: 3.000000
</pre>
</details>