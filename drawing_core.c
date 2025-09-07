#include <stdio.h>
#include <stdlib.h>
#include <string.h>  // For strcmp() function
#include <math.h>
#include <string.h>

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


// 4. DRAW LINE - Connect two points with characters
void drawLine(int x1, int y1, int x2, int y2, char brush) {
    // Check if both points are within canvas bounds
    if (x1 < 0 || x1 >= WIDTH || y1 < 0 || y1 >= HEIGHT ||
        x2 < 0 || x2 >= WIDTH || y2 < 0 || y2 >= HEIGHT) {
        printf("Error: Line coordinates are out of bounds!\n");
        printf("Both points must be within (0,0) to (%d,%d)\n", WIDTH-1, HEIGHT-1);
        return;
    }

    // Bresenham's line algorithm
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;  // Step direction for x
    int sy = (y1 < y2) ? 1 : -1;  // Step direction for y
    int err = dx - dy;
    int x = x1, y = y1;
    
    while (1) {
        // Draw current point
        canvas[y][x] = brush;
        
        // Check if we've reached the end point
        if (x == x2 && y == y2) break;
        
        // Calculate error and adjust coordinates
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x += sx;
        }
        if (e2 < dx) {
            err += dx;
            y += sy;
        }
    }
    
    printf("Drew line from (%d,%d) to (%d,%d)\n", x1, y1, x2, y2);
}

    
// 5. CLEAR CANVAS - Reset to empty
void clearCanvas() {
    initializeCanvas();
    printf("Canvas cleared!\n");
}

// 6. DISPLAY MENU - Show available commands
void displayMenu() {
    printf("\n=== DRAWING COMMANDS ===\n");
    printf("draw x y     - Draw at position (x, y)\n");
    printf("line x1 y1 x2 y2 - Draw line from (x1,y1) to (x2,y2)\n");
    printf("brush c      - Change brush to character 'c'\n");
    printf("clear        - Clear the canvas\n");
    printf("show         - Display current canvas\n");
    printf("menu         - Show this menu\n");
    printf("quit         - Exit program\n");
    printf("========================\n\n");
}

// 7. SAVE CANVAS TO FILE - Write current canvas to a text file
void saveCanvas(const char* filename) {
    // Step 1: Try to open file for writing
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error: Could not create file '%s'\n", filename);
        printf("Make sure you have write permissions in this directory.\n");
        return;
    }
    
    printf("Saving canvas to '%s'...\n", filename);
    
    // Step 2: Write a header with canvas dimensions
    // This helps us validate the file when loading
    fprintf(file, "CANVAS %d %d\n", WIDTH, HEIGHT);
    
    // Step 3: Write canvas data row by row
    for (int row = 0; row < HEIGHT; row++) {
        // Write each character in the row
        for (int col = 0; col < WIDTH; col++) {
            fputc(canvas[row][col], file);
        }
        // End each row with a newline for readability
        fputc('\n', file);
    }
    
    // Step 4: Close the file (very important!)
    fclose(file);
    printf("Canvas saved successfully!\n");
}
// LOAD CANVAS FROM FILE - Read canvas data from a text file
void loadCanvas(const char* filename) {
    // Step 1: Try to open file for reading
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Could not open file '%s'\n", filename);
        printf("Make sure the file exists and you have read permissions.\n");
        return;
    }
    
    printf("Loading canvas from '%s'...\n", filename);
    
    // Step 2: Read and validate the header
    char header[10];  // Buffer to store "CANVAS" 
    int fileWidth, fileHeight;
    
    // Read the header line: "CANVAS width height"
    if (fscanf(file, "%s %d %d\n", header, &fileWidth, &fileHeight) != 3) {
        printf("Error: Invalid file format. Expected 'CANVAS width height' on first line.\n");
        fclose(file);
        return;
    }
    
    // Check if it's actually a canvas file
    if (strcmp(header, "CANVAS") != 0) {
        printf("Error: Not a valid canvas file. File should start with 'CANVAS'.\n");
        fclose(file);
        return;
    }
    
    // Check if dimensions match our current canvas
    if (fileWidth != WIDTH || fileHeight != HEIGHT) {
        printf("Error: File dimensions (%d x %d) don't match current canvas (%d x %d)\n", 
               fileWidth, fileHeight, WIDTH, HEIGHT);
        printf("This version only supports files with matching dimensions.\n");
        fclose(file);
        return;
    }
    
    // Step 3: Read canvas data
    for (int row = 0; row < HEIGHT; row++) {
        for (int col = 0; col < WIDTH; col++) {
            int ch = fgetc(file);
            
            // Check for unexpected end of file
            if (ch == EOF) {
                printf("Error: Unexpected end of file at row %d, column %d\n", row, col);
                fclose(file);
                return;
            }
            
            // Store the character in our canvas
            canvas[row][col] = (char)ch;
        }
        
        // Skip the newline character at the end of each row
        int newline = fgetc(file);
        if (newline != '\n' && newline != EOF) {
            printf("Warning: Expected newline at end of row %d\n", row);
        }
    }
    
    // Step 4: Close the file
    fclose(file);
    printf("Canvas loaded successfully!\n");
}

// 8. INTERACTIVE DRAWING LOOP - Main program logic
int main() {
    char command[20];
    int x, y, x1, y1, x2, y2;
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
        else if (strcmp(command, "line") == 0) {
            // Get coordinates for line drawing
            printf("Enter x1 y1 x2 y2 coordinates: ");
            fflush(stdout);  // Force output to appear
            if (scanf("%d %d %d %d", &x1, &y1, &x2, &y2) == 4) {
                drawLine(x1, y1, x2, y2, brushChar);
                displayCanvas();
            } else {
                printf("Invalid coordinates. Use format: x1 y1 x2 y2\n");
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
        else if (strcmp(command, "save") == 0) {
            // Save canvas to file
            char filename[50];
            printf("Enter filename to save: ");
            fflush(stdout);
            if (scanf("%s", filename) == 1) {
                saveCanvas(filename);
            } else {
                printf("Invalid filename.\n");
            }
        }
        else {
            printf("Unknown command '%s'. Type 'menu' for help.\n", command);
        }
        
        // Clear any remaining input
        while (getchar() != '\n');
    }
    
    return 0;
}