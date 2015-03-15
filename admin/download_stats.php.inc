<?php
    echo "<h3>Downloads</h3>\n";

    $thetable = $wpdb->prefix . $TABLE_NAME[TABLE_ARTIST_FILE_DOWNLOAD];
    $query = "SELECT *
              FROM $thetable";
    $num = $wpdb->query($query);

    echo '<table style="border:1px dash black;width:100%;">';
    echo '<tr><th style="border:2px solid black;">' .__('ID', $i18n_domain).'</th>';
    echo '<th style="border:2px solid black;">' .__('Filename', $i18n_domain). '</th>';
    echo '<th style="border:2px solid black;">' .__('Times Downloaded', $i18n_domain). '</th></tr>';

    if ($num === 0 || $num === FALSE) {
      echo '<tr><td colspan="3" style="border:2px solid black;text-align:center;">';
      _e('No Downloads Available in the Database!', $i18n_domain);
      echo '</td></tr>';
    } else {
        for($i = 0; $i < $num; $i++) {
            $row = $wpdb->get_row($query, ARRAY_A, $i);
            $id = $row['id'];
            $filename = $row['file_name'];
            $counter = $row['download_count'];

            echo "<tr><td style='border:2px solid black;'><a href='" .SITEURL. "/artistography_download/$id' target='_blank'>$id</a></td>";
            echo "  <td style='border:2px solid black;'><a href='" .SITEURL. "$download_folder$filename' target='_blank'>$filename</a></td>";
            echo "  <td style='border:2px solid black;'>$counter</td></tr>";
            unset($row);
        }
    }
    echo '</table>';
?>
