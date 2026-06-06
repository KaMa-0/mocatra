/*
 *
 * MOCATRA configuration file
 * 
 *
 * */


char* output_image_path   = "mocatra_image.ppm"; /* relative path to output  */
float image_aspect_ratio  = 1.0f / 1.0f;         /* must be float value      */
int   image_width         = 600;                 /* width of output in pixel */
int   samples_per_pixel   = 100;                 /* number of samples/pixel  */
int   maximum_ray_depth   = 20;                  /* iteration depth for ray  */
float camera_focal_length = 1.0f;                /* focal length of camera   */


/* -----------------------------------------
 *  NOTE:
 *
 *      The output path to the final image must be a relative path with this 
 *      directory (project root) being the base. If a new folder is created 
 *      for storing the images, such as "images" it must be within this 
 *      project directory. There is no checking if the path exists or not.
 *
 *      Only image width is required, because the image height will be 
 *      consequently derived from it via the aspect ration.
 *
 *      The number of samples per pixel will and the maximum depth for the 
 *      ray recursion will both be crucial as to how much/little noise the 
 *      final render has and how true to life the light and shadows are.
 *      Playing around with these values can give interesting insights into 
 *      how the pathtracer works and how the different configurations affect 
 *      the final render.
 *
 * -----------------------------------------
 * */
