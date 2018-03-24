<?php

/**
 * @file
 * Deploy Plus admin pages functions.
 */

/**
 * Title callback for iterator config page.
 *
 * @param array $iterator_info
 *   Iterator info (array) from the deploy_plus_dependency_plugin_info_load().
 *
 * @return string
 *   Iterator config page title.
 */
function deploy_plus_dependency_plugin_config_title(array $iterator_info) {
  return t('!iterator Settings', array('!iterator' => $iterator_info['title']));
}

/**
 * Dependency Interator Plugin settings page general callback.
 *
 * @param array $plugin
 *   Plugin info (array) from the deploy_plus_dependency_plugin_info_load().
 */
function deploy_plus_dependency_plugin_config(array $plugin) {
  return drupal_get_form($plugin['config_form'], $plugin);
}

/**
 * Callback for iterators list page.
 */
function drupal_plus_admin_iterators_page() {
  $output = t('No data available.');

  if (module_exists('ctools')) {
    ctools_include('plugins');
    $iterators = ctools_get_plugins('entity_dependency', 'iterator');
    $items = array();
    foreach ($iterators as $key => $iterator) {
      $title = $iterator['title'];

      if (isset($iterator['config_form']) && $iterator['config_form']) {
        $config = l(
          t('configure'),
          DEPLOY_PLUS_ITERATORS_PATH . "/{$key}/config",
          array('query' => array('destination' => current_path()))
        );
        $title .= " <small><span class='admin-link'>{$config}</span></small>";
      }
      $title .= "<div class='description'>{$iterator['description']}</div>";
      $items[] = $title;
    }
    $output = theme('item_list', array('items' => $items));
  }

  return $output;
}
