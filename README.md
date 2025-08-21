# Simple Drawing Program

Draw ASCII art in your terminal with this C program.

## What it does
- Draw pixels on a 40x15 canvas
- Change brush characters (*, #, @, etc.)
- Clear and redraw as needed

## How to run
```bash
gcc drawing_core.c -o drawing_core.exe
./drawing_core.exe
```

## Commands
- `draw` → Draw a pixel (asks for x y coordinates)
- `brush` → Change drawing character  
- `clear` → Erase canvas
- `show` → Display canvas
- `quit` → Exit

## Example
```
Enter command: draw
Enter x y coordinates: 10 5
Enter command: brush  
Enter new brush character: #
```

## Made with
C programming language - a beginner project to learn 2D arrays and user input.