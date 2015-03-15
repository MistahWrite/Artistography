var $ = jQuery.noConflict();

var default_droppable_message = "<br/>Drag an artist to an album to associate them.<br/><br/>";

$(document).ready(function() {

  $('.used-hidden').hide();

  if ( $( "#accordion1" ).length != 0 ) $( "#accordion1" ).accordion({ autoHeight: false});
  if ( $( "#accordion2" ).length != 0 ) $( "#accordion2" ).accordion({ autoHeight: false});
  if ( $( "#accordion3" ).length != 0 ) $( "#accordion3" ).accordion({ autoHeight: false});

  if ( $( ".draggable" ).length != 0 && $( ".droppable" ).length != 0) {

    function showDroppableMessage(message) {
        $('.droppable').each(function() {
            if ( $.trim($(this).text()).length <= 0 ) {
                $(this).html(message);
            }
        });
    }

    function delSongAlbum(del_song_id, del_album_id) {
        var data_delete = {
                action: 'Disconnect_Song_from_Album',
                song_id: del_song_id,
                album_id: del_album_id
        };

        // since 2.8 ajaxurl is always defined in the admin header and points to admin-ajax.php
        $.post(ajaxurl, data_delete, function(response) {
            if(response === '1') {
                $('#album_id_' + del_album_id + '-song_id_' + del_song_id).hide('explode', '0').remove();
		$('#song_id_' + del_song_id).show();
                showDroppableMessage(default_droppable_message);
            } else {
                alert('Got this from the server: ' + response);
            }
        });
    }

    function addSongAlbum(add_song_id, add_album_id) {
        var data_add = {
                action: 'Connect_Song_to_Album',
                song_id: add_song_id,
                album_id: add_album_id
        };

        // since 2.8 ajaxurl is always defined in the admin header and points to admin-ajax.php
        $.post(ajaxurl, data_add, function(response) {
            //alert('Got this from the server: ' + response);
            if(response === '1') {
                showDroppableMessage('');
                if ($.trim($('#album_id_' + add_album_id).html()).length == default_droppable_message.length) {
                    $('#album_id_' + add_album_id).html('');
                }
                $('#song_id_' + add_song_id).clone(true)
                  .attr('id', 'album_id_' + add_album_id + '-song_id_' + add_song_id)
                  .appendTo('#album_id_' + add_album_id)
                  .toggleClass('draggable', false).toggleClass('pseudodraggable')
                  .append( "<button id='delete_" + add_album_id + "-" + add_song_id + "'>Delete</button>");
		$('#song_id_' + add_song_id).hide('fade', '0');
                $( "#delete_" + add_album_id + "-" + add_song_id ).button({
                    text: true,
                    icons: {
                        primary: "ui-icon-trash"
                    }
		}).click(function() {
                    delSongAlbum(add_song_id, add_album_id);
                });
            } else {
                alert('Got this from the server: ' + response);
            }
        });
    }

    showDroppableMessage(default_droppable_message);

    $(".original_connection").each(function() {
        $( this ).button({
            text: true,
            icons: {
                primary: "ui-icon-trash"
            }
        }).click(function() {
            var id = $( this ).attr('id')
                .replace('delete_','')
                .split('-', 2);
            var album_id = id[0];
            var song_id = id[1];
            delSongAlbum(song_id, album_id);
        });
    });

    $(".draggable").draggable({
      zIndex: 1,
      revert: true,
      cursor: 'crosshair',
      opacity: 0.8,
      helper: 'clone'
    });

    $(".droppable").droppable({
      activeClass: "ui-state-hover",
      hoverClass: "ui-state-active",
      drop: function(event, ui) {
          var song_id = ui.draggable.attr('id');
          var album_id = $(this).attr('id');
          addSongAlbum(song_id.replace('song_id_', ''), album_id.replace('album_id_', ''));
      }
    });
  }

});

 //used to apply alternating row styles
function zebraRows(selector, className)
{
  $(selector).removeClass(className).addClass(className);
}

 //filter results based on query   
function filter(selector, query) {
  query = $.trim(query); //trim white space
  query = query.replace(/ /gi, '|'); //add OR for regex query

  $(selector).each(function() {
    ($(this).text().search(new RegExp(query, "i")) < 0) ? $(this).hide().removeClass('visible') : $(this).show().addClass('visible');
  });
}
