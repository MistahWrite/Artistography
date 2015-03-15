<?php
 // Shortcode helpers
add_filter( 'the_posts', 'Gallery::conditionally_add_scripts_and_styles'); // the_posts gets triggered before wp_head

 // AJAX Administration
add_action('wp_ajax_Create_Gallery', 'Gallery::callback_Create_Gallery');
add_action('wp_ajax_Update_Gallery', 'Gallery::callback_Update_Gallery');
add_action('wp_ajax_Edit_Gallery', 'Gallery::callback_Edit_Gallery');
add_action('wp_ajax_Delete_Gallery', 'Gallery::callback_Delete_Gallery');

class Gallery
{
    protected static $galleryTable = NULL;  // TABLE NAME of ARTIST TABLE

     // property declaration
    public $id = NULL;
    public $page_views = NULL;

    public $artist_id = NULL;
    public $name = NULL;
    public $gallery = NULL;
    public $cover_picture_url = NULL;
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

    public static function callback_Create_Gallery(){
        GLOBAL $i18n_domain;

        $gallery = new Gallery;

        $artist_id = stripslashes($_POST['artist_id']);
        $gallery_name = stripslashes($_POST['gallery_name']);
        $gallery_ids = stripslashes($_POST['gallery']);
        $picture_url = stripslashes($_POST['picture_url']);
        $gallery_descr = stripslashes($_POST['gallery_descr']);

        $result = $gallery->insert($artist_id, $gallery_name, $gallery_ids, $picture_url, $gallery_descr);
        if(is_numeric($result)) {
          echo $result;
        } else {
          echo sprintf(__("Failed: %s", $i18n_domain), $result);  // failed
        }

        unset($gallery);
        die;
    }

    public static function callback_Update_Gallery(){
        GLOBAL $i18n_domain;

        $gallery = new Gallery;

	$gallery_id = stripslashes($_POST['gallery_id']);
        $artist_id = stripslashes($_POST['artist_id']);
        $gallery_name = stripslashes($_POST['gallery_name']);
        $gallery_ids = stripslashes($_POST['gallery']);
        $picture_url = stripslashes($_POST['picture_url']);
        $gallery_descr = stripslashes($_POST['gallery_descr']);

        if($gallery->updateById ($gallery_id, $artist_id, $gallery_name, $gallery_ids, $picture_url, $gallery_descr) ) {
          echo '1'; //success
        } else {
          echo sprintf(__("Failed: Unable to Update Artist id: %u", $i18n_domain), $artist_id);
        }

        unset($gallery);
        die;
    }

    public static function callback_Edit_Gallery(){
        GLOBAL $i18n_domain;

        $gallery = new Gallery;

        if($gallery->loadById($_POST['gallery_id'])->getTotalRows() > 0) {

          echo $gallery->id
              ."##$$##". stripslashes($gallery->artist_id)
              ."##$$##". stripslashes($gallery->name)
              ."##$$##".  stripslashes($gallery->cover_picture_url)
              ."##$$##".  stripslashes($gallery->gallery)
              ."##$$##".  stripslashes($gallery->description) ."";
        }

        unset($gallery);
        die;
    }

    public static function callback_Delete_Gallery(){
        GLOBAL $i18n_domain;

        $gallery_id = $_POST['gallery_id'];

        $gallery = new Gallery;
        $gallery->deleteById($gallery_id);

        if ($gallery->getTotalRows() > 0) {
            echo "1"; // success
        } else {
            echo sprintf(__("Failed: Unable to Delete Gallery id: %u", $i18n_domain), $gallery_id);  // failed
        }

        unset($gallery);
        die;
    }

    public static function conditionally_add_scripts_and_styles($posts){
        GLOBAL $artistography_plugin_dir;
         // scripts are already being loaded on every page
        if (true == get_option('wp_artistography_ajax_loader')) return $posts;
        if (empty($posts)) return $posts;
 
        $shortcode_found = false; // use this flag to see if styles and scripts need to be enqueued
        foreach ($posts as $the_post) {
            if (FALSE !== stripos($the_post->post_content, '[artistography_display_enabled_artists')) {
                $shortcode_found = true; // bingo!
                break;
            }
        }

        if ($shortcode_found) {
           // enqueue here
          artistography_enqueue_style_and_scripts();
        }
 
        return $posts;
    }

    public function __construct() {
        GLOBAL $wpdb, $TABLE_NAME;

        self::$galleryTable = $wpdb->prefix . $TABLE_NAME[TABLE_ARTIST_IMAGE_GALLERIES];

        $this->loadAll();
    }

    public function __destruct() {
        /* This is a placeholder just in case it's needed eventually */
    }

    protected function &loadCurrNodeValues () {
        GLOBAL $wpdb, $_SERVER;

        if ($this->getTotalRows() > 0) {
            $this->query_result = $wpdb->get_row($this->query, OBJECT, $this->getCurrNode());

             // meta data info update
            $this->id = $this->query_result->id;
            $this->page_views = $this->query_result->page_views;

            $this->poster_id = $this->query_result->poster_id;
            $this->last_updated_by_id = $this->query_result->last_updated_by_id;
            $this->create_date = $this->query_result->create_date;
            $this->update_date = $this->query_result->update_date;

            $this->name = $this->query_result->name;
	    $this->artist_id = $this->query_result->artist_id;
            $this->gallery = $this->query_result->gallery;
            $this->cover_picture_url = $this->query_result->cover_picture;
            $this->description = $this->query_result->description;
        } else {
            $this->query_result = 

            $this->id = 
            $this->page_views =
            $this->poster_id = 
            $this->last_updated_by_id = 
            $this->create_date = 
            $this->update_date = 

            $this->name =
            $this->artist_id =
            $this->gallery =
            $this->cover_picture_url =
            $this->description = 0; 
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
                        FROM " .self::$galleryTable. "
                        WHERE id = %u", $id);
        $this->setTotalRows($wpdb->query($this->query));

        if ($this->getTotalRows() === FALSE) wp_die( sprintf(__('An error occurred while trying to perform a query: "%s"', $i18n_domain), $this->query) );

        $this->setCurrNode(0); // set to first node
        return $this->loadCurrNodeValues();
    }

    public function &loadLastInsertId () {
        GLOBAL $wpdb, $i18n_domain;

        $this->query = $wpdb->prepare("SELECT LAST_INSERT_ID()");
        $this->setTotalRows($wpdb->query($this->query));

        if ($this->getTotalRows() === FALSE) wp_die( sprintf(__('An error occurred while trying to perform a query: "%s"', $i18n_domain), $this->query) );

        $this->setCurrNode(0); // set to first node
        return $this->loadCurrNodeValues();
    }

    public function &loadByArtistId ($id) {
        GLOBAL $wpdb, $i18n_domain;

        $this->query = $wpdb->prepare("SELECT *
                        FROM " .self::$galleryTable. "
                        WHERE artist_id = %u", $id);
        $this->setTotalRows($wpdb->query($this->query));

        if ($this->getTotalRows() === FALSE) wp_die( sprintf(__('An error occurred while trying to perform a query: "%s"', $i18n_domain), $this->query) );

        $this->setCurrNode(0); // set to first node
        return $this->loadCurrNodeValues();
    }
    public function &loadAll ($order_by = 'id') {
        GLOBAL $wpdb, $i18n_domain;

        $this->query = "SELECT *
                        FROM " .self::$galleryTable. "
                        ORDER BY $order_by";
        $this->setTotalRows($wpdb->query($this->query));

        if ($this->getTotalRows() === FALSE) wp_die( sprintf(__('An error occurred while trying to perform a query: "%s"', $i18n_domain), $this->query) );

        $this->setCurrNode(0); // set to first node
        return $this->loadCurrNodeValues();
    }

    public function incrementPageViewsById ($id) {
        GLOBAL $wpdb, $i18n_domain;

        $this->query = $wpdb->prepare(
            "UPDATE " .self::$galleryTable. "
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

    public function deleteById ($id) {
        GLOBAL $wpdb, $i18n_domain;

        $this->query = $wpdb->prepare(
                       "DELETE FROM " .self::$galleryTable. "
                        WHERE id = %u", $id);
        $this->setTotalRows($wpdb->query($this->query));

        if ($this->getTotalRows() === FALSE) {
            wp_die( sprintf(__('An error occurred while trying to perform a query: "%s"', $i18n_domain), $this->query) );
        }
        return $this->getTotalRows();
    }

     // update artist entry
    public function updateById ($gallery_id, $artist_id, $name, $gallery, $picture_url, $gallery_descr) {
        GLOBAL $wpdb, $i18n_domain;
        GLOBAL $current_user;

        get_currentuserinfo();

        $this->query = $wpdb->prepare(
                 "UPDATE " .self::$galleryTable. "
                  SET update_date = now(),
		      artist_id = %u,
                      name = %s,
                      gallery = %s,
                      cover_picture = %s,
                      description = %s,
                      last_updated_by_id = %u
                  WHERE id = %s",
                  $artist_id, $name, $gallery, $picture_url, $gallery_descr, $current_user->ID, $gallery_id);
        $this->setTotalRows($wpdb->query($this->query));

        if ($this->getTotalRows() === FALSE) {
            wp_die( sprintf(__('An error occurred while trying to perform a query: "%s"', $i18n_domain), $this->query) );
        } else {
             // was entered successfully into database
            return true;
        }
    }

    public function insert ($artist_id, $name, $gallery, $picture_url, $gallery_descr) {
        GLOBAL $wpdb, $i18n_domain;
        GLOBAL $current_user;

        get_currentuserinfo();

        $this->query = $wpdb->prepare(
                       "INSERT INTO " .self::$galleryTable. "
                        (create_date, update_date, poster_id, last_updated_by_id, artist_id, name, gallery, cover_picture, description)
                        VALUES (now(), now(), %u, %u, %u, %s, %s, %s, %s)",
                        $current_user->ID, $current_user->ID, $artist_id, $name, $gallery, $picture_url, $gallery_descr);
        $this->setTotalRows($wpdb->query($this->query));

        if ($this->getTotalRows() === 0 OR $this->getTotalRows() === FALSE) {
            return sprintf(__('An error occurred while trying to perform a query: "%s"', $i18n_domain), $this->query); 
        } else {
             // was entered successfully into database
            $this->loadLastInsertId();
	    return $this->id;
        }
    }
}  /* end class Gallery */

?>
