# CSC
Program designed to determine, how many edges need to be removed in a tournament graph in order to guarantee acyclicity.

# Build and Run

Before compiling the actual program, we need to compile kissat:
Since Kissat is quite verbose for our purpose, we want to suppress some of its output.
The easiest way to do that is to open the "kissat_folder/kissat/configure.sh" and to change **quiet=no** to **quiet=yes**.
(There is a smoother way to achieve the same goal, but I was unable to reproduce, what I previously did when writing this README :'D)
Now we can build kissat:

```console
cd kissat_folder/kissat
./configure && make
cd ../..
```

Before compiling our program we should set some parameters in constants.h:
* **num_vertices**: defines the number of vertices of the tournament graphs we look at (should never be below 4)
* **number_of_removable_edges**: defines the number of edges the program is allowed to remove in a tournament graph
* **ONLY_COUNT_PLEASE**: When this variable is defined, no SAT-instances get initialized. The program iterates over all possible tournament graphs without actually verifying their solvability.

After configuring these variables to our liking we can compile the program

```console
mkdir build && cd build
cmake ..
make target=release
```

We can now start the program

```console
./csc
```

I advise trying out the program in one configuration, where the program should realize, 
that all problem instances are solvable (for example num_vertices=5, number_of_removable_edges=3)
and one configuration, where this is not the case (for example num_vertices=5, number_of_removable_edges=2).