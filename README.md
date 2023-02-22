# Simulated Annealing for Expected Value Problems

**Resumen:**
Unas pequeñas clases/interfaces en C++ implementando el método de optimización simulated annealing para resolver problemas de optimización donde hay una esperanza en la función valor.

**Summary:**
A few classes/interfaces in C++ implementing "Simulated Annealing" optimization method on optimization problems involving an objective function that is the expected value of a random variable.

## Summarized Theory

TODO.

## How to use

TODO.

## Theory

TODO.

Let $(\Omega,\mathbb P)$ be a probability space with $\mathbb E[\cdot]$ expected value operator. We assume there is a source of uncertainity that can be observed through a random variable $z$ into a space $Z$ ($z:\Omega \rightarrow Z$). We define our optimization problem by a domain $X$ of feasible decisions, and a value function $f:X\times Z \longrightarrow \mathbb R$, which takes the value of a decision $x\in X$ and the observed random phenomenon $z=z(\omega)$ and produces the random value function $f(x,z)$. The problem we aim to solve is to minimize the expected value of $f(x,z)$ by selecting a suitable decision $x$, in other words:

$$ \min_{x \in X} \quad \mathbb E[f(x,z)] $$

## Recommended headers for g++
```g++ -O2 -Wall -Weffc++ -Wextra -Wsign-conversion -std=c++17 file.cpp -o file.exe```

