# SCbf: Static Convert brain f*ck
This is the implementation of a compiler that converts a custom language into bf, allowing the development of complex bf codes without the trouble

# Usage
All the code is contained within `scbf.cpp`, and can be compiled like:
```bash
g++ scbf.cpp -o scbf
```
To compile and run a `.sbf` file, use the compiler like this:
```bash
./scbf <filename>
```
If you only want to convert to bf withour running, then add anything as another argument
```bash
./scbf <filename> 1
```

# Commands
```python
import X        imports another code in the same folder and appends it at the end
var X           declare a variable
vec X <size>    declare a vector
{               opens a loop, equivalent of bfs [
t{              opens a loop, with a 1 to always enter
}               closes a loop, equivalent of bfs ]
f}              closes and exits with value 0
true            defines the current number to 1
false           defines the current number to 0
flip            flips curent true or false

_               prefix to print a string, ex: _hello
.               print the current expression as number, ex: 32 .
..              print the current character as character, ex: 'A' .. (or 65)
&               print a space chacater, ex: _hello & _world!
endl            print a end line character, ex: _hello & _world! endl

get_number      read a decimal number from terminal up to 255
get_char        read a char from the terminal

+ X             adds a number or variable to current expression
- X             subtracts a number or variable to current expression
* X             multiply a number or variable to current expression
/ X             divide a number or variable to current expression
% X             calculate modulo of current expression by number
>> X            send the value of the current expression to a variable
== X            compare if equal X
!= X            compare if diferent then X
< X             compare if less then X
> X             compare if greater then X

set X           set value at vector X to last X value, uses current expression as position
get X           get value of vector X to operate at base, uses current expression as position

macro X         create a macro, can be used like a function
endmacro        close a macro
```