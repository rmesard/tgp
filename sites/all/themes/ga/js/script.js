/**
 * @file
 * A JavaScript file for the theme.
 *
 * In order for this JavaScript to be loaded on pages, see the instructions in
 * the README.txt next to this file.
 */

// JavaScript should be made compatible with libraries other than jQuery by
// wrapping it with an "anonymous closure". See:
// - http://drupal.org/node/1446420
// - http://www.adequatelygood.com/2010/3/JavaScript-Module-Pattern-In-Depth
(function ($, Drupal, window, document, undefined) {


// Place your code here.
/*
  Drupal.behaviors.agenda = {
    attach: function(context, settings) {
      $('.agenda-block > p').click(function () {
        //$(this).next('ol > li > .calendar_title > .moreinfo').toggle('show');
	console.log('hellow');
      });
    }
  };
*/

  Drupal.behaviors.agenda = {
    attach: function(context, settings) {
      $('body:not(".front") .agenda-block .calendar_title').click(function () {
        $(this).next('ul').toggle('show');
      });
      $('body:not(".front") .agenda-block > p').click(function () {
        $(this).next('ol').find('.moreinfo').toggle('show');
      });
    }
  };

  Drupal.behaviors.navigation = { 
  	attach: function(context, settings) { 
		$('body:not(".nav-open") body.nav-open #page, #block-block-10 a').live('click', function() { 
			$('body').toggleClass('nav-open');
		});
		$('#content .external a').each( function() { 
//			console.log(".external a, " + $(this).attr('href'));
			if (!$(this).attr('target')) $(this).attr('target','_blank');
		});
		$('#content a.external').each( function() { 
			if (!$(this).attr('target')) $(this).attr('target','_blank');
//			console.log("a.external, " + $(this).attr('href'));
		});
  	}
  };

	Drupal.behaviors.flickr = {
		attach: function(context, settings) { 
			//console.log("document :" + $(document));
			$('a[href*="flickr.com"]').each( function() { 
				if (!$(this).attr('target')) $(this).attr('target','flickr');
			});
			$('#content a[href^="http"]').each( function() { 
				if (!$(this).attr('target')) $(this).attr('target','_blank');
			});

			$('.flickr-citation').each( function() {
				// console.log("found flickr-cititation");
				if ($(this).prev().hasClass('flickr-photoset-img')) {
				//	console.log($(this).prev().find('a').attr('title'));
//					$(this).find('a').text('Click here to see pictures from ' + $(this).prev().find('a').attr('title'));
					$(this).find('a').text($(this).prev().find('a').attr('title'));
				}
			});
			var fromset = (window.location.hash.substr(1,3)=='set');
			if (fromset) var thefromset = window.location.hash.substr(1);
//			console.log("fromset = " + fromset);
			$('#flickrgallery-albums .flickr-wrap').each( function() {
				// if there's a botched retrieval from flickr, remove it from DOM to preserve layout
				// console.log("flickr-wrap , " + $(this).find('.flickrgallery-title').text());
				if ($(this).find('.flickrgallery-title').is(':empty')) $(this).remove();
				if (fromset) {
//					console.log('name attr = ' + $(this).find('a.flickrgallery').attr('name') + ', thefromset = ' + thefromset);
					if ($(this).find('a.flickrgallery').attr('name') == thefromset) $(this).addClass('hover fromset');
				}
			}); 
			$('#flickrgallery-albums .flickr-wrap a').hover( function() {
				$(this).parent().toggleClass('hover');
			});
			$('#flickrgallery-albums .flickr-wrap:first-child .flickrgallery').prepend('<span></span>');
//			$('#flickrgallery-albums .flickr-wrap:first-child .flickrgallery .flickrgallery-set-image').attr('src',$(this).attr('src').replace('_s.jpg','_z.jpg'));
//			$('#flickrgallery-albums .flickr-wrap:first-child .flickrgallery .flickrgallery-set-image').each( function() {
//				$(this).attr('src',$(this).attr('src').replace('_s.jpg','_n.jpg'));
//			});
//			$('.front #block-flickr-4 .flickr-photoset, #block-flickr-6 .flickr-group').addClass('animate');
			if (!$('html').hasClass('lt-ie8')) { 
				jQuery('#block-flickr-6 .flickr-photoset').animate({left:'-1340px'},120000);
			}

			// FROM: http://www.flickr.com/photos/certaindamage/8711979101
			// TO:	 http://www.flickr.com/groups/greyhoundadventures/pool/with/8711979101/
//			$('.front #content .block-flickr .flickr-group a[href*="flickr"]').each( function() {
//				var _href = $(this).attr('href');
//				$(this).attr('href', 'http://www.flickr.com/groups/greyhoundadventures/pool/with/' + _href.substr(_href.lastIndexOf('/')+1));
//			});
			$('#block-menu-menu-dead-dogs ul.menu li a').each( function() {
//				console.log($(this).text());
				if (!!!$(this).attr('title')) {
					$(this).removeAttr('href');
				} else {
					$(this).attr('href','/flickr/1/tags/'+$(this).attr('title'));
				}
			});
//			$('article.node-blog.node-teaser img.flickr-photoset-img').css('width','100%').css('width','auto');

		}
	};

	Drupal.behaviors.iframe = {
		attach: function(context, settings) { 
		//	$('article.node-blog iframe.map').each( function() {
		//		console.log("we have an iframe");
		//		if ($(this).attr('rel')) $(this).attr('src',$(this).attr('rel'));
		//	}); 
//			console.log("hellow");
//			alert('hellow');
		}
	};

/*	Drupal.behaviors.constant_contact = {
		attach: function(context, settings) {
			console.log('in behaviors.constant_contact');
			$('form#webform-client-form-16').submit( function() {
				$(this).find('input#edit-submitted-hidden-name').val( $(this).find('input#edit-submitted-human-name').val() );
			});
		}
	};
*/
})(jQuery, Drupal, this, this.document);
