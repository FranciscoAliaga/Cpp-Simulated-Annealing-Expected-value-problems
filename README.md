# Simulated Annealing for Expected Value Problems

**Resumen:**
Unas pequeñas clases/interfaces en C++ implementando el método de optimización simulated annealing para resolver problemas de optimización donde hay una esperanza en la función valor.

**Summary:**
A few classes/interfaces in C++ implementing "Simulated Annealing" optimization method on optimization problems involving an objective function defined as the expected value of a random variable.

## Summarized Theory

These classes aim to solve problems of the following type:

$$\text{EVP})\quad\quad \min_{x \in X} \quad \mathbb E[f(x,z)] $$

Here, $X$ is a domain of feasible, deterministic decisions $x$, $z$ is a random variable, and $f(x,z)$ is a random quantity dependent of the deterministic decision and the randomness of $z$. 

## How to use

TODO.

For now, check the file `Rn_problem_template.cpp` and figure it out.
Essentially, you need to create a derived class for the problem and the solver, and after that you can run it and retrieve your results.

## Theory

TODO.

Let $(\Omega,\mathbb P)$ be a probability space with $\mathbb E[\cdot]$ as the expected value operator. We assume there is a source of uncertainity that can be observed through a random variable $z$ into a space $Z$ ($z:\Omega \rightarrow Z$). We define our optimization problem by setting a domain $X$ of feasible decisions, and a value function $f:X\times Z \longrightarrow \mathbb R$, which takes the value of a decision $x\in X$ and the observed random phenomenon $z=z(\omega), \omega \in \Omega$, and produces the random value function $f(x,z)$. The objective is to minimize the expected value of $f(x,z)$ by selecting a suitable decision $x$, in other words:

$$\text{EVP})\quad\quad \min_{x \in X} \quad \mathbb E[f(x,z)] $$

EVP stands for Expected value problem.

## Recommended headers for g++
```g++ -O2 -Wall -Weffc++ -Wextra -Wsign-conversion -std=c++17 file.cpp -o file.exe```
