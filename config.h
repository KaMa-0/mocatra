/*
 * \file        config.h
 * \brief       MOCATRA configuration file
 * */

#ifndef _MOCATRA_CONFIG_H
#define _MOCATRA_CONFIG_H

/* =========================== USER CONFIGURATION =========================== */
/* these configuration values can be adjusted to give desired render output */

#define USER_OUTPUT_PATH        "out/mocatra_image.ppm"
#define USER_IMAGE_WIDTH        600
#define USER_ASPECT_RATIO       (1.0f / 1.0f)   /* width / height       */
#define USER_SPP                100             /* samples per pixel    */
#define USER_RAY_DEPTH          50              /* maximum bounces      */
#define USER_SEED               0u              /* seed from the clock  */

/* ========================= END OF USER CONFIGURATION ====================== */

/* -------------------------------------------------------------------------- */
/* Nothing below this line needs to be edited by the user.                    */
/* Following configuration is for development and testing only.               */ 
/* -------------------------------------------------------------------------- */

typedef struct mocatra_conf_t {
        char*        img_path;
        int          img_width;
        float        img_ratio;
        int          img_height;
        int          spp;
        int          ray_depth;
        unsigned int seed;
} mocatra_conf_t;

#if defined(MOCATRA_REGRESSION) && defined(MOCATRA_TEST)
#error "MOCATRA_REGRESSION and MOCATRA_TEST are mutually exclusive"
#endif

#if defined(MOCATRA_REGRESSION) || defined(MOCATRA_TEST)

#define CONF_IMAGE_WIDTH  128
#define CONF_ASPECT_RATIO (1.0f / 1.0f)
#define CONF_SPP          64
#define CONF_RAY_DEPTH    5
#define CONF_SEED         43u
 
#ifdef MOCATRA_REGRESSION       /* build for regression-test baseline         */
#define CONF_OUTPUT_PATH "out/regression/baseline.ppm"
#else                           /* build for regression-test run (compare)    */
#define CONF_OUTPUT_PATH "out/regression/test.ppm"
#endif
 
#else                           /* normal build: take user configuration      */
 
#define CONF_OUTPUT_PATH  USER_OUTPUT_PATH
#define CONF_IMAGE_WIDTH  USER_IMAGE_WIDTH
#define CONF_ASPECT_RATIO USER_ASPECT_RATIO
#define CONF_SPP          USER_SPP
#define CONF_RAY_DEPTH    USER_RAY_DEPTH
#define CONF_SEED         USER_SEED
 
#endif
 
#define CONF_MAX_DIMENSIONS 2073600

static inline int
conf_image_height(int width, float ratio)
{
        float h;
        
        if (width < 1 || width > CONF_MAX_DIMENSIONS || !(ratio > 0.0f)) {
                return 0;
        }

        h = (float)width / (float)ratio;
        if (h > CONF_MAX_DIMENSIONS) {
                return 0;
        }

        return (h < 1.0) ? 1 : (int)h;
}

static inline mocatra_conf_t
load_config(void)
{
        return (mocatra_conf_t){
                .img_path   = CONF_OUTPUT_PATH,
                .img_ratio  = CONF_ASPECT_RATIO,
                .img_width  = CONF_IMAGE_WIDTH,
                .img_height = conf_image_height(CONF_IMAGE_WIDTH, CONF_ASPECT_RATIO),
                .spp        = CONF_SPP,
                .ray_depth  = CONF_RAY_DEPTH,
                .seed       = CONF_SEED,
        };
}

static inline mocatra_error_t
config_validate(const mocatra_conf_t* c)
{
        if (c == NULL || c->img_path == NULL || c->img_height < 1 || c->spp < 1 || c->ray_depth < 1) {
                return MOCATRA_INVCONFIG;
        }

        return MOCATRA_OK;
}

#endif /* MOCATRA_CONFIG_H */

