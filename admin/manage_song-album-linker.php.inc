<?php
$song = new Song;
$song2 = new Song;
$artist = new Artist;
$music = new Music;
$songalbumlinker = new SongAlbumLinker;

$num = $song->loadAll('number, id')->getTotalRows();
$width = '330px';
$height = '3.0em';

echo "    <div style='float:right;border:1px dotted black; padding:10px;'>"
   . "      <label for='filter'>" .__("Filter", $i18n_domain). ":</label>"
   . "      <input type='text' name='filter' value='' id='filter' />"
   . "    </div>"
   . '<div style="float:left;clear:none;height:400px;width:385px;overflow:auto;overflow-x:hidden;overflow-y:auto;-ms-overflow-y: auto;">';

    if ($num == 0) {
        _e('No Songs Available in the Database!', $i18n_domain);
    } else {
        for ($i = 0; $i < $song->getTotalRows(); $i++) {
		$hide = $songalbumlinker->loadBySongId($song->id)->getTotalRows();
            echo '  <div class="draggable ' .(($hide == 1) ? 'used-hidden' : ''). '" id="song_id_' .$song->id. "\" style='width:$width;height:$height'>"
               . '    <font style="font-size:8pt">'
	       . $song->number . " - "
	       . $artist->loadById($song->artist_id)->name . " - "
               . $song->name
               . '    </font>'
               . '  </div>';
		$song->getNodeNext();
        }
    }
    echo "</div>";

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

            $songalbumlinker->loadByAlbumId($music->id);

            if ($songalbumlinker->getTotalRows() > 0) {
                for($j = 0; $j < $songalbumlinker->getTotalRows(); $j++) {
                    $songalbumlinker->loadByNode($j);
                    $song2->loadById($songalbumlinker->song_id);
                    echo "<div class='pseudodraggable' id='album_id_$songalbumlinker->album_id-song_id_$songalbumlinker->song_id'  style='width:$width;height:$height'>"
                       . '  <font style="font-size:8pt">'
		       . $song2->number ." - "
		       . $artist->loadById($song2->id)->name . " - "
                       . "$song2->name<br/>\n"
                       . '  </font>'
                       . "  <button class='original_connection' id='delete_$songalbumlinker->album_id-$songalbumlinker->song_id'>Delete</button>"
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

unset($song);
unset($song2);
unset($artist);
unset($music);
unset($songalbumlinker);
?>
