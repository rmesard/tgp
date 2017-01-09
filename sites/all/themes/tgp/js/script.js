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
    console.log('tgp script.js');
    $('#navigation .menu > li a').each( function() {
    	if (!!!$(this).attr('href')) {
	    	//console.log('men item: ' + $(this).text() + ', href:' + $(this).attr('href'));
	    	$(this).attr('href','javascript:void(0)');
	    }
    });

    $('.agency-list form select').on('change', function() {
        console.log('agency-list form select change, this: form:' + $(this).parents('form').attr('id') + ', name:' + $(this).attr('name') + ', val:' + $(this).val());
        //$(this).parents('form').find('select').not($(this)).prop('selectedIndex',0).parents('form').submit();
        $(this).parents('form').find('select').not($(this)).prop('selectedIndex',0);

    });

    if (!!$('.agency-list form select').length) {
        console.log('we have an agency form');
        $('.agency-list form select').each( function() {
            if ($(this).prop('selectedIndex') > 0) {
                $(this).closest('.view').find('.view-header h2, .view-empty .agency-region').html($(this).find('option:selected').text());
            }
        });
        //move any 'phone2' into 'phone'
        $('.field-name-field-phone2').each( function() {
            console.log('phone2: ' + $(this).find('.field-items').text());
            $(this).find('.field-items').appendTo($(this).siblings('.location-locations-display').find('.tel .value'));
        });
    }

    $('.feedback-faq .views-field-title a').on('click', function(e) {
    	e.preventDefault();
    	if ($(this).closest('.views-field').next('.views-field').is(':visible')) {
	    	$(this).closest('.views-field').next('.views-field').slideUp();
    	} else {
	    	$(this).closest('.views-field').next('.views-field').slideDown();
	    }
    });

    $('#page').on('click', function(event) {
        console.log('body click, target:' + $(event.target).attr('id'));
        if ($('body').hasClass('mobilenav')) {
            //console.log('body click, hasClass mobilenav, target:' + $(event.target).attr('id')); // + $(this).attr('class'));
            //console.log('not nav in parents? ' + (!!!$(event.target).parents('#navigation').length));
            if (!!!($(event.target).parents('#navigation').length)) {
                $('body').removeClass('mobilenav');
                //console.log('remove mobilenav from body');
            }
        }
    });

    $('#mobile-nav').on('click', function(e) {
        //console.log('mobile-nav click');
        e.preventDefault();
        e.stopPropagation();
        $('body').toggleClass('mobilenav');
    });

    //rearrange book review node, cherrypick fields from related book node
    $('.node-book-review').each( function() {
        $(this).find('header').first().after($(this).find('.field-name-field-subtitle'), $(this).find('.field-name-field-author'), $(this).find('.field-name-field-publisher'), $(this).find('.field-name-field-published-date'));
    });

    //if we linked to document library with a doc name in the URL hash, find the entry and make it stand out
    if (!!$('.document-library').length) { 
        var theHash = window.location.hash.replace('#','').toLowerCase();
        $('.document-library .views-row').each( function() {
            //console.log('hash: ' + theHash + ', filename: ' + $(this).find('.views-field-filename').text().trim());
            if (theHash === $(this).find('.views-field-filename').text().trim().toLowerCase()) {
                $(this).addClass('active');
            }
        });
    }
  }



};


})(jQuery, Drupal, this, this.document);
