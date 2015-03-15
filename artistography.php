<?php
/*
 * Plugin Name: Artistography
 * Plugin URI: http://www.artistography.org/
 * Description: Build a collection of artist's media (videos, music, pictures) and organize them into a portfolio on your blog/website with PayPal functionality.
 * Version: 0.3.3-alpha
 * Author: MistahWrite
 * Author URI: http://www.LavaMonsters.com
 * Text Domain: artistography
 */
session_start();

if (!defined('WP_DEBUG')) define('WP_DEBUG', true); 
if (!defined('WP_DEBUG_LOG')) define('WP_DEBUG_LOG', true); 
if (!defined('WP_DEBUG_DISPLAY')) define('WP_DEBUG_DISPLAY', true);

define('LOG_FILE', "./ipn.log");

define('ARTISTOGRAPHY_VERSION', '0.3.3-alpha');

 // used to reference database tablenames in $TABLE_NAME, which is a globalized array
define('TABLE_ARTISTS', 0);
define('TABLE_SONGS', 1);
define('TABLE_SONG_ALBUM_LINKER', 2);
define('TABLE_ARTIST_ALBUM_LINKER', 3);
define('TABLE_ARTIST_FILE_DOWNLOAD', 4);
define('TABLE_ARTIST_MUSIC_ALBUMS', 5);
define('TABLE_ARTIST_IMAGE_GALLERIES', 6);
define('TABLE_ARTIST_ORDERS', 7);

define('WP_ROOT', str_replace('/wp-admin', '', dirname($_SERVER['SCRIPT_FILENAME'])));
define('SITEURL', get_option('siteurl'));

define('ARTISTOGRAPHY_FILE_PATH', '/artistography'); //dirname(__FILE__));
define('ARTISTOGRAPHY_DIR_NAME', basename(ARTISTOGRAPHY_FILE_PATH));

define('CURRENCY', '$');

GLOBAL $i18n_domain; $i18n_domain = 'artistography'; // Localization/Internationalization

GLOBAL $artistography_plugin_dir; $artistography_plugin_dir = plugins_url('artistography', 'artistography');
GLOBAL $artistography_plugin_lang_dir; $artistography_plugin_lang_dir = basename(dirname(__FILE__));

GLOBAL $download_folder; $download_folder = '/downloads/';
GLOBAL $download_path; $download_path = WP_ROOT . $download_folder;
GLOBAL $explorer_path; $explorer_path = dirname(dirname($_SERVER['SCRIPT_FILENAME'])) . "/wp-content/plugins/artistography/downloads/index.php";

GLOBAL $ajax_loader_url; $ajax_loader_url = $artistography_plugin_dir . '/css/images/300.GIF';

GLOBAL $download_icon_url; $download_icon_url = $artistography_plugin_dir . '/css/images/download.gif';
GLOBAL $download_icon_width; $download_icon_width = '150';
GLOBAL $download_icon_height; $download_icon_height = '127';

GLOBAL $add_to_cart_icon_url; $add_to_cart_icon_url = $artistography_plugin_dir . '/css/images/addtocartcc-orange.png';
GLOBAL $add_to_cart_icon_width; $add_to_cart_icon_width = '150';
GLOBAL $add_to_cart_icon_height; $add_to_cart_icon_height = '51';

GLOBAL $checkout_icon_url; $checkout_icon_url = $artistography_plugin_dir . '/css/images/yellow_checkout.png';
GLOBAL $checkout_icon_width; $checkout_icon_width = '150';
GLOBAL $checkout_icon_height; $checkout_icon_height = '50';

GLOBAL $buynow_icon_url; $buynow_icon_url = $artistography_plugin_dir . '/css/images/buynowcc-orange-2.png';
GLOBAL $buynow_icon_width; $buynow_icon_width = '213';
GLOBAL $buynow_icon_height; $buynow_icon_height = '95';

GLOBAL $transparent_pixel_url; $transparent_pixel_url = $artistography_plugin_dir . '/css/images/1x1.png';

GLOBAL $TABLE_NAME; $TABLE_NAME =
  array ('artistography_artists',
	 'artistography_songs',
	 'artistography_song_album_linker',
         'artistography_artist_album_linker',
         'artistography_file_download',
         'artistography_music_albums',
         'artistography_image_galleries',
         'artistography_orders');

require_once('class/item.php.inc');
require_once('class/artist.php.inc');
require_once('class/song.php.inc');
require_once('class/music.php.inc');
require_once('class/song-album-linker.php.inc');
require_once('class/discography.php.inc');
require_once('class/download.php.inc');
require_once('class/galleries.php.inc');
require_once('class/orders.php.inc');
require_once('admin/general_funcs.php.inc');

// widgets
require_once('class/cart_widget.php.inc');
require_once('class/products_widget.php.inc');

function folder_is_empty($folder) {
  $counter = 0;
  if ($handle = opendir($dir)) {
     // This is the correct way to loop over the directory.
    while (false !== ($file = readdir($handle))) {
      if(!strcmp($file, ".") || !strcmp($file, "..")) continue;
      $counter++;
      closedir($handle);
      return false;
    }
    closedir($handle);
  }
  return true;
}

 // ACTIVATION FOR PLUGIN
function artistography_pluginInstall() {
  GLOBAL $wpdb, $TABLE_NAME, $download_path, $explorer_path, $download_folder, $i18n_domain;
 
  $version = get_option('wp_artistography_version', false);

   /*** Store any options ***/
  add_option('wp_artistography_business_name', 'My Business Name');
  add_option('wp_artistography_donate_email', 'mistahwrite@gmail.com');
  add_option('wp_artistography_paypal_sandbox', true);
  add_option('wp_artistography_preserve_hidden_pages', true);
  add_option('wp_artistography_preserve_database', true);
  add_option('wp_artistography_email_notify_ftp', true);
  add_option('wp_artistography_ftp_host', '');
  add_option('wp_artistography_ftp_user', '');
  add_option('wp_artistography_ftp_pass', '');
  add_option('wp_artistography_ftp_path', '');
  add_option('wp_artistography_version', ARTISTOGRAPHY_VERSION, NULL, true);
  add_option('wp_artistography_gallery_style', 'Colorbox');
  add_option('wp_artistography_ajax_loader', false);

   /* Create Download Page */
  if (!get_option('wp_artistography_download_page')) {
     // Create post object
    $my_post = array(
       'post_title' => 'Download',
       'post_name' => 'artistography_download',
       'post_content' => '[artistography_download]',
       'comment_status' => 'closed',
       'ping_status' => 'closed',
       'post_status' => 'publish',
       'post_type' => 'page'
    );

     // Insert the post into the database
    $my_post_id = wp_insert_post( $my_post );
    if($my_post_id) {
      add_option('wp_artistography_download_page', $my_post_id);
    }
  }
   /* Create Cart Page */
  if (!get_option('wp_artistography_cart_page')) {
     // Create post object
    $my_post = array(
       'post_title' => 'Cart',
       'post_name' => 'artistography_cart',
       'post_content' => '[artistography_show_cart]',
       'comment_status' => 'closed',
       'ping_status' => 'closed',
       'post_status' => 'publish',
       'post_type' => 'page'
    );

     // Insert the post into the database
    $my_post_id = wp_insert_post( $my_post );
    if($my_post_id) {
      add_option('wp_artistography_cart_page', $my_post_id);
    }
  }
   /* Create Checkout Page */
  if (!get_option('wp_artistography_checkout_page')) {
     // Create post object
    $my_post = array(
       'post_title' => 'Checkout',
       'post_name' => 'artistography_checkout',
       'post_content' => '[artistography_show_checkout]',
       'comment_status' => 'closed',
       'ping_status' => 'closed',
       'post_status' => 'publish',
       'post_type' => 'page'
    );

     // Insert the post into the database
    $my_post_id = wp_insert_post( $my_post );
    if($my_post_id) {
      add_option('wp_artistography_checkout_page', $my_post_id);
    }
  }

   /* Create Thank You Page */
  if (!get_option('wp_artistography_thankyou_page')) {
     // Create post object
    $my_post = array(
       'post_title' => 'Thank You',
       'post_name' => 'artistography_thankyou',
       'post_content' => '[artistography_show_thankyou]',
       'comment_status' => 'closed',
       'ping_status' => 'closed',
       'post_status' => 'publish',
       'post_type' => 'page'
    );

     // Insert the post into the database
    $my_post_id = wp_insert_post( $my_post );
    if($my_post_id) {
      add_option('wp_artistography_thankyou_page', $my_post_id);
    }
  }

   /* Create Orders Page */
  if (!get_option('wp_artistography_orders_page')) {
     // Create post object
    $my_post = array(
       'post_title' => 'Orders',
       'post_name' => 'artistography_orders',
       'post_content' => '[artistography_show_orders]',
       'comment_status' => 'closed',
       'ping_status' => 'closed',
       'post_status' => 'publish',
       'post_type' => 'page'
    );

     // Insert the post into the database
    $my_post_id = wp_insert_post( $my_post );
    if($my_post_id) {
      add_option('wp_artistography_orders_page', $my_post_id);
    }
  }

   /* Create IPN Page - PayPal Instant Notifications */
  if (!get_option('wp_artistography_ipn_page')) {
     // Create post object
    $my_post = array(
       'post_title' => 'IPN',
       'post_name' => 'artistography_ipn',
       'post_content' => '[artistography_ipn]',
       'comment_status' => 'closed',
       'ping_status' => 'closed',
       'post_status' => 'publish',
       'post_type' => 'page'
    );

     // Insert the post into the database
    $my_post_id = wp_insert_post( $my_post );
    if($my_post_id) {
      add_option('wp_artistography_ipn_page', $my_post_id);
    }
  }
 
  if (!is_dir($download_path)) {
    mkdir($download_path);
  }
  exec("cp $explorer_path $download_path/");

  if (version_compare($version, "0.1.1", '<')) {
     // this is an update to at least 0.1.1
    $thetable = $wpdb->prefix . $TABLE_NAME[TABLE_ARTISTS];
    $query = "ALTER TABLE $thetable DROP COLUMN myspace_url";
    $wpdb->query($query);

    $query = "ALTER TABLE $thetable DROP COLUMN birthday";
    $wpdb->query($query); 

    $query = "ALTER TABLE $thetable DROP COLUMN enabled";
    $wpdb->query($query);      

    $thetable = $wpdb->prefix . "artistography_track_list";
    $query = "DROP TABLE $thetable";
    $wpdb->query($query);
  }

  if (version_compare($version, "0.1.8", '<')) {
    $cart_page = get_option('wp_artistography_products_cart_page');
    if($cart_page) {
      $result = wp_delete_post($cart_page, true);
      if($result) {
        delete_option('wp_aristography_products_cart_page');
      }
    }

    $checkout_page = get_option('wp_artistography_products_checkout_page');
    if($checkout_page) {
      $result = wp_delete_post($checkout_page, true);
      if($result) {
        delete_option('wp_aristography_products_checkout_page');
      }
    }
  }

  if (version_compare($version, "0.1.9", '<')) {
    $thetable = $wpdb->prefix . $TABLE_NAME[TABLE_ARTIST_MUSIC_ALBUMS];
    $query = "ALTER TABLE $thetable ADD COLUMN price DECIMAL (7,2) DEFAULT '0.00' NOT NULL";
    $wpdb->query($query);

    $query = "ALTER TABLE $thetable DELETE COLUMN free_download_enabled";
    $wpdb->query($query);

    $query = "ALTER TABLE $thetable DELETE COLUMN featured";
    $wpdb->query($query);

    $query = "ALTER TABLE $thetable DELETE COLUMN enabled";
    $wpdb->query($query);

    $query = "DROP TABLE wp_artistography_sales";
    $wpdb->query($query);

    $my_post = array(
      'ID'           => get_option('wp_artistography_download_page'),
      'comment_status' => 'closed',
      'ping_status' => 'closed'
    );
    wp_update_post( $my_post );

    $my_post = array(
      'ID'           => get_option('wp_artistography_cart_page'),
      'comment_status' => 'closed',
      'ping_status' => 'closed'
    );
    wp_update_post( $my_post );

    $my_post = array(
      'ID'           => get_option('wp_artistography_checkout_page'),
      'comment_status' => 'closed',
      'ping_status' => 'closed'
    );
    wp_update_post( $my_post );
  }

  if (version_compare($version, "0.2.1", '<')) {
    $thetable = $wpdb->prefix . $TABLE_NAME[TABLE_ARTIST_ORDERS];
    $query = "ALTER TABLE $thetable ADD COLUMN user_id INT(10) UNSIGNED DEFAULT '0' NOT NULL";
    $wpdb->query($query);
  }

  if (version_compare($version, "0.2.1-alpha4", '<')) {
    $thetable = $wpdb->prefix . 'artistography_image_gallery';
    $query = "DROP TABLE $thetable";
    $wpdb->query($query);

    $thetable = $wpdb->prefix . $TABLE_NAME[TABLE_ARTIST_IMAGE_GALLERIES];
    $query = "DROP TABLE $thetable";
    $wpdb->query($query);
  }

  if (version_compare($version, "0.2.8-alpha2", '<')) {
    	$thetable = $wpdb->prefix . $TABLE_NAME[TABLE_ARTIST_ORDERS];
        $query = "DROP TABLE $thetable";
        $wpdb->query($query);
  }
  if (version_compare($version, "0.2.8-alpha6", '<')) {
	delete_option('wp_artistography_business_name');
  }
  if (version_compare($version, "0.2.8-alpha10", '<')) {
	$thetable = $wpdb->prefix . $TABLE_NAME[TABLE_ARTIST_ORDERS];
	$query = "DROP TABLE $thetable";
	$wpdb->query($query);

	$thetable = $wpdb->prefix . $TABLE_NAME[TABLE_ARTIST_MUSIC_ALBUMS];
	$query = "ALTER TABLE $thetable DROP COLUMN artist_id";
	$wpdb->query($query);
  }
  if (version_compare($version, "0.3.0-alpha4", '<')) {
        $thetable = $wpdb->prefix . $TABLE_NAME[TABLE_SONGS];
        $query = "DROP TABLE $thetable";
        $wpdb->query($query);
  }
  update_option('wp_artistography_version', ARTISTOGRAPHY_VERSION);

    // Create Data Tables If They Don't Already Exist
  foreach ($TABLE_NAME as $key => $value) {
    $thetable = $wpdb->prefix . $value;

     // if tables don't already exist create them
    if($wpdb->get_var("show tables like '$thetable'") != $thetable) {
      $query = "CREATE TABLE $thetable (
                     id INT(10) UNSIGNED NOT NULL AUTO_INCREMENT,
                     create_date DATETIME NOT NULL,
                     update_date DATETIME NOT NULL,
                     poster_id INT(10) UNSIGNED DEFAULT '0' NOT NULL,
                     last_updated_by_id INT(10) UNSIGNED DEFAULT '0' NOT NULL,
                     ";
      switch ($key) {
        case TABLE_ARTISTS:
          $query .= "page_views INT(10) UNSIGNED DEFAULT '0' NOT NULL,
                     name TEXT NOT NULL,
                     picture_url TEXT NOT NULL,
                     url TEXT,
                     description LONGTEXT,
                     enabled BOOLEAN DEFAULT false NOT NULL,";
          break;

	case TABLE_SONGS:
          $query .= "number INT(5) UNSIGNED,
		     artist_id INT(10) UNSIGNED DEFAULT '0' NOT NULL,
		     name TEXT NOT NULL,
                     url TEXT,
		     price DECIMAL (7,2) DEFAULT '0.00' NOT NULL,
		     explicit BOOLEAN DEFAULT FALSE NOT NULL,";
          break;

	case TABLE_SONG_ALBUM_LINKER:
          $query .= "song_id INT(10) UNSIGNED DEFAULT '0' NOT NULL,
                     album_id INT(10) UNSIGNED DEFAULT '0' NOT NULL,";
	  break;

        case TABLE_ARTIST_ALBUM_LINKER:
          $query .= "artist_id INT(10) UNSIGNED DEFAULT '0' NOT NULL,
                     album_id INT(10) UNSIGNED DEFAULT '0' NOT NULL,";
          break;

        case TABLE_ARTIST_FILE_DOWNLOAD:
          $query .= "file_name VARCHAR(250),
                     download_count INT(10) UNSIGNED DEFAULT '0' NOT NULL,
                     enabled BOOLEAN DEFAULT FALSE NOT NULL,";
          break;

        case TABLE_ARTIST_MUSIC_ALBUMS:
          $query .= "page_views INT(10) UNSIGNED DEFAULT '0' NOT NULL,
                     artist_name TEXT NOT NULL,
                     album_name TEXT NOT NULL,
                     album_date DATETIME NOT NULL,
                     artist_url TEXT,
                     picture_url TEXT NOT NULL,
                     store_url TEXT NOT NULL,
                     download_id INT(10) UNSIGNED DEFAULT '0' NOT NULL,
                     price DECIMAL (7,2) DEFAULT '0.00' NOT NULL,
                     description LONGTEXT,";
          break;

        case TABLE_ARTIST_IMAGE_GALLERIES:
          $query .= "page_views INT(10) UNSIGNED DEFAULT '0' NOT NULL,
                     artist_id INT(10),
		     name TEXT NOT NULL,
                     gallery TEXT NOT NULL,
		     cover_picture TEXT NOT NULL,
                     description LONGTEXT,";
          break;

	case TABLE_ARTIST_ORDERS:
	$query .= "  payer_id TEXT NOT NULL,
		     address_status TEXT,
		     payer_status TEXT,
		     forename TEXT NOT NULL,
		     surname TEXT NOT NULL,
		     email TEXT NOT NULL,
		     address_line_1 TEXT NOT NULL,
                     postcode TEXT NOT NULL,
		     town TEXT NOT NULL,
		     state TEXT NOT NULL,
		     country_code TEXT NOT NULL,
		     contact_phone TEXT,
		     itemsOrdered TEXT,
		     itemsNumberOrdered TEXT,
		     quantityOrdered TEXT,
		     txn_id TEXT NOT NULL,
		     txn_type TEXT,
		     parent_txn_id TEXT,
		     verify_sign TEXT,
		     payment_status TEXT NOT NULL,
		     payment_amount TEXT,
		     payment_currency TEXT,
		     item_name TEXT,
		     item_number TEXT,
		     quantity TEXT,
		     shipping TEXT,
		     tax TEXT,
		     mc_fee TEXT,
		     mc_gross TEXT,
		     mc_handling TEXT,
		     mc_shipping TEXT,
		     memo TEXT,
		     pending_reason TEXT,
		     refund_reason_code TEXT,
		     refund_receipt_id TEXT,
		     user_id INT(10) UNSIGNED DEFAULT '0' NOT NULL,";
	  break;

      } // end switch($key)
      $query .= "\n  UNIQUE KEY id (id));";
      $wpdb->query($query);
    }
  } // end foreach
}
register_activation_hook( __FILE__, 'artistography_pluginInstall' );

 // DEACTIVATION FOR PLUGIN
function artistography_pluginUninstall() {
  GLOBAL $wpdb, $TABLE_NAME, $download_path, $i18n_domain;

  if (!strcmp(get_option('wp_artistography_preserve_hidden_pages'), NULL)) {
     /* Delete Download Page - So it isn't visible while Artistography is disabled */
    $download_page_id = get_option('wp_artistography_download_page');
    if ($download_page_id) {
       // force delete download page
      $result = wp_delete_post( $download_page_id, true ); 
      if($result) {
        delete_option('wp_artistography_download_page');
      }
    }

     /* Delete Cart Page - So it isn't visible while Artistography is disabled */
    $cart_page_id = get_option('wp_artistography_cart_page');
    if ($cart_page_id) {
       // force delete cart page
      $result = wp_delete_post( $cart_page_id, true );
      if($result) {
        delete_option('wp_artistography_cart_page');
      }
    }

     /* Delete Checkout Page - So it isn't visible while Artistography is disabled */
    $checkout_page_id = get_option('wp_artistography_checkout_page');
    if ($checkout_page_id) {
       // force delete checkout page
      $result = wp_delete_post( $checkout_page_id, true );
      if($result) {
        delete_option('wp_artistography_checkout_page');
      }
    }

     /* Delete Orders Page - So it isn't visible while Artistography is disabled */
    $orders_page_id = get_option('wp_artistography_orders_page');
    if ($orders_page_id) {
       // force delete checkout page
      $result = wp_delete_post( $orders_page_id, true );
      if($result) {
        delete_option('wp_artistography_orders_page');
      }
    }

     /* Delete Thank You Page - So it isn't visible while Artistography is disabled */
    $thankyou_page_id = get_option('wp_artistography_thankyou_page');
    if ($thankyou_page_id) {
       // force delete checkout page
      $result = wp_delete_post( $thankyou_page_id, true );
      if($result) {
        delete_option('wp_artistography_thankyou_page');
      }
    }

     /* Delete IPN Page - So it isn't visible while Artistography is disabled */
    $ipn_page_id = get_option('wp_artistography_ipn_page');
    if ($ipn_page_id) {
       // force delete checkout page
      $result = wp_delete_post( $ipn_page_id, true );
      if($result) {
        delete_option('wp_artistography_ipn_page');
      }
    }
  }

  if (!strcmp(get_option('wp_artistography_preserve_database'), NULL)) {
     //Delete any options that's stored
    delete_option('wp_artistography_donate_email');
    delete_option('wp_artistography_paypal_sandbox');
    delete_option('wp_artistography_preserve_hidden_pages');
    delete_option('wp_artistography_preserve_database');
    delete_option('wp_artistography_email_notify_ftp');
    delete_option('wp_artistography_ftp_host');
    delete_option('wp_artistography_ftp_user');
    delete_option('wp_artistography_ftp_pass');
    delete_option('wp_artistography_ftp_path');
    delete_option('wp_artistography_gallery_style');
    delete_option('wp_artistography_ajax_loader');
    delete_option('wp_artistography_version');

    foreach ($TABLE_NAME as $key => $value) {
      $thetable = $wpdb->prefix . $value;
      $query = "DROP TABLE IF EXISTS $thetable";
      $wpdb->query($query);
    }
  }
}
register_deactivation_hook( __FILE__, 'artistography_pluginUninstall' );

function artistography_is_current_version() {
  $version = get_option('wp_artistography_version');
  return version_compare($version, ARTISTOGRAPHY_VERSION, '=') ? true : false;
}

function artistography_exclude_pages ($pages) {

	$excluded_ids =  array( get_option('wp_artistography_download_page'),
				get_option('wp_artistography_cart_page'),
				get_option('wp_artistography_checkout_page'),
				get_option('wp_artistography_thankyou_page'),
				get_option('wp_artistography_orders_page'),
				get_option('wp_artistography_ipn_page') );
	$shaved_pages = array();
	foreach($pages as $page) {
		if ( ! in_array($page->ID, $excluded_ids) ) {
			$shaved_pages[] = $page;
		}
	}
	return $shaved_pages;
}

function my_soundmanager2_bar_hook() {
	$songs = new Song;
	$artist = new Artist;

	if ($songs->loadByPrice('0.00')->getTotalRows() > 0) {
		echo '
<!-- fixed, bottom-aligned, full-width player -->

<div class="sm2-bar-ui full-width fixed dark-text" style="height:75%:bottom:0px;">

 <div class="bd sm2-main-controls">

  <div class="sm2-inline-texture"></div>
  <div class="sm2-inline-gradient"></div>

  <div class="sm2-inline-element sm2-button-element">
   <div class="sm2-button-bd">
    <a href="#prev" title="Previous" class="sm2-inline-button sm2-previous">&lt; previous</a>
   </div>
  </div>

  <div class="sm2-inline-element sm2-button-element">
   <div class="sm2-button-bd">
    <a href="#play" class="sm2-inline-button play-pause">Play / pause</a>
   </div>
  </div>

  <div class="sm2-inline-element sm2-button-element">
   <div class="sm2-button-bd">
    <a href="#next" title="Next" class="sm2-inline-button sm2-next">&gt; next</a>
   </div>
  </div>

  <div class="sm2-inline-element sm2-inline-status">

   <div class="sm2-playlist">
    <div class="sm2-playlist-target">
     <!-- playlist <ul> + <li> markup will be injected here -->
     <!-- if you want default / non-JS content, you can put that here. -->
     <noscript><p>JavaScript is required.</p></noscript>
    </div>
   </div>

   <div class="sm2-progress">
    <div class="sm2-row">
    <div class="sm2-inline-time">0:00</div>
     <div class="sm2-progress-bd">
      <div class="sm2-progress-track">
       <div class="sm2-progress-bar"></div>
       <div class="sm2-progress-ball"><div class="icon-overlay"></div></div>
      </div>
     </div>
     <div class="sm2-inline-duration">0:00</div>
    </div>
   </div>

  </div>

  <div class="sm2-inline-element sm2-button-element sm2-volume">
   <div class="sm2-button-bd">
    <span class="sm2-inline-button sm2-volume-control volume-shade"></span>
    <a href="#volume" class="sm2-inline-button sm2-volume-control">volume</a>
   </div>
  </div>

  <!-- unimplemented -->
  <!--
  <div class="sm2-inline-element sm2-button-element disabled">
   <div class="sm2-button-bd">
    <a href="#repeat" title="Repeat playlist" class="sm2-inline-button repeat">&infin; repeat</a>
   </div>
  </div>
  -->

  <div class="sm2-inline-element sm2-button-element sm2-menu">
   <div class="sm2-button-bd">
    <a href="#menu" class="sm2-inline-button sm2-menu-button">menu</a>
   </div>
  </div>

 </div>

 <div class="bd sm2-playlist-drawer sm2-element">

  <div class="sm2-inline-texture">
   <div class="sm2-box-shadow"></div>
  </div>

  <!-- playlist content is mirrored here -->
  <div class="sm2-album-art-wrapper" style="position:fixed;left:0;top:0;height:100%"><img id="sm2-album-art" /></div>
  <div class="sm2-playlist-wrapper" style="width:50%:position:fixed;left:50%;top:0">
    <ul class="sm2-playlist-bd">';
		for($i = 0; $i < $songs->getTotalRows(); $i++) {
			$song_list[$i] = $songs->id;
			$songs->getNodeNext();
		}
		shuffle($song_list);
		for($i = 0; $i < count($song_list); $i++) {
			$songs->loadById($song_list[$i]);
			echo "<li id='playlist_song_$songs->id'><a href='$songs->url'><b>" .$artist->loadById($songs->artist_id)->name. "</b> - $songs->name" .(($songs->explicit) ? "<span class='label'>Explicit</span>" : ''). "</a></li>";
		}
		echo '</ul>
  </div>

 </div>
</div>';
	}
}

function artistography_init() {
  GLOBAL $i18n_domain, $artistography_plugin_dir;

   /* Hide Pages from Menu */
  add_filter('get_pages', 'artistography_exclude_pages');
  if(!is_admin()) {
  	add_action('wp_print_footer_scripts', 'my_soundmanager2_bar_hook'); //, PHP_INT_MAX);
  }

  if(!artistography_is_current_version()) artistography_pluginInstall();

  if(true == get_option('wp_artistography_ajax_loader')) {
     // we want to load the scripts/styles on all pages because we may need it later
    add_action( 'wp_enqueue_scripts', 'artistography_enqueue_style_and_scripts', 0);
  }

  add_thickbox();

  load_plugin_textdomain( $i18n_domain, false, $artistography_plugin_dir );
}
add_action('init', 'artistography_init');

  define('ADMIN_MENU_ORDERS', __('Orders', $i18n_domain));
  define('ADMIN_MENU_MANAGE_ARTISTS', __('Manage Artists', $i18n_domain));
  define('ADMIN_MENU_MANAGE_SONGS', __('Manage Songs', $i18n_domain));
  define('ADMIN_MENU_MANAGE_ALBUMS', __('Manage Albums', $i18n_domain));
  define('ADMIN_MENU_MANAGE_DOWNLOADS', __('Downloads', $i18n_domain));
  define('ADMIN_MENU_MANAGE_ARTIST_ALBUM_LINKER', __('Artist/Album Linker', $i18n_domain));
  define('ADMIN_MENU_MANAGE_SONG_ALBUM_LINKER', __('Song/Album Linker', $i18n_domain));
  define('ADMIN_MENU_MANAGE_GALLERIES', __('Galleries', $i18n_domain));
  define('ADMIN_MENU_FTP_UPLOADER', __('FTP Uploader', $i18n_domain));
  define('ADMIN_MENU_OPTIONS', __('Options', $i18n_domain));
  define('ADMIN_MENU_ABOUT', __('About', $i18n_domain));

  define('TOP_LEVEL_HANDLE', 'artistography-top-level');
  define('SUBMENU_MANAGE_ORDERS_HANDLE', 'artistography-submenu-manage-orders');
  define('SUBMENU_MANAGE_ARTISTS_HANDLE', 'artistography-submenu-manage-artists');
  define('SUBMENU_MANAGE_SONGS_HANDLE', 'artistography-submenu-manage-songs');
  define('SUBMENU_MANAGE_ALBUMS_HANDLE', 'artistography-submenu-manage-albums');
  define('SUBMENU_MANAGE_DOWNLOADS_HANDLE', 'artistography-submenu-manage-downloads');
  define('SUBMENU_MANAGE_ARTIST_ALBUM_LINKER_HANDLE', 'artistography-submenu-manage-artist-album-linker');
  define('SUBMENU_MANAGE_SONG_ALBUM_LINKER_HANDLE', 'artistography-submenu-manage-song-album-linker');
  define('SUBMENU_MANAGE_GALLERIES_HANDLE', 'artistography-submenu-manage-galleries');
  define('SUBMENU_FTP_UPLOADER', 'artistography-submenu-ftp-uploader');
  define('SUBMENU_OPTIONS_HANDLE', 'artistography-submenu-options');
  define('SUBMENU_ABOUT_HANDLE', 'artistography-submenu-about');

function artistography_enqueue_admin_style_and_scripts() {
    GLOBAL $artistography_plugin_dir;

    wp_enqueue_media();
    wp_enqueue_style('thickbox');
    wp_enqueue_script('media-upload');
    wp_enqueue_script('thickbox');

    wp_enqueue_script('jquery');

    wp_enqueue_style( 'jquery-ui', $artistography_plugin_dir . '/js/jquery-ui-1.11.2/jquery-ui.css', array(), '1.11.2', 'all');
    wp_enqueue_style( 'jquery-ui', $artistography_plugin_dir . '/js/jquery-ui-1.11.2/jquery-ui.theme.css', array(), '1.11.2', 'all');
    wp_enqueue_script( 'jquery-ui',  $artistography_plugin_dir . '/js/jquery-ui-1.11.2/jquery-ui.js', array( 'jquery' ), '1.0.0');

    wp_enqueue_style( 'artistography', $artistography_plugin_dir . '/css/admin-style.css', array(), ARTISTOGRAPHY_VERSION, 'all');

    switch($_GET['page']) {
	case SUBMENU_MANAGE_ARTISTS_HANDLE:
		$admin_script = 'admin-artist.js';
		break;
	case SUBMENU_MANAGE_SONGS_HANDLE:
                $admin_script = 'admin-song.js';
                break;
	case SUBMENU_MANAGE_ALBUMS_HANDLE:
		$admin_script = 'admin-music.js';
		break;
	case SUBMENU_MANAGE_ARTIST_ALBUM_LINKER_HANDLE:
		$admin_script = 'admin-discography.js';
		break;
	case SUBMENU_MANAGE_SONG_ALBUM_LINKER_HANDLE:
		$admin_script = 'admin-song-album-linker.js';
		break;
	case SUBMENU_MANAGE_GALLERIES_HANDLE:
		wp_enqueue_media();
		$admin_script = 'admin-gallery.js';
		break;
	case TOP_LEVEL_HANDLE:
	case SUBMENU_MANAGE_ORDERS_HANDLE:
		$admin_script = 'admin-orders.js';
		break;
	default:
    		$admin_script = 'admin.js';
    }
    wp_enqueue_script( 'artistography',  $artistography_plugin_dir . "/js/$admin_script", array( 'jquery-ui' ), ARTISTOGRAPHY_VERSION);
}

function artistography_enqueue_style_and_scripts() {
    GLOBAL $artistography_plugin_dir;

	if(strcmp("Lightbox", get_option('wp_artistography_gallery_style')) == 0) {
    		wp_enqueue_style('thickbox');
    		wp_enqueue_script('thickbox');
	} else if (strcmp("Colorbox", get_option('wp_artistography_gallery_style')) == 0) {
		wp_enqueue_style( 'colorbox', $artistography_plugin_dir . '/js/colorbox-master/example5/colorbox.css', array(), '1.0.0', 'all');
		wp_enqueue_script( 'colorbox',  $artistography_plugin_dir . '/js/colorbox-master/jquery.colorbox.js', array( 'jquery' ), '1.0.0');
	}

	wp_enqueue_script('jquery');

	wp_enqueue_style( 'jquery-ui', $artistography_plugin_dir . '/js/jquery-ui-1.11.2/jquery-ui.css', array(), '1.11.2', 'all');
	wp_enqueue_style( 'jquery-ui', $artistography_plugin_dir . '/js/jquery-ui-1.11.2/jquery-ui.theme.css', array(), '1.11.2', 'all');
	wp_enqueue_script( 'jquery-ui',  $artistography_plugin_dir . '/js/jquery-ui-1.11.2/jquery-ui.js', array( 'jquery' ), '1.0.0');

	wp_enqueue_style( '', $artistography_plugin_dir . '/soundmanagerv297a-20140901/demo/artistography-bar-bui/css/bar-ui.css', array(), '2.9.7a', 'all');
	wp_enqueue_script( '', $artistography_plugin_dir . '/soundmanagerv297a-20140901/script/soundmanager2.js', array(), '2.9.7a');
	wp_enqueue_script( '', $artistography_plugin_dir . '/soundmanagerv297a-20140901/demo/artistography-bar-bui/script/bar-ui.js', array( 'soundmanager2' ), '2.9.7a');

	wp_enqueue_style( 'artistography', $artistography_plugin_dir . '/css/style.css', array(), ARTISTOGRAPHY_VERSION, 'all');
	wp_enqueue_script( 'artistography',  $artistography_plugin_dir . '/js/script.js', array( 'jquery-ui' ), ARTISTOGRAPHY_VERSION);
	 // Include the Ajax library on the front end
        wp_localize_script( 'artistography', 'MyAjax', array( 'ajaxurl' => admin_url( 'admin-ajax.php')));
}


function artistography_enqueue_sm_style_and_scripts() {
	GLOBAL $artistography_plugin_dir;

        wp_enqueue_style( 'soundmanager2-bar-ui', $artistography_plugin_dir . '/soundmanagerv297a-20140901/demo/artistography-bar-ui/css/bar-ui.css', array(), '297a-20140901', 'all');
        wp_enqueue_script( 'soundmanager2', $artistography_plugin_dir . '/soundmanagerv297a-20140901/script/soundmanager2.js', array(), '297a-20140901');
        wp_enqueue_script( 'soundmanager2-bar-ui', $artistography_plugin_dir . '/soundmanagerv297a-20140901/demo/artistography-bar-ui/script/bar-ui.js', array( 'soundmanager2' ), '297a-20140901');
	wp_enqueue_style( 'soundmanager2', $artistography_plugin_dir . '/css/soundmanager2.css', array(), ARTISTOGRAPHY_VERSION, 'all');
}
add_action( 'wp_enqueue_scripts', 'artistography_enqueue_sm_style_and_scripts');

 /* START ADMIN MENU INTERFACE */
add_action('admin_menu', 'artistography_plugin_menu');
function artistography_plugin_menu() {

  GLOBAL $artistography_plugin_dir, $i18n_domain;

  if( array_key_exists('page', $_GET) ) {
  	if( FALSE !== stripos($_GET['page'], 'artistography') ) {
		artistography_enqueue_admin_style_and_scripts();
	}
  }

  add_menu_page(TOP_LEVEL_HANDLE, __('Artistography', $i18n_domain), 'manage_options', TOP_LEVEL_HANDLE, 'artistography_plugin_options');
  add_submenu_page(TOP_LEVEL_HANDLE, sprintf(__('Artistography %s', $i18n_domain), ADMIN_MENU_ORDERS), ADMIN_MENU_ORDERS, 'manage_options', TOP_LEVEL_HANDLE, 'artistography_plugin_options');
  add_submenu_page(TOP_LEVEL_HANDLE, sprintf(__('Artistography %s', $i18n_domain), ADMIN_MENU_MANAGE_ARTISTS), ADMIN_MENU_MANAGE_ARTISTS, 'manage_options', SUBMENU_MANAGE_ARTISTS_HANDLE, 'artistography_plugin_options');
  add_submenu_page(TOP_LEVEL_HANDLE, sprintf(__('Artistography %s', $i18n_domain), ADMIN_MENU_MANAGE_SONGS), ADMIN_MENU_MANAGE_SONGS, 'manage_options', SUBMENU_MANAGE_SONGS_HANDLE, 'artistography_plugin_options');
  add_submenu_page(TOP_LEVEL_HANDLE, sprintf(__('Artistography %s', $i18n_domain), ADMIN_MENU_MANAGE_ALBUMS), ADMIN_MENU_MANAGE_ALBUMS, 'manage_options', SUBMENU_MANAGE_ALBUMS_HANDLE, 'artistography_plugin_options');
  add_submenu_page(TOP_LEVEL_HANDLE, sprintf(__('Artistography %s', $i18n_domain), ADMIN_MENU_MANAGE_ARTIST_ALBUM_LINKER), ADMIN_MENU_MANAGE_ARTIST_ALBUM_LINKER, 'manage_options', SUBMENU_MANAGE_ARTIST_ALBUM_LINKER_HANDLE, 'artistography_plugin_options');
  add_submenu_page(TOP_LEVEL_HANDLE, sprintf(__('Artistography %s', $i18n_domain), ADMIN_MENU_MANAGE_SONG_ALBUM_LINKER), ADMIN_MENU_MANAGE_SONG_ALBUM_LINKER, 'manage_options', SUBMENU_MANAGE_SONG_ALBUM_LINKER_HANDLE, 'artistography_plugin_options');
  add_submenu_page(TOP_LEVEL_HANDLE, sprintf(__('Artistography %s', $i18n_domain), ADMIN_MENU_MANAGE_DOWNLOADS), ADMIN_MENU_MANAGE_DOWNLOADS, 'manage_options', SUBMENU_MANAGE_DOWNLOADS_HANDLE, 'artistography_plugin_options');
  add_submenu_page(TOP_LEVEL_HANDLE, sprintf(__('Artistography %s', $i18n_domain), ADMIN_MENU_MANAGE_GALLERIES), ADMIN_MENU_MANAGE_GALLERIES, 'manage_options', SUBMENU_MANAGE_GALLERIES_HANDLE, 'artistography_plugin_options');
  add_submenu_page(TOP_LEVEL_HANDLE, sprintf(__('Artistography %s', $i18n_domain), ADMIN_MENU_FTP_UPLOADER), ADMIN_MENU_FTP_UPLOADER, 'manage_options', SUBMENU_FTP_UPLOADER, 'artistography_plugin_options');
  add_submenu_page(TOP_LEVEL_HANDLE, sprintf(__('Artistography %s', $i18n_domain), ADMIN_MENU_OPTIONS), ADMIN_MENU_OPTIONS, 'manage_options', SUBMENU_OPTIONS_HANDLE, 'artistography_plugin_options');
  add_submenu_page(TOP_LEVEL_HANDLE, sprintf(__('Artistography %s', $i18n_domain), ADMIN_MENU_ABOUT), ADMIN_MENU_ABOUT, 'manage_options', SUBMENU_ABOUT_HANDLE, 'artistography_plugin_options');

}

function artistography_plugin_options() {
  GLOBAL $wpdb, $i18n_domain, $title, $TABLE_NAME, $download_folder, $download_path, $explorer_path, $artistography_plugin_dir;

  if (!current_user_can('manage_options'))  {
    wp_die( __('You do not have sufficient permissions to access this page.', $i18n_domain) );
  }

  echo "<div class='wrap'>\n";
  echo "<h2>$title</h2>\n";
  switch($title) {
    case sprintf(__('Artistography %s', $i18n_domain), ADMIN_MENU_OPTIONS):
      require_once('admin/options.php.inc');
      break;

    case sprintf(__('Artistography %s', $i18n_domain), ADMIN_MENU_ORDERS):
      require_once('admin/orders.php.inc');
      break;

    case sprintf(__('Artistography %s', $i18n_domain), ADMIN_MENU_MANAGE_ARTISTS):
      require_once('admin/manage_artists.php.inc');
      break;

    case sprintf(__('Artistography %s', $i18n_domain), ADMIN_MENU_MANAGE_SONGS):
      require_once('admin/manage_songs.php.inc');
      break;

    case sprintf(__('Artistography %s', $i18n_domain), ADMIN_MENU_MANAGE_ALBUMS):
      require_once('admin/manage_music.php.inc');
      break;

    case sprintf(__('Artistography %s', $i18n_domain), ADMIN_MENU_MANAGE_DOWNLOADS):
      require_once('admin/download_insert.php.inc');
      require_once('admin/download_stats.php.inc');
      break;

    case sprintf(__('Artistography %s', $i18n_domain), ADMIN_MENU_MANAGE_ARTIST_ALBUM_LINKER):
      require_once('admin/manage_discography.php.inc');
      break;

    case sprintf(__('Artistography %s', $i18n_domain), ADMIN_MENU_MANAGE_SONG_ALBUM_LINKER):
      require_once('admin/manage_song-album-linker.php.inc');
      break;

    case sprintf(__('Artistography %s', $i18n_domain), ADMIN_MENU_MANAGE_GALLERIES):
      require_once('admin/manage_galleries.php.inc');
      break;

    case sprintf(__('Artistography %s', $i18n_domain), ADMIN_MENU_FTP_UPLOADER):
      require_once('admin/ftp_uploader.php.inc');
      break;

    case sprintf(__('Artistography %s', $i18n_domain), ADMIN_MENU_ABOUT):    default:
      require_once('admin/about.php.inc');
      break;
  }
  echo '</div>';
}
 /* END ADMIN MENU INTERFACE */

?>
