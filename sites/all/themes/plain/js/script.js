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
(function ($, Drupal, window, document, undefined) {


// To understand behaviors, see https://drupal.org/node/756722#behaviors
Drupal.behaviors.my_custom_behavior = {
  attach: function(context, settings) {

    // Place your code here.
    console.log('plain script.js');
    $('#navigation .menu > li a').each( function() {
    	if (!!!$(this).attr('href')) {
	    	console.log('men item: ' + $(this).text() + ', href:' + $(this).attr('href'));
	    	$(this).attr('href','javascript:void(0)');
	    }
    });

    $('.feedback-faq .views-field-title a').on('click', function(e) {
    	e.preventDefault();
    	if ($(this).closest('.views-field').next('.views-field').is(':visible')) {
	    	$(this).closest('.views-field').next('.views-field').slideUp();
    	} else {
	    	$(this).closest('.views-field').next('.views-field').slideDown();
	    }
    });
  }
};


})(jQuery, Drupal, this, this.document);
