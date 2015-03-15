<?php

function display_manage_discography_form ($artist_id = 0, $init_artist = '', $init_picture_url = '/', $init_artist_url = '/') {
  $text_width = "400px";
  $faalinker = 'fupdatelinker';

  echo "<center>\n
        <form name='$faalinker' action='' method='post'>\n
        <table>\n
        <tr><td colspan='2' style='text-align:center;font-weight:bold;'><a href='$init_artist_url'>$init_artist</a></td></tr>\n
        <input type='hidden' name='artist_id' value='$artist_id' />\n";

  $thetable = $wpdb->prefix . $TABLE_NAME[TABLE_ARTIST_MUSIC_ALBUMS];
  $query = "SELECT *
            FROM $thetable
            ORDER BY album_date DESC";
  $num = $wpdb->query($query);

  if($num === FALSE) {
    wp_die( sprintf(__('An error occurred while trying to perform a query: "%s"', $i18n_domain), $query) );
  } else {
    $row = $wpdb->get_row($query, ARRAY_A, 0);
    $cols = 4;
    $rows = ((int)($num / $cols) < (float)($num / $cols)) ? (int)($num / $cols) + 1 : (int)($num / $cols);

    $width = '150px';
    $height = '150px';

    echo "    <table border='0'>\n";
    if ($num === 0) {
      echo "    <tr>\n
                  <td colspan='$cols' style='border:none;'>" .__('No Music Available in the Database!', $i18n_domain). "</td>\n
                </tr>\n";
    } else {
      for ($i = 0; $i < $rows; $i++) {
        echo "      <tr>\n";
        for($j = 0; $j < $cols && ((($cols * ($i)) + ($j+1)) <= $num); $j++) {
          $album_id = $row['id'];
          $picture_url = stripslashes($row['picture_url']);
          $download_id = stripslashes($row['download_id']);
          $artist_name = stripslashes($row['artist_name']);
          $album_name = stripslashes($row['album_name']);
          $album_date = stripslashes($row['album_date']);
          $artist_url = stripslashes($row['artist_url']);
          $store_url = stripslashes($row['store_url']);
          $free_download_enabled = $row['free_download_enabled'];
          $featured = $row['featured'];
          $description = stripslashes($row['description']);

          $thetable = $wpdb->prefix . $TABLE_NAME[TABLE_ARTIST_ALBUM_LINKER];
          $query = "SELECT *
                    FROM $thetable
                    WHERE artist_id='$artist_id'
                      AND album_id='$album_id'";
          $num2 = $wpdb->query($query);

          if($num2 === FALSE) {
            wp_die( sprintf(__('An error occurred while trying to perform a query: "%s"', $i18n_domain), $query) );
          } else {
            $row2 = $wpdb->get_row($query, ARRAY_A, $j);
            if ($num2 > 0) {
              $album_checked = 'checked ';
            } else {
              $album_checked = '';
            }
            echo "          <td style='border:0;'>\n
                              <div>\n
                                <center><font style='font-size:8pt'>\n
                                <a href='$artist_url'>$artist_name</a><br/>\n
                                <a href='$store_url'>$album_name</a><br/>\n
                                <a href='$store_url' target='_blank'><img src='$picture_url' style='border:1px solid black;height:$height;width:$width;' /></a><br/>\n
                                <input type='checkbox' name='album_$album_id' $album_checked/>" .__("Link this Album", $i18n_domain). "<br/>\n
                                </font></center>\n
                              </div>\n
                            </td>\n";
          }
        }
        for($j; $j < $cols; $j++) {
          echo "<td>&nbsp;</td>\n";
        }
        echo "      </tr>\n";
        if ($i < ($rows-1)) { echo "<tr><td colspan='$cols'><hr style='width:100%;'></td></tr>\n"; }
      }
    }
  }

  echo "<tr><td colspan='2'></td></tr>
        <tr><td colspan='2' style='text-align:center;'><input type='submit' name='update' value='" .__("Update Artist Album Features", $i18n_domain). "' /></td></tr>\n
        </table>
        </form>
        </center>";
}

function display_artist_discography ($artist_id = 0) {

  $thetable = $wpdb->prefix . $TABLE_NAME[TABLE_ARTIST_MUSIC_ALBUMS];
  $thetable2 = $wpdb->prefix . $TABLE_NAME[TABLE_ARTIST_ALBUM_LINKER];
  $query = "SELECT *
            FROM $thetable
            WHERE id IN (SELECT album_id
                         FROM $thetable2
                         WHERE artist_id=$artist_id
                           AND enabled=1)
            ORDER BY album_date DESC";
  $num = $wpdb->query($query);
  if($num === FALSE) {
    wp_die( sprintf(__('An error occurred while trying to perform a query: "%s"', $i18n_domain), $query) );
  } else {
    $cols = 1;
    $rows = ((int)($num / $cols) < (float)($num / $cols)) ? (int)($num / $cols) + 1 : (int)($num / $cols);
    $width = '225px'; //'150px';
    $height = '225px'; //'150px';

    $discog_html .= "    <center>
                         <table border='0'>\n";
    if ($num == 0) {
      $discog_html .= "    <tr>
                             <td colspan='$cols' style='border:none;'>" .__('No Music Available in the Database!', $i18n_domain). "</td>
                           </tr>\n";
    } else {
      for ($i = 0; $i < $rows; $i++) {
        $discog_html .= "      <tr>\n";
        for($j = 0; $j < $cols && ((($cols * ($i)) + ($j+1)) <= $num); $j++) {
          $row = $wpdb->get_row($query, ARRAY_A, $j);

          $id = $row['id'];
          $picture_url = stripslashes($row['picture_url']);
          $download_id = stripslashes($row['download_id']);
          $artist_id = NULL;
          $artist_name = stripslashes($row['artist_name']);
          $album_name = stripslashes($row['album_name']);
          $album_date = stripslashes($row['album_date']);
          $artist_url = stripslashes($row['artist_url']);
          $store_url = stripslashes($row['store_url']);
          $free_download_enabled = $row['free_download_enabled'];
          $featured = $row['featured'];
          $description = stripslashes($row['description']);

          $released = date("M",mktime(0,0,0,(int)substr($album_date, 5, 2), (int)substr($album_date, 8, 2), (int)substr($album_date, 0, 4))). "." .substr($album_date, 8, 2). "." .substr($album_date, 0, 4);
          $discog_html .= "        <td style='border:0;'>\n
                                     <center><font style='font-size:8pt'>\n
                                     <a href='$artist_url' alt='$artist_name' title='$artist_name'>$artist_name</a><br/>\n
                                     <a href='$store_url' alt='$released - $artist_name - $album_name' title='$released - $artist_name - $album_name'>$album_name</a><br/>\n
                                     <a href='$store_url' target='_blank'><img src='$picture_url' style='border:1px solid black;height:$height;width:$width;' alt='" .$released. " - $artist_name - $album_name' title='" .$released. " - $artist_name - $album_name' /></a><br/>\n
                                     Released: " .date("M",mktime(0,0,0,(int)substr($album_date, 5, 2), (int)substr($album_date, 8, 2), (int)substr($album_date, 0, 4))). "." .substr($album_date, 8, 2). ".<a href='#' target='_blank'>" .substr($album_date, 0, 4). "</a><br/>\n
                                     <a href='/download.php?id=$download_id' target='_blank'><img width='112' height='75' src='/images/download.grey.icon.jpg' /></a>\n
                                     </font></center>\n
                                 </td>\n
                                 <td style='border:0;'>\n
                                     <div class='tracklist'>\n
                                     <font style='font-size:8pt'>\n
                                       $description\n
                                     </font>\n
                                     </div>\n
                                 </td>\n";
        }
        for($j; $j < $cols; $j++) {
          $discog_html .= "<td>&nbsp;</td>\n";
        }
        $discog_html .= "      </tr>\n";
        if ($i < ($rows-1)) { $discog_html .= "<tr><td colspan='" .($cols*2). "'><hr style='width:100%;'></td></tr>\n"; }
      }
    }
    $discog_html .= "    </table>
                         </center>\n";
  }

  return $discog_html;
}

?>
