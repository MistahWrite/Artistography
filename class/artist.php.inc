<?php
 // Shortcode helpers
add_filter( 'the_posts', 'Artist::conditionally_add_scripts_and_styles'); // the_posts gets triggered before wp_head

add_shortcode( 'artistography_artist_name', 'Artist::shortCodeArtistName' );
add_shortcode( 'artistography_display_artists', 'Artist::shortCodeDisplayArtists' );
add_shortcode( 'artistography_display_artist', 'Artist::shortCodeDisplayArtist' );

 // AJAX Administration
add_action('wp_ajax_Create_Artist', 'Artist::callback_Create_Artist');
add_action('wp_ajax_Update_Artist', 'Artist::callback_Update_Artist');
add_action('wp_ajax_Edit_Artist', 'Artist::callback_Edit_Artist');
add_action('wp_ajax_Delete_Artist', 'Artist::callback_Delete_Artist');

class Artist
{
    protected static $artistTable = NULL;  // TABLE NAME of ARTIST TABLE

     // property declaration
    public $id = NULL;
    public $page_views = NULL;

    public $name = NULL;
    public $url = NULL;
    public $picture_url = NULL;
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

    public static function callback_Create_Artist(){
        GLOBAL $i18n_domain;

        $artist = new Artist;

        $artist_id = stripslashes($_POST['artist_id']);
        $artist_name = stripslashes($_POST['artist_name']);
        $url = stripslashes($_POST['url']);
        $picture_url = stripslashes($_POST['picture_url']);
        $artist_descr = stripslashes($_POST['artist_descr']);

        $result = $artist->insert($artist_name, $url, $picture_url, $artist_descr);

        echo $result;

        unset($artist);
        die;
    }

    public static function callback_Update_Artist(){
        GLOBAL $i18n_domain;

        $artist = new Artist;

        $artist_id = stripslashes($_POST['artist_id']);
        $artist_name = stripslashes($_POST['artist_name']);
        $url = stripslashes($_POST['url']);
        $picture_url = stripslashes($_POST['picture_url']);
        $artist_descr = stripslashes($_POST['artist_descr']);

        if($artist->updateById ($artist_id, $artist_name, $url, $picture_url, $artist_descr) ) {
          echo '1'; //success
        } else {
          echo sprintf(__("Failed: Unable to Update Artist id: %u", $i18n_domain), $artist_id);
        }

        unset($artist);
        die;
    }

    public static function callback_Edit_Artist(){
        GLOBAL $i18n_domain;

        $artist = new Artist;

        if($artist->loadById($_POST['artist_id'])->getTotalRows() > 0) {

          echo $artist->id
               ."##$$##". stripslashes($artist->name)
              ."##$$##".  stripslashes($artist->picture_url)
              ."##$$##".  stripslashes($artist->url)
              ."##$$##".  stripslashes($artist->description) ."";
        }

        unset($artist);
        die;
    }

    public static function callback_Delete_Artist(){
        GLOBAL $i18n_domain;

        $artist_id = $_POST['artist_id'];

        $artist = new Artist;
        $artist->deleteById($artist_id);

        $discography = new Discography;
        $discography->deleteByArtistId($artist_id);

        if ($artist->getTotalRows() > 0) {
            echo "1"; // success
        } else {
            echo sprintf(__("Failed: Unable to Delete Artist id: %u", $i18n_domain), $artist_id);  // failed
        }

        unset($artist);
        die;
    }

    public static function conditionally_add_scripts_and_styles($posts){
        GLOBAL $artistography_plugin_dir;
	 // scripts are already being loaded on every page
	if (true == get_option('wp_artistography_ajax_loader')) return $posts;
        if (empty($posts)) return $posts;
 
        $shortcode_found = false; // use this flag to see if styles and scripts need to be enqueued
        foreach ($posts as $the_post) {
            if ( stripos($the_post->post_content, '[artistography_display_artists') !== FALSE ) {
                	$shortcode_found = true; // bingo!
                	break;
        	} else if ( stripos($the_post->post_content, '[artistography_display_artist') !== FALSE ) {
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

     // [artistography_artist_name id="1"]
    public static function shortCodeArtistName( $atts ) {
	extract( shortcode_atts( array(
		'id' => '1'
	), $atts ) );

        $artist = new Artist;
        $html = $artist->loadById(esc_attr($id))->name;
        unset($artist);
        return $html;
    }

     // [artistography_display_artists cols="4" size="75"]
    public static function shortCodeDisplayArtists( $atts ) {
	extract( shortcode_atts( array(
		'cols' => '3',
                'size' => '190'
	), $atts ) );

        $artist = new Artist;
        $discography = new Discography;
	$gallery = new Gallery;

        $artist->loadAll('name');
        $num = $artist->getTotalRows();
        $html = "";

        $cols = esc_attr($cols);
        $rows = ((int)($num / $cols) < (float)($num / $cols)) ? (int)($num / $cols) + 1 : (int)($num / $cols);
        $height = esc_attr($size) .'px'; $width = esc_attr($size) .'px';

        $html .= "<center><div class='artist_table' style='display;'>";
        if ($num == 0) {
            $html .= __('No Artists Available in the Database!', $i18n_domain);
        } else {
            for ( $i = 0; $i < $rows; $i++ ) {
                for ( $j = 0; $j < $cols && ((($cols * ($i)) + ($j+1)) <= $num); $j++ ) {
                    $artist->loadByNode($i * $cols + $j);
                    $artist_albums = $discography->loadMusicByArtistId($artist->id)->getTotalRows();
		    $galleries = $gallery->loadByArtistId($artist->id)->getTotalRows();
                    $video_posts = count($artist->getVideoPosts());
                    $related_posts = count($artist->getRelatedPosts());

                    $html .= "    <div style='float:left' height='" .esc_attr($size). "'>"
			  .  "	    <a href='" .$artist->url. "'>"
                          .  "        <div class='qitem' style='height:$height;width:$width'>"
                          .  "            <img src='" .$artist->picture_url. "' alt='" .$artist->name. "' title='' height='$height' width='$width' style='border:1px solid black;' />"
                          .  "            <span class='caption' height='100%' width='100%'><h2>" .$artist->name. "</h2><p style='font-size:1.0em;'>";
                    if ($artist_albums > 0) {
                        $html .= sprintf(_n("%d Album", "%d Albums", $artist_albums), $artist_albums) . "<br/>\n";
                    }
		    if ($galleries > 0) {
			$html .= sprintf(_n("%d Image Gallery", "%d Image Galleries", $galleries), $galleries) . "<br/>\n";
		    }
                    if ($video_posts > 0) {
                        $html .= sprintf(_n("%d Video", "%d Videos", $video_posts), $video_posts) . "<br/>\n";
                    }
                    if ($related_posts > 0) {
                        $html .= sprintf(_n("%d Related Post", "%d Related Posts", $related_posts), $related_posts) . "<br/>\n";
                    }
                    $html .= "            </p></span>"
                          .  "          </div>"
			  .  "        </a>"
                          .  "      </div>";
                }
                for($j; $j < $cols; $j++) {
                    $html .= "&nbsp;\n";
                }
                if ($i < ($rows-1)) { $html .= "<hr style='width:100%;'>\n"; }
            }
        }
        $html .= "</div></center>";
        unset($artist);
        unset($discography);
        return $html;
    }

     // [artistography_display_artist id="1"]
    public static function shortCodeDisplayArtist( $atts ) {
	extract( shortcode_atts( array(
		'id' => '0'
	), $atts ) );

        $artist = new Artist;

        $html = $artist->loadById(esc_attr($id))->name;

        unset($artist);
        return $html;
    }

    public function __construct() {
        GLOBAL $wpdb, $TABLE_NAME;

        self::$artistTable = $wpdb->prefix . $TABLE_NAME[TABLE_ARTISTS];

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
            $this->url = $this->query_result->url;
//            $this->url = "http://" .$_SERVER['SERVER_NAME'] . "/artists/" . strtolower(str_replace(' ', '-', str_replace('& ', '', str_replace('.', '', $this->name))));
            $this->picture_url = $this->query_result->picture_url;
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
            $this->url = 
            $this->picture_url = 
            $this->description = 0; 
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

    public function &loadById ($id) {
        GLOBAL $wpdb, $i18n_domain;

        $this->query = $wpdb->prepare("SELECT *
                        FROM " .self::$artistTable. "
                        WHERE id = %u", $id);
        $this->setTotalRows($wpdb->query($this->query));

        if ($this->getTotalRows() === FALSE) wp_die( sprintf(__('An error occurred while trying to perform a query: "%s"', $i18n_domain), $this->query) );

        $this->setCurrNode(0); // set to first node
        return $this->loadCurrNodeValues();
    }

    public function &loadAll ($order_by = 'id') {
        GLOBAL $wpdb, $i18n_domain;

        $this->query = "SELECT *
                        FROM " .self::$artistTable. "
                        ORDER BY $order_by";
        $this->setTotalRows($wpdb->query($this->query));

        if ($this->getTotalRows() === FALSE) wp_die( sprintf(__('An error occurred while trying to perform a query: "%s"', $i18n_domain), $this->query) );

        $this->setCurrNode(0); // set to first node
        return $this->loadCurrNodeValues();
    }

    public function incrementPageViewsById ($id) {
        GLOBAL $wpdb, $i18n_domain;

        $this->query = $wpdb->prepare(
            "UPDATE " .self::$artistTable. "
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

     /* Get posts in "Videos" category via tags */
    public function getVideoPosts ($order_by='date') {
	$video_term = get_term_by('name', 'Videos', 'category');
	if(is_object($video_term)) {
	        $cat_id = $video_term->term_id;
	        $args = array('tag' => strtolower(str_replace("&", "", str_replace(" ", "-", str_replace(".", "-", str_replace("& ", "", $this->name))))),
			'orderby' => $order_by,
			'numberposts' => -1);
       		$posts = get_posts($args);
       		foreach ($posts as $key => $value) {
       			if(!in_category($cat_id, $value)) {
               			unset($posts[$key]);
       			}
       		}
       		return $posts;
	}
	return NULL;
    }

     /* Get posts in all categories (except "Videos") via tags */
    public function getRelatedPosts ($order_by='date') {
	$related_term = get_term_by('name', 'Videos', 'category');
	if(is_object($related_term)) {
       		$cat_id = $related_term->term_id;
	        $args = array('tag' => strtolower(str_replace("&", "", str_replace(" ", "-", str_replace(".", "-", str_replace("& ", "", $this->name))))),
	        	'orderby' => $order_by,
	       		'numberposts' => -1);
	        $posts = get_posts($args);
	        foreach ($posts as $key => $value) {
	            if(in_category($cat_id, $value)) {
	                unset($posts[$key]);
	            }
	        }
		return $posts;
	}
        return NULL;
    }

    public function deleteById ($id) {
        GLOBAL $wpdb, $i18n_domain;

        $this->query = $wpdb->prepare(
                       "DELETE FROM " .self::$artistTable. "
                        WHERE id = %u", $id);
        $this->setTotalRows($wpdb->query($this->query));

        if ($this->getTotalRows() === FALSE) {
            wp_die( sprintf(__('An error occurred while trying to perform a query: "%s"', $i18n_domain), $this->query) );
        }
        return $this->getTotalRows();
    }

     // update artist entry
    public function updateById ($artist_id, $artist_name, $url, $picture_url, $artist_descr) {
        GLOBAL $wpdb, $i18n_domain;
        GLOBAL $current_user;

        get_currentuserinfo();

        $this->query = $wpdb->prepare(
                 "UPDATE " .self::$artistTable. "
                  SET update_date = now(),
                      name = %s,
                      url = %s,
                      picture_url = %s,
                      description = %s,
                      last_updated_by_id = %u
                  WHERE id = %s",
                  $artist_name, $url, $picture_url, $artist_descr, $current_user->ID, $artist_id);
        $this->setTotalRows($wpdb->query($this->query));

        if ($this->getTotalRows() === FALSE) {
            wp_die( sprintf(__('An error occurred while trying to perform a query: "%s"', $i18n_domain), $this->query) );
        } else {
             // was entered successfully into database
            return true;
        }
    }

    public function insert ($artist_name, $url, $picture_url, $artist_descr) {
        GLOBAL $wpdb, $i18n_domain;
        GLOBAL $current_user;

        get_currentuserinfo();

        $this->query = $wpdb->prepare(
                       "INSERT INTO " .self::$artistTable. "
                        (create_date, update_date, poster_id, last_updated_by_id, name, url, picture_url, description)
                        VALUES (now(), now(), %u, %u, %s, %s, %s, %s)",
                        $current_user->ID, $current_user->ID, $artist_name, $url, $picture_url, $artist_descr);
        $this->setTotalRows($wpdb->query($this->query));

        if ($this->getTotalRows() === 0 OR $this->getTotalRows() === FALSE) {
            return sprintf(__('An error occurred while trying to perform a query: "%s"', $i18n_domain), $this->query); 
        } else {
             // was entered successfully into database
            return $wpdb->insert_id;
        }
    }
}  /* end class Artist */

?>
