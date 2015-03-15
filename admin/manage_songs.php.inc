<?php
$song = new Song;
$artist = new Artist;

echo "<div class='aform'><div id='dialog-form' title='";
echo __("Song", $i18n_domain);

echo "' style='display:none;'>\n"
   . "	<p class='validateTips'>" .__("All form fields are required.", $i18n_domain). "</p>\n"
   . "	<form>\n"
   . "	<fieldset>\n"
   . "          <input type='hidden' name='id' id='id' />
		<label for='number'>" .__("Track Number", $i18n_domain). "</label>
		<input type='text' name='number' id='number' class='text ui-widget-content ui-corner-all' />
		<label for='artist_id'>" .__("Artist Name", $i18n_domain). "</label>
		<select id='artist_id' name='artist_id' class='text ui-widget-content ui-corner-all'>";
	for($i=0; $i<$artist->getTotalRows(); $i++) {
		echo "<option value='$artist->id'>$artist->name</option>";
		$artist->getNodeNext();
	}
echo "		</select>"
   . "		<label for='name'>" .__("Song Name", $i18n_domain). "</label>\n"
   . "		<input type='text' name='name' id='name' class='text ui-widget-content ui-corner-all' />\n"
   . "		<label for='url'>" .__("Song URL", $i18n_domain). "</label>\n"
   . "		<table class='text ui-widget-content ui-corner-all'><tr class='ui-widget-content ui-corner-all'><td class='ui-widget-content ui-corner-all'><input type='text' name='url' id='url' class='ui-widget-content ui-corner-all' />\n"
   . "          <td class='ui-widget-content ui-corner-all'><input id='upload_song_button' type='button' value='Upload Song' class='ui-widget-content ui-corner-all' /></td></tr></table>
                <label for='price'>" .__("Price", $i18n_domain). "</label>
                <input type='text' name='price' id='price' value='0.00' class='text ui-widget-content ui-corner-all' />
		<label for='explicit'>" .__("Explicit Lyrics", $i18n_domain). "</label>
		<select id='explicit' name='explicit' class='text ui-widget-content ui-corner-all'>
			<option value='0'>No</option>
			<option value='1'>Yes</option>
		</select>
   	</fieldset>
   	</form>
      </div>
      <button id='create-song'>" .__('Create New Song', $i18n_domain). "</button></div>\n";

 /*** DISPLAY EXISTING SONGS TABLE ***/
echo "	<div style='float:left;'><h1>" .__('Existing Songs:', $i18n_domain). "</h1></div>\n"
   . "  <div style='float:right;border:1px dotted black; padding:10px;'>"
   . "    <label for='filter'>" .__('Filter:', $i18n_domain). "</label>
        <input type='text' name='filter' value='' id='filter' /></div>
   	<table id='songsTable' class='ui-widget ui-widget-content'>
   	    <thead>
   	        <tr class='ui-widget-header '>
   	            <th>#</th>
		    <th>" .__("Track", $i18n_domain). "</th>
   	            <th>" .__("Artist", $i18n_domain). "</th>
                    <th>" .__("Song Name", $i18n_domain). "</th>
                    <th>" .__("Price", $i18n_domain). "</th>
		    <th>" .__("Explicit", $i18n_domain). "</th>
   	            <th>" .__("Actions", $i18n_domain). "</th>
   	        </tr>
   	    </thead>
            <tbody>";

if ($song->loadAll('name ASC')->getTotalRows() === 0) {
  echo "	        <tr>\n"
     . "	            <td colspan='7'>\n"
     . __('No Songs Available in the Database!', $i18n_domain)
     . "\n	            </td>\n"
     . "	        </tr>\n";
} else {

    for($i = 0; $i < $song->getTotalRows(); $i++) {
        $song->loadByNode($i);

        echo "	        <tr>
           	            <td align='center'>
				$song->id
			    </td><td align='center'>
				$song->number
			    </td><td align='center'>"
				.stripslashes($artist->loadById($song->artist_id)->name)
           . "              </td><td align='center'>
                                <a href='" .stripslashes($song->url). "'>" .stripslashes($song->name). "</a>
                            </td><td align='center'>
                                $song->price
                            </td><td align='center'>
				" .(($song->explicit) ? "YES" : "NO"). "
			    </td><td align='center'>";

         /* Edit Button */
        echo '<div style="float:left;">'
           . "  <button class='edit_button' id='edit_$song->id'>" .__('Edit', $i18n_domain). "</button>"
           . '</div>';
         /* Delete Button */
        echo '<div style="float:left;">'
           . "  <button class='delete_button' id='delete_$song->id'>" .__('Delete', $i18n_domain). "</button>"
           . '</div>'
           . "</td></tr>";
    }
}

echo "	    </tbody>\n"
   . "	</table>\n"
   . "</div>\n";

unset($artist);
unset($song);

?>
