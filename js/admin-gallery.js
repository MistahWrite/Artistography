var $ = jQuery.noConflict();

var default_droppable_message = "<br/>Drag an artist to an album to associate them.<br/><br/>";

$(document).ready(function() {

        jQuery('#upload_image_button').click(function() {
                formfield = jQuery('#picture_url').attr('name');
                tb_show('', 'media-upload.php?type=image&TB_iframe=true');
                return false;
        });
        window.send_to_editor = function(html) {
                imgurl = jQuery('img',html).attr('src');
                jQuery('#picture_url').val(imgurl);
                tb_remove();
        }

// Uploading files
var file_frame;
var wp_media_post_id = wp.media.model.settings.post.id; // Store the old id
var set_to_post_id = 10; // Set this

  jQuery('#upload_images_button').live('click', function( event ){

    event.preventDefault();

    // If the media frame already exists, reopen it.
    if ( file_frame ) {
      // Set the post ID to what we want
      file_frame.uploader.uploader.param( 'post_id', set_to_post_id );
      // Open frame
      file_frame.open();
      return;
    } else {
      // Set the wp.media post id so the uploader grabs the ID we want when initialised
      wp.media.model.settings.post.id = set_to_post_id;
    }

    // Create the media frame.
    file_frame = wp.media.frames.file_frame = wp.media({
      title: jQuery( this ).data( 'uploader_title' ),
      button: {
        text: jQuery( this ).data( 'uploader_button_text' ),
      },
      multiple: true  // Set to true to allow multiple files to be selected
    });

    // When an image is selected, run a callback.
    file_frame.on( 'select', function() {
      // We set multiple to false so only get one image from the uploader
      attachment = file_frame.state().get('selection').toJSON();
//      attachment = file_frame.state().get('selection').first().toJSON();

      jQuery('#gallery').val('');
      // Do something with attachment.id and/or attachment.url here
      for(i = 0; i < attachment.length; i++) {
        jQuery('#gallery').val(jQuery('#gallery').val() + attachment[i].id);
	if ((i+1) < attachment.length) {
          jQuery('#gallery').val(jQuery('#gallery').val() + ', ');
        } 
      }
      // Restore the main post ID
      wp.media.model.settings.post.id = wp_media_post_id;
    });

    // Finally, open the modal
    file_frame.open();
  });
  
  // Restore the main ID when the add media button is pressed
  jQuery('a.add_media').on('click', function() {
    wp.media.model.settings.post.id = wp_media_post_id;
  });

/*
        jQuery('#upload_images_button').click(function() {
                formfield = jQuery('#gallery').attr('name');
                tb_show('', 'media-upload.php?type=image&TB_iframe=true');
                return false;
        });
        window.send_to_editor = function(html) {
                imgurl = jQuery('img',html).attr('src');
                jQuery('#gallery').val(imgurl);
                tb_remove();
        }
*/
  if ( $( "#galleryTable" ).length != 0 ) {

      function addGallery(name, the_artist_id, gallery_ids, cover_picture_url, description) {
          var data_add = {
                  action: 'Create_Gallery',

		  artist_id: the_artist_id,
		  gallery_name: name,
                  gallery: gallery_ids,
                  picture_url: cover_picture_url,
                  gallery_descr: description
          };
          // since 2.8 ajaxurl is always defined in the admin header and points to admin-ajax.php
          $.post(ajaxurl, data_add, function(response) {
              if($.isNumeric(response)) {
                  // gallery was just added successfully
                return response; // TODO: make the id of newly added artist
              } else {
                  alert('Got this from the server: ' + response);
              }
          });
      }

      function updateGallery(update_gallery_id, name, update_artist_id, gallery_ids, gallery_picture_url, gallery_description) {
          var data_update = {
              action: 'Update_Gallery',

	      gallery_id: update_gallery_id,
              artist_id: update_artist_id,
              gallery_name: name,
              gallery: gallery_ids,
              picture_url: gallery_picture_url,
              gallery_descr: gallery_description
          };

          $.post(ajaxurl, data_update, function(response) {
              if(response === '1') {
              } else {
                  alert('Got this from the server: ' + response);
              }
          });
      }

      function editGallery(edit_gallery_id) {
          var data_edit = {
                  action: 'Edit_Gallery',
                  gallery_id: edit_gallery_id
          };

          $.post(ajaxurl, data_edit, function(response) {
              var res = response.split("##$$##");
              
              $( "#dialog-form" ).dialog( "open" );
              $( "#id" ).val(res[0]);
              $( "#artist_id" ).val(res[1]);
              $( "#name" ).val(res[2]);
              $( "#picture_url" ).val(res[3]);
	      $( "#gallery" ).val(res[4]);
              $( "#gallery_descr" ).val(res[5]);

          });
      }

      function delGallery(del_gallery_id) {
          var data_delete = {
                  action: 'Delete_Gallery',
                  gallery_id: del_gallery_id
          };

          // since 2.8 ajaxurl is always defined in the admin header and points to admin-ajax.php
          $.post(ajaxurl, data_delete, function(response) {
              if(response === '1') {
                   // drop table if deleted from database
                  $( '#delete_' + del_artist_id ).parent().parent().parent().hide('explode', '1000').remove();

                   //reapply zebra rows
                  $('.visible td').removeClass('odd');
                  zebraRows('.visible:odd td', 'odd');
              } else {
                  alert('Got this from the server: ' + response);
              }
          });
      }

      $( ".edit_button" ).button({
          text: true,
          icons: {
              primary: "ui-icon-pencil"
          }
      }).click(function() {
          var id = $( this ).attr('id').replace('edit_', '');
          editGallery(id);
      });

      $( ".delete_button" ).button({
          text: true,
          icons: {
              primary: "ui-icon-trash"
          }
      }).click(function() {
           // show alert to verify whether we are really wanting to delete this album
          if ( confirm('Are you sure you want to delete that gallery?') ) {
              var id = $( this ).attr('id').replace('delete_', '');

              delGallery(id);
          }
      });

      zebraRows('tbody tr:odd td', 'odd');

      $('tbody tr').hover(function(){
          $(this).find('td').addClass('hovered');
      }, function(){
          $(this).find('td').removeClass('hovered');
      });

      //default each row to visible
      $('tbody tr').addClass('visible');

      $('#filter').show();

      $('#filter').keyup(function(event) {
           //if esc is pressed or nothing is entered
          if (event.keyCode == 27 || $(this).val() == '') {
               //if esc is pressed we want to clear the value of search box
              $(this).val('');

               //we want each row to be visible because if nothing   
               //is entered then all rows are matched.   
              $('tbody tr').removeClass('visible').show().addClass('visible'); 
          } else { //if there is text, lets filter
              filter('tbody tr', $(this).val());
          }

           //reapply zebra rows
          $('.visible td').removeClass('odd');
          zebraRows('.visible:odd td', 'odd');
      });

      // ***** START SORTING CODE ********/
    //grab all header rows   
    $('thead th').each(function(column) {   
      $(this).addClass('sortable').click(function(){   
        var findSortKey = function($cell) {   
          return $cell.find('.sort-key').text().toUpperCase() + ' ' + $cell.text().toUpperCase();   
        };   
        var sortDirection = $(this).is('.sorted-asc') ? -1 : 1;   
      
        //step back up the tree and get the rows with data   
        //for sorting   
        var $rows = $(this).parent().parent().parent().find('tbody tr').get();   
      
        //loop through all the rows and find   
        $.each($rows, function(index, row) {   
          row.sortKey = findSortKey($(row).children('td').eq(column));   
        });   
      
        //compare and sort the rows alphabetically   
        $rows.sort(function(a, b) {   
            if (a.sortKey < b.sortKey) return -sortDirection;   
            if (a.sortKey > b.sortKey) return sortDirection;   
            return 0;   
        });   
      
        //add the rows in the correct order to the bottom of the table   
        $.each($rows, function(index, row) {   
            $('tbody').append(row);   
            row.sortKey = null;   
        });   
      
        //identify the column sort order   
        $('th').removeClass('sorted-asc sorted-desc');   
        var $sortHead = $('th').filter(':nth-child(' + (column + 1) + ')');   
        sortDirection == 1 ? $sortHead.addClass('sorted-asc') : $sortHead.addClass('sorted-desc');   
      
        //identify the column to be sorted by   
        $('td').removeClass('sorted')   
                      .filter(':nth-child(' + (column + 1) + ')')   
                      .addClass('sorted');   
        
          $('.visible td').removeClass('odd');   
          zebraRows('.visible:even td', 'odd');   
        });
      });
  }

  if ( $( "#dialog-form" ).length != 0) {
    // a workaround for a flaw in the demo system (http://dev.jqueryui.com/ticket/4375), ignore!
    $( "#dialog:ui-dialog" ).dialog( "destroy" );

    var id = $( "#id" ),
     name = $( "#name" ),
     artist_id = $( "#artist_id" ),
     picture_url = $( "#picture_url" ),
     gallery = $( "#gallery" ),
     gallery_descr = $( "#gallery_descr" ),
     allFields = $( [] ).add( id )
			.add( name )
			.add( artist_id )
			.add( picture_url )
			.add( gallery )
			.add( gallery_descr ),
     tips = $( ".validateTips" );

    function updateTips( t ) {
     tips
      .text( t )
      .addClass( "ui-state-highlight" );
     setTimeout(function() {
      tips.removeClass( "ui-state-highlight", 1500 );
     }, 500 );
    }

    function checkLength( o, n, min, max ) {
      if ( o.val().length > max || o.val().length < min ) {
        o.addClass( "ui-state-error" );
        updateTips( "Length of " + n + " must be between " + min + " and " + max + "." );
        return false;
      } else {
        return true;
      }
    }

    $( "#dialog-form" ).dialog({
      autoOpen: false,
      show: "implode",
      hide: "explode",
      height: 550,
      width: 350,
      modal: false,
      buttons: {
        "Save": function() {
          var result = false, bValid = true;
          allFields.removeClass( "ui-state-error" );

          bValid = bValid && checkLength( name, "name", 3, 100 );

          if ( id.val() === '' && bValid ) {
            id = addGallery(name.val(), artist_id.val(), gallery.val(), picture_url.val(), gallery_descr.val());
            $( "#galleryTable tbody" ).append( "<tr>" + "<td align='center'>" + id + "</td><td align='center'><img src='" + picture_url.val() + "' height='75' width='75' /></td><td align='center'>" + name.val() + "</td><td align='center'>0</td><td align'center'>Pending Creation...</td></tr>" );

            zebraRows('tbody tr:odd td', 'odd');
          } else {
            result = updateGallery(id.val(), name.val(), artist_id.val(), gallery.val(), picture_url.val(), gallery_descr.val());
            location.reload();
          }
	  $( this ).dialog( 'close' );
        },
        Cancel: function() {
          $( this ).dialog( "close" );
        }
      },
      close: function() {
        allFields.val( "" ).removeClass( "ui-state-error" );
	zebraRows('.visible:even td', 'odd');
      }
    });

    $( "#create-gallery" )
      .button()
      .click(function() {
        $( "#dialog-form" ).dialog( "open" );
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
