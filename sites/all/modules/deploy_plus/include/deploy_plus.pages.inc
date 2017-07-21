<?php

/**
 * @file
 * Deploy Plus page functions.
 */

/**
 * Deployment plan review callback.
 */
function deploy_plus_review_plan($plan) {
  $variables = array(
    'info' => array(),
    'message' => NULL,
    'actions' => array(),
  );

  // Get the entity keys from the aggregator.
  $entity_keys = $plan->getEntities();
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
    $title = "{$entity_key['type']}:{$entity_key['id']}";
    $label = entity_label($entity_key['type'], $entity);
    if ($label) {
      $title = $label;
    }
    // @see deploy_plus.helper.inc
    $title = deploy_plus_build_entity_title($entity_key['type'], $entity);
    $title .= deploy_plus_build_entity_info($entity_key['type'], $entity);

    // Get entity url to build edit link.
    $uri = entity_uri($entity_key['type'], $entity);
    $edit = array();
    if ($uri) {
      $edit = array(
        'title' => t('Edit'),
        'href' => $uri['path'] . '/edit',
        'query' => array(
          'destination' => current_path(),
        ),
      );
    }

    // Construct a usable array for the theme function.
    $dependencies = deploy_plus_get_entity_dependencies($entity, $entity_key['type']);
    $variables['info'][] = array(
      'title' => $title,
      'language' => entity_language($entity_key['type'], $entity),
      'type' => $entity_info['label'],
      'entity_type' => deploy_plus_entity_type_label($entity_key['type'], $entity),
      'dependencies' => $dependencies['processed'],
      'operations' => array(
        array(
          'title' => t('Remove from plan'),
          'href' => "admin/structure/deploy/{$plan->name}/{$entity_key['id']}/{$entity_key['type']}/delete",
        ),
        $edit,
      ),
    );
  }

  // Check if there are any redirects in the dependencies that are not in
  // the plan.
  if (deploy_plus_entity_type_in_dependency($plan->name, 'redirect')) {
    // Validate that redirect entities not in plan, but in dependencies only.
    if (!empty(deploy_plus_plan_entity_dependency($plan->name, 'redirect'))) {
      $link = l(
        t('here'),
        "admin/structure/deploy/{$plan->name}/dependency/redirect/add"
      );
      $variables['message'] = array(
        'text' => t('In order to deploy redirects from dependency they should be added to the plan. Click !here to add redirects to plan.', array('!here' => $link)),
        'status' => 'warning',
      );
    }
  }

  $variables['actions'] = array(
    array(
      'title' => t('Go Back'),
      'href' => isset($_GET['return']) ? $_GET['return'] : 'admin/structure/deploy/overview',
    ),
    array(
      'title' => t('Deploy'),
      'href' => DEPLOY_PLUS_PLANS_PATH . "/{$plan->name}/deploy",
      'query' => array(
        'cancel_path' => current_path(),
      ),
    ),
  );

  // Title.
  $output = '<h2>' . check_plain($plan->title) . '</h2>';
  $output .= theme('deploy_plus_plan_configuration', array('plan' => $plan));
  $output .= theme('deploy_plus_overview_plan_page', $variables);
  return $output;
}

/**
 * Form constructor for adding/deleting entity to/from plan.
 */
function deploy_plus_manage_entity_plan_form($form, &$form_state, $deploy_plan, $entity_id, $type, $operation) {
  $form = array();

  $form['deploy_plan'] = array(
    '#type' => 'value',
    '#value' => $deploy_plan,
  );

  $entity = entity_load_single($type, $entity_id);
  $wentity = entity_metadata_wrapper($type, $entity);
  $entity_info = entity_get_info($type);

  $form['entity'] = array(
    '#type' => 'value',
    '#value' => $entity,
  );

  $form['item'] = array(
    '#type' => 'value',
    '#value' => $entity_info['label'] . ' "' . $wentity->label() . '"',
  );

  $form['type'] = array(
    '#type' => 'value',
    '#value' => $type,
  );

  $form['operation'] = array(
    '#type' => 'value',
    '#value' => $operation,
  );

  $markup = deploy_plus_build_entity_title($type, $entity);
  $markup .= deploy_plus_build_entity_info($type, $entity);
  $form['items'] = array('#markup' => $markup);

  if ($operation == 'delete') {
    $message = t('Are you sure you want to delete item below from deployment plan?');
    $button = t('Delete');
  }
  else {
    $message = t('Are you sure you want to add item below to deployment plan?');
    $button = t('Add');
  }

  $return = "admin/structure/deploy/{$deploy_plan->name}/review";

  return confirm_form($form, $message, $return, '', $button, t('Cancel'));
}

/**
 * Submit handler for adding/deleting entity to/from plan form.
 */
function deploy_plus_manage_entity_plan_form_submit(&$form, &$form_state) {
  $values = $form_state['values'];

  if ($values['operation'] == 'delete') {
    $operation = 'delete_from';
    $message = t('!item was deleted from !plan.', array(
      '!item' => $values['item'],
      '!plan' => $values['deploy_plan']->title,
    ));
  }
  else {
    $operation = 'add_to';
    $message = t('!item was deleted from !plan.', array(
      '!item' => $values['item'],
      '!plan' => $values['deploy_plan']->title,
    ));
  }

  // @see deploy.manager.inc
  // @see deploy_manager_add_to_plan()
  // @see deploy_manager_delete_from_plan()
  $function = "deploy_manager_{$operation}_plan";
  if (function_exists($function)) {
    call_user_func(
      $function,
      $values['deploy_plan']->name,
      $values['type'],
      $values['entity']
    );
    drupal_set_message($message);
  }

  $form_state['redirect'] = "admin/structure/deploy/{$values['deploy_plan']->name}/review";
}

/**
 * Page callback for the overview page.
 */
function deploy_plus_overview_page() {
  $plans = deploy_plan_load_all_enabled();
  $blocks = array();

  // Iterate over all plans.
  foreach ($plans as $plan) {
    $info = array();

    // Get the entity keys from the aggregator.
    $entity_keys = $plan->getEntities();
    foreach ($entity_keys as $entity_key) {
      // Get the entity info and all entities of this type.
      $entity_info = entity_get_info($entity_key['type']);

      if (!empty($entity_info['entity keys']['revision']) && !empty($entity_key['revision_id'])) {
        $entity = entity_revision_load($entity_key['type'], $entity_key['revision_id']);
      }
      else {
        $entity = entity_load_single($entity_key['type'], $entity_key['id']);
      }

      $title = "{$entity_key['type']}:{$entity_key['id']}";
      $label = entity_label($entity_key['type'], $entity);
      if ($label) {
        $title = $label;
      }

      if ($entity_info['entity keys']['revision'] && !empty($entity_key['revision_id'])) {
        $title = t('@title (rev:@rev_id)', array('@title' => $title, '@rev_id' => $entity_key['revision_id']));
      }

      $uri = entity_uri($entity_key['type'], $entity);
      if ($uri) {
        $title = l($title, $uri['path'], $uri['options']);
      }
      // Construct a usable array for the theme function.
      $info[] = array(
        'title' => $title,
        'type' => $entity_info['label'],
      );
    }

    // Construct a usable array for the theme function.
    $blocks[] = array(
      'plan_name' => check_plain($plan->name),
      'plan_title' => check_plain($plan->title),
      'plan_description' => check_plain($plan->description),
      'content' => theme('deploy_ui_overview_plan_content', array('info' => $info)),
      'fetch_only' => $plan->fetch_only,
      'status' => deploy_plan_get_status($plan->name),
    );
  }
  return theme('deploy_plus_overview', array('blocks' => $blocks));
}

/**
 * Add all entities of given type from dependencies to plan form.
 */
function deploy_plus_add_entity_type_plan_form($form, &$form_state, $deploy_plan, $entity_type) {
  $form = array();

  $dependency = deploy_plus_plan_entity_dependency($deploy_plan->name, $entity_type);

  $items = array();
  foreach ($dependency as $entity) {
    $title = deploy_plus_build_entity_title($entity->entity_type, $entity);
    $info = deploy_plus_build_entity_info($entity->entity_type, $entity);
    $items[] = $title . $info;
  }

  $form['deploy_plan'] = array(
    '#type' => 'value',
    '#value' => $deploy_plan,
  );

  $form['entity_type'] = array(
    '#type' => 'value',
    '#value' => $entity_type,
  );

  $form['dependency'] = array(
    '#type' => 'value',
    '#value' => $dependency,
  );

  $form['items'] = array(
    '#markup' => theme('item_list', array('items' => $items, 'type' => 'ol')),
  );

  return confirm_form(
    $form,
    t('Are you sure you want to add items below to deployment plan?'),
    "admin/structure/deploy/{$deploy_plan->name}/review",
    '',
    t('Yes'),
    t('Cancel')
  );
}

/**
 * Add all entities of given type from dependencies to plan form submit.
 */
function deploy_plus_add_entity_type_plan_form_submit(&$form, &$form_state) {
  $values = $form_state['values'];

  foreach ($values['dependency'] as $entity) {
    deploy_manager_add_to_plan(
      $values['deploy_plan']->name,
      $entity->entity_type,
      $entity
    );
  }

  drupal_set_message(
    t('All items were add to !plan.', array('!plan' => $values['deploy_plan']->title))
  );

  $form_state['redirect'] = "admin/structure/deploy/{$values['deploy_plan']->name}/review";
}
