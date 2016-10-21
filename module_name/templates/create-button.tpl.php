<?php
/**
 * Create button template.
 */
?>

<div class="create-edit-buttons">
  <?php foreach($buttons as $button): ?>
    <a <?php print $button['anchor_attributes']; ?>>
      <span <?php print $button['span_attributes']; ?>>
        <?php print $button['title']; ?>
      </span>
    </a>
  <?php endforeach; ?>
</div>
