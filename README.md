# RISC-V Assembler and Emulator

This project is a **first-year engineering school student project**, implemented in C. It features both an **assembler** and an **emulator** for a subset of the **RISC-V instruction set architecture**. It allows:

- assembling RISC-V assembly programs into binary or hexadecimal files,
- executing those programs in an emulated environment.

---

## Authors

- Herri Tahar  
- Le Berre Nathan

---

## Prerequisites

Make sure you have the following installed on your Linux system:

### System Tools
- `riscv64-elf-gcc`, `clang`, and `clang-14` – C compilers  
- `llvm-14` & `llvm-14-tools` – Required for `objcopy`  
- `make` – Compilation automation tool  
- `python3` – For running tests  
- `pytest` – To run `test.py`

---

## Installation and Compilation

Clone or download the project, then compile the two main components:

```bash
make
```

This generates two executables at the project root:
- `riscv-assembler` – the RISC-V assembler
- `riscv-emulator` – the RISC-V emulator

---

## Project Structure

```
Assembleur-Emulateur-RISC-V/
├── assembler/           # Assembler source code
├── emulator/            # Emulator source code
├── tests/               # Folder containing test cases
├── test.py              # Python test script
├── Makefile             # Build script
└── README.md            # This file
```

---

## Usage

### 1. Write an Assembly Program

Create a file `example.s` with RISC-V code, for example:

```asm
li x1, 5
li x2, 10
add x3, x1, x2
```

### 2. Assemble the Program

Use the assembler to translate the code:

```bash
./riscv-assembler example.s
```

This will produce one or more of the following files:
- `example.hex` – hexadecimal representation for the emulator
- `example.bin` – binary version
- `example.state` – initial system state

### 3. Run the Program with the Emulator

Once assembled, the `.hex` file can be executed with:

```bash
./riscv-emulator example.hex
```

The emulator loads the file, executes the instructions, and can:
- display the state of registers or memory,
- generate a `.state` file with the final system state.

---

## Testing

The project includes an automated testing script:

```bash
make test
```

This runs `test.py` using `pytest` to test the assembler and emulator with different files in the `tests/` folder.

---

## Cleaning

To remove generated files:

```bash
make clean        # Removes compiled and temporary test files
make cleanall     # Full cleanup (includes executables and Python caches)
```

---

## Creating an Archive

To create a `.tgz` archive of the project:

```bash
make tar
```

This generates a `../HerriTahar_LeBerreNathan.tgz` archive containing the full project, after cleanup.

---

## Notes

- The project supports a subset of the RISC-V instructions, tailored for educational purposes.

### Supported Instructions
- `bge`
- `mv`
- `beq`
- `sd`
- `j`
- `li`
- `addi`
- `ld`
- `add`
- `sub`
- `blt`
- `bne`
- `jal`
