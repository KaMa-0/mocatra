# MOCATRA v1.1.0-alpha Refactor Decision Record

## Purpose and Scope

This document records the design decisions made while refactoring the code on `refactor/v1.1.0_alpha`, relative to `main`. It explains the motivation, benefits, costs, and regression procedure so the refactor can be reviewed and maintained as an intentional design change rather than just a collection of file moves.

The goal was to make the path tracer easier to navigate and evolve without changing its rendering behavior accidentally. The change was incremental: a deterministic render was established first, then responsibilities and file structure were reorganized in a series of commits. The branch also contains one explicitly intended output correction, described below.

## Regression Baseline and Refactor Procedure

### Why an image baseline was used

The path tracer's main observable result is an image. Compilation and ordinary unit-level checks cannot reveal every accidental change to sampling, camera rays, geometry intersections, material scattering, or accumulated color. A fixed render therefore provides a useful end-to-end regression check across those components.

Commit `20182e3` (`Basic restructure and add seeding option`) introduced the stored `test/baseline_original.ppm` image and seed control. Fixing render dimensions, samples per pixel, ray depth, and the random seed makes the test configuration repeatable. Without a fixed seed, Monte Carlo variance would cause legitimate pixel differences on each run and make exact comparison unusable.

The regression builds use a dedicated low-cost configuration in `config.h` rather than the interactive/default render settings. The baseline and test modes use the same source files and compiler options; only their output path and mode macro differ. The Makefile builds separate executables for the two modes so an executable built with one configuration cannot accidentally be reused for the other. Headers are explicit build prerequisites so configuration and interface changes trigger recompilation.

### Running the check

From the repository root, run:

```sh
make test
```

This renders the test-mode image and compares it with `test/baseline_original.ppm` using `tools/compare_ppm.py`. The default tolerance is zero: image dimensions and every RGB channel must match exactly. The comparator accepts an optional per-channel tolerance when run directly, but the standard `make test` path intentionally uses exact comparison. A format or file-reading error is reported separately from an image mismatch.

The baseline should be treated as evidence, not as a generated build artifact. `make baseline-update` refuses to overwrite an existing baseline unless `FORCE=1` is supplied. To accept an intentional output change, regenerate and then rerun the comparison:

```sh
make baseline-update FORCE=1
make test
```

An update should only be made after confirming that the output difference is intentional, recording why it is intentional, and reviewing the resulting image. Replacing the baseline merely to make a failing test pass would remove the regression signal.

### The intentional baseline change

Commit `4e29c04` (`Fix channel to byte coversion overflows (BASELINE)`) deliberately changed both `src/io/image.c` and the stored PPM image. It added `channel_to_byte`, which clamps a floating-point color channel below zero to zero and above one to one before converting it to an 8-bit value. The earlier conversion multiplied by `255.999f` and cast directly to an unsigned byte; out-of-range values could wrap or otherwise produce invalid output instead of being clipped to the representable color range.

This is a behavior correction, not an unexplained regression. Converting a floating-point value outside the representable range directly to an integer does not provide a reliable clipped result in C; clamping first makes the conversion defined for the intended color range. The image was intentionally regenerated in the same commit to make the corrected output the new expected result. Because the fixture retains the name `baseline_original.ppm`, that name is historical: after `4e29c04` it no longer means an untouched render from the pre-refactor `main` branch. This distinction matters when comparing renders across the whole branch history. The seed and test settings continue to make subsequent comparisons deterministic against the corrected reference.

## Design Decisions

### Group code by responsibility and keep interfaces nearby

The initial structural change, `a1d3cdd` (`Split sources into folders and unify clang-format`), replaced the flat source layout and separate `include/` directory with responsibility-based groups under `src/`: `core`, `geometry`, `scene`, `material`, `io`, `render`, `sampler`, and `integrator`. A component's `.c` and `.h` files now generally live together, and include paths communicate which subsystem owns a dependency.

This makes the architecture visible in the tree and makes related implementation and interface easier to find together. It also avoids maintaining parallel source and include hierarchies while the project is developed as a complete executable. The tradeoff is that headers are not isolated in a public include tree: consumers that include them depend on the internal `src/` layout. That is acceptable for the current project, which is not presented as a separately packaged C library. If a stable external library API becomes a goal, a deliberate public-header boundary should be introduced rather than assuming these internal headers are already stable.

The same commit consolidated formatting rules into the root `.clang-format` configuration. One formatter policy is easier to apply consistently across implementation and header files; the cost is that formatting changes can create broad diffs, so formatting-only changes should remain distinguishable from behavioral edits during review.

### Separate application setup from rendering work

The renderer extraction (`a052ae4`, `Refactor main loop to renderer file`) moves the per-pixel and per-sample traversal out of the application setup code and into `src/render/renderer.c`. The top-level code is consequently responsible for configuring and assembling the render, while the renderer owns image traversal, sample accumulation, pixel writes, and progress reporting.

This boundary gives the main program a clearer role and gives rendering a named interface that can be configured independently. The tradeoff is another interface and some setup plumbing; for a small tracer, a single file would have fewer declarations. The split was chosen because the render loop is a central responsibility with distinct inputs and outputs, and keeping it separate makes later changes easier to localize.

The application entry point was also separated into `src/main.c` and the MOCATRA initialization API. This keeps the executable entry point small, although the current initialization path still owns application-level setup and error handling rather than defining a general-purpose library lifecycle.

### Make configuration explicit and validate it centrally

The configuration changes in `3ed167a` and `b1cf3e5` distinguish user-facing render settings from development/test settings. `config.h` provides a `mocatra_conf_t`, computes image height from width and aspect ratio, and supplies validation. Regression builds select fixed test parameters independently of user render settings.

Keeping values together avoids scattered constants and reduces the chance that the stored baseline and test run use different dimensions or sampling parameters. Compile-time selection keeps the test configuration simple and reproducible. Its costs are that changing configuration requires a rebuild and that compile-time modes are less flexible than runtime configuration. This is an appropriate tradeoff for a small command-line program with a deliberately fixed regression render.

The Makefile refactor (`0fcc920`) supports separate normal, baseline-generation, and test builds, includes headers in target dependencies, and uses common regression compiler flags. These choices favor repeatability and prevent stale mode-specific binaries. They do not make renders portable across every compiler, architecture, or floating-point implementation; the regression image is most dependable when produced and checked in the same supported build environment and with the same flags.

### Give the camera a configuration object

Commit `e1a872d` changes camera construction to accept an explicit `camera_config_t` containing the view origin, target, up vector, and vertical field of view. A default configuration preserves the scene's existing camera while exposing the inputs as data rather than burying them inside camera construction.

This separates camera choices from viewport geometry calculations and makes alternate views easier to express. The added struct and initialization path are slightly more verbose than fixed local constants; that cost is justified by making the camera's inputs inspectable and changeable without rewriting its math.

### Separate material behavior from ray traversal

Commits `e909f83` and `be4552e` move material construction and scattering behavior into `src/material/`. Ray/path traversal should ask a material how it scatters and what attenuation or emission it contributes, rather than owning the details of each material type. This reduces the amount of material-specific branching in the ray code and gives material behavior one implementation home.

The refactor introduces an interface boundary and may require touching both the material and integrator when adding a new scattering model. That is preferable to allowing ray traversal, geometry, and material rules to become interleaved again. It remains a compact design, not a fully general material plug-in system.

### Make vector operations header-only

Commit `8653fae` moves the small 3D vector operations into `src/core/vec3.h` as `static inline` functions and removes the separate implementation file. This keeps simple arithmetic close to the type, removes a compilation unit and its linkage overhead, and gives the compiler an opportunity to inline frequently used operations.

The cost is increased rebuild scope: because vector operations are included by most modules, changing `vec3.h` can cause much of the program to recompile. Header-defined functions can also increase code size if inlining is not beneficial. The decision favors a small, stable set of fundamental operations; it should be revisited if vector functionality grows substantially or compile times become material.

### Keep scene construction and geometry operations distinct

The world and quad refactors (`b808ffe`, `1057ca5`) move scene assembly into `src/scene/world.c` and keep shape intersection behavior in the geometry components. The world describes which objects and materials make up the scene; geometry code describes how a particular object is intersected. This avoids making scene setup the owner of low-level shape math and makes reusable geometry easier to locate.

This is organizational separation, not a general scene-description or asset-loading system. The scene remains constructed in code, which is simple and type-checked but requires recompilation to change it.

### Extract sampling and path integration

Commit `3e07747` introduces `src/sampler/` and `src/integrator/`. The renderer now consumes a sampler and integrator interface: sampling generates pixel offsets and random directions, while the path-tracing integrator evaluates the recursive light transport. This separates three different concerns: pixel iteration and averaging, random sample generation, and ray-path evaluation.

The benefit is a clearer place to add or compare sampling strategies and integration algorithms without rewriting the renderer's pixel loop. The cost is additional callback interfaces and indirection in a program that currently uses a default sampler and path tracer. The interfaces are intentionally small; they create an extension point, not a guarantee that every alternative algorithm is already supported or tested.

The first sampler interface left `state` unused and used the process-global C `rand()` generator. That made the API appear stateful without actually giving a sampler independent state, and coupled reproducibility to global seeding. The sampler now receives caller-owned RNG state initialized for each render. The default generator updates only that state, so different sampler instances do not share a global random stream and the seed has an explicit owner and lifetime.

Pixel-offset callbacks also receive pixel coordinates, sample index, and total samples per pixel. These inputs let future stratified or sample-sequence strategies place samples using their identity rather than relying on call counts hidden in mutable state. The default random-offset strategy ignores those inputs and preserves the existing independent-jitter behavior. The camera now receives an explicit offset instead of selecting a hidden global default sampler, keeping sampling policy in the sampler and ray construction in the camera.

Replacing `rand()` with a sampler-owned generator intentionally changes the exact seeded pixel sequence, even though the default sampling distribution and rendering algorithm are unchanged. The regression baseline must therefore be regenerated once after this change and then checked exactly on subsequent runs. This is a test-reference update for a changed deterministic sequence, not a visual-quality correction like the separate channel-clamping baseline change documented above.

The API is ready for pixel-sampling alternatives, but it does not make arbitrary direction distributions unbiased automatically. The current path tracer and material scattering model assume their existing direction-sampling behavior; a different directional distribution must be paired with the appropriate probability-density weighting in the integrator/material calculation. The RNG state is also currently sequential and intended for the serial renderer; parallel rendering will need independent per-worker or per-sample streams to avoid races and scheduling-dependent results.

### Keep project-wide errors and remove unused interfaces

The error enum was consolidated into `src/mocatra.h` (`4d76a9c`), and the file-output error value was added (`6138128`). This keeps the small set of project-wide error results in one central project header instead of requiring a separate header for a handful of closely related values. The tradeoff is that modules including the central header may inherit broader dependencies; if the error API grows or more subsystems become independently reusable, a dedicated shared error header may again be the cleaner boundary.

The unused renderer-default-settings function was removed in `f8bb805`. Removing an unused API avoids suggesting that there is a supported behavior callers can rely on when there is not. These cleanups favor a smaller, accurate interface over preserving declarations solely for possible future use.

## Commit Landmarks

The following commits mark the main steps in the documented sequence:

| Commit               | Role                                                               |
| -------------------- | ------------------------------------------------------------------ |
| `20182e3`            | Added deterministic seeding and the initial image baseline.        |
| `a1d3cdd`            | Grouped sources by responsibility and unified formatting.          |
| `a052ae4`            | Extracted the render loop.                                         |
| `3ed167a`, `b1cf3e5` | Reworked user and regression configuration.                        |
| `0fcc920`            | Reworked build targets and regression workflow.                    |
| `a45fc9d`            | Refactored image I/O.                                              |
| `4e29c04`            | Fixed channel clamping and intentionally replaced the baseline.    |
| `e1a872d`            | Made camera parameters explicit.                                   |
| `e909f83`, `be4552e` | Separated material behavior from ray traversal.                    |
| `8653fae`            | Made vector operations header-only.                                |
| `b808ffe`, `1057ca5` | Refactored world and quad/geometry responsibilities.               |
| `3e07747`, `3234d38` | Extracted sampler and integrator, then corrected follow-up issues. |

The commit history is the authoritative record of exact code changes. This document describes the intent and tradeoffs visible in those changes; it does not claim that every possible alternative was benchmarked or that the new boundaries are immutable.

## Summary of Tradeoffs

The refactor favors navigability, localized responsibilities, and reproducible behavioral checks over the smallest possible number of files and interfaces. The result has more modules, headers, and setup code, but each major concern has a clearer owner. The image baseline provides a practical safeguard while those boundaries evolve, with the important qualification that the reference was deliberately advanced once to account for the color-clamping fix.

The changes do not by themselves establish a stable library API, guarantee bit-identical output across toolchains, or make the renderer parallel-safe. Those remain separate design questions and should not be inferred from the new folder structure or callback interfaces.
