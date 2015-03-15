<?php

add_action( 'widgets_init', function(){
     register_widget( 'Artistography_Products_Widget' );
});

class Artistography_Products_Widget extends WP_Widget {

	/**
	 * Sets up the widgets name etc
	 */
	public function __construct() {
		GLOBAL $i18n_domain;
		// widget actual processes
		parent::__construct(
			'artistography_products_widget', // Base ID
			__( 'Artistography Products', $i18n_domain ), // Name
			array( 'description' => __( 'Displays the products entered into Artistography', $i18n_domain ), ) // Args
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

		extract( $args );

		$albums = new Music;

		// outputs the content of the widget
		echo $args['before_widget'];
		if ( ! empty( $instance['title'] ) ) {
			echo $args['before_title'] . apply_filters( 'widget_title', $instance['title'] ). $args['after_title'];
		}
		if ( ! empty( $instance['number'] ) ) {
			$number = $instance['number'];
		} else {
			$number = 2;
		}

		 // load albums and shuffle them
		if ( $instance['free'] == '' ) {
			$albums->loadAllNotFree();
		} else {
			$albums->loadAllLinked();
		}

		for($i = 0; $i < $albums->getTotalRows(); $i++) {
			$albums_preload[] = $albums->id;
			$albums->getNodeNext();
		}

		if( $albums->getTotalRows() > 0) {
			shuffle($albums_preload);
		} else {
			echo "Nothing to show yet.";
		}

		for($i = 0; $i < $number and $i < $albums->getTotalRows(); $i++) {
			echo do_shortcode("[artistography_display_album id=" .$albums_preload[$i]. "]");
		}
		
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
		$title = ! empty( $instance['title'] ) ? $instance['title'] : __( 'Products', $i18n_domain );
		$number = ! empty( $instance['number'] ) ? $instance['number'] : '2';
		$free = ! empty( $instance['free'] ) ? $instance['free'] : '';

		?>
		<p>
		<label for="<?php echo $this->get_field_id( 'title' ); ?>"><?php _e( 'Title:' ); ?></label> 
		<input class="widefat" id="<?php echo $this->get_field_id( 'title' ); ?>" name="<?php echo $this->get_field_name( 'title' ); ?>" type="text" value="<?php echo esc_attr( $title ); ?>">
		</p>
		<p>
                <label for="<?php echo $this->get_field_id( 'number' ); ?>"><?php _e( 'Number of products to show:' ); ?></label>
                <input id="<?php echo $this->get_field_id( 'number' ); ?>" name="<?php echo $this->get_field_name( 'number' ); ?>" type="text" value="<?php echo esc_attr( $number ); ?>">
                </p>
                <p>
                <label for="<?php echo $this->get_field_id( 'free' ); ?>"><?php _e( 'Display free downloads:' ); ?></label>
                <input id="<?php echo $this->get_field_id( 'free' ); ?>" name="<?php echo $this->get_field_name( 'free' ); ?>" type="checkbox" <?php echo esc_attr( $free ) == 'on' ? 'checked="checked"' : ''; ?>>
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
		$instance['number'] = ( ! empty( $new_instance['number'] ) ) ? strip_tags( $new_instance['number'] ) : '';
		$instance['free'] = ( ! empty( $new_instance['free'] ) ) ? strip_tags( $new_instance['free'] ) : '';

		return $instance;
	}
}
?>
