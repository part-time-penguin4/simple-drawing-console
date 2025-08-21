#include <stdio.h>
#include <stdlib.h>
#include <string.h>  // For strcmp() function

// Canvas dimensions
#define WIDTH 40
#define HEIGHT 15

// Global canvas array
char canvas[HEIGHT][WIDTH];

// 1. INITIALIZE CANVAS - Set up empty canvas
void initializeCanvas() {
    int i, j;
    for (i = 0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            canvas[i][j] = '.';  // Use dots for empty space
        }
    }
    printf("Canvas initialized (%d x %d)\n", WIDTH, HEIGHT);
}

// 2. DISPLAY CANVAS - Show the current drawing
void displayCanvas() {
    int i, j;
    
    // Print top border with column numbers
    printf("\n   ");
    for (j = 0; j < WIDTH; j++) {
        printf("%d", j % 10);  // Show column numbers (0-9 repeating)
    }
    printf("\n");
    
    // Print each row with row numbers
    for (i = 0; i < HEIGHT; i++) {
        printf("%2d ", i);  // Row number (with padding)
        for (j = 0; j < WIDTH; j++) {
            printf("%c", canvas[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// 3. DRAW PIXEL - Place a character at specific coordinates
void drawPixel(int x, int y, char brush) {
    // Check if coordinates are within canvas bounds
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
        printf("Error: Coordinates (%d, %d) are out of bounds!\n", x, y);
        printf("Canvas size is %d x %d (0-%d, 0-%d)\n", 
               WIDTH, HEIGHT, WIDTH-1, HEIGHT-1);
        return;
    }
    
    // Place the character on canvas
    canvas[y][x] = brush;  // Note: y is row, x is column
    printf("Drew '%c' at position (%d, %d)\n", brush, x, y);
}

// 4. CLEAR CANVAS - Reset to empty
void clearCanvas() {
    initializeCanvas();
    printf("Canvas cleared!\n");
}

// 5. DISPLAY MENU - Show available commands
void displayMenu() {
    printf("\n=== DRAWING COMMANDS ===\n");
    printf("draw x y     - Draw at position (x, y)\n");
    printf("brush c      - Change brush to character 'c'\n");
    printf("clear        - Clear the canvas\n");
    printf("show         - Display current canvas\n");
    printf("menu         - Show this menu\n");
    printf("quit         - Exit program\n");
    printf("========================\n\n");
}

// 6. INTERACTIVE DRAWING LOOP - Main program logic
int main() {
    char command[20];
    int x, y;
    char brushChar = '*';  // Default brush
    char newBrush;
    
    printf("=== INTERACTIVE DRAWING PROGRAM ===\n");
    printf("Canvas size: %d x %d\n", WIDTH, HEIGHT);
    
    // Initialize and show empty canvas
    initializeCanvas();
    displayCanvas();
    displayMenu();
    
    // Main interactive loop
    while (1) {
        printf("Enter command: ");
        fflush(stdout);  // Force output to appear immediately
        
        // Get user input
        if (scanf("%s", command) != 1) {
            printf("Invalid input. Try again.\n");
            continue;
        }
        
        // Process different commands
        if (strcmp(command, "draw") == 0) {
            // Get coordinates for drawing
            printf("Enter x y coordinates: ");
            fflush(stdout);  // Force output to appear
            if (scanf("%d %d", &x, &y) == 2) {
                drawPixel(x, y, brushChar);
                displayCanvas();
            } else {
                printf("Invalid coordinates. Use format: x y\n");
                // Clear input buffer
                while (getchar() != '\n');
            }
        }
        else if (strcmp(command, "brush") == 0) {
            // Change brush character
            printf("Enter new brush character: ");
            fflush(stdout);  // Force output to appear
            if (scanf(" %c", &newBrush) == 1) {  // Space before %c to skip whitespace
                brushChar = newBrush;
                printf("Brush changed to '%c'\n", brushChar);
            } else {
                printf("Invalid brush character.\n");
            }
        }
        else if (strcmp(command, "clear") == 0) {
            clearCanvas();
            displayCanvas();
        }
        else if (strcmp(command, "show") == 0) {
            displayCanvas();
        }
        else if (strcmp(command, "menu") == 0) {
            displayMenu();
        }
        else if (strcmp(command, "quit") == 0) {
            printf("Thanks for drawing! Goodbye!\n");
            break;
        }
        else {
            printf("Unknown command '%s'. Type 'menu' for help.\n", command);
        }
        
        // Clear any remaining input
        while (getchar() != '\n');
    }
    
    return 0;
}