# Compiler Design Laboratory – Complete C Implementations (Labs 1–15)

This repository provides **fully working, educational, and viva-ready C programs** covering the **complete Compiler Design Laboratory syllabus (Experiments 1–15)**.

It is designed for:

* 📘 **University lab submission**
* 🎓 **Viva & exam preparation**
* 🧠 **Concept-driven learning through implementation**
* 💻 **Practical understanding of compiler front-end design**

All programs are:

* Written in **pure C**
* **Easy to compile with GCC**
* Structured for **clarity + learning**
* Aligned with **standard Compiler Design curriculum**

---

# 📚 List of Experiments

## 🔹 Lab 1 – Lexical Analyzer

**Objective:**
Implement a lexical analyzer to tokenize a C source program.

**Key Features:**

* Keyword, identifier, number recognition
* Operators, delimiters, and string literals
* Single-line & multi-line comment removal
* Symbol table construction
* Line-wise debugging token output

**Core Concepts:**
Tokenization, implicit DFA scanning, symbol table management.

---

## 🔹 Lab 2 – Regular Expression → NFA

**Objective:**
Construct an **NFA using Thompson’s Construction** from a regular expression.

**Key Features:**

* Infix → postfix conversion
* Support for union (`|`), concatenation (`.`), Kleene star (`*`)
* Linked-list transition representation
* Formal NFA transition display

**Core Concepts:**
Regular expressions, ε-transitions, automata construction.

---

## 🔹 Lab 3 – NFA → DFA Conversion

**Objective:**
Convert an NFA into an equivalent DFA using **subset construction**.

**Key Features:**

* ε-closure computation
* Move function
* DFA state generation from NFA state sets
* Final state detection
* DFA transition table output

**Core Concepts:**
Deterministic automata, power-set construction, closure operations.

---

## 🔹 Lab 4 – Elimination of Left Recursion & Left Factoring

**Objective:**
Transform grammar to make it suitable for **top-down parsing**.

**Key Features:**

* Immediate left recursion removal
* Creation of new non-terminals (A′)
* ε-production handling
* Left factoring using common prefix detection
* Stepwise grammar transformation output

**Core Concepts:**
Grammar normalization, predictive parsing preparation.

---

## 🔹 Lab 5 – FIRST and FOLLOW Set Computation

**Objective:**
Compute **FIRST** and **FOLLOW** sets for a given context-free grammar.

**Key Features:**

* Recursive FIRST computation
* FOLLOW propagation rules
* ε-handling
* Table-style output

**Core Concepts:**
Predictive parsing theory, LL(1) prerequisites.

---

## 🔹 Lab 6 – Construction of LL(1) Parsing Table

**Objective:**
Generate **LL(1) parsing table** using FIRST & FOLLOW sets.

**Key Features:**

* Table generation algorithm
* Conflict detection
* LL(1) grammar validation

**Core Concepts:**
Top-down parsing, predictive parser design.

---

## 🔹 Lab 7 – Predictive Parsing (LL(1) Parser)

**Objective:**
Implement **string parsing using LL(1) parsing table**.

**Key Features:**

* Stack-based parsing simulation
* Input buffer handling
* Step-by-step derivation display
* Accept / reject decision

**Core Concepts:**
Top-down parsing execution.

---

## 🔹 Lab 8 – Shift-Reduce Parser

**Objective:**
Implement **basic bottom-up parsing** using shift-reduce method.

**Key Features:**

* Stack operations (shift & reduce)
* Handle detection
* Parsing trace output

**Core Concepts:**
Bottom-up parsing fundamentals.

---

## 🔹 Lab 9 – Operator Precedence Parser

**Objective:**
Construct parser using **operator precedence relations**.

**Key Features:**

* Precedence table creation
* Shift-reduce decisions using precedence
* Expression validation

**Core Concepts:**
Operator grammar, precedence parsing.

---

## 🔹 Lab 10 – SLR(1) Parsing Table Construction

**Objective:**
Build **SLR parsing table** for a grammar.

**Key Features:**

* LR(0) item generation
* Closure & GOTO computation
* ACTION & GOTO table creation
* Conflict identification

**Core Concepts:**
LR parsing theory, bottom-up deterministic parsing.

---

## 🔹 Lab 11 – Canonical LR(1) Parser

**Objective:**
Construct **LR(1) parsing table** with lookahead symbols.

**Key Features:**

* LR(1) item sets
* Closure with lookahead
* ACTION/GOTO table
* Parsing simulation

**Core Concepts:**
Full LR parsing power, deterministic CFG parsing.

---

## 🔹 Lab 12 – LALR(1) Parser

**Objective:**
Create **LALR(1) parsing table** by merging LR(1) states.

**Key Features:**

* State merging
* Conflict preservation checks
* Efficient LR parsing

**Core Concepts:**
Practical parser design used in real compilers.

---

## 🔹 Lab 13 – Intermediate Code Generation

**Objective:**
Generate **three-address code (TAC)** for expressions.

**Key Features:**

* Temporary variable creation
* Quadruple / triple representation
* Expression translation

**Core Concepts:**
Intermediate representation, syntax-directed translation.

---

## 🔹 Lab 14 – Code Optimization Techniques

**Objective:**
Apply **basic optimization** on intermediate code.

**Key Features:**

* Constant folding
* Common subexpression elimination
* Dead code removal
* Strength reduction

**Core Concepts:**
Compiler optimization fundamentals.

---

## 🔹 Lab 15 – Target Code Generation

**Objective:**
Generate **simple assembly-like target code** from TAC.

**Key Features:**

* Register allocation (basic)
* Instruction mapping
* Final code emission

**Core Concepts:**
Back-end compilation, machine-level translation.

---

# 🛠️ Technologies Used

* **Programming Language:** C
* **Compiler:** GCC / Clang
* **Domain:** Compiler Design + Automata Theory

---

# ▶️ How to Compile & Run

```bash
gcc lab_file.c -o output
./output
```

Example:

```bash
gcc lab1_lexical_analyzer.c -o lab1
./lab1
```

---

# 🎯 Learning Outcomes

After completing all 15 experiments, you will understand:

* Complete **compiler front-end workflow**
* **Automata theory in real implementation**
* **Parsing techniques (LL, LR, SLR, LALR)**
* **Intermediate code generation & optimization**
* Basics of **target code generation**

---

# 📌 Ideal For

* B.Tech / BE **Compiler Design Laboratory**
* **University practical exams**
* **Viva preparation**
* **GATE CS fundamentals**
* Beginners exploring **compiler construction**

---

# 👨‍💻 Author

**Zian Rajeshkumar Surani**
Computer Science Engineering (AI & ML)
SRM Institute of Science and Technology

Focused on **AI systems, compilers, and deep technical learning**.

---

# ⭐ Support

If this repository helped you in learning or lab submission,
please consider giving it a **star ⭐ on GitHub** to support open educational resources.
