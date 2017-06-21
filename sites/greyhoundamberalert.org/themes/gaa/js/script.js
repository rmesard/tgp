/**
 * @file
 * A JavaScript file for the theme.
 *
 * In order for this JavaScript to be loaded on pages, see the instructions in
 * the README.txt next to this file.
 */

// JavaScript should be made compatible with libraries other than jQuery by
// wrapping it with an "anonymous closure". See:
// - https://drupal.org/node/1446420
// - http://www.adequatelygood.com/2010/3/JavaScript-Module-Pattern-In-Depth
(function ($, Drupal, window, document) {

  'use strict';

  // To understand behaviors, see https://drupal.org/node/756722#behaviors
  Drupal.behaviors.my_custom_behavior = {
    attach: function (context, settings) {

      // Place your code here.

    }
  };

	Drupal.behaviors.webform = {
		attach: function(context, settings) {
			$('#edit-submitted-missing-from').blur(function() {
				console.log('edit-submitted-missing-from blurred, value:' + encodeURIComponent($(this).val()) + ', img: ' + $('.webform-component--missing-from-map img').attr('src'));
				$('input[name="submitted[missing_from_hidden]"]').val(encodeURIComponent($(this).val()));
				if (!!!$(this).val()) {
					$('.webform-component--missing-from-map img').slideUp();
				} else {
					$('.webform-component--missing-from-map img').attr('src', '//maps.googleapis.com/maps/api/staticmap?center='+encodeURIComponent($(this).val())+'&markers='+encodeURIComponent($(this).val())+'&size=450x275&zoom=17&scale=2&key=AIzaSyAHkISHTA7qMB6Gl9BsFksuVF_huCYFJBI').slideDown();
				}
			});
			$('#edit-submitted-dogs-name').blur(function() {
				console.log('edit-submitted-dogs-name blurred, value:' + encodeURIComponent($(this).val()));
				$('input[name="submitted[dogs_name_hidden]"]').val(encodeURIComponent($(this).val()));
			});
			$('#edit-submitted-contact-phone').blur(function() {
				console.log('edit-submitted-contact-phone blurred, value:' + encodeURIComponent($(this).val()));
				$('input[name="submitted[contact_phone_hidden]"]').val(encodeURIComponent($(this).val()));
			});
			$('#edit-submitted-contact-phone-2').blur(function() {
				console.log('edit-submitted-contact-phone-2 blurred, value:' + encodeURIComponent($(this).val()));
				$('input[name="submitted[contact_phone_2_hidden]"]').val(encodeURIComponent($(this).val()));
			});
		}
	};



})(jQuery, Drupal, this, this.document);
