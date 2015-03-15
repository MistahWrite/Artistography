<?php
add_shortcode( 'artistography_show_cart', 'Orders::shortCodeShowCart' );
add_shortcode( 'artistography_show_checkout', 'Orders::shortCodeShowCheckout' );
add_shortcode( 'artistography_show_thankyou', 'Orders::shortCodeShowThankyou' );
add_shortcode( 'artistography_show_orders', 'Orders::shortCodeShowOrders' );
add_shortcode( 'artistography_ipn', 'Orders::shortCodeIPN' );

//AJAX Administration
add_action('wp_ajax_View_Order', 'Orders::callback_View_Order');

add_action('wp_ajax_Get_Basic_Cart_Contents', 'Orders::callback_Get_Basic_Cart_Contents');
add_action('wp_ajax_nopriv_Get_Basic_Cart_Contents', 'Orders::callback_Get_Basic_Cart_Contents');

add_action('wp_ajax_Get_Cart_Contents', 'Orders::callback_Get_Cart_Contents');
add_action('wp_ajax_nopriv_Get_Cart_Contents', 'Orders::callback_Get_Cart_Contents');

add_action('wp_ajax_Add_Album_Download_To_Cart', 'Orders::callback_Add_Album_Download_To_Cart');
add_action('wp_ajax_nopriv_Add_Album_Download_To_Cart', 'Orders::callback_Add_Album_Download_To_Cart');

add_action('wp_ajax_Add_Song_To_Cart', 'Orders::callback_Add_Song_To_Cart');
add_action('wp_ajax_nopriv_Add_Song_To_Cart', 'Orders::callback_Add_Song_To_Cart');

add_action('wp_ajax_Remove_From_Cart', 'Orders::callback_Remove_From_Cart');
add_action('wp_ajax_nopriv_Remove_From_Cart', 'Orders::callback_Remove_From_Cart');

class Orders
{
	protected static $ordersTable = NULL;  // TABLE NAME of MUSIC ALBUM TABLE

	public $id = NULL;

	public $payer_id = NULL;
	public $address_status = NULL;
	public $payer_status = NULL;
	public $forename = NULL;
        public $surname = NULL;
        public $email = NULL;
        public $address_line_1 = NULL;
        public $postcode = NULL;
        public $town = NULL;
	public $state = NULL;
	public $country_code = NULL;
	public $contact_phone = NULL;
	public $itemsOrdered = NULL;
	public $itemsNumberOrdered = NULL;
	public $quantityOrdered = NULL;
        public $created = NULL;
	public $txn_id = NULL;
	public $txn_type = NULL;
	public $parent_txn_id = NULL;
	public $verify_sign = NULL;
	public $custom = NULL;
        public $payment_status = NULL;
        public $payment_amount = NULL;
        public $payment_currency = NULL;
        public $item_name = NULL;
	public $item_number = NULL;
	public $quantity = NULL;
	public $shipping = NULL;
	public $mc_fee = NULL;
	public $mc_gross = NULL;
	public $mc_shipping = NULL;
	public $mc_handling = NULL;
	public $tax = NULL;
	public $refund_reason_code = NULL;
	public $refund_receipt_id = NULL;
	public $memo = NULL;
	public $pending_reason = NULL;

	public $user_id = NULL;

	 // this class interfaces with the database
	protected $query = NULL;
	protected $query_result = NULL;
	protected $query_total_rows = NULL;
	protected $query_curr_node = NULL;

	 // set funcs (protected)
	protected function setTotalRows ($rows) {    $this->query_total_rows = $rows;    }
	protected function setCurrNode ($node)  {    $this->query_curr_node = $node;    }

	 // get funcs (public)
	public function getOrdersTableName () {return self::$ordersTable; }
	public function getTotalRows ()   {    return $this->query_total_rows;    }
	public function getCurrNode ()    {    return $this->query_curr_node;    }
	public function getQueryResult () {    return $this->query_result;    }

        public static function callback_Add_Album_Download_To_Cart() {
                $id = $_POST['album_id'];

                $_SESSION['artistography_cart']["album_download_$id"] = 1;
                die;
        }

	public static function callback_Add_Song_To_Cart() {
		$id = $_POST['song_id'];

                $_SESSION['artistography_cart']["song_download_$id"] = 1;
		die;
	}

	public static function callback_Remove_From_Cart() {
		$product = $_POST['product'];

		unset($_SESSION['artistography_cart'][$product]);
		die;
	}

	public static function callback_View_Order(){
		GLOBAL $i18n_domain;

		$orders = new Orders;

		if($orders->loadById($_POST['order_id'])->getTotalRows() > 0) {
			$products = unserialize(stripslashes($orders->itemsOrdered));

			$output = $orders->id
				."##$$##". stripslashes($orders->forename)
				    ." ".  stripslashes($orders->surname)
		                ."##$$##". stripslashes($orders->email)
				."##$$##". stripslashes($orders->address_line_1)
				."##$$##". stripslashes($orders->town)
			        ."##$$##". stripslashes($orders->state)
				."##$$##". stripslashes($orders->country_code)
				."##$$##". stripslashes($orders->contact_phone)
				."##$$##". stripslashes($orders->payer_id)
				."##$$##". stripslashes($orders->address_status)
				."##$$##". stripslashes($orders->payer_status)
				."##$$##". stripslashes($orders->itemsNumberOrdered)
				."##$$##". stripslashes($orders->quantityOrdered)
				."##$$##". stripslashes($orders->txn_type)
			        ."##$$##". stripslashes($orders->parent_txn_id)
				."##$$##". stripslashes($orders->verify_sign)
			        ."##$$##". stripslashes($orders->quantity)
				."##$$##". stripslashes($orders->shipping)
				."##$$##". stripslashes($orders->mc_fee)
				."##$$##". stripslashes($orders->mc_gross)
				."##$$##". stripslashes($orders->mc_shipping)
				."##$$##". stripslashes($orders->mc_handling)
				."##$$##". stripslashes($orders->tax)
				."##$$##". stripslashes($orders->refund_reason_code)
				."##$$##". stripslashes($orders->refund_receipt_id)
				."##$$##". stripslashes($orders->memo)
			 	."##$$##". stripslashes($orders->pending_reason)
		                ."##$$##". stripslashes($orders->postcode)
                      		."##$$##";
				for($j = 0; $j < count($products); $j++) {
                        		$output .= ($j+1) .$products[$j]. "\r\n";
                		}
                      $output .= "##$$##". stripslashes($orders->created)
                       		."##$$##". stripslashes($orders->txn_id)
                       		."##$$##". stripslashes($orders->custom)
               			."##$$##". stripslashes($orders->payment_status)
               			."##$$##". stripslashes($orders->payment_amount)
               			."##$$##". stripslashes($orders->payment_currency)
               			."##$$##". stripslashes($orders->item_name)
               			."##$$##". stripslashes($orders->item_number)
               			."##$$##". stripslashes($orders->user_id);
			echo $output;
		}

		unset($artist);
		die;
	}

	public function __construct() {
		GLOBAL $wpdb, $TABLE_NAME;

		self::$ordersTable = $wpdb->prefix . $TABLE_NAME[TABLE_ARTIST_ORDERS];

		$this->loadAll();
	}

	public function __destruct() {
		 /* Just in case we need it */
	}

	protected function &loadCurrNodeValues () {
        	GLOBAL $wpdb, $_SERVER;

        	if ($this->getTotalRows() > 0) {
            		$this->query_result = $wpdb->get_row($this->query, OBJECT, $this->getCurrNode());

			 // meta data info update
			$this->id = $this->query_result->id;
			$this->forename = $this->query_result->forename;
			$this->surname = $this->query_result->surname;
			$this->email = $this->query_result->email;
			$this->address_line_1 = $this->query_result->address_line_1;
			$this->postcode = $this->query_result->postcode;
			$this->town = $this->query_result->town;
			$this->state = $this->query_result->state;

			$this->itemsOrdered = $this->query_result->itemsOrdered;
			$this->created = $this->query_result->create_date;
			$this->txn_id = $this->query_result->txn_id;
			$this->user_id = $this->query_result->user_id;
			$this->payment_status = $this->query_result->payment_status;
			$this->payment_amount = $this->query_result->payment_amount;
			$this->payment_currency = $this->query_result->payment_currency;
			$this->item_name = $this->query_result->item_name;
		        $this->item_number = $this->query_result->item_number;

			$this->payer_id = $this->query_result->payer_id;
			$this->address_status = $this->query_result->address_status;
		        $this->payer_status = $this->query_result->payer_status;
		        $this->state = $this->query_result->state;
		        $this->country_code = $this->query_result->country_code;
		        $this->contact_phone = $this->query_result->contact_phone;
		        $this->itemsNumberOrdered = $this->query_result->itemsNumberOrdered;
		        $this->quantityOrdered = $this->query_result->quantityOrdered;
		        $this->txn_type = $this->query_result->txn_type;
		        $this->parent_txn_id = $this->query_result->parent_txn_id;
		        $this->verify_sign = $this->query_result->verify_sign;
		        $this->quantity = $this->query_result->quantity;
		        $this->shipping = $this->query_result->shipping;
		        $this->mc_fee = $this->query_result->mc_fee;
		        $this->mc_gross =$this->query_result->mc_gross;
	                $this->mc_shipping = $this->query_result->mc_shipping;
			$this->mc_handling = $this->query_result->mc_handling;
                        $this->tax = $this->query_result->tax;
                        $this->refund_reason_code = $this->query_result->refund_reason_code;
		        $this->refund_receipt_id = $this->query_result->refund_receipt_id;
		        $this->memo = $this->query_result->memo;
	                $this->pending_reason = $this->query_result->pending_reason;
		} else {
			$this->query_result =
            
			$this->payer_id =
			$this->address_status =
			$this->payer_status =
			$this->state =
			$this->country_code =
			$this->contact_phone =
			$this->itemsNumberOrdered =
			$this->quantityOrdered =
			$this->txn_type =
			$this->parent_txn_id =
			$this->verify_sign =
			$this->quantity =
		        $this->shipping =
		        $this->mc_fee =
		        $this->mc_gross =
	        	$this->mc_shipping =
			$this->mc_handling = 
	        	$this->tax =
	       		$this->refund_reason_code =
		        $this->refund_receipt_id =
	        	$this->memo =
		        $this->pending_reason =

			$this->id =
			$this->forename =          
			$this->surname =
			$this->email =
			$this->address_line_1 =
			$this->postcode =   
			$this->town =       

			$this->itemsOrdered =
			$this->create_date =
			$this->txn_id =
			$this->custom =

			$this->payment_status =
			$this->payment_amount =
			$this->payment_currency =
			$this->item_name =
			$this->item_number =
			$this->user_id = 0;
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
                        FROM " .self::$ordersTable. "
                        WHERE id = %u", $id);
        $this->setTotalRows($wpdb->query($this->query));

        if ($this->getTotalRows() === FALSE) wp_die( sprintf(__('An error occurred while trying to perform a query: "%s"', $i18n_domain), $this->query) );

        $this->setCurrNode(0); // set to first node
        return $this->loadCurrNodeValues();
    }

    public function &loadByUserId ($id, $order_by = 'id') {
        GLOBAL $wpdb, $i18n_domain;

        $this->query = $wpdb->prepare("SELECT *
                        FROM " .self::$ordersTable. "
                        WHERE user_id = %u
			ORDER BY $order_by", $id);
        $this->setTotalRows($wpdb->query($this->query));

        if ($this->getTotalRows() === FALSE) wp_die( sprintf(__('An error occurred while trying to perform a query: "%s"', $i18n_domain), $this->query) );

        $this->setCurrNode(0); // set to first node
        return $this->loadCurrNodeValues();
    }

    public function &loadAll ($order_by = 'id') {
        GLOBAL $wpdb, $i18n_domain;

        $this->query = "SELECT *
                        FROM " .self::$ordersTable. "
                        ORDER BY $order_by";
        $this->setTotalRows($wpdb->query($this->query));

        if ($this->getTotalRows() === FALSE) wp_die( sprintf(__('An error occurred while trying to perform a query: "%s"', $i18n_domain), $this->query) );

        $this->setCurrNode(0); // set to first node
        return $this->loadCurrNodeValues();
    }

	public function locateOrderMusicDownloadId($music_id) {
		GLOBAL $wpdb, $i18n_domain;

		$Music = new Music;
		$Music->loadById($music_id);
		$item_name = $Music->id .' - '. $Music->artist_name .' - '. $Music->album_name;
		
		$found = false;
		$i = 0;
		
        	if ($this->getTotalRows() > 0) {
                	do {
				$products = unserialize($this->itemsOrdered);
				if(strcmp("Completed", $this->payment_status) == 0) {
                			for($j = 0; $j < count($products); $j++) {
						$product_id = str_replace('album_download_', '', explode(' - ', $products[$j])[0]);
                        			if (strcmp($music_id, $product_id) == 0) {
							$found = true;
						}
                			}
				}
                		$this->getNodeNext();
                		$i++;
        	        } while($i<$this->getTotalRows());
	        } else {
			$found = false;
		}

		return $found;
	}

        public static function shortCodeShowCart( $atts, $content=null, $code="" ) {
                GLOBAL $i18n_domain;

		 /*** Display Cart Contents ***/
		$html = self::showCart();
		return $html;
	}

	public static function shortCodeShowCheckout( $atts, $content=null, $code="" ) {
		GLOBAL $i18n_domain, $buynow_icon_url, $buynow_icon_width, $buynow_icon_height;
		$html = '';

		$album = new Music;
		$artist = new Artist;
		$song = new Song;

		$sandbox = get_option('wp_artistography_paypal_sandbox');

		if(!is_user_logged_in()) {
			$html .= "In order to buy you must first register or be logged into our website that way we can keep track of your orders, and you can return to download your files again at a later time if you lose them.<br/>\n";
			$html .= "<div>Existing Users:<br/>\n";
			$html .= wp_login_form(array('echo' => false));
			$html .= "<a href=\"" .wp_lostpassword_url( get_permalink() ). "\" title=\"Lost Password\">Lost Password</a>";
			$html .= " | New Users: <a href=\"" .wp_registration_url(). "\" target='_blank'>Register</a>";
			$html .= "</div>\n";
		} else {
			$result = self::showInvoice();
			if(!$result) {
				$html .= "<font style='font-size:14pt'>The shopping cart is empty.</font>";
			} else {
				$html .= "Welcome, " .wp_get_current_user()->user_login. "!";
				$html .= $result;

				if($sandbox) {
				  $html .= "<form action='https://www.sandbox.paypal.com/cgi-bin/webscr' method='post' class='standard-form'>";
				} else {
				  $html .= "<form action='https://www.paypal.com/cgi-bin/webscr' method='post' class='standard-form'>";
				}
				$i = 1;
				foreach($_SESSION['artistography_cart'] as $product => $qty) {
		                  if(strpos($product, 'album_download_') !== FALSE) {
                     		  	$product_id = str_replace('album_download_', '', $product);
				  	$album->loadById($product_id);
				  	$html .= "<input type='hidden' name='item_name_$i' value='$product - $album->artist_name - $album->album_name' />";
				  	$html .= "<input type='hidden' name='quantity_$i' value='$qty' />";
				  	$html .= "<input type='hidden' name='amount_$i' value='$album->price' />";
				  } else if (strpos($product, 'song_download_') !== FALSE) {
                                        $product_id = str_replace('song_download_', '', $product);
                                        $song->loadById($product_id);
                                        $html .= "<input type='hidden' name='item_name_$i' value='$product - " .$artist->loadById($song->artist_id)->name. " - $song->name' />";
                                        $html .= "<input type='hidden' name='quantity_$i' value='$qty' />";
                                        $html .= "<input type='hidden' name='amount_$i' value='$song->price' />";
				  }
				  $i++;
				}
				$html .= "<input type='hidden' name='cmd' value='_cart' />
					  <input type='hidden' name='upload' value='1' />
					  <input type='hidden' name='business' value='" .get_option('wp_artistography_donate_email'). "' />";

				$html .= "<input type='hidden' name='custom' value='user_id_" .get_current_user_id(). "' />";
				$html .= "<input type='hidden' name='currency_code' value='USD' />\n";
                                $html .= "<input type='hidden' name='lc' value='US' />\n";
                                $html .= "<input type='hidden' name='rm' value='2' />\n";
                                $html .= "<input type='hidden' name='return' value='" .get_permalink(get_option('wp_artistography_thankyou_page')). "' />\n";
                                $html .= "<input type='hidden' name='cancel_return' value='" .get_permalink(get_option('wp_artistography_cart_page')). "' />\n";
                                $html .= "<input type='hidden' name='notify_url' value='" .get_permalink(get_option('wp_artistography_ipn_page')). "' />\n";

                		$html .= "  <div align='right'><input type='submit' value=''
						style='border:none;
						       width: " .$buynow_icon_width. "px; height: " .$buynow_icon_height. "px;
				              	       background:url($buynow_icon_url) no-repeat;
					               background-size:" .$buynow_icon_width. "px " .$buynow_icon_height. "px;' align='right' /></div>";
				$html .= "</form>";
				$html .= "<button class='edit_cart'>Go Back to Edit Cart</button>";
			}
		}
                return $html;
        }

        public static function shortCodeShowThankyou( $atts, $content=null, $code="" ) {
                GLOBAL $i18n_domain;
		$html = "";

		 /* empty the cart */
		session_destroy();

                $html .= "Thank you for completing your order with us.  To see all of your completed orders please click <a href='" .SITEURL. "/artistography_orders'>here</a>.<br/><br/>";
		$html .= "";
                return $html;
        }

        public static function shortCodeShowOrders( $atts, $content=null, $code="" ) {
                GLOBAL $i18n_domain;

                $html = "";
                return $html;
        }

	 // https://github.com/paypal/ipn-code-samples/blob/master/paypal_ipn.php
	public static function shortCodeIPN( $atts, $content=null, $code="" ) {
                GLOBAL $wpdb, $i18n_domain, $TABLE_NAME;
                $html = "";
		$header = "";

		$orders = new Orders;

		// Read POST data
		// // reading posted data directly from $_POST causes serialization
		// // issues with array data in POST. Reading raw POST data from input stream instead.
		 $raw_post_data = file_get_contents('php://input');
		 $raw_post_array = explode('&', $raw_post_data);
		 $myPost = array();
		 foreach ($raw_post_array as $keyval) {
			$keyval = explode ('=', $keyval);
		 		if (count($keyval) == 2)
		 				$myPost[$keyval[0]] = urldecode($keyval[1]);
		}

		// read the post from PayPal system and add 'cmd'
		$req = 'cmd=_notify-validate';
		if(function_exists('get_magic_quotes_gpc')) {
		 	$get_magic_quotes_exists = true;
		}
		foreach ($myPost as $key => $value) {
			if($get_magic_quotes_exists == true && get_magic_quotes_gpc() == 1) {
				$value = urlencode(stripslashes($value));
			} else {
				$value = urlencode($value);
			}
			$req .= "&$key=$value";
		}
		// Post IPN data back to PayPal to validate the IPN data is genuine
		// Without this step anyone can fake IPN data
		if(true == get_option('wp_artistography_paypal_sandbox')) {
			$paypal_url = 'https://www.sandbox.paypal.com/cgi-bin/webscr';
		} else {
			$paypal_url = 'https://www.paypal.com/cgi-bin/webscr';
		}

		$ch = curl_init($paypal_url);
		if ($ch == FALSE) {
				return FALSE;
		}
		curl_setopt($ch, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_1);
		curl_setopt($ch, CURLOPT_POST, 1);
		curl_setopt($ch, CURLOPT_RETURNTRANSFER,1);
		curl_setopt($ch, CURLOPT_POSTFIELDS, $req);
		curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, 1);
		curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, 2);
		curl_setopt($ch, CURLOPT_FORBID_REUSE, 1);
		if(DEBUG == true) {
				curl_setopt($ch, CURLOPT_HEADER, 1);
					curl_setopt($ch, CURLINFO_HEADER_OUT, 1);
		}
		// CONFIG: Optional proxy configuration
		//curl_setopt($ch, CURLOPT_PROXY, $proxy);
		//curl_setopt($ch, CURLOPT_HTTPPROXYTUNNEL, 1);
		// Set TCP timeout to 30 seconds
		curl_setopt($ch, CURLOPT_CONNECTTIMEOUT, 30);
		curl_setopt($ch, CURLOPT_HTTPHEADER, array('Connection: Close'));

		// CONFIG: Please download 'cacert.pem' from "http://curl.haxx.se/docs/caextract.html" and set the directory path
		// of the certificate as shown below. Ensure the file is readable by the webserver.
		// This is mandatory for some environments.
		$cert = __DIR__ . "/cacert.pem";
		curl_setopt($ch, CURLOPT_CAINFO, $cert);

		$res = curl_exec($ch);
		if (curl_errno($ch) != 0) // cURL error
		{
			if(DEBUG == true) {	
				error_log(date('[Y-m-d H:i e] '). "Can't connect to PayPal to validate IPN message: " . curl_error($ch) . PHP_EOL, 3, LOG_FILE);
			}
			curl_close($ch);
			exit;
		} else {
			// Log the entire HTTP response if debug is switched on.
			if(DEBUG == true) {
				error_log(date('[Y-m-d H:i e] '). "HTTP request of validation request:". curl_getinfo($ch, CURLINFO_HEADER_OUT) ." for IPN payload: $req" . PHP_EOL, 3, LOG_FILE);
				error_log(date('[Y-m-d H:i e] '). "HTTP response of validation request: $res" . PHP_EOL, 3, LOG_FILE);
			}
			curl_close($ch);
		}

		// Inspect IPN validation result and act accordingly
		// Split response headers and payload, a better way for strcmp
		$tokens = explode("\r\n\r\n", trim($res));
		$res = trim(end($tokens));
		if (strcmp ($res, "VERIFIED") == 0) {
			// check whether the payment_status is Completed
			// check that txn_id has not been previously processed
			// check that receiver_email is your PayPal email
			// check that payment_amount/payment_currency are correct
			// process payment and mark item as paid
			$addressStatus		= $_POST['address_status'];
			$payerStatus		= $_POST['payer_status'];
			$firstName		= $_POST['first_name'];
			$lastName		= $_POST['last_name'];
			$payerEmail		= $_POST['payer_email'];
			$payerID		= $_POST['payer_id'];
			$addressStatus		= $_POST['address_status'];
			$addressStreet		= $_POST['address_street'];
			$addressZip		= $_POST['address_zip'];
			$addressCity		= $_POST['address_city'];
			$addressState		= $_POST['address_state'];
			$countryCode		= $_POST['country_code'];
			$contactPhone		= $_POST['contact_phone'];
			$productsBought		= $_POST[''];
			$verifySign		= $_POST['verify_sign'];
			$txnType		= $_POST['txn_type'];
			$txnID			= $_POST['txn_id'];
			$parentTxnId		= $_POST['parent_txn_id'];
			$custom			= $_POST['custom'];

			$paymentStatus		= $_POST['payment_status'];
			$paymentAmount		= $_POST['payment_amount'];
			$paymentCurrency	= $_POST['mc_currency'];
			$receiverEmail		= $_POST['receiver_email'];
			$itemName		= $_POST['item_name'];
			$itemNumber		= $_POST['item_number'];
			$quantity		= $_POST['quantity'];
			$shipping		= $_POST['shipping'];
			$tax			= $_POST['tax'];
			$mcFee			= $_POST['mc_fee'];
			$mcGross		= $_POST['mc_gross'];
			$mcHandling		= $_POST['mc_handling'];
			$mcShipping		= $_POST['mc_shipping'];
			$memo			= $_POST['memo'];
			$pendingReason		= $_POST['pending_reason'];
			$refundReasonCode	= $_POST['reason_code'];
			$refundReceiptID	= $_POST['receipt_id'];

			$userID = str_replace('user_id_', '', $custom);

			//Used to store quickly items bought
			$i = 1;
			foreach($_POST as $key => $value) {
				if($key == 'item_name'.$i) {
					$products_bought[] = $value;
					$i++;
				}
			}
			$i = 1;
			foreach($_POST as $key => $value) {
				if ($key == 'item_number'.$i) {
					$products_number_bought[] = $value;
					$i++;
				}
			}
			$i = 1;
			foreach($_POST as $key => $value) {
				if ($key == 'quantity'.$i) {
					$quantity_bought[] = $value;
					$i++;
				}
			}
			$products = serialize($products_bought);
			$products_number = serialize($products_number_bought);
			$quantity = serialize($quantity_bought);

			$wpdb->insert($orders->getOrdersTableName(),
			array(	'address_status'	=> $addressStatus,
				'payer_status'		=> $payerStatus,
				'forename'		=> $firstName,
				'surname'		=> $lastName,
				'email'			=> $payerEmail,
				'payer_id'		=> $payerID,
				'address_line_1'	=> $addressStreet,
				'postcode'		=> $addressZip,
				'town'			=> $addressCity,
				'state'			=> $addressState,
				'country_code'		=> $countryCode,
				'contact_phone'		=> $contactPhone,
				'itemsOrdered'		=> $products,
				'itemsNumberOrdered'	=> $products_number,
				'quantityOrdered'	=> $quantity,
				'create_date'		=> current_time('mysql'),
				'parent_txn_id'		=> $parentTxnId,
				'txn_type'		=> $txnType,
				'txn_id'		=> $txnID,
				'verify_sign'		=> $verifySign,
				'payment_status'	=> $paymentStatus,
				'payment_amount'	=> $paymentAmount,
				'payment_currency'	=> $paymentCurrency,
				'item_name'		=> $itemName,
				'item_number'		=> $itemNumber,
				'quantity'		=> $quantity,
				'shipping'		=> $shipping,
				'tax'			=> $tax,
				'mc_fee'		=> $mcFee,
				'mc_gross'		=> $mcGross,
				'mc_handling'		=> $mcHandling,
				'mc_shipping'		=> $mcShipping,
				'memo'			=> $memo,
				'pending_reason'	=> $pendingReason,
				'refund_reason_code'	=> $refundReasonCode,
				'refund_receipt_id'	=> $refundReceiptID,
				'user_id'		=> $userID
			),
			array(	'%s',			// FORENAME
				'%s',			// SURNAME
				'%s',			// EMAIL
				'%s',			// ADDRESS 1
				'%s',			// PCODE
				'%s',			// TOWN
				'%s',			// STATE
				'%s',			// ORDERED
				'%s',			// CREATED
				'%s',			// TXN ID
				'%s',			// PAYMENT STATUS
				'%s',			// PAYMENT AMOUNT
				'%s',			// PAYMENT CURRENCY
				'%s',			// ITEM NAME
				'%s',			// ITEM NUMBER
				'%s',
				'%s',
				'%s',
				'%s',
				'%s',
				'%s',
				'%s',
				'%s',
				'%s',
				'%s',
				'%s',
				'%s',
				'%s',
				'%s',
				'%s',
				'%s',
				'%s',
				'%s',
				'%s',
				'%s',
				'%s',
				'%s'			// WORDPRESS USER ID
			));
                        if(DEBUG == true) {
				error_log(date('[Y-m-d H:i e] '), "Insert IPN: $wpdb->last_result" . PHP_EOL, 3, LOG_FILE);
			}
		} else if (strcmp($res, "INVALID") == 0) {
			// You may prefer to store the transaction even if fail
			// log for manual investigation
			// Add business logic here which deals with invalid IPN messages
			if(DEBUG == true) {
				error_log(date('[Y-m-d H:i e] '), "Invalid IPN: $req" . PHP_EOL, 3, LOG_FILE);
			}
		} /* end if */

                return $html;
        }

	public static function processCartItem($product, $qty, &$total, $showActions = true) {
		GLOBAL $i18n_domain;

		$order = new Orders;
		$album = new Music;
		$song = new Song;
		$artist = new Artist;
		$html = "";

                $html .= "<tr>
                                <td>$qty</td>";
	
                if(strpos($product, 'album_download_') !== FALSE) {
                    $product_id = str_replace('album_download_', '', $product);

                    $album->loadById($product_id);
                    $total += ($qty * (float)($album->price));

                    $html .= "      <td><img src='" .stripslashes($album->picture_url). "' width='50' height='50' />
                                            $album->artist_name - $album->album_name
                                    </td>
                                    <td>" .CURRENCY. "$album->price</td>";
                } else if(strpos($product, 'song_download_') !== FALSE){
                    $product_id = str_replace('song_download_', '', $product);
                    $song->loadById($product_id);
                    $total += ($qty * (float)($song->price));

                    $html .= "      <td>
                                            " .$artist->loadById($song->artist_id)->name. " - $song->name
                                    </td>
                                    <td>" .CURRENCY. "$song->price</td>";
                }
		if($showActions) {
                     // REMOVE BUTTON
                    $html .= "  <td>
                                        <button class='remove_from_cart' id='$product'>Remove</button>
                            </td>";
		}
                $html .= "</tr>";

		return $html;
	}

	public static function showCart() {
		GLOBAL $ajax_loader_url;
		$html = "<div class='artistography_cart'><center><img style='border:none' src='$ajax_loader_url' /></center></div>";
                return $html;
	}

        public static function showBasicCart() {
                GLOBAL $ajax_loader_url;
                $html = "<div class='artistography_basic_cart'><center><img style='border:none' src='$ajax_loader_url' /></center></div>";
                return $html;
        }

	public static function getCartContents($showActions = true) {
		GLOBAL $i18n_domain, $checkout_icon_url, $checkout_icon_width, $checkout_icon_height;
		$total = (float)"0.00";
		$html = "";

		if(!empty($_SESSION['artistography_cart'])) {
                  $html .= "<center><table>
                           <tr>
                              <th>" .__("Qty", $i18n_domain). "</th>
                              <th>" .__("Item", $i18n_domain). "</th>
                              <th>" .__("Price", $i18n_domain). "</th>";
		  if($showActions) {
                    $html .= "<th>" .__("Action", $i18n_domain). "</th>";
		  }
                  $html .= "</tr>";
                  foreach($_SESSION['artistography_cart'] as $product => $qty) {
			$html .= self::processCartItem($product, $qty, $total, $showActions);
                  }
                $html .= "<tr>
                                <td colspan='2'>Total:</td><td>" .CURRENCY.number_format(round($total, 2), 2). "</td>";
		if($showActions) { $html .= "<td></td>"; }
                $html .= "</tr>
                             </table></center><br/>\n
                                <a href='" .get_permalink(get_option('wp_artistography_checkout_page')). "'><img src='$checkout_icon_url' width='$checkout_icon_width' height='$checkout_icon_height' style='border:0px;' align='right' /></a>";
                } else {
                  $html .= "<font style='font-size:14pt'>The shopping cart is empty.</font>";
                }
		return $html;
	}

	public static function callback_Get_Cart_Contents() {
                echo self::getCartContents();
		die;
	}

	public static function callback_Get_Basic_Cart_Contents() {
		echo self::getCartContents(false);
		die;
	}

	public static function showInvoice() {
		GLOBAL $i18n_domain;

                $total = (float)"0.00";

                 /*** Display Cart Contents ***/
                if(!empty($_SESSION['artistography_cart'])) {
		  $html = "<center><table>
                             <tr>
                                <th>" .__("Quanity", $i18n_domain). "</th>
                                <th>" .__("Item", $i18n_domain). "</th>
                                <th>" .__("Price", $i18n_domain). "</th>
                             </tr>";
                  foreach($_SESSION['artistography_cart'] as $product => $qty) {
			$html .= self::processCartItem($product, $qty, $total, false);
                  }
                $html .= "<tr>
                                <td colspan='2'>Total:</td><td>" .CURRENCY.number_format(round($total, 2), 2). "</td>
                             </tr>
                             </table></center><br/>";
                } else {
		  return false;
                }
                return $html;
	}
} /* end class Sales */
?>
