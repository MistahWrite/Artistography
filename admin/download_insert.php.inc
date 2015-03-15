<?php
echo "download path = " .$download_path ."<br/>";

$download = new Download;

if(array_key_exists('do_choose', $_POST)) if ($_POST['do_choose'] && !strcmp($_POST['file'], "")) {
  _e("No file Chosen.<br/>\n", $i18n_domain);
} else if ($_POST['do_choose']) {
  $file = $_POST['file'];

  if (!$download->insert($file)) {
    wp_die( __('An error occurred while trying to perform the insert query.', $i18n_domain) );
  } else {
    // successfully inserted file to upload
    printf( __("File added to the database: '%s'<br/>\n", $i18n_domain), $file);
  }

  echo '<center><form method="post" action=""><table><tr><td><input type="submit" value="';
  _e('Select A File To Add To Download Manager', $i18n_domain);
  echo '" name="do_select" /></td></tr></table></form></center>';
}
else if($_POST['do_select']) {
  $num = $download->loadAll()->getTotalRows();
  // read files from DB
  for($i = 0; $i < $num; $i++) {
    $files_db[$i]['id'] = $download->id;
    $files_db[$i]['file_name'] = $download->file_name;
  }
  echo "<form method='post' action=''><table>";
   // read files from $download_path
   // don't display files if we already added them to the database
  if ($handle = opendir($download_path)) {
    while (false !== ($file = readdir($handle))) {
      if(!strcmp($file, ".") || !strcmp($file, "..")) continue;
      $found = false;
      for ($i = 0; $i < $num; $i++) {
        if(!strcmp($files_db[$i]['file_name'], $file)) {
          $id = $files_db[$i]['id'];
           // do not display
          $found = true;
        }
      }
      if($found != true && !is_dir($file)) {
         // is it already in the database, and is this a directory or a file?
        echo "<tr><td><input type='radio' name='file' value='$file' /></td><td>$file [" .round(((filesize($download_path . $file) / 1024) / 1024), 2). " MB]</td></tr>\n";
      }
    } // end while
    closedir($handle);
    echo '<tr><td></td><Td><input type="submit" value="';
    _e('Select File!', $i18n_domain);
    echo '" name="do_choose" /></td></tr></table></form>';
  }
}
else {
  echo '<center><form method="post" action=""><table><tr><td><input type="submit" value="';
  _e('Select A File To Add To Download Manager', $i18n_domain);
  echo '" name="do_select" /></td></tr></table></form></center>';
}

?>
