<?php
/**
 * @file
 * Contains the theme's functions to manipulate Drupal's default markup.
 *
 * A QUICK OVERVIEW OF DRUPAL THEMING
 *
 *   The default HTML for all of Drupal's markup is specified by its modules.
 *   For example, the comment.module provides the default HTML markup and CSS
 *   styling that is wrapped around each comment. Fortunately, each piece of
 *   markup can optionally be overridden by the theme.
 *
 *   Drupal deals with each chunk of content using a "theme hook". The raw
 *   content is placed in PHP variables and passed through the theme hook, which
 *   can either be a template file (which you should already be familiary with)
 *   or a theme function. For example, the "comment" theme hook is implemented
 *   with a comment.tpl.php template file, but the "breadcrumb" theme hooks is
 *   implemented with a theme_breadcrumb() theme function. Regardless if the
 *   theme hook uses a template file or theme function, the template or function
 *   does the same kind of work; it takes the PHP variables passed to it and
 *   wraps the raw content with the desired HTML markup.
 *
 *   Most theme hooks are implemented with template files. Theme hooks that use
 *   theme functions do so for performance reasons - theme_field() is faster
 *   than a field.tpl.php - or for legacy reasons - theme_breadcrumb() has "been
 *   that way forever."
 *
 *   The variables used by theme functions or template files come from a handful
 *   of sources:
 *   - the contents of other theme hooks that have already been rendered into
 *     HTML. For example, the HTML from theme_breadcrumb() is put into the
 *     $breadcrumb variable of the page.tpl.php template file.
 *   - raw data provided directly by a module (often pulled from a database)
 *   - a "render element" provided directly by a module. A render element is a
 *     nested PHP array which contains both content and meta data with hints on
 *     how the content should be rendered. If a variable in a template file is a
 *     render element, it needs to be rendered with the render() function and
 *     then printed using:
 *       <?php print render($variable); ?>
 *
 * ABOUT THE TEMPLATE.PHP FILE
 *
 *   The template.php file is one of the most useful files when creating or
 *   modifying Drupal themes. With this file you can do three things:
 *   - Modify any theme hooks variables or add your own variables, using
 *     preprocess or process functions.
 *   - Override any theme function. That is, replace a module's default theme
 *     function with one you write.
 *   - Call hook_*_alter() functions which allow you to alter various parts of
 *     Drupal's internals, including the render elements in forms. The most
 *     useful of which include hook_form_alter(), hook_form_FORM_ID_alter(),
 *     and hook_page_alter(). See api.drupal.org for more information about
 *     _alter functions.
 *
 * OVERRIDING THEME FUNCTIONS
 *
 *   If a theme hook uses a theme function, Drupal will use the default theme
 *   function unless your theme overrides it. To override a theme function, you
 *   have to first find the theme function that generates the output. (The
 *   api.drupal.org website is a good place to find which file contains which
 *   function.) Then you can copy the original function in its entirety and
 *   paste it in this template.php file, changing the prefix from theme_ to
 *   STARTERKIT_. For example:
 *
 *     original, found in modules/field/field.module: theme_field()
 *     theme override, found in template.php: STARTERKIT_field()
 *
 *   where STARTERKIT is the name of your sub-theme. For example, the
 *   zen_classic theme would define a zen_classic_field() function.
 *
 *   Note that base themes can also override theme functions. And those
 *   overrides will be used by sub-themes unless the sub-theme chooses to
 *   override again.
 *
 *   Zen core only overrides one theme function. If you wish to override it, you
 *   should first look at how Zen core implements this function:
 *     theme_breadcrumbs()      in zen/template.php
 *
 *   For more information, please visit the Theme Developer's Guide on
 *   Drupal.org: http://drupal.org/node/173880
 *
 * CREATE OR MODIFY VARIABLES FOR YOUR THEME
 *
 *   Each tpl.php template file has several variables which hold various pieces
 *   of content. You can modify those variables (or add new ones) before they
 *   are used in the template files by using preprocess functions.
 *
 *   This makes THEME_preprocess_HOOK() functions the most powerful functions
 *   available to themers.
 *
 *   It works by having one preprocess function for each template file or its
 *   derivatives (called theme hook suggestions). For example:
 *     THEME_preprocess_page    alters the variables for page.tpl.php
 *     THEME_preprocess_node    alters the variables for node.tpl.php or
 *                              for node--forum.tpl.php
 *     THEME_preprocess_comment alters the variables for comment.tpl.php
 *     THEME_preprocess_block   alters the variables for block.tpl.php
 *
 *   For more information on preprocess functions and theme hook suggestions,
 *   please visit the Theme Developer's Guide on Drupal.org:
 *   http://drupal.org/node/223440 and http://drupal.org/node/1089656
 */


/**
 * Override or insert variables into the maintenance page template.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("maintenance_page" in this case.)
 */
/* -- Delete this line if you want to use this function
function ga_preprocess_maintenance_page(&$variables, $hook) {
  // When a variable is manipulated or added in preprocess_html or
  // preprocess_page, that same work is probably needed for the maintenance page
  // as well, so we can just re-use those functions to do that work here.
  ga_preprocess_html($variables, $hook);
  ga_preprocess_page($variables, $hook);
}
// */

/**
 * Override or insert variables into the html templates.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("html" in this case.)
 */
/* -- Delete this line if you want to use this function
function ga_preprocess_html(&$variables, $hook) {
  $variables['sample_variable'] = t('Lorem ipsum.');

  // The body tag's classes are controlled by the $classes_array variable. To
  // remove a class from $classes_array, use array_diff().
  //$variables['classes_array'] = array_diff($variables['classes_array'], array('class-to-remove'));
}
// */

/**
 * Override or insert variables into the page templates.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("page" in this case.)
 */
/* -- Delete this line if you want to use this function
function ga_preprocess_page(&$variables, $hook) {
  $variables['sample_variable'] = t('Lorem ipsum.');
}
// */

/**
 * Override or insert variables into the node templates.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("node" in this case.)
 */
/* -- Delete this line if you want to use this function
function ga_preprocess_node(&$variables, $hook) {
  $variables['sample_variable'] = t('Lorem ipsum.');

  // Optionally, run node-type-specific preprocess functions, like
  // ga_preprocess_node_page() or ga_preprocess_node_story().
  $function = __FUNCTION__ . '_' . $variables['node']->type;
  if (function_exists($function)) {
    $function($variables, $hook);
  }
}
// */

/**
 * Override or insert variables into the comment templates.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("comment" in this case.)
 */
/* -- Delete this line if you want to use this function
function ga_preprocess_comment(&$variables, $hook) {
  $variables['sample_variable'] = t('Lorem ipsum.');
}
// */

/**
 * Override or insert variables into the region templates.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("region" in this case.)
 */
/* -- Delete this line if you want to use this function
function ga_preprocess_region(&$variables, $hook) {
  // Don't use Zen's region--sidebar.tpl.php template for sidebars.
  //if (strpos($variables['region'], 'sidebar_') === 0) {
  //  $variables['theme_hook_suggestions'] = array_diff($variables['theme_hook_suggestions'], array('region__sidebar'));
  //}
}
// */

/**
 * Override or insert variables into the block templates.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("block" in this case.)
 */
/* -- Delete this line if you want to use this function
function ga_preprocess_block(&$variables, $hook) {
  // Add a count to all the blocks in the region.
  // $variables['classes_array'][] = 'count-' . $variables['block_id'];

  // By default, Zen will use the block--no-wrapper.tpl.php for the main
  // content. This optional bit of code undoes that:
  //if ($variables['block_html_id'] == 'block-system-main') {
  //  $variables['theme_hook_suggestions'] = array_diff($variables['theme_hook_suggestions'], array('block__no_wrapper'));
  //}
}
// */

/* override flickrgallery */
// function ga_flickrgallery_set($set_id = NULL) {
function ga_flickrgallery_setxx($set_id = NULL) {
  // Require FlickrAPI.
  module_load_include('module', 'flickrapi');

  // Add CSS file.
  drupal_add_css(drupal_get_path('module', 'flickrgallery') . '/flickrgallery.css');

  // Create Flickr object.
  //$f = flickrapi_phpFlickr(); @todo fix when flickrapi supports private pictures -> token is missing
  $f = new phpFlickr(variable_get('flickrapi_api_key', NULL), variable_get('flickrapi_api_secret', NULL));

  // Because we're not using flickrapi to create the flickr object, we need to handle cache ourselves
  $cache = variable_get('flickrapi_cache', '');
  $cache_dir = variable_get('flickrapi_cache_path', '');
  
  if ($cache == TRUE) {
    $f->enableCache('fs', $cache_dir);
  }
  
  // Check for private pictures
  $token = variable_get('flickrgallery_token', NULL);
  $private = variable_get('flickrgallery_private_pictures', NULL);
  
  if (!empty($token) && $private == 1) {
    $f->setToken($token);
  }

  // Get Flickr set title.
  $set = $set_id['set_id'];
  $set_info = ($f->photosets_getInfo($set));

  // Set Flickr set title as page title.
  drupal_set_title($set_info['title'], 'Flickr Set');

  // Get Flickr photos for this set.
  $photos = $f->photosets_getPhotos($set,'tags');

  // Get META data for this set.
  $meta = $f->photosets_getInfo($set);

  // If there aren't any photo's, display message.
  if (empty($set) || empty($photos)) {
    drupal_set_message(t('This set doesn\'t exists or there aren\'t any pictures available for this set.'), 'error');
    drupal_not_found();
    exit();
  }

  // Get the type for Lightbox.
  $type = variable_get('flickrgallery_lightbox_type', 'lighbox2');

  // Declare variables.
  $photoset = array();
  $image_meta = array();

  foreach ($photos['photoset']['photo'] as $photo ) {
    if (variable_get('flickrgallery_display_type') == 1 && module_exists('image') && module_exists('imagecache_external')) {
      $original = $f->buildPhotoURL($photo, 'large');
      $img =  theme('imagecache_external', array('path' => $original, 'style_name'=> variable_get('flickrgallery_thumb_imagestyle', 'large'))); 
      
      $url_external = imagecache_external_generate_path($original, variable_get('flickrgallery_large_imagestyle', 'large'));
      $url = image_style_url(variable_get('flickrgallery_large_imagestyle', 'large'), $url_external);
    }
    else {
      $variables = array(
        'path' => $f->buildPhotoURL($photo, variable_get('flickrgallery_thumb', 'square')),
        'tags' => $photo['tags'],
        'alt' => $photo['title'] . " " . $photo['tags'],
        'title' => $photo['title'] . " " . $photo['tags'],
        'attributes' => array('class' => 'flickrgallery-set-image'),
      );
      $img = theme('image', $variables);
      $url = $f->buildPhotoURL ($photo, variable_get('flickrgallery_large', 'large'));
    } 

    $image = array();

    // Get META data for this image, only if flickrgallery_override is set to TRUE in the admin screen
    // This will lead to slower performance
    if (variable_get('flickrgallery_override') == TRUE) {
      $image_meta = $f->photos_getInfo($photo['id']);
    }

    $image['info'] = $photo;
    $image['image'] = l($img, $url, array('attributes' => array('class' => 'flickrgallery-image ' . $type, 'rel' => $type . "[flickrgallery]", 'title' => $photo['title'] . " " . $photo['tags']), 'html' => 'true'));
    $photoset[] = theme('flickrgallery_photo', array('image' => $image, 'image_meta' => $image_meta));
  }

  // Return the output.
  return theme('flickrgallery_photoset', array('photoset' => $photoset, 'meta' => $meta));
}
 


/* override collections display */
function ga_flickrgallery_wrapper_albums() {
  // Require FlickrAPI.
  module_load_include('module', 'flickrapi');

  // Add CSS file.
  drupal_add_css(drupal_get_path('module', 'flickrgallery') . '/flickrgallery.css');
  
  // Set custom title, only when viewing the page -> not when using the block because it will override every page title
  if (arg(0) == variable_get('flickrgallery_path', 'flickr')) {
    drupal_set_title(t(variable_get('flickrgallery_title', 'Flickr Gallery')));
  }

  // Create Flickr object.
  //$f = flickrapi_phpFlickr(); -> flickrapi does not support set token for private pictures
  // @todo, use flickrapi when it supports token for private pictures
  $f = new phpFlickr(variable_get('flickrapi_api_key', NULL), variable_get('flickrapi_api_secret', NULL));
  
  // Because we're not using flickrapi to create the flickr object, we need to handle cache ourselves
  $cache = variable_get('flickrapi_cache', '');
  $cache_dir = variable_get('flickrapi_cache_path', '');
  
  if ($cache == TRUE) {
    $f->enableCache('fs', $cache_dir);
  }
  
  // Check for private pictures
  $token = variable_get('flickrgallery_token', NULL);
  $private = variable_get('flickrgallery_private_pictures', NULL);
  
  if (!empty($token) && $private == 1) {
    $f->setToken($token);
  }
    
  // Get Flickr User info and User ID.
  $flickr_user = $f->people_getInfo(variable_get('flickrgallery_userID', NULL));
  $flickr_uid = $flickr_user['id'];
  

  // Get Flickr sets.
  if (variable_get('flickrgallery_displaysets_bool') == 1) {
    // If true, then select the flickrgallery_displaysets_values
    $array_set = explode("\n", trim(variable_get('flickrgallery_displaysets_values')));
    for ($j = 0; $j < count($array_set); $j++) {
      $sets['photoset'][$j] = $f->photosets_getInfo($array_set[$j]);
    }
  }
  else {
    $sets = $f->photosets_getList ($flickr_uid);
  }

  $description = t(variable_get('flickrgallery_description', NULL));
  $albums = array();
  $flickr_path = variable_get('flickrgallery_path', 'flickr');
  
  if (!empty($sets)) {
    foreach ($sets['photoset'] as $set) {
      if (variable_get('flickrgallery_display_type') == 1 && module_exists('image') && module_exists('imagecache_external')) {
        $original = "http://farm" . $set['farm'] . ".static.flickr.com/" . $set['server'] . "/" . $set['primary'] . "_" . $set['secret'] . "_b.jpg";
        $img =  theme('imagecache_external', array('path' => $original, 'style_name'=> variable_get('flickrgallery_albums_imagestyle', 'thumbnail'))); 
      }
      else {
        $thumb_url = "http://farm" . $set['farm'] . ".staticflickr.com/" . $set['server'] . "/" . $set['primary'] . "_" . $set['secret'] . "_" . variable_get('flickrgallery_albums', 's') . ".jpg";
        $variables = array(
          'path' => $thumb_url,
          'alt' => $set['title'],
          'title' => $set['title'],
          'name' => 'set' . $set['id'],
          'attributes' => array('class' => 'flickrgallery-set-image'),
        );
        $img = theme('image', $variables);
      }
      
      $image = array();
      $image['info'] = $set;
      $image['total'] = $set['photos'];
//      $image['image_link'] = l($img,  $flickr_path . "/set/" . $set['id'], array('attributes' => array('class' => array('flickrgallery'), 'title' => $set['title']), 'html' => 'true'));
      $image['image_link'] = l($img,  $flickr_path . "/set/" . $set['id'], array('attributes' => array('class' => array('flickrgallery'), 'name' => 'set' . $set['id'], 'title' => $set['title']), 'html' => 'true'));
      $image['title_link'] =  l($set['title'], $flickr_path . "/set/" . $set['id'], array('attributes' => array('class' => array('flickrgallery-title'), 'title' => $set['title']), 'html' => 'true'));

      $albums[] = $image;
    }
    return theme('flickrgallery_albums', array('description' => $description, 'albums' => $albums));
  }
  else {
    // If no sets, display msg.
    return "<h2>" . t("No pictures available") . "</h2>";
  }
}
