<?php

 // Short code
add_shortcode( 'artistography_playlist', 'SongAlbumLinker::shortCodePlaylist' );

 // AJAX Administration Drag/Drop Discography Auto-Save feature
add_action('wp_ajax_Connect_Song_to_Album', 'SongAlbumLinker::callback_Connect_Song_to_Album');
add_action('wp_ajax_Disconnect_Song_from_Album', 'SongAlbumLinker::callback_Disconnect_Song_from_Album');

 // AJAX Playlist image
add_action('wp_ajax_Load_Album_Art', 'SongAlbumLinker::callback_Load_Album_Art');
add_action('wp_ajax_nopriv_Load_Album_Art', 'SongAlbumLinker::callback_Load_Album_Art');

class SongAlbumLinker
{
    protected static $songalbumTable = NULL;  // TABLE NAME of DISCOGRAPHY TABLE
    protected static $songTable = NULL;

     // property declaration
    public $id = NULL;

    public $song_id = NULL;
    public $album_id = NULL;

    public $song = NULL;
    public $album = NULL;

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

    public function __construct() {
        GLOBAL $wpdb, $TABLE_NAME;

        self::$songalbumTable = $wpdb->prefix . $TABLE_NAME[TABLE_SONG_ALBUM_LINKER];
	self::$songTable = $wpdb->prefix . $TABLE_NAME[TABLE_SONGS];
        $this->loadAll();

        $this->song = new Song;
        $this->album = new Music;
    }

    public function __destruct() {
        unset($this->song);
        unset($this->album);
    }

    public static function callback_Connect_Song_to_Album(){
        GLOBAL $i18n_domain;

        $song_id = $_POST['song_id'];
        $album_id = $_POST['album_id'];

        $songalbumlinker = new SongAlbumLinker;
        $songalbumlinker->loadAllBySongAlbum($song_id, $album_id);

        if ($songalbumlinker->getTotalRows() > 0) {
            _e("The Song is already attached to that album.", $i18n_domain);  // failed
        } else {
            $songalbumlinker->insert($song_id, $album_id);
            echo "1"; // success
        }

        unset($songalbumlinker);
        die;
    }

    public static function callback_Disconnect_Song_from_Album(){
        GLOBAL $i18n_domain;

        $song_id = $_POST['song_id'];
        $album_id = $_POST['album_id'];

        $songalbumlinker = new SongAlbumLinker;
        $songalbumlinker->loadAllBySongAlbum($song_id, $album_id);

        if ($songalbumlinker->getTotalRows() > 0) {
            $songalbumlinker->deleteById($songalbumlinker->id);
            echo "1"; // success
        } else {
            _e("Failed to Disconnect Song from Album as it appears there currently is no connection between them on the server.  "
             . "It's possible another administrator may be using this feature at the same time.", $i18n_domain);  // failed
        }

        unset($songalbumlinker);
        die;
    }

    public static function callback_Load_Album_Art(){
        GLOBAL $i18n_domain;
	GLOBAL $transparent_pixel_url;

        $song_id = $_POST['song_id'];

	$album = new Music;
        $songalbumlinker = new SongAlbumLinker;
        $songalbumlinker->loadBySongId($song_id);

        if ($songalbumlinker->getTotalRows() > 0) {
            echo $album->loadById($songalbumlinker->album_id)->picture_url; // success
        } else {
            echo $transparent_pixel_url;
        }

	unset($album);
        unset($songalbumlinker);
        die;
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

     // [artistography_playlist id="1"]
     // id is the album_id
    public static function shortCodePlaylist( $atts ) {
        extract( shortcode_atts( array(
                'id' => '1'
        ), $atts ) );

	$songalbumlinker = new SongAlbumLinker;
	$html = "";

	 // load all songs by album id and sort by track number
        $songalbumlinker->loadByAlbumIdJoinSong(esc_attr($id));

	for ($i = 0; $i < $songalbumlinker->getTotalRows(); $i++) {
		$html .= "[artistography_display_song id='$songalbumlinker->song_id']";
		$songalbumlinker->getNodeNext();
	}

        unset($songalbumlinker);

        return do_shortcode($html);
    }

    protected function &loadCurrNodeValues () {
        GLOBAL $wpdb;

        if ($this->getTotalRows() > 0) {
            $this->query_result = $wpdb->get_row($this->query, OBJECT, $this->getCurrNode());

             // meta data info update
            $this->id = $this->query_result->id;
            $this->poster_id = $this->query_result->poster_id;
            $this->last_updated_by_id = $this->query_result->last_updated_by_id;
            $this->create_date = $this->query_result->create_date;
            $this->update_date = $this->query_result->update_date;

            $this->song_id = $this->query_result->song_id;
            $this->album_id = $this->query_result->album_id;

            if($this->song_id > 0) {
//                $this->artist->loadById($this->artist_id);
            }

            if($this->album_id > 0) {
//                $this->album->loadById($this->album_id);
            }
        } else {
            $this->query_result = 

            $this->id =
            $this->poster_id =
            $this->last_updated_by_id =
            $this->create_date =
            $this->update_date =

            $this->song_id =
            $this->album_id = 0;
        }
        return $this;
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

    public function &loadById ($id) {
        GLOBAL $wpdb, $i18n_domain;

        $this->query = $wpdb->prepare("SELECT *
                        FROM  " .self::$songalbumTable. "
                        WHERE id = %u", $id);
        $this->setTotalRows($wpdb->query($this->query));

        if ($this->getTotalRows() === FALSE) wp_die( sprintf(__('An error occurred while trying to perform a query: "%s"', $i18n_domain), $this->query) );

        $this->setCurrNode(0); // set to first node
        return $this->loadCurrNodeValues();
    }

    public function &loadBySongId ($song_id, $order_by = 'id') {
        GLOBAL $wpdb, $i18n_domain;

        $this->query = $wpdb->prepare("SELECT *
                        FROM  " .self::$songalbumTable. "
                        WHERE song_id = %u
                        ORDER BY $order_by",
                        $song_id);
        $this->setTotalRows($wpdb->query($this->query));

        if ($this->getTotalRows() === FALSE) wp_die( sprintf(__('An error occurred while trying to perform a query: "%s"', $i18n_domain), $this->query) );

        $this->setCurrNode(0); // set to first node
        return $this->loadCurrNodeValues();
    }

    public function &loadByAlbumId ($album_id, $order_by = 'id') {
        GLOBAL $wpdb, $i18n_domain;

        $this->query = $wpdb->prepare("SELECT *
                        FROM  " .self::$songalbumTable. "
                        WHERE album_id = %u
                        ORDER BY $order_by",
                        $album_id);
        $this->setTotalRows($wpdb->query($this->query));

        if ($this->getTotalRows() === FALSE) wp_die( sprintf(__('An error occurred while trying to perform a query: "%s"', $i18n_domain), $this->query) );

        $this->setCurrNode(0); // set to first node
        return $this->loadCurrNodeValues();
    }

    public function &loadByAlbumIdJoinSong ($album_id, $order_by = 's.number') {
        GLOBAL $wpdb, $i18n_domain;

        $this->query = $wpdb->prepare("SELECT *
                        FROM  " .self::$songalbumTable. " sa
			RIGHT JOIN " .self::$songTable. " s
			ON sa.song_id = s.id
                        WHERE album_id = %u
                        ORDER BY $order_by",
                        $album_id);
        $this->setTotalRows($wpdb->query($this->query));

        if ($this->getTotalRows() === FALSE) wp_die( sprintf(__('An error occurred while trying to perform a query: "%s"', $i18n_domain), $this->query) );

        $this->setCurrNode(0); // set to first node
        return $this->loadCurrNodeValues();
    }

    public function &loadAllBySongAlbum ($song_id, $album_id, $order_by = 'id') {
        GLOBAL $wpdb, $i18n_domain;

        $this->query = $wpdb->prepare("SELECT *
                        FROM  " .self::$songalbumTable. "
                        WHERE song_id = %u
                          AND album_id = %u
                        ORDER BY $order_by",
                        $song_id, $album_id);
        $this->setTotalRows($wpdb->query($this->query));

        if ($this->getTotalRows() === FALSE) wp_die( sprintf(__('An error occurred while trying to perform a query: "%s"', $i18n_domain), $this->query) );

        $this->setCurrNode(0); // set to first node
        return $this->loadCurrNodeValues();
    }

    public function &loadAll ($order_by = 'id') {
        GLOBAL $wpdb, $i18n_domain;

        $this->query = "SELECT *
                        FROM " .self::$songalbumTable. "
                        ORDER BY $order_by";
        $this->setTotalRows($wpdb->query($this->query));

        if ($this->getTotalRows() === FALSE) wp_die( sprintf(__('An error occurred while trying to perform a query: "%s"', $i18n_domain), $this->query) );

        $this->setCurrNode(0); // set to first node
        return $this->loadCurrNodeValues();
    }

    public function deleteById ($id) {
        GLOBAL $wpdb, $i18n_domain;

        $this->query = $wpdb->prepare(
                       "DELETE FROM " .self::$songalbumTable. "
                        WHERE id = %u", $id);
        $this->setTotalRows($wpdb->query($this->query));

        if ($this->getTotalRows() === FALSE) {
            wp_die( sprintf(__('An error occurred while trying to perform a query: "%s"', $i18n_domain), $this->query) );
        }
        return $this->getTotalRows();
    }

    public function deleteBySongId ($id) {
        GLOBAL $wpdb, $i18n_domain;

        $this->query = $wpdb->prepare(
                       "DELETE FROM " .self::$songalbumTable. "
                        WHERE song_id = %u", $id);
        $this->setTotalRows($wpdb->query($this->query));

        if ($this->getTotalRows() === FALSE) {
            wp_die( sprintf(__('An error occurred while trying to perform a query: "%s"', $i18n_domain), $this->query) );
        }
        return $this->getTotalRows();
    }

    public function deleteByAlbumId ($id) {
        GLOBAL $wpdb, $i18n_domain;

        $this->query = $wpdb->prepare(
                       "DELETE FROM " .self::$songalbumTable. "
                        WHERE album_id = %u", $id);
        $this->setTotalRows($wpdb->query($this->query));

        if ($this->getTotalRows() === FALSE) {
            wp_die( sprintf(__('An error occurred while trying to perform a query: "%s"', $i18n_domain), $this->query) );
        }
        return $this->getTotalRows();
    }

     // update music album entry
    public function updateById ($songalbum_id, $song_id, $album_id) {
        GLOBAL $wpdb, $i18n_domain, $current_user;

        get_currentuserinfo();

        $this->query = $wpdb->prepare(
            "UPDATE " .self::$songalbumTable. "
             SET update_date = now(),
                 song_id = %u,
                 album_id = %u,
                 last_updated_by_id = %u
             WHERE id = %u",
             $song_id, $album_id, $current_user->ID, $discography_id);
        $this->setTotalRows($wpdb->query($this->query));

        if ($this->getTotalRows() === FALSE) {
            wp_die( sprintf(__('An error occurred while trying to perform a query: "%s"', $i18n_domain), $this->query) );
        } else {
             // was entered successfully into database
            return $this->getTotalRows();
        }
    }

    public function insert ($song_id, $album_id) {
        GLOBAL $wpdb, $i18n_domain, $current_user;

        get_currentuserinfo();

        $this->query = $wpdb->prepare(
                       "INSERT INTO " .self::$songalbumTable. "
                        (create_date, update_date, poster_id, last_updated_by_id, song_id, album_id)
                        VALUES (now(), now(), %u, %u, %u, %u)",
                        $current_user->ID, $current_user->ID, $song_id, $album_id);
        $this->setTotalRows($wpdb->query($this->query));

        if ($this->getTotalRows() === 0 OR $this->getTotalRows() === FALSE) {
            wp_die( sprintf(__('An error occurred while trying to perform a query: "%s"', $i18n_domain), $this->query) );
        } else {
             // was entered successfully into database
            return true;
        }
    }

}  /* end class Song-Album-Linker */
?>
