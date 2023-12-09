# COL333: Artificial Intelligence

This repository contains assignments for the course COL333: Artificial Intelligence at the Indian Institute of Technology, Delhi.

**Professor:** [Mausam](https://www.cse.iitd.ac.in/~mausam/)  
**Semester:** Diwali 23.

## 1. Sports Layout

**Collaborator:** [Kushagra Gupta](https://www.github.com/KushagraGupta02)

In this assignment, the goal was to find an optimal allocation of sports zones to locations, minimizing the time spent walking between them. We defined a set $Z$ of $z$ zones and a set $L$ of $l$ locations, with matrices $T_{l\times l}$ and $N_{z\times z}$ representing walk times and daily walks, respectively.

The problem was formulated as a search problem, and local search techniques were employed to find the optimal solution. We implemented Greedy Hill Climbing with Random Restarts for small inputs and Best-first Search for larger inputs. State transitions involved swapping zone locations or using an unused location for a zone. To handle the large number of potential neighbors ( $O(zl)$ ), we transitioned to the first better-cost neighbor found.

## 2. Rollerball

**Collaborator:** [Ankit Mondal](https://www.github.com/AnkMon2025)

This assignment involved implementing an engine for playing rollerball chess. The approach included using mini-max search with alpha-beta pruning and move reordering. Various heuristics were added for material values, pawn promotions, checks, and rooks in the outer circle.

## 3. SAT

**Collaborator:** [Manas Singla](https://www.github.com/singlamanas)

The $\mathsf{MaxClique}$ problem was formulated as a Boolean Satisfiability (SAT) problem, and the <code>minisat</code> solver was employed for its resolution. For every vertex $x_i$, we keep $k$ variables $x_{i,r}, 1\leq r\leq k$. $x_{i,r}$ denotes that $x_{i}$ is the $r^{th}$ vertex of the clique. We have the following clauses if we have a clique of size atleast $k$:

1. Two vertices cannot be the in the same clique if they don't have an edge in the original graph. So, for all $r\neq s$:
    $$\neg e_{i,j} =>  (\neg x_{i,r}) \vee (\neg x_{j,s})$$

2. **Cardinality Constraint** : Sum of the booleans must be $k$. For this we use counter variable encoding.


## 4. Medical Diagnosis using Bayesian Networks

**Collaborator:** [Piyush Chauhan](https://www.github.com/PiyushC1408)

This assignment focused on implementing the EM algorithm to compute the Conditional Probability Table (CPT) tables of a Bayesian Network. The project earned extra credit, and the detailed report can be found [here](https://github.com/AB271202/COL333-Artificial-Intelligence/blob/main/4.%20Medical%20Diagnosis%20with%20Bayesian%20Networks/Report.pdf).

## 5. Rollerball v2

**Collaborator:** [Ankit Mondal](https://www.github.com/AnkMon2025)

This advanced version of the rollerball assignment extended the game to three different boards. Piece-wise heuristics and singular extensions were added to enhance the previous code.
