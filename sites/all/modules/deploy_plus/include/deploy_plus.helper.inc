<?php

/**
 * @file
 * Deploy Plus helper function.
 */

/**
 * Get entity dependencies. Support nodes and menu links for now.
 *
 * @param object $entity
 *   Entity object.
 * @param string $type
 *   Entity type.
 *
 * @return array
 *   Array of entities links.
 */
function deploy_plus_get_entity_dependencies($entity, $type) {
  $dependencies = array(
    'raw' => array(),
    'processed' => array(),
  );

  $entity_dependencies = module_invoke_all('entity_dependencies', $entity, $type);
  // This is how we make sure to show a dependency only once.
  $traversed = array();

  foreach ($entity_dependencies as $dependency) {
    if (!empty($traversed[$dependency['type']][$dependency['id']])) {
      continue;
    }
    if (!isset($traversed[$dependency['type']])) {
      $traversed[$dependency['type']] = array();
    }

    $traversed[$dependency['type']][$dependency['id']] = 1;

    // Some ids are actually uuids, but others are serial ids.
    // We distinguish them by is_numeric check.
    if (is_numeric($dependency['id'])) {
      $dep_entity = entity_load($dependency['type'], array($dependency['id']));
    }
    else {
      $dep_entity = entity_uuid_load($dependency['type'], array($dependency['id']));
    }
    $dep_entity = reset($dep_entity);

    if ($dep_entity) {
      $dep_entity->entity_type = $dependency['type'];
      $dependencies['raw'][$dep_entity->uuid] = $dep_entity;

      $title = deploy_plus_build_entity_title($dependency['type'], $dep_entity);
      $title .= deploy_plus_build_entity_info($dependency['type'], $dep_entity);
      $label = deploy_plus_entity_type_label($dependency['type'], $dep_entity);
      $dependencies['processed'][$label][$dep_entity->uuid] = $title;
    }
  }

  return $dependencies;
}

/**
 * Helper function to build entity title.
 *
 * @param string $type
 *   Entity type.
 * @param object $entity
 *   Entity object.
 *
 * @return string
 *   URL strign.
 */
function deploy_plus_build_entity_title($type, $entity) {
  $title = entity_label($type, $entity);

  switch ($type) {
    case 'file':
      $url = is_array($entity) ? $entity['uri'] : $entity->uri;
      $title = l($title, file_create_url($url), array(
        'attributes' => array('target' => '_blank'),
      ));
      break;

    case 'redirect':
      $title = l(
        t('Redirect #!rid', array('!rid' => $entity->rid)),
        'admin/config/search/redirect/edit/' . $entity->rid,
        array(
          'attributes' => array('target' => '_blank'),
          'html' => TRUE,
        )
      );
      break;

    default:
      $uri = entity_uri($type, $entity);
      if ($uri) {
        $uri['options']['attributes']['target'] = '_blank';
        $title = l($title, $uri['path'], $uri['options']);
      }
      break;
  }

  // Show add to plan link if this dependency is not in plan.
  // If this is a review page.
  if (NULL !== arg(3) && arg(3) && NULL !== arg(4) && arg(4) == 'review') {
    $plan = arg(3);
    $info = entity_get_info($type);
    $entity_id = $entity->{$info['entity keys']['id']};
    if (!deploy_plus_entity_in_plan($plan, $type, $entity_id)) {
      $add = l(
        t('add to plan'),
        "admin/structure/deploy/{$plan}/{$entity_id}/{$type}/add"
      );
      $title .= " <small><span class='admin-link'>{$add}</span></small>";
    }
  }

  return $title;
}

/**
 * Helper function to build entity info.
 *
 * @param string $type
 *   Entity type.
 * @param object $entity
 *   Entity object.
 *
 * @return string
 *   HTML strign.
 */
function deploy_plus_build_entity_info($type, $entity) {
  $output = array();

  $items = array();
  $ids = array(
    'nid',
    'tid',
    'rid',
    'mlid',
    'fid',
    'item_id',
    'field_name',
    'uid',
    'uuid',
    'link_path',
    'menu_name',
    'vocabulary_machine_name',
    'source',
    'redirect',
    'changed',
  );
  foreach ($ids as $key) {
    $value = FALSE;
    if (is_object($entity) && isset($entity->{$key})) {
      $value = $entity->{$key};
    }
    elseif (is_array($entity) && isset($entity[$key])) {
      $value = $entity[$key];
    }
    if ($value !== FALSE) {
      $add_item = TRUE;
      switch ($key) {
        case 'menu_name':
          $menus = menu_get_menus();
          $value = $menus[$value];
          break;

        case 'vocabulary_machine_name':
          $vocabulary = taxonomy_vocabulary_machine_name_load($value);
          $value = $vocabulary->name;
          $key = 'vocabulary_name';
          break;

        case 'source':
          $key = t('From');
          break;

        case 'redirect':
          $key = t('To');
          $value = $value == '' ? '<front>' : $value;
          break;

        case 'uid':
          $add_item = ($value !== '0');
          break;

        case 'changed':
          $key = t('Last updated');
          $value = format_date($value, 'short');
          break;

        default:
          break;
      }
      if ($add_item) {
        $items[] = t('@key: @value', array('@key' => $key, '@value' => $value));
      }
    }
  }

  if (!empty($items)) {
    $output = theme('item_list', array('items' => $items));
  }

  return $output;
}

/**
 * Get entity type label.
 *
 * @param string $type
 *   Entity type.
 * @param object $entity
 *   Entity object.
 *
 * @return string
 *   Entity type human readable name.
 */
function deploy_plus_entity_type_label($type, $entity) {
  $label = '';
  if ($type == 'node') {
    $wrapped = entity_metadata_wrapper($type, $entity);
    $label = $wrapped->type->label();
  }
  else {
    $info = entity_get_info($type);
    $label = $info['label'];
  }
  return $label;
}

/**
 * Entity edit form submit callback to add entity to deployment plan.
 */
function _deploy_plus_form_entity_edit_form_submit(&$form, &$form_state) {
  if (isset($form_state['values']['deploy_managed_ui']['plans'])) {
    $entity_type = FALSE;
    if (isset($form['#entity_type'])) {
      $entity_type = $form['#entity_type'];
    }
    elseif (isset($form['type']['#value'])) {
      $entity_type = $form['type']['#value'];
    }
    if ($entity_type) {
      $info = entity_get_info($entity_type);
      if (isset($form_state['values'][$info['entity keys']['id']])) {
        $entity = entity_load_single($entity_type, $form_state['values'][$info['entity keys']['id']]);
        foreach ($form_state['values']['deploy_managed_ui']['plans'] as $plan) {
          if ($plan) {
            deploy_manager_add_to_plan($plan, $entity_type, $entity);
          }
        }
      }
    }
  }
}

/**
 * Get array of dependecies for selected plan.
 *
 * @param string $plan_name
 *   Deployment plan name.
 * @param string $entity_type
 *   (optional) Entity type to return, by default all.
 *
 * @return array
 *   Array of dependecies or empty.
 */
function deploy_plus_plan_entity_dependency($plan_name, $entity_type = 'all') {
  $dependecy = array();

  $deploy_plan = deploy_plan_load($plan_name);
  $entity_keys = $deploy_plan->getEntities();
  foreach ($entity_keys as $entity_key) {
    // Get the entity info and all entities of this type.
    $entity_info = entity_get_info($entity_key['type']);

    $entity = entity_load_single($entity_key['type'], $entity_key['id']);
    if (empty($entity_info['entity keys']['revision'])) {
      $entity = entity_load_single($entity_key['type'], $entity_key['id']);
    }
    else {
      $entity = entity_revision_load($entity_key['type'], $entity_key['revision_id']);
    }

    // Construct a usable array for the theme function.
    $dependencies = deploy_plus_get_entity_dependencies($entity, $entity_key['type']);
    $dependecy += $dependencies['raw'];
  }

  // Remove items that are already in the plan.
  foreach ($dependecy as $key => $d) {
    $info = entity_get_info($d->entity_type);
    if (deploy_plus_entity_in_plan($plan_name, $d->entity_type, $d->{$info['entity keys']['id']})) {
      unset($dependecy[$key]);
    }
  }

  if ($entity_type != 'all') {
    foreach ($dependecy as $key => $d) {
      if ($d->entity_type != $entity_type) {
        unset($dependecy[$key]);
      }
    }
  }

  return $dependecy;
}

/**
 * Check if given entity type in dependencie list.
 *
 * @param string $plan_name
 *   Deployment plan name.
 * @param string $entity_type
 *   Entity type to check.
 *
 * @return bool
 *   TRUE is entity present in the dependencies array, FALSE otherwise.
 */
function deploy_plus_entity_type_in_dependency($plan_name, $entity_type) {
  $in = FALSE;

  $dependency = deploy_plus_plan_entity_dependency($plan_name);

  foreach ($dependency as $value) {
    if ($value->entity_type == $entity_type) {
      $in = TRUE;
      break;
    }
  }

  return $in;
}

/**
 * Check if given entity already added to plan.
 *
 * @param string $plan_name
 *   Deployment plan name.
 * @param string $entity_type
 *   Entity type to check.
 * @param string $entity_id
 *   Entity id to check.
 *
 * @return bool
 *   TRUE if entity present in the plan.
 */
function deploy_plus_entity_in_plan($plan_name, $entity_type, $entity_id) {
  $in = FALSE;

  $plan = deploy_plan_load($plan_name);
  $entity_keys = $plan->getEntities();

  foreach ($entity_keys as $value) {
    if ($value['type'] == $entity_type && $value['id'] == $entity_id) {
      $in = TRUE;
      break;
    }
  }

  return $in;
}

/**
 * Get plan configuration info.
 *
 * @param object $plan
 *   Deployment plan.
 *
 * @return array
 *   Plan configuration info.
 */
function deploy_plus_helper_plan_info($plan) {
  $info = array(
    'dependency_plugin' => array(
      'config' => FALSE,
    ),
    'endpoints' => array(),
  );

  if ($plan->dependency_plugin) {
    $plugins = entity_dependency_get_all_ctools_plugins();
    $plugin_info = deploy_plus_dependency_plugin_info_load($plan->dependency_plugin);
    if (isset($plugin_info['title callback']) && function_exists($plugin_info['title callback'])) {
      $info['dependency_plugin']['title'] = call_user_func($plugin_info['title callback']);
    }
    else {
      $info['dependency_plugin']['title'] = $plugins[$plan->dependency_plugin];
    }
    if ($plugin_info['config_form']) {
      $info['dependency_plugin']['config'] = DEPLOY_PLUS_ITERATORS_PATH . "/{$plan->dependency_plugin}/config";
    }
  }

  foreach ($plan->endpoints as $name) {
    $endpoint = deploy_endpoint_load($name);
    if ($endpoint) {
      $info['endpoints'][] = $endpoint;
    }
  }

  return $info;
}
