# mocatra

**mocatra** (**mo**nte **ca**rlo **tra**cer) is a semester long project at the 
[HCW](https://www.hcw.ac.at/) (Hochschule Campus Wien - 
_Computer Science and Digital Communications_), which aims to implement a 
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
  ``sudo apt install build-essential``  

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
  The aspect ratio of the output image, which is given by division of 2 float 
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

Once the prerequisites are installed and optionally any configuration changes 
have been done in the ``config.h`` file, navigate to the root folder of the 
repository in your terminal and use the following commands:  

``make clean``  
``make ``  

This creates a binary for your OS and runs it automatically.  

For development purposes, a debugger friendly version, along with Address 
Sanitizer can be built and run using the following commands:  

``make clean``  
``make debug``  

---  

## How it works  

The following is an attempt at explaining the basic structure and mathematical 
functionality behind the C source code.  
Although a much better structure and explanation can be found in the used 
[References](#references).  

At it's core, the goal of the path tracer is to solve the following problem in 
order to create an image as close to reality as possible:  

**How much light reaches the camera from every single pixel?**  

Physically, the amount of light for any given point, depends on the light 
coming from all directions on the hemisphere above the point, which is 
formulated in the [Rendering Equation](https://en.wikipedia.org/wiki/Rendering_equation#cite_note-Kajiya1986-2):  

$L_{o}(p,\omega_{o})=L_{e}(p,\omega_{o})+\int_{\Omega}{f_{r}(p,\omega_{i},\omega_{o})L_{i}(p,\omega_{i})cos(\theta)d\omega_{i}}$  

Where $L_{o}$ is the light leaving a certain point $p$ in the direction 
$\omega_{o}$, which is the sum of:  

* $L_{e}$ the emitted light  
* $L_{i}$ all incoming light from all directions $\omega_{i}$, depending on 
  how much is reflected $f_{r}$  

Due to the complexity of this integral, solving it analytically is impossible 
and requires another approach, which is where the Monte Carlo Method comes into 
play.  

Instead of solving the integral of the Rendering Equation, we instead average 
the values of random samples, estimating the real value with the following 
estimator:  

$\int{f(x) dx} \approx \frac{1}{N} \sum_{i = 1}^{N}{\frac{f(x_{i})}{p(x_{i})}}$  

Where $f(x_{i})$ is the color returned by a single ray path, $p(x_{i})$ is the 
probability density function (PDF) which gives the probability of choosing a 
specific direction.  

In terms of the Rendering Equation, the Monte Carlo Estimator becomes:  

$L_{o} \approx L_{e} + \frac{1}{N} \sum_{i=1}^{N}{ \frac{ f_{r}(p, \omega_{i}, \omega_{o}) L_{i}(p, \omega_{i}) \cos(\theta) }{ p(\omega_{i}) } }$  

This form is what the Monte Carlo Path Tracer builds upon in the available 
source code. The following is a mapping showing which part in the code 
corresponds to which mathematical component:  

* $N$ . . . ``samples_per_pixel`` in [``config.h``](https://github.com/KaMa-0/mocatra/blob/main/config.h)  

  This value defines how many samples (number of random direction vectors) are 
  used for evaluating the color of one pixel as a whole.  

* $L_{e}$ . . . ``rec.mat.emission`` in [``src/ray.c``](https://github.com/KaMa-0/mocatra/blob/main/src/ray.c)  

  This is the direct light source contribution to the color calculation of a 
  specific hit point, which in turn is used to average the color of the entire 
  pixel.  

* $f_{r}$ . . . Material ([BRDF](https://en.wikipedia.org/wiki/Bidirectional_reflectance_distribution_function))
  Derived from ``rec.mat.albedo`` in [``src/ray.c``](https://github.com/KaMa-0/mocatra/blob/main/src/ray.c)  

  This is the surface property which defines how reflective a given object is. 
  For a perfect Lambertian diffuse surface the BRDF is $\frac{\text{albedo}}{\pi}$.
  However because of the sampling PDF also contains division by $\pi$, the 
  terms cancel out leaving just $\text{albedo}$.  

* $L_{i}$ . . . ``incoming_col`` in [``src/ray.c``](https://github.com/KaMa-0/mocatra/blob/main/src/ray.c)  

  The result of recursive calling of the main ``ray_color`` function where 
  once a ray hits an object in the scene, it scatters in a random direction 
  until either the maximum recursion depth is reached, or a light source from 
  the scene is hit by the ray, which contributes to the color calculation as 
  mentioned for point $L_{e}$.  

* $\cos(\theta)$ . . . included in ``scattering_pdf`` in [``src/ray.c``](https://github.com/KaMa-0/mocatra/blob/main/src/ray.c)  

  [Lambert's Cosine Law](https://en.wikipedia.org/wiki/Lambert%27s_cosine_law) 
  which is included in the ``scattering_pdf``, makes the rays which are 
  randomly scattered, biased towards the center of the hemisphere. 
  (Cosine weighted Sampling)  

* $p(\omega_{i})$ . . . ``pdf`` in [``src/ray.c``](https://github.com/KaMa-0/mocatra/blob/main/src/ray.c)  
 
  The Probability Density Function (PDF) of ``random_unit_vector()`` which 
  serves as a correction factor to divide out the previously introduced 
  sampling bias from **Lambert's Cosine Law** in ``scattering_pdf``,
  to make the Monte Carlo Estimator unbiased.  

---  

## References  

The main resource used for this project is the book series 
[Ray Tracing In One Weekend](https://raytracing.github.io/) by [Peter Shirley](https://github.com/petershirley). 
Specifically the books [_Ray Tracing in One Weekend_](https://raytracing.github.io/books/RayTracingInOneWeekend.html)
and [_Ray Tracing: The Rest of Your Life_](https://raytracing.github.io/books/RayTracingTheRestOfYourLife.html) 
are particularly relevant for this implementation.  

Since the books provide complete code examples for programming along, the 
source code of this project is heavily influenced by the aformentioned material 
and although this is a different implementation in a different language, 
the core principles, structures and logic are taken from the source material 
and adapted to fit the specific project needs in terms of complexity and 
expected results.  

For the basic understanding of Monte Carlo Integration specifically in the 
context of computer graphics and path tracing, the 4th edition of the free 
web version of 
[Physically Based Rendering: From Theory To Implementation](https://pbr-book.org/4ed/contents) 
was used as a reference.  
