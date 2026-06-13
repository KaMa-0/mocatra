# mocatra

**mocatra** (**mo**nte **ca**rlo **tra**cer) is a semester long project at the 
[HCW](https://www.hcw.ac.at/) (Hochschule Campus Wien - 
_Computer Science and Digital Communications_), which aims at implementing a 
monte carlo path tracer from scratch, in the C programming language.  

---  

## Quick Start  

The following are instructions on how to compile the project from source code.  
Alternatively, the pre-compiled binaries can be used, which are accessible 
through the [Releases](https://github.com/KaMa-0/mocatra/releases) page.  

**Note:** By using the pre-compiled binaries, default parameters for the render 
(samples per pixel, recursion depth for rays, image size, etc.) will be used, 
without the ability to adjust any of the options.  

### Prerequisites

In order to compile the source code via the Makefile, some utilities will be 
needed. The following segment explains how to get and setup the necessary tools 
in order to successfully build the mocatra application.  

#### Linux

Most linux distributions come with the necessary tools available in the 
default package manager.  

**Arch Linux / Manjaro (``pacman``):**  

* recommended update:  
  ``sudo pacman -Syu``  

* for gcc compiler & make build tool:  
  ``sudo pacman -S base-devel``  
  
* gdb debugger (optional):  
  ``sudo pacman -S gdb``  

**Ubuntu / Debian (``apt``):**  

* recommended update:  
  ``sudo apt update`` 

* for gcc compiler & make build tool:  
  ``sudo apt install build-essentials``  

* gdb debugger (optional):  
  ``sudo apt install gdb``  

**Fedora / RHEL (``dnf``):**  

* for gcc compiler & make build tool:  
  ``sudo dnf groupinstall "Development Tools"``  

* gdb debugger (optional):  
  ``sudo dnf install gdb``  

#### MacOS

Apple Xcode Command Line Tools provide gcc and make.  

* for gcc compiler & make build tool:  
  ``xcode-select --install``

* gdb debugger (optional) via [brew](https://brew.sh/):  
  ``brew install gdb``  

#### Windows  

Since windows does not natively include Unix tools like ``make`` or ``gcc``, 
using [MSYS2](https://www.msys2.org/) is the recommended approach.  

* Download and install [MSYS2](https://www.msys2.org/#installation)  

* Open MSYS2 UCRT64 terminal, run:  
  ``pacman -S mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-gdb make``  

* Add MSYS2 binary folder (e.g. ``C:\msys64\ucrt64\bin``) to Windows System 
  Environment ``PATH`` variables (requires administrator access)  

### Configuration

As mentioned, compiling from sources provides the ability to edit the 
configuration of the path tracer, located in the [``config.h``](https://github.com/KaMa-0/mocatra/blob/main/config.h) 
file.

The following are the configuration options available in ``config.h``:  

* ``output_image_path``:  
  The relative path to where the output image should be stored. The path must 
  be relative to the project root, where the config.h file is located.  

* ``image_aspect_ratio``:  
  The aspect ration of the output image, which is given by division of 2 float 
  values (in C float values must be of format: ``1.0f``). 
  Example ratios would be: ``1.0f / 1.0f``, ``19.0f / 6.0f``, ``4.0f / 3.0f``.  
  This parameter has an **impact on render quality and performance**.  

* ``image_width``:  
  The width of the output image in pixels, the appropriate height is calculated 
  via the aspect ratio and is not part of the configuration.  
  This parameter has an **impact on render quality and performance**.  

* ``samples_per_pixel``:  
  The number of samples is the number of distinct, randomized rays which are 
  sent through different locations inside the boundaries of a single pixel. 
  The more samples per pixel are configured the less "noise" the final render 
  will have, because there will be more rays exploring the scene and interacting 
  with the light source and objects around it.  
  This parameter has an **impact on render quality and performance**.  

* ``maximum_ray_depth``:  
  The number of recursions allowed for a single ray, which controls how many 
  times a ray is allowed to bounce off surfaces after entering the scene.  
  The higher the maximum depth, the better (realistic) the scene lighting.  
  This parameter has an **impact on render quality and performance**.  

**Note:** the configuration must be adapted before compiling the code for the 
changes to take effect, also the values for the configuration options must 
strictly match the C syntax and expected types!  

### Building and Running  

Once the prerequisites are installed and optionally  navigate to the root folder of the 
repository in your terminal and use the following commands:  

``make clean``  
``make ``  

This creates a binary for your OS and runs it automatically.  

For development purposes, a debugger friendly version, along with Address 
Sanitizer can be built using the following commands:  

``make clean``  
``make debug``  

---  

## References  

The main resource used for this project is the book series 
[Ray Tracing In One Weekend](https://raytracing.github.io/) by [Peter Shirley](https://github.com/petershirley). 
Specifically the books [_Ray Tracing in One Weekend_](https://raytracing.github.io/books/RayTracingInOneWeekend.html)
and [_Ray Tracing: The Rest of Your Life_](https://raytracing.github.io/books/RayTracingTheRestOfYourLife.html) are particularly relevant for this 
implementation.  

Since the books provide complete code examples for programming along, the 
source code of this project is heavily influenced by the aformentioned material 
and although a different implementation in a different language, the core 
principles, structures and logic are taken from the source material and adapted 
to fit the specific project needs in terms of complexity and expected results.  
