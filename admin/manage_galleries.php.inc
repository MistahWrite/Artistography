<?php
/* http://mikejolley.com/2012/12/using-the-new-wordpress-3-5-media-uploader-in-plugins/ */
$gallery = new Gallery;
$artist = new Artist;

echo "<div class='aform'><div id='dialog-form' title='";
echo __("Gallery", $i18n_domain);

echo "' style='display:none;'>\n"
   . "	<p class='validateTips'>" .__("All form fields are required.", $i18n_domain). "</p>\n"
   . "	<form>\n"
   . "	<fieldset>\n"
   . "          <input type='hidden' name='id' id='id' />\n"
   . "		<label for='name'>" .__("Name", $i18n_domain). "</label>\n"
   . "		<input type='text' name='name' id='name' class='text ui-widget-content ui-corner-all' />\n"
   . "          <label for='artist_id'>" .__("Artist", $i18n_domain). "</label>\n"
   . "          <select name='artist_id' id='artist_id' class='text ui-widget-content ui-corner-all' />\n";
for($i = 0; $i < $artist->getTotalRows(); $i++) {
	echo "            <option value='$artist->id'>$artist->name</option>\n";
	$artist->getNodeNext();
}
echo "          </select>\n"
   . "		<label for='picture_url'>" .__("Cover Picture URL", $i18n_domain). "</label>\n"
   . "		<table class='text ui-widget-content ui-corner-all'><tr class='ui-widget-content ui-corner-all'><td class='ui-widget-content ui-corner-all'><input type='text' name='picture_url' id='picture_url' class='ui-widget-content ui-corner-all' />\n"
   . "          <td class='ui-widget-content ui-corner-all'><input id='upload_image_button' type='button' value='Upload Image' class='ui-widget-content ui-corner-all' /></td></tr></table>\n"

   . "          <label for='gallery'>" .__("Gallery", $i18n_domain). "</label>\n"
   . "          <table class='text ui-widget-content ui-corner-all'><tr class='ui-widget-content ui-corner-all'><td class='ui-widget-content ui-corner-all'><input type='text' name='gallery' id='gallery' class='ui-widget-content ui-corner-all' />\n"
   . "          <td class='ui-widget-content ui-corner-all'><input id='upload_images_button' type='button' value='Upload Images' class='ui-widget-content ui-corner-all' /></td></tr></table>\n"

   . "		<label for='gallery_descr'>" .__("Description", $i18n_domain). "</label>\n"
   . "          <textarea name='gallery_descr' id='gallery_descr' style='width:100%' rows='10' class='text ui-widget-content ui-corner-all'/></textarea>\n"
   . "	</fieldset>\n"
   . "	</form>\n"
   . "</div>\n"
   . "  <button id='create-gallery'>" .__('Create New Gallery', $i18n_domain). "</button></div>\n";

 /*** DISPLAY EXISTING GALLERY TABLE ***/
$pic_size ='75';
echo "	<div style='float:left;'><h1>" .__('Existing Galleries:', $i18n_domain). "</h1></div>\n"
   . "  <div style='float:right;border:1px dotted black; padding:10px;'>"
   . "    <label for='filter'>" .__('Filter:', $i18n_domain). "</label>"
   . "    <input type='text' name='filter' value='' id='filter' /></div>"
   . "	<table id='galleryTable' class='ui-widget ui-widget-content'>\n"
   . "	    <thead>\n"
   . "	        <tr class='ui-widget-header '>\n"
   . "	            <th>" .__("ID", $i18n_domain). "</th>\n"
   . "	            <th width='$pic_size'>" .__("Art", $i18n_domain). "</th>\n"
   . "	            <th>" .__("Gallery Name", $i18n_domain). "</th>\n"
   . "              <th>" .__("Page Views", $i18n_domain). "</th>\n"
   . "	            <th>" .__("Actions", $i18n_domain). "</th>\n"
   . "	        </tr>\n"
   . "	    </thead>\n"
   . "      <tbody>\n";

if ($gallery->loadAll('name ASC')->getTotalRows() === 0) {
  echo "	        <tr>\n"
     . "	            <td colspan='5'>\n"
     . __('No Galleries Available in the Database!', $i18n_domain)
     . "\n	            </td>\n"
     . "	        </tr>\n";
} else {

    for($i = 0; $i < $gallery->getTotalRows(); $i++) {
        $gallery->loadByNode($i);

        echo "	        <tr>\n"
           . "	            <td align='center' height='$pic_size'>" .$gallery->id. "</td>\n"
           . "	            <td align='center'>\n"
           . "<a href='" .stripslashes($gallery->cover_picture_url)
           . "'><img width='$pic_size' height='$pic_size' src='" .stripslashes($gallery->cover_picture_url). "' /></a>"
           . "              </td><td align='center'>\n"
           . "                  " .stripslashes($gallery->name)
           . "              </td><td align='center'>\n"
           . "                  " .$gallery->page_views
           . "              </td><td align='center'>\n";

         /* Edit Button */
        echo '<div style="float:left;">'
           . "  <button class='edit_button' id='edit_$gallery->id'>" .__('Edit', $i18n_domain). "</button>"
           . '</div>';
         /* Delete Button */
        echo '<div style="float:left;">'
           . "  <button class='delete_button' id='delete_$gallery->id'>" .__('Delete', $i18n_domain). "</button>"
           . '</div>'
           . "</td></tr>";
    }
}

echo "	    </tbody>\n"
   . "	</table>\n"
   . "</div>\n";

unset($gallery);

?>
