<?php
class Video
{
    protected static $artistTable = NULL;  // TABLE NAME of ARTIST TABLE
    protected static $musicTable = NULL;  // TABLE NAME of MUSIC ALBUM TABLE
    protected static $videoTable = NULL;  // TABLE NAME of MUSIC ALBUM TABLE

     // property declaration
    public $id = NULL;
    public $enabled = NULL;

    public $artist_name = NULL;

    public $artist = NULL;
    public $artist_id = NULL;
    public $artist_url = NULL;

    public $title = NULL;
    public $description = NULL;

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

        self::$artistTable = $wpdb->prefix . $TABLE_NAME[TABLE_ARTISTS];
        self::$videoTable = $wpdb->prefix . $TABLE_NAME[TABLE_ARTIST_MUSIC_ALBUMS];
        self::$videoTable = $wpdb->prefix . $TABLE_NAME[TABLE_ARTIST_VIDEO];

        $this->loadAll();

        $this->artist = new Artist;
    }

    public function __destruct() {
        unset($this->artist);
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

    protected function &loadCurrNodeValues () {
        GLOBAL $wpdb;

        if ($this->getTotalRows() > 0) {
            $this->query_result = $wpdb->get_row($this->query, OBJECT, $this->getCurrNode());

             // meta data info update
            $this->id = $this->query_result->id;
            $this->enabled = $this->query_result->enabled;
            $this->poster_id = $this->query_result->poster_id;
            $this->last_updated_by_id = $this->query_result->last_updated_by_id;
            $this->create_date = $this->query_result->create_date;
            $this->update_date = $this->query_result->update_date;

            $this->artist_id = $this->query_result->artist_id;
            $this->artist_name = $this->query_result->artist_name;
            $this->artist_url = $this->query_result->artist_url;

            $this->description = $this->query_result->description;

            if($this->artist_id !== 0) {
                $this->artist->loadById($this->artist_id);
            }
        } else {
            $this->query_result =

            $this->id =
            $this->enabled =
            $this->poster_id =
            $this->last_updated_by_id =
            $this->create_date =
            $this->update_date =

            $this->artist_id =
            $this->artist_name =
            $this->artist_url =

            $this->description = 0;
        }

        return $this;
    }

    public function &getNodeNext () {
    // when this function is used, it's assumed that a query was already run
    // meant to simply load the nth item in the query through a for loop
    // assumes query, query_curr_node, and query_total_rows is set

        if (($this->query_curr_node + 1) <= $this->getTotalRows()) {
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
                        FROM  " .self::$videoTable. "
                        WHERE id = %u", $id);
        $this->setTotalRows($wpdb->query($this->query));

        if ($this->getTotalRows() === FALSE) {
            wp_die( sprintf(__('An error occurred while trying to perform a query: "%s"', $i18n_domain), $this->query) );
        } else if ($this->getTotalRows() > 0) {
             // set class variables to first node of query
            $this->setCurrNode(0); // set to first node
            $this->loadCurrNodeValues();
        }
        return $this;
    }

    public function &loadAll () {
        GLOBAL $wpdb, $i18n_domain;

        $this->query = "SELECT *
                        FROM " .self::$videoTable;
        $this->setTotalRows($wpdb->query($this->query));

        if ($this->getTotalRows() === FALSE) wp_die( sprintf(__('An error occurred while trying to perform a query: "%s"', $i18n_domain), $this->query) );

        $this->setCurrNode(0); // set to first node
        return $this->loadCurrNodeValues();
    }

    public function &loadAllEnabled () {
        GLOBAL $wpdb, $i18n_domain;

        $this->query = "SELECT *
                        FROM " .self::$videoTable. "
                        WHERE enabled = true";
        $this->setTotalRows($wpdb->query($this->query));

        if ($this->getTotalRows() === FALSE) wp_die( sprintf(__('An error occurred while trying to perform a query: "%s"', $i18n_domain), $this->query) );

        $this->setCurrNode(0); // set to first node
        return $this->loadCurrNodeValues();
    }

    public function &loadAllDisabled () {
        GLOBAL $wpdb, $i18n_domain;

        $this->query = "SELECT *
                        FROM " .self::$videoTable. "
                        WHERE enabled = false";
        $this->setTotalRows($wpdb->query($this->query));

        if ($this->getTotalRows() === FALSE) wp_die( sprintf(__('An error occurred while trying to perform a query: "%s"', $i18n_domain), $this->query) );

        $this->setCurrNode(0); // set to first node
        return $this->loadCurrNodeValues();
    }

    public function incrementPageViewsById ($id) {
        GLOBAL $wpdb, $i18n_domain;

        $this->query = $wpdb->prepare(
            "UPDATE " .self::$videoTable. "
             SET page_views = page_views + 1
             WHERE id = %u",
             $id);
        $this->setTotalRows($wpdb->query($this->query));

        if ($this->getTotalRows() === FALSE) {
            wp_die( sprintf(__('An error occurred while trying to perform a query: "%s"', $i18n_domain), $this->query) );
        } else {
             // was entered successfully into database
            return $this->getTotalRows();
        }
    }

     // update music album entry
    public function updateById ($music_id, $artist_id, $artist_name, $album_name, $album_date, $artist_url, $picture_url, $store_url, $download_id, $free_download_enabled, $featured, $description, $enabled) {
        GLOBAL $wpdb, $i18n_domain, $current_user;

        get_currentuserinfo();

/* TODO FIX THIS FUNCTION */
        $this->query = $wpdb->prepare(
            "UPDATE " .self::$videoTable. "
             SET update_date = now(),
                 artist_id = %u,
                 artist_name = %s,
                 album_name = %s,
                 album_date = %s,
                 artist_url = %s,
                 picture_url = %s,
                 store_url = %s,
                 download_id = %u,
                 free_download_enabled = %b,
                 featured = %b,
                 description = %s,
                 enabled = %b,
                 last_updated_by_id = %u
             WHERE id = %u",
             $artist_id, $artist_name, $album_name, $album_date, $artist_url, $picture_url, $store_url, $download_id, $free_download_enabled, $featured, $description, $enabled, $current_user->ID, $music_id);
        $this->setTotalRows($wpdb->query($this->query));

        if ($this->getTotalRows() === FALSE) {
            wp_die( sprintf(__('An error occurred while trying to perform a query: "%s"', $i18n_domain), $this->query) );
        } else {
             // was entered successfully into database
            return $this->getTotalRows();
        }
    }

    public function insert ($artist_id, $artist_name, $artist_url, $title, $description, $enabled) {
        GLOBAL $wpdb, $i18n_domain, $current_user;

        get_currentuserinfo();

        $this->query = $wpdb->prepare(
                       "INSERT INTO " .(string)self::$videoTable. "
                        (create_date, update_date, poster_id, last_updated_by_id, artist_id, artist_name, artist_url, title, description, enabled)
                        VALUES (now(), now(), %u, %u, %u, %s, %s, %s, %s, %b)",
                        $current_user->ID, $current_user->ID, $artist_id, $artist_name, $artist_url, $title, $description, $enabled);
        $this->setTotalRows($wpdb->query($this->query));

        if ($this->getTotalRows() === 0 OR $this->getTotalRows() === FALSE) {
            wp_die( sprintf(__('An error occurred while trying to perform a query: "%s"', $i18n_domain), $this->query) );
        } else {
             // was entered successfully into database
            return true;
        }
    }

}  /* end class Video */
?>