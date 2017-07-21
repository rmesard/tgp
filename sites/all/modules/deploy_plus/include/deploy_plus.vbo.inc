<?php

/**
 * @file
 * Deploy Plus VBO function.
 */

/**
 * VBO action to perform for each entity.
 *
 * Used in node/term admin views and at admin/structure/menu/manage/%menu.
 *
 * @see deploy_plus_action_info()
 * @see deploy_plus_action_form_submit()
 */
function deploy_plus_plan_action($entity, $context) {
  // @see deploy.manager.inc
  // @see deploy_manager_add_to_plan()
  // @see deploy_manager_delete_from_plan()
  $function = "deploy_manager_{$context['operation']}_plan";
  if (function_exists($function)) {
    call_user_func($function, $context['plan'], $context['entity_type'], $entity);
  }
}

/**
 * Form to select action builder.
 *
 * @see deploy_plus_action_info()
 */
function deploy_plus_plan_action_form($context, $form_state) {
  $form = array();

  $plans = deploy_plan_load_all();

  $options = array();
  foreach ($plans as $name => $plan) {
    $options[t('Add to managed deployment plan')]["add_to:{$name}"] = $plan->title;
    $options[t('Remove from managed deployment plan')]["delete_from:{$name}"] = $plan->title;
  }

  $form['plan'] = array(
    '#type' => 'select',
    '#title' => t('Deploy operations'),
    '#options' => $options,
  );

  return $form;
}

/**
 * Submit handler for deploy_plus_plan_action_form().
 *
 * Entity add/delete to/from deploy plan submit.
 */
function deploy_plus_plan_action_submit(&$form, &$form_state) {
  $context = array();
  $action = explode(':', $form_state['values']['plan']);
  list($context['operation'], $context['plan']) = $action;

  return $context;
}

/**
 * Form to select action builder.
 *
 * @see deploy_plus_action_info()
 */
function deploy_plus_plan_action_add_form($context, $form_state) {
  $form = array();

  $plans = deploy_plan_load_all();

  $options = array();
  foreach ($plans as $name => $plan) {
    $options["add_to:{$name}"] = $plan->title;
  }

  $form['plan'] = array(
    '#type' => 'select',
    '#title' => t('Add to managed deployment plan'),
    '#options' => $options,
  );

  return $form;
}

/**
 * Submit handler for deploy_plus_plan_action_remove_form().
 *
 * Entity add to deploy plan submit.
 */
function deploy_plus_plan_action_add_submit(&$form, &$form_state) {
  return deploy_plus_plan_action_submit($form, $form_state);
}

/**
 * VBO action to perform for each entity.
 *
 * Used in node/term admin views and at admin/structure/menu/manage/%menu.
 *
 * @see deploy_plus_action_info()
 * @see deploy_plus_action_form_submit()
 */
function deploy_plus_plan_action_add($entity, $context) {
  deploy_plus_plan_action($entity, $context);
}

/**
 * Form to select action builder.
 *
 * @see deploy_plus_action_info()
 */
function deploy_plus_plan_action_remove_form($context, $form_state) {
  $form = array();

  $plans = deploy_plan_load_all();

  $options = array();
  foreach ($plans as $name => $plan) {
    $options["delete_from:{$name}"] = $plan->title;
  }

  $form['plan'] = array(
    '#type' => 'select',
    '#title' => t('Remove from managed deployment plan'),
    '#options' => $options,
  );

  return $form;
}

/**
 * Submit handler for deploy_plus_plan_action_remove_form().
 *
 * Entity delete from deploy plan submit.
 */
function deploy_plus_plan_action_remove_submit(&$form, &$form_state) {
  return deploy_plus_plan_action_submit($form, $form_state);
}

/**
 * VBO action to perform for each entity.
 *
 * Used in node/term admin views and at admin/structure/menu/manage/%menu.
 *
 * @see deploy_plus_action_info()
 * @see deploy_plus_action_form_submit()
 */
function deploy_plus_plan_action_remove($entity, $context) {
  deploy_plus_plan_action($entity, $context);
}
