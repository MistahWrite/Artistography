<?php

 // Shortcode helpers
add_shortcode( 'artistography_display_albums', 'Music::shortCodeDisplayAlbums' );
add_shortcode( 'artistography_display_album', 'Music::shortCodeDisplayAlbum' );
add_shortcode( 'artistography_display_album_tracklist', 'Music::shortCodeDisplayAlbumTracklist' );
add_shortcode( 'artistography_display_album_artist', 'Music::shortCodeDisplayAlbumArtist' );

 // AJAX Administration
add_action('wp_ajax_Create_Album', 'Music::callback_Create_Album');
add_action('wp_ajax_Update_Album', 'Music::callback_Update_Album');
add_action('wp_ajax_Edit_Album', 'Music::callback_Edit_Album');
add_action('wp_ajax_Delete_Album', 'Music::callback_Delete_Album');

class Music
{
    protected static $musicTable = NULL;  // TABLE NAME of MUSIC ALBUM TABLE

     // property declaration
    public $id = NULL;

    public $artist_name = NULL;
    public $album_name = NULL;

    public $artist = NULL;
    public $artist_url = NULL;
    public $picture_url = NULL;
    public $store_url = NULL;

    public $price = NULL;

    public $download_id = NULL;
    public $total_downloads = NULL;
    public $description = NULL;

    public $album_year = NULL;
    public $album_month = NULL;
    public $album_day = NULL;
    public $album_date = NULL;

     // meta data property declarations
    public $last_updated_by_id = NULL;
    public $poster_id = NULL;
    public $create_date = NULL;
    public $update_date = NULL;

     // this class interfaces with the database
    protected $query = NULL;
    protected $query_result = NULL;
    protected $query_total_rows = NULL;
    protected $query_curr_node = NULL;

     // set funcs (protected)
    protected function setTotalRows ($rows) {    $this->query_total_rows = $rows;    }
    protected function setCurrNode ($node)  {    $this->query_curr_node = $node;    }

     // get funcs (public)
    public function getTotalRows ()   {    return $this->query_total_rows;    }
    public function getCurrNode ()    {    return $this->query_curr_node;    }
    public function getQueryResult () {    return $this->query_result;    }

    public static function callback_Create_Album(){
        GLOBAL $i18n_domain;

        $music = new Music;
        $music->insert($_POST['artist_name'],
                       $_POST['album_name'],
                       $music->formatDateTime($_POST['album_day'], $_POST['album_month'], $_POST['album_year']),
                       $_POST['artist_url'],
                       $_POST['picture_url'],
                       $_POST['store_url'],
		       $_POST['price'],
                       $_POST['download_id'],
                       $_POST['description']);

        if ($music->getTotalRows() <= 0) {
            _e("Failure...", $i18n_domain);  // failed
        } else {
            echo "1"; // success
        }

        unset($music);
        die;
    }

    public static function callback_Update_Album(){
        GLOBAL $i18n_domain;

        $album = new Music;

        $music_id = stripslashes($_POST['album_id']);
	$artist_name = stripslashes($_POST['artist_name']);
        $album_name = stripslashes($_POST['album_name']);
	$album_date = $album->formatDateTime($_POST['album_day'], $_POST['album_month'], $_POST['album_year']);
        $artist_url = stripslashes($_POST['artist_url']);
        $picture_url = stripslashes($_POST['picture_url']);
	$store_url = stripslashes($_POST['store_url']);
	$price = stripslashes($_POST['price']);
	$download_id = stripslashes($_POST['download_id']);
        $description = stripslashes($_POST['description']);

        if($album->updateById ($music_id, $artist_name, $album_name, $album_date, $artist_url, $picture_url, $store_url, $price, $download_id, $description) ) {

          echo '1'; //success
        } else {
          echo sprintf(__("Failed: Unable to Update Artist id: %u", $i18n_domain), $artist_id);
        }

        unset($album);
        die;
    }

    public static function callback_Edit_Album(){
        GLOBAL $i18n_domain;

        $album = new Music;

        if($album->loadById($_POST['album_id'])->getTotalRows() > 0) {

          echo $album->id
		."##$$##". stripslashes($album->artist_name)
		."##$$##". stripslashes($album->album_name)
		."##$$##". stripslashes($album->album_day)
		."##$$##". stripslashes($album->album_month)
		."##$$##". stripslashes($album->album_year)
		."##$$##". stripslashes($album->artist_url)
		."##$$##". stripslashes($album->picture_url)
		."##$$##". stripslashes($album->store_url)
		."##$$##". stripslashes($album->price)
		."##$$##". stripslashes($album->download_id)
		."##$$##". stripslashes($album->description) ."";
        }

        unset($album);
        die;
    }

    public static function callback_Delete_Album(){
        GLOBAL $i18n_domain;

        $album_id = $_POST['album_id'];

        $music = new Music;
        $music->deleteById($album_id);

        $discography = new Discography;
        $discography->deleteByAlbumId($album_id);

        if ($music->getTotalRows() > 0) {
            echo "1"; // success
        } else {
            echo sprintf(__("Failed: Unable to Delete Album id: %u", $i18n_domain), $album_id);  // failed
        }

        unset($music);
        die;
    }

     // [artistography_display_albums cols="4"]
    public static function shortCodeDisplayAlbums( $atts ) {
	extract( shortcode_atts( array(
		'cols' => '3',
		'size' => '200'
	), $atts ) );
	GLOBAL $download_icon_height, $download_icon_width, $download_icon_url;
        GLOBAL $add_to_cart_icon_height, $add_to_cart_icon_width, $add_to_cart_icon_url;
	$width = esc_attr($size). "px";
	$height = esc_attr($size). "px";

        $music = new Music;
        $html = "  <center>\n";
        $num = $music->loadAll('album_date DESC')->getTotalRows();
	$rows = ((int)($num / esc_attr($cols)) < (float)($num / esc_attr($cols))) ? (int)($num / esc_attr($cols)) + 1 : (int)($num / esc_attr($cols));

        if ($num <= 0) {
            $html .= __("No Music Available in the Database!", $i18n_domain);
        } else {

            for ($i = 0; $i < $rows; $i++) {
                for ($j = 0; $j < $cols AND ((($cols * ($i)) + ($j+1)) <= $num); $j++) {
                    $music->loadByNode(($i * $cols) + $j);
                    $html .= "[artistography_display_album id=$music->id size=" .esc_attr($size). "]";
                }

                for ($j; $j < $cols; $j++) {
                    $html .= "&nbsp;\n";
                }
                if ($i < ($rows-1)) {
                    $html .= "<hr style='width:100%;'>\n";
                }
            }
        }
        $html .= "  </center>\n";

        unset($music);
        return do_shortcode($html);
    }

     // [artistography_display_album id="1"]
    public static function shortCodeDisplayAlbum ( $atts ) {
	extract( shortcode_atts( array(
		'id' => '0',
		'size' => '200'
	), $atts ) );

        $width = esc_attr($size). "px";
        $height = esc_attr($size). "px";
	$html = "";

        $music = new Music;
        $music->loadById(esc_attr($id));

        $html .= "          <div style='float:left;'>\n"
              .  "            <center><font style='font-size:8pt'>\n"
              .  "            <a href='" .$music->artist_url. "'>" .$music->artist_name. "</a><br/>\n"
              .  "            <a href='" .$music->store_url. "'>" .$music->album_name. "</a><br/>\n"
              .  "            Released: <a href='" .$music->store_url. "'>" .$music->album_month.".".$music->album_day.".".$music->album_year. "</a><br/>\n"
              .  "            <a href='" .$music->store_url. "' target='_blank'><img src='" .$music->picture_url. "' style='border:1px solid black;height:" .$height. ";width:" .$width. ";' /></a><br/>\n"
              .  "[artistography_album_download_link id='$music->id']";
        $html .= "            </font></center>\n"
              .  "          </div>\n";

        unset($music);
        return do_shortcode($html);
    }

     // [artistography_display_album_tracklist id="1"]
    public static function shortCodeDisplayAlbumTracklist ( $atts ) {
	extract( shortcode_atts( array(
		'id' => '0'
	), $atts ) );

        $music = new Music;
        $music->loadById(esc_attr($id));

        $html = $music->description;

        unset($music);
        return $html;
    }

     // [artistography_display_album_artist id="1"]
    public static function shortCodeDisplayAlbumArtist( $atts ) {
	extract( shortcode_atts( array(
		'id' => '0'
	), $atts ) );

        $music = new Music;

        $html = $music->loadById(esc_attr($id))->album_year . " - " .$music->loadById(esc_attr($id))->artist_name. " - " .$music->loadById(esc_attr($id))->album_name;

        unset($music);
        return $html;
    }

    public function __construct() {
        GLOBAL $wpdb, $TABLE_NAME;

        self::$musicTable = $wpdb->prefix . $TABLE_NAME[TABLE_ARTIST_MUSIC_ALBUMS];

        $this->loadAll();

        $this->artist = new Artist;
    }

    public function __destruct() {
        unset($this->artist);
    }

    protected function &loadCurrNodeValues () {
        GLOBAL $wpdb;

        if($this->getTotalRows() > 0) {
            $this->query_result = $wpdb->get_row($this->query, OBJECT, $this->getCurrNode());

             // meta data info update
            $this->id = $this->query_result->id;
            $this->poster_id = $this->query_result->poster_id;
            $this->last_updated_by_id = $this->query_result->last_updated_by_id;
            $this->create_date = $this->query_result->create_date;
            $this->update_date = $this->query_result->update_date;

            $this->album_name = stripslashes($this->query_result->album_name);
            $this->artist_name = stripslashes($this->query_result->artist_name);

            $this->artist_url = stripslashes($this->query_result->artist_url);
            $this->picture_url = stripslashes($this->query_result->picture_url);
            $this->description = stripslashes($this->query_result->description);
            $this->store_url = stripslashes($this->query_result->store_url);

	    $this->price = stripslashes($this->query_result->price);

            $this->album_date = $this->query_result->album_date;
            $this->album_year = date("Y", strtotime($this->album_date));
            $this->album_month = date("m", strtotime($this->album_date));
            $this->album_day = date("d", strtotime($this->album_date));

            $this->download_id = $this->query_result->download_id;
	    if($this->download_id != NULL and $this->download_id != 0) {
	    	$downloads = new Download;
	    	$this->total_downloads = $downloads->loadById($this->download_id)->download_count;
	    } else {
                $this->total_downloads = NULL;
	    }

        }
        return $this;
    }

    public function formatDateTime($day, $month, $year, $hour = 0, $min = 0, $sec = 0) {
         // "YYYY-MM-DD HH:mm:SS";
        if(strlen($month) < 2) $month = "0$month";
        if(strlen($day) < 2) $day = "0$day";
        if(strlen($hour) < 2) $hour = "0$hour";
        if(strlen($min) < 2) $min = "0$min";
        if(strlen($sec) < 2) $sec = "0$sec";
        return "$year-$month-$day $hour:$min:$sec";
    }

    public function &getNodeNext () {
    // when this function is used, it's assumed that a query was already run
    // meant to simply load the nth item in the query through a for loop
    // assumes query, query_curr_node, and query_total_rows is set

        if (($this->query_curr_node + 1) < $this->getTotalRows()) {
            $this->query_curr_node += 1;
            $this->loadCurrNodeValues();
        }
        return $this;
    }

    public function &getNodePrev () {
    // when this function is used, it's assumed that a query was already run
    // meant to simply load the nth item in the query through a for loop
    // assumes query, query_curr_node, and query_total_rows is set

        if (($this->query_curr_node - 1) >= 0 AND $this->getTotalRows() > 0) {
            $this->query_curr_node -= 1;
            $this->loadCurrNodeValues();
        }
        return $this;
    }

    public function &loadByNode ($node) {
    // when this function is used, it's assumed that a query was already run
    // meant to simply load the nth item in the query through a for loop

        if ($node < $this->getTotalRows() AND $node >= 0 AND $this->getTotalRows() > 0) {
            $this->setCurrNode($node);
            $this->loadCurrNodeValues();
        }
        return $this;
    }

    public function &loadById ($id, $order_by = 'id') {
        GLOBAL $wpdb, $i18n_domain;

        $this->query = $wpdb->prepare("SELECT *
                        FROM " .self::$musicTable. "
                        WHERE id = %u
			ORDER BY $order_by", $id);
        $this->setTotalRows($wpdb->query($this->query));

        if ($this->getTotalRows() === FALSE) wp_die( sprintf(__('An error occurred while trying to perform a query: "%s"', $i18n_domain), $this->query) );

        $this->setCurrNode(0); // set to first node
        return $this->loadCurrNodeValues();
    }

    public function &loadAllNotFree ($order_by = 'id') {
        GLOBAL $wpdb, $i18n_domain;

        $this->query = "SELECT *
                        FROM " .self::$musicTable. "
                        WHERE price != '0.00'
                        ORDER BY $order_by";
        $this->setTotalRows($wpdb->query($this->query));

        if ($this->getTotalRows() === FALSE) wp_die( sprintf(__('An error occurred while trying to perform a query: "%s"', $i18n_domain), $this->query) );

        $this->setCurrNode(0); // set to first node
        return $this->loadCurrNodeValues();
    }

    public function &loadAllLinked ($order_by = 'id') {
        GLOBAL $wpdb, $i18n_domain;

        $this->query = "SELECT *
                        FROM " .self::$musicTable. "
                        WHERE download_id > 0
                        ORDER BY $order_by";
        $this->setTotalRows($wpdb->query($this->query));

        if ($this->getTotalRows() === FALSE) wp_die( sprintf(__('An error occurred while trying to perform a query: "%s"', $i18n_domain), $this->query) );

        $this->setCurrNode(0); // set to first node
        return $this->loadCurrNodeValues();
    }

    public function &loadByDownloadId ($download_id, $order_by = 'id') {
        GLOBAL $wpdb, $i18n_domain;

        $this->query = $wpdb->prepare("SELECT *
                        FROM " .self::$musicTable. "
                        WHERE download_id = %u
			ORDER BY $order_by", $download_id);
        $this->setTotalRows($wpdb->query($this->query));

        if ($this->getTotalRows() === FALSE) wp_die( sprintf(__('An error occurred while trying to perform a query: "%s"', $i18n_domain), $this->query) );

        $this->setCurrNode(0); // set to first node
        return $this->loadCurrNodeValues();
    }

    public function &loadAll ($order_by = 'id') {
        GLOBAL $wpdb, $i18n_domain;

        $this->query = "SELECT *
                        FROM " .self::$musicTable. "
                        ORDER BY $order_by";
        $this->setTotalRows($wpdb->query($this->query));

        if ($this->getTotalRows() === FALSE) {
            wp_die( sprintf(__('An error occurred while trying to perform a query: "%s"', $i18n_domain), $this->query) );
        }

        $this->setCurrNode(0); // set to first node
        return $this->loadCurrNodeValues();
    }

    public function incrementPageViewsById ($id) {
        GLOBAL $wpdb, $i18n_domain;

        $this->query = $wpdb->prepare(
            "UPDATE " .self::$musicTable. "
             SET page_views = page_views + 1
             WHERE id = %u", $id);
        $this->setTotalRows($wpdb->query($this->query));

        if ($this->getTotalRows() === FALSE) {
            wp_die( sprintf(__('An error occurred while trying to perform a query: "%s"', $i18n_domain), $this->query) );
        } else {
             // was entered successfully into database
            return $this->getTotalRows();
        }
    }

    public function deleteById ($id) {
        GLOBAL $wpdb, $i18n_domain;

        $this->query = $wpdb->prepare(
                       "DELETE FROM " .self::$musicTable. "
                        WHERE id = %u", $id);
        $this->setTotalRows($wpdb->query($this->query));

        if ($this->getTotalRows() === FALSE) {
            wp_die( sprintf(__('An error occurred while trying to perform a query: "%s"', $i18n_domain), $this->query) );
        }
        return $this->getTotalRows();
    }

     // update music album entry
    public function updateById ($music_id, $artist_name, $album_name, $album_date, $artist_url, $picture_url, $store_url, $price, $download_id, $description) {
        GLOBAL $wpdb, $i18n_domain, $current_user;

        get_currentuserinfo();

        $this->query = $wpdb->prepare(
            "UPDATE " .self::$musicTable. "
             SET update_date = now(),
                 artist_name = %s,
                 album_name = %s,
                 album_date = %s,
                 artist_url = %s,
                 picture_url = %s,
                 store_url = %s,
                 price = %s,
                 download_id = %u,
                 description = %s,
                 last_updated_by_id = %u
             WHERE id = %u",
             $artist_name, $album_name, $album_date, $artist_url, $picture_url, $store_url, $price, $download_id, $description, $current_user->ID, $music_id);
        $this->setTotalRows($wpdb->query($this->query));

        if ($this->getTotalRows() === FALSE) {
            wp_die( sprintf(__('An error occurred while trying to perform a query: "%s"', $i18n_domain), $this->query) );
        } else {
             // was entered successfully into database
            return true;
        }
    }

    public function insert ( $artist_name, $album_name, $album_date, $artist_url, $picture_url, $store_url, $price, $download_id, $description) {
        GLOBAL $wpdb, $i18n_domain, $current_user;

        get_currentuserinfo();

        $this->query = $wpdb->prepare(
                       "INSERT INTO " .self::$musicTable. "
                        (create_date, update_date, poster_id, last_updated_by_id, artist_name, album_name, album_date, artist_url, picture_url, store_url, price, download_id, description)
                        VALUES (now(), now(), %u, %u, %s, %s, %s, %s, %s, %s, %s, %u, %s)",
                        $current_user->ID, $current_user->ID, $artist_name, $album_name, $album_date, $artist_url, $picture_url, $store_url, $price, $download_id, $description);
        $this->setTotalRows($wpdb->query($this->query));

        if ($this->getTotalRows() === 0 OR $this->getTotalRows() === FALSE) {
            wp_die( sprintf(__('An error occurred while trying to perform a query: "%s"', $i18n_domain), $this->query) );
        } else {
             // was entered successfully into database
            return $wpdb->insert_id;
        }
    }
}  /* end class Music */
?>
