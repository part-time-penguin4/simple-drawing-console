#!/bin/bash

# Exit immediately if a command exits with a non-zero status.
set -e

# Compile the program
echo "Compiling program..."
gcc drawing_core.c -o drawing_core.exe
echo "Compilation successful."

# --- Test Case 1: Invalid command ---
# Checks if the program correctly handles a command that doesn't exist.
# It should print an error and then successfully process the next command ('quit').
echo -e "\n--- Running Test Case 1: Invalid Command ---"
output_case1=$(./drawing_core.exe <<EOF
invalidcommand
quit
EOF
)

# Verification
if echo "$output_case1" | grep -q "Unknown command 'invalidcommand'" && \
   echo "$output_case1" | grep -q "Thanks for drawing! Goodbye!"; then
    echo "Test Case 1 PASSED: Program handled invalid command and exited gracefully."
else
    echo "Test Case 1 FAILED: Program did not handle invalid command correctly."
    echo "--- Output ---"
    echo "$output_case1"
    echo "----------------"
    exit 1
fi

# --- Test Case 2: Invalid arguments for 'draw' command ---
# Checks if the program correctly handles non-numeric input for a command expecting numbers.
# It should print an error and then successfully process the next command ('quit').
echo -e "\n--- Running Test Case 2: Invalid 'draw' Arguments ---"
output_case2=$(./drawing_core.exe <<EOF
draw
a b
quit
EOF
)

# Verification
if echo "$output_case2" | grep -q "Invalid coordinates. Use format: x y" && \
   echo "$output_case2" | grep -q "Thanks for drawing! Goodbye!"; then
    echo "Test Case 2 PASSED: Program handled invalid arguments and exited gracefully."
else
    echo "Test Case 2 FAILED: Program did not handle invalid arguments correctly."
    echo "--- Output ---"
    echo "$output_case2"
    echo "----------------"
    exit 1
fi

# --- Test Case 3: Valid command after invalid command ---
# A regression test to ensure that valid commands still work.
echo -e "\n--- Running Test Case 3: Valid Command (Regression) ---"
output_case3=$(./drawing_core.exe <<EOF
brush
#
show
quit
EOF
)

# Verification
if echo "$output_case3" | grep -q "Brush changed to '#'" && \
   echo "$output_case3" | grep -q "Thanks for drawing! Goodbye!"; then
    echo "Test Case 3 PASSED: Program processed valid commands correctly."
else
    echo "Test Case 3 FAILED: A valid command sequence failed."
    echo "--- Output ---"
    echo "$output_case3"
    echo "----------------"
    exit 1
fi

echo -e "\nAll test cases passed successfully!"
