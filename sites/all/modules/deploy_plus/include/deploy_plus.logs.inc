<?php

/**
 * @file
 * Deploy Plus Logs.
 */

/**
 * Page callback for logs page.
 */
function drupal_plus_logs($plan) {
  $output = '';
  $table = '';

  $data = _drupal_plus_logs_data($plan->name);

  if (!empty($data)) {
    $msg = t('Shown latest up to 50 records.');

    $header = array(
      array('data' => '#', 'width' => '2%'),
      array('data' => t('Date'), 'width' => '12%'),
      array('data' => t('Status'), 'width' => '15%'),
      array('data' => t('Message'), 'width' => '70%'),
    );
    $rows = array();
    foreach ($data as $index => $row) {
      $status_info = deploy_status_info($row->status);
      $status = '';
      if ($status_info) {
        $status .= '<div class="messages ' . $status_info['class'] . '">';
        $status .= $status_info['title'];
        $status .= '</div>';
      }

      $rows[] = array(
        $index + 1,
        format_date($row->timestamp),
        $status,
        $row->message,
      );
    }
    $table = theme('table', array(
      'header' => $header,
      'rows' => $rows,
    ));
  }
  else {
    $msg = t('No data were found.');
  }

  $output .= theme('deploy_plus_plan_configuration', array('plan' => $plan));

  $output .= theme('html_tag', array(
    'element' => array('#tag' => 'div', '#value' => $msg),
  ));

  $output .= $table;

  return $output;
}

/**
 * Get log data for given plan.
 *
 * @param string $plan_name
 *   Plan machine name.
 *
 * @return array
 *   Deploy log data array.
 */
function _drupal_plus_logs_data($plan_name) {
  $data = array();

  $query = db_select('deploy_log', 'l');
  $query->join('deploy_deployments', 'd', 'd.lid = l.lid');

  $data = $query
    ->fields('l', array('status', 'timestamp', 'message'))
    ->condition('d.plan_name', $plan_name)
    ->orderBy('l.timestamp', 'DESC')
    ->range(0, 50)
    ->execute()
    ->fetchAll();

  return $data;
}
