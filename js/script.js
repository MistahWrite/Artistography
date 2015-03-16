var $ = jQuery.noConflict();
var console = window.console;

$(document).ready(function() {
    var $tabs = $('#tabs').tabs({ fx: { opacity: 'toggle', height: 'toggle', width: 'toggle' } }).slideDown();

    if ( $( "#accordion1" ).length != 0 ) $( "#accordion1" ).accordion({ autoHeight: false});
    if ( $( "#accordion2" ).length != 0 ) $( "#accordion2" ).accordion({ autoHeight: false});
    if ( $( "#accordion3" ).length != 0 ) $( "#accordion3" ).accordion({ autoHeight: false});
    if ( $( "#accordion4" ).length != 0 ) $( "#accordion4" ).accordion({ autoHeight: false});

	function LoadShoppingCart() {
	    var data = {
	        action: 'Get_Cart_Contents'
	    };
	    $.post(MyAjax.ajaxurl, data, function(response) {
	    	$(".artistography_cart").empty().append(response);

		 /* Delete From Cart Button */
		$( ".remove_from_cart" ).button({
		    text: true,
		    icons: {
		        primary: "ui-icon-trash"
		    }    }).click(function() {
		    var id = $( this ).attr('id');
		    var data = {
		        action: 'Remove_From_Cart',
        	        product: id
		    };
		    $.post(MyAjax.ajaxurl, data, function(response) {
		        LoadShoppingCart();
			LoadBasicShoppingCart(false);
        	   });
    		});
	    });
	}
	LoadShoppingCart();

        function LoadBasicShoppingCart(scroll_to) {
                if (typeof scroll_to === "undefined" || scroll_to === null) {
                        scroll_to = true; 
                }
            var data = {
                action: 'Get_Basic_Cart_Contents'
            };
            $.post(MyAjax.ajaxurl, data, function(response) {
                $(".artistography_basic_cart").empty().append(response);
                if(scroll_to) {
                        $('html, body').animate({
                                scrollTop: $(".widget_artistography_cart_widget").offset().top
                        }, 2000);
                }

            });
        }
        LoadBasicShoppingCart(false);

     /* Edit Shopping Cart Button */
    $( ".edit_cart" ).button({
        text: true,
        icons: {
            primary: "ui-icon-cart"
        }
    }).click(function() {
	/* Do AJAX call to get URL of cart and go there */
	  var data = {
                  action: 'Get_Cart_URL'
          };
          $.post(MyAjax.ajaxurl, data, function(response) {
		location.assign(response);
          });
    });

     /* Play Song Button */
    $( ".artistography_play" ).button({
        text: false,
        icons: {
            primary: "ui-icon-play"
        }
    }).click(function() {
        var id = $( this ).attr('id').replace('song_play_', '');
          var data = {
                  action: 'Play_Song',
                  song_id: id
          };
          $.post(MyAjax.ajaxurl, data, function(response) {
		 /* Put song or album Into soundmanager2 playing */
		$( ".sm2-playlist-wrapper" ).children( ".sm2-playlist-bd" ).empty().append(response);
		var i = -1;
		var num = 0;
		 /* For some reason this gives me i = li * 2 */
		$(".sm2-playlist-bd").children('li').each(function() {
			i += 1;
			if ( $( this ).is('.selected') ) {
				num = i;
			}
		});
		var offset = num - 1;
		console.log('i: ' + i + ', num: ' + num + ', offset: ' + offset);

		var item = window.sm2BarPlayers[0].playlistController.getItem(offset);
		window.sm2BarPlayers[0].playlistController.playItemByOffset(offset);
          });
    });

     /* Buy Song Button */
    $( ".song_to_cart" ).button({
        text: true,
        icons: {
            primary: "ui-icon-cart"
        }
    }).click(function() {
        var id = $( this ).attr('id').replace('song_download_', '');
          var data = {
                  action: 'Add_Song_To_Cart',
		  song_id: id
          };
          $.post(MyAjax.ajaxurl, data, function(response) {
                LoadBasicShoppingCart();
		LoadShoppingCart();
          });
    });

     /* Buy Album Download Button */
    $( ".album_download_to_cart" ).button({
        text: false,
        icons: {
            primary: "ui-icon-cart"
        }
    }).click(function() {
        var id = $( this ).attr('id').replace('album_download_', '');
          var data = {
                  action: 'Add_Album_Download_To_Cart',
                  album_id: id
          };
          $.post(MyAjax.ajaxurl, data, function(response) {
		LoadBasicShoppingCart();
                LoadShoppingCart();
          });
    });

    $(".colorbox").colorbox({scalePhotos:true, maxWidth: '90%', maxHeight: '90%', slideshow:true});

    $('.artist_table').slideDown();

    $('.qitem')
	.hover(
		function() {
			$(this).find('img').fadeOut();
		}, function(){
        	        $(this).find('img').fadeIn(1000);
   		}
    	);

	soundManager.setup({
		url: '../soundmanagerv297a-20140901/swf/',
		flashVersion: 9, // optional: shiny features (default = 8)
		 // optional: ignore Flash where possible, use 100% HTML5 mode
		 //preferFlash: false,

		onready: function() {
		 // Ready to use; soundManager.createSound() etc. can now be called.
        		var link;
			$( "a.track_length" ).each(function(index) {
				link = $( this ).attr('href');
				setDuration($( this ), link);
        		});
		}
	});

	function msToTime(duration) {
		duration = duration | 0;
        	var milliseconds = parseInt((duration%1000)/100)
        	    , seconds = parseInt((duration/1000)%60)
        	    , minutes = parseInt((duration/(1000*60))%60)
        	    , hours = parseInt((duration/(1000*60*60))%24);

        	hours = (hours < 10) ? "0" + hours : hours;
        	minutes = (minutes < 10) ? "0" + minutes : minutes;
        	seconds = (seconds < 10) ? "0" + seconds : seconds;

		if (hours == "00") {
			return minutes + ":" + seconds;
		} else {
        		return hours + ":" + minutes + ":" + seconds;
		}
    	}

        function setDuration(object, url)
        {
                var mySound = soundManager.createSound({
			url: url,
			autoLoad: true,
			whileloading: function () {
				if (this.durationEstimate > 0) {
					object.replaceWith(msToTime('' + this.durationEstimate + ''));
					this.destruct();
				}
			},
			onload: function() {
				object.replaceWith(msToTime('' + this.duration + ''));
				this.destruct();
			}
		});
        }

	function getSongId() {
                 /* Put song or album Into soundmanager2 playing */
                return $(".sm2-playlist-bd").children('li.selected').attr('id').replace('playlist_song_', '');
	}

        function evtHandlerSetTitle() {
		var song_id = getSongId();
		loadAlbumArt(song_id);
		if( typeof window.sm2BarPlayers[0] !== 'undefined' ) {
			loadAlbumArtRefresh(window.sm2BarPlayers[0].dom.playlistContainer.style.height.replace('px', ''));
		} else {
			loadAlbumArtRefresh(0);
		}
        }

	function evtHandlerMenu() {
		if( typeof window.sm2BarPlayers[0] !== 'undefined' ) {
			loadAlbumArtRefresh(window.sm2BarPlayers[0].dom.playlistContainer.style.height.replace('px', ''));
		} else {
			loadAlbumArtRefresh(0);
		}
	}

	var evtNameSetTitle = 'sm2-bar-ui-setTitle';
	var evtNameMenu = 'sm2-bar-ui-menu';

        if (window.addEventListener) {
          window.addEventListener(evtNameSetTitle, evtHandlerSetTitle, false);
	  window.addEventListener(evtNameMenu, evtHandlerMenu, false);
        } else {
          window.attachEvent('on' + evtNameSetTitle, evtHandlerSetTitle);
	  window.attachEvent('on' + evtNameMenu, evtHandlerMenu);
        }

      function loadAlbumArt(song_id) {
          var data = {
                  action: 'Load_Album_Art',
                  song_id: song_id
          };
          $.post(MyAjax.ajaxurl, data, function(response) {
              $( "#sm2-album-art" ).attr('src', response);
              $( "#sm2-album-art" ).attr('height', '100%');
	      var height = $( "#sm2-album-art" ).height();
              $("#sm2-album-art" ).attr('width', height);
          });
      }

      function loadAlbumArtRefresh(height) {
              $( "#sm2-album-art" ).attr('height', height);
              $("#sm2-album-art" ).attr('width', height);
      }

});
