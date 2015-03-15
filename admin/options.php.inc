<?php

if(array_key_exists('save', $_POST)) if (strcmp($_POST['save'], NULL)) {
  update_option('wp_artistography_donate_email', $_POST['donate_email']);
  update_option('wp_artistography_paypal_sandbox', ((strcmp($_POST['paypal_sandbox'], NULL)) ? true : false));
  update_option('wp_artistography_preserve_hidden_pages', ((strcmp($_POST['preserve_hidden_pages'], NULL)) ? true : false));
  update_option('wp_artistography_preserve_database', ((strcmp($_POST['preserve_database'], NULL)) ? true : false));
  update_option('wp_artistography_email_notify_ftp', ((strcmp($_POST['email_notify_ftp'], NULL)) ? true : false));
  update_option('wp_artistography_ftp_host', $_POST['ftp_host']);
  update_option('wp_artistography_ftp_user', $_POST['ftp_user']);
  update_option('wp_artistography_ftp_pass', $_POST['ftp_pass']);
  update_option('wp_artistography_ftp_path', $_POST['ftp_path']);
  update_option('wp_artistography_gallery_style', $_POST['gallery_style']);
  update_option('wp_artistography_ajax_loader', ((strcmp($_POST['ajax_loader'], NULL)) ? true : false));
  echo "Options saved.<br/>\n";
}

echo '<p>';
echo "<form method='post' action=''>";
echo "<u><h3>" .__("General", $i18n_domain). "</h3></u>";
    echo __("PayPal Payments and Donations Email", $i18n_domain) .": <input type='textbox' name='donate_email' id='donate_email' value='". get_option('wp_artistography_donate_email') ."' /><br/>";
    echo "<input type='checkbox' name='paypal_sandbox' id='paypal_sandbox' " .((strcmp(get_option('wp_artistography_paypal_sandbox'), NULL)) ? "checked='checked'" : ''). " />" .__("Use PayPal Sandbox for Testing Store", $i18n_domain). "<br/>";
    echo "<input type='checkbox' name='email_notify_ftp' id='email_notify_ftp' " .((strcmp(get_option('wp_artistography_email_notify_ftp'), NULL)) ? "checked='checked'" : ''). " />" .__("Email Notifications (to Admin email) on FTP Uploader Page Load", $i18n_domain). "<br/>";
    echo "<input type='checkbox' name='ajax_loader' id='ajax_loader' " .((strcmp(get_option('wp_artistography_ajax_loader'), NULL)) ? "checked='checked'" : ''). " />" .__("AJAX Page Loader (such as AJAXify or Advanced AJAX Page Loader)", $i18n_domain). "<br/>";

	$gallery_style = get_option('wp_artistography_gallery_style');;
	echo __("Image Gallery Style", $i18n_domain) .": <select name='gallery_style'>\n";
	echo "<option value='Accordion' " .((strcmp($gallery_style, 'Accordion') == 0) ? 'selected' : ''). ">Accordion</option>\n";
	echo "<option value='Thickbox' " .((strcmp($gallery_style, 'Thickbox') == 0) ? 'selected' : ''). ">Thickbox</option>\n";
	echo "<option value='Colorbox' " .((strcmp($gallery_style, 'Colorbox') == 0) ? 'selected' : ''). ">Colorbox</option>\n";
	echo "</select><br/>\n";

    echo "<u><h3>" .__("FTP Uploader", $i18n_domain). "</h3></u>";
    echo __("FTP Host", $i18n_domain) .": <input type='textbox' name='ftp_host' id='ftp_host' value='". get_option('wp_artistography_ftp_host') ."' /><br/>";
    echo __("FTP User", $i18n_domain) .": <input type='textbox' name='ftp_user' id='ftp_user' value='". get_option('wp_artistography_ftp_user') ."' /><br/>";
    echo __("FTP Pass", $i18n_domain) .": <input type='password' name='ftp_pass' id='ftp_pass' value='". get_option('wp_artistography_ftp_pass') ."' /><br/>";
    echo __("FTP Path", $i18n_domain) .": <input type='textbox' name='ftp_path' id='ftp_path' value='". get_option('wp_artistography_ftp_path') ."' /><br/>";

    echo "<u><h3>" .__("Backend", $i18n_domain). "</h3></u>";
    echo "<input type='checkbox' name='preserve_hidden_pages' id='preserve_hidden_pages' " .((strcmp(get_option('wp_artistography_preserve_hidden_pages'), NULL)) ? "checked='checked'" : ''). " />" .__("Preserve Hidden Pages (Do Not Uncheck unless you're going to delete the plugin)", $i18n_domain). "<br/>";
    echo "<input type='checkbox' name='preserve_database' id='preserve_database' " .((strcmp(get_option('wp_artistography_preserve_database'), NULL)) ? "checked='checked'" : ''). " />" .__("Preserve Database (DO NOT UNCHECK: will delete ALL artistography data between Updates or After Plugin Deactivation/Deletion)", $i18n_domain). "<br/>";
    echo "<br/><br/>";
    echo "<input type='submit' name='save' id='save' value='" .__("Save", $i18n_domain). "' />";
echo '</form>';
echo '</p>';
?>
