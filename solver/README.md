# solver
A math solver for the TI-84+ CE.

## Add solvers
To add solvers, go to the source file `solvers.cpp`.

In this file, at the bottom, you will see many lines in the format:
```cpp
SLVER(name) { return ...; }
```
And:
```cpp
registerSolver(LST(...), type, name, "Solver");
```
These are the definitions and registrations, respectively.

To add a new solver, please use:
```cpp
SLVER(<name>) { return <expr>; }
```
Where:
- `<name>` is the name of the function.
- `<expr>` is the expression that is returned.
  - You may use `TIME(n)`, `RATE(n)`, etc. to access the inputs.

Then, you need to register your new solver, like this:
```cpp
registerSolver(LST(<inputs>), <type>, <name>, "<Solver>");
```
Where:
- `<inputs>` is a comma-separated list of the number of inputs, see top of file in the `enum unit_type_t` for the order.
- `<type>` is the return type as defined in the `enum unit_type_t`.
- `<name>` is your function name.
- `<Solver>` is the display name of the solver.