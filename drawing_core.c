#include <stdio.h>
#include <stdlib.h>
#include <string.h>  // For strcmp() function
#include <strings.h> // For strcasecmp()
#include <math.h>

// Canvas dimensions
#define WIDTH 40
#define HEIGHT 15

// ANSI color codes enum
typedef enum {
    COLOR_DEFAULT = 39,
    COLOR_BLACK = 30,
    COLOR_RED = 31,
    COLOR_GREEN = 32,
    COLOR_YELLOW = 33,
    COLOR_BLUE = 34,
    COLOR_MAGENTA = 35,
    COLOR_CYAN = 36,
    COLOR_WHITE = 97, // Bright white for better visibility
    COLOR_RESET = 0
} Color;

// Pixel structure to hold character and color
typedef struct {
    char character;
    Color color;
} Pixel;

// Global canvas array
Pixel canvas[HEIGHT][WIDTH];

// 1. INITIALIZE CANVAS - Set up empty canvas
void initializeCanvas() {
    int i, j;
    for (i = 0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            canvas[i][j].character = '.';  // Use dots for empty space
            canvas[i][j].color = COLOR_DEFAULT; // Default color
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
            Pixel p = canvas[i][j];
            // Set color, print character, then reset color
            printf("\x1b[%dm%c", p.color, p.character);
        }
        printf("\x1b[%dm", COLOR_RESET); // Reset color at the end of the line
        printf("\n");
    }
    printf("\n");
}


// 3. DRAW PIXEL - Place a character at specific coordinates
void drawPixel(int x, int y, char brush, Color color) {
    // Check if coordinates are within canvas bounds
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
        printf("Error: Coordinates (%d, %d) are out of bounds!\n", x, y);
        printf("Canvas size is %d x %d (0-%d, 0-%d)\n",
               WIDTH, HEIGHT, WIDTH-1, HEIGHT-1);
        return;
    }

    // Place the character and color on canvas
    canvas[y][x].character = brush;  // Note: y is row, x is column
    canvas[y][x].color = color;
    // We will not print here anymore to reduce noise, displayCanvas will be called
}


// 4. DRAW LINE - Connect two points with characters
void drawLine(int x1, int y1, int x2, int y2, char brush, Color color) {
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
        drawPixel(x, y, brush, color);
        
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
    // No need to print here, displayCanvas will be called
}

// A simple Point struct for our queue-based flood fill
typedef struct {
    int x;
    int y;
} Point;

// 5. FLOOD FILL - Fill an area with the current color
void floodFill(int x, int y, Color newColor) {
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
        printf("Error: Fill coordinates are out of bounds.\n");
        return;
    }

    Color originalColor = canvas[y][x].color;
    if (originalColor == newColor) {
        return; // No work to do
    }

    Point queue[WIDTH * HEIGHT];
    int head = 0;
    int tail = 0;

    queue[tail++] = (Point){x, y};

    while (head < tail) {
        Point p = queue[head++];
        int px = p.x;
        int py = p.y;

        if (px >= 0 && px < WIDTH && py >= 0 && py < HEIGHT && canvas[py][px].color == originalColor) {
            canvas[py][px].color = newColor;

            // Enqueue neighbors if queue is not full
            if (tail < WIDTH * HEIGHT) queue[tail++] = (Point){px + 1, py};
            if (tail < WIDTH * HEIGHT) queue[tail++] = (Point){px - 1, py};
            if (tail < WIDTH * HEIGHT) queue[tail++] = (Point){px, py + 1};
            if (tail < WIDTH * HEIGHT) queue[tail++] = (Point){px, py - 1};
        }
    }
}
    
// 6. CLEAR CANVAS - Reset to empty
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
    printf("color name   - Change drawing color (e.g., red, blue)\n");
    printf("fill x y     - Fill an area starting at (x, y)\n");
    printf("clear        - Clear the canvas\n");
    printf("show         - Display current canvas\n");
    printf("menu         - Show this menu\n");
    printf("quit         - Exit program\n");
    printf("========================\n\n");
}

// Helper function to parse color name string to Color enum
Color getColorFromString(const char* colorStr) {
    if (strcasecmp(colorStr, "red") == 0) return COLOR_RED;
    if (strcasecmp(colorStr, "green") == 0) return COLOR_GREEN;
    if (strcasecmp(colorStr, "yellow") == 0) return COLOR_YELLOW;
    if (strcasecmp(colorStr, "blue") == 0) return COLOR_BLUE;
    if (strcasecmp(colorStr, "magenta") == 0) return COLOR_MAGENTA;
    if (strcasecmp(colorStr, "cyan") == 0) return COLOR_CYAN;
    if (strcasecmp(colorStr, "white") == 0) return COLOR_WHITE;
    if (strcasecmp(colorStr, "black") == 0) return COLOR_BLACK;
    if (strcasecmp(colorStr, "default") == 0) return COLOR_DEFAULT;
    return -1; // Invalid color indicator
}

// 7. INTERACTIVE DRAWING LOOP - Main program logic
int main() {
    char command[20];
    int x, y, x1, y1, x2, y2;
    char brushChar = '*';  // Default brush
    Color currentColor = COLOR_WHITE; // Default color
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
                drawPixel(x, y, brushChar, currentColor);
                displayCanvas();
            } else {
                printf("Invalid coordinates. Use format: x y\n");
                // Clear input buffer
                while (getchar() != '\n');
            }
        }
        else if (strcmp(command, "fill") == 0) {
            // Get coordinates for fill
            printf("Enter x y coordinates to start fill: ");
            fflush(stdout);
            if (scanf("%d %d", &x, &y) == 2) {
                floodFill(x, y, currentColor);
                displayCanvas();
            } else {
                printf("Invalid coordinates. Use format: x y\n");
                while (getchar() != '\n');
            }
        }
        else if (strcmp(command, "line") == 0) {
            // Get coordinates for line drawing
            printf("Enter x1 y1 x2 y2 coordinates: ");
            fflush(stdout);  // Force output to appear
            if (scanf("%d %d %d %d", &x1, &y1, &x2, &y2) == 4) {
                drawLine(x1, y1, x2, y2, brushChar, currentColor);
                displayCanvas();
            } else {
                printf("Invalid coordinates. Use format: x1 y1 x2 y2\n");
                // Clear input buffer
                while (getchar() != '\n');
            }
        }
        else if (strcmp(command, "color") == 0) {
            char colorStr[20];
            printf("Enter color name (e.g., red, green, blue): ");
            fflush(stdout);
            if (scanf("%19s", colorStr) == 1) {
                Color newColor = getColorFromString(colorStr);
                if (newColor != -1) {
                    currentColor = newColor;
                    printf("Color changed to %s.\n", colorStr);
                } else {
                    printf("Unknown color: '%s'.\n", colorStr);
                    printf("Available colors: red, green, yellow, blue, magenta, cyan, white, black, default\n");
                }
            } else {
                printf("Invalid input for color.\n");
                while(getchar() != '\n'); // Clear buffer
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