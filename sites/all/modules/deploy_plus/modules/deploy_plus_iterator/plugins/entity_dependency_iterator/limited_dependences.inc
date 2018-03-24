<?php

/**
 * @file
 * Limited Dependences plugin class definition.
 */

/**
 * Iterator class for when there are no dependencies to iterate through.
 */
class LimitedDependences extends DeployIterator {

  /**
   * Duplicate array of dependencies to the entity being parsed.
   *
   * @var array
   */
  public $deps = array();

  /**
   * Curently deployed entity.
   *
   * @var array
   */
  public $current = array(
    'type' => '',
    'id' => '',
  );

  /**
   * Get allowed to deploy list of dependencies.
   */
  public function getAllowed() {
    return variable_get('deploy_plus_iterator_limited_dependency_conf', array('file' => 'file'));
  }

  /**
   * Remove duplicate dependencies.
   */
  public function filterOutDuplicatesDependencies() {
    $dep = array();
    foreach ($this->dependencies as $entity) {
      $dep[$entity['type'] . '-' . $entity['id']] = $entity;
    }
    $this->dependencies = array();
    foreach ($dep as $entity) {
      $this->dependencies[] = $entity;
    }
  }

  /**
   * Helper method to remove translations form dependencies.
   */
  public function filterOutTranslation() {
    // Check if nid of 'node' dependency equal to tnid of current entity,
    // if yes - remove it.
    $dependencies = $this->deps;
    $this->deps = array();

    // Get translation nid of current entity.
    $tnid = db_select('node', 'n')
      ->fields('n', array('tnid'))
      ->condition('nid', $this->current['id'])
      ->execute()
      ->fetchField();

    foreach ($dependencies as $item) {
      if ($item['type'] == 'node' && $item['id'] == $tnid) {
        continue;
      }
      $this->deps[] = $item;
    }
  }

  /**
   * Helper method to get entity dependencies.
   */
  public function getChildrenEntities() {
    $entities = array();
    $this->current = current($this->entities);

    if (!empty($this->dependencies)) {
      // Filter out duplicates.
      $this->filterOutDuplicatesDependencies();

      // Remove all dependencies exept files.
      $this->deps = $this->dependencies;
      $this->dependencies = array();
      $allow = $this->getAllowed();

      switch ($this->current['type']) {
        case 'node':
          // Remove translation from dependencies, if needed. As they are in
          // type 'node' separate validation required.
          if (isset($allow['i18n_translation']) && $allow['i18n_translation'] == '0') {
            $this->filterOutTranslation();
          }
          break;

        case 'menu_link':
          // If current item is menu_link and menu_link is not in skip list -
          // add referenced in current menu_link node to the dependencies in
          // any case.
          // TODO: add same for other entities (e.g. terms?).
          $link = menu_link_load($this->current['id']);
          if ($link) {
            foreach ($this->deps as $item) {
              if ($item['type'] == 'node' && "node/{$item['id']}" == $link['link_path']) {
                $this->dependencies[] = $item;
              }
            }
          }
          break;

        default:
          break;
      }

      // Leave only selected in iterator cofiguration content types.
      foreach ($this->deps as $item) {
        if (isset($allow[$item['type']]) && $allow[$item['type']]) {
          $this->dependencies[] = $item;
        }
        else {
          switch ($item['type']) {
            case 'node':
              // Node are not allowed globally, check current node type.
              $node = node_load($item['id']);
              if ($node) {
                $key = 'node_' . $node->type;
                if (isset($allow[$key]) && $allow[$key]) {
                  $this->dependencies[] = $item;
                }
              }
              break;

            case 'taxonomy_term':
              // Taxonomy are not allowed globally, check current term
              // vocabulary.
              $term = taxonomy_term_load($item['id']);
              if ($term) {
                $key = 'taxonomy_term_' . $term->vocabulary_machine_name;
                if (isset($allow[$key]) && $allow[$key]) {
                  $this->dependencies[] = $item;
                }
              }
              break;

            default:
              // code...
              break;
          }
        }
      }

      if (!empty($this->dependencies)) {
        $entities = $this->dependencies;
      }
      // In an iterator, having children means that the current key itself
      // isn't a part of the entities. However, we need that entity.. So we add
      // the parent as a part of the entities. And since children always should
      // go first, we add the parent last.
      $entities[] = $this->current;
    }
    return $entities;
  }

  /**
   * {@inheritdoc}
   */
  public function getChildren() {
    return new LimitedDependences($this->getChildrenEntities(), $this);
  }

}
