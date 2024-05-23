# Deinterlace JPEG Image

This command-line application deinterlaces a JPEG image using the libjpeg library. It takes two arguments: the input JPEG file path and the output JPEG file path.

## Features

- Deinterlaces an image by averaging lines N and N-1 from the input image.
- Processes the image in the YCbCr color space.
- Compatible with UNIX-like platforms (Linux and macOS).

## Prerequisites

### macOS

- Homebrew
- libjpeg

### Linux

- libjpeg-dev

## Installation

### macOS

1. **Install Homebrew** (if not already installed):
   ```bash
   /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
2. **Install libjpeg** (if not already installed):
    brew install jpeg

### Linux

1. **Install libjpeg** (if not already installed):
    (if not already installed):

### Build Instructions
    make

### Usage
    ./deinterlace <input_path> <output_path>
