<?php

/**
 * @file
 * Create button pane.
 */

$plugin = array(
  'single' => TRUE,
  'title' => t('Button(s)'),
  'description' => t('Create both create and(or) edit buttons.'),
  'category' => t('OWN Category'),
  'render callback' => 'create_button_render_pane',
  'edit form' => 'create_button_edit_form',
  'defaults' => array(
    'create_button_variants' => '',
    'create_button_create_title' => '',
    'create_button_update_title' => '',
    'create_button_type' => '',
  ),
  'required context' => new ctools_context_optional(t('Node'), 'node'),
  'no title override' => TRUE,
);

/**
 * Form constructor.
 */
function create_button_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  $form['create_button_variants'] = array(
    '#title' => t('Button(s)'),
    '#type' => 'select',
    '#options' => array(
      'create' => t('Create button'),
      'update' => t('Edit button'),
      'both' => t('Create and edit buttons'),
    ),
    '#default_value' => $conf['create_button_variants'],
  );

  $form['create_button_create_title'] = array(
    '#title' => t('Create button title'),
    '#type' => 'textfield',
    '#default_value' => $conf['create_button_create_title'],
    '#states' => array(
      'visible' => array(
        'select[name="create_button_variants"]' => array(
          array('value' => 'create'),
          array('value' => 'both'),
        ),
      ),
    ),
  );

  $form['create_button_update_title'] = array(
    '#title' => t('Edit button title'),
    '#type' => 'textfield',
    '#default_value' => $conf['create_button_update_title'],
    '#states' => array(
      'visible' => array(
        'select[name="create_button_variants"]' => array(
          array('value' => 'update'),
          array('value' => 'both'),
        ),
      ),
    ),
  );

  $form['create_button_type'] = array(
    '#title' => t('Node type'),
    '#type' => 'select',
    '#options' => array(
      'post' => t('Post'),
      'event' => t('Event'),
      'page' => t('Basic-page'),
    ),
    '#default_value' => $conf['create_button_type'],
  );

  return $form;
}

/**
 * Form submit callback.
 */
function create_button_edit_form_submit($form, &$form_state) {
  form_state_values_clean($form_state);
  $form_state['conf'] = $form_state['values'];
}

/**
 * Render callback for Create Button plugin.
 */
function create_button_render_pane($subtype, $conf, $args, $contexts) {
  $block = new stdClass();

  $variables = array(
    'buttons' => _create_button_get_buttons(
      $conf['create_button_type'],
      $conf['create_button_create_title'],
      $conf['create_button_update_title'],
      $conf['create_button_variants'],
      $contexts
    )
  );

  $block->content = theme('create_button', $variables);

  return $block;
}

/**
 * Helper function.
 * Return filled buttons array that will be passed to theme function.
 */
function _create_button_get_buttons($type, $title_create, $title_update, $variant, $contexts) {
  $buttons = array();

  $default_anchor_attributes = array(
    'class' => array('btn', 'btn--primary', 'ajaxToSub'),
  );

  $default_span_attributes = array(
    'class' => array('iconic', 'iconic-sm'),
    'data-glyph' => 'pencil',
    'title' => $type,
    'aria-hidden' => 'true',
  );

  switch ($variant) {
    case 'both':
      _create_button_create(
        $title_create,
        $type,
        $default_anchor_attributes,
        $default_span_attributes,
        $buttons
      );

      _create_button_update(
        $contexts,
        $title_update,
        $type,
        $default_anchor_attributes,
        $default_span_attributes,
        $buttons
      );
      break;

    case 'create':
      _create_button_create(
        $title_create,
        $type,
        $default_anchor_attributes,
        $default_span_attributes,
        $buttons
      );
      break;

    case 'update':
      _create_button_update(
        $contexts,
        $title_update,
        $type,
        $default_anchor_attributes,
        $default_span_attributes,
        $buttons
      );

      break;
  }

  return $buttons;
}

/**
 * Helper function.
 * Check whether user has permission to create a node of certain type
 * and fill buttons array with proper info.
 */
function _create_button_create($title_create, $type, $default_anchor_attributes, $default_span_attributes, &$buttons) {
  $info = _create_button_determine_href($type);

  if (node_access('create', $info['node_type'])) {
    $buttons['create']['title'] = $title_create;
    $buttons['create']['anchor_attributes']['class'][] = "btn--create-$type";
    $buttons['create']['anchor_attributes']['href'] = url($info['href'], array('absolute' => TRUE));

    $anchor_attributes = array_merge_recursive(
      $default_anchor_attributes,
      $buttons['create']['anchor_attributes']
    );
    $buttons['create']['anchor_attributes'] = drupal_attributes($anchor_attributes);
    $buttons['create']['span_attributes'] = drupal_attributes($default_span_attributes);
  }
}

/**
 * Helper function.
 * Check whether user has permission to edit current node
 * and fill buttons array with proper info.
 */
function _create_button_update($contexts, $title_update, $type, $default_anchor_attributes, $default_span_attributes, &$buttons) {
  if ($contexts->keyword == 'node' && node_access('update', $contexts->data)) {
      $href = '/node/' . $contexts->data->nid . '/edit';
      $buttons['update']['title'] = $title_update;
      $buttons['update']['anchor_attributes']['class'][] = "btn--edit-$type";
      $buttons['update']['anchor_attributes']['href'] = url($href, array('absolute' => TRUE));
      $buttons['update']['span_attributes']['class'] = array('iconic');
      $buttons['update']['span_attributes']['data-glyph'] = 'cog';

      $anchor_attributes = array_merge_recursive(
        $default_anchor_attributes,
        $buttons['update']['anchor_attributes']
      );
      $buttons['update']['anchor_attributes'] = drupal_attributes($anchor_attributes);

      $span_attributes = array_merge(
        $default_span_attributes,
        $buttons['update']['span_attributes']
      );
      $buttons['update']['span_attributes'] = drupal_attributes($span_attributes);
  }
}

/**
 * Helper function.
 * Get proper node type and url of certain type pointed to the
 * node creation form.
 */
function _create_button_determine_href($type) {
  $node_info = array();

  switch ($type) {
    case 'event':
      $node_info = array(
        'node_type' => $type,
        'href' => "node/add/$type",
      );
      break;

    case 'page':
      $node_info = array(
        'node_type' => 'basic_page',
        'href' => 'node/add/basic-page',
      );
      break;

    case 'post':
      $node_info = array(
        'node_type' => 'article',
        'href' => '/content/new',
      );
      break;
  }
  return $node_info;
}
