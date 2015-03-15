<?php
$music = new Music;
$download = new Download;

echo "<div class='aform'><div id='dialog-form' title='" .__('Music', $i18n_domain). "' style='display:none;'>
   	<p class='validateTips'>" .__("All form fields are required.", $i18n_domain). "</p>
   	<form>
   	<fieldset>
		  <input type='hidden' name='id' id='id' />
   		<label for='name'>Artist Name</label>
   		  <input type='text' name='artist_name' id='artist_name' class='text ui-widget-content ui-corner-all' />
                <label for='name'>Album Name</label>
                  <input type='text' name='album_name' id='album_name' class='text ui-widget-content ui-corner-all' />
   		<label for='release_date'>" .__("Released", $i18n_domain). "</label>
   		  <select name='album_day' id='album_day' class='ui-widget-content ui-corner-all'>";
			for($day=1; $day <= 31; $day++) {
				echo "<option value='$day'>$day</option>";
			}
echo "		  </select>
		  <select name='album_month' id='album_month' class='ui-widget-content ui-corner-all'>";
			for($month=1; $month <= 12; $month++) {
				echo "<option value='$month'>" 
					.date("m", mktime(date("H"), date("i"), date("s"), $month, date("j"), date("Y"))) 
					." - "
					. date("F", mktime(date("H"), date("i"), date("s"), $month, date("j"), date("Y")))
				   . "</option>";
			}
echo "		  </select>
		  <select name='album_year' id='album_year' class='ui-widget-content ui-corner-all'>";
			for($year=1+(int)date("Y"); $year >= 1965; $year--) {
				echo "<option value='$year'>$year</option>";
			}
echo "		  </select>
   		<label for='picture_url'>" .__("Album Art", $i18ndomain). "</label>
		  <table class='text ui-widget-content ui-corner-all'>
		    <tr class='ui-widget-content ui-corner-all'>
		      <td class='ui-widget-content ui-corner-all'>
   			<input type='text' name='picture_url' id='picture_url' class='text ui-widget-content ui-corner-all' />
		      </td><td class='ui-widget-content ui-corner-all'>
			<button id='upload_image_button' class='ui-widget-content ui-corner-all'>Select Image</button>
		      </td>
		    </tr>
		  </table>
   		<label for='artist_url'>" .__("Artist URL", $i18ndomain). "</label>
   		  <input type='text' name='artist_url' id='artist_url' class='text ui-widget-content ui-corner-all' />
		<label for='store_url'>" .__("Store URL", $i18ndomain). "</label>
		  <input type='text' name='store_url' id='store_url' class='text ui-widget-content ui-corner-all' />
		<label for='price'>" .__("Price", $i18ndomain). "</label>
		  <input type='text' name='price' id='price' class='text ui-widget-content ui-corner-all' />
		<label for='download_id'>" .__("Link to Download", $i18ndomain). "</label>
		  <select name='download_id' id='download_id' class='ui-widget-content ui-corner-all'>
			<option value='0'>Unlinked</option>";
			for ($i = 0; $i < $download->getTotalRows(); $i++) {
				echo "<option value='$download->id'>$download->id - $download->file_name</option>";
				$download->getNodeNext();
			}
echo "		  </select>
   		<label for='name'>" .__("Description", $i18ndomain). "</label>
   		  <textarea name='description' id='description' class='text ui-widget-content ui-corner-all'></textarea>
   	</fieldset>
   	</form>
   </div>
   <div id='music-contain' class='ui-widget'>
   <button id='create-album'>" .__('Create New Album', $i18n_domain). "</button></div>
   	<div style='float:left;'><h1>" .__('Existing Albums', $i18n_domain). ":</h1></div>
     <div style='float:right;border:1px dotted black; padding:10px;'>
       <label for='filter'>" .__('Filter:', $i18n_domain). "</label>
       <input type='text' name='filter' value='' id='filter' />
     </div>
   	<table id='musicTable' class='ui-widget ui-widget-content'>
   	    <thead>
   	        <tr class='ui-widget-header '>
   	            <th>#</th>
   	            <th>" .__("Art", $i18n_domain). "</th>
   	            <th>" .__("Album", $i18n_domain). "</th>
   		    <th>" .__("Downloads", $i18n_domain). "</th>
   	            <th>" .__("Price", $i18n_domain). "</th>
   	            <th>" .__("Actions", $i18n_domain). "</th>
   	        </tr>
   	    </thead>
         <tbody>\n";

if ($music->loadAll('id DESC')->getTotalRows() === 0) {
    echo '<tr><td colspan="7">';
    _e('No Albums Available in the Database!', $i18n_domain);
    echo '</td></tr>';
} else {
    for($i = 0; $i < $music->getTotalRows(); $i++) {
        $music->loadByNode($i);
        echo "<tr>
         <td>" .stripslashes($music->id). "</td>
         <td>
            <center>\n";
        echo "<a href='/download/$music->download_id' target='_blank'><img src='"
           . $music->picture_url. "' width='75' height='75' /></a>
         </td>
         <td>
              <a href='" .stripslashes($music->artist_url). "'>" .stripslashes($music->artist_name). "</a><br/>
              <a href='" .stripslashes($music->store_url). "'>" .stripslashes($music->album_name). "</a><br/>
          Released: <a href='" .stripslashes($music->store_url). "'>";
        echo $music->album_month. '.' .$music->album_day. '.' .$music->album_year. "
            </center>
         </td>";
	echo "<td>$music->total_downloads</td>\n";
        echo "<td>" .CURRENCY.$music->price. "</td>\n
          <td>";

         /* Edit Button */
        echo "<div style='float:left;'>
		<button class='edit_button' id='edit_$music->id'>" .__('Edit', $i18n_domain). "</button>
              </div>";

         /* Delete Button */
        echo "<div style='float:left;'>
                <button class='delete_button' id='delete_$music->id'>" .__('Delete', $i18n_domain). "</button>
              </div>";

        echo "</td>
        </tr>\n";
    }
}

echo "	    </tbody>\n"
   . "	</table>\n"
   . "</div>\n";

unset($music);

?>
