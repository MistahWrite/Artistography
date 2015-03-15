<?php
$artist = new Artist;
$artist2 = new Artist;
$music = new Music;
$discography = new Discography;

$num = $artist->loadAll('name')->getTotalRows();
$cols = 3;
$rows = ((int)($num / $cols) < (float)($num / $cols)) ? (int)($num / $cols) + 1 : (int)($num / $cols);
$width = '50px';
$height = '50px';

echo '<table>'
   . '<tr>'
   . '  <td></td>'
   . '  <td></td>'
   . '  <td>'
   . "    <div style='float:right;border:1px dotted black; padding:10px;'>"
   . "      <label for='filter'>" .__("Filter", $i18n_domain). ":</label>"
   . "      <input type='text' name='filter' value='' id='filter' />"
   . "    </div>"
   . '  </td>'
   . '</tr>'
   . '<tr><td>'
   . '<div style="height:400px;width:385px;overflow:auto;overflow-x:hidden;overflow-y:auto;-ms-overflow-y: auto;">'
   . '<table style="border:2px solid black;">';

    if ($num == 0) {
        echo "<tr><td colspan='$cols' style='border:2px solid black;text-align:center;'>";
        _e('No Artists Available in the Database!', $i18n_domain);
        echo '</td></tr>';
    } else {
        $j = 0;
        for ($i = 0; $i < $rows; $i++) {
            echo "<tr>\n";
            for($j = 0; $j < $cols && ((($cols * ($i)) + ($j+1)) <= $num); $j++) {
                $artist->loadByNode($i * $cols + $j);
                echo '<td style="border:0;">'
                   . '  <div class="draggable" id="artist_id_' .$artist->id. '">'
                   . '    <center><font style="font-size:8pt">'
                   . $artist->name. "<br/>\n"
                   . "<img src='" .$artist->picture_url. "' style='border:1px solid black;height:$height;width:$width;' /></a>"
                   . '    </font></center>'
                   . '  </div>'
                   . '</td>';
            }
            for($j; $j < $cols; $j++) {
                echo "<td>&nbsp;</td>\n";
            }
            echo "      </tr>\n";
            if ($i < ($rows-1)) { echo "<tr><td colspan='$cols'><hr style='width:100%;'></td></tr>\n"; }
        }
    }
    echo "</table>"
       . "</div>"
       . "</td><td width='25'></td>"
       . "<td>";

    $music->loadAll('album_date DESC');

    if ( $music->getTotalRows() > 0 ) {
        echo '<div style="height:500px;overflow:auto;width:438px">'
           . "<div id='accordion1'>\n";

        for($i = 0; $i < $music->getTotalRows(); $i++) {
            $music->loadByNode($i);
            echo "  <h3 valign='absmiddle'><a href='#'><table><tr><td><img src='" .$music->picture_url. "' width='50' height='50' border='1'></td>"
               . "<td>" . $music->album_year. "<br/>" .stripslashes($music->artist_name). "<br/>" .stripslashes($music->album_name) . "</td>"
               . "</tr></table></a></h3>\n"
               . "  <div>\n"
               . "    <div class='droppable' id='album_id_$music->id'>";

            $discography->loadByAlbumId($music->id);

            if ($discography->getTotalRows() > 0) {
                for($j = 0; $j < $discography->getTotalRows(); $j++) {
                    $discography->loadByNode($j);
                    $artist2->loadById($discography->artist_id);
                    echo "<div class='pseudodraggable' id='album_id_$discography->album_id-artist_id_$discography->artist_id'>"
                       . '  <center><font style="font-size:8pt">'
                       . "$artist2->name<br/>\n"
                       . "  <img src='$artist2->picture_url' style='border:1px solid black;height:$height;width:$width;' /></a>"
                       . '  </font></center>'
                       . "  <button class='original_connection' id='delete_$discography->album_id-$discography->artist_id'>Delete</button>"
                       . '</div>';
                }
            }

            echo "</div>\n"
               . "  </div>\n";
        }
        echo "  </div>\n"
           . "</div>\n";
    } else {
        _e("There's no music in the database yet.<br/>\n", $i18n_domain);
    }

echo "</td></tr>"
   . "</table>";

unset($artist);
unset($artist2);
unset($music);
unset($discography);
?>
