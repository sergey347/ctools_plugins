<?php

/**
 * @file
 * Site logo pane.
 */

$plugin = array(
  'single' => TRUE,
  'title' => t('Site logo'),
  'category' => t('OWN Category'),
  'render callback' => 'site_logo_render_pane',
);

/**
 * Render callback for Site logo pane.
 */
function site_logo_render_pane($subtype, $conf, $args, $contexts) {
  $block = new stdClass();

  $variables = array(
    'to_frontpage' => url('<front>', array('absolute' => TRUE)),
    'logo_src' => 'sites/default/files/logo_example.svg',
  );

  $block->content = theme('site_logo', $variables);

  return $block;
}
