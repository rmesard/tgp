<?php

/**
 * @file
 * Template for displaying the agenda in a block
 */

// Build some neat dates
$today = time() - time() % (3600 * 24);
//$dates[date('Y-m-d', $today)]         = t('Today');
//$dates[date('Y-m-d', $today - 86400)] = t('Yesterday');
//$dates[date('Y-m-d', $today + 86400)] = t('Tomorrow');

// List of keys to display
$keys    = array_map('trim', explode(',', $block->display_keys));
$nolabel = array_map('trim', explode(',', $block->hide_labels));
?>
<div class="agenda-block">
  <?php foreach ($events as $day): ?>
  <?php
  $date = $day[0]['start date'];

  // Substitute today/yesterday/tomorrow
//  if (isset($dates[$day[0]['when']])) {
//    $date = $dates[$day[0]['when']];
//  }
  ?>
  <p><!-- Date: <?php echo $date; ?> --></p>
  <ol>
  <?php foreach ($day as $event): ?>
    <li class="cal_<?php echo $event['index']; ?>">
      <span class="calendar_title"><?php echo $date . " - " . $event['title']; ?>
	<?php if (!empty($event['start time'])) echo " - " . $event['start time']; ?>
        </span>
      <ul class="moreinfo">

        <?php foreach ($keys as $key): ?>
          <?php if (!empty($event[$key])): ?>
		<!--  key : <?php echo $key; ?> -->
            <li class="moreinfo-<?php echo $key; ?>">
            <?php if (!in_array($key, $nolabel)): ?>
              <em><?php echo _agenda_translate($key); ?></em>:
            <?php endif; ?>
            <?php echo $event[$key]; ?>
            </li>
          <?php endif; ?>
        <?php endforeach; ?>

<?php
if (!empty($event['where'])) {
  echo "<li><a title='See on Map' target='_blank' href='http://maps.google.com/maps/dir//"; echo htmlentities($event['where'], ENT_QUOTES); echo "'>See on map</a></li>";
} 
?>
      </ul>
    </li>
  <?php endforeach; ?>
  </ol>
<?php endforeach; ?>
</div>
