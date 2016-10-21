<?php

/**
 * @file
 * Back button pane.
 */

$plugin = array(
  'single' => TRUE,
  'title' => t('Back button'),
  'category' => t('OWN Category'),
  'render callback' => 'back_button_render_pane',
);

/**
 * Render callback for Back button pane.
 */
function back_button_render_pane($subtype, $conf, $args, $contexts) {
  $block = new stdClass();
  $block->content = theme('back_button');

  return $block;
}
