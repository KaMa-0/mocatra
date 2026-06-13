/*
 *
 * MOCATRA configuration file
 * 
 *
 * */


/* relative path to output  */
char* output_image_path   = "mocatra_image.ppm"; 

/* must be float value      */
float image_aspect_ratio  = 1.0f / 1.0f;         

/* width of output in pixel */
int   image_width         = 600;                 

/* number of samples/pixel  */
int   samples_per_pixel   = 1000;                

/* iteration depth for ray  */
int   maximum_ray_depth   = 50;                  

/* focal length of camera, must be float value  */
float camera_focal_length = 1.0f;                


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
