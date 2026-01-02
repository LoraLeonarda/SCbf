# SCbf: Static Convert brain f*ck
This is the implementation of a compiler that converts a custom language into bf, allowing the development of complex bf codes without the bf trouble

# Usage
All the code is contained within `scbf.cpp`, and can be compiled like:
```bash
g++ scbf.cpp -o scbf
```
To compile and run a `.sbf` file, use the compiler like this:
```bash
./scbf <filename>
```
If you only want to convert to bf without running, then add anything as another argument
```bash
./scbf <filename> 1
```

# Commands
```python
# Commands

import X         imports another code in the same folder and appends it at the end
var X            declare a variable
vec X <size>     declare a vector
<stc> X          declares a variable X of the struct <stc> defined anywhere in the code
{                opens a loop, equivalent of bfs [
t{               opens a loop, with a 1 to always enter
}                closes a loop, equivalent of bfs ]
f}               closes and exits with value 0 on the curent expression
true             defines the curent expression to 1
false            defines the curent expression to 0
flip             flips curent expression true or false to false or true

_                prefix to print a string, ex: _hello
.                print the current expression as number, ex: 32 .
..               print the curent expression as ASCII character, ex: 'A' .. (or 65)
&                print a space chacater, ex: _hello & _world!
endl             print a end line character, ex: _hello & _world! endl

get_number       read a decimal number from terminal, wraps around 255 (1 byte)
get_char         read a char from the terminal

+ X              adds a number or variable to current expression
- X              subtracts a number or variable to current expression
* X              multiply a number or variable to current expression
/ X              divide a number or variable to current expression
% X              calculate modulo of current expression by number
>> X             send the value of the current expression to a variable
== X             compare if equal X
!= X             compare if diferent then X
< X              compare if less then X
> X              compare if greater then X
>= X             compare if greater or equal X
<= X             compare if less or equal X
and X            sets curent expression to true if both current expression and loaded value are not 0
or X             sets curent expression to true if any current expression or loaded value are not 0
lda X            loads value in the position 1, imediatly right of current expression. at 0, use with hardcode to create custom operations

hardcode <bf>    appends pure bf code into the output, use with lda and macro to create custom operations, example: 5 lda 8 hardcode >[-<+>]<, this sums up 5 and 8 and put the result in the current expression, you can put this hardcode on macro sum hardcode >[-<+>]< endmacro, then write the line like 5 lda 8 sum

set X            set value at vector X to last X value, uses current expression as position
get X            get value of vector X to operate at base, uses current expression as position

struct X         opens a struct with the name X, every token after is a variable in the struct until the end of the struct, example struct point X Y endstruct

endstruct        closes a struct

macro X          create a macro with the name X, can be used like a function, every token after is the macro code until the end of the macro
endmacro         close a macro

```