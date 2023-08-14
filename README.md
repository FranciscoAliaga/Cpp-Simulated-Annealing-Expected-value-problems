# Simulated Annealing for Expected Value Problems

**Summary:**
A few classes/interfaces in C++ implementing "Simulated Annealing" optimization method on optimization problems involving an objective function defined as the expected value of a random variable.

## Summarized Theory

These classes aim to solve problems of the following type:

$$\text{EVP})\quad\quad \min_{x \in X} \quad \mathbb E[f(x,z)] $$

Here, $X$ is a domain of feasible, deterministic decisions $x$, $z$ is a random variable, and $f(x,z)$ is a random quantity dependent of the deterministic decision and the randomness of $z$. Note that if $z$ es deterministic (only takes one value) the problem turns into a deterministic one.

## How to use

Check the file `Rn_problem_template.cpp` it has explanations sufficent for you to figure it out ;).
Essentially, you need to create a derived class for the problem and a derived class for the solver, in them, fill out the details by providing functions specific to your application, and after that, you can run it and retrieve your results.
I also included a `Traveling Salesman Problem` solver example.

## Theory
Let $(\Omega,\mathbb P)$ be a probability space with $\mathbb E[\cdot]$ as the expected value operator. We assume there is a source of uncertainity that can be observed through a random variable $z$ into a space $Z$ ($z:\Omega \rightarrow Z$). We define our optimization problem by setting a domain $X$ of feasible decisions, and a value function $f:X\times Z \longrightarrow \mathbb R$, which takes the value of a decision $x\in X$ and the observed random phenomenon $z=z(\omega), \omega \in \Omega$, and produces the random value function $f(x,z)$. The objective is to minimize the expected value of $f(x,z)$ by selecting a suitable decision $x$, in other words:

$$\text{EVP})\quad\quad \min_{x \in X} \quad \mathbb E[f(x,z)] $$

Note that if $z$ is deterministic (only takes one value) the problem turns into a deterministic one, so this tool can be used for deterministic problems.

EVP stands for Expected value problem.

## Recommended headers for g++
```g++ -O2 -Wall -Weffc++ -Wextra -Wsign-conversion -std=c++17 file.cpp -o file.exe```

# Spanish

TODO.

**Resumen:**
Unas pequeñas clases/interfaces en C++ implementando el método de optimización simulated annealing para resolver problemas de optimización donde hay una esperanza en la función valor.
