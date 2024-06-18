# graphiX.c

![C](https://img.shields.io/badge/language-C-blue)
![License](https://img.shields.io/badge/license-MIT-green)
![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux-lightgrey)

Mango Graphics Library is a lightweight 2D graphics library implemented in C, designed to provide basic graphics rendering capabilities and event handling without relying on external graphics libraries. It allows you to create windows, handle keyboard and mouse input, and draw basic shapes and lines.

## Features

- **Event Handling**: Keyboard and mouse input handling
- **Basic Drawing Capabilities**:
  - Draw lines
  - Draw rectangles
  - Draw circles
  - Draw polygon of any side
  - Fill areas with color
  - Sprites rendering
- **Customizable**: Easily extendable for more advanced graphics operations

## Table of Contents

- [Installation](#installation)
- [Usage](#usage)
- [Examples](#examples)
- [Contributing](#contributing)
- [License](#license)
- [Acknowledgments](#acknowledgments)

## Installation

### Prerequisites

- A C compiler (GCC recommended)
- Windows: Windows SDK

### Windows

1. Clone the repository:
    ```sh
    git clone https://github.com/Kraken57/graphiX.c.git
    cd mango
    ```

2. Compile the project:
    ```sh
    gcc main.c mango.c -o graphiX.c -lgdi32
    ```
### OR
    ```sh
    1. Open the solution in Visual Studio 2022
    2. Click on Local Window Debugger
    ```


## Usage

### Windows

```c
#include "mangoc_graphics.h"

int main() {
    // Initialize the window and graphics
    initialize_window();
    
    // Main event loop
    while (running) {
        handle_events();
        draw();
    }
    
    // Clean up
    cleanup();
    
    return 0;
}


## Examples

### Drawing a Line

```c
mangoc_draw_line(pixels, WIDTH, HEIGHT, 100, 100, 200, 200, 0xFFFFFFFF);


