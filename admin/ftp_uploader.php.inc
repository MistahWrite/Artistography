<?php
  GLOBAL $i18n_domain, $download_folder;

if (strcmp(get_option('wp_artistography_email_notify_ftp'), NULL)) {
  $IP_Address = $_SERVER['REMOTE_ADDR'];
  $website = $_SERVER['SERVER_NAME'];

  $email = get_option( 'admin_email' );
  $subject = __("Artistography FTP Uploader", $i18n_domain);
  $message = __("This is an automatically generated email to let you know somebody used the Artistography>FTP Uploader.", $i18n_domain)
           . __("Website: ", $i18n_domain). ": $website\n"
           . __("IP Address", $i18n_domain). ": http://whatismyipaddress.com/ip/$IP_Address\n"
           . __("Time", $i18n_domain). ": " . date("D M j G:i:s T Y") . "\n\n\n";

  mail($email, $subject, $message);
}
?>
<div align="left">
<applet code="ZUpload" Archive="<? echo plugins_url('artistography/ftp/ZUpload.jar', 'artistography'); ?>" width="450" height="300" border="0">
<param name="host" value="<? echo get_option('wp_artistography_ftp_host'); ?>">
<param name="user" value="<? echo get_option('wp_artistography_ftp_user'); ?>">
<param name="pass" value="<? echo get_option('wp_artistography_ftp_pass'); ?>">
<param name="path" value="<? echo get_option('wp_artistography_ftp_path'); ?>">
<param name="postscript" value="url_of_script">
</applet>
</div><div align="right">
<iframe src='<? echo get_option('siteurl') . $download_folder; ?>' height="400" width="100%"></iframe>
</div>
