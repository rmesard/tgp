<?php
/*
 * Available vars:
 * $set: containing the thumbnail images (Output from flickrgallery_photo.tpl.php)
 * $meta: more information about the set
 */
?>
<div id="flickrgallery">
<div class="flickgallery-description red"><?php print $meta['description']; ?></div>
  <div class='flickrgallery-return'><?php print l(t('Back to Galleries'), variable_get('flickrgallery_path', 'flickr'), array('fragment' => 'set' . $meta['id'])); ?></div>
  <?php foreach ($photoset as $key => $set) : ?>
    <!-- in flickrgallery_photoset.tpl.php, printing $set BEGIN -->
    <div class='flickr-wrap'>
      <?php print $set; ?>
    </div>
    <!-- in flickrgallery_photoset.tpl.php, printing $set END -->
  <?php endforeach; ?>
  <div class='flickrgallery-return'><?php print l(t('Back to Galleries'), variable_get('flickrgallery_path', 'flickr'), array('fragment' => 'set' . $meta['id'])); ?></div>
  <!-- meta:  -->
</div>
