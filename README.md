# Bentest

`Bentest` is a **Performance Monitoring Tool** written in C. It measures runtime statistics of a given program.
It repeatedly executes the specified program, collects metrics like execution time, CPU usage, and voluntary context switches, and outputs performance summaries to help identify potential inefficiencies.

## Features

- Executes a program multiple times and measures:
  - User time
  - System time
  - Total execution time
  - CPU usage
  - Voluntary context switches
  - Involuntary context switches
- Uses `getrusage` for high-accuracy CPU time measurement.
- Accepts command-line arguments for flexibility.
- Outputs statistics to help analyze program performance.

## Requirements

- A Linux-based operating system. (Planning to make it portable)
- GCC (GNU Compiler Collection) to compile the source code.

## Installation

1. Clone the repository:
   ```bash
   git clone <repository_url>
   cd <repository_name>
   ```

2. Build the tool using the provided `Makefile`:
   ```bash
   make
   ```

   This will create the `bentest` executable as `build/bentest`.

   > **Note**: Ensure `make` is installed on your system.

## Usage

Run the tool with the following command:
```bash
./build/bentest <num_iterations> <program> [args...]
```

### Arguments:
- `<num_iterations>`: The number of times to execute the program. Must be a positive integer.
- `<program>`: The program to execute (e.g., `gzip`, `curl`, `python` scripts, etc.).
- `[args...]`: Additional arguments for the program.

### Examples:
1. **Compress a file with `gzip`:**
   ```bash
   ./build/bentest 3 gzip sample.txt
   ```
   This runs `gzip sample.txt` three times and monitors its performance.

## Output
The program outputs statistics for each run and a summary of:
- User time
- System time
- Total execution time
- CPU usage percentage
- Voluntary context switches (indicating potential system call frequency)

## How It Works
1. **Argument Parsing**: The tool validates the number of iterations and the program to execute.
2. **Process Creation**: Uses `fork()` to create a child process for each execution.
3. **Program Execution**: The child process runs the program using `execvp()`.
4. **Timing and Metrics**:
   - Captures execution time using `clock_gettime`.
   - Collects resource usage data using `getrusage` for higher accuracy.
   - Tracks voluntary context switches.
   - Tracks involuntary context switches.
5. **Statistics Calculation**: Updates and aggregates runtime statistics across iterations.

## Limitations
- Internal commands (e.g., `cd`, `exit`) are not supported directly.

## Future Enhancements
- Add memory usage monitoring.
- Support for additional metrics like involuntary context switches.
- Portability to non-Linux systems.

## Contributing
Contributions are welcome! If you find a bug or want to add a feature:
1. Fork the repository.
2. Create a new branch.
3. Submit a pull request with a detailed description.

## License
This project is licensed under the [MIT License](LICENSE).

---

### Contact
For any inquiries or feedback, feel free to reach out via GitHub issues.

---

Happy monitoring! :rocket:
