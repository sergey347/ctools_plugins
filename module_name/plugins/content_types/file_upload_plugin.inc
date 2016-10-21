<?php

/**
 * @file
 * File upload ctools panel pane.
 */

$plugin = array(
  'single' => TRUE,
  'title' => t('File upload'),
  'category' => t('OWN Category'),
  'render callback' => 'file_upload_render_pane',
  'edit form' => 'file_upload_edit_form',
  'no title override' => TRUE,
  'defaults' => array(
    'file_upload_title' => array(
      'value' => '',
      'format' => 'full_html',
    ),
    'file_upload_image' => '',
    'file_upload_body' => array(
      'value' => '',
      'format' => 'full_html',
    ),
    'file_upload_button' => array(
      'title' => '',
      'url' => '',
    ),
  ),
);

/**
 * Form constructor.
 */
function file_upload_edit_form($form, &$form_state) {
  ctools_form_include(
    $form_state,
    'file_upload',
    'module_name',
    'plugins/content_types'
  );

  $form['#attributes']['enctype'] = array('multipart/form-data');
  form_load_include($form_state, 'inc', 'file', 'file.field');

  $conf = $form_state['conf'];

  $form['file_upload_title'] = array(
    '#title' => t('Title'),
    '#type' => 'text_format',
    '#format' => $conf['file_upload_title']['format'],
    '#default_value' => $conf['file_upload_title']['value'],
    '#required' => TRUE,
  );

  $form['file_upload_image'] = array(
    '#type' => 'managed_file',
    '#title' => t('Image'),
    '#default_value' => $conf['file_upload_image'],
    '#field_name' => 'file_upload_image',
    '#upload_location' => 'public://',
    '#upload_validators' => array(
      'file_validate_size' => array(file_upload_max_size()),
      'file_validate_extensions' => array('png jpg jpeg'),
    ),
    '#required' => TRUE,
  );

  $form['file_upload_body'] = array(
    '#title' => t('Text'),
    '#type' => 'text_format',
    '#format' => $conf['file_upload_body']['format'],
    '#default_value' => $conf['file_upload_body']['value'],
    '#required' => TRUE,
  );

  $form['file_upload_button'] = array(
    '#tree' => TRUE,
    '#type' => 'link_field',
    '#title' => t('Button'),
    '#default_value' => array(
      'title' => $conf['file_upload_button']['title'],
      'url' => $conf['file_upload_button']['url'],
    ),
    '#field_name' => 'file_upload_button',
    '#language' => LANGUAGE_NONE,
    '#field_parents' => array('file_upload_button'),
    '#delta' => 0,
    '#required' => TRUE,
  );

  return $form;
}

/**
 * Form submit callback.
 */
function file_upload_edit_form_submit($form, &$form_state) {
  form_state_values_clean($form_state);
  $form_state['conf'] = $form_state['values'];

  // Load the file via file.fid.
  // We won`t check whether file.fid is not empty due to field is required.
  $file = file_load($form_state['values']['file_upload_image']);
  // Change status to permanent.
  $file->status = FILE_STATUS_PERMANENT;
  // Save.
  file_save($file);
}

/**
 * Render callback for Create Button plugin.
 */
function file_upload_render_pane($subtype, $conf, $args, $contexts) {
  $block = new stdClass();

  $image_markup = '';
  if ($file = file_load($conf['file_upload_image'])) {
    $image_markup = theme('image_style',
      array(
        'style_name' => 'large',
        'path' => $file->uri,
      )
    );
  }

  $variables = array(
    'file_upload_title' => $conf['file_upload_title']['value'],
    'file_upload_image' => $image_markup,
    'file_upload_body' => $conf['file_upload_body']['value'],
    'file_upload_button' => $conf['file_upload_button'],
  );

  $block->content = theme('file_upload', $variables);

  return $block;
}
