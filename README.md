CC105 Week 8 Laboratory

* Project Description
This is a console based application that demonstrates the use of the Stack data structure. It is a balanced bracket checker, able to convert an infix expression to a postfix expression and can evaluate arithmetic expressions.

* Data Structure Used
The project uses a dynamically resized array implemented through a template class called Stack<T>.

* Project Structure
main.cpp - Contains the main menu and program functions, including bracket validation, expression conversion, and postfix evaluation.
Stack.h - Contains the custom template-based Stack class, which manages elements through a dynamically allocated array.

* Stack Operations
-push(value) – Adds an element to the top of the stack.
-pop(value) – Removes the top element.
-peek(value) – Returns the top element without removing it.
-isEmpty() – Checks whether the stack is empty.
-size() – Returns the number of elements.

* Supported Operators
-Addition (+)
-Subtraction (-)
-Multiplication (*)
-Division (/)

* Supported Brackets
-Parentheses: ()
-Square brackets: []
-Curly brackets: {}

* Program Menu Features
1. Check Balanced Brackets
2. Convert and Evaluate Arithmetic Expressions
3. Run Stack Demonstration
4. Exit the Application

* Testing and Validation
The program was tested using the test cases provided in the laboratory activity and three additional test cases created by the group.
Testing covered:

-Stack operations and empty-stack behavior.
-Balanced and unbalanced grouping symbols.
-Infix-to-postfix conversion.
-Postfix arithmetic evaluation.
-Invalid mathematical expressions.
-Division-by-zero errors.
-Dynamic array growth.

The actual program outputs were recorded and compared with the expected results in the test report.
