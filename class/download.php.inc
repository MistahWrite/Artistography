<?php
add_shortcode( 'artistography_download', 'Download::shortCodeDownload' );

class Download
{
    protected static $downloadTable = NULL;  // TABLE NAME of MUSIC ALBUM TABLE

     // property declaration
    public $id = NULL;
    public $enabled = NULL;

    public $file_name = NULL;
    public $download_count = NULL;

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

        self::$downloadTable = $wpdb->prefix . $TABLE_NAME[TABLE_ARTIST_FILE_DOWNLOAD];

        $this->loadAll();
    }

    public function __destruct() {
         /* Just in case we need it */
    }

     // [artistography_download id=""]
     // this is a hidden/undocumented shortcode used by the artistography. i.e. user shouldn't use it
    public static function shortCodeDownload( $atts, $content=null, $code="" ) {
      GLOBAL $wpdb, $i18n_domain, $artistography_plugin_dir;

      $Download = new Download;
      $Music = new Music;
      $Orders = new Orders;

      $uri = explode('/', $_SERVER["REQUEST_URI"]);
      $this_id = 0; $download_id = 0;

      $post_slug = get_post(get_option('wp_artistography_download_page'))->post_name;

      foreach ($uri as $key => $value) {
        if ($value === $post_slug) {
          $this_id = $key + 1;
          $download_id = $uri[$this_id];
          break;
        }
      }
      unset($value);

      if($download_id == 0 || $download_id == NULL) {
          $html = "Nothing to see here...";
      } else {
	$price = $Music->loadByDownloadId($download_id)->price;
	if ($price == "0.00") {
		$html = $Download->do_download($download_id);
	} else {
	   /* Music is not free */
	  /* TODO: Test if order has processed */
		$Orders->loadByUserId(get_current_user_id());
		$order_processed = $Orders->locateOrderMusicDownloadId($Music->id);
		if($order_processed) {
			$html = $Download->do_download($download_id);
		} else {
			$html = "This download is not free!<br/>\n";
			$html .= "[artistography_album_download_link id='$Music->id']<br/>\n";
		}
	}
      }
      return do_shortcode($html);
    }

    public function do_download($download_id) {
	$Download = new Download;

          if($Download->loadById ($download_id)) {
              $Download->increaseCounterById ($download_id);

              $loc = SITEURL . "/downloads/" . $Download->file_name;

              $donate_email = get_option('wp_artistography_donate_email');

              $html = "<meta http-equiv='refresh' content='2;url=$loc'>"
                    . __("If your download does not begin in 2-5 seconds please click here", $i18n_domain)
                    . ": <a href='$loc' target='_blank'>$loc</a><br/><br/>"
                    . sprintf(__("This has been downloaded <i><b>%s</b></i> times.", $i18n_domain), $Download->download_count)
                    . __("We appreciate your interest, and hope that you will consider at least a $1.00 donation by clicking the link below.  Feel free to tell us why you're donating to us as it means a lot.  Many thanks from the crew!", $i18n_domain)
                    . '<br/><br/>
                        <center>
                            <form action="https://www.paypal.com/cgi-bin/webscr" method="post">
                                <input type="hidden" name="cmd" value="_donations">
                                <input type="hidden" name="business" value="' .$donate_email. '">
                                <input type="hidden" name="lc" value="US">
                                <input type="hidden" name="no_note" value="0">
                                <input type="hidden" name="currency_code" value="USD">
                                <input type="hidden" name="bn" value="PP-DonationsBF:btn_donateCC_LG.gif:NonHostedGuest">
                                <input type="image" src="https://www.paypalobjects.com/WEBSCR-640-20110429-1/en_US/i/btn/btn_donateCC_LG.gif" border="0" name="submit" alt="PayPal - The safer, easier way to pay online!">
                                <img alt="" border="0" src="https://www.paypalobjects.com/WEBSCR-640-20110429-1/en_US/i/scr/pixel.gif" width="1" height="1">
                            </form>
                            <br/><br/>
                        </center>';
          } else {
              $html = "Invalid download id.  Nothing to see here...";
          }
	return $html;
    }

    protected function &loadCurrNodeValues () {
        GLOBAL $wpdb;

        if ($this->getTotalRows() > 0) {
            $this->query_result = $wpdb->get_row($this->query, OBJECT, $this->getCurrNode());

             // meta data info update
            $this->id = $this->query_result->id;

            $this->enabled = $this->query_result->id;
            $this->file_name = $this->query_result->file_name;
            $this->download_count = $this->query_result->download_count;

            $this->poster_id = $this->query_result->poster_id;
            $this->last_updated_by_id = $this->query_result->last_updated_by_id;
            $this->create_date = $this->query_result->create_date;
            $this->update_date = $this->query_result->update_date;

        } else {
            $this->query_result =

            $this->id =
            $this->poster_id =
            $this->last_updated_by_id =
            $this->create_date =
            $this->update_date =

            $this->enabled =
            $this->file_name =
            $this->download_count = 0;
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
                        FROM  " .self::$downloadTable. "
                        WHERE id = %u", $id);
        $this->setTotalRows($wpdb->query($this->query));

        if ($this->getTotalRows() === FALSE) {
		return false; // failure
	}

        $this->setCurrNode(0); // set to first node
        return $this->loadCurrNodeValues();
    }

    public function &loadAll ($order_by = 'id') {
        GLOBAL $wpdb, $i18n_domain;

        $this->query = "SELECT *
                        FROM " .self::$downloadTable. "
                        ORDER BY $order_by";
        $this->setTotalRows($wpdb->query($this->query));

        if ($this->getTotalRows() === FALSE) wp_die( sprintf(__('An error occurred while trying to perform a query: "%s"', $i18n_domain), $this->query) );

        $this->setCurrNode(0); // set to first node
        return $this->loadCurrNodeValues();
    }

    public function deleteById ($id) {
        GLOBAL $wpdb, $i18n_domain;

        $this->query = $wpdb->prepare(
                       "DELETE FROM " .self::$downloadTable. "
                        WHERE id = %u", $id);
        $this->setTotalRows($wpdb->query($this->query));

        if ($this->getTotalRows() === FALSE) {
            wp_die( sprintf(__('An error occurred while trying to perform a query: "%s"', $i18n_domain), $this->query) );
        }
        return $this->getTotalRows();
    }

    public function increaseCounterById ($download_id) {
        GLOBAL $wpdb, $i18n_domain;

         //increase counter
        $this->query = $wpdb->prepare(
            "UPDATE " .self::$downloadTable. "
             SET download_count=download_count+1
             WHERE id = %u", $download_id);
        $wpdb->query($this->query);
    }

    public function updateById ($download_id, $enabled, $file_name) {
        GLOBAL $wpdb, $i18n_domain, $current_user;

        get_currentuserinfo();

        $this->query = $wpdb->prepare(
            "UPDATE " .self::$downloadTable. "
             SET update_date = now(),
                 enabled = %b,
                 file_name = %s,
                 last_updated_by_id = %u
             WHERE id = %u",
             $enabled, $file_name, $current_user->ID, $download_id);
        $this->setTotalRows($wpdb->query($this->query));

        if ($this->getTotalRows() === FALSE) {
            wp_die( sprintf(__('An error occurred while trying to perform a query: "%s"', $i18n_domain), $this->query) );
        } else {
             // was entered successfully into database
            return $this->getTotalRows();
        }
    }

    public function insert ($file_name) {
        GLOBAL $wpdb, $i18n_domain, $current_user;

        get_currentuserinfo();

        $this->query = $wpdb->prepare(
                       "INSERT INTO " .(string)self::$downloadTable. "
                        (create_date, update_date, poster_id, last_updated_by_id, enabled, file_name)
                        VALUES (now(), now(), %u, %u, %b, %s)",
                        $current_user->ID, $current_user->ID, true, $file_name);
        $this->setTotalRows($wpdb->query($this->query));

        if ($this->getTotalRows() === 0 OR $this->getTotalRows() === FALSE) {
            return false;
        } else {
             // was entered successfully into database
            return true;
        }
    }
} /* end class Download */
?>
