<?php

add_action( 'widgets_init', function(){
     register_widget( 'Artistography_Cart_Widget' );
});

 // AJAX
add_action('wp_ajax_Get_Cart_URL', 'Artistography_Cart_Widget::callback_Get_Cart_URL');
add_action('wp_ajax_nopriv_Get_Cart_URL', 'Artistography_Cart_Widget::callback_Get_Cart_URL');

class Artistography_Cart_Widget extends WP_Widget {

	public static function callback_Get_Cart_URL() {
		echo get_permalink(get_option('wp_artistography_cart_page'));
		die;
	}

	/**
	 * Sets up the widgets name etc
	 */
	public function __construct() {
		GLOBAL $i18n_domain;
		// widget actual processes
		parent::__construct(
			'artistography_cart_widget', // Base ID
			__( 'Artistography Shopping Cart', $i18n_domain ), // Name
			array( 'description' => __( 'Displays the contents of the shopping cart for Artistography', $i18n_domain ), ) // Args
		);
	}

	/**
	 * Outputs the content of the widget
	 *
	 * @param array $args
	 * @param array $instance
	 */
	public function widget( $args, $instance ) {
		GLOBAL $i18n_domain;

		$orders = new Orders;

		// outputs the content of the widget
		echo $args['before_widget'];
		if ( ! empty( $instance['title'] ) ) {
			echo $args['before_title'] . apply_filters( 'widget_title', $instance['title'] ). $args['after_title'];
		}

		echo $orders->showBasicCart();

		echo "<button class='edit_cart'>Edit Cart</button>";

		echo $args['after_widget'];
	}

	/**
	 * Outputs the options form on admin
	 *
	 * @param array $instance The widget options
	 */
	public function form( $instance ) {
		GLOBAL $i18n_domain;
		// outputs the options form on admin
		$title = ! empty( $instance['title'] ) ? $instance['title'] : __( 'Shopping Cart', $i18n_domain );
		?>
		<p>
		<label for="<?php echo $this->get_field_id( 'title' ); ?>"><?php _e( 'Title:' ); ?></label> 
		<input class="widefat" id="<?php echo $this->get_field_id( 'title' ); ?>" name="<?php echo $this->get_field_name( 'title' ); ?>" type="text" value="<?php echo esc_attr( $title ); ?>">
		</p>
		<?php 
	}

	/**
	 * Processing widget options on save
	 *
	 * @param array $new_instance The new options
	 * @param array $old_instance The previous options
	 */
	public function update( $new_instance, $old_instance ) {
		// processes widget options to be saved
		$instance = array();
		$instance['title'] = ( ! empty( $new_instance['title'] ) ) ? strip_tags( $new_instance['title'] ) : '';

		return $instance;
	}
}
?>
