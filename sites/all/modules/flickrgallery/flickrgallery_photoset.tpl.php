<?php
/*
 * Available vars:
 * $set: containing the thumbnail images (Output from flickrgallery_photo.tpl.php)
 * $meta: more information about the set
 */
?>
<div id="flickrgallery">
  <?php foreach ($photoset as $key => $set) : ?>
    <!-- in flickrgallery_photoset.tpl.php, printing $set BEGIN -->
    <div class='flickr-wrap'>
      <?php print $set; ?>
    </div>
    <!-- in flickrgallery_photoset.tpl.php, printing $set END -->
  <?php endforeach; ?>
  <div class='flickrgallery-return'><?php print l(t('Back to sets'), variable_get('flickrgallery_path', 'flickr')); ?></div>
</div>
