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

  if ( $( "#artistsTable" ).length != 0 ) {

      function addArtist(name, artist_url, artist_picture_url, artist_description) {
          var data_add = {
                  action: 'Create_Artist',
                  artist_name: name,
                  url: artist_url,
                  picture_url: artist_picture_url,
                  artist_descr: artist_description
          };

          // since 2.8 ajaxurl is always defined in the admin header and points to admin-ajax.php
          $.post(ajaxurl, data_add, function(response) {
	   // artist was just added successfully
alert(response);
          return response; // TODO: make the id of newly added artist
          });
      }

      function updateArtist(update_artist_id, name, artist_url, artist_picture_url, artist_description) {
          var data_update = {
              action: 'Update_Artist',
              artist_id: update_artist_id,
              artist_name: name,
              url: artist_url,
              picture_url: artist_picture_url,
              artist_descr: artist_description
          };

          $.post(ajaxurl, data_update, function(response) {
              if(response === '1') {
              } else {
                  alert('Got this from the server: ' + response);
              }
          });
      }

      function editArtist(edit_artist_id) {
          var data_edit = {
                  action: 'Edit_Artist',
                  artist_id: edit_artist_id
          };

          $.post(ajaxurl, data_edit, function(response) {
              var res = response.split("##$$##");
              
              $( "#dialog-form" ).dialog( "open" );
              $( "#id" ).val(res[0]);
              $( "#name" ).val(res[1]);
              $( "#url" ).val(res[3]);
              $( "#picture_url" ).val(res[2]);
              $( "#artist_descr" ).val(res[4]);

          });
      }

      function delArtist(del_artist_id) {
          var data_delete = {
                  action: 'Delete_Artist',
                  artist_id: del_artist_id
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
          editArtist(id);
      });

      $( ".delete_button" ).button({
          text: true,
          icons: {
              primary: "ui-icon-trash"
          }
      }).click(function() {
           // show alert to verify whether we are really wanting to delete this album
          if ( confirm('Are you sure you want to delete that artist?  Also deletes all references to this artist in the Discography.') ) {
              var id = $( this ).attr('id').replace('delete_', '');

              delArtist(id);
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
//    $( "#dialog:ui-dialog" ).dialog( "destroy" );

    var dialog, form,
     id = $( "#id" ),
     name = $( "#name" ),
     url = $( "#url" ),
     picture_url = $( "#picture_url" ),
     artist_descr = $( "#artist_descr" ),
     allFields = $( [] ).add( id ).add( name ).add( url ).add( picture_url ).add( artist_descr ),
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

    dialog = $( "#dialog-form" ).dialog({
      autoOpen: false,
      show: "implode",
      hide: "explode",
      height: 450,
      width: 350,
      modal: true,
      buttons: {
        "Save": function() {
          var result = false, bValid = true;
          allFields.removeClass( "ui-state-error" );

          bValid = bValid && checkLength( name, "name", 3, 100 );

          if ( id.val() === '' && bValid ) {
            var artist_id = addArtist(name.val(), url.val(), picture_url.val(), artist_descr.val());
alert(artist_id);
            $( "#artistsTable tbody" ).append( "<tr>" + "<td align='center'>" + artist_id + "</td><td align='center'><a href='" + url.val() + "' target='_blank'><img src='" + picture_url.val() + "' height='75' width='75' /></a></td><td align='center'><a href='" + url.val() + "' target='_blank'>" + name.val() + "</a></td><td align='center'>0</td><td align'center'>Pending Creation...</td></tr>" );

		$( this ).dialog( "close" );
          } else {
            result = updateArtist(id.val(), name.val(), url.val(), picture_url.val(), artist_descr.val());
            $( this ).dialog( 'close' );
            location.reload();
          }
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

    $( "#create-artist" )
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
